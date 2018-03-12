#include "LibXenoverse.h"



int main(int argc, char** argv)
{
	printf("*******************************************************************\n\
 This tool is for extract some 3d data of Havok file into fbx mesh for edition into Blender or 3dsmax, and rematch to update hkx in reverse.\n\
 Usage: 'havokfbx.exe [options] file.hkx' or 'havokfbx.exe [options] file.fbx file.hkx'\n\
 Options : '-NoWait', '-AlwaysWait', '-WaitOnError' (default), or '-WaitOnWarning'.\n\
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


	size_t nbArg = arguments.size();
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
	if (arguments.size() > 1)
	{
		filename2 = arguments.at(1);
		extension2 = LibXenoverse::extensionFromFilename(filename2, true);
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

			havok->importFBX(fbxNode);				//by the anme could update values on a Object hierarchy.
		}


		havok->SaveToFile(basefilename +"_modified.hkx");
		delete havok;

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