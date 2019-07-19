#ifdef LIBXENOVERSE_FBX_SUPPORT

namespace LibXenoverse
{





/*-------------------------------------------------------------------------------\
|                             importFBXSkeleton									 |
\-------------------------------------------------------------------------------*/
void ESK::importFBXSkeleton(FbxScene *scene, bool allowCamera)
{
	bones.clear();
	EskTreeNode* rootNode = getTreeOrganisation();

	mHaveExtraBytesOnEachBone = !allowCamera;

	FbxNode *lNode;
	FbxSkeleton *lSkeleton;
	FbxStatus lStatus;
	FbxMatrix parent_fbx_matrix, parent_fbx_matrix_b, fbx_matrix;
	FbxVector4 pTranslation;
	FbxVector4 lcl_rotation;
	FbxQuaternion pRotation;
	FbxVector4 pShearing;
	FbxVector4 pScaling;
	std::vector<FbxNode*> mlistParents;

	size_t nbOriginalBones = bones.size();				//in this case, we allready have bone definitions in EAN file, now we have to define transformation matrix.
	string name = "";
	ESKBone *bone = 0;


	const int lNodeCount = scene->GetSrcObjectCount<FbxNode>();

	for (int lIndex = 0; lIndex < lNodeCount; lIndex++)
	{
		lNode = scene->GetSrcObject<FbxNode>(lIndex);
		if (!lNode)
			continue;
		//printf("Node found: %s\n", lNode->GetName());

		lSkeleton = lNode->GetSkeleton();
		
		if (!lSkeleton)
		{
			//second try for RAD_BAS_BONE
			bool isRootSkeleton = false;
			size_t nbChilds = lNode->GetChildCount();
			for (size_t i = 0; i < nbChilds; i++)
			{
				if (lNode->GetChild(i)->GetSkeleton())
				{
					isRootSkeleton = true;
					break;
				}
			}

			if (!isRootSkeleton)
			{
				//a last try to Camera case.
				if ((!allowCamera) || (!lNode->GetCamera()))
					continue;
			}
		}

		name = lNode->GetName();

		printf("FbxSkeleton found: %s\n", name.c_str());

		if (rootNode->getBoneWithName(name))
		{
			printf("error: this bone allready into hierarchie. skipped\n");
			LibXenoverse::notifyError();
			continue;
		}

		bone = new ESKBone(name);
		bones.push_back(bone);


		FbxNode* boneParent = lNode->GetParent();
		if (boneParent)
		{
			EskTreeNode* parent = rootNode->getBoneWithName(boneParent->GetName());
			if (parent)
			{
				std::vector<std::vector<size_t>> aa;
				parent->addTree( new EskTreeNode(bone, bones.size()-1, parent), false, bones.size(), aa, bones, false );
			}else {
				std::vector<std::vector<size_t>> aa;
				rootNode->addTree(new EskTreeNode(bone, bones.size() - 1, rootNode), false, bones.size(), aa, bones, false);
				printf("Warning: parent not found (%s), put on rootNode\n", boneParent->GetName());
				LibXenoverse::notifyWarning();
			}
		}else {
			std::vector<std::vector<size_t>> aa;
			rootNode->addTree(new EskTreeNode(bone, bones.size() - 1, rootNode), false, bones.size(), aa, bones, false);
		}



		//get fbx Transformation matrix informations
		pTranslation = lNode->LclTranslation.Get();
		lcl_rotation = lNode->LclRotation.Get();
		pScaling = lNode->LclScaling.Get();


		//on verifit la convention sur les rotation, l'ordre des rotations d'Euler.
		FbxEuler::EOrder order;
		lNode->GetRotationOrder(FbxNode::EPivotSet::eSourcePivot, order);

		FbxQuaternion rotation;

		switch (order)
		{
		case FbxEuler::EOrder::eOrderXZY:								//that one from XenoVerse's file
			{
				//test = FbxDouble3(lcl_rotation[1], lcl_rotation[2], lcl_rotation[0]);	//lacet sur Y //tangage sur Z //roulis sur X
				rotation = FbxQuaternion(FbxVector4(0, 1, 0, 1), lcl_rotation[1]) * FbxQuaternion(FbxVector4(0, 0, 1, 1), lcl_rotation[2]) * FbxQuaternion(FbxVector4(1,0,0,1), lcl_rotation[0]);	//lacet sur Y //tangage sur Z //roulis sur X
			}
			break;



		case FbxEuler::EOrder::eOrderXYZ:
			{
				rotation = FbxQuaternion(FbxVector4(0, 0, 1, 1), lcl_rotation[2]) * FbxQuaternion(FbxVector4(0, 1, 0, 1), lcl_rotation[1]) * FbxQuaternion(FbxVector4(1, 0, 0, 1), lcl_rotation[0]);
			}
			break;



		case FbxEuler::EOrder::eOrderYZX:
			{
				rotation = FbxQuaternion(FbxVector4(1, 0, 0, 1), lcl_rotation[0]) * FbxQuaternion(FbxVector4(0, 0, 1, 1), lcl_rotation[2]) * FbxQuaternion(FbxVector4(0, 1, 0, 1), lcl_rotation[1]);
			}
			break;
		case FbxEuler::EOrder::eOrderYXZ:
			{
				rotation = FbxQuaternion(FbxVector4(0, 0, 1, 1), lcl_rotation[2]) * FbxQuaternion(FbxVector4(1, 0, 0, 1), lcl_rotation[0]) * FbxQuaternion(FbxVector4(0, 1, 0, 1), lcl_rotation[1]);
			}
			break;



		case FbxEuler::EOrder::eOrderZXY:
			{
				rotation = FbxQuaternion(FbxVector4(0, 1, 0, 1), lcl_rotation[1]) * FbxQuaternion(FbxVector4(1, 0, 0, 1), lcl_rotation[0]) * FbxQuaternion(FbxVector4(0, 0, 1, 1), lcl_rotation[2]);
			}
			break;


		case FbxEuler::EOrder::eOrderZYX:
			{
				rotation = FbxQuaternion(FbxVector4(1, 0, 0, 1), lcl_rotation[0]) * FbxQuaternion(FbxVector4(0, 1, 0, 1), lcl_rotation[1]) * FbxQuaternion(FbxVector4(0, 0, 1, 1), lcl_rotation[2]);
			}
			break;


		case FbxEuler::EOrder::eOrderSphericXYZ:
			{
				//todo put a warning : we will use eOrderXYZ instead of spherique version witch we don't have any informations.
				rotation = FbxQuaternion(FbxVector4(0, 0, 1, 1), lcl_rotation[2]) * FbxQuaternion(FbxVector4(0, 1, 0, 1), lcl_rotation[1]) * FbxQuaternion(FbxVector4(1, 0, 0, 1), lcl_rotation[0]);
			}
			break;
		}


		//using Relative information directly.
		float* relativeTransform = bone->skinning_matrix;

		relativeTransform[0] = (float)pTranslation[0];
		relativeTransform[1] = (float)pTranslation[1];
		relativeTransform[2] = (float)pTranslation[2];
		relativeTransform[3] = (float)pTranslation[3];

		relativeTransform[4] = (float)rotation[0];
		relativeTransform[5] = (float)rotation[1];
		relativeTransform[6] = (float)rotation[2];
		relativeTransform[7] = (float)rotation[3];

		relativeTransform[8] = (float)pScaling[0];
		relativeTransform[9] = (float)pScaling[1];
		relativeTransform[10] = (float)pScaling[2];
		relativeTransform[11] = (float)pScaling[3];
	}


	setTreeOrganisation(rootNode);


	//last things todo is to calcul the absolute matrix  (we do this here to be sure to have parent allready defined). could be optimize.
	size_t nbBones = bones.size();
	for (size_t i = 0; i < nbBones; ++i)
		bones.at(i)->calculTransformMatrixFromSkinningMatrix(bones, true);	
}


/*-------------------------------------------------------------------------------\
|                             exportFBXBone										 |
\-------------------------------------------------------------------------------*/
void ESK::exportFBXBone(FbxScene *scene, vector<ESK::FbxBonesInstance_DBxv> &global_fbx_bones, FbxNode* parentFbxNode, unsigned short parentIndex)
{
	bool haveRealParent = (parentIndex != 0xFFFF);

	if ((!parentFbxNode) || (!haveRealParent))
		parentFbxNode = scene->GetRootNode();


	ESKBone* bone = 0;
	FbxNode* fbxNode = 0;
	FbxSkeleton* skeletonAttribute = 0;
	FbxVector4 position;
	FbxQuaternion quaternion;
	FbxVector4 anglesRotation;
	FbxVector4 scale;

	size_t nbBones = bones.size();
	for (size_t i = 0; i < nbBones; i++)
	{
		bone = bones.at(i);
		if (bone->parent_index != parentIndex)						//search All direct Children.
			continue;


		fbxNode = FbxNode::Create(scene, bone->name.c_str());

		fbxNode->SetRotationActive(true);
		fbxNode->SetRotationOrder(FbxNode::EPivotSet::eSourcePivot, FbxEuler::EOrder::eOrderXZY);		// => good for both with giveAngleTaitBryan.


		skeletonAttribute = FbxSkeleton::Create(scene, bone->name.c_str());
		skeletonAttribute->SetSkeletonType((haveRealParent) ? FbxSkeleton::eLimbNode : FbxSkeleton::eRoot);
		fbxNode->SetNodeAttribute(skeletonAttribute);


		//using Relative information directly.
		float* relativeTransform = bone->skinning_matrix;

		position = FbxVector4(relativeTransform[0], relativeTransform[1], relativeTransform[2], relativeTransform[3]);
		quaternion = FbxQuaternion(relativeTransform[4], relativeTransform[5], relativeTransform[6], relativeTransform[7]);
		scale = FbxVector4(relativeTransform[8], relativeTransform[9], relativeTransform[10], relativeTransform[11]);



		
		FbxVector4 anglesRotation = quaternion.DecomposeSphericalXYZ();

		if (anglesRotation[0] != anglesRotation[0])		//check Nan possibility
			anglesRotation[0] = 0.0;
		if (anglesRotation[1] != anglesRotation[1])		//check Nan possibility
			anglesRotation[1] = 0.0;
		if (anglesRotation[2] != anglesRotation[2])		//check Nan possibility
			anglesRotation[2] = 0.0;



		//version with is good for FbxEuler::EOrder::eOrderXZY
		FbxDouble3 test = giveAngleOrientationForThisOrientationTaitBryan(FbxVector4(quaternion[0], quaternion[1], quaternion[2], quaternion[3]));
		anglesRotation[1] = test[0];			//yaw sur Y
		anglesRotation[2] = test[1];			//pitch sur Z
		anglesRotation[0] = test[2];			//roll sur X		=>YZX order



		fbxNode->LclTranslation.Set(position);
		fbxNode->LclRotation.Set(anglesRotation);
		fbxNode->LclScaling.Set(scale);


		// attach to parent.
		parentFbxNode->AddChild(fbxNode);
		global_fbx_bones.push_back(ESK::FbxBonesInstance_DBxv(bone->name, fbxNode, i));				//save for futur manipulations

		exportFBXBone(scene, global_fbx_bones, fbxNode, i);					// do the same to Child, recursive style.
	}
}




}


#endif