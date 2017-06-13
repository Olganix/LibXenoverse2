namespace LibXenoverse {





/*-------------------------------------------------------------------------------\
|                             EANAnimationNode		                             |
\-------------------------------------------------------------------------------*/
EANAnimationNode::EANAnimationNode(EANAnimationNode *source)
{
	this->bone_index = source->bone_index;
	this->keyframed_animations = source->keyframed_animations;
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void EANAnimationNode::read(File *file, unsigned char index_size, unsigned char keyframe_size)
{
	unsigned int base_animation_node_address = file->getCurrentAddress();

	unsigned short keyframed_animation_count = 0;
	unsigned int keyframed_animation_offset = 0;
	file->readInt16E(&bone_index);
	file->readInt16E(&keyframed_animation_count);
	file->readInt32E(&keyframed_animation_offset);

	LOG_DEBUG("[%i] bone_index : %i, keyframed_animation_count : %i, keyframed_animation_offset : [%i]\n", base_animation_node_address, bone_index, keyframed_animation_count, keyframed_animation_offset);

	//LOG_DEBUG("Reading Animation Node at %d\n", base_animation_node_address);
	//LOG_DEBUG("Animation Node Keyframed Animations (%d) for node %d:\n", keyframed_animation_count, bone_index);
	keyframed_animations.resize(keyframed_animation_count);
	for (size_t i = 0; i < keyframed_animation_count; i++) {
		file->goToAddress(base_animation_node_address + keyframed_animation_offset + i * 4);
		unsigned int address = 0;
		file->readInt32E(&address);
		file->goToAddress(base_animation_node_address + address);

		LOG_DEBUG("-- KFanim %i : [%i] = %i => [%i]\n", i, base_animation_node_address + keyframed_animation_offset + i * 4, address, base_animation_node_address + address);
		keyframed_animations[i].read(file, index_size, keyframe_size);
	}
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
size_t EANAnimationNode::write(File *file, unsigned char index_size, unsigned char keyframe_size)
{
	unsigned int base_animation_node_address = file->getCurrentAddress();

	unsigned short keyframed_animation_count = keyframed_animations.size();
	unsigned int keyframed_animation_offset = 8;			//this header size
	file->writeInt16E(&bone_index);
	file->writeInt16E(&keyframed_animation_count);
	file->writeInt32E(&keyframed_animation_offset);

	LOG_DEBUG("[%i] bone_index : %i, keyframed_animation_count : %i, keyframed_animation_offset : [%i]\n", base_animation_node_address, bone_index, keyframed_animation_count, keyframed_animation_offset);

	size_t keyframedAnim_size = 0;
	for (size_t i = 0; i < keyframed_animation_count; i++)
	{
		file->goToAddress(base_animation_node_address + keyframed_animation_offset + i * 4);
		unsigned int address = keyframed_animation_offset + keyframed_animation_count * 4 + keyframedAnim_size;
		file->writeInt32E(&address);
		file->goToAddress(base_animation_node_address + address);

		LOG_DEBUG("-- KFanim %i : [%i] = %i => [%i]\n", i, base_animation_node_address + keyframed_animation_offset + i * 4, address, base_animation_node_address + address);
		keyframedAnim_size += keyframed_animations[i].write(file, index_size, keyframe_size);
	}

	return (keyframed_animation_offset + keyframed_animation_count * 4 + keyframedAnim_size);
}
/*-------------------------------------------------------------------------------\
|                             haveKeyFrameAnimation	                             |
\-------------------------------------------------------------------------------*/
bool EANAnimationNode::haveKeyFrameAnimation(unsigned int flag)
{
	for (size_t i = 0; i < keyframed_animations.size(); i++)
		if (keyframed_animations[i].getFlag() == flag)
			return true;
	return false;
}
/*-------------------------------------------------------------------------------\
|                             getInterpolatedFrame	                             |
\-------------------------------------------------------------------------------*/
bool EANAnimationNode::getInterpolatedFrame(unsigned int frame, unsigned int flag, float &x, float &y, float &z, float &w)
{
	for (size_t i = 0; i < keyframed_animations.size(); i++)
	{
		if (keyframed_animations.at(i).getFlag() == flag)
		{
			keyframed_animations.at(i).getInterpolatedFrame(frame, x, y, z, w);
			return true;
		}
	}
	return false;
}
/*-------------------------------------------------------------------------------\
|                             addTPoseAnimation		                             |
\-------------------------------------------------------------------------------*/
void EANAnimationNode::addTPoseAnimation(void)
{
	keyframed_animations.push_back(EANKeyframedAnimation(LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION));
	vector<EANKeyframe> &listPos = keyframed_animations.back().getKeyframes();
	listPos.push_back(EANKeyframe(0, 0.0f, 0.0f, 0.0f, 1.0f));
	listPos.push_back(EANKeyframe(1, 0.0f, 0.0f, 0.0f, 1.0f));

	keyframed_animations.push_back(EANKeyframedAnimation(LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION));
	vector<EANKeyframe> &listRot = keyframed_animations.back().getKeyframes();
	listRot.push_back(EANKeyframe(0, 0.0f, 0.0f, 0.0f, 1.0f));
	listRot.push_back(EANKeyframe(1, 0.0f, 0.0f, 0.0f, 1.0f));

	keyframed_animations.push_back(EANKeyframedAnimation(LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE));
	vector<EANKeyframe> &listScale = keyframed_animations.back().getKeyframes();
	listScale.push_back(EANKeyframe(0, 1.0f, 1.0f, 1.0f, 1.0f));
	listScale.push_back(EANKeyframe(1, 1.0f, 1.0f, 1.0f, 1.0f));
}

/*-------------------------------------------------------------------------------\
|                             operator=					                           |
\-------------------------------------------------------------------------------*/
void EANAnimationNode::operator=(EANAnimationNode &source)
{
	this->bone_index = source.bone_index;
	this->keyframed_animations = source.keyframed_animations;
}



#ifdef LIBXENOVERSE_FBX_SUPPORT

/*-------------------------------------------------------------------------------\
|                             importFBXAnimationCurves                           |
\-------------------------------------------------------------------------------*/
size_t EANAnimationNode::importFBXAnimationCurves(FbxNode *lNode, FbxAnimLayer* lAnimLayer, size_t boneIndex)
{
	float fps = 60.0f;
	float intervale_ms = 1000.0f / fps;

	this->bone_index = boneIndex;
		
	FbxAnimCurve* fbx_animCurve_translation_x = lNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
	FbxAnimCurve* fbx_animCurve_translation_y = lNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
	FbxAnimCurve* fbx_animCurve_translation_z = lNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
	FbxAnimCurve* fbx_animCurve_rotation_x = lNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
	FbxAnimCurve* fbx_animCurve_rotation_y = lNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
	FbxAnimCurve* fbx_animCurve_rotation_z = lNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
	FbxAnimCurve* fbx_animCurve_scale_x = lNode->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
	FbxAnimCurve* fbx_animCurve_scale_y = lNode->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
	FbxAnimCurve* fbx_animCurve_scale_z = lNode->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);

	//determine the frame count (must put empty/duplicate datas if there isn't enought frames between them)
	size_t nbframes = 0;
	if ((fbx_animCurve_translation_x) && (fbx_animCurve_translation_x->KeyGetCount() != 0)) nbframes = max(nbframes, 1 + (size_t)round(fbx_animCurve_translation_x->KeyGetTime(fbx_animCurve_translation_x->KeyGetCount() - 1).GetMilliSeconds() / intervale_ms));
	if ((fbx_animCurve_translation_y) && (fbx_animCurve_translation_y->KeyGetCount() != 0)) nbframes = max(nbframes, 1 + (size_t)round(fbx_animCurve_translation_y->KeyGetTime(fbx_animCurve_translation_y->KeyGetCount() - 1).GetMilliSeconds() / intervale_ms));
	if ((fbx_animCurve_translation_z) && (fbx_animCurve_translation_z->KeyGetCount() != 0)) nbframes = max(nbframes, 1 + (size_t)round(fbx_animCurve_translation_z->KeyGetTime(fbx_animCurve_translation_z->KeyGetCount() - 1).GetMilliSeconds() / intervale_ms));
	if ((fbx_animCurve_rotation_x) && (fbx_animCurve_rotation_x->KeyGetCount() != 0)) nbframes = max(nbframes, 1 + (size_t)round(fbx_animCurve_rotation_x->KeyGetTime(fbx_animCurve_rotation_x->KeyGetCount() - 1).GetMilliSeconds() / intervale_ms));
	if ((fbx_animCurve_rotation_y) && (fbx_animCurve_rotation_y->KeyGetCount() != 0)) nbframes = max(nbframes, 1 + (size_t)round(fbx_animCurve_rotation_y->KeyGetTime(fbx_animCurve_rotation_y->KeyGetCount() - 1).GetMilliSeconds() / intervale_ms));
	if ((fbx_animCurve_rotation_z) && (fbx_animCurve_rotation_z->KeyGetCount() != 0)) nbframes = max(nbframes, 1 + (size_t)round(fbx_animCurve_rotation_z->KeyGetTime(fbx_animCurve_rotation_z->KeyGetCount() - 1).GetMilliSeconds() / intervale_ms));
	if ((fbx_animCurve_scale_x) && (fbx_animCurve_scale_x->KeyGetCount() != 0)) nbframes = max(nbframes, 1 + (size_t)round(fbx_animCurve_scale_x->KeyGetTime(fbx_animCurve_scale_x->KeyGetCount() - 1).GetMilliSeconds() / intervale_ms));
	if ((fbx_animCurve_scale_y) && (fbx_animCurve_scale_y->KeyGetCount() != 0)) nbframes = max(nbframes, 1 + (size_t)round(fbx_animCurve_scale_y->KeyGetTime(fbx_animCurve_scale_y->KeyGetCount() - 1).GetMilliSeconds() / intervale_ms));
	if ((fbx_animCurve_scale_z) && (fbx_animCurve_scale_z->KeyGetCount() != 0)) nbframes = max(nbframes, 1 + (size_t)round(fbx_animCurve_scale_z->KeyGetTime(fbx_animCurve_scale_z->KeyGetCount() - 1).GetMilliSeconds() / intervale_ms));

	if ((fbx_animCurve_translation_x) || (fbx_animCurve_translation_y) || (fbx_animCurve_translation_z) )
	{
		EANKeyframedAnimation typedAnim;
		if (typedAnim.importFBXPositionAnimCurve(lNode, nbframes, fbx_animCurve_translation_x, fbx_animCurve_translation_y, fbx_animCurve_translation_z))
			keyframed_animations.push_back(typedAnim);
	}

	if ((fbx_animCurve_rotation_x) || (fbx_animCurve_rotation_y) || (fbx_animCurve_rotation_z))
	{
		EANKeyframedAnimation typedAnim;
		if (typedAnim.importFBXRotationAnimCurve(lNode, nbframes, fbx_animCurve_rotation_x, fbx_animCurve_rotation_y, fbx_animCurve_rotation_z))
			keyframed_animations.push_back(typedAnim);
	}

	if ((fbx_animCurve_scale_x) || (fbx_animCurve_scale_y) || (fbx_animCurve_scale_z))
	{
		EANKeyframedAnimation typedAnim;
		if(typedAnim.importFBXScalingAnimCurve(lNode, nbframes, fbx_animCurve_scale_x, fbx_animCurve_scale_y, fbx_animCurve_scale_z))
			keyframed_animations.push_back(typedAnim);
	}

	return nbframes;
}

#endif


}