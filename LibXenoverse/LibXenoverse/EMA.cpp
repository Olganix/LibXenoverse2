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
	type = ean->getParent()->getType() & 0x1FF;			//todo find a way to have the type (may be the extension of the file)
	frame_float_size = ean->frame_float_size;


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

					commands.back().readEANKeyframedAnimation(&ean->nodes.at(i).keyframed_animations.at(j), values, k, duration);

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
	ean->frame_index_size = 0;
	ean->getParent()->setType(type | 0x400);
	ean->frame_float_size = (unsigned char)frame_float_size;
	



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



	std::vector<bool> isValuesReallyUsed;  //test Todo remove
	isValuesReallyUsed.resize(values.size(), false);



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
					printf("Warning: unknow flag %s (different position/rotation/scale)", EMO_BaseFile::UnsignedToString(flag_tmp, true));
					LibXenoverse::notifyWarning();
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
					cmdBoneTransformComponent->command->writeEANKeyframe(kf, values, time, isValuesReallyUsed);
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

					FbxVector4 quat = quatMulQuat(quatZ, quatMulQuat(quatY, quatX)); //XYZ		=> not like the XYZ order in ean, strange. (but it's the only order solution, others give weird)

					kf->x = (float)quat[0];
					kf->y = (float)quat[1];
					kf->z = (float)quat[2];
					kf->w = (float)quat[3];
				}
			}


			
		}
	}


	
	//test Todo remove
	std::vector<size_t> listUnUsed;

	size_t nbValues = isValuesReallyUsed.size();
	for (size_t i = 0; i < nbValues; i++)
		if (!isValuesReallyUsed.at(i))
			listUnUsed.push_back(i);

	if(listUnUsed.size() != 0)
	{
		int aa = 42;
	}
}









/*-------------------------------------------------------------------------------\
|                             readANKeyframedAnimation                           |
\-------------------------------------------------------------------------------*/
void EmaCommand::readEANKeyframedAnimation(EANKeyframedAnimation* ean, std::vector<float> &values, size_t currComponent, size_t duration)
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
				value = (float)angles[2];					//roll
			else if (currComponent == 1)			//for Y axis
				value = (float)angles[1];					//yaw on disc from pitch.
			else if (currComponent == 2)			//for Z axis
				value = (float)angles[0];					//pitch
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


	//apparently, not having the last keyframe make the game in infinte loop, so we will check for having frame 0 and last from duration
	if (steps.size())
	{
		if (steps.at(0).time != 0)
		{
			steps.insert(steps.begin(), steps.at(0));					//clone the first index
			steps.at(0).time = 0;
		}

		if((duration > 0) && (steps.back().time + 1 != duration))
		{
			steps.push_back(steps.back());					//clone the first index
			steps.back().time = duration - 1;
		}
	}

}
/*-------------------------------------------------------------------------------\
|                             writeEANKeyframedAnimation                         |
\-------------------------------------------------------------------------------*/
void EmaCommand::writeEANKeyframe(EANKeyframe* ean, std::vector<float> &values, size_t frame, std::vector<bool> &isValuesReallyUsed)
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
				factor = ((float)frame - (float)steps.at(i-1).time) /((float)steps.at(i).time - (float)steps.at(i-1).time);
				break;
			}
		}
	}
	
	float result = 0.0;

	size_t indexValue_p = steps.at(index_p).index;
	size_t indexValue_n = steps.at(index_n).index;


	size_t nbValues = values.size();
	uint32_t mask = (!this->indexesByteSize) ? 0x3FFF : 0x3FFFFFFF;			//we remove the 2 last bits, seam the solution to remove upper index.
	
	// 0x4xxx => isQuadraticBezier
	bool isQuadraticBezier_p = false;
	bool isQuadraticBezier_n = false;
	bool isCubicBezier_p = false;
	bool isCubicBezier_n = false;
	if (indexValue_p >= nbValues)
	{
		isQuadraticBezier_p = (indexValue_p & ((!this->indexesByteSize) ? 0x4000 : 0x40000000)) != 0;
		isCubicBezier_p = (indexValue_p & ((!this->indexesByteSize) ? 0x8000 : 0x80000000)) != 0;
		
		indexValue_p = indexValue_p & mask;

		if (indexValue_p >= nbValues)
			assert(false);
	}
	if (indexValue_n >= nbValues)
	{
		isQuadraticBezier_n = (indexValue_n & ((!this->indexesByteSize) ? 0x4000 : 0x40000000)) != 0;
		isCubicBezier_n = (indexValue_p & ((!this->indexesByteSize) ? 0x8000 : 0x80000000)) != 0;
		
		indexValue_n = indexValue_n & mask;

		if (indexValue_n >= nbValues)
			assert(false);
	}

	isValuesReallyUsed.at(indexValue_p) = true;
	if((isQuadraticBezier_p) ||(isCubicBezier_p))
		isValuesReallyUsed.at(indexValue_p + 1) = true;
	if (isCubicBezier_p)
		isValuesReallyUsed.at(indexValue_p + 2) = true;

	isValuesReallyUsed.at(indexValue_n) = true;
	if ((isQuadraticBezier_n)|| (isCubicBezier_n))
		isValuesReallyUsed.at(indexValue_n + 1) = true;
	if (isCubicBezier_n)
		isValuesReallyUsed.at(indexValue_n + 2) = true;


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

		double value_p = (double)values.at(indexValue_p);
		double value_n = (double)values.at(indexValue_n);


		result = (float)(value_p + factor * (value_n - value_p));
		
		if (isQuadraticBezier_p)
		{
			double tangent_p = (double)values.at(indexValue_p + 1);

			result = (float)quadraticBezier(factor, value_p, value_p + tangent_p, value_n);
		}

		if (isCubicBezier_p)
		{
			double tangent_p = (double)values.at(indexValue_p + 1);
			double tangent_n = (double)values.at(indexValue_p + 2);

			result = (float)cubicBezier(factor, value_p, value_p + tangent_p, value_n - tangent_n, value_n);
		}
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
	EMO_BaseFile::WriteParamUnsigned(entry_root, "type", type, true);
	EMO_BaseFile::WriteParamUnsigned(entry_root, "frame_float_size", frame_float_size, true);

	EMO_BaseFile::WriteParamUnsigned(entry_root, "ValuesOffset", debugValuesOffset, true);
	

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

	uint32_t tmp = 0;
	EMO_BaseFile::GetParamUnsigned(root, "type", &tmp);
	type = (uint16_t)tmp;
	EMO_BaseFile::GetParamUnsigned(root, "frame_float_size", &tmp);
	frame_float_size = (uint16_t)tmp;
		

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


	//section 0 - HavokHeader : black/grey
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#232323");			//background color
	listTagColors.back().back().push_back("#FFFFFF");			//font color (just to possibility read from background color)
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#464646");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

																//section 1 - Section0a : red
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC0000");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC7777");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

																//section 2 - Section0b : green
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#00CC00");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#77CC77");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

																//section 3 - Section0b_names : green
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#009900");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#339933");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


																//section 4 - Section1 : bleu
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#0000CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7777CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


																//section 5 - Section2 : yellow
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCC00");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCC77");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


																//section 6 - Section3 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#00CCCC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#77CCCC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

																//section 7 - Section3 indexes : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#009999");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#339999");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


																//section 8 - Section4 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC00CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC77CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


																//section 9 - Section4_Next : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#990099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#993399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f



																//section 10 - Section4_Next_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

																//section 11 - Section4_Next_c : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC0099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC3399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

																//section 12 - Section4_Next_d : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900AA");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933AA");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

																//section 13 - Section4_Next_e : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA0099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA3399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f



																//section 14 - Section4_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#770077");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#773377");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

																//section 15 - Section4_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#550055");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#553355");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f





																//section 16 - Section4_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA00FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA33FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

																//section 17 - Section4_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

																//section 18 - Section4_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7700FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7733FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

																//section 19 - Section4_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#5500FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#5533FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f




																//section 20 - Section5 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCCCC");			//bg
	listTagColors.back().back().push_back("#000000");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AAAAAA");			//bg
	listTagColors.back().back().push_back("#000000");			//f


																//section 21 - Section5_Box : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#999999");			//bg
	listTagColors.back().back().push_back("#000000");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#777777");			//bg
	listTagColors.back().back().push_back("#000000");			//f
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
		animation.type = val32(ahdr->type);
		animation.frame_float_size = val32(ahdr->frame_float_size);

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
				
				//if (step.index >= ahdr->value_count)				//test to recomment
				//	printf("step.index > ahdr->value_count : %s > %s  on %s %s \n", UnsignedToString(step.index, true).c_str(), UnsignedToString(ahdr->value_count, true).c_str(),  animation.name.c_str(), (command.bone ? command.bone->GetName() : "").c_str()  );
			}
		}

		animation.values.resize(val32(ahdr->value_count));
		float* values = (float *)GetOffsetPtr(ahdr, ahdr->values_offset);
		uint16_t* values_uint6 = (uint16_t*)values;

		animation.debugValuesOffset = anim_offsets[i] + ahdr->values_offset;				//test debug Todo remove.

		for (size_t j = 0; j < animation.values.size(); j++)
		{	
			if (animation.frame_float_size==0)
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

		if (a.frame_float_size == 0)
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
				LibXenoverse::notifyError();
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
		LibXenoverse::notifyError();
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
		ahdr->type = val16(animation.type);
		ahdr->frame_float_size = val16(animation.frame_float_size);

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
					assert(command.steps[k].index < 65536);
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

		if (animation.frame_float_size == 0)
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










/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// wxHexEditor coloration ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////




/*-------------------------------------------------------------------------------\
|                             save_Coloration		                             |
\-------------------------------------------------------------------------------*/
void EMA::save_Coloration(string filename, bool show_error)
{
	uint8_t *buf;
	size_t size;

	buf = ReadFile(filename, &size, show_error);
	if (!buf)
		return;

	TiXmlDocument *doc = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	doc->LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("wxHexEditor_XML_TAG");


	TiXmlElement* filename_node = new TiXmlElement("filename");
	EMO_BaseFile::WriteParamString(filename_node, "path", filename);



	write_Coloration(filename_node, buf, size);



	root->LinkEndChild(filename_node);
	doc->LinkEndChild(root);



	delete[] buf;

	doc->SaveFile(filename + ".tags");

	return;
}






/*-------------------------------------------------------------------------------\
|                             write_Coloration				                     |
\-------------------------------------------------------------------------------*/
void EMA::write_Coloration(TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	EMAHeader *hdr = (EMAHeader *)buf;
	if (size < sizeof(EMAHeader) || hdr->signature != EMA_SIGNATURE)
		return;


	std::vector<bool> listBytesAllreadyTagged;
	listBytesAllreadyTagged.resize(size, false);				//to check override of the same byte (overflow)


	size_t idTag = 0;
	size_t offset = 0;
	size_t incSection = 0;
	size_t incParam = 0;

	write_Coloration_Tag("signature", "string", "", offset, 4 * sizeof(char), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);	offset += 4 * sizeof(char);
	write_Coloration_Tag("endianess_check", "uint16_t", "", offset, sizeof(uint16_t), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("header_size", "uint16_t", "", offset, sizeof(uint16_t), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("unk_08", "uint16_t", "", offset, sizeof(uint16_t), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("unk_0A", "uint16_t", "", offset, sizeof(uint16_t), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("skeleton_offset", "uint32_t", "", offset, sizeof(uint32_t), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("anim_count", "uint16_t", "", offset, sizeof(uint16_t), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("unk_14x3", "uint32_t", "", offset, 3 * sizeof(uint32_t), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 3 * sizeof(uint32_t);


	incParam = 0;
	incSection++;
	if (hdr->skeleton_offset)
	{
		write_Coloration_Skeleton(parent, GetOffsetPtr(buf, hdr->skeleton_offset), size - val32(hdr->skeleton_offset), hdr->skeleton_offset, listBytesAllreadyTagged);
	}

	size_t startOffset_animAdress = hdr->header_size;

	uint32_t* anim_offsets = (uint32_t *)GetOffsetPtr(buf, val16(startOffset_animAdress), true);
	animations.resize(val16(hdr->anim_count));

	for (size_t i = 0; i < animations.size(); i++)
	{
		incParam = 0;
		incSection = 2;
		write_Coloration_Tag("offset_animation", "uint32_t", "", startOffset_animAdress + i * sizeof(uint32_t), sizeof(uint32_t), "Animation", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i);

		size_t startAnimOffset = anim_offsets[i];
		offset = startAnimOffset;

		EmaAnimation &animation = animations[i];
		EMAAnimationHeader *ahdr = (EMAAnimationHeader*)GetOffsetPtr(buf, offset);

		incParam = 0;
		incSection++;
		write_Coloration_Tag("duration", "uint16_t", "", offset, sizeof(uint16_t), "EMAAnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("cmd_count", "uint16_t", "", offset, sizeof(uint16_t), "EMAAnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("value_count", "uint32_t", "", offset, sizeof(uint32_t), "EMAAnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_08", "uint32_t", "", offset, sizeof(uint32_t), "EMAAnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("name_offset", "uint32_t", "", offset, sizeof(uint32_t), "EMAAnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("values_offset", "uint32_t", "", offset, sizeof(uint32_t), "EMAAnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		
		incParam = 0;
		incSection++;
		for (size_t j = 0; j < ahdr->cmd_count; j++)
		{
			write_Coloration_Tag("cmd_offsets_"+ std::to_string(i), "uint32_t", "", offset, sizeof(uint32_t), "EMAAnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
		}


		animation.duration = val16(ahdr->duration);
		animation.type = val16(ahdr->type);
		animation.frame_float_size = val16(ahdr->frame_float_size);
		animation.commands.resize(val16(ahdr->cmd_count));

		animation.name = (char *)GetOffsetPtr(buf, startAnimOffset + ahdr->name_offset) + 11;
		write_Coloration_Tag("nbChar", "uint8_t", "", startAnimOffset + ahdr->name_offset + 10, sizeof(uint8_t), "NAME", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("name", "string", "", startAnimOffset + ahdr->name_offset + 11, animation.name.length() + 1, "NAME", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);


		for (size_t j = 0; j < animation.commands.size(); j++)
		{
			size_t startCommandOffset = startAnimOffset + ahdr->cmd_offsets[j];
			offset = startCommandOffset;

			EmaCommand &command = animation.commands[j];
			EMAAnimationCommandHeader *chdr = (EMAAnimationCommandHeader *)GetOffsetPtr(buf, startCommandOffset);

			incParam = 0;
			incSection = 5;
			write_Coloration_Tag("bone_idx", "uint16_t", "", offset, sizeof(uint16_t), "EMAAnimationCommandHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
			write_Coloration_Tag("transform", "uint8_t", "", offset, sizeof(uint8_t), "EMAAnimationCommandHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint8_t);
			write_Coloration_Tag("transformComponent", "uint8_t", "", offset, sizeof(uint8_t), "EMAAnimationCommandHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint8_t);
			write_Coloration_Tag("step_count", "uint16_t", "", offset, sizeof(uint16_t), "EMAAnimationCommandHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
			write_Coloration_Tag("indices_offset", "uint16_t", "", offset, sizeof(uint16_t), "EMAAnimationCommandHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);



			if (HasSkeleton())
			{
				if (val16(chdr->bone_idx) >= GetNumBones())
				{
					LOG_DEBUG("Bone idx 0x%x out of bounds, in animation \"%s\", in command 0x%x\n", chdr->bone_idx, animation.name.c_str(), j);
					return;
				}
				command.bone = &bones[val16(chdr->bone_idx)];
			}else{
				command.bone = nullptr;
			}

			command.transformComponent = (chdr->transformComponent & 0xF);
			command.timesByteSize = (chdr->transformComponent & 0x20);
			command.indexesByteSize = (chdr->transformComponent & 0x40);

			command.transform = chdr->transform;
			command.steps.resize(val16(chdr->step_count));

			
			size_t startOffsetStepTimeIndexes = startCommandOffset + sizeof(EMAAnimationCommandHeader);
			offset = startOffsetStepTimeIndexes;

			incParam = 0;
			incSection++;

			uint8_t* timing_u8 = (uint8_t*)GetOffsetPtr(buf, startOffsetStepTimeIndexes);
			uint16_t* timing_16 = (uint16_t*)GetOffsetPtr(buf, startOffsetStepTimeIndexes);

			for (size_t k = 0; k < command.steps.size(); k++)
			{
				EmaStep &step = command.steps[k];

				if (!(chdr->transformComponent & 0x20))
				{
					step.time = timing_u8[k];
					write_Coloration_Tag("indexForTiming", "uint8_t", "", offset, sizeof(uint8_t), "STEP", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint8_t);
				}else {
					step.time = val16(timing_16[k]);
					write_Coloration_Tag("indexForTiming", "uint16_t", "", offset, sizeof(uint16_t), "STEP", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint16_t);
				}
			}



			size_t startOffsetStepIndexes = startCommandOffset + val16(chdr->indices_offset);
			offset = startOffsetStepIndexes;

			incParam = 0;
			incSection++;

			uint16_t* indices_u16 = (uint16_t*)GetOffsetPtr(buf, startOffsetStepIndexes);
			uint32_t* indices_u32 = (uint32_t*)GetOffsetPtr(buf, startOffsetStepIndexes);

			for (size_t k = 0; k < command.steps.size(); k++)
			{
				EmaStep &step = command.steps[k];

				if (!(chdr->transformComponent & 0x40))
				{
					step.index = val16(indices_u16[k]);
					write_Coloration_Tag("indexForValue", "uint16_t", "", offset, sizeof(uint16_t), "STEP", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint16_t);
				}else {
					step.index = val32(indices_u32[k]);
					write_Coloration_Tag("indexForValue", "uint32_t", "", offset, sizeof(uint32_t), "STEP", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint32_t);
				}
			}
		}




		animation.values.resize(val32(ahdr->value_count));

		size_t startoffsetFloatValues = startAnimOffset + ahdr->values_offset;
		offset = startoffsetFloatValues;

		float* values_32 = (float *)GetOffsetPtr(buf, startoffsetFloatValues);
		uint16_t* values_u16 = (uint16_t*)values_32;

		animation.debugValuesOffset = startoffsetFloatValues;				//test debug Todo remove.

		incParam = 0;
		incSection++;

		for (size_t j = 0; j < animation.values.size(); j++)
		{
			if (animation.frame_float_size == 0)
			{
				animation.values[j] = val_float(values_32[j]);
				write_Coloration_Tag("Float", "uint32_t", "", offset, sizeof(uint32_t), "ListFloatValues", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
			}else {
				animation.values[j] = float16ToFloat(values_u16[j]);
				write_Coloration_Tag("Float", "uint16_t", "", offset, sizeof(uint16_t), "ListFloatValues", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
			}
		}
	}



}





/*-------------------------------------------------------------------------------\
|                             write_Coloration_Skeleton		                     |
\-------------------------------------------------------------------------------*/
void EMA::write_Coloration_Skeleton(TiXmlElement *parent, const uint8_t *buf, size_t size, size_t startOffset_Skeleton, std::vector<bool> &listBytesAllreadyTagged)
{
	size_t idTag = 0;
	size_t offset = 0;
	size_t incSection = 0;
	size_t incParam = 0;

	SkeletonHeader *hdr = (SkeletonHeader *)buf;

	write_Coloration_Tag("node_count", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("unk_02", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("ik_count", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("unk_06", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("start_offset", "uint32_t", "", startOffset_Skeleton + offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("names_offset", "uint32_t", "", startOffset_Skeleton + offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_10_0", "uint32_t", "", startOffset_Skeleton + offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_10_1", "uint32_t", "", startOffset_Skeleton + offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_skd_offset", "uint32_t", "", startOffset_Skeleton + offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("matrix_offset", "uint32_t", "", startOffset_Skeleton + offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("ik_data_offset", "uint32_t", "", startOffset_Skeleton + offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_24_0", "uint32_t", "", startOffset_Skeleton + offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_24_1", "uint32_t", "", startOffset_Skeleton + offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_24_2", "uint32_t", "", startOffset_Skeleton + offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_24_3", "uint32_t", "", startOffset_Skeleton + offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_34_0", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("unk_34_1", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("unk_38_0", "float", "", startOffset_Skeleton + offset, sizeof(float), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_38_1", "float", "", startOffset_Skeleton + offset, sizeof(float), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);



	incParam = 0;
	incSection++;

	uint16_t node_count = val16(hdr->node_count);
	size_t startOffset_data = hdr->ik_data_offset;

	size_t ik_size = 0;
	uint8_t* ik_data;

	if (startOffset_data)
	{
		ik_size = 0;
		for (uint16_t i = 0; i < hdr->ik_count; i++)
			ik_size += val16(*(uint16_t *)(buf + startOffset_data + ik_size + 2));


		ik_data = new uint8_t[ik_size];

		if (this->big_endian == false)
		{
			memcpy(ik_data, buf + startOffset_data, ik_size);
		}
		else {

			const IKEntry *ike_src = (const IKEntry *)(buf + startOffset_data);
			IKEntry *ike_dst = (IKEntry *)ik_data;
			IKEntry *top = (IKEntry *)(ik_data + size);

			while (ike_dst < top)
			{
				ike_dst->unk_00 = val16(ike_src->unk_00);
				ike_dst->entry_size = val16(ike_src->entry_size);
				assert(ike_dst->entry_size == 0x18);
				ike_dst->unk_03 = ike_src->unk_03;
				ike_dst->unk_04 = ike_src->unk_04;
				ike_dst->unk_06 = val16(ike_src->unk_06);
				ike_dst->unk_08[0] = val16(ike_src->unk_08[0]);
				ike_dst->unk_08[1] = val16(ike_src->unk_08[1]);
				assert(ike_src->unk_08[2] == 0 && ike_src->unk_08[3] == 0);
				ike_dst->unk_08[2] = 0;
				ike_dst->unk_08[3] = 0;
				ike_dst->unk_10[0] = val32(ike_src->unk_10[0]);
				ike_dst->unk_10[1] = val32(ike_src->unk_10[1]);
				assert(ike_dst->unk_10[0] == 0x3F000000 && ike_dst->unk_10[1] == 0);
				ike_src++;
				ike_dst++;
			}
		}

		write_Coloration_Tag("Data InverseKinematic", "blob", "", startOffset_Skeleton + startOffset_data, ik_size, "DataIK", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += ik_size;

	}
	else {
		ik_data = nullptr;
		ik_size = 0;
	}

	if (hdr->unk_24[0] != 0 || hdr->unk_24[1] != 0 || hdr->unk_24[2] != 0 || hdr->unk_24[3] != 0)
	{
		LOG_DEBUG("%s: unk_24 not zero as expected.\n", FUNCNAME);
		return;
	}


	uint32_t *names_table = (uint32_t *)GetOffsetPtr(buf, hdr->names_offset);
	SkeletonNode *nodes = (SkeletonNode*)GetOffsetPtr(buf, hdr->start_offset);
	UnkSkeletonData *unks = nullptr;
	MatrixData *matrixes = nullptr;


	incParam = 0;
	incSection++;

	if (hdr->names_offset)
	{
		offset = hdr->names_offset;
		for (size_t i = 0; i < node_count; i++)
		{
			write_Coloration_Tag("nameOffset", "uint32_t", "", startOffset_Skeleton + offset, sizeof(uint32_t), "NameOffset", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		}
	}


	incParam = 0;
	incSection++;
	if (hdr->unk_skd_offset)
	{
		unks = (UnkSkeletonData*)GetOffsetPtr(buf, hdr->unk_skd_offset);

		offset = hdr->unk_skd_offset;
		for (size_t i = 0; i < node_count; i++)
		{
			incParam = 0;
			write_Coloration_Tag("unk_00_0", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "UnkSkeletonData", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			write_Coloration_Tag("unk_00_1", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "UnkSkeletonData", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			write_Coloration_Tag("unk_00_2", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "UnkSkeletonData", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			write_Coloration_Tag("unk_00_3", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "UnkSkeletonData", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		}
	}

	incParam = 0;
	incSection++;
	if (hdr->matrix_offset)
	{
		matrixes = (MatrixData *)GetOffsetPtr(buf, hdr->matrix_offset);

		offset = hdr->matrix_offset;
		for (size_t i = 0; i < 16; i++)
		{
			write_Coloration_Tag("matrix_00", "float", "", startOffset_Skeleton + offset, sizeof(float), "MatrixData", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		}
	}



	for (uint16_t i = 0; i < node_count; i++)
	{
		EMO_Bone bone;

		size_t startSkeletonNode = hdr->start_offset + i * sizeof(SkeletonNode);
		offset = startSkeletonNode;

		incParam = 0;
		incSection = 5;

		write_Coloration_Tag("parent_id", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "SkeletonNode", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("child_id", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "SkeletonNode", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("sibling_id", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "SkeletonNode", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("emgIndex", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "SkeletonNode", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("index_4", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "SkeletonNode", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_0A_0", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "SkeletonNode", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_0A_1", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "SkeletonNode", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_0A_2", "uint16_t", "", startOffset_Skeleton + offset, sizeof(uint16_t), "SkeletonNode", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		for (size_t j = 0; j < 16; j++)
		{
			write_Coloration_Tag("matrix_" + std::to_string(j), "float", "", startOffset_Skeleton + offset, sizeof(float), "SkeletonNode", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		}

		incParam = 0;
		incSection++;

		bone.meta_original_offset = EMO_BaseFile::DifPointer(&nodes[i], buf);
		bone.name = std::string((char *)GetOffsetPtr(buf, names_table, i));
		write_Coloration_Tag("BoneName", "string", "", startOffset_Skeleton + names_table[i], bone.name.length() + 1, "SkeletonName", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += bone.name.length() + 1;
	}
}





/*-------------------------------------------------------------------------------\
|                             write_Coloration_Tag			                     |
\-------------------------------------------------------------------------------*/
void EMA::write_Coloration_Tag(string paramName, string paramType, string paramComment, size_t startOffset, size_t size, string sectionName, TiXmlElement* parent, size_t idTag, size_t sectionIndex, size_t paramIndex, std::vector<bool> &listBytesAllreadyTagged, size_t sectionIndexInList, bool checkAllreadyTaggued)
{
	TiXmlElement* tag_node = new TiXmlElement("TAG");

	tag_node->SetAttribute("id", UnsignedToString(idTag, false));


	TiXmlElement* start_offset_node = new TiXmlElement("start_offset");
	TiXmlText* text = new TiXmlText(std::to_string(startOffset));
	start_offset_node->LinkEndChild(text);
	tag_node->LinkEndChild(start_offset_node);

	TiXmlElement* end_offset_node = new TiXmlElement("end_offset");
	text = new TiXmlText(std::to_string(startOffset + size - 1));
	end_offset_node->LinkEndChild(text);
	tag_node->LinkEndChild(end_offset_node);

	TiXmlElement* text_node = new TiXmlElement("tag_text");
	text = new TiXmlText(sectionName + ((sectionIndexInList != (size_t)-1) ? "[" + std::to_string(sectionIndexInList) + "]" : "") + "." + paramName + " (" + paramType + ") : " + paramComment);
	text_node->LinkEndChild(text);
	tag_node->LinkEndChild(text_node);





	if (listTagColors.size() == 0)
	{
		listTagColors.push_back(std::vector<std::vector<string>>());
		listTagColors.back().push_back(std::vector<string>());
		listTagColors.back().back().push_back("#000000");			//background color
		listTagColors.back().back().push_back("#FFFFFF");			//font color (just to possibility read from background color)
	}

	size_t sectionIndex_tmp = sectionIndex % listTagColors.size();
	std::vector<std::vector<string>> &sectionColorlist = listTagColors.at(sectionIndex_tmp);
	size_t paramIndex_tmp = paramIndex % sectionColorlist.size();
	std::vector<string> &paramColors = sectionColorlist.at(paramIndex_tmp);

	TiXmlElement* font_colour_node = new TiXmlElement("font_colour");
	text = new TiXmlText(paramColors.at(1));
	font_colour_node->LinkEndChild(text);
	tag_node->LinkEndChild(font_colour_node);

	TiXmlElement* bg_colour_node = new TiXmlElement("note_colour");
	text = new TiXmlText(paramColors.at(0));
	bg_colour_node->LinkEndChild(text);
	tag_node->LinkEndChild(bg_colour_node);

	parent->LinkEndChild(tag_node);




	//un check d'overide , pour savoir si des blocks se chevauche.
	size_t index;
	size_t limit = listBytesAllreadyTagged.size();
	for (size_t i = 0; i < size; i++)
	{
		index = startOffset + i;
		if (index >= limit)
		{
			printf("Error on tagID %i : overflow %s >= %s.\n", idTag, UnsignedToString(index, true).c_str(), UnsignedToString(limit, true).c_str());
			LibXenoverse::notifyError();
			continue;
		}

		if (index == 0x11ac)			//for test
			int aa = 42;

		if ((checkAllreadyTaggued) && (listBytesAllreadyTagged.at(index)))
		{
			printf("warning on tagID %i : the byte %s allready taggued, may be a overflow between blocks. Infos : %s. \n", idTag, UnsignedToString(index, true).c_str(), (sectionName + ((sectionIndexInList != (size_t)-1) ? "[" + std::to_string(sectionIndexInList) + "]" : "") + "." + paramName + " (" + paramType + ") : " + paramComment).c_str());
			LibXenoverse::notifyWarning();
		}

		listBytesAllreadyTagged.at(index) = true;
	}
}



}

