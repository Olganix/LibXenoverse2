namespace LibXenoverse
{





/*-------------------------------------------------------------------------------\
|                             EAN								                 |
\-------------------------------------------------------------------------------*/
EAN::~EAN(void)
{
	if (skeleton != NULL)
		delete skeleton;
	skeleton = NULL;
}
/*-------------------------------------------------------------------------------\
|                             load								                 |
\-------------------------------------------------------------------------------*/
bool EAN::load(string filename)
{
	name = nameFromFilenameNoExtension(filename, true);

	File file(filename, LIBXENOVERSE_FILE_READ_BINARY);
	if (file.valid() && file.readHeader(LIBXENOVERSE_EAN_SIGNATURE))
	{
		read(&file);
		file.close();
	}else{
		return false;
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             save					                             |
\-------------------------------------------------------------------------------*/
void EAN::save(string filename, bool big_endian)
{
	File file(filename, LIBXENOVERSE_FILE_WRITE_BINARY);
	if (file.valid())
	{
		file.writeHeader(LIBXENOVERSE_EAN_SIGNATURE, big_endian);
			
		file.goToAddress(4);
		unsigned int idfile = 0x0020FFFE;
		file.writeInt32(&idfile);
			
		file.goToAddress(8);

		write(&file);
		file.close();
	}
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void EAN::read(File *file)
{
	file->goToAddress(8);

	unsigned int skeleton_offset = 0;
	unsigned int animation_keyframes_offset = 0;
	unsigned int animation_names_offset = 0;
	unsigned short animation_count = 0;

	file->readInt32E(&unknown_total);
	file->moveAddress(6);
	file->readInt16E(&animation_count);
	file->readInt32E(&skeleton_offset);
	file->readInt32E(&animation_keyframes_offset);
	file->readInt32E(&animation_names_offset);

	LOG_DEBUG("--------------- read EAN \n[8] unkTotal : %i, animation_count : %i, SkeletonOffset : [%i], animation_keyframes_offset : [%i], animation_names_offset : [%i]\n", unknown_total, animation_count, skeleton_offset, animation_keyframes_offset, animation_names_offset);

	// Read Skeleton
	LOG_DEBUG("----------- Skeleton\n");
	file->goToAddress(skeleton_offset);
	skeleton = new ESK();
	skeleton->read(file);

	// Read Animations
	LOG_DEBUG("----------- Animations KeyFrames\n");
	animations.resize(animation_count);
	unsigned int address = 0;
	for (size_t i = 0; i < animation_count; i++)
	{
		// Read Keyframes
		file->goToAddress(animation_keyframes_offset + i * 4);
		file->readInt32E(&address);
		file->goToAddress(address);

		LOG_DEBUG("------ animation %i : [%i] => [%i]\n", i, animation_keyframes_offset + i * 4, address);

		animations[i].setParent(this);
		animations[i].read(file);
	}
		
	LOG_DEBUG("----------- Animations Names\n");
	for (size_t i = 0; i < animation_count; i++)
	{
		// Read Name
		file->goToAddress(animation_names_offset + i * 4);
		file->readInt32E(&address);
		file->goToAddress(address);
		animations[i].readName(file);
		LOG_DEBUG("------ animation %i : [%i] => [%i] => %s\n", i, animation_names_offset + i * 4, address, animations[i].getName().c_str());
	}


	LOG_DEBUG("--------------- End reading EAN \n");
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void EAN::write(File *file)
{
	file->goToAddress(8);

	unsigned int ean_header_size = 32;

	unsigned short animation_count = animations.size();

	//differents parts of the file
	unsigned int skeleton_offset = ean_header_size;
	unsigned int animation_keyframes_offset = 0;
	unsigned int animation_names_offset = 0;	//define after.

	file->writeInt32E(&unknown_total);
	file->writeNull(2);
	unsigned int unknown_1 = 0x04000000;
	file->writeInt32E(&unknown_1);				//non iddentifier
	file->writeInt16E(&animation_count);
	file->writeInt32E(&skeleton_offset);
	file->writeInt32E(&animation_keyframes_offset);
	file->writeInt32E(&animation_names_offset);
		
	LOG_DEBUG("--------------- write EAN \n[8] unkTotal : %i, animation_count : %i, SkeletonOffset : [%i], animation_keyframes_offset : [%i], animation_names_offset : [%i]\n", unknown_total, animation_count, skeleton_offset, animation_keyframes_offset, animation_names_offset);

	// Write Skeleton
	LOG_DEBUG("----------- Skeleton\n");
	file->goToAddress(skeleton_offset);
	skeleton->write(file, false);

		
	// Write Animations
	animation_keyframes_offset = file->getCurrentAddress();
	LOG_DEBUG("----------- Animations KeyFrames - animation_keyframes_offset : [%i] \n", animation_keyframes_offset);
	size_t keyframe_size = 0;
	size_t current_keyframe_size = 0;
	size_t sizeToFill = 0;
	unsigned int address_start_keyframeDef = (size_t)(ceil((animation_keyframes_offset + animation_count * 4) / 16.0f) * 16);			//the adresss is always a start of a 16octets lines.
	unsigned int address = 0;

	for (size_t i = 0; i < animation_count; i++)
	{
		// Write Keay frames
		file->goToAddress(animation_keyframes_offset + i * 4);
		address = address_start_keyframeDef + keyframe_size;
		file->writeInt32E(&address);
		file->goToAddress(address);

		LOG_DEBUG("------ animation %i : [%i] => [%i]\n", i, animation_keyframes_offset + i * 4, address);
			
		animations[i].setParent(this);
		current_keyframe_size = animations[i].write(file);


		//fill zero on end of 16 octets lines. that why we don't have the wright number.
		sizeToFill = (size_t)(ceil(current_keyframe_size / 16.0f) * 16) - current_keyframe_size;
		if (sizeToFill >= 16)
			assert("sizeToFill >= 16");
		if (sizeToFill != 0)
		{
			file->writeNull(sizeToFill);
			current_keyframe_size += sizeToFill;
		}
		keyframe_size += current_keyframe_size;


		if (i + 1 != animation_count)			//strangely not for the last.
		{
			//there is an empty line of 16octet
			file->writeNull(16);
			keyframe_size += 16;
		}
	}

	//for Apple (the character), there is a strange behaviour on the last animaion definition. So I have 12 octet like reading say. This is not a 16octets lines cast.
	file->writeNull(12);
	keyframe_size += 12;

		
	animation_names_offset = file->getCurrentAddress();
	LOG_DEBUG("----------- Animations Names - animation_names_offset : [%i] \n", animation_names_offset);

	size_t names_size = 0;
	for (size_t i = 0; i < animation_count; i++)
	{
		// Write Name
		file->goToAddress(animation_names_offset + i * 4);
		address = animation_names_offset + animation_count * 4 + names_size;
		file->writeInt32E(&address);
		file->goToAddress(address);
		animations[i].writeName(file);
		names_size += animations[i].getName().length() + 1;		//+1 for \0

		LOG_DEBUG("------ animation %i : [%i] => [%i] => %s\n", i, animation_names_offset + i * 4, address, animations[i].getName().c_str());
	}


	//update header.
	file->goToAddress(8 + 4 + 6 + 2 + 4);
	file->writeInt32E(&animation_keyframes_offset);
	file->writeInt32E(&animation_names_offset);


	file->goToEnd();

	LOG_DEBUG("--------------- End writting EAN \n");
}
/*-------------------------------------------------------------------------------\
|                             merge					                             |
\-------------------------------------------------------------------------------*/
void EAN::merge(EAN *ean)
{
	if (!ean)
		return;
		
	//merge skeleton definitions
	if (ean->getSkeleton() != NULL)
	{
		if (this->skeleton == NULL)
			this->skeleton = new ESK();
		this->skeleton->merge(ean->getSkeleton());				//add missed bones and update transformation matrices. 
	}
		
		
	//merge animations
	size_t nbAnim = this->animations.size();
	size_t nbNewAnim = ean->animations.size();
	bool isfound = false;
	string name = "";

	for (size_t i = 0; i < nbNewAnim; i++)
	{
		EANAnimation &eanAnim = ean->animations.at(i);
		name = eanAnim.getName();

		isfound = false; 
		for (size_t j = 0; j < nbAnim; j++)
		{
			if (this->animations.at(j).getName() == name)
			{
				isfound = true;

				std::vector<string> &list = eanAnim.getFilenameOriginList();
				std::vector<string> &mergedList = this->animations.at(j).getFilenameOriginList();
				size_t nbfilename = list.size();
				for (size_t k = 0; k < nbfilename; k++)
					mergedList.push_back(list.at(k));

				break;
			}
		}

		if (!isfound)										//we add only new named animation.
		{
			this->animations.push_back(eanAnim);					//it's a copy, not a reference.
			nbAnim++;
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             changeBoneIndexes		                             |
\-------------------------------------------------------------------------------*/
void EAN::changeBoneIndexes(const std::vector<std::vector<size_t>> &listMovingindex)
{
	size_t nbAnim = animations.size();
	for (size_t i = 0; i < nbAnim; i++)
		animations.at(i).changeBoneIndexes(listMovingindex);
}
/*-------------------------------------------------------------------------------\
|                             addBoneAnimationFromAnotherEan                     |
\-------------------------------------------------------------------------------*/
void EAN::addBoneAnimationFromAnotherEan(EAN* ean_src, std::vector<std::vector<size_t>> &listRelationIndex, bool matchDuration)
{
	size_t nbAnim = animations.size();
	size_t nbAnim_src = ean_src->animations.size();
	string baseAnim_Name = "";
	string baseAnim_Name_src = "";
	size_t first_ = 0;
	for (size_t i = 0; i < nbAnim; i++)
	{
		baseAnim_Name = animations.at(i).getName();
		first_ = baseAnim_Name.find_first_of('_');
		if (first_ != std::string::npos)
			baseAnim_Name = baseAnim_Name.substr(first_);

			
		for (size_t j = 0; j < nbAnim_src; j++)
		{
			baseAnim_Name_src = ean_src->animations.at(j).getName();
			first_ = baseAnim_Name_src.find_first_of('_');
			if (first_ != std::string::npos)
				baseAnim_Name_src = baseAnim_Name_src.substr(first_);
				
			if (baseAnim_Name == baseAnim_Name_src)
			{
				animations.at(i).addBoneAnimationFromAnotherEan(ean_src->animations.at(j), listRelationIndex, matchDuration);
				break;
			}
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             addTPoseAnimation				                     |
\-------------------------------------------------------------------------------*/
void EAN::addTPoseAnimation(void)
{
	size_t nbAnim = animations.size();
	for (size_t i = 0; i < nbAnim; i++)
		if (animations.at(i).getName() == "Tpose")
			return;

	animations.push_back(EANAnimation());
	animations.back().setName("Tpose");
	animations.back().addTPoseAnimation(skeleton);
}



#ifdef LIBXENOVERSE_FBX_SUPPORT







/*-------------------------------------------------------------------------------\
|                             importFBXAnimations	                             |
\-------------------------------------------------------------------------------*/
void EAN::importFBXAnimations(FbxScene *scene, std::vector<FbxAnimStack *> list_AnimStack)
{
	//Skeleton is needed because aniamtion use skeleton original position/rotation/scale. definition (name of bone) is written in ean file.
	if (!skeleton)
		skeleton = new ESK();
	skeleton->importFBXSkeleton(scene);
		
		
	size_t nbAnims = list_AnimStack.size();
	for (size_t i = 0; i < nbAnims; i++)
	{
		EANAnimation animation;
		animation.setParent(this);
		animation.importFBXAnimation(scene, list_AnimStack.at(i), skeleton);
		animations.push_back(animation);
	}
}
/*-------------------------------------------------------------------------------\
|                             exportFBXAnimations	                             |
\-------------------------------------------------------------------------------*/
vector<FbxAnimCurveNode *> EAN::exportFBXAnimations(FbxScene *scene, std::vector<FbxAnimStack *> list_AnimStack, ESK::FbxBonesInstance_DBxv* fbxBoneInst)
{
	size_t indexBone = fbxBoneInst->mIndexInAnimation;
	string fbxBoneInstName = fbxBoneInst->mName;
	FbxNode* fbx_node = fbxBoneInst->mNode;
		

	vector<FbxAnimCurveNode *> fbx_anims;
	if (fbx_node == NULL)
		return fbx_anims;
		
	EANAnimation *animation;
	int keyframes_count;
	FbxAnimStack* lAnimStack;
	FbxAnimLayer* lAnimLayer;

	size_t nbAnims = animations.size();
	for (size_t i = 0; i < nbAnims; i++)
	{
		if (i >= list_AnimStack.size())
			break;

		animation = &(animations.at(i));
		keyframes_count = animation->getFrameCount();
		vector<EANAnimationNode> &anim_nodes = animation->getNodes();
			
		lAnimStack = list_AnimStack.at(i);
		lAnimLayer = (FbxAnimLayer*)lAnimStack->GetMember(0);


		size_t nbAnimsNodes = anim_nodes.size();
		for (size_t j = 0; j < nbAnimsNodes; j++)
		{
			if (fbxBoneInstName == skeleton->getBone( anim_nodes.at(j).getBoneIndex())->getName() )		//animations and bones are linked by the indexBone
			{
				FbxAnimCurveNode* fbx_anim_curveNode = createFBXAnimationCurveNode(fbx_node, animation, &anim_nodes.at(j), lAnimStack, lAnimLayer);
				fbx_anims.push_back(fbx_anim_curveNode);
			}
		}
	}

	return fbx_anims;
}
/*-------------------------------------------------------------------------------\
|                             createFBXAnimationCurveNode                        |
\-------------------------------------------------------------------------------*/
FbxAnimCurveNode *EAN::createFBXAnimationCurveNode(FbxNode *fbx_node, EANAnimation *animation, EANAnimationNode* anim_node, FbxAnimStack *lAnimStack, FbxAnimLayer* lAnimLayer) {


	float fps = 60.0f;


		
	ESKBone *esk_bone = 0;
	FbxMatrix bone_relative_matrix;
	bone_relative_matrix.SetIdentity();

	FbxVector4 pos_tmp;
	FbxVector4 scale_tmp;
	FbxQuaternion rot_tmp;
	FbxVector4 rotation_eulerAngles;

	size_t index_bone = anim_node->getBoneIndex();

	FbxEuler::EOrder aa;
	FbxEuler::EOrder ab;
	fbx_node->GetRotationOrder(FbxNode::EPivotSet::eSourcePivot, aa);
	fbx_node->GetRotationOrder(FbxNode::EPivotSet::eDestinationPivot, ab);


	
	pos_tmp = fbx_node->LclTranslation.Get();
	rotation_eulerAngles = fbx_node->LclRotation.Get();
	

	scale_tmp = fbx_node->LclScaling.Get();
	FbxMatrix fbx_bone_relative_matrix;
	fbx_bone_relative_matrix.SetTRS(pos_tmp, rotation_eulerAngles, scale_tmp);
		

	if ((skeleton) && (index_bone < skeleton->getBones().size()))
	{
		esk_bone = skeleton->getBones().at(index_bone);

		if (esk_bone)
		{
			pos_tmp = FbxVector4(esk_bone->skinning_matrix[0], esk_bone->skinning_matrix[1], esk_bone->skinning_matrix[2], esk_bone->skinning_matrix[3]);
			rot_tmp = FbxQuaternion(esk_bone->skinning_matrix[4], esk_bone->skinning_matrix[5], esk_bone->skinning_matrix[6], esk_bone->skinning_matrix[7]);
			scale_tmp = FbxVector4(esk_bone->skinning_matrix[8], esk_bone->skinning_matrix[9], esk_bone->skinning_matrix[10], esk_bone->skinning_matrix[11]);

			bone_relative_matrix.SetTQS(pos_tmp, rot_tmp, scale_tmp);
		}
	}



	FbxAnimCurveNode *fbx_animCurveNode = fbx_node->GeometricTranslation.GetCurveNode(lAnimLayer, true);

	FbxAnimCurve *fbx_animCurve_translation_x = 0;
	FbxAnimCurve *fbx_animCurve_translation_y = 0;
	FbxAnimCurve *fbx_animCurve_translation_z = 0;
	FbxAnimCurve *fbx_animCurve_rotation_x = 0;
	FbxAnimCurve *fbx_animCurve_rotation_y = 0;
	FbxAnimCurve *fbx_animCurve_rotation_z = 0;
	FbxAnimCurve *fbx_animCurve_scale_x = 0;
	FbxAnimCurve *fbx_animCurve_scale_y = 0;
	FbxAnimCurve *fbx_animCurve_scale_z = 0;

		
	if (anim_node->haveKeyFrameAnimation(LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION))
	{
		fbx_animCurve_translation_x = fbx_node->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
		fbx_animCurve_translation_y = fbx_node->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
		fbx_animCurve_translation_z = fbx_node->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
	}
	if (anim_node->haveKeyFrameAnimation(LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION))
	{
		fbx_animCurve_rotation_x = fbx_node->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
		fbx_animCurve_rotation_y = fbx_node->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
		fbx_animCurve_rotation_z = fbx_node->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
	}
	if (anim_node->haveKeyFrameAnimation(LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE))
	{
		fbx_animCurve_scale_x = fbx_node->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
		fbx_animCurve_scale_y = fbx_node->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
		fbx_animCurve_scale_z = fbx_node->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
	}

	if (fbx_animCurve_translation_x)
		fbx_animCurve_translation_x->KeyModifyBegin();
	if (fbx_animCurve_translation_y)
		fbx_animCurve_translation_y->KeyModifyBegin();
	if (fbx_animCurve_translation_z)
		fbx_animCurve_translation_z->KeyModifyBegin();
	if (fbx_animCurve_rotation_x)
		fbx_animCurve_rotation_x->KeyModifyBegin();
	if (fbx_animCurve_rotation_y)
		fbx_animCurve_rotation_y->KeyModifyBegin();
	if (fbx_animCurve_rotation_z)
		fbx_animCurve_rotation_z->KeyModifyBegin();
	if (fbx_animCurve_scale_x)
		fbx_animCurve_scale_x->KeyModifyBegin();
	if (fbx_animCurve_scale_y)
		fbx_animCurve_scale_y->KeyModifyBegin();
	if (fbx_animCurve_scale_z)
		fbx_animCurve_scale_z->KeyModifyBegin();

	float px, py, pz, pw, rx, ry, rz, rw, sx, sy, sz, sw;
	float frame;
	FbxTime lTime;
	FbxMatrix anim_matrix;
	int lKeyIndex = 0;

	size_t frame_count = animation->getFrameCount();
	for (size_t i = 0; i < frame_count; i++)
	{
		frame = (float)i / fps;
		px = py = pz = rx = ry = rz = 0.0f;
		pw = rw = sx = sy = sz = sw = 1.0f;

		FbxVector4 translate = pos_tmp;
		FbxQuaternion rotation = rot_tmp;
		FbxVector4 scale = scale_tmp;

		if (anim_node->getInterpolatedFrame(i, LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION, px, py, pz, pw))
			translate = FbxVector4(px, py, pz, pw);

		if (anim_node->getInterpolatedFrame(i, LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION, rx, ry, rz, rw))
			rotation = FbxQuaternion(rx, ry, rz, rw);

		if (anim_node->getInterpolatedFrame(i, LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE, sx, sy, sz, sw))
			scale = FbxVector4(sx, sy, sz, sw);

		if ((pw != 1.0f) && (pw != 0.0f))
			translate /= pw;
		translate[3] = 1.0;

		if ((sw != 1.0f) && (sw != 0.0f))
			scale /= sw;
		scale[3] = 1.0;



		FbxDouble3 test = giveAngleOrientationForThisOrientationTaitBryan(FbxVector4(rotation[0], rotation[1], rotation[2], rotation[3]));

		rotation_eulerAngles[1] = test[0];			//lacet sur Y
		rotation_eulerAngles[2] = test[1];			//tangage sur Z
		rotation_eulerAngles[0] = test[2];			//roulis sur X		=>YZX order





		//fbx part
		lTime.SetSecondDouble(frame);


		//position
		if (fbx_animCurve_translation_x)
		{
			lKeyIndex = fbx_animCurve_translation_x->KeyAdd(lTime);
			fbx_animCurve_translation_x->KeySetValue(lKeyIndex, (float)(translate[0]));
			fbx_animCurve_translation_x->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
		}
		if (fbx_animCurve_translation_y)
		{
			lKeyIndex = fbx_animCurve_translation_y->KeyAdd(lTime);
			fbx_animCurve_translation_y->KeySetValue(lKeyIndex, (float)(translate[1]));
			fbx_animCurve_translation_y->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
		}

		if (fbx_animCurve_translation_z)
		{
			lKeyIndex = fbx_animCurve_translation_z->KeyAdd(lTime);
			fbx_animCurve_translation_z->KeySetValue(lKeyIndex, (float)(translate[2]));
			fbx_animCurve_translation_z->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
		}

		//rotation
		if (fbx_animCurve_rotation_x)
		{
			lKeyIndex = fbx_animCurve_rotation_x->KeyAdd(lTime);
			fbx_animCurve_rotation_x->KeySetValue(lKeyIndex, (float)(rotation_eulerAngles[0]));
			fbx_animCurve_rotation_x->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
		}
		if (fbx_animCurve_rotation_y)
		{
			lKeyIndex = fbx_animCurve_rotation_y->KeyAdd(lTime);
			fbx_animCurve_rotation_y->KeySetValue(lKeyIndex, (float)(rotation_eulerAngles[1]));
			fbx_animCurve_rotation_y->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
		}
		if (fbx_animCurve_rotation_z)
		{
			lKeyIndex = fbx_animCurve_rotation_z->KeyAdd(lTime);
			fbx_animCurve_rotation_z->KeySetValue(lKeyIndex, (float)(rotation_eulerAngles[2]));
			fbx_animCurve_rotation_z->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
		}



		//scale
		if (fbx_animCurve_scale_x)
		{
			lKeyIndex = fbx_animCurve_scale_x->KeyAdd(lTime);
			fbx_animCurve_scale_x->KeySetValue(lKeyIndex, (float)(scale[0]));
			fbx_animCurve_scale_x->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
		}
		if (fbx_animCurve_scale_y)
		{
			lKeyIndex = fbx_animCurve_scale_y->KeyAdd(lTime);
			fbx_animCurve_scale_y->KeySetValue(lKeyIndex, (float)(scale[1]));
			fbx_animCurve_scale_y->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
		}
		if (fbx_animCurve_scale_z)
		{
			lKeyIndex = fbx_animCurve_scale_z->KeyAdd(lTime);
			fbx_animCurve_scale_z->KeySetValue(lKeyIndex, (float)(scale[2]));
			fbx_animCurve_scale_z->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
		}
	}


	if (fbx_animCurve_translation_x)
	{
		fbx_animCurve_translation_x->KeyModifyEnd();
		fbx_animCurve_translation_x->ConnectSrcObject(fbx_animCurveNode);
	}
	if (fbx_animCurve_translation_y)
	{
		fbx_animCurve_translation_y->KeyModifyEnd();
		fbx_animCurve_translation_y->ConnectSrcObject(fbx_animCurveNode);
	}
	if (fbx_animCurve_translation_z)
	{
		fbx_animCurve_translation_z->KeyModifyEnd();
		fbx_animCurve_translation_z->ConnectSrcObject(fbx_animCurveNode);
	}
	if (fbx_animCurve_rotation_x)
	{
		fbx_animCurve_rotation_x->KeyModifyEnd();
		fbx_animCurve_rotation_x->ConnectSrcObject(fbx_animCurveNode);
	}
	if (fbx_animCurve_rotation_y)
	{
		fbx_animCurve_rotation_y->KeyModifyEnd();
		fbx_animCurve_rotation_y->ConnectSrcObject(fbx_animCurveNode);
	}
	if (fbx_animCurve_rotation_z)
	{
		fbx_animCurve_rotation_z->KeyModifyEnd();
		fbx_animCurve_rotation_z->ConnectSrcObject(fbx_animCurveNode);
	}
	if (fbx_animCurve_scale_x)
	{
		fbx_animCurve_scale_x->KeyModifyEnd();
		fbx_animCurve_scale_x->ConnectSrcObject(fbx_animCurveNode);
	}
	if (fbx_animCurve_scale_y)
	{
		fbx_animCurve_scale_y->KeyModifyEnd();
		fbx_animCurve_scale_y->ConnectSrcObject(fbx_animCurveNode);
	}
	if (fbx_animCurve_scale_z)
	{
		fbx_animCurve_scale_z->KeyModifyEnd();
		fbx_animCurve_scale_z->ConnectSrcObject(fbx_animCurveNode);
	}
	return fbx_animCurveNode;
}



#endif


}