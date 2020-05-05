#include "LibXenoverse.h"
#include "EMO_BaseFile.h"


bool isNumber(const string& val)
{
	std::basic_stringstream<char, std::char_traits<char>, std::allocator<char> > str(val);
	float tst;
	str >> tst;
	return !str.fail() && str.eof();
}

std::string FloatToString(float value)
{
	char temp[32];
	std::string str;

	sprintf_s(temp, "%.9g", value);
	str = temp;

	if (str.find('.') == std::string::npos && str.find('e') == std::string::npos)
		str = str + ".0";

	return str;
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}


void replaceAll_component_mad(std::string& str, const std::string& searchComponent, double factor, double offset)
{
	if (searchComponent.empty())
		return;


	size_t start_pos = 0;
	size_t end_pos = 0;
	size_t start_number = 0;
	while ((start_pos = str.find(searchComponent, start_pos)) != std::string::npos)
	{
		start_number = start_pos + searchComponent.length();
		if ((end_pos = str.find("\"", start_number)) == std::string::npos)
		{
			str.replace(start_pos, searchComponent.length(), "");
			return;
		}

		string number_str = str.substr(start_number, (end_pos - start_number));
		if (!isNumber(number_str))
		{
			printf("'%s' is not a number. skipped\n", number_str);

			str.replace(start_pos, searchComponent.length(), "");
			continue;
		}

		double value = std::stod(number_str);
		double newValue = offset + value * factor;
		string newValue_str = FloatToString((float)newValue);

		str.replace(start_pos, end_pos - start_pos, newValue_str);
		start_pos += newValue_str.length();
	}
}



int main(int argc, char** argv)
{
	printf("*******************************************************************\n\
 This tool is for extract some 3d data of Havok file into fbx mesh for edition into Blender or 3dsmax, and rematch to update hkx in reverse.\n\
 Usage: 'havokfbx.exe [options] file.hkx' or 'havokfbx.exe [options] file.fbx file.hkx'\n\
 Options : '-NoWait', '-AlwaysWait', '-WaitOnError' (default), or '-WaitOnWarning'. '-epsilon'\n\
 Notice: the Extract concern only the types : hkAabb, hkGeometry, and hkcdSimdTree::Node. \n\
 hknpConvexPolytopeShape, hknpCompressedMeshShape, hknpConvexPolytopeShape and hknpTriangleShape should be add later.\n\
 Notice: by doing a shortcut, you could use another option and keep drag and drop of files.\n\
 Notice: \"path With Spaces\" allowed now. \n\
*******************************************************************\n");

	std::vector<string> arguments = LibXenoverse::initApplication(argc, argv);

	if (arguments.size() == 0)
	{
		printf("Error not enougth arguments.\n");
		LibXenoverse::notifyError();
		LibXenoverse::waitOnEnd();
		return 1;
	}

	double epsilon = 0.00001;

	size_t nbArg = arguments.size();
	for (size_t i = 0; i < nbArg; i++)
	{
		if ((arguments.at(i) == "-epsilon") && (i + 1<nbArg) && (isNumber(arguments.at(i + 1))))
		{
			epsilon = std::stof(arguments.at(i + 1));

			arguments.erase(arguments.begin() + i);
			arguments.erase(arguments.begin() + i);
			break;
		}
	}

	if (arguments.size() == 0)
	{
		printf("Error not enougth arguments.\n");
		LibXenoverse::notifyError();
		LibXenoverse::waitOnEnd();
		return 1;
	}

	nbArg = arguments.size();
	for (size_t i = 0; i <nbArg; i++)										//we need to have hkx first.
	{
		if (LibXenoverse::extensionFromFilename(arguments.at(i), true) == "hkx")
		{
			arguments.insert(arguments.begin(), arguments.at(i));
			arguments.erase(arguments.begin() + i + 1);
			break;
		}
	}


	string filename = arguments.at(0);
	string extension = LibXenoverse::extensionFromFilename(filename, true);
	string basefilename = filename.substr(0, filename.length() - (extension.size() + 1));
	

	string filename2 = "";
	string extension2 = "";
	string basefilename2 = "";
	if (arguments.size() > 1)
	{
		filename2 = arguments.at(1);
		extension2 = LibXenoverse::extensionFromFilename(filename2, true);
		basefilename2 = filename2.substr(0, filename2.length() - (extension2.size() + 1));
	}




	if ((extension == "hkx") && (extension2 == ""))
	{
		LibXenoverse::Havok* havok = new LibXenoverse::Havok();
		if (!havok->load(filename))
		{
			printf("Error on loading havok file %s", filename.c_str());
			LibXenoverse::notifyError();
			LibXenoverse::waitOnEnd();
			return 0;
		}

		//havok->SaveToFile(basefilename +"_resave.hkx");		//test Todo remove


		
		FbxManager *sdk_manager = FbxManager::Create();								// Create FBX Manager
		FbxIOSettings *ios = FbxIOSettings::Create(sdk_manager, IOSROOT);
		ios->SetBoolProp(EXP_FBX_EMBEDDED, true);
		sdk_manager->SetIOSettings(ios);

		FbxScene *scene = FbxScene::Create(sdk_manager, "HavokFBXScene");			// Create Scene
		scene->GetGlobalSettings().SetCustomFrameRate(60.0);				//specify the frameRate, the number of image by second. (not working well in blender fbx importer. so, I had to modify default value into blender fbx importer).
																			
		FbxTime::EProtocol protocol = FbxTime::GetGlobalTimeProtocol();
		FbxTime::SetGlobalTimeProtocol(FbxTime::EProtocol::eDefaultProtocol);
		protocol = FbxTime::GetGlobalTimeProtocol();

		scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::eOpenGL);	//eOpenGL,			!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded
		scene->GetGlobalSettings().SetSystemUnit(FbxSystemUnit::m);

		FbxNode *lRootNode = scene->GetRootNode();


		printf("Build FBX geometrie\n");
		havok->exportFBX(scene);


		printf("Export FBX\n");
		{
			int lFileFormat = sdk_manager->GetIOPluginRegistry()->GetNativeWriterFormat();
			FbxExporter* lExporter = FbxExporter::Create(sdk_manager, "");
			bool lExportStatus = lExporter->Initialize((basefilename + ".fbx").c_str(), lFileFormat, sdk_manager->GetIOSettings());
			if (!lExportStatus)
			{
				printf("Call to FbxExporter::Initialize() failed.\n");
				printf("Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
				LibXenoverse::notifyError();
				LibXenoverse::waitOnEnd();
				return 1;
			}
			lExporter->Export(scene);												// Export scene
			lExporter->Destroy();
		}

		delete havok;

		printf("finished.\n");
		LibXenoverse::waitOnEnd();
		return 0;





	}else if ((extension == "hkx") && (extension2 == "fbx")) {


		LibXenoverse::Havok *havok = new LibXenoverse::Havok();
		if (!havok->load(filename))
		{
			printf("Error on loading havok file %s", filename.c_str());
			LibXenoverse::notifyError();
			LibXenoverse::waitOnEnd();
			return 0;
		}


		printf("-------------- Load Fbx. Please wait ...\n");



		// Create FBX
		FbxManager *sdk_manager = FbxManager::Create();
		FbxIOSettings *ios = FbxIOSettings::Create(sdk_manager, IOSROOT);
		ios->SetBoolProp(EXP_FBX_EMBEDDED, true);
		sdk_manager->SetIOSettings(ios);

		// Import FBX
		FbxImporter* lImporter = FbxImporter::Create(sdk_manager, "");
		bool lImportStatus = lImporter->Initialize(filename2.c_str(), -1, sdk_manager->GetIOSettings());
		if (!lImportStatus)
		{
			// Handle error
			printf("Call to FbxImporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
			LibXenoverse::notifyError();
			LibXenoverse::waitOnEnd();
			return NULL;
		}
		FbxScene* lScene = FbxScene::Create(sdk_manager, "FBXHavokImport");
		lImporter->Import(lScene);
		int lFileMajor, lFileMinor, lFileRevision;
		lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);
		lImporter->Destroy();

		FbxAxisSystem fbxAxisSys = lScene->GetGlobalSettings().GetAxisSystem();
		int dir = 1;

		if (fbxAxisSys == FbxAxisSystem::eMayaZUp)			/*!< UpVector = ZAxis, FrontVector = -ParityOdd, CoordSystem = RightHanded */
			printf("FbxAxisSystem::eMayaZUp\n");
		else if (fbxAxisSys == FbxAxisSystem::eMayaYUp)		/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded */
			printf("FbxAxisSystem::eMayaYUp\n");
		else if (fbxAxisSys == FbxAxisSystem::eMax)			/*!< UpVector = ZAxis, FrontVector = -ParityOdd, CoordSystem = RightHanded */
			printf("FbxAxisSystem::eMax\n");
		else if (fbxAxisSys == FbxAxisSystem::eMotionBuilder)	/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded */
			printf("FbxAxisSystem::eMotionBuilder\n");
		else if (fbxAxisSys == FbxAxisSystem::eOpenGL)		/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded */
			printf("FbxAxisSystem::eOpenGL\n");
		else if (fbxAxisSys == FbxAxisSystem::eDirectX)		/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = LeftHanded */
			printf("FbxAxisSystem::eDirectX\n");
		else if (fbxAxisSys == FbxAxisSystem::eLightwave)	/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = LeftHanded */
			printf("FbxAxisSystem::eLightwave\n");
		else
			printf("Unknow AxisSystem : up: %i, forward : %i, coordsyst: %s\n", fbxAxisSys.GetUpVector(dir), fbxAxisSys.GetFrontVector(dir), ((fbxAxisSys.GetCoorSystem() == FbxAxisSystem::eRightHanded) ? "Right" : "Left"));

		if (fbxAxisSys != FbxAxisSystem::eOpenGL)
		{
			printf("try to convert fbxAxisSys in eOpenGL (UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded) \n");
			FbxAxisSystem::OpenGL.ConvertScene(lScene);
		}



		FbxSystemUnit fbxSysUnit = lScene->GetGlobalSettings().GetSystemUnit();
		string systemUnit = "cm";
		if (fbxSysUnit == FbxSystemUnit::m)
			systemUnit = "m";
		else if (fbxSysUnit == FbxSystemUnit::dm)
			systemUnit = "dm";
		else if (fbxSysUnit == FbxSystemUnit::mm)
			systemUnit = "mm";
		else if (fbxSysUnit == FbxSystemUnit::km)
			systemUnit = "km";
		else if (fbxSysUnit == FbxSystemUnit::Inch)
			systemUnit = "Inch";
		else if (fbxSysUnit == FbxSystemUnit::Foot)
			systemUnit = "Foot";
		else if (fbxSysUnit == FbxSystemUnit::Mile)
			systemUnit = "Mile";
		else if (fbxSysUnit == FbxSystemUnit::Yard)
			systemUnit = "Yard";

		printf("systemUnit is : %s\n", systemUnit.c_str());

		if (fbxSysUnit != FbxSystemUnit::m)
		{
			printf("try to convert systemUnit in meters\n");
			FbxSystemUnit::m.ConvertScene(lScene);
		}



		FbxNode* fbxRootNode = lScene->GetRootNode();




		printf("-------------- Converting FbxMeshes to Havok\n");

		


		// Search Scene
		FbxStatus lStatus;
		FbxNode* fbxNode;
		const int lNodeCount = lScene->GetSrcObjectCount<FbxNode>();
		std::vector<FbxNode*> listNodeAllReadyCreated;
		for (int lIndex = 0; lIndex < lNodeCount; lIndex++)
		{
			fbxNode = lScene->GetSrcObject<FbxNode>(lIndex);
			if (!fbxNode)
				continue;
			//printf("Node found: %s\n", fbxNode->GetName());

			FbxMesh* fbxMesh = fbxNode->GetMesh();
			if (!fbxMesh)
				continue;
			printf("Mesh found: %s\n", fbxNode->GetName());



			//the new way need to have the first parent of meshNode. so, by iterate meshnode, you could have the same first parent many time. so we have to skip some of.
			size_t nbEmdNode = listNodeAllReadyCreated.size();
			bool isfound = false;
			for (size_t i = 0; i < nbEmdNode; i++)
			{
				if (listNodeAllReadyCreated.at(i) == fbxNode)
				{
					isfound = true;
					break;
				}
			}
			if (isfound)							//allready done, skip.
				continue;

			listNodeAllReadyCreated.push_back(fbxNode);

			havok->importFBX(fbxNode);				//by the name could update values on a Object hierarchy.
		}


		havok->SaveToFile(basefilename2 +".hkx");
		delete havok;

		printf("-------------- Destroying Fbx instance\n");
		lScene->Destroy();






















	}else if ((extension == "fbx") && (extension2 == "")) {					//new version with search for "box" to make cubes collisions, witha series of havok cubes files.



		char buf_tmp[MAX_PATH + 1];
		GetModuleFileName(NULL, buf_tmp, MAX_PATH);
		string exeFilename = string(buf_tmp);
		string exePath = LibXenoverse::folderFromFilename(exeFilename);




		string name = LibXenoverse::nameFromFilename(filename);
		name = name.substr(0, name.length() - (extension.size() + 1));

		//we have to load the texte file , template for box.
		char *buf;
		size_t size;
		buf = (char*)LibXenoverse::EMO_BaseFile::ReadFile(exePath +"/resources/box_template.hkx.xml", &size, true);
		if (!buf)
		{
			printf("Error loading file : resources/box_template.hkx.xml");
			LibXenoverse::notifyError();
			return -1;
		}
		string xmlBoxTemplate = string(buf);
		delete[] buf;


		//we have to load the texte file , template for Cylinder with axe on Oy.
		buf = (char*)LibXenoverse::EMO_BaseFile::ReadFile(exePath + "/resources/cylinderY_template.hkx.xml", &size, true);
		if (!buf)
		{
			printf("Error loading file : resources/cylinderY_template.hkx.xml");
			LibXenoverse::notifyError();
			return -1;
		}
		string xmlCylinderTemplate = string(buf);
		delete[] buf;

		//we have to load the texte file , template for Cylinder with axe on Ox.
		buf = (char*)LibXenoverse::EMO_BaseFile::ReadFile(exePath + "/resources/cylinderX_template.hkx.xml", &size, true);
		if (!buf)
		{
			printf("Error loading file : resources/cylinderX_template.hkx.xml");
			LibXenoverse::notifyError();
			return -1;
		}
		string xmlCylinderXTemplate = string(buf);
		delete[] buf;

		//we have to load the texte file , template for Cylinder with axe on Oz.
		buf = (char*)LibXenoverse::EMO_BaseFile::ReadFile(exePath + "/resources/cylinderZ_template.hkx.xml", &size, true);
		if (!buf)
		{
			printf("Error loading file : resources/cylinderZ_template.hkx.xml");
			LibXenoverse::notifyError();
			return -1;
		}
		string xmlCylinderZTemplate = string(buf);
		delete[] buf;

		//we have to load the texte file , template for Sphere.
		buf = (char*)LibXenoverse::EMO_BaseFile::ReadFile(exePath + "/resources/sphere_template.hkx.xml", &size, true);
		if (!buf)
		{
			printf("Error loading file : resources/sphere_template.hkx.xml");
			LibXenoverse::notifyError();
			return -1;
		}
		string xmlSphereTemplate = string(buf);
		delete[] buf;


		
		





		string listHitboxXmlCouple_str = "";
		string listHitboxXml_str = "";

		printf("-------------- Load Fbx. Please wait ...\n");

		// Create FBX
		FbxManager *sdk_manager = FbxManager::Create();
		FbxIOSettings *ios = FbxIOSettings::Create(sdk_manager, IOSROOT);
		ios->SetBoolProp(EXP_FBX_EMBEDDED, true);
		sdk_manager->SetIOSettings(ios);

		// Import FBX
		FbxImporter* lImporter = FbxImporter::Create(sdk_manager, "");
		bool lImportStatus = lImporter->Initialize(filename.c_str(), -1, sdk_manager->GetIOSettings());
		if (!lImportStatus)
		{
			// Handle error
			printf("Call to FbxImporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
			LibXenoverse::notifyError();
			LibXenoverse::waitOnEnd();
			return NULL;
		}
		FbxScene* lScene = FbxScene::Create(sdk_manager, "FBXHavokImport");
		lImporter->Import(lScene);
		int lFileMajor, lFileMinor, lFileRevision;
		lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);
		lImporter->Destroy();

		FbxAxisSystem fbxAxisSys = lScene->GetGlobalSettings().GetAxisSystem();
		int dir = 1;

		if (fbxAxisSys == FbxAxisSystem::eMayaZUp)			/*!< UpVector = ZAxis, FrontVector = -ParityOdd, CoordSystem = RightHanded */
			printf("FbxAxisSystem::eMayaZUp\n");
		else if (fbxAxisSys == FbxAxisSystem::eMayaYUp)		/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded */
			printf("FbxAxisSystem::eMayaYUp\n");
		else if (fbxAxisSys == FbxAxisSystem::eMax)			/*!< UpVector = ZAxis, FrontVector = -ParityOdd, CoordSystem = RightHanded */
			printf("FbxAxisSystem::eMax\n");
		else if (fbxAxisSys == FbxAxisSystem::eMotionBuilder)	/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded */
			printf("FbxAxisSystem::eMotionBuilder\n");
		else if (fbxAxisSys == FbxAxisSystem::eOpenGL)		/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded */
			printf("FbxAxisSystem::eOpenGL\n");
		else if (fbxAxisSys == FbxAxisSystem::eDirectX)		/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = LeftHanded */
			printf("FbxAxisSystem::eDirectX\n");
		else if (fbxAxisSys == FbxAxisSystem::eLightwave)	/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = LeftHanded */
			printf("FbxAxisSystem::eLightwave\n");
		else
			printf("Unknow AxisSystem : up: %i, forward : %i, coordsyst: %s\n", fbxAxisSys.GetUpVector(dir), fbxAxisSys.GetFrontVector(dir), ((fbxAxisSys.GetCoorSystem() == FbxAxisSystem::eRightHanded) ? "Right" : "Left"));

		if (fbxAxisSys != FbxAxisSystem::eOpenGL)
		{
			printf("try to convert fbxAxisSys in eOpenGL (UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded) \n");
			FbxAxisSystem::OpenGL.ConvertScene(lScene);
		}



		FbxSystemUnit fbxSysUnit = lScene->GetGlobalSettings().GetSystemUnit();
		string systemUnit = "cm";
		if (fbxSysUnit == FbxSystemUnit::m)
			systemUnit = "m";
		else if (fbxSysUnit == FbxSystemUnit::dm)
			systemUnit = "dm";
		else if (fbxSysUnit == FbxSystemUnit::mm)
			systemUnit = "mm";
		else if (fbxSysUnit == FbxSystemUnit::km)
			systemUnit = "km";
		else if (fbxSysUnit == FbxSystemUnit::Inch)
			systemUnit = "Inch";
		else if (fbxSysUnit == FbxSystemUnit::Foot)
			systemUnit = "Foot";
		else if (fbxSysUnit == FbxSystemUnit::Mile)
			systemUnit = "Mile";
		else if (fbxSysUnit == FbxSystemUnit::Yard)
			systemUnit = "Yard";

		printf("systemUnit is : %s\n", systemUnit.c_str());

		if (fbxSysUnit != FbxSystemUnit::m)
		{
			printf("try to convert systemUnit in meters\n");
			FbxSystemUnit::m.ConvertScene(lScene);
		}



		FbxNode* fbxRootNode = lScene->GetRootNode();




		printf("-------------- Converting FbxMeshes to Havok\n");




		// Search Scene
		FbxStatus lStatus;
		FbxNode* fbxNode;
		const int lNodeCount = lScene->GetSrcObjectCount<FbxNode>();
		std::vector<FbxNode*> listNodeAllReadyCreated;
		size_t inc = 1;
		for (int lIndex = 0; lIndex < lNodeCount; lIndex++)
		{
			fbxNode = lScene->GetSrcObject<FbxNode>(lIndex);
			if (!fbxNode)
				continue;
			//printf("Node found: %s\n", fbxNode->GetName());

			FbxMesh* fbxMesh = fbxNode->GetMesh();
			if (!fbxMesh)
				continue;

			bool isBox = ((string(fbxNode->GetName()).length() >= 3) && (string(fbxNode->GetName()).substr(0, 3) == "Box"));
			bool isCylinder = ((string(fbxNode->GetName()).length() >= 8) && (string(fbxNode->GetName()).substr(0, 8) == "Cylinder"));
			bool isSphere = ((string(fbxNode->GetName()).length() >= 6) && (string(fbxNode->GetName()).substr(0, 6) == "Sphere"));

			if (isBox)
			{
				printf("Box Mesh found: %s\n", fbxNode->GetName());
				
				double eps = epsilon;

				double Xmin = -1.0;
				double Xmax = 1.0;
				double Ymin = 1.5;
				double Ymax = 2.5;
				double Zmin = -1.0;
				double Zmax = 1.0;


				//search limits for the box definition.
				{
					FbxVector4 nodeScale = fbxNode->LclScaling.Get();
					if (nodeScale[0] == 0.0) nodeScale[0] = 1.0f;
					if (nodeScale[1] == 0.0) nodeScale[1] = 1.0f;
					if (nodeScale[2] == 0.0) nodeScale[2] = 1.0f;

					FbxAMatrix transform_matrix = fbxNode->EvaluateGlobalTransform();
					FbxAMatrix rotation_matrix = transform_matrix;
					rotation_matrix.SetT(FbxVector4(0.0, 0.0, 0.0, 0.0));
					rotation_matrix.SetS(FbxVector4(1.0, 1.0, 1.0, 1.0));


					FbxMesh* fbxMesh = fbxNode->GetMesh();
					FbxVector4* listPositions = fbxMesh->GetControlPoints();
					
					LibXenoverse::AABB vertex_aabb;
					FbxVector4 position;
					
					size_t nbVertex = fbxMesh->GetControlPointsCount();
					for (size_t i = 0; i < nbVertex; i++)
					{
						position = transform_matrix.MultT(listPositions[i]);
						//vertex_aabb.addPoint((float)(position[0] / nodeScale[0]), (float)(position[1] / nodeScale[1]), (float)(position[2] / nodeScale[2]));
						vertex_aabb.addPoint((float)(position[0]), (float)(position[1]), (float)(position[2]));
					}

					Xmin = vertex_aabb.start_x;
					Ymin = vertex_aabb.start_y;
					Zmin = vertex_aabb.start_z;
					Xmax = vertex_aabb.end_x;
					Ymax = vertex_aabb.end_y;
					Zmax = vertex_aabb.end_z;
				}


				double Xmin_sub_eps = Xmin - eps;
				double Ymin_sub_eps = Ymin - eps;
				double Zmin_sub_eps = Zmin - eps;
				double Xmax_add_eps = Xmax + eps;
				double Ymax_add_eps = Ymax + eps;
				double Zmax_add_eps = Zmax + eps;

				
				

				
				string xmlBoxTemplate_tmp = xmlBoxTemplate;
				replaceAll(xmlBoxTemplate_tmp, "template_Xmin_sub_eps", FloatToString((float)Xmin_sub_eps));		//because of names to replace, keep longer first.
				replaceAll(xmlBoxTemplate_tmp, "template_Ymin_sub_eps", FloatToString((float)Ymin_sub_eps));
				replaceAll(xmlBoxTemplate_tmp, "template_Zmin_sub_eps", FloatToString((float)Zmin_sub_eps));
				replaceAll(xmlBoxTemplate_tmp, "template_Xmax_add_eps", FloatToString((float)Xmax_add_eps));
				replaceAll(xmlBoxTemplate_tmp, "template_Ymax_add_eps", FloatToString((float)Ymax_add_eps));
				replaceAll(xmlBoxTemplate_tmp, "template_Zmax_add_eps", FloatToString((float)Zmax_add_eps));
				replaceAll(xmlBoxTemplate_tmp, "template_Xmin", FloatToString((float)Xmin));
				replaceAll(xmlBoxTemplate_tmp, "template_Ymin", FloatToString((float)Ymin));
				replaceAll(xmlBoxTemplate_tmp, "template_Zmin", FloatToString((float)Zmin));
				replaceAll(xmlBoxTemplate_tmp, "template_Xmax", FloatToString((float)Xmax));
				replaceAll(xmlBoxTemplate_tmp, "template_Ymax", FloatToString((float)Ymax));
				replaceAll(xmlBoxTemplate_tmp, "template_Zmax", FloatToString((float)Zmax));
				


				string newName = name + "_" + string(fbxNode->GetName()) + ".hkx";
				string newfilename = basefilename + "_" + string(fbxNode->GetName()) + ".hkx";
				

				listHitboxXml_str += "\n\
					<Destruction>\n\
						<unk_0 u32=\"0x3\" />\n\
						<unk_1 u32=\"0xc0\" />\n\
						<unk_2 u32=\"0x0\" />\n\
						<unk_5 u32=\"0x0\" />\n\
						<unk_6 u32=\"0x40\" />\n\
						<unk_7 float=\"0.00999999978\" />\n\
						<DestructionSubPart />\n\
						<HavokFile filename=\"" + newName + "\" />\n\
					</Destruction>\n\
				";

				listHitboxXmlCouple_str += "<IndexCouple Index0 = \"" + std::to_string(inc) + "\" Index1 = \"4\" / >\n";
				inc++;

				printf("-------------- Make Box Havok Xml \n");
				TiXmlDocument doc;
				if (!doc.Parse(xmlBoxTemplate_tmp.c_str()))
				{
					printf("Error parsing file \"%s\". This is what tinyxml has to say: %s. Row=%d, col=%d.\n", filename.c_str(), doc.ErrorDesc(), doc.ErrorRow(), doc.ErrorCol());
					LibXenoverse::notifyError();
					continue;
				}

				printf("processing...(please wait)\n");
				


				printf("-------------- Make Havok Box\n");

				LibXenoverse::Havok *havok = new LibXenoverse::Havok();
				if (!havok->Compile(&doc))
				{
					printf("Error on parsing havok");
					LibXenoverse::notifyError();
					LibXenoverse::waitOnEnd();
				}

				havok->importFBX(fbxNode);				//by the name could update values on a Object hierarchy.
				havok->SaveToFile(newfilename);
				delete havok;





























			}else if ((isCylinder)||(isSphere)) {
				 
				printf("Cylinder/Sphere Mesh found: %s\n", fbxNode->GetName());

				double eps = epsilon;

				double Xmin = -1.0;
				double Xmax = 1.0;
				double Ymin = 1.5;
				double Ymax = 2.5;
				double Zmin = -1.0;
				double Zmax = 1.0;


				//search limits for the box definition.
				bool isCylinderX = false;
				bool isCylinderZ = false;
				{
					FbxVector4 nodeScale = fbxNode->LclScaling.Get();
					if (nodeScale[0] == 0.0) nodeScale[0] = 1.0f;
					if (nodeScale[1] == 0.0) nodeScale[1] = 1.0f;
					if (nodeScale[2] == 0.0) nodeScale[2] = 1.0f;

					FbxAMatrix transform_matrix = fbxNode->EvaluateGlobalTransform();
					FbxAMatrix rotation_matrix = transform_matrix;
					rotation_matrix.SetT(FbxVector4(0.0, 0.0, 0.0, 0.0));
					rotation_matrix.SetS(FbxVector4(1.0, 1.0, 1.0, 1.0));


					FbxMesh* fbxMesh = fbxNode->GetMesh();
					FbxVector4* listPositions = fbxMesh->GetControlPoints();

					LibXenoverse::AABB vertex_aabb;
					FbxVector4 position;

					size_t nbVertex = fbxMesh->GetControlPointsCount();
					for (size_t i = 0; i < nbVertex; i++)
					{
						position = transform_matrix.MultT(listPositions[i]);
						//vertex_aabb.addPoint((float)(position[0] / nodeScale[0]), (float)(position[1] / nodeScale[1]), (float)(position[2] / nodeScale[2]));
						vertex_aabb.addPoint((float)(position[0]), (float)(position[1]), (float)(position[2]));
					}

					Xmin = vertex_aabb.start_x;
					Ymin = vertex_aabb.start_y;
					Zmin = vertex_aabb.start_z;
					Xmax = vertex_aabb.end_x;
					Ymax = vertex_aabb.end_y;
					Zmax = vertex_aabb.end_z;


					if (isCylinder)
					{
						FbxVector4 axisX = rotation_matrix.MultT(FbxVector4(1, 0, 0));
						FbxVector4 axisY = rotation_matrix.MultT(FbxVector4(0, 1, 0));
						FbxVector4 axisZ = rotation_matrix.MultT(FbxVector4(0, 0, 1));

						//CylinderX: axisX(0,0,1), axisY(0,1,0),  axisZ(-1,0,0)
						//CylinderY: axisX(1,0,0), axisY(0,0,-1), axisZ(0,1,0)
						//CylinderZ: axisX(1,0,0), axisY(0,1,0),  axisZ(0,0,1)

						isCylinderX = (abs(axisZ[0]) > abs(axisZ[1])) && (abs(axisZ[0]) > abs(axisZ[2]));
						isCylinderZ = (abs(axisZ[2]) > abs(axisZ[0])) && (abs(axisZ[2]) > abs(axisZ[1]));
					}

				}


				double Xcenter = (Xmin + Xmax) / 2.0;
				double XradiusScaled = (Xmax - Xmin) / 2.0;
				double Ycenter = (Ymin + Ymax) / 2.0;
				double YradiusScaled = (Ymax - Ymin) / 2.0;
				double Zcenter = (Zmin + Zmax) / 2.0;
				double ZradiusScaled = (Zmax - Zmin) / 2.0;
				

				string xmlCylinderTemplate_tmp = "";
				if (isSphere)
				{
					xmlCylinderTemplate_tmp = xmlSphereTemplate;

					replaceAll_component_mad(xmlCylinderTemplate_tmp, "X_Scale_", XradiusScaled, Xcenter);
					replaceAll_component_mad(xmlCylinderTemplate_tmp, "Y_Scale_", YradiusScaled, Ycenter);
					replaceAll_component_mad(xmlCylinderTemplate_tmp, "Z_Scale_", ZradiusScaled, Zcenter);

				}else if (isCylinder){

					xmlCylinderTemplate_tmp = (isCylinderX) ? xmlCylinderXTemplate : ((isCylinderZ) ? xmlCylinderZTemplate : xmlCylinderTemplate);

					if (isCylinderZ)
					{
						double ZScale = (Zmax - Zmin);
						replaceAll_component_mad(xmlCylinderTemplate_tmp, "X_Scale_", XradiusScaled, Xcenter);
						replaceAll_component_mad(xmlCylinderTemplate_tmp, "Y_Scale_", YradiusScaled, Ycenter);
						replaceAll_component_mad(xmlCylinderTemplate_tmp, "Z_Scale_", ZScale, Zmin);
						
					}else if (isCylinderX) {

						double XScale = (Xmax - Xmin);
						replaceAll_component_mad(xmlCylinderTemplate_tmp, "X_Scale_", XScale, Xmin);
						replaceAll_component_mad(xmlCylinderTemplate_tmp, "Y_Scale_", YradiusScaled, Ycenter);
						replaceAll_component_mad(xmlCylinderTemplate_tmp, "Z_Scale_", ZradiusScaled, Zcenter);

					}else {
						double YScale = (Ymax - Ymin);
						replaceAll_component_mad(xmlCylinderTemplate_tmp, "X_Scale_", XradiusScaled, Xcenter);
						replaceAll_component_mad(xmlCylinderTemplate_tmp, "Y_Scale_", YScale, Ymin);
						replaceAll_component_mad(xmlCylinderTemplate_tmp, "Z_Scale_", ZradiusScaled, Zcenter);
					}
				}

				 


				

				
				
					





				string newName = name + "_" + string(fbxNode->GetName()) + ".hkx";
				string newfilename = basefilename + "_" + string(fbxNode->GetName()) + ".hkx";

				/*
				listHitboxXml_str += "\n\
					<Destruction>\n\
						<unk_0 u32=\"0x3\" />\n\
						<unk_1 u32=\"0xc0\" />\n\
						<unk_2 u32=\"0x0\" />\n\
						<unk_5 u32=\"0x0\" />\n\
						<unk_6 u32=\"0x40\" />\n\
						<unk_7 float=\"0.00999999978\" />\n\
						<DestructionSubPart />\n\
						<HavokFile filename=\"" + newName + "\" />\n\
					</Destruction>\n\
				";
				*/

				//you can walk on this.
				listHitboxXml_str += "\n\
					<Destruction>\n\
						<unk_0 u32=\"0xd\" />\n\
						<unk_1 u32=\"0x60\" />\n\
						<unk_2 u32=\"0x0\" />\n\
						<unk_5 u32=\"0xc0\" />\n\
						<unk_6 u32=\"0x20\" />\n\
						<unk_7 float=\"0.00999999978\" />\n\
						<DestructionSubPart />\n\
						<HavokFile filename=\"" + newName + "\" />\n\
					</Destruction>\n\
				";

				/*
				//you fly up to this one
				listHitboxXml_str += "\n\
							<Destruction>\n\
								<unk_0 u32=\"0x10\" />\n\
								<unk_1 u32=\"0x1b0\" />\n\
								<unk_2 u32=\"0x0\" />\n\
								<unk_5 u32=\"0x0\" />\n\
								<unk_6 u32=\"0x90\" />\n\
								<unk_7 float=\"0.00999999978\" />\n\
								<DestructionSubPart />\n\
								<HavokFile filename=\"" + newName + "\" />\n\
							</Destruction>\n\
						";
				*/


				listHitboxXmlCouple_str += "<IndexCouple Index0 = \"" + std::to_string(inc) + "\" Index1 = \"4\" / >\n";
				inc++;

				printf("-------------- Make Cylinder/Sphere Havok Xml \n");

				//LibXenoverse::EMO_BaseFile::WriteFile(newfilename +".xml", (uint8_t*)xmlCylinderTemplate_tmp.c_str(), xmlCylinderTemplate_tmp.size(), true);			//test for debug.

				TiXmlDocument doc;
				if (!doc.Parse(xmlCylinderTemplate_tmp.c_str()))
				{
					printf("Error parsing file \"%s\". This is what tinyxml has to say: %s. Row=%d, col=%d.\n", filename.c_str(), doc.ErrorDesc(), doc.ErrorRow(), doc.ErrorCol());
					LibXenoverse::notifyError();
					continue;
				}

				printf("processing...(please wait)\n");



				printf("-------------- Make Havok Cylinder/Sphere\n");

				LibXenoverse::Havok *havok = new LibXenoverse::Havok();
				if (!havok->Compile(&doc))
				{
					printf("Error on parsing havok");
					LibXenoverse::notifyError();
					LibXenoverse::waitOnEnd();
				}

				havok->importFBX(fbxNode);				//by the name could update values on a Object hierarchy.
				havok->SaveToFile(newfilename);
				delete havok;
			}
			

			//todo sphere
		}

		printf("-------------- Make Map Extract\n");

		
			


		listHitboxXml_str = "\n\
	<ListIndexCouple>\n\
			" + listHitboxXmlCouple_str + "\n\
	</ListIndexCouple>\n\
	\n\
	\n\
	\n\
	<Hitbox>\n\
		<name value = \"TodoChangeName\" />\n\
		<child u16 = \"0xffff\" />\n\
		<unk_a0 u16 = \"0xffff\" />\n\
		<sibling u16 = \"0x3\" />\n\
		<parent u16 = \"0x0\" />\n\
		<ListOnDestruction>\n\
			<DestructionGroup>\n\
			" + listHitboxXml_str +"\n\
			</DestructionGroup>\n\
		</ ListOnDestruction>\n\
		<ListHavokFiles />\n\
		<CollisionGeometry filename=\"\">\n\
		</ CollisionGeometry>\n\
	</ Hitbox>\
		";

		LibXenoverse::EMO_BaseFile::WriteFile(basefilename + "_extract_map.xml", (const uint8_t*)listHitboxXml_str.c_str(), listHitboxXml_str.length() + 1, true, false);

		printf("-------------- Destroying Fbx instance\n");
		lScene->Destroy();

	}else {
		printf("Error on arguments.\n");
		LibXenoverse::notifyError();
	}



	printf("finished.\n");
	LibXenoverse::waitOnEnd();
	return 0;
}