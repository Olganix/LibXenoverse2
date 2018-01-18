#include "EMA.h"


namespace LibXenoverse
{


	
/*-------------------------------------------------------------------------------\
|                             readEAN				                             |
\-------------------------------------------------------------------------------*/
void EMA::readEAN(EAN* ean)
{
	name = ean->name;
	unk_08 = ean->unknown_total & 0xffff;
	unk_12 = (ean->unknown_total >> 16) & 0xffff;
	
	readEsk(ean->getSkeleton());
	
	size_t nbElements = ean->animations.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		animations.push_back(EmaAnimation());
		animations.back().readEANAnimation(&ean->animations.at(i), ean->getSkeleton(), this);
	}
}
/*-------------------------------------------------------------------------------\
|                             writeEAN				                             |
\-------------------------------------------------------------------------------*/
void EMA::writeEAN(EAN* ean)
{
	ean->name = name;
	ean->unknown_total = (unsigned int)unk_08 + (((unsigned int)unk_12) << 16);
	
	ESK* skeleton = new ESK();
	writeEsk(skeleton);
	ean->setSkeleton(skeleton);

	size_t nbElements = animations.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		ean->animations.push_back(EANAnimation());
		animations.at(i).writeEANAnimation(&ean->animations.back(), skeleton);
		ean->animations.back().setParent(ean);
	}
}


/*-------------------------------------------------------------------------------\
|                             readEsk				                             |
\-------------------------------------------------------------------------------*/
void EMA::readEsk(ESK* esk)
{
	name = esk->name;

	EskTreeNode* rootNode = esk->getTreeOrganisation();

	vector<EskTreeNode*> newListBonesOrdered;
	rootNode->getTreeBonesList(newListBonesOrdered);					//we get the list by following the hierarchy. it's for not having miss of a parent.

	bones.clear();

	EskTreeNode* node;
	size_t nbBones = newListBonesOrdered.size();
	for (size_t i = 1; i < nbBones; i++)					//we begin by 1 because the first isn't a bone.
	{
		node = newListBonesOrdered.at(i);

		bones.push_back(EMO_Bone());
		bones.back().readESKBone(node->mBone);
	}


	for (size_t i = 1; i < nbBones; i++)					//same
	{
		node = newListBonesOrdered.at(i);

		if ((node->mParent) && (node->mParent->mBone))
		{
			string parentName = node->mParent->mBone->getName();
			for (size_t j = i - 1; j < i; --j)
			{
				if (bones.at(j).GetName() == parentName)
				{
					bones.at(i - 1).parent = &(bones.at(j));
					bones.at(i - 1).sn_u0A[0] = 1;

					if (bones.at(j).child == nullptr)
					{
						bones.at(j).child = &bones.at(i - 1);
					}
					else {

						size_t inc = 0;
						EMO_Bone* childBone = bones.at(j).child;

						while (childBone->sibling != nullptr)
						{
							childBone = childBone->sibling;
							++inc;
						}
						++inc;

						childBone->sibling = &bones.at(i - 1);
						//bones.at(i - 1).indexInChildren = inc;					// apparently with Ema , is all Null = 0xffff.
					}

					break;
				}
			}
		}
	}

}








