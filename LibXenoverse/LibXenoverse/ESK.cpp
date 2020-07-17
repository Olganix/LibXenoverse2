


namespace LibXenoverse
{


/*-------------------------------------------------------------------------------\
|                             ESK					                             |
\-------------------------------------------------------------------------------*/
ESK::ESK(ESK* esk)
{
	name = "";
	version = "192.146.0.0";
	unknow_0 = unknow_1 = unknow_2 = 0;
	flag = 0;
	skeletonUniqueId = 0;

	mHaveExtraBytesOnEachBone = true;

	if (esk)
	{
		name = esk->name;
		clone(esk);
	}
}
/*-------------------------------------------------------------------------------\
|                             ESK					                             |
\-------------------------------------------------------------------------------*/
ESK::~ESK(void)
{
	clear();
}





/*-------------------------------------------------------------------------------\
|                             load					                             |
\-------------------------------------------------------------------------------*/
bool ESK::load(string filename)
{
	name = nameFromFilenameNoExtension(filename, true);

	File file(filename, LIBXENOVERSE_FILE_READ_BINARY);
	bool ret = file.valid() && load(&file, name);
	file.close();

	return ret;
}
/*-------------------------------------------------------------------------------\
|                             load					                             |
\-------------------------------------------------------------------------------*/
bool ESK::load(File *file, std::string s_name)
{
	if (!file->valid())
		return false;
		
	name = s_name;
		

	size_t startAdress = file->getCurrentAddress();
	if (!file->readHeader(LIBXENOVERSE_ESK_SIGNATURE))
		return false;

	file->goToAddress(startAdress + 0x6);

	uint16_t header_size;
	file->readInt16E(&header_size);

	version = "";
	uint8_t tmp = 0;
	file->readUChar(&tmp);
	version += ToString((uint32_t)tmp) + ".";
	file->readUChar(&tmp);
	version += ToString((uint32_t)tmp) + ".";
	file->readUChar(&tmp);
	version += ToString((uint32_t)tmp) + ".";
	file->readUChar(&tmp);
	version += ToString((uint32_t)tmp);


	file->readInt32E(&unknow_0);
	
	unsigned int address = 0;
	size_t offset_nextPart = 0;						//only un nsk for target the emd start.

	file->readInt32E(&address);
	file->readInt32E(&offset_nextPart);
	file->readInt32E(&unknow_1);
	file->readInt32E(&unknow_2);

	file->goToAddress(startAdress + address);
	read(file);
		
	return true;
}
/*-------------------------------------------------------------------------------\
|                             save					                             |
\-------------------------------------------------------------------------------*/
void ESK::save(string filename, bool big_endian)
{
	File file(filename, LIBXENOVERSE_FILE_WRITE_BINARY);
	if (file.valid())
	{
		save(&file);
		file.close();
	}
}
/*-------------------------------------------------------------------------------\
|                             save					                             |
\-------------------------------------------------------------------------------*/
void ESK::save(File *file, bool big_endian)
{
	if (!file->valid())
		return;

	//signature
	size_t startAdress = file->getCurrentAddress();
	file->writeHeader(LIBXENOVERSE_ESK_SIGNATURE, big_endian);

	file->goToAddress(startAdress + 6);
	
	uint16_t header_size = 0x1C;
	file->writeInt16E(&header_size);

	if (version.length() == 0)
		version = "0";
	std::vector<string> sv = split(version, '.');
	for (size_t i = 0; i < 4; i++)
	{
		if (i < sv.size())
		{
			uint8_t tmp = std::stoi(sv.at(i));
			file->writeUChar(&tmp);
		}
		else {
			file->writeNull(1);
		}
	}

	file->writeInt32E(&unknow_0);

	unsigned int address = 0x20;
	size_t offset_nextPart = 0;						//only un nsk for target the emd start.

	file->writeInt32E(&address);
	file->writeInt32E(&offset_nextPart);
	file->writeInt32E(&unknow_1);
	file->writeInt32E(&unknow_2);

	file->goToAddress(startAdress + address);

	write(file);
}
/*-------------------------------------------------------------------------------\
|                             getWriteSize			                             |
\-------------------------------------------------------------------------------*/
unsigned int ESK::getWriteSize(bool withAbsoluteMatrix)
{
	size_t bone_count = bones.size();
	unsigned int size = sizeof(ESK_Skeleton_Section) + bone_count * (sizeof(ESK_Bone_Hierarchy) + sizeof(uint32_t));
	
	for (size_t i = 0; i < bone_count; i++)
		size += bones.at(i)->getName().length() + 1;				//name + \0

	if (size % 16 !=0)												//padding.
		size += (16 - (size % 16) );

	size += bone_count * (sizeof(ESK_Bone_Relative_Transform) + ((withAbsoluteMatrix) ? sizeof(ESK_Bone_Absolute_Matrix) : 0));

	size += calculIksize() + ((mHaveExtraBytesOnEachBone) ? (bone_count * sizeof(ESK_Bone_extraInfo)) : 0);

	return size;
}
/*-------------------------------------------------------------------------------\
|                             calculIksize			                             |
\-------------------------------------------------------------------------------*/
size_t ESK::calculIksize()
{
	size_t filesize = 0;
	size_t nbGroup = listInverseKinematic.size();
	if (nbGroup == 0)
		return filesize;

	filesize += sizeof(uint32_t);
	for (size_t i = 0; i < nbGroup; i++)
	{
		Esk_IK_Group &group = listInverseKinematic.at(i);
		size_t nbIk = group.mListIK.size();
		filesize += sizeof(ESK_IK_Group);

		size_t size_tmp = 0;
		for (size_t j = 0; j < nbIk; j++)
		{
			size_t nbBones = group.mListIK.at(j).mListBones.size();
			size_tmp += sizeof(ESK_IK) + nbBones * (sizeof(uint16_t) + sizeof(float));
			if (nbBones % 2 == 0)
				size_tmp += sizeof(uint16_t) + sizeof(float);
		}
		filesize += size_tmp;
	}
	return filesize;
}






/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void ESK::read(File *file)
{
	unsigned int base_skeleton_address = file->getCurrentAddress();
	unsigned short bone_count = 0;
	unsigned int bone_indices_offset = 0;
	unsigned int bone_names_offset = 0;
	unsigned int relativeTransform_offset = 0;
	unsigned int absoluteMatrix_offset = 0;
	unsigned int ik_offset = 0;								//inverse Kinematic
	unsigned int bone_extraInfo_offset = 0;
		
	file->readInt16E(&bone_count);
	file->readInt16E(&flag);		
	file->readInt32E(&bone_indices_offset);
	file->readInt32E(&bone_names_offset);
	file->readInt32E(&relativeTransform_offset);
	file->readInt32E(&absoluteMatrix_offset);
	file->readInt32E(&ik_offset);
	file->readInt32E(&bone_extraInfo_offset);
	file->readInt64E(&skeletonUniqueId);


	bones.resize(bone_count);
	for (size_t i = 0; i < bone_count; i++)
		bones[i] = new ESKBone();

	// Read Bone Indices
	for (size_t i = 0; i < bone_count; i++)
	{
		unsigned int address = 0;
		file->goToAddress(base_skeleton_address + bone_indices_offset + i * 8);

		bones[i]->readIndices(file);
	}

	// Read Bone Names
	for (size_t i = 0; i < bone_count; i++)
	{
		unsigned int address = 0;
		file->goToAddress(base_skeleton_address + bone_names_offset + i*4);
		file->readInt32E(&address);
		file->goToAddress(base_skeleton_address + address);

		bones[i]->readName(file);

		//LOG_DEBUG("bone %i: %s\n", i, bones[i]->getName().c_str() );
	}

	// Read Bone RelativeTransform (position quaternion and scale)
	for (size_t i = 0; i < bone_count; i++)
	{
		unsigned int address = 0;
		file->goToAddress(base_skeleton_address + relativeTransform_offset + i * 48);

		bones[i]->readRelativeTransform(file);
		
		//LOG_DEBUG("bone %i: RelativeTransform\n%s\n", i, bones[i]->getRelativeTransformDebug().c_str());
	}

	// Read Bone absoluteMatrix
	if (absoluteMatrix_offset)
	{
		for (size_t i = 0; i < bone_count; i++)
		{
			unsigned int address = 0;
			file->goToAddress(base_skeleton_address + absoluteMatrix_offset + i * 64);

			bones[i]->readMatrix(file);

			//LOG_DEBUG("bone %i: AbsoluteMatrix\n%s\n", i, bones[i]->getAbsoluteMatrixDebug().c_str());
		}
	}


	//some files have double hierarchy. we have to check if it's the case to tag bones with hierarchyIndex for other conversion.
	if(bone_count>1)
	{
		string name = bones.at(0)->getName();
		size_t isDuplicateFirstBoneName = (size_t)-1;
		for (size_t i = 1; i < bone_count; i++)
		{
			if (bones.at(i)->getName() == name)
			{
				isDuplicateFirstBoneName = i;
				break;
			}
		}

		if (isDuplicateFirstBoneName != (size_t)-1)
		{
			bones.at(0)->hierarchyIndex = LIBXENOVERSE_ESK_BONE_HIERARCHY_FIRST | LIBXENOVERSE_ESK_BONE_HIERARCHY_SECOND;

			for (size_t i = 1; i < bone_count; i++)
			{
				if ((bones.at(i)->parent_index == 0) && (i != isDuplicateFirstBoneName))
					bones.at(i)->hierarchyIndex = (i < isDuplicateFirstBoneName) ? LIBXENOVERSE_ESK_BONE_HIERARCHY_FIRST : LIBXENOVERSE_ESK_BONE_HIERARCHY_SECOND;
			}
		}
	}

	listInverseKinematic.clear();
	if (ik_offset)
	{
		size_t nbBones = bones.size();

		file->goToAddress(base_skeleton_address + ik_offset);
		
		size_t nbIkGroup = 0;
		file->readInt32E(&nbIkGroup);

		bool wrongOldCaseDetected = false;

		uint16_t val_u16;
		float val_f;
		for (size_t i = 0; i < nbIkGroup; i++)
		{
			listInverseKinematic.push_back(Esk_IK_Group());

			size_t start_group_adress = file->getCurrentAddress();

			uint16_t nbIk = 0;
			file->readInt16E(&nbIk);
			uint16_t size_tmp = 0;
			file->readInt16E(&size_tmp);

			for (size_t j = 0; j < nbIk; j++)
			{
				listInverseKinematic.back().mListIK.push_back(Esk_IK_Relation());

				Esk_IK_Relation &ik = listInverseKinematic.back().mListIK.back();

				uint8_t unknow_0 = 0, nbRelations = 0;
				file->readUChar(&unknow_0);
				file->readUChar(&nbRelations);
				ik.unknow_0 = unknow_0;

				for (size_t k = 0; k <= nbRelations; k++)		// >= because of +1 of bone influenced (the first)
				{
					file->readInt16E(&val_u16);
					if (val_u16 < nbBones)
					{
						ik.mListBones.push_back(Esk_IK_Relation::IKR_Bone(bones.at(val_u16)));
					}else {
						wrongOldCaseDetected = true;
						break;
					}
				}
				if (wrongOldCaseDetected)
					break;

				if (nbRelations % 2 != 0)					//padding for 32bits
					file->readInt16E(&val_u16);

				for (size_t k = 0; k <= nbRelations; k++)
				{
					file->readFloat32E(&val_f);
					ik.mListBones.at(k).value = val_f;
				}
			}

			if (wrongOldCaseDetected)
				break;

			file->goToAddress(start_group_adress + size_tmp);
		}

		if (wrongOldCaseDetected)
		{
			listInverseKinematic.clear();
			
			printf("Error on reading IK parts. Did you use a file created with old version of Olganix's Tools ? if yes, try to add IK part from original game's file, or you will not have full functionnalities. Skipped.\n");
			LibXenoverse::notifyError();
		}
	}




	mHaveExtraBytesOnEachBone = false;
	if (bone_extraInfo_offset)										//only camera animation don't have this.
	{
		mHaveExtraBytesOnEachBone = true;
		file->goToAddress(base_skeleton_address + bone_extraInfo_offset);

		unsigned short value;
		for (size_t i = 0; i < bone_count; i++)
		{
			file->readInt16E(&value);
			bones[i]->unk_extraInfo_0 = value;
			file->readInt16E(&value);
			bones[i]->unk_extraInfo_1 = value;
			file->readInt16E(&value);
			bones[i]->unk_extraInfo_2 = value;
			file->readInt16E(&value);
			bones[i]->unk_extraInfo_3 = value;
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             setupBoneIkLinks			                         |
\-------------------------------------------------------------------------------*/
void ESK::setupBoneIkLinks()											//to update Ik_flag on each bone concerned (first of IK)
{
	return;													//To test, for ean, there is not only 0 or 4 (for bone influenced in Ik), there is also 1. Todo remove afeter found what is 0x1. (also for emo and ema)
	
	for (size_t i = 0, nb = bones.size(); i < nb; i++)
		bones.at(i)->ik_flag = 0;

	if (listInverseKinematic.size() == 0)
		return;

	for (size_t j = 0, nb2 = listInverseKinematic.size(); j < nb2; j++)
	{
		Esk_IK_Group &group = listInverseKinematic.at(j);
		for (size_t k = 0, nb3 = group.mListIK.size(); k < nb3; k++)
		{
			Esk_IK_Relation &ik = group.mListIK.at(k);
			if (ik.mListBones.size())
				ik.mListBones.at(0).bone->ik_flag = LIBXENOVERSE_ESK_BONE_HAVE_IK;
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void ESK::write(File *file, bool withAbsoluteMatrix)
{
	setupBoneIkLinks();							//to update Ik_flag on each bone concerned (first of IK)


	unsigned int base_skeleton_address = file->getCurrentAddress();
	unsigned short bone_count = bones.size();
		
	size_t name_size = 0;
	for (size_t i = 0; i < bone_count; i++)
	{
		name_size += bones.at(i)->getName().length() + 1;		// +1 for \0
		withAbsoluteMatrix = withAbsoluteMatrix && (bones.at(i)->haveAbsoluteMatrix);
	}

	unsigned int bone_indices_offset = sizeof(ESK_Skeleton_Section);
	unsigned int bone_names_offset = bone_indices_offset + bone_count * sizeof(ESK_Bone_Hierarchy);
	unsigned int relativeTransform_offset = (unsigned int)ceil((bone_names_offset + bone_count * sizeof(uint32_t) + name_size) / 16.0) * 16;
	size_t offset = (relativeTransform_offset + bone_count * sizeof(ESK_Bone_Relative_Transform));

	unsigned int absoluteMatrix_offset = ((withAbsoluteMatrix) ? offset : 0);
	offset += (withAbsoluteMatrix) ? (bone_count * sizeof(ESK_Bone_Absolute_Matrix)) : 0;

	size_t ikSize = calculIksize();
	unsigned int ik_offset = (ikSize!=0) ? offset : 0;
	offset += ikSize;

	unsigned int bone_extraInfo_offset = (mHaveExtraBytesOnEachBone) ? offset : 0;
	offset += bone_count * sizeof(ESK_Bone_extraInfo);
	


	file->writeInt16E(&bone_count);
	file->writeInt16E(&flag);
	file->writeInt32E(&bone_indices_offset);
	file->writeInt32E(&bone_names_offset);
	file->writeInt32E(&relativeTransform_offset);
	file->writeInt32E(&absoluteMatrix_offset);
	file->writeInt32E(&ik_offset);
	file->writeInt32E(&bone_extraInfo_offset);
	file->writeInt64E(&skeletonUniqueId);

	
	// Write Bone Indices
	for (size_t i = 0; i < bone_count; i++)
	{
		file->goToAddress(base_skeleton_address + bone_indices_offset + i * 8);
		bones[i]->writeIndices(file);
	}

	// Write Bone Names
	name_size = 0;
	for (size_t i = 0; i < bone_count; i++)
	{
		file->goToAddress(base_skeleton_address + bone_names_offset + i * 4);
			
		unsigned int address = bone_names_offset + bone_count * 4 + name_size;
		file->writeInt32E(&address);
		file->goToAddress(base_skeleton_address + address);

		name_size += bones[i]->getName().length() + 1;		//+1 for \0
		bones[i]->writeName(file);
	}
	
	// Write Bone relativeTransform  (position quaternion and scale)
	for (size_t i = 0; i < bone_count; i++)
	{
		file->goToAddress(base_skeleton_address + relativeTransform_offset + i * 48);
		bones[i]->writeRelativeTransform(file);
	}

	// Write Bone absoluteMatrix
	if (absoluteMatrix_offset)
	{
		for (size_t i = 0; i < bone_count; i++)
		{
			file->goToAddress(base_skeleton_address + absoluteMatrix_offset + i * 64);
			bones[i]->writeMatrix(file);
		}
	}

		
	

	if (ik_offset)
	{
		file->goToAddress(base_skeleton_address + ik_offset);

		size_t nbGroup = listInverseKinematic.size();
		file->writeInt32E(&nbGroup);

		for (size_t i = 0; i < nbGroup; i++)
		{
			Esk_IK_Group &group = listInverseKinematic.at(i);

			size_t nbIk = group.mListIK.size();
			file->writeInt16E((unsigned short*)&nbIk);

			size_t size_tmp = sizeof(ESK_IK_Group);
			for (size_t j = 0; j < nbIk; j++)
			{
				size_t nbBones = group.mListIK.at(j).mListBones.size();
				size_tmp += sizeof(ESK_IK) + nbBones * (sizeof(uint16_t) + sizeof(float));
				if (nbBones % 2 == 0)
					size_tmp += sizeof(uint16_t) + sizeof(float);
			}
			file->writeInt16E((unsigned short*)&size_tmp);

			for (size_t j = 0; j < nbIk; j++)
			{
				Esk_IK_Relation &ik = group.mListIK.at(j);
				size_t nbrelations = ik.mListBones.size();
				if (nbrelations > 1)
					nbrelations--;

				file->writeUChar(&ik.unknow_0);
				file->writeUChar((unsigned char*)&nbrelations);

				for (size_t k = 0; k <= nbrelations; k++)
				{
					ESKBone* bone = ik.mListBones.at(k).bone;
						
					size_t isfound = (size_t)-1;
					for (size_t m = 0; m < bone_count; m++)
					{
						if(bones.at(m)== bone)
						{
							isfound = m;
							break;
						}
					}
					if (!isfound)			//secu
						isfound = 0;

					file->writeInt16E((unsigned short*)&isfound);
				}
				if (nbrelations % 2 != 0)				//padding
					file->writeNull(2);

				for (size_t k = 0; k <= nbrelations; k++)
					file->writeFloat32E(&ik.mListBones.at(k).value);

				if (nbrelations % 2 != 0)				//padding
					file->writeNull(4);
			}
		}
	}


	if (mHaveExtraBytesOnEachBone)
	{
		file->goToAddress(base_skeleton_address + bone_extraInfo_offset);

		for (size_t i = 0; i < bone_count; i++)
		{
			file->writeInt16E((unsigned short*)&bones.at(i)->unk_extraInfo_0);
			file->writeInt16E((unsigned short*)&bones.at(i)->unk_extraInfo_1);
			file->writeInt16E((unsigned short*)&bones.at(i)->unk_extraInfo_2);
			file->writeInt16E((unsigned short*)&bones.at(i)->unk_extraInfo_3);
		}
	}

	file->goToEnd();
}
/*-------------------------------------------------------------------------------\
|                             merge					                             |
\-------------------------------------------------------------------------------*/
void ESK::merge(ESK *esk)
{
	if ((!esk) || (esk->bones.size()==0))
		return;

	size_t nbNewBones = esk->bones.size();
	size_t nbBones = this->bones.size();
	size_t old_nbBone = nbBones;

	ESKBone *eskBone = NULL;
	ESKBone *eskNewBone = NULL;
	string name = "";
	bool isfound = false;
	std::vector<size_t> listRefindexInMergedList;
	std::vector<size_t> listRefindexInOldList;
	listRefindexInOldList.resize(nbBones, 0);

	for (size_t i = 0; i < nbNewBones; i++)
	{
		eskBone = esk->bones.at(i);
		name = eskBone->getName();
			

		if (name.substr(0, 4) == "EXP_")				//there is a different name for the same bone in files : in ESK file : bone 0: EXP_APL_000, in EAN file : bone 0: APL_BAS_BONE but it's the same. I will choose EAN definition (complete version of skeleton)
		{
			size_t pos = name.find_first_of('_', 4);
				
			name = name.substr(4, pos - 4) +"_BAS_BONE";					//TODO check with face animations (fce) and custom body part bones (scd).
		}

		//first , we need to compare if this is the same index.
		if ((i < nbBones) && (this->bones.at(i)->getName() == name))				//same name in same index, it's all good.
		{
			LOG_DEBUG("Merge : bone %s have the same index %i, just update matrix\n", name.c_str(), i);
				
			this->bones.at(i)->mergeMatrix(eskBone);		//just check if we could complete transform matrix. (no transform matrix in ean file)
			this->bones.at(i)->mergeFilenameOrigin(eskBone);

			listRefindexInMergedList.push_back(i);
			continue;
		}

		//else
		isfound = false;
		for (size_t j = 0; j < nbBones; j++)
		{
			if (this->bones.at(j)->getName() == name)
			{
				isfound = true;

				LOG_DEBUG("Merge : bone %s have index %i and old index %i, just update matrix\n", name.c_str(), j, i);
				this->bones.at(j)->mergeMatrix(eskBone);
				this->bones.at(j)->mergeFilenameOrigin(eskBone);
				listRefindexInMergedList.push_back(j);

				break;
			}
		}

		if (!isfound)
		{
			LOG_DEBUG("Merge : bone %s with old index %i was not found in previous list, will be created\n", name.c_str(), i);

			eskNewBone = new ESKBone(eskBone);			//notice : parent and chid index will be just copied, we update after, when all new bones are in merged list.
			eskNewBone->name = name;					//for modified version of name.
			this->bones.push_back(eskNewBone);
			listRefindexInOldList.push_back(i);
			listRefindexInMergedList.push_back(nbBones);
			nbBones++;
		}
	}


	//update parent, child and sibling index for new bones.
		

		
	ESKBone *eskParentBone = NULL;

	for (size_t i = old_nbBone; i < nbBones; i++)							//first pass to define index and first child, and old sibling
	{
		eskNewBone = this->bones.at(i);
		eskBone = esk->bones.at( listRefindexInOldList.at(i) );

		//try to find the child in mergedlist
		size_t childIndex = eskBone->child_index;
		if (childIndex < nbNewBones)
		{
			childIndex = listRefindexInMergedList.at(childIndex);
			eskNewBone->child_index = childIndex;
			//TODO check if there not another bones with the same child_index. normally not, but ...
		}else{
			eskNewBone->child_index = 65535;
		}

		//try to find the old sibling in mergedlist
		size_t siblingIndex = eskBone->sibling_index;
		if (siblingIndex < nbNewBones)
		{
			siblingIndex = listRefindexInMergedList.at(siblingIndex);
			eskNewBone->sibling_index = siblingIndex;
			//TODO check if there not another bones with the same sibling_index. normally not, but ...
		}else{
			eskNewBone->child_index = 65535;
		}
	}



	for (size_t i = old_nbBone; i < nbBones; i++)							//second pass to define parent and new sibling (need child updated)
	{
		eskNewBone = this->bones.at(i);
		eskBone = esk->bones.at(listRefindexInOldList.at(i));

		//try to find the parent in mergedlist
		size_t parentIndex = eskBone->parent_index;
		if (parentIndex < nbNewBones)
		{
			parentIndex = listRefindexInMergedList.at(parentIndex);
			eskNewBone->parent_index = parentIndex;

			//when you find it, the goal is to add as a Child
			eskParentBone = this->bones.at(parentIndex);
			if (eskParentBone->child_index >= nbBones)			//if no Children, we add it
			{
				eskParentBone->child_index = i;
			}else{												//if there is children, we follow the pass to add on the last sibling

				size_t index = eskBone->child_index;					
				while ((index < nbBones) && (index!=i))
				{
					eskBone = this->bones.at(index);
					index = eskBone->sibling_index;
				}
				eskBone->sibling_index = i;
			}
		}else{
			eskNewBone->child_index = 65535;
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             merge_byAttach					                 |
\-------------------------------------------------------------------------------*/
void ESK::merge_byAttach(ESK *esk)
{
	if ((!esk) || (esk->bones.size() == 0))
		return;

	EskTreeNode* skeleton_tree = this->getTreeOrganisation();
	EskTreeNode* skeleton_tree_toAdd = esk->getTreeOrganisation();

	size_t nbChilds = skeleton_tree_toAdd->mChildren.size();
	for (size_t i = 0; i < nbChilds; i++)
		merge_byAttach_recursive(skeleton_tree_toAdd->mChildren.at(i), skeleton_tree);

	this->setTreeOrganisation(skeleton_tree);			//apply merging hierarchie
}
/*-------------------------------------------------------------------------------\
|                             merge_byAttach_recursive					         |
\-------------------------------------------------------------------------------*/
void ESK::merge_byAttach_recursive(EskTreeNode* skeleton_tree_toAdd, EskTreeNode* skeleton_tree)
{
	if (skeleton_tree_toAdd->mBone)
	{
		//we search if there is a match
		EskTreeNode* treeNode = skeleton_tree->getBoneWithName( skeleton_tree_toAdd->mBone->getName() );
		
		if((!treeNode)&&(skeleton_tree_toAdd->mParent) && (skeleton_tree_toAdd->mParent->mBone))				//if there isn't match, and we have a parent.
		{
			//we take the parent, we search a match with it, normally, with the recursivity, the bone must exist.
			treeNode = skeleton_tree->getBoneWithName(skeleton_tree_toAdd->mParent->mBone->getName());
			if (treeNode)					//par secu
			{
				std::vector<std::vector<size_t>> aa;
				std::vector<ESKBone*> bb;
				treeNode->addTree(skeleton_tree_toAdd, true, bones.size(), aa, bb, false);	//we will add the recursivity

			}else{				//if we didn't found the parent, we add it to root
				std::vector<std::vector<size_t>> aa;
				std::vector<ESKBone*> bb;
				skeleton_tree->addTree(skeleton_tree_toAdd, true, bones.size(), aa, bb, false);	//we will add the recursivity
			}
		}
	}

	size_t nbChilds = skeleton_tree_toAdd->mChildren.size();
	for (size_t i = 0; i < nbChilds; i++)
		merge_byAttach_recursive(skeleton_tree_toAdd->mChildren.at(i), skeleton_tree);
}
/*-------------------------------------------------------------------------------\
|                             clone						                         |
\-------------------------------------------------------------------------------*/
void ESK::clone(ESK *esk)
{
	clear();									//we are oblige to clear for 2 reasons : first, the function call a clone, but also because of index reference between parent, childs and sibling will not hae any sence after.

	vector<ESKBone*> &src_bones = esk->bones;
	size_t nbBones = src_bones.size();
	for (size_t i = 0; i < nbBones; i++)
		bones.push_back( new ESKBone(src_bones.at(i)) );

	this->version = esk->version;
	this->unknow_0 = esk->unknow_0;
	this->unknow_1 = esk->unknow_1;
	this->unknow_2 = esk->unknow_2;
	this->flag = esk->flag;
	this->skeletonUniqueId = esk->skeletonUniqueId;
	this->mHaveExtraBytesOnEachBone = esk->mHaveExtraBytesOnEachBone;
	this->listInverseKinematic = esk->listInverseKinematic;
}
/*-------------------------------------------------------------------------------\
|                             getListIndexOfChildBones                           |
\-------------------------------------------------------------------------------*/
std::vector<size_t> ESK::getListIndexOfChildBones(size_t index)
{
	std::vector<size_t> listchild;

	size_t nbBones = bones.size();
	if ((index >= nbBones) || (bones.at(index)->child_index == 65535))
		return listchild;


	std::vector<size_t> listChild_AllDescendances;

	listchild.push_back(bones.at(index)->child_index);
	std::vector<size_t> listChild_descendance = getListIndexOfChildBones(listchild.back());
	listChild_AllDescendances.insert(listChild_AllDescendances.end(), listChild_descendance.begin(), listChild_descendance.end());
	ESKBone* child = bones.at(listchild.back());
		

	while (child->sibling_index != 65535)
	{
		listchild.push_back(bones.at(index)->sibling_index);
		listChild_descendance = getListIndexOfChildBones(listchild.back());
		listChild_AllDescendances.insert(listChild_AllDescendances.end(), listChild_descendance.begin(), listChild_descendance.end());
		child = bones.at(listchild.back());
	}

	listchild.insert(listchild.end(), listChild_AllDescendances.begin(), listChild_AllDescendances.end());
	return listchild;
}
/*-------------------------------------------------------------------------------\
|                             getTreeOrganisation	                             |
\-------------------------------------------------------------------------------*/
//
// because some of file have a wrong children_index, we have to use the parent_index as priority for making the hierarchy.
//
EskTreeNode* ESK::getTreeOrganisation(size_t index, EskTreeNode* parent, std::vector<bool>* listAllReadyUsed)
{
	bool haveToClean = false;
	if (listAllReadyUsed == 0)
	{
		haveToClean = true;
		listAllReadyUsed = new std::vector<bool>();
		listAllReadyUsed->resize(bones.size(), false);
	}else {

		if ((index != 0) && (index < bones.size()) && (bones.at(index)->getName() == bones.at(0)->getName()))
			return 0;
	}

	//this recursive fonction will make a tree of pointer to easelly see heirarchy, and after doing some opeartion on that, and use the setTreeOrganisation() to reOrganize correctly
	EskTreeNode* tree = new EskTreeNode((index < bones.size()) ? bones.at(index) : nullptr, (index < bones.size()) ? index : (size_t)-1, parent);
	if (index >= bones.size())
		index = 0xFFFF;							//root node


	std::vector<EskTreeNode*> children;
	EskTreeNode* tree_tmp;
	for (size_t i = 0; i < bones.size(); i++)
	{
		if (bones.at(i)->parent_index == index)
		{
			if (listAllReadyUsed->at(i) == false)
			{
				listAllReadyUsed->at(i) = true;
				tree_tmp = getTreeOrganisation(i, tree, listAllReadyUsed);
				if(tree_tmp)
					children.push_back(tree_tmp);
			}
		}
	}

	//now order the children by sibling.

	//first case, we have a correct child_index, so we try to do as before.
	bool isfound = false;
	ESKBone* bone = 0;
	if ((index != 65535) && (tree->mBone->child_index != 65535))
	{
		bone = bones.at(tree->mBone->child_index);

		size_t nbChild = children.size();
		for (size_t i = 0; i < nbChild; i++)
		{
			if (children.at(i)->mBone == bone)
			{
				tree->mChildren.push_back(children.at(i));
				children.erase(children.begin() + i);
				isfound = true;
				break;
			}
		}

		if (isfound)
		{
			while (bone->sibling_index != 65535)
			{
				bone = bones.at(bone->sibling_index);

				size_t nbChild = children.size();
				for (size_t i = 0; i < nbChild; i++)
				{
					if (children.at(i)->mBone == bone)
					{
						tree->mChildren.push_back(children.at(i));
						children.erase(children.begin() + i);
						break;
					}
				}
			}
		}
	}


	//second case if child_index is incorrect (not part of the children found previously)
	if (!isfound)
	{
		//first we search the last to the first.
		bool isfound = true;
		size_t index_tmp = 65535;

		while (!isfound)
		{
			isfound = false;

			size_t nbChild = children.size();
			for (size_t i = 0; i < nbChild; i++)
			{
				if (children.at(i)->mBone->sibling_index == index_tmp)
				{
					bone = children.at(i)->mBone;
					tree->mChildren.insert(tree->mChildren.begin(), children.at(i));
					children.erase(children.begin() + i);

					size_t nbBones = bones.size();
					for (size_t j = 0; j < nbBones; j++)
					{
						if (bones.at(j) == bone)
						{
							index_tmp = j;
							isfound = true;
							break;
						}
					}

					break;
				}
			}
		}
	}


	//last test , to keep bone missed
	if (children.size() != 0)
	{
		size_t nbChild = children.size();
		for (size_t i = 0; i < nbChild; i++)
			tree->mChildren.push_back(children.at(i));
		children.clear();
	}

	if (haveToClean)
	{
		delete listAllReadyUsed;
		listAllReadyUsed = 0;
	}

	return tree;
}
/*-------------------------------------------------------------------------------\
|                             setTreeOrganisation	                             |
\-------------------------------------------------------------------------------*/
std::vector<std::vector<size_t>> ESK::setTreeOrganisation(EskTreeNode* treeBone)
{
	//the first thing to do is to make the new list bone and refresh index of each bone.
	vector<ESKBone*> newListBones;
	treeBone->setupBones_list(newListBones);

	//the second thing is to refresh parent , child and sibling index for each bone.
	treeBone->setupBones(newListBones);











	if ((newListBones.size()) && (newListBones.at(0)->hierarchyIndex != 0))			//special case with 2 hierarchy. have to insert the first node again, and adapt indexes
	{
		size_t bone_count = newListBones.size();
		size_t startHierarchy2 = (size_t)-1;
		for (size_t i = 0; i < bone_count; i++)
		{
			if (newListBones.at(i)->hierarchyIndex == LIBXENOVERSE_ESK_BONE_HIERARCHY_SECOND)
			{
				startHierarchy2 = i;
				break;
			}
		}


		if (startHierarchy2 != (size_t)-1)
		{
			newListBones.insert(newListBones.begin() + startHierarchy2, new ESKBone(treeBone->mChildren.at(0)->mBone));
			bone_count++;

			if ((newListBones.at(startHierarchy2 + 1)->sibling_index != 0xFFFF) && 
				(newListBones.at(newListBones.at(startHierarchy2 + 1)->sibling_index + 1)->child_index == 0xFFFF) )				//one of only differences between sdbh bckalb00.esk and SCT00_MJEB40.esk 
			{
				newListBones.at(startHierarchy2)->sibling_index = newListBones.at(startHierarchy2 + 1)->sibling_index;
				newListBones.at(startHierarchy2 + 1)->sibling_index = 0xFFFF;
			}
		}else {										//case SFxTk MAD_MAN03.emo, the duplicate is at the end.
			startHierarchy2 = bone_count;
			newListBones.push_back(new ESKBone(treeBone->mChildren.at(0)->mBone));
			bone_count++;
		}

		newListBones.at(startHierarchy2)->parent_index = 0;
		newListBones.at(startHierarchy2)->child_index = (startHierarchy2 + 1 != bone_count) ? startHierarchy2 : 0xFFFF;		// it should be startHierarchy2 + 1, but the +1 come after.

		if (newListBones.at(0)->child_index == startHierarchy2)
			newListBones.at(0)->child_index = 0;


		for (size_t i = 0; i < bone_count; i++)
		{
			ESKBone *bone = newListBones.at(i);
			if ((bone->parent_index != 0xFFFF) && (bone->parent_index >= startHierarchy2))
				bone->parent_index++;

			if ((bone->child_index != 0xFFFF) && (bone->child_index >= startHierarchy2))
				bone->child_index++;

			if (bone->sibling_index != 0xFFFF)
			{
				if (bone->sibling_index == startHierarchy2)
					bone->sibling_index = 0;
				else if (bone->sibling_index > startHierarchy2)
					bone->sibling_index++;
			}
		}	
	}









	//the last thing : compare the two list to delete bones witch are not in tree.
	std::vector<std::vector<size_t>> listMovingindex;								//we also try to give the list of modified index (for animation part by example)
	size_t nbBone_newlist = newListBones.size();
	size_t nbBones_oldList = bones.size();
	bool isFound = false;
	for (size_t i = 0; i < nbBones_oldList; i++)
	{
		ESKBone* bone = bones.at(i);
		isFound = false;

		for (size_t j = 0; j < nbBone_newlist; j++)
		{
			if (newListBones.at(j) == bone)
			{
				isFound = true;
				if (i != j)
				{
					listMovingindex.push_back(std::vector<size_t>());
					listMovingindex.back().push_back(i);
					listMovingindex.back().push_back(j);
				}
				break;
			}
		}

		if (!isFound)
		{
			delete bone;									//delete bone witch is not in previous list.

			listMovingindex.push_back(std::vector<size_t>());
			listMovingindex.back().push_back(i);
			listMovingindex.back().push_back(65535);		//notify it is destroyed
		}
	}




	bones = newListBones;									//replace the old list.
	return listMovingindex;
}
/*-------------------------------------------------------------------------------\
|                             remove				                             |
\-------------------------------------------------------------------------------*/
std::vector<std::vector<size_t>> ESK::remove(size_t index, bool recursive)
{
	//TODO check in relationlist if it's not existe to  also remove it.


	size_t nbBones = bones.size();
	if (index >= nbBones)
	{
		std::vector<std::vector<size_t>> listMovingindex;
		return listMovingindex;
	}

	EskTreeNode* rootNode = getTreeOrganisation();

	EskTreeNode* bone = rootNode->getBoneWithIndex(index);		//find the EskTreeNode with index, 
	if (bone == nullptr)
	{
		std::vector<std::vector<size_t>> listMovingindex;
		return listMovingindex;
	}
		
	EskTreeNode* boneParent = bone->mParent;

	if (boneParent)
	{
		size_t indexChild = (size_t)-1;
		size_t nbchild = boneParent->mChildren.size();
		for (size_t i = 0; i < nbchild; i++)
		{
			if (boneParent->mChildren.at(i) == bone)
			{
				indexChild = i;
				break;
			}
		}
			
		if (indexChild != (size_t)-1)
		{
			boneParent->mChildren.erase(boneParent->mChildren.begin() + indexChild);
				
			if (!recursive)											//if we don't want to destroy all child by recursive, we will attach the child of bon on is parent, so we keep this child in tree.
			{
				boneParent->mChildren.insert(boneParent->mChildren.begin() + indexChild, bone->mChildren.begin(), bone->mChildren.end());
				bone->mChildren.clear();
			}
		}
	}
	delete bone;

	std::vector<std::vector<size_t>> listMovingindex = setTreeOrganisation(rootNode);

	delete rootNode;				//clean temporary hierarchie.

	return listMovingindex;
}
/*-------------------------------------------------------------------------------\
|                             clear					                             |
\-------------------------------------------------------------------------------*/
void ESK::clear(void)
{
	size_t nbBones = bones.size();
	for (size_t i = 0; i < nbBones; i++)
		delete bones.at(i);
	bones.clear();

	listInverseKinematic.clear();
}
/*-------------------------------------------------------------------------------\
|                             addTree				                             |
\-------------------------------------------------------------------------------*/
std::vector<std::vector<std::vector<size_t>>> ESK::addTree(EskTreeNode* treeBone, size_t index)
{
	std::vector<std::vector<std::vector<size_t>>> listMovingindex;
	listMovingindex.push_back(std::vector<std::vector<size_t>>());
	listMovingindex.push_back(std::vector<std::vector<size_t>>());

	std::vector<std::vector<size_t>> &listNewBonesAdded = listMovingindex.at(0);
	std::vector<std::vector<size_t>> &listMovingindex_forPrevious = listMovingindex.at(1);

	size_t nbBones = bones.size();
	if (index >= nbBones)
		return listMovingindex;

	EskTreeNode* rootNode = getTreeOrganisation();

	EskTreeNode* bone = rootNode->getBoneWithIndex(index);		//find the EskTreeNode with index, 
	if (bone == nullptr)
		return listMovingindex;

	bone->addTree(treeBone, true, bones.size(), listNewBonesAdded, bones);				//addTree, duplicate Bones and start at Index = bones.size() for new bones. return a link between oldIndex (in another file), and new index.

	EskTreeNode* boneParent = bone->mParent;

	listMovingindex_forPrevious = setTreeOrganisation(rootNode);

	delete rootNode;				//clean temporary hierarchie.

	return listMovingindex;
}
/*-------------------------------------------------------------------------------\
|                             getBoneIndex			                             |
\-------------------------------------------------------------------------------*/
size_t ESK::getBoneIndex(const string &name)
{
	size_t nbBones = bones.size();
	for (size_t i = 0; i < nbBones; i++)
		if (bones.at(i)->getName() == name)
			return i;
	return (size_t)-1;
}











/*-------------------------------------------------------------------------------\
|                             getBoneIndex			                             |
\-------------------------------------------------------------------------------*/
EskTreeNode::EskTreeNode(ESKBone* bone, size_t index, EskTreeNode* parent)
{
	mBone = bone;
	mIndex = index;
	mParent = parent;
}
/*-------------------------------------------------------------------------------\
|                             getBoneIndex			                             |
\-------------------------------------------------------------------------------*/
EskTreeNode::~EskTreeNode()
{
	size_t nbChild = mChildren.size();
	for (size_t i = 0; i < nbChild; i++)
		delete mChildren.at(i);
	mChildren.clear();
}
/*-------------------------------------------------------------------------------\
|                             getBoneIndex			                             |
\-------------------------------------------------------------------------------*/
EskTreeNode* EskTreeNode::clone(bool alsoCloneBone, bool recursif)
{
	ESKBone* bone = this->mBone;
	if (alsoCloneBone)
		bone = new ESKBone(this->mBone);

	EskTreeNode* treeBone = new EskTreeNode(bone, mIndex, nullptr);

	if (recursif)
	{
		size_t nbChild = this->mChildren.size();
		for (size_t i = 0; i < nbChild; i++)
		{
			treeBone->mChildren.push_back(this->mChildren.at(i)->clone(alsoCloneBone, recursif));
			treeBone->mChildren.back()->mParent = treeBone;
		}
	}

	return treeBone;
}
/*-------------------------------------------------------------------------------\
|                             getBoneIndex			                             |
\-------------------------------------------------------------------------------*/
void EskTreeNode::setupBones_list(vector<ESKBone*> &newListBones)						//the first thing to do is to make the new list bone and refresh index.
{
	if (mBone)
	{
		mIndex = newListBones.size();
		newListBones.push_back(mBone);
	}else {
		mIndex = 65535;
	}

	size_t nbChild = mChildren.size();
	for (size_t i = 0; i < nbChild; i++)
		mChildren.at(i)->setupBones_list(newListBones);
}
/*-------------------------------------------------------------------------------\
|                             getBoneIndex			                             |
\-------------------------------------------------------------------------------*/
void EskTreeNode::setupBones(vector<ESKBone*> &newListBones)						//the second thing is to refresh parent , child and sibling index.
{
	size_t nbChild = mChildren.size();
	size_t previous_child = (size_t)-1;
	ESKBone* lastChild = nullptr;
	for (size_t i = 0; i < nbChild; i++)
	{
		if (!mChildren.at(i)->mBone)
			continue;

		mChildren.at(i)->mBone->parent_index = mIndex;

		if (previous_child == (size_t)-1)
		{
			if (this->mBone)
				this->mBone->child_index = mChildren.at(i)->mIndex;
		} else {
			lastChild->sibling_index = mChildren.at(i)->mIndex;
		}

		previous_child = mChildren.at(i)->mIndex;
		lastChild = mChildren.at(i)->mBone;
	}

	if ((nbChild == 0) && (mBone))
		mBone->child_index = 65535;
	if (lastChild != nullptr)
		lastChild->sibling_index = 65535;

	for (size_t i = 0; i < nbChild; i++)					//recursive
		mChildren.at(i)->setupBones(newListBones);
}
/*-------------------------------------------------------------------------------\
|                             getBoneIndex			                             |
\-------------------------------------------------------------------------------*/
string EskTreeNode::getDisplayStr(size_t indent)
{
	string str = "";

	for (size_t i = 0; i < indent; i++)
		str += "\t|";

	if (!mBone)
		str += "-----\n";
	else
		str += std::to_string(mIndex) + ": " + mBone->getName() + "\n";

	size_t nbChild = mChildren.size();
	for (size_t i = 0; i < nbChild; i++)
		str += mChildren.at(i)->getDisplayStr(indent + 1);

	return str;
}
/*-------------------------------------------------------------------------------\
|                             getBoneIndex			                             |
\-------------------------------------------------------------------------------*/
string EskTreeNode::getJsonStr(size_t indent, bool havePrevious)
{
	string str = ((havePrevious == true) ? ",{" : "{");

	bool isfirst = true;
	if (mBone)
	{
		str += "\"index\" : " + std::to_string(mIndex) + ",\"parentIndex\" : " + ((mBone->parent_index != 65535) ? std::to_string(mBone->parent_index) : "null") + ",\"name\": \"" + mBone->getName() + "\",\"RelativeTransform\": {" + mBone->getRelativeTransformDebug() + "},\"AbsoluteMatrix\": " + mBone->getAbsoluteMatrixDebug();
		isfirst = false;
	}

	size_t nbChild = mChildren.size();
	if (nbChild != 0)
	{
		str += ((!isfirst) ? "," : "") + string("\"children\": [");
		isfirst = false;
		for (size_t i = 0; i < nbChild; i++)
			str += mChildren.at(i)->getJsonStr(indent + 1, i != 0);
		str += "]";
	}
	str += "}";

	return str;
}
/*-------------------------------------------------------------------------------\
|                             getBoneWithIndex		                             |
\-------------------------------------------------------------------------------*/
EskTreeNode* EskTreeNode::getBoneWithIndex(size_t index)
{
	if (mIndex == index)
		return this;

	EskTreeNode* child = nullptr;
	size_t nbChild = mChildren.size();
	for (size_t i = 0; i < nbChild; i++)
	{
		child = mChildren.at(i)->getBoneWithIndex(index);
		if (child)
			return child;
	}

	return child;
}
/*-------------------------------------------------------------------------------\
|                             getBoneWithName		                             |
\-------------------------------------------------------------------------------*/
EskTreeNode* EskTreeNode::getBoneWithName(string name)
{
	if((mBone) && (mBone->getName() == name))
		return this;

	EskTreeNode* child = nullptr;
	size_t nbChild = mChildren.size();
	for (size_t i = 0; i < nbChild; i++)
	{
		child = mChildren.at(i)->getBoneWithName(name);
		if (child)
			return child;
	}

	return child;
}
/*-------------------------------------------------------------------------------\
|                             addTree				                             |
\-------------------------------------------------------------------------------*/
void EskTreeNode::addTree(EskTreeNode* treeBoneSrc, bool duplicateBone, size_t startIndexForNewBones, std::vector<std::vector<size_t>> &listNewBonesAdded, vector<ESKBone*> &listBones, bool recursif)
{
	if (!treeBoneSrc)
		return;

	EskTreeNode* treeBoneSrc_cloned = treeBoneSrc->clone(duplicateBone, recursif);
	treeBoneSrc_cloned->mParent = this;
	this->mChildren.push_back(treeBoneSrc_cloned);

	//have to get and set new Index
	vector<EskTreeNode*> newListBones;
	treeBoneSrc_cloned->getTreeBonesList(newListBones);

	size_t nbBones = newListBones.size();
	for (size_t i = 0; i < nbBones; i++)
	{
		listNewBonesAdded.push_back(std::vector<size_t>());
		listNewBonesAdded.back().push_back(newListBones.at(i)->mIndex);				//save old index relation.
		listNewBonesAdded.back().push_back(startIndexForNewBones);

		listBones.push_back(newListBones.at(i)->mBone);
		newListBones.at(i)->mIndex = startIndexForNewBones;
		startIndexForNewBones++;

	}
}
/*-------------------------------------------------------------------------------\
|                             getBoneIndex			                             |
\-------------------------------------------------------------------------------*/
void EskTreeNode::getTreeBonesList(vector<EskTreeNode*> &newListBones)
{
	newListBones.push_back(this);

	size_t nbChild = mChildren.size();
	for (size_t i = 0; i < nbChild; i++)
		mChildren.at(i)->getTreeBonesList(newListBones);
}




}