namespace LibXenoverse
{



/*-------------------------------------------------------------------------------\
|                             EANAnimation			                             |
\-------------------------------------------------------------------------------*/
EANAnimation::EANAnimation(EAN *v)
{
	name = "unknow";
	parent = v;
	duration = 0;
	frame_float_size = 2;			//default value from Apple ean.
	unknow_0 = 0;
}
/*-------------------------------------------------------------------------------\
|                             EANAnimation			                             |
\-------------------------------------------------------------------------------*/
EANAnimation::EANAnimation(EANAnimation *source, EAN *v)
{
	this->name = source->name;
	this->parent = v;
	this->duration = source->duration;
	this->frame_float_size = source->frame_float_size;
	this->mListFilenameOrigin = source->mListFilenameOrigin;
	this->unknow_0 = source->unknow_0;
	copy(*source, false);						//this kind of copy take care of presence of the bone in skeleton.
}
/*-------------------------------------------------------------------------------\
|                             EANAnimation			                             |
\-------------------------------------------------------------------------------*/
EANAnimation::EANAnimation(EANAnimation *source, vector<string> &listBoneFilterNames, EAN *v)			//version with a bone filter
{
	this->name = source->name;
	this->parent = v;
	this->duration = source->duration;
	this->frame_float_size = source->frame_float_size;
	this->mListFilenameOrigin = source->mListFilenameOrigin;
	this->unknow_0 = source->unknow_0;
	copy(*source, listBoneFilterNames, false);						//this kind of copy take care of presence of the bone in skeleton.
}



/*-------------------------------------------------------------------------------\
|                             readName				                             |
\-------------------------------------------------------------------------------*/
void EANAnimation::readName(File *file)
{
	file->readString(&name);
}
/*-------------------------------------------------------------------------------\
|                             writeName				                             |
\-------------------------------------------------------------------------------*/
void EANAnimation::writeName(File *file)
{
	file->writeString(&name);
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void EANAnimation::read(File *file) 
{
	string filename = nameFromFilename(file->getPath());
	mListFilenameOrigin.push_back(filename);
		
	unsigned int base_animation_address = file->getCurrentAddress();

	uint8_t frame_index_size = (duration >= 256) ? 1 : 0;

	unsigned int nodes_count = 0;
	unsigned int nodes_offset = 0;
	file->readInt16E(&unknow_0);
	file->readUChar(&frame_index_size);
	file->readUChar(&frame_float_size);
	file->readInt32E(&duration);
	file->readInt32E(&nodes_count);
	file->readInt32E(&nodes_offset);

	nodes.resize(nodes_count);
	unsigned int address = 0;
	for (size_t i = 0; i < nodes_count; i++)
	{
		file->goToAddress(base_animation_address + nodes_offset + i * 4);
		file->readInt32E(&address);
		file->goToAddress(base_animation_address + address);

		nodes[i].read(file, frame_index_size, frame_float_size);
	}
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
size_t EANAnimation::write(File *file) 
{
	unsigned int base_animation_address = file->getCurrentAddress();

	uint8_t frame_index_size = ((duration >= 256) ? 1 : 0);

	unsigned int nodes_count = nodes.size();
	unsigned int nodes_offset = 16;				//this header size
	file->writeInt16E(&unknow_0);
	file->writeUChar(&frame_index_size);
	file->writeUChar(&frame_float_size);
	file->writeInt32E(&duration);
	file->writeInt32E(&nodes_count);
	file->writeInt32E(&nodes_offset);

	for (size_t i = 0; i < nodes_count; i++)						//that will clean keyframes up to the limit, and also add keyframe for the first time if missed (else there will have strange effect on animation or infinite loop  (case stages .ema))
		nodes.at(i).cleanAnimationForDuration(duration);

	size_t nodes_size = 0;
	for (size_t i = 0; i < nodes_count; i++)
	{
		file->goToAddress(base_animation_address + nodes_offset + i * 4);
		unsigned int address = nodes_size + nodes_offset + nodes_count * 4;
		file->writeInt32E(&address);
		file->goToAddress(base_animation_address + address);

		nodes_size += nodes[i].write(file, frame_index_size, frame_float_size);
	}
	return (nodes_offset + nodes_count * 4 + nodes_size);
}
/*-------------------------------------------------------------------------------\
|                             cut						                         |
\-------------------------------------------------------------------------------*/
void EANAnimation::cut(size_t indexKfStart, size_t indexKfEnd)
{
	if (duration == 0)
		return;

	if (indexKfStart == (size_t)-1)
		indexKfStart = 0;
	if (indexKfEnd == (size_t)-1)
		indexKfEnd = duration - 1;

	if (indexKfEnd < indexKfStart)
	{
		size_t tmp = indexKfEnd;
		indexKfEnd = indexKfStart;
		indexKfStart = tmp;
	}

	if (indexKfStart >= duration)
		indexKfStart = duration -1;
	if (indexKfEnd >= duration)
		indexKfEnd = duration -1;

	duration = (indexKfEnd - indexKfStart) + 1;

	size_t nbNodes = nodes.size();
	for (size_t i = 0; i < nbNodes; i++)
		nodes.at(i).cut(indexKfStart, indexKfEnd);
}
/*-------------------------------------------------------------------------------\
|                             getFilenameOriginList_str                          |
\-------------------------------------------------------------------------------*/
string EANAnimation::getFilenameOriginList_str(void)
{
	size_t nbelement = mListFilenameOrigin.size();
	string ret = "";
	for (size_t i = 0; i < nbelement; i++)
		ret += ((i != 0) ? ";" : "") + mListFilenameOrigin.at(i);
	return ret;
}
/*-------------------------------------------------------------------------------\
|                             copy					                             |
\-------------------------------------------------------------------------------*/
void EANAnimation::copy(EANAnimation &source, bool keepName)
{
	if (!keepName)
		this->name = source.name;

	this->duration = source.duration;
	this->mListFilenameOrigin = source.mListFilenameOrigin;
		
	ESK* skeleton_dest = this->parent->getSkeleton();
	ESK* skeleton_src = source.parent->getSkeleton();

	if ((!skeleton_dest) || (!skeleton_src))
	{												//default erase, don't care about skeleton matching
		printf("no skeleton informations, by default case erase all nodes. could have weird effect.\n");
		notifyError();
		this->nodes = source.nodes;
		return;
	}


	std::vector<EANAnimationNode> &nodes_dest = this->nodes;
	std::vector<EANAnimationNode> &nodes_src = source.nodes;

	nodes_dest.clear();										//by default animation was cleared.
	printf("destination animation cleared.\n");

	size_t nbBones_dest = skeleton_dest->getBones().size();
	size_t nbBones_src = skeleton_src->getBones().size();
	size_t nbNodes_src = nodes_src.size();
	for (size_t i = 0; i < nbNodes_src; i++)
	{
		size_t index_src = nodes_src.at(i).getBoneIndex();
		if (index_src >= nbBones_src)
			continue;

		string name_src = skeleton_src->getBones().at(index_src)->getName();

		bool isfound = false;
		for (size_t j = 0; j < nbBones_dest; j++)
		{
			if (name_src == skeleton_dest->getBones().at(j)->getName())
			{
				printf("animation for bone %s was found in source animation. copying\n", name_src.c_str());
				nodes_dest.push_back(nodes_src.at(i));
				nodes_dest.back().setBoneIndex(j);
				isfound = true;
				break;
			}
		}

		if (!isfound)
		{
			printf("bone %s wasn't in source animation. skipped\n", name_src.c_str());
			notifyWarning();
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             append				                             |
\-------------------------------------------------------------------------------*/
void EANAnimation::append(EANAnimation &source, bool keepName)
{
	if (!keepName)
		this->name = source.name;

	size_t startFrameForNew = this->duration + 1;
	this->duration += source.duration;

	ESK* skeleton_dest = this->parent->getSkeleton();
	ESK* skeleton_src = source.parent->getSkeleton();

	if ((!skeleton_dest) || (!skeleton_src))
	{												//default erase, don't care about skeleton matching
		printf("no skeleton informations, by default case erase all nodes. could have weird effect.\n");
		notifyError();
		this->nodes = source.nodes;
		return;
	}

	std::vector<EANAnimationNode> &nodes_dest = this->nodes;
	std::vector<EANAnimationNode> &nodes_src = source.nodes;

	size_t nbBones_dest = skeleton_dest->getBones().size();
	size_t nbBones_src = skeleton_src->getBones().size();
	size_t nbNodes_src = nodes_src.size();
	size_t nbNodes_dest = nodes_dest.size();
	for (size_t i = 0; i < nbNodes_src; i++)
	{
		size_t index_src = nodes_src.at(i).getBoneIndex();
		if (index_src >= nbBones_src)
			continue;

		string name_src = skeleton_src->getBones().at(index_src)->getName();

		bool isfound = false;
		for (size_t j = 0; j < nbNodes_dest; j++)
		{
			if (nodes_dest.at(j).getBoneIndex() == index_src)
			{
				printf("animation for bone %s was found in source animation. (append) copying\n", name_src.c_str());
				nodes_dest.at(i).append(nodes_src.at(i), startFrameForNew);
				isfound = true;
				break;
			}
		}


		if (!isfound)
		{
			for (size_t j = 0; j < nbBones_dest; j++)
			{
				if (name_src == skeleton_dest->getBones().at(j)->getName())
				{
					printf("animation for bone %s was found in source animation. (append) copying\n", name_src.c_str());
					nodes_dest.push_back(nodes_src.at(i));
					nodes_dest.back().setBoneIndex(j);

					nodes_dest.at(i).delayTimeFrame(startFrameForNew);

					isfound = true;
					break;
				}
			}
		}

		if (!isfound)
		{
			printf("bone %s wasn't in source animation. skipped\n", name_src.c_str());
			notifyWarning();
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             copy					                             |
\-------------------------------------------------------------------------------*/
void EANAnimation::copy(EANAnimation &source, vector<string> &listBoneFilterNames, bool keepName)
{
	if (!keepName)
		this->name = source.name;

	this->duration = source.duration;
	this->mListFilenameOrigin = source.mListFilenameOrigin;

	ESK* skeleton_dest = this->parent->getSkeleton();
	ESK* skeleton_src = source.parent->getSkeleton();

	if ((!skeleton_dest) || (!skeleton_src))
	{												//default erase, don't care about skeleton matching
		printf("no skeleton informations, by default case erase all nodes. could have weird effect.\n");
		notifyError();
		this->nodes = source.nodes;
		return;
	}


	std::vector<EANAnimationNode> &nodes_dest = this->nodes;
	std::vector<EANAnimationNode> &nodes_src = source.nodes;


	//in us case, We will clear only node of bone name in the list.
	size_t nbBones_dest = skeleton_dest->getBones().size();
	size_t nbBones_src = skeleton_src->getBones().size();
	size_t nbNodes_src = nodes_src.size();
	size_t nbNodes_dest = nodes_dest.size();

	size_t nbNamedBones = listBoneFilterNames.size();
	for (size_t i = 0; i < nbNodes_dest; i++)
	{
		size_t boneIndex_dest = nodes_dest.at(i).getBoneIndex();
		if (boneIndex_dest >= skeleton_dest->getBones().size())
			continue;
		string boneName = skeleton_dest->getBones().at(boneIndex_dest)->getName();

		for (size_t k = 0; k < nbNamedBones; k++)
		{
			if (boneName == listBoneFilterNames.at(k))
			{
				printf("destination animation for bone %s is erased.\n", boneName.c_str());
				nodes_dest.erase(nodes_dest.begin() + i);				//we delete only nodes witch have some bones in filter.
				nbNodes_dest--;
				i--;
				break;
			}
		}
	}
			


	nbNodes_dest = nodes_dest.size();
	for (size_t i = 0; i < nbBones_dest; i++)
	{
		string name_dest = skeleton_dest->getBones().at(i)->getName();

		bool isfound = false;
		for (size_t k = 0; k < nbNamedBones; k++)
		{
			if (name_dest == listBoneFilterNames.at(k))
			{
				isfound = true;
				break;
			}
		}
		if (!isfound)								//we deal with only bones in filter.
			continue;


		isfound = false;
		for (size_t j = 0; j < nbNodes_src; j++)
		{
			size_t boneIndex_src = nodes_src.at(j).getBoneIndex();
			if (boneIndex_src >= skeleton_src->getBones().size())
				continue;

			if (name_dest == skeleton_src->getBones().at(boneIndex_src)->getName())
			{
				printf("animation for bone %s was found in source animation. copying\n", name_dest.c_str());
				nodes_dest.push_back(nodes_src.at(j));
				nodes_dest.back().setBoneIndex(i);
				isfound = true;
				break;
			}
		}
		if (!isfound)
		{
			printf("bone %s wasn't in source animation. skiped\n", name_dest.c_str());
			notifyWarning();
		}
	}	
}
/*-------------------------------------------------------------------------------\
|                             changeBoneIndexes		                             |
\-------------------------------------------------------------------------------*/
void EANAnimation::changeBoneIndexes(const std::vector<std::vector<size_t>> &listMovingindex)
{
	size_t nbChange = listMovingindex.size();
	size_t index = 0;
	for (size_t i = 0; i < nodes.size(); i++)
	{
		index = nodes.at(i).getBoneIndex();
		for (size_t j = 0; j < nbChange; j++)
		{
			if (listMovingindex.at(j).at(0) != index)
				continue;

			
			if (listMovingindex.at(j).at(1) != 65535)
			{
				nodes.at(i).setBoneIndex(listMovingindex.at(j).at(1));
				break;
			}else{										//notify bone have been deleted
				nodes.erase(nodes.begin() + i);			//so erase also its animations.
				i--;
			}
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             addBoneAnimationFromAnotherEan                     |
\-------------------------------------------------------------------------------*/
void EANAnimation::addBoneAnimationFromAnotherEan(EANAnimation &ean_Anim_src, std::vector<std::vector<size_t>> &listRelationIndex, bool matchDuration)
{
	size_t nbChange = listRelationIndex.size();
	size_t indexBoneToCopy = 0;
	size_t nbAnim_src = ean_Anim_src.nodes.size();


	struct InterpolatedKey
	{
		float x;
		float y;
		float z;
		float w;

		InterpolatedKey(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}
	};

	for (size_t j = 0; j < nbChange; j++)
	{
		indexBoneToCopy = listRelationIndex.at(j).at(0);

		for (size_t i = 0; i < nbAnim_src; i++)
		{
			if (ean_Anim_src.nodes.at(i).getBoneIndex() != indexBoneToCopy)
				continue;

			nodes.push_back(ean_Anim_src.nodes.at(i));							//add the animation of the bone to the end of animations
			nodes.back().setBoneIndex( listRelationIndex.at(j).at(1) );			//use the new index of the cloned bone.

			if (!matchDuration)										//resampling to match animation.
				break;
			
			EANAnimationNode &animNode = nodes.back();
					
			size_t target_Duration_inFrames = this->getDuration();
			size_t original_Duration_inFrames = ean_Anim_src.getDuration();

			double factor = ((double)target_Duration_inFrames) / ((double)original_Duration_inFrames);
			printf("Ok. it's about resample %i frames (%f seconds) into %i frames (%f seconds => factor of %f on keys)\nLet's go\n", original_Duration_inFrames, ((double)original_Duration_inFrames)*(1.0 / 60.0), target_Duration_inFrames, ((double)target_Duration_inFrames)*(1.0 / 60.0), factor);

					
			size_t nblastKeyframe = 0;
			vector<LibXenoverse::EANKeyframedAnimation> &listKeyFramesAnim = animNode.getKeyframed_animations();
			size_t nbKeyFramesAnim = listKeyFramesAnim.size();
			for (size_t k = 0; k < nbKeyFramesAnim; k++)
			{
				LibXenoverse::EANKeyframedAnimation &keyframeAnim = listKeyFramesAnim.at(k);

				size_t flag = keyframeAnim.getFlag();



				//first, we have to create interpolation
				std::vector<InterpolatedKey> listInterpolKey;
				float x, y, z, w;
				for (size_t k = 0; k < target_Duration_inFrames; k++)
				{
					animNode.getInterpolatedFrame((size_t)(round(((double)k) / factor)), flag, x, y, z, w);

					listInterpolKey.push_back(InterpolatedKey(x, y, z, w));
				}

				//next, we replace for the flag
				vector<LibXenoverse::EANKeyframe> &listKeyFrames = keyframeAnim.getKeyframes();		//get a copy
				listKeyFrames.clear();

				for (size_t k = 0; k < target_Duration_inFrames; k++)				//change the frame number with the factor
				{
					InterpolatedKey &pos = listInterpolKey.at(k);
					listKeyFrames.push_back(LibXenoverse::EANKeyframe(k, pos.x, pos.y, pos.z, pos.w));
				}
			}
				
			break;
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             addTPoseAnimation							         |
\-------------------------------------------------------------------------------*/
void EANAnimation::addTPoseAnimation(ESK* skeleton, bool addCameraComponent)
{
	std::vector<ESKBone*> bones = skeleton->getBones();
	size_t nbBones = bones.size();
	for (size_t i = 0; i < nbBones; i++)
	{
		nodes.push_back(EANAnimationNode());
		nodes.back().setBoneIndex(i);
		nodes.back().addTPoseAnimation(addCameraComponent);
	}
}
/*-------------------------------------------------------------------------------\
|                             operator=				                             |
\-------------------------------------------------------------------------------*/
void EANAnimation::operator=(EANAnimation &source)
{
	this->name = source.name;
	this->duration = source.duration;
	this->frame_float_size = source.frame_float_size;
	this->mListFilenameOrigin = source.mListFilenameOrigin;
	this->nodes = source.nodes;
}


#ifdef LIBXENOVERSE_FBX_SUPPORT

/*-------------------------------------------------------------------------------\
|                             importFBXAnimation	                             |
\-------------------------------------------------------------------------------*/
void EANAnimation::importFBXAnimation(FbxScene *scene, FbxAnimStack *lAnimStack, ESK* skeleton, bool allowCamera)
{
	FbxAnimLayer* lAnimLayer = (FbxAnimLayer*)lAnimStack->GetMember(0);

	this->name = lAnimStack->GetName();

		

	//Blender change AnimationName, so we need to clean. (SO Interdiction to use '|' in name of Animations)
	size_t lastPipe = this->name.find_last_of('|');
	if (lastPipe != string::npos)
		this->name = this->name.substr(lastPipe + 1);

	if ((this->name.length() > 7) && (this->name.substr(this->name.length() - 7) == "_Layer0"))
		this->name = this->name.substr(0, this->name.length() - 7);

	printf("Reading Animation %s\n", this->name.c_str());


	//Search all node
	FbxStatus lStatus;
	duration = 0;
		
	size_t boneIndex = 0;
	std::vector<ESKBone *> bones = skeleton->getBones();
	size_t nbOriginalBones = bones.size();


	const int lNodeCount = scene->GetSrcObjectCount<FbxNode>();
	for (int lIndex = 0; lIndex < lNodeCount; lIndex++)
	{
		FbxNode *lNode = scene->GetSrcObject<FbxNode>(lIndex);
		if (!lNode)
			continue;
		printf("   Node found: %s", lNode->GetName());

		FbxSkeleton* lSkeleton = lNode->GetSkeleton();
		if (!lSkeleton)
		{
			if ((!allowCamera) || (!lNode->GetCamera()))
			{
				printf("\n");
				continue;
			}
		}
		
		if (lSkeleton)
		{
			printf(" is Skeleton\n");
		}else {
			printf(" is Camera\n");
		}

		//find the right index, by reading previous information from ean original file.
		boneIndex = (size_t)-1;
		for (size_t i = 0; i < nbOriginalBones; i++)
		{
			if (bones.at(i)->getName() == lNode->GetName())
			{
				boneIndex = i;
				break;
			}
		}

		if (boneIndex >= nbOriginalBones)
			continue;

		EANAnimationNode anim_node;
		size_t nbframe = anim_node.importFBXAnimationCurves(lNode, lAnimLayer, boneIndex, scene);
		if (nbframe!=0)												//we don't need empty Node.
			nodes.push_back(anim_node);

		if (nbframe > duration)
			duration = nbframe;
	}


	size_t lastKfTime = (duration >0) ? (duration -1) : 0;
	size_t nbNodes = nodes.size();
	for (size_t i = 0; i < nbNodes; i++)
	{
		vector<EANKeyframedAnimation> &keyframed_animations  = nodes.at(i).getKeyframed_animations();

		ESKBone* bone = bones.at(nodes.at(i).getBoneIndex());


		size_t nbAnim = keyframed_animations.size();
		if (nbAnim != 3)
		{
			bool have_POS = false;
			bool have_ROT = false;
			bool have_SCA = false;
			bool have_CAM = false;
			for (size_t j = 0; j < nbAnim; j++)
			{
				if (keyframed_animations.at(j).getFlag() == LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION)
				{
					have_POS = true;
				}else if (keyframed_animations.at(j).getFlag() == LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION_or_TargetPosition) {
					have_ROT = true;
				}else if (keyframed_animations.at(j).getFlag() == LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE_or_CAMERA) {

					if(!allowCamera)
						have_SCA = true;
					else
						have_CAM = true;
				}
			}

			if (!have_POS)
			{
				keyframed_animations.insert(keyframed_animations.begin(), EANKeyframedAnimation((unsigned int) LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION));
				vector<EANKeyframe> &listPos = keyframed_animations.at(0).getKeyframes();
				//TODO use the default position of bone instead of neutral.
				listPos.push_back(EANKeyframe(0, bone->relativeTransform[0], bone->relativeTransform[1], bone->relativeTransform[2], 1.0f));
				listPos.push_back(EANKeyframe(lastKfTime, bone->relativeTransform[0], bone->relativeTransform[1], bone->relativeTransform[2], 1.0f));
			}
			
			if (!have_ROT)
			{
				keyframed_animations.insert(keyframed_animations.begin()+1, EANKeyframedAnimation(LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION_or_TargetPosition));
				vector<EANKeyframe> &listRot = keyframed_animations.at(1).getKeyframes();
				listRot.push_back(EANKeyframe(0, bone->relativeTransform[4], bone->relativeTransform[5], bone->relativeTransform[6], bone->relativeTransform[7]));
				listRot.push_back(EANKeyframe(lastKfTime, bone->relativeTransform[4], bone->relativeTransform[5], bone->relativeTransform[6], bone->relativeTransform[7]));
			}

			if ((!allowCamera) && (!have_SCA))
			{
				keyframed_animations.insert(keyframed_animations.begin() + 2, EANKeyframedAnimation(LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE_or_CAMERA));
				vector<EANKeyframe> &listScale = keyframed_animations.at(2).getKeyframes();
				listScale.push_back(EANKeyframe(0, bone->relativeTransform[8], bone->relativeTransform[9], bone->relativeTransform[10], 1.0f));
				listScale.push_back(EANKeyframe(lastKfTime, bone->relativeTransform[8], bone->relativeTransform[9], bone->relativeTransform[10], 1.0f));
			}

			if ((allowCamera)&&(!have_CAM))
			{
				keyframed_animations.push_back(EANKeyframedAnimation(LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE_or_CAMERA));
				vector<EANKeyframe> &listScale = keyframed_animations.back().getKeyframes();
				listScale.push_back(EANKeyframe(0, 0.0f, 39.9783516f * 3.14159265358979f / 180.0f, 1.0f, 1.0f));
				listScale.push_back(EANKeyframe(lastKfTime, 0.0f, 39.9783516f * 3.14159265358979f / 180.0f, 1.0f, 1.0f));
			}
		}

	}

	printf("Animation Frames: %d\n", duration);
	printf("Animation Nodes (%d):\n", nodes.size());
}

#endif


}