/*-------------------------------------------------------------------------------\
|                             readEANAnimation		                             |
\-------------------------------------------------------------------------------*/
void EmaAnimation::readEANAnimation(EANAnimation* ean, ESK* esk, EMO_Skeleton* emoSkeleton)
{
	name = ean->name;
	duration = ean->frame_count;
	unk_08 = ean->frame_index_size + (ean->frame_float_size << 16);


	ESKBone* eskBone;
	EMO_Bone* emo_Bone;
	size_t nbElements = ean->nodes.size();
	size_t currentTransform = 0;
	
	for (size_t m = 0; m < 3; m++)								//we pass from position to Orientation to scale.
	{
		switch (m)
		{
		case 0: currentTransform = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION; break;
		case 1: currentTransform = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION; break;
		case 2: currentTransform = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE; break;
		}
		
		
		
		for (size_t i = 0; i < nbElements; i++)						//Command is about a couple Bone+Transform+transformComponent, but EanAnimationNode is only bone, and under, EanKeyframeAnimation is about transform (position, rotation, scale).
		{
			emo_Bone = 0;
			eskBone = esk->getBone(ean->nodes.at(i).bone_index);
			if (eskBone)
				emo_Bone = emoSkeleton->GetBone(eskBone->getName());

			size_t nbKeyFramAnim = ean->nodes.at(i).keyframed_animations.size();
			for (size_t j = 0; j < nbKeyFramAnim; j++)
			{
				if (currentTransform != ean->nodes.at(i).keyframed_animations.at(j).getFlag())			//it will do the transform's order.
					continue;
				
				for (size_t k = 0; k < 3; k++)					//so the part for each component of the transform component.
				{
					commands.push_back(EmaCommand());
					commands.back().bone = emo_Bone;
					commands.back().transformComponent = k;			//0 for X, 1 one Y, 2 for Z.

					commands.back().timesByteSize = ((duration > 0xff) ? 0x20 : 0x0);

					commands.back().readEANKeyframedAnimation(&ean->nodes.at(i).keyframed_animations.at(j), values, k);

					commands.back().indexesByteSize = ((values.size() > 0xffff) ? 0x40 : 0x0);
				}
			}
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             writeEANAnimation		                             |
\-------------------------------------------------------------------------------*/
void EmaAnimation::writeEANAnimation(EANAnimation* ean, ESK* esk)
{
	ean->name = name;
	ean->frame_count = duration;
	ean->frame_index_size = unk_08 & 0xffff;
	ean->frame_float_size = (unk_08 >> 16) & 0xffff;



	//Command is about a couple Bone+Transform+TransformComponent,
	//but EanAnimationNode is only bone, and under, EanKeyframeAnimation is about transform (position, rotation, scale).
	//so the first we have to do is to regroupe command by Bone, transform and transformComponent. withc will easiers to use.
	struct CmdBoneTransformComponent
	{
		size_t transformComponentId;								//0:X, 1:Y, 2:Z 
		EmaCommand* command;
	};
	struct CmdBoneTransform
	{
		size_t transformId;								//0:position, 1:rotation, 2:scale
		std::vector<CmdBoneTransformComponent> list;
	};
	struct CmdBone
	{
		EMO_Bone* emo_Bone;
		std::vector<CmdBoneTransform> list;
	};
	std::vector<CmdBone> listByBone;



	EmaCommand* command;
	CmdBone* cmdBone;
	CmdBoneTransform* cmdBoneTransform;
	CmdBoneTransformComponent* cmdBoneTransformComponent;
	EMO_Bone* emo_Bone;
	EANAnimationNode* node;
	size_t boneIndex;
	size_t nbBones;
	size_t nbTransform;
	size_t nbTransformComponent;
	size_t nbElements = commands.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		command = &(commands.at(i));
		emo_Bone = command->bone;

		cmdBone = 0;
		nbBones = listByBone.size();
		for (size_t j = 0; j < nbBones; j++)
		{
			if (listByBone.at(j).emo_Bone == emo_Bone)
			{
				cmdBone = &(listByBone.at(j));
				break;
			}
		}
		if (!cmdBone)
		{
			listByBone.push_back(CmdBone());
			cmdBone = &listByBone.back();
			cmdBone->emo_Bone = emo_Bone;

			cmdBone->list.push_back(CmdBoneTransform());
			cmdBoneTransform = &cmdBone->list.back();
			cmdBoneTransform->transformId = command->transform;

			cmdBoneTransform->list.push_back(CmdBoneTransformComponent());
			cmdBoneTransformComponent = &cmdBoneTransform->list.back();
			cmdBoneTransformComponent->transformComponentId = command->transformComponent & 0x7;		//0x7 is limiting about 3 component (auras have a 0x9 for 0x1 + a flag at 0x8)
			cmdBoneTransformComponent->command = command;

			continue;
		}


		cmdBoneTransform = 0;
		nbTransform = cmdBone->list.size();
		for (size_t j = 0; j < nbTransform; j++)
		{
			if (cmdBone->list.at(j).transformId == command->transform)
			{
				cmdBoneTransform = &(cmdBone->list.at(j));
				break;
			}
		}
		if (!cmdBoneTransform)
		{
			cmdBone->list.push_back(CmdBoneTransform());
			cmdBoneTransform = &cmdBone->list.back();
			cmdBoneTransform->transformId = command->transform;

			cmdBoneTransform->list.push_back(CmdBoneTransformComponent());
			cmdBoneTransformComponent = &cmdBoneTransform->list.back();
			cmdBoneTransformComponent->transformComponentId = command->transformComponent;
			cmdBoneTransformComponent->command = command;
			continue;
		}


		cmdBoneTransformComponent = 0;
		nbTransformComponent = cmdBoneTransform->list.size();
		for (size_t j = 0; j < nbTransformComponent; j++)
		{
			if (cmdBoneTransform->list.at(j).transformComponentId == command->transformComponent)
			{
				cmdBoneTransformComponent = &(cmdBoneTransform->list.at(j));
				break;
			}
		}
		if (!cmdBoneTransformComponent)
		{
			cmdBoneTransform->list.push_back(CmdBoneTransformComponent());
			cmdBoneTransformComponent = &cmdBoneTransform->list.back();
			cmdBoneTransformComponent->transformComponentId = command->transformComponent;
			cmdBoneTransformComponent->command = command;
			continue;
		}

		//not a good case because there is a duplicate component. TODO add a warning.
		//assert(false);
	}



	//now we could make ean.
	nbBones = listByBone.size();
	for (size_t i = 0; i < nbBones; i++)
	{
		boneIndex = -1;
		cmdBone = &listByBone.at(i);
		emo_Bone = cmdBone->emo_Bone;
		if(emo_Bone)
			boneIndex = esk->getBoneIndex(emo_Bone->GetName());		//Note: if boneIndex = -1 because there is no bone, it still work (ex : for material animation)

		node = 0;
		size_t nbNodes = ean->nodes.size();
		for (size_t j = 0; j < nbNodes; j++)
		{
			if (ean->nodes.at(j).getBoneIndex()== boneIndex)
			{
				node = &ean->nodes.at(j);
				break;
			}
		}
		if (!node)
		{
			ean->nodes.push_back(EANAnimationNode());
			node = &ean->nodes.back();
			node->setBoneIndex(boneIndex);							//not -1 , it's a kind of root.
		}

		nbTransform = cmdBone->list.size();
		for (size_t j = 0; j < nbTransform; j++)
		{
			cmdBoneTransform = &cmdBone->list.at(j);

			size_t flag_tmp = 0;
			switch (cmdBoneTransform->transformId)
			{
				case 0: flag_tmp = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION; break;
				case 1: flag_tmp = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION; break;
				case 2: flag_tmp = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE; break;
				default:
				{
					//todo add a warning.
					//assert(false);
				}
			}


			node->keyframed_animations.push_back(EANKeyframedAnimation());
			EANKeyframedAnimation* eanKFAnim = &node->keyframed_animations.back();
			eanKFAnim->setFlag(flag_tmp);
			

			//here, the trouble is about one animation by channel, so you could not have the same keyframe on all channel.
			//so we have to merge list of frameIndex on each components.
			std::vector<size_t> listIndexMerged;
			nbTransformComponent = cmdBoneTransform->list.size();
			for (size_t k=0; k < nbTransformComponent; k++)
			{
				command = cmdBoneTransform->list.at(k).command;

				size_t nbFrames = command->steps.size();
				for (size_t m=0; m < nbFrames; m++)
				{
					size_t time = command->steps.at(m).time;
					
					size_t nbIndex = listIndexMerged.size();
					bool isfound = false;
					for (size_t n=0; n < nbIndex; n++)
					{
						if (listIndexMerged.at(n)== time)
						{
							isfound = true;
							break;
						}
					}
					if (!isfound)
						listIndexMerged.push_back(time);
				}
			}

			struct TestToOrder { bool operator() (const size_t & a, const size_t & b) const { return ((a == b)||(a < b)); } };
			std::sort(listIndexMerged.begin(), listIndexMerged.end(), TestToOrder());



			size_t nbIndex = listIndexMerged.size();
			for (size_t n = 0; n < nbIndex; n++)
			{
				eanKFAnim->keyframes.push_back(EANKeyframe());
				EANKeyframe* kf = &eanKFAnim->keyframes.back();

				size_t time = listIndexMerged.at(n);
				kf->frame = time;

				for (size_t k=0; k < nbTransformComponent; k++)
				{
					cmdBoneTransformComponent = &cmdBoneTransform->list.at(k);
					cmdBoneTransformComponent->command->writeEANKeyframe(kf, values, time);
				}

				//need to convert Euler Angles to Quaternion
				if (flag_tmp == LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION)
				{
					float rotX = kf->x;
					float rotY = kf->y;
					float rotZ = kf->z;
					
					kf->x = kf->y = kf->z = 0.0;
					kf->w = 1.0;
					
					FbxVector4 quatX = fromAngleAxis(rotX, FbxDouble3(1, 0, 0) );
					FbxVector4 quatY = fromAngleAxis(rotY, FbxDouble3(0, 1, 0));
					FbxVector4 quatZ = fromAngleAxis(rotZ, FbxDouble3(0, 0, 1));

					FbxVector4 quat = quatMulQuat(quatZ, quatMulQuat(quatY, quatX)); //XYZ		=> not like the XYZ order in ean, strange.
					kf->x = (float)quat[0];
					kf->y = (float)quat[1];
					kf->z = (float)quat[2];
					kf->w = (float)quat[3];
				}
			}


			
		}
	}
}









/*-------------------------------------------------------------------------------\
|                             readANKeyframedAnimation                           |
\-------------------------------------------------------------------------------*/
void EmaCommand::readEANKeyframedAnimation(EANKeyframedAnimation* ean, std::vector<float> &values, size_t currComponent)
{
	size_t flag_tmp = 0;
	switch (ean->flag)
	{
		case LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION	: flag_tmp = 0; break;
		case LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION	: flag_tmp = 1; break;
		case LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE	: flag_tmp = 2; break;
		default:
		{
			//todo ajouter un warning.
			//assert(false);
		}
	}

	transform = flag_tmp;


	size_t nbElements = ean->keyframes.size();
	for (size_t i = 0; i < nbElements; i++)						//Command is about a couple Bone+Transform, but EanAnimationNode is only bone, and under, EanKeyframeAnimation is about transform (position, rotation, scale).
	{
		steps.push_back(EmaStep());

		EANKeyframe &kf = ean->keyframes.at(i);

		steps.back().time = kf.frame;
		
		float value = 0.0;
		if (ean->flag != LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION)
		{
			if (currComponent == 0)
				value = kf.x;
			else if (currComponent == 1)
				value = kf.y;
			else if (currComponent == 2)
				value = kf.z;
			else if (currComponent == 3)
				value = kf.w;
		} else {
			

			//Quaternion -> Euler Angle (in real, is TaitBryan angles)
			FbxDouble3 angles = giveAngleOrientationForThisOrientationTaitBryan_XYZ(FbxVector4(kf.x, kf.y, kf.z, kf.w));		// yaws, pitch, roll

			//Xenoverse data is on XYZ order.
			if (currComponent == 0)					//rotation for X axis
				value = (float)angles[2];						//roll
			else if (currComponent == 1)			//for Y axis
				value = (float)angles[0];						//yaw on disc from pitch.
			else if (currComponent == 2)			//for Z axis
				value = (float)angles[1];						//pitch
		}
		
		
		//to reduce the number of Values (goal of this format)
		size_t isfound = (size_t)-1;
		size_t nbValues = values.size();
		for (size_t j = 0; j < nbValues; j++)
		{
			if (values.at(j)== value)
			{
				isfound = j;
				break;
			}
		}
		if (isfound == (size_t)-1)
		{
			isfound = nbValues;
			values.push_back(value);
		}
		
		steps.back().index = isfound;
	}
}
/*-------------------------------------------------------------------------------\
|                             writeEANKeyframedAnimation                         |
\-------------------------------------------------------------------------------*/
void EmaCommand::writeEANKeyframe(EANKeyframe* ean, std::vector<float> &values, size_t frame)
{
	uint8_t transformComponent_tmp = transformComponent & 0x7;
	
	size_t nbElements = steps.size();
	if (nbElements == 0)
	{
		if (transform == 2)				//default for scale
		{
			if (transformComponent_tmp == 0)
				ean->x = 1.0;
			else if (transformComponent_tmp == 1)
				ean->y = 1.0;
			else if (transformComponent_tmp == 2)
				ean->z = 1.0;
			else if (transformComponent_tmp == 3)
				ean->w = 1.0;
			//else
				//assert(false);
		}

		return;
	}

	
	size_t index_p = (size_t)-1;					//prev
	size_t index_n = (size_t)-1;					//next
	float factor = 0.0;								//between prev and next.
	if (frame <= steps.at(0).time)
	{
		index_p = 0;
		index_n = 0;
	}
	if (frame >= steps.back().time)
	{
		index_p = steps.size() - 1;
		index_n = index_p;
	}

	if (index_p == (size_t)-1)
	{
		for (size_t i = 1; i < nbElements; i++)						//Command is about a couple Bone+Transform, but EanAnimationNode is only bone, and under, EanKeyframeAnimation is about transform (position, rotation, scale).
		{
			if (frame == steps.at(i).time)
			{
				index_p = i;
				index_n = i;
				factor = 0.0;
				break;

			} else if (frame < steps.at(i).time) {
				index_p = i-1;
				index_n = i;
				factor = ((float)frame - (float)steps.at(i).time) /((float)steps.at(i+1).time - (float)steps.at(i).time);
				break;
			}
		}
	}

	
	float result = 0.0;

	size_t indexValue_p = steps.at(index_p).index;
	size_t indexValue_n = steps.at(index_n).index;


	size_t nbValues = values.size();
	uint32_t mask = (!this->indexesByteSize) ? 0x3FFF : 0x3FFFFFFF;			//we remove the 2 last bits, seam the solution to remove upper index
	//todo try to find what is about the last 2 bits.

	if (indexValue_p >= nbValues)
	{
		indexValue_p = indexValue_p & mask;

		if (indexValue_p >= nbValues)
			assert(false);
	}
	if (indexValue_n >= nbValues)
	{
		indexValue_n = indexValue_n & mask;

		if (indexValue_n >= nbValues)
			assert(false);
	}

	

	if (indexValue_p == indexValue_n)
	{
		if (indexValue_p >= nbValues)								//saddly , so index are not into array range.
		{
			if (transform == 2)				//default for scale
			{
				if (transformComponent_tmp == 0)
					ean->x = 1.0;
				else if (transformComponent_tmp == 1)
					ean->y = 1.0;
				else if (transformComponent_tmp == 2)
					ean->z = 1.0;
				else if (transformComponent_tmp == 3)
					ean->w = 1.0;
				//else
					//assert(false);
			}

			return;
		}

		result = values.at(indexValue_p);

	} else {

		if (indexValue_n >= nbValues)
		{
			indexValue_n = indexValue_p;
			factor = 0.0;
		}

		if (indexValue_p >= nbValues)
		{
			if (transform == 2)				//default for scale
			{
				if (transformComponent_tmp == 0)
					ean->x = 1.0;
				else if (transformComponent_tmp == 1)
					ean->y = 1.0;
				else if (transformComponent_tmp == 2)
					ean->z = 1.0;
				else if (transformComponent_tmp == 3)
					ean->w = 1.0;
				//else
					//assert(false);
			}
			
			return;
		}

		float value_p = values.at(indexValue_p);
		float value_n = values.at(indexValue_n);

		result = value_p + factor * (value_n - value_p);
	}


	//now, it still to have find the component for 
	if (transformComponent_tmp == 0)
		ean->x = result;
	else if (transformComponent_tmp == 1)
		ean->y = result;
	else if (transformComponent_tmp == 2)
		ean->z = result;
	else if (transformComponent_tmp == 3)
		ean->w = result;
	//else
		//assert(false);


}










void EmaStep::Decompile(TiXmlNode *root, const std::vector<float> &values) const
{
	TiXmlElement *entry_root = new TiXmlElement("Step");

	EMO_BaseFile::WriteParamUnsigned(entry_root, "TIME", time);
	EMO_BaseFile::WriteParamUnsigned(entry_root, "INDEX", index, true);

	size_t index_tmp = index;
	if (index_tmp >= values.size())
		index_tmp = index_tmp & 0x3fffffff;
	if (index_tmp >= values.size())
		index_tmp = index_tmp & 0x3fff;

	if (index_tmp < values.size())
		EMO_BaseFile::WriteParamFloat(entry_root, "TestValues", values.at(index_tmp) );

	root->LinkEndChild(entry_root);
}

bool EmaStep::Compile(const TiXmlElement *root)
{
	uint32_t time;

	if (!EMO_BaseFile::GetParamUnsigned(root, "TIME", &time))
		return false;

	if (time > 65535)
	{
		LOG_DEBUG("%s: time can't be bigger than 65535.\n", FUNCNAME);
		return false;
	}

	this->time = time;

	if (!EMO_BaseFile::GetParamUnsigned(root, "INDEX", &index))
		return false;

	return true;
}

void EmaCommand::Decompile(TiXmlNode *root, uint32_t id, const std::vector<float> &values) const
{
	TiXmlElement *entry_root = new TiXmlElement("Command");
	entry_root->SetAttribute("id", EMO_BaseFile::UnsignedToString(id, true));

	if (bone)
		EMO_BaseFile::WriteParamString(entry_root, "BONE", bone->GetName());

	EMO_BaseFile::WriteParamUnsigned(entry_root, "TRANSFORM", transform, true);
	EMO_BaseFile::WriteParamUnsigned(entry_root, "FLAGS", transformComponent, true);

	for (const EmaStep &step : steps)
	{
		step.Decompile(entry_root, values);
	}

	root->LinkEndChild(entry_root);
}

bool EmaCommand::Compile(const TiXmlElement *root, EMO_Skeleton &skl)
{
	std::string bone;
	uint32_t transform, flags;

	if (skl.GetNumBones() > 0)
	{
		if (!EMO_BaseFile::GetParamString(root, "BONE", bone))
			return false;

		this->bone = skl.GetBone(bone);
		if (!this->bone)
		{
			LOG_DEBUG("%s: Bone \"%s\" doesn't exist.\n", FUNCNAME, bone.c_str());
			return false;
		}
	}
	else
	{
		this->bone = nullptr;
	}

	if (!EMO_BaseFile::GetParamUnsigned(root, "TRANSFORM", &transform))
		return false;

	if (!EMO_BaseFile::GetParamUnsigned(root, "FLAGS", &flags))
		return false;

	if (transform > 255)
	{
		LOG_DEBUG("%s: \"TRANSFORM\" cannot be bigger than 255.\n", FUNCNAME);
		return false;
	}

	if (flags > 255)
	{
		LOG_DEBUG("%s: \"FLAGS\" cannot be bigger than 255.\n", FUNCNAME);
		return false;
	}

	this->transform = transform;
	this->transformComponent = transformComponent;

	for (const TiXmlElement *elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement())
	{
		if (elem->ValueStr() == "Step")
		{
			EmaStep step;

			if (!step.Compile(elem))
				return false;

			steps.push_back(step);
		}
	}

	return true;
}

void EmaAnimation::Decompile(TiXmlNode *root, uint32_t id) const
{
	TiXmlElement *entry_root = new TiXmlElement("Animation");
	entry_root->SetAttribute("id", EMO_BaseFile::UnsignedToString(id, true));
	entry_root->SetAttribute("name", name);

	EMO_BaseFile::WriteParamUnsigned(entry_root, "DURATION", duration);
	EMO_BaseFile::WriteParamUnsigned(entry_root, "U_08", unk_08, true);

	for (size_t i = 0; i < commands.size(); i++)
	{
		commands[i].Decompile(entry_root, i, values);
	}

	EMO_BaseFile::WriteParamMultipleFloats(entry_root, "VALUES", values);
	//EMO_BaseFile::WriteParamBlob(entry_root, "VALUES", (const uint8_t *)values.data(), values.size() * sizeof(float));

	root->LinkEndChild(entry_root);
}

bool EmaAnimation::Compile(const TiXmlElement *root, EMO_Skeleton &skl)
{
	uint32_t duration;

	if (!EMO_BaseFile::ReadAttrString(root, "name", name))
		return false;

	if (!EMO_BaseFile::GetParamUnsigned(root, "DURATION", &duration))
		return false;

	if (duration > 65535)
	{
		LOG_DEBUG("%s: duration cannot be bigger than 65535 (on animation \"%s\")\n", FUNCNAME, name.c_str());
		return false;
	}

	this->duration = duration;

	if (!EMO_BaseFile::GetParamUnsigned(root, "U_08", &unk_08))
		return false;

	size_t count = EMO_BaseFile::GetElemCount(root, "Command");
	if (count > 0)
	{
		std::vector<bool> initialized;

		commands.resize(count);
		initialized.resize(count);

		for (const TiXmlElement *elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement())
		{
			if (elem->ValueStr() == "Command")
			{
				uint32_t id;

				if (!EMO_BaseFile::ReadAttrUnsigned(elem, "id", &id))
				{
					LOG_DEBUG("%s: Cannot read attribute \"id\"\n", FUNCNAME);
					return false;
				}

				if (id >= commands.size())
				{
					LOG_DEBUG("%s: Command id 0x%x out of range.\n", FUNCNAME, id);
					return false;
				}

				if (initialized[id])
				{
					LOG_DEBUG("%s: Command id 0x%x was already specified.\n", FUNCNAME, id);
					return false;
				}

				if (!commands[id].Compile(elem, skl))
				{
					LOG_DEBUG("%s: Compilation of Command failed.\n", FUNCNAME);
					return false;
				}

				initialized[id] = true;
			}
		}
	}

	if (!EMO_BaseFile::GetParamMultipleFloats(root, "VALUES", values))
		return false;

	return true;
}

EMA::EMA()
{
	this->big_endian = false;
}

EMA::~EMA()
{
}

bool EMA::LinkAnimation(EmaAnimation &anim, EMO_Bone **not_found)
{
	if (!HasSkeleton())
		return true;

	EmaAnimation temp = anim;

	for (EmaCommand &c : temp)
	{
		EMO_Bone *b = GetBone(c.bone->GetName());

		if (!b)
		{
			if (not_found)
				*not_found = c.bone;

			return false;
		}

		c.bone = b;
	}

	anim = temp;
	return true;
}

uint16_t EMA::AppendAnimation(const EmaAnimation &animation)
{
	if (HasSkeleton())
	{
		for (const EmaCommand &c : animation.commands)
		{
			if (!BoneExists(c.bone->GetName()))
				return 0xFFFF;
		}
	}

	uint16_t id = animations.size();
	animations.push_back(animation);

	if (HasSkeleton())
	{
		for (EmaCommand &c : animations[id].commands)
		{
			c.bone = GetBone(c.bone->GetName());
		}
	}

	return id;
}

uint16_t EMA::AppendAnimation(const EMA &other, const std::string &name)
{
	const EmaAnimation *animation = other.GetAnimation(name);
	if (!animation)
		return (uint16_t)-1;

	return AppendAnimation(*animation);
}

void EMA::Copy(const EMA &other)
{
	EMO_Skeleton::Copy(other);

	this->animations = other.animations;
	this->unk_08 = other.unk_08;
	this->unk_12 = other.unk_12;

	if (HasSkeleton())
	{
		for (EmaAnimation &a : animations)
		{
			for (EmaCommand &c : a.commands)
			{
				c.bone = GetBone(c.bone->GetName());
				assert(c.bone != nullptr);
			}
		}
	}
}

void EMA::Reset()
{
	EMO_Skeleton::Reset();
	animations.clear();
}

bool EMA::Load(const uint8_t *buf, unsigned int size)
{
	Reset();

	EMAHeader *hdr = (EMAHeader *)buf;
	if (size < sizeof(EMAHeader) || hdr->signature != EMA_SIGNATURE)
		return false;

	this->big_endian = (buf[4] != 0xFE);

	if (hdr->skeleton_offset == 0)
	{
		// No skeleton
	}else{

		if (!EMO_Skeleton::Load(GetOffsetPtr(buf, hdr->skeleton_offset), size - val32(hdr->skeleton_offset)))
			return false;
	}

	unk_08 = val16(hdr->unk_08);
	unk_12 = val16(hdr->unk_12);

	uint32_t *anim_offsets = (uint32_t *)GetOffsetPtr(buf, val16(hdr->header_size), true);
	animations.resize(val16(hdr->anim_count));

	for (size_t i = 0; i < animations.size(); i++)
	{
		EmaAnimation &animation = animations[i];
		EMAAnimationHeader *ahdr = (EMAAnimationHeader *)GetOffsetPtr(buf, anim_offsets, i);

		animation.duration = val16(ahdr->duration);
		animation.name = (char *)GetOffsetPtr(ahdr, ahdr->name_offset) + 11;
		animation.unk_08 = val32(ahdr->unk_08);

		animation.commands.resize(val16(ahdr->cmd_count));

		for (size_t j = 0; j < animation.commands.size(); j++)
		{
			EmaCommand &command = animation.commands[j];
			EMAAnimationCommandHeader *chdr = (EMAAnimationCommandHeader *)GetOffsetPtr(ahdr, ahdr->cmd_offsets, j);

			if (HasSkeleton())
			{

				if (val16(chdr->bone_idx) >= GetNumBones())
				{
					LOG_DEBUG("Bone idx 0x%x out of bounds, in animation \"%s\", in command 0x%x\n", chdr->bone_idx, animation.name.c_str(), j);
					return false;
				}

				command.bone = &bones[val16(chdr->bone_idx)];
			}
			else
			{
				command.bone = nullptr;
			}

			command.transformComponent = (chdr->transformComponent & 0xF);
			command.timesByteSize = (chdr->transformComponent & 0x20);
			command.indexesByteSize = (chdr->transformComponent & 0x40);

			command.transform = chdr->transform;
			command.steps.resize(val16(chdr->step_count));

			for (size_t k = 0; k < command.steps.size(); k++)
			{
				EmaStep &step = command.steps[k];

				if(!(chdr->transformComponent & 0x20))
				{
					uint8_t *timing = (uint8_t *)GetOffsetPtr(chdr, sizeof(EMAAnimationCommandHeader), true);
					step.time = timing[k];
				}else{
					uint16_t *timing = (uint16_t *)GetOffsetPtr(chdr, sizeof(EMAAnimationCommandHeader), true);
					step.time = val16(timing[k]);
				}

				if (!(chdr->transformComponent & 0x40))
				{
					uint16_t *indices = (uint16_t *)GetOffsetPtr(chdr, val16(chdr->indices_offset), true);
					step.index = val16(indices[k]);
					
				}else{
					uint32_t *indices = (uint32_t *)GetOffsetPtr(chdr, val16(chdr->indices_offset), true);
					step.index = val32(indices[k]);
				}

				//if (step.index >= ahdr->value_count)
				//	assert(false);
			}
		}

		animation.values.resize(val32(ahdr->value_count));
		float* values = (float *)GetOffsetPtr(ahdr, ahdr->values_offset);
		uint16_t* values_uint6 = (uint16_t*)values;

		for (size_t j = 0; j < animation.values.size(); j++)
		{	
			if (!(animation.unk_08 & 0x10000))
				animation.values[j] = val_float(values[j]);
			else
				animation.values[j] = float16ToFloat(values_uint6[j]);
		}
	}

	return true;
}

unsigned int EMA::CalculateFileSize() const
{
	unsigned int file_size = sizeof(EMAHeader);

	file_size += animations.size() * sizeof(uint32_t);

	for (const EmaAnimation &a : animations)
	{
		if (file_size & 3)
			file_size += (4 - (file_size & 3));

		file_size += sizeof(EMAAnimationHeader) - sizeof(uint32_t);
		file_size += a.commands.size() * sizeof(uint32_t);

		for (const EmaCommand &c : a.commands)
		{
			if (file_size & 3)
				file_size += (4 - (file_size & 3));

			file_size += sizeof(EMAAnimationCommandHeader);

			if (!c.timesByteSize)
				file_size += c.steps.size();					//uint8
			else
				file_size += c.steps.size() * sizeof(uint16_t);

			if (file_size & 3)
				file_size += (4 - (file_size & 3));

			if (!c.indexesByteSize)
				file_size += c.steps.size() * sizeof(uint16_t);
			else
				file_size += c.steps.size() * sizeof(uint32_t);
		}

		if (file_size & 3)
			file_size += (4 - (file_size & 3));

		if (!(a.unk_08 & 0x10000))
			file_size += a.values.size() * sizeof(float);
		else
			file_size += a.values.size() * sizeof(uint16_t);
	}

	if (HasSkeleton())
	{
		if (file_size & 0x3F)
			file_size += (0x40 - (file_size & 0x3F));

		file_size += EMO_Skeleton::CalculateFileSize();
	}

	for (const EmaAnimation &a : animations)
	{
		if (file_size & 3)
			file_size += (4 - (file_size & 3));

		assert(a.name.length() < 256);
		file_size += 11 + a.name.length() + 1;
	}

	return file_size;
}

void EMA::RebuildSkeleton(const std::vector<EMO_Bone *> &old_bones_ptr)
{
	if (!HasSkeleton())
		return;

	EMO_Skeleton::RebuildSkeleton(old_bones_ptr);

	for (EmaAnimation &a : animations)
	{
		for (EmaCommand &c : a.commands)
		{
			uint16_t id = FindBone(old_bones_ptr, c.bone, false);

			if (id == 0xFFFF)
			{
				throw std::runtime_error(std::string(FUNCNAME) + ": bone " + c.bone->GetName() + " is not resolved.\n");
			}

			c.bone = &bones[id];
		}
	}
}

uint8_t *EMA::CreateFile(unsigned int *psize)
{
	unsigned int file_size;
	uint32_t offset;

	file_size = CalculateFileSize();
	uint8_t *buf = new uint8_t[file_size];

	if (!buf)
	{
		LOG_DEBUG("%s: Memory allocation error (0x%x)\n", FUNCNAME, file_size);
		return nullptr;
	}

	memset(buf, 0, file_size);
	assert(animations.size() < 65536);

	EMAHeader *hdr = (EMAHeader *)buf;
	hdr->signature = EMA_SIGNATURE;
	hdr->endianess_check = val16(0xFFFE);
	hdr->header_size = val16(sizeof(EMAHeader));
	hdr->unk_08 = val16(unk_08);
	hdr->anim_count = val16(animations.size());
	hdr->unk_12 = val16(unk_12);

	offset = sizeof(EMAHeader);
	uint32_t *anim_offsets = (uint32_t *)GetOffsetPtr(buf, offset, true);

	offset += animations.size() * sizeof(uint32_t);

	for (size_t i = 0; i < animations.size(); i++)
	{
		if (offset & 3)
			offset += (4 - (offset & 3));

		const EmaAnimation &animation = animations[i];
		EMAAnimationHeader *ahdr = (EMAAnimationHeader *)GetOffsetPtr(buf, offset, true);
		anim_offsets[i] = val32(offset);

		assert(animation.commands.size() < 65536);

		ahdr->duration = val16(animation.duration);
		ahdr->cmd_count = val16(animation.commands.size());
		ahdr->value_count = val32(animation.values.size());
		ahdr->unk_08 = val32(animation.unk_08);

		offset += sizeof(EMAAnimationHeader) - sizeof(uint32_t);
		offset += animation.commands.size() * sizeof(uint32_t);

		for (size_t j = 0; j < animation.commands.size(); j++)
		{
			if (offset & 3)
				offset += (4 - (offset & 3));

			const EmaCommand &command = animation.commands[j];
			EMAAnimationCommandHeader *chdr = (EMAAnimationCommandHeader *)GetOffsetPtr(buf, offset, true);
			ahdr->cmd_offsets[j] = val32(EMO_BaseFile::DifPointer(buf + offset, ahdr));

			assert(command.steps.size() < 65536);

			if (HasSkeleton())
				chdr->bone_idx = val16(BoneToIndex(command.bone));
			else
				chdr->bone_idx = 0;

			chdr->transform = command.transform;
			chdr->transformComponent = command.transformComponent | command.timesByteSize | command.indexesByteSize;
			chdr->step_count = val16(command.steps.size());
			offset += sizeof(EMAAnimationCommandHeader);

			if (!command.timesByteSize)
			{
				uint8_t *timing = GetOffsetPtr(buf, offset, true);
				for (size_t k = 0; k < command.steps.size(); k++)
				{
					assert(command.steps[k].time < 256);
					timing[k] = (uint8_t)command.steps[k].time;
					offset += sizeof(uint8_t);
				}
					
			} else {

				uint16_t *timing = (uint16_t *)GetOffsetPtr(buf, offset, true);
				for (size_t k = 0; k < command.steps.size(); k++)
				{
					timing[k] = val16(command.steps[k].time);
					offset += sizeof(uint16_t);
				}
			}


			if (offset & 3)
				offset += (4 - (offset & 3));

			uint32_t dif = EMO_BaseFile::DifPointer(buf + offset, chdr);
			assert(dif < 65536);

			chdr->indices_offset = val16(dif);

			if (!command.indexesByteSize)
			{
				uint16_t *indices = (uint16_t *)GetOffsetPtr(buf, offset, true);
				for (size_t k = 0; k < command.steps.size(); k++)
				{
					//assert(command.steps[k].index < 65536);
					indices[k] = val16(command.steps[k].index);
					offset += sizeof(uint16_t);
				}

			} else {
				uint32_t *indices = (uint32_t *)GetOffsetPtr(buf, offset, true);

				for (size_t k = 0; k < command.steps.size(); k++)
				{
					indices[k] = val32(command.steps[k].index);
					offset += sizeof(uint32_t);
				}
			}
		}

		if (offset & 3)
			offset += (4 - (offset & 3));

		if (!(animation.unk_08 & 0x10000))
		{
			float *values = (float *)GetOffsetPtr(buf, offset, true);
			ahdr->values_offset = val32(EMO_BaseFile::DifPointer(buf + offset, ahdr));

			for (size_t j = 0; j < animation.values.size(); j++)
			{
				values[j] = val_float(animation.values[j]);
				copy_float(values + j, animation.values[j]);
				offset += sizeof(float);
			}

		}else {

			uint16_t *values = (uint16_t *)GetOffsetPtr(buf, offset, true);
			ahdr->values_offset = val32(EMO_BaseFile::DifPointer(buf + offset, ahdr));

			for (size_t j = 0; j < animation.values.size(); j++)
			{
				copy_float16(values + j, animation.values[j]);
				offset += sizeof(uint16_t);
			}
		}
	}

	if (HasSkeleton())
	{

		if (offset & 0x3F)
			offset += (0x40 - (offset & 0x3F));

		unsigned int skl_size;
		uint8_t *skl = EMO_Skeleton::CreateFile(&skl_size);
		if (!skl)
		{
			delete[] buf;
			return nullptr;
		}

		hdr->skeleton_offset = val32(offset);
		memcpy(buf + offset, skl, skl_size);
		delete[] skl;

		offset += skl_size;
	}

	for (size_t i = 0; i < animations.size(); i++)
	{
		const EmaAnimation &animation = animations[i];
		EMAAnimationHeader *ahdr = (EMAAnimationHeader *)GetOffsetPtr(buf, anim_offsets, i);

		if (offset & 3)
			offset += (4 - (offset & 3));

		ahdr->name_offset = val32(EMO_BaseFile::DifPointer(buf + offset, ahdr));
		offset += 10;

		assert(animation.name.length() < 256);
		buf[offset] = animation.name.length();
		offset++;

		strcpy((char *)buf + offset, animation.name.c_str());
		offset += animation.name.length() + 1;
	}

	assert(offset == file_size);

	*psize = file_size;
	return buf;
}

TiXmlDocument *EMA::Decompile() const
{
	TiXmlDocument *doc = new TiXmlDocument();

	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "utf-8", "");
	doc->LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("EMA");

	EMO_BaseFile::WriteParamUnsigned(root, "U_08", unk_08, true);
	EMO_BaseFile::WriteParamUnsigned(root, "U_12", unk_12, true);

	for (size_t i = 0; i < animations.size(); i++)
	{
		animations[i].Decompile(root, i);
	}

	doc->LinkEndChild(root);

	if (HasSkeleton())
	{
		EMO_Skeleton::Decompile(doc);
	}

	return doc;
}

bool EMA::Compile(TiXmlDocument *doc, bool big_endian)
{
	Reset();
	this->big_endian = big_endian;

	TiXmlHandle handle(doc);

	if (EMO_BaseFile::FindRoot(&handle, "Skeleton"))
	{
		if (!EMO_Skeleton::Compile(doc, big_endian))
			return false;
	}

	const TiXmlElement *root = EMO_BaseFile::FindRoot(&handle, "EMA");

	if (!root)
	{
		LOG_DEBUG("Cannot find\"EMA\" in xml.\n");
		return false;
	}

	uint32_t unk_08, unk_12;

	if (!EMO_BaseFile::GetParamUnsigned(root, "U_08", &unk_08))
		return false;

	if (!EMO_BaseFile::GetParamUnsigned(root, "U_12", &unk_12))
		return false;

	if (unk_08 > 65535)
	{
		LOG_DEBUG("%s: U_08 cannot be bigger than 65535.\n", FUNCNAME);
		return false;
	}

	if (unk_12 > 65535)
	{
		LOG_DEBUG("%s: U_12 cannot be bigger than 65535.\n", FUNCNAME);
		return false;
	}

	this->unk_08 = unk_08;
	this->unk_12 = unk_12;

	size_t count = EMO_BaseFile::GetElemCount(root, "Animation");
	if (count > 0)
	{
		std::vector<bool> initialized;

		animations.resize(count);
		initialized.resize(count);

		for (const TiXmlElement *elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement())
		{
			if (elem->ValueStr() == "Animation")
			{
				uint32_t id;

				if (!EMO_BaseFile::ReadAttrUnsigned(elem, "id", &id))
				{
					LOG_DEBUG("%s: Cannot read attribute \"id\"\n", FUNCNAME);
					return false;
				}

				if (id >= animations.size())
				{
					LOG_DEBUG("%s: Animation id 0x%x out of range.\n", FUNCNAME, id);
					return false;
				}

				if (initialized[id])
				{
					LOG_DEBUG("%s: Animation id 0x%x was already specified.\n", FUNCNAME, id);
					return false;
				}

				if (!animations[id].Compile(elem, *this))
				{
					LOG_DEBUG("%s: Compilation of Animation failed.\n", FUNCNAME);
					return false;
				}

				initialized[id] = true;
			}
		}
	}

	return true;
}

bool EMA::DecompileToFile(const std::string &path, bool show_error, bool build_path)
{
	LOG_DEBUG("Ema is being saved to .xml file. This process may take a while.\n");
	bool ret = EMO_BaseFile::DecompileToFile(path, show_error, build_path);

	if (ret)
	{
		LOG_DEBUG("Ema has been saved to .xml.\n");
	}

	return ret;
}

bool EMA::CompileFromFile(const std::string &path, bool show_error, bool big_endian)
{
	LOG_DEBUG("Ema is being loaded from .xml file. This process may take a while.\n");
	bool ret = EMO_BaseFile::CompileFromFile(path, show_error, big_endian);

	if (ret)
	{
		LOG_DEBUG("Ema has been loaded from .xml.\n");
	}

	return ret;
}

bool EMA::operator==(const EMA &rhs) const
{
	if (this->animations != rhs.animations)
		return false;

	if (this->unk_08 != rhs.unk_08)
		return false;

	if (this->unk_12 != rhs.unk_12)
		return false;

	if (!HasSkeleton())
	{
		if (rhs.HasSkeleton())
			return false;
	}
	else if (!rhs.HasSkeleton())
	{
		return false;
	}

	if (HasSkeleton())
	{
		if (EMO_Skeleton::operator ==(rhs) != true)
			return false;
	}

	return true;
}


}

