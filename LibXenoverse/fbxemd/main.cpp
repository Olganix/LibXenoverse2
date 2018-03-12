

#include "LibXenoverse.h"






bool importFbxCameraAnimation(std::vector<string> &arguments)
{
	string filename = "";
	string extension = "";
	string basefilename = "";

	size_t nbArg = arguments.size();
	for (size_t i = 0; i < nbArg; i++)
	{
		filename = arguments.at(i);
		extension = LibXenoverse::extensionFromFilename(filename, true);
		basefilename = filename.substr(0, filename.length() - (extension.size() + 1));
		string extension2 = LibXenoverse::extensionFromFilename(basefilename, true);
		string basefilename2 = basefilename.substr(0, basefilename.length() - (extension2.size() + 1));
		string extension3 = LibXenoverse::extensionFromFilename(basefilename2, true);
		string basefilename3 = basefilename2.substr(0, basefilename2.length() - (extension3.size() + 1));

		if ((extension == "fbx") && (extension2 == "ean") && (extension3 == "cam"))
		{
			arguments.erase(arguments.begin() + i);
			break;
		}
		filename = "";
	}

	if (!filename.size())
		return false;






	printf("------- Camera Animation detected\n");


	FbxManager *sdk_manager = FbxManager::Create();								// Create FBX
	FbxIOSettings *ios = FbxIOSettings::Create(sdk_manager, IOSROOT);
	ios->SetBoolProp(EXP_FBX_EMBEDDED, true);
	sdk_manager->SetIOSettings(ios);

	FbxImporter* lImporter = FbxImporter::Create(sdk_manager, "");				// Import FBX
	bool lImportStatus = lImporter->Initialize(filename.c_str(), -1, sdk_manager->GetIOSettings());
	if (!lImportStatus)
	{
		// Handle error
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		LibXenoverse::notifyError();
		return false;
	}
	FbxScene* lScene = FbxScene::Create(sdk_manager, "EMDFBXScene_camAnimation");
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



	printf("-------------- Listing FbxAnimations\n");
	FbxAnimStack* lAnimStack = 0;
	FbxAnimLayer* lAnimLayer = 0;
	std::vector<FbxAnimStack *> list_AnimStack;

	const int lAnimCount = lScene->GetSrcObjectCount<FbxAnimStack>();
	for (int lIndex = 0; lIndex < lAnimCount; lIndex++)
	{
		lAnimStack = lScene->GetSrcObject<FbxAnimStack>(lIndex);
		if ((!lAnimStack) || (lAnimStack->GetMemberCount() == 0))
			continue;
		//printf("FbxAnimStack found: %s\n", lAnimStack->GetName());

		lAnimLayer = (FbxAnimLayer*)lAnimStack->GetMember(0);
		if (!lAnimLayer)
			continue;
		printf("FbxAnimLayer found: %s\n", lAnimStack->GetName());

		list_AnimStack.push_back(lAnimStack);
	}

	const int lNodeCount = lScene->GetSrcObjectCount<FbxNode>();
	if ((list_AnimStack.size() == 0) || (lNodeCount == 0))
	{
		printf("Error: No Animation or Node detected in %s. skipped\n", filename.c_str());
		LibXenoverse::notifyError();
		return false;
	}



	

	LibXenoverse::EAN* ean_anim = new LibXenoverse::EAN();
	ean_anim->setType(0x401);										//special type for Camera Animations.
	LibXenoverse::ESK* esk = new LibXenoverse::ESK();
	esk->addBone(new LibXenoverse::ESKBone("Node"));
	ean_anim->setSkeleton(esk);

	//esk->addBone("CameraTarget");			//todo see if necessary


	printf("Converting Animations\n");
	ean_anim->importFBXAnimations(lScene, list_AnimStack);

	ean_anim->removeAnimation("Tpose");
	

	ean_anim->save(filename + ".ean");
	delete ean_anim;

	lScene->Destroy();


	printf("------- \n");
	return true;
}






int main(int argc, char** argv)
{
	printf("*******************************************************************\n\
 This tool is for convert a fbx file into files of Dbxv2.\n\
 Usage: 'fbxemd.exe [options] file.fbx [originalFile.ean]'\n\
 originalFile.ean is for keep order on animations, you better to use it if there is animations, but it's not a obligation anymore.\n\
 IMPORTANT: Blender and 3dsmax miss some textures. SO YOU HAVE to check texture definitions in xml version of emd (80 percents chance there are wrong).\n\
 IMPORTANT: from this version, xxx.cam.ean.fbx will be converted as a xxxxx.cam.ean, independently of others files.\n\
 Options : '-noCompressedFlag', '-NoWait', '-AlwaysWait', '-WaitOnError' (default), or '-WaitOnWarning'.\n\
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


	bool cameraAnimationImported = importFbxCameraAnimation(arguments);


	if (arguments.size() == 0)
	{
		if (cameraAnimationImported)
		{
			LibXenoverse::waitOnEnd();
			return 0;
		}

		printf("Error not enougth arguments.\n");
		LibXenoverse::notifyError();
		LibXenoverse::waitOnEnd();
		return 1;
	}

	
	//options
	bool compressedFlag = true;

	size_t nbArg = arguments.size();
	for (size_t i = 0; i < nbArg; i++)
	{
		if (arguments.at(i) == "-noCompressedFlag")
		{
			arguments.erase(arguments.begin() + i);
			compressedFlag = false;
			break;
		}
	}
	

	nbArg = arguments.size();
	for (size_t i = 0; i <nbArg; i++)										//we need to have ean first.
	{
		if (LibXenoverse::extensionFromFilename(arguments.at(i), true) == "ean")
		{
			arguments.insert(arguments.begin(), arguments.at(i));
			arguments.erase(arguments.begin() + i + 1);
			break;
		}
	}







	printf("Converter FBX to Emd/Esk/Ean Started.\n");



	
	string filename = arguments.at(0);
	string extension = LibXenoverse::extensionFromFilename(filename, true);



	//load original ean to setup the name list of animations
	LibXenoverse::EAN* ean_anim = new LibXenoverse::EAN();
	std::vector<string> listNameOriginalAnimation;			//saddly, it's must have some where a list of index of animations, so We have to keep animations orders. we will do that with original ean file. TODO : may be we could break this after look at bcs, bcm files.

	if (extension == "ean")
	{
		printf("-------------- Load original Ean (for bones order). Please wait ...\n");

		if (ean_anim->load(filename))
		{
			//keep animation order for after
			vector<LibXenoverse::EANAnimation> &animList = ean_anim->getAnimations();
			size_t nbelements = animList.size();
			for (size_t i = 0; i < nbelements; i++)
				listNameOriginalAnimation.push_back(animList.at(i).getName());

			ean_anim->getAnimations().clear();
		}

		arguments.erase(arguments.begin());
	}
	




	printf("-------------- Load Fbx. Please wait ...\n");


	filename = arguments.at(0);
	extension = LibXenoverse::extensionFromFilename(filename, true);
	string folder = LibXenoverse::folderFromFilename(filename);

	if (extension != "fbx")
	{
		printf("error: faild to load fbx.\n");
		LibXenoverse::notifyError();
		LibXenoverse::waitOnEnd();
		return 1;
	}
	


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
	FbxScene* lScene = FbxScene::Create(sdk_manager, "FBXEMDImport");
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


	

	printf("-------------- Converting FbxMeshes to Emd\n");

	// Search Scene
	FbxStatus lStatus;
	FbxNode* fbxNode;
	bool isOldMethode = false;				//old's blender use or very custom fbx need to be take care differently. the new methode is for rebuild the exact Emd file in structure use (model mesh submesh ...)
	const int lNodeCount = lScene->GetSrcObjectCount<FbxNode>();
	std::vector<FbxNode*> listEmdNodeAllReadyCreated;
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


		//check if it's the new methode, witch it will have 4 parents (3 if you don't count RootNode)
		FbxNode* fbxNode_tmp = fbxNode;
		size_t nbParents = 0;

		while ((fbxNode_tmp->GetParent())&&(fbxNode_tmp->GetParent() != fbxRootNode))
		{
			nbParents++;
			fbxNode_tmp = fbxNode_tmp->GetParent();
		}

		isOldMethode = (nbParents!=3);
		if (isOldMethode)
			fbxNode_tmp = fbxNode;


		//the new way need to have the first parent of meshNode. so, by iterate meshnode, you could have the same first parent many time. so we have to skip some of.
		size_t nbEmdNode = listEmdNodeAllReadyCreated.size();
		bool isfound = false;
		for (size_t i = 0; i < nbEmdNode; i++)
		{
			if (listEmdNodeAllReadyCreated.at(i)== fbxNode_tmp)
			{
				isfound = true;
				break;
			}
		}
		if (isfound)							//allready done, skip.
			continue;

		listEmdNodeAllReadyCreated.push_back(fbxNode_tmp);
		
		LibXenoverse::EMD *emd_model = new LibXenoverse::EMD();
		if (emd_model)
		{
			emd_model->importFBX(fbxNode_tmp, compressedFlag);
			emd_model->save(folder + fbxNode_tmp->GetName() + ".emd");
			delete emd_model;
		}
	}

	






	printf("-------------- Listing FbxAnimations\n");
	
	FbxAnimStack* lAnimStack = 0;
	FbxAnimLayer* lAnimLayer = 0;
	std::vector<FbxAnimStack *> list_AnimStack;

	const int lAnimCount = lScene->GetSrcObjectCount<FbxAnimStack>();
	for (int lIndex = 0; lIndex < lAnimCount; lIndex++)
	{
		lAnimStack = lScene->GetSrcObject<FbxAnimStack>(lIndex);
		if ((!lAnimStack) || (lAnimStack->GetMemberCount() == 0))
			continue;
		//printf("FbxAnimStack found: %s\n", lAnimStack->GetName());

		lAnimLayer = (FbxAnimLayer*)lAnimStack->GetMember(0);
		if (!lAnimLayer)
			continue;
		printf("FbxAnimLayer found: %s\n", lAnimStack->GetName());

		list_AnimStack.push_back(lAnimStack);
	}


	if ((list_AnimStack.size() != 0) && (lNodeCount!=0))
	{
		printf("-------------- Converting Animations\n");
		
		//in first time, we make only one File by animations. TODO generalise to have only one file.
		{
			ean_anim->importFBXAnimations(lScene, list_AnimStack);
			


			//saddly , we must keep the same animation order, because of others file, bsc, bcm witch use index in Game, apparently.
			vector<LibXenoverse::EANAnimation> &animList = ean_anim->getAnimations();
			vector<bool> animUsed;
			
			size_t nbelements = animList.size();
			animUsed.resize(nbelements, false);

			vector<LibXenoverse::EANAnimation> newAnimList;

			size_t nbelements_oldAnima = listNameOriginalAnimation.size();
			for (size_t i = 0; i < nbelements_oldAnima; i++)
			{
				string name_tmp = listNameOriginalAnimation.at(i);

				size_t isfound = (size_t)-1;
				for (size_t j = 0; j < nbelements; j++)
				{
					if (animList.at(j).getName() == name_tmp)
					{
						newAnimList.push_back(animList.at(j));			//we keep the animation founded in the same place, to keep index order.
						isfound = j;
						animUsed.at(j) = true;
						break;
					}
				}
				if (isfound==(size_t)-1)								//we add old animations to keep index order (about the game's file).
				{
					newAnimList.push_back(LibXenoverse::EANAnimation(ean_anim));
					newAnimList.back().setName(name_tmp);
				}
			}

			//new animations will be add at the end (witch will not break the indexes order).
			for (size_t j = 0; j < nbelements; j++)
				if(!animUsed.at(j))
					newAnimList.push_back(animList.at(j));


			//and after, we copy animations in good order.
			nbelements_oldAnima = newAnimList.size();
			animList.clear();
			for (size_t i = 0; i < nbelements_oldAnima; i++)
				animList.push_back(newAnimList.at(i));
			
			
			
			ean_anim->save(filename + ".ean");
			

			// "RAD_BAS_BONE" -> "EXP_RAD_000"
			LibXenoverse::ESK* skeleton = ean_anim->getSkeleton();
			LibXenoverse::ESKBone* bone;
			size_t nbBones = skeleton->getBones().size();
			for (size_t i = 0; i < nbBones; i++)
			{
				bone = skeleton->getBones().at(i);
				if (bone->parent_index >= nbBones)				//is one of rootNode
				{
					string name = bone->getName();
					if ((name.length() > 9) && (name.substr(name.length() - 9)) == "_BAS_BONE")
					{	
						bone->setName("EXP_" + name.substr(0, name.length() - 9) +"_000");
						break;											//only one could have this name.
					}
				}
			}

			skeleton->save(filename + ".esk");

			

			delete ean_anim;
		}


	}else {			

		printf("-------------- NoAnimations, but try to have the skeleton.\n");

		if (ean_anim)
			delete ean_anim;
		
		LibXenoverse::ESK esk = LibXenoverse::ESK();
		esk.importFBXSkeleton(lScene);

		// "RAD_BAS_BONE" -> "EXP_RAD_000"
		LibXenoverse::ESKBone* bone;
		size_t nbBones = esk.getBones().size();
		for (size_t i = 0; i < nbBones; i++)
		{
			bone = esk.getBones().at(i);
			if (bone->parent_index >= nbBones)				//is one of rootNode
			{
				string name = bone->getName();
				if ((name.length() > 9) && (name.substr(name.length() - 9)) == "_BAS_BONE")
				{
					bone->setName("EXP_" + name.substr(0, name.length() - 9) + "_000");
					break;											//only one could have this name.
				}
			}
		}

		esk.save(filename + ".esk");
	}





	printf("-------------- Destroying Fbx instance\n");
	lScene->Destroy();



	printf("finished.\n");
	LibXenoverse::waitOnEnd();
	return 0;
}