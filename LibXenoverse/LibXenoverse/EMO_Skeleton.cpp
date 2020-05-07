#include <stdlib.h>

#include <stdexcept>
#include <algorithm>

#include "EMO_Skeleton.h"
#include "EMA.h"
//#include "debug.h"


namespace LibXenoverse
{

/*
 * Structure of skeleton:
 *
 * Skeleton Header
 * node_count * SkeletonNode
 * node_count * EMO_Ske_Bone_extraInfo
 * names_table
 * (pad to next 0x10 boundary if necessary)
 * node_count * matrix_data
 * IKData
 *
*/


bool EMO_Skeleton::Load(const uint8_t *buf, unsigned int size)							//read
{
	Reset();

	UNUSED(size);

	SkeletonHeader *hdr = (SkeletonHeader *)buf;
	uint16_t node_count = val16(hdr->node_count);
	unknow_0 = val32(hdr->unknow_0);
	unknow_1 = val32(hdr->unknow_1);
	unknow_2 = val32(hdr->unknow_2);
	unknow_3 = val32(hdr->unknow_3);
	unknow_4 = val32(hdr->unknow_4);
	skeletonUniqueId = val64(hdr->skeletonUniqueId);
	

	uint32_t *names_table = (uint32_t *)GetOffsetPtr(buf, hdr->names_offset);
	SkeletonNode *nodes = (SkeletonNode *)GetOffsetPtr(buf, hdr->bones_offset);
	EMO_Ske_Bone_extraInfo *extra_infos = nullptr;
	EMO_Ske_Bone_Absolute_Matrix *absMatrixes = nullptr;

	if (hdr->offset_boneExtraInfo)
	{
		extra_infos = (EMO_Ske_Bone_extraInfo *)GetOffsetPtr(buf, hdr->offset_boneExtraInfo);
		mHaveExtraBytesOnEachBone = true;
	}else {
		mHaveExtraBytesOnEachBone = false;
	}

	if (hdr->offset_absMatrices)
		absMatrixes = (EMO_Ske_Bone_Absolute_Matrix *)GetOffsetPtr(buf, hdr->offset_absMatrices);


	for (uint16_t i = 0; i < node_count; i++)
	{
		EMO_Bone bone;

		
		bone.name = std::string((char *)GetOffsetPtr(buf, names_table, i));
		bone.emgIndex = val16(nodes[i].emgIndex);
		bone.unknow_0 = val16(nodes[i].unknow_0);
		bone.ik_flag = val16(nodes[i].ik_flag);
		bone.unknow_1 = val_float(nodes[i].unknow_1);
		bone.debugIndex = i;

		bone.meta_original_offset = EMO_BaseFile::DifPointer(&nodes[i], buf);

		for (int j = 0; j < 16; j++)
			bone.relativeMatrix[j] = val_float(nodes[i].relativeMatrix[j]);

		if (absMatrixes)
		{
			bone.haveAbsoluteMatrix = true;
			for (int j = 0; j < 16; j++)
				bone.absoluteMatrix[j] = val_float(absMatrixes[i].absoluteMatrix[j]);
		}

		if (extra_infos)
		{
			bone.unk_extraInfo_0 = val16(extra_infos[i].unknow_0);
			bone.unk_extraInfo_1 = val16(extra_infos[i].unknow_1);
			bone.unk_extraInfo_2 = val16(extra_infos[i].unknow_2);
			bone.unk_extraInfo_3 = val16(extra_infos[i].unknow_3);
		}

		bones.push_back(bone);
	}

	uint16_t parent, child, sibling;
	for (uint16_t i = 0; i < node_count; i++)							//second pass when all bones are here
	{
		EMO_Bone &bone = bones.at(i);

		parent = val16(nodes[i].parent_id);
		child = val16(nodes[i].child_id);
		sibling = val16(nodes[i].sibling_id);
		bones[i].parent = ((parent != 0xFFFF) && (parent < node_count)) ? &bones[parent] : nullptr;
		bones[i].child = ((child != 0xFFFF) && (child < node_count)) ? &bones[child] : nullptr;
		bones[i].sibling = ((sibling != 0xFFFF) && (sibling < node_count)) ? &bones[sibling] : nullptr;

	}


	size_t offset = 0;
	listInverseKinematic.clear();
	if (hdr->ik_data_offset)
	{
		offset = val32(hdr->ik_data_offset);
		size_t nbIkGroup = val16(hdr->ik_count);
		size_t nbBones = bones.size();

		bool wrongOldCaseDetected = false;

		uint16_t val_u16;
		for (size_t i = 0; i < nbIkGroup; i++)
		{
			listInverseKinematic.push_back(Emo_IK_Group());

			size_t startOffset_group = offset;
			uint16_t* u16_values = (uint16_t *)GetOffsetPtr(buf, offset, true);

			
			uint16_t typeGroup = val16(u16_values[0]);
			uint16_t size_tmp = val16(u16_values[1]);
			offset += 2 * sizeof(uint16_t);

			listInverseKinematic.back().type = typeGroup;

			if(typeGroup == 1)										//use weights
			{
				listInverseKinematic.back().mListIK.push_back(Emo_IK_Relation());
				Emo_IK_Relation &ik = listInverseKinematic.back().mListIK.back();

				ik.unknow_0 = buf[offset];
				offset++;
				uint8_t nbRelations = buf[offset];
				offset++;

				u16_values = (uint16_t *)GetOffsetPtr(buf, offset, true);
				offset += (nbRelations + 1) * sizeof(uint16_t);
				for (size_t k = 0; k <= nbRelations; k++)
				{
					val_u16 = val16(u16_values[k]);
					if (val_u16 < nbBones)
					{
						ik.mListBones.push_back(Emo_IK_Relation::IKR_Bone(&bones.at(val_u16)));
					}else {
						wrongOldCaseDetected = true;
						break;
					}
				}
				if (wrongOldCaseDetected)
					break;

				if (nbRelations % 2 != 0)					//padding for 32bits
					offset += sizeof(uint16_t);


				float* float_values = (float*)GetOffsetPtr(buf, offset, true);
				offset += (nbRelations + 1) * sizeof(float);

				for (size_t k = 0; k <= nbRelations; k++)
					ik.mListBones.at(k).value = val_float(float_values[k]);



			}else if (typeGroup == 0) {							//no use of weights case SSSS HSY.ema
				
				listInverseKinematic.back().mListIK.push_back(Emo_IK_Relation());
				Emo_IK_Relation &ik = listInverseKinematic.back().mListIK.back();

				ik.unknow_0 = buf[offset];
				offset++;
				ik.unknow_1 = buf[offset];
				offset++;

				uint8_t nbRelations = 4;

				u16_values = (uint16_t *)GetOffsetPtr(buf, offset, true);
				offset += (nbRelations + 1) * sizeof(uint16_t);
				for (size_t k = 0; k <= nbRelations; k++)
				{
					val_u16 = val16(u16_values[k]);
					if (val_u16 < nbBones)
					{
						ik.mListBones.push_back(Emo_IK_Relation::IKR_Bone(&bones.at(val_u16)));
					}else {
						wrongOldCaseDetected = true;
						break;
					}
				}
				if (wrongOldCaseDetected)
					break;

			}else {
				printf("Error: Ik_Group type unknow %i: \n", typeGroup);
				LibXenoverse::notifyError();
			}
			if (wrongOldCaseDetected)
				break;

			offset = startOffset_group + size_tmp;
		}

		if (wrongOldCaseDetected)
			listInverseKinematic.clear();
	}









	listInverseKinematic2.clear();
	if (hdr->ik2_offset)										//version from StreetFighters
	{
		offset = val32(hdr->ik2_offset);
		size_t nbIk2 = val16(hdr->ik2_count);
		size_t nbBones = bones.size();

		size_t startOffset_ik2 = offset;
		
		ESK_IK_2* ik2 = (ESK_IK_2*)GetOffsetPtr(buf, offset, true);
		size_t startOffset_ik2_listbones = offset = offset + nbIk2 * sizeof(ESK_IK_2);
		uint16_t* u16_values = (uint16_t*)GetOffsetPtr(buf, offset, true);

		size_t inc_bones = 0;
		uint16_t val_u16;
		for (size_t i = 0; i < nbIk2; i++)
		{
			listInverseKinematic2.push_back(Emo_IK_Relation2());
			Emo_IK_Relation2 &ik = listInverseKinematic2.back();

			size_t number_bones = val32(ik2[i].number_bones);
			ik.unknow_0 = val32(ik2[i].unknow_0);
			
			for (size_t k = 0; k < number_bones; k++)
			{
				val_u16 = val16(u16_values[inc_bones + k]);
				if (val_u16 < nbBones)
					ik.mListBones.push_back(Emo_IK_Relation::IKR_Bone(&bones.at(val_u16)));
			}
			
			inc_bones += number_bones;
			offset = startOffset_ik2_listbones + inc_bones * sizeof(uint16_t);
		}


		size_t startNameOffsets = val32(hdr->ik2_names_offset);
		uint32_t* namesOffets = (uint32_t*)GetOffsetPtr(buf, startNameOffsets, true);

		for (size_t i = 0; i < nbIk2; i++)
		{
			offset = val32(namesOffets[i]);
			listInverseKinematic2.at(i).name = std::string((char *)GetOffsetPtr(buf, offset));
			offset += listInverseKinematic2.at(i).name.length() + 1;
		}
	}

	return true;
}







bool EMO_Skeleton::SaveSkeletonToFile(const std::string &path, bool show_error, bool build_path)
{
	unsigned int size;

	uint8_t *buf = EMO_Skeleton::CreateFile(&size, EMA_TYPE_ANIM_Object_or_Camera);

	if (!buf)
		return false;

	bool ret = EMO_BaseFile::WriteFileBool(path, buf, size, show_error, build_path);
	delete[] buf;

	return ret;
}
uint8_t *EMO_Skeleton::CreateFile(unsigned int *psize, uint16_t typeFile)							//write
{
	std::vector<string> listEMG;
	return CreateFile(psize, listEMG, EMA_TYPE_ANIM_Object_or_Camera);
}

uint8_t *EMO_Skeleton::CreateFile(unsigned int *psize, std::vector<string> listEMG, uint16_t typeFile, bool addLastPadding)
{
	setupBoneIkLinks();							//to update Ik_flag on each bone concerned (first of IK)

	if (bones.size() == 0)
		return nullptr;

	uint8_t *buf;
	unsigned int file_size;
	uint32_t offset;

	bool isSfiv = false;					//Todo


	file_size = CalculateFileSize(addLastPadding);
	
	
	buf = new uint8_t[file_size];
	if (!buf)
	{
		printf("%s: Memory allocation error (0x%x)\n", FUNCNAME, file_size);
		notifyError();
		return nullptr;
	}

	memset(buf, 0, file_size);

	SkeletonHeader *hdr = (SkeletonHeader *)buf;

	hdr->node_count = val16(bones.size());
	hdr->ik2_count = 0;
	hdr->ik2_offset = 0;
	hdr->ik2_names_offset = 0;
	hdr->unknow_0 = val32(unknow_0);
	hdr->unknow_1 = val32(unknow_1);
	hdr->unknow_2 = val32(unknow_2);
	hdr->unknow_3 = val32(unknow_3);
	hdr->unknow_4 = val32(unknow_4);
	hdr->skeletonUniqueId = val64(skeletonUniqueId);
	
	offset = sizeof(SkeletonHeader);
	hdr->bones_offset = val32(offset);

	SkeletonNode *nodes = (SkeletonNode *)GetOffsetPtr(buf, offset, true);

	size_t nbEMG = listEMG.size();
	std::vector<string> listEMG_tmp = listEMG;
	std::vector<size_t> listEMGIndex_tmp;
	for (size_t i = 0; i < nbEMG; i++)
		listEMGIndex_tmp.push_back( i );

	for (size_t i = 0; i < bones.size(); i++)
	{
		nodes[i].parent_id = val16(BoneToIndex(bones[i].parent));
		nodes[i].child_id = val16(BoneToIndex(bones[i].child));
		nodes[i].sibling_id = val16(BoneToIndex(bones[i].sibling));

		nodes[i].emgIndex = (typeFile == EMA_TYPE_ANIM_Material) ? 0xFFFF : val16(bones[i].emgIndex);

		nodes[i].unknow_0 = val16(bones[i].unknow_0);
		nodes[i].ik_flag = val16(bones[i].ik_flag);
		nodes[i].unknow_1 = val_float(bones[i].unknow_1);

		for (int j = 0; j < 16; j++)
			copy_float(&nodes[i].relativeMatrix[j], bones[i].relativeMatrix[j]);

		offset += sizeof(SkeletonNode);
	}

	if (mHaveExtraBytesOnEachBone)
	{
		hdr->offset_boneExtraInfo = val32(offset);

		EMO_Ske_Bone_extraInfo *extra_infos = (EMO_Ske_Bone_extraInfo *)GetOffsetPtr(buf, offset, true);

		for (size_t i = 0; i < bones.size(); i++)
		{
			extra_infos[i].unknow_0 = val16(bones[i].unk_extraInfo_0);
			extra_infos[i].unknow_1 = val16(bones[i].unk_extraInfo_1);
			extra_infos[i].unknow_2 = val16(bones[i].unk_extraInfo_2);
			extra_infos[i].unknow_3 = val16(bones[i].unk_extraInfo_3);

			offset += sizeof(EMO_Ske_Bone_extraInfo);
		}
	}else{
		hdr->offset_boneExtraInfo = 0;
	}

	hdr->names_offset = val32(offset);

	uint32_t *names_table = (uint32_t *)GetOffsetPtr(buf, offset, true);
	offset += bones.size() * sizeof(uint32_t);
	size_t startNames = offset;

	for (size_t i = 0; i < bones.size(); i++)
	{
		names_table[i] = val32(offset);
		strcpy((char *)buf + offset, bones[i].name.c_str());
		offset += bones[i].name.length() + 1;
	}


	



	// Padding special , sort of match from the 8 before the names.
	if ( (offset > startNames) && 
		((addLastPadding) || (bones[0].haveAbsoluteMatrix) || (listInverseKinematic.size()) || (listInverseKinematic2.size())) )
	{
		/*
		startNames += (size_t)(ceil((double)(startNames) / 8.0) * 8.0) - startNames;

		size_t size_padding = (size_t)(ceil((double)(offset - startNames) / 16.0) * 16.0) - (offset - startNames);
		offset += size_padding;
		*/

		float padding = (isSfiv) ? 4.0f : 16.0f;
		size_t size_padding = (size_t)(ceil((double)offset / padding) * padding) - offset;
		offset += size_padding;
	}


	if (bones[0].haveAbsoluteMatrix)
	{
		hdr->offset_absMatrices = val32(offset);

		EMO_Ske_Bone_Absolute_Matrix *matrixes = (EMO_Ske_Bone_Absolute_Matrix *)GetOffsetPtr(buf, offset, true);

		for (size_t i = 0; i < bones.size(); i++)
		{
			for (int j = 0; j < 16; j++)
				copy_float(&matrixes[i].absoluteMatrix[j], bones[i].absoluteMatrix[j]);

			offset += sizeof(EMO_Ske_Bone_Absolute_Matrix);
		}
	}else{
		hdr->offset_absMatrices = 0;
	}




	if (listInverseKinematic.size())
	{
		hdr->ik_data_offset = val32(offset);

		size_t bone_count = bones.size();
		size_t nbGroup = listInverseKinematic.size();
		hdr->ik_count = val16(nbGroup);

		for (size_t i = 0; i < nbGroup; i++)
		{
			Emo_IK_Group &group = listInverseKinematic.at(i);

			size_t startOffset_group = offset;
			uint16_t* u16_values = (uint16_t *)GetOffsetPtr(buf, offset, true);

			size_t size_tmp = 2 * sizeof(uint16_t);
			offset += 2 * sizeof(uint16_t);

			u16_values[0] = val16(group.type);

			if (group.type == LIBXENOVERSE_EMO_IK_TYPE_WEIGHT)
			{
				size_t nbBones = group.mListIK.at(0).mListBones.size();
				size_tmp += 2 * sizeof(uint8_t) + nbBones * (sizeof(uint16_t) + sizeof(float));
				if (nbBones % 2 == 0)
					size_tmp += sizeof(uint16_t);
				if (size_tmp & 0x7)
					size_tmp += (0x8 - (size_tmp & 0x7));

				u16_values[1] = val16(size_tmp);
				

				Emo_IK_Relation &ik = group.mListIK.at(0);
				size_t nbrelations = ik.mListBones.size();
				if (nbrelations > 1)
					nbrelations--;

				buf[offset++] = ik.unknow_0;
				buf[offset++] = nbrelations;

				u16_values = (uint16_t *)GetOffsetPtr(buf, offset, true);
				offset += (nbrelations + 1) * sizeof(uint16_t);

				for (size_t k = 0; k <= nbrelations; k++)
				{
					EMO_Bone* bone = ik.mListBones.at(k).bone;

					size_t isfound = (size_t)-1;
					for (size_t m = 0; m < bone_count; m++)
					{
						if (&bones.at(m) == bone)
						{
							isfound = m;
							break;
						}
					}
					if (!isfound)			//secu
						isfound = 0;

					u16_values[k] = val16(isfound);
				}
				if (nbrelations % 2 != 0)				//padding
				{
					buf[offset++] = 0;
					buf[offset++] = 0;
				}

				float* float_values = (float*)GetOffsetPtr(buf, offset, true);
				offset += (nbrelations + 1) * sizeof(float);

				for (size_t k = 0; k <= nbrelations; k++)
					float_values[k] = val_float(ik.mListBones.at(k).value);




			}else if (group.type == LIBXENOVERSE_EMO_IK_TYPE_EQUAL) {

				size_t nbBones = group.mListIK.at(0).mListBones.size();
				size_tmp += 2 * sizeof(uint8_t) + nbBones * sizeof(uint16_t);
				u16_values[1] = val16(size_tmp);

				Emo_IK_Relation &ik = group.mListIK.at(0);
				
				buf[offset++] = ik.unknow_0;
				buf[offset++] = ik.unknow_1;

				u16_values = (uint16_t *)GetOffsetPtr(buf, offset, true);
				offset += nbBones * sizeof(uint16_t);

				for (size_t k = 0; k < nbBones; k++)
				{
					EMO_Bone* bone = ik.mListBones.at(k).bone;

					size_t isfound = (size_t)-1;
					for (size_t m = 0; m < bone_count; m++)
					{
						if (&bones.at(m) == bone)
						{
							isfound = m;
							break;
						}
					}
					if (!isfound)			//secu
						isfound = 0;

					u16_values[k] = val16(isfound);
				}
			}

			offset = startOffset_group + size_tmp;
		}
	}














	if (listInverseKinematic2.size())
	{
		// Padding
		if ((offset & 0x7) && (!isSfiv))
			offset += (0x8 - (offset & 0x7));

		hdr->ik2_offset = val32(offset);

		
		size_t bone_count = bones.size();
		size_t nbIk2 = listInverseKinematic2.size();
		hdr->ik2_count = val16(nbIk2);


		size_t startOffset_ik = offset;
		ESK_IK_2* ik2 = (ESK_IK_2 *)GetOffsetPtr(buf, offset, true);

		size_t startOffset_ik_listBones = offset = offset + nbIk2 * sizeof(ESK_IK_2);
		uint16_t* u16_values = (uint16_t *)GetOffsetPtr(buf, startOffset_ik_listBones, true);

		size_t inc_bones = 0;
		for (size_t i = 0; i < nbIk2; i++)
		{
			Emo_IK_Relation2 &ik = listInverseKinematic2.at(i);

			uint32_t number_bones = ik.mListBones.size();
			ik2[i].number_bones = val32(number_bones);
			ik2[i].unknow_0 = val32(ik.unknow_0);
			
			for (size_t j = 0; j < number_bones; j++)
			{
				EMO_Bone* bone = ik.mListBones.at(j).bone;

				size_t isfound = (size_t)-1;
				for (size_t m = 0; m < bone_count; m++)
				{
					if (&bones.at(m) == bone)
					{
						isfound = m;
						break;
					}
				}
				if (!isfound)			//secu
					isfound = 0;

				u16_values[inc_bones + j] = val16(isfound);
			}

			inc_bones += number_bones;
			offset = startOffset_ik_listBones + inc_bones * sizeof(uint16_t);
		}

		// Padding
		if ((offset & 0x3) && (addLastPadding))
			offset += (0x4 - (offset & 0x3));

		size_t startNameOffsets = offset;
		hdr->ik2_names_offset = val32(offset);

		uint32_t* nameOffets = (uint32_t*)GetOffsetPtr(buf, startNameOffsets, true);
		offset = startNameOffsets + nbIk2 * sizeof(uint32_t);

		for (size_t i = 0; i < nbIk2; i++)
		{
			nameOffets[i] = val32(offset);
			strcpy((char *)buf + offset, listInverseKinematic2.at(i).name.c_str());
			offset += listInverseKinematic2.at(i).name.length() + 1;
		}

		// Padding
		if ((offset & 0x3) && (addLastPadding))
			offset += (0x4 - (offset & 0x3));
	}


	if (offset != file_size)
	{
		printf("Error: the size of the final file is not the same as allocated memory. so there certainly is a mistake. please contact the dev team and send them yours current sample.\n");
		notifyError();
	}

	*psize = file_size;
	return buf;
}

/*-------------------------------------------------------------------------------\
|                             setupBoneIkLinks			                         |
\-------------------------------------------------------------------------------*/
void EMO_Skeleton::setupBoneIkLinks()											//to update Ik_flag on each bone concerned (first of IK)
{
	return;										// for prefect rebuild , we keep the value from Xml. todo better.
	//todo complete with 0x0 vs 0x1


	for (size_t i = 0, nb = bones.size(); i < nb; i++)
		bones.at(i).ik_flag = 0;

	if (listInverseKinematic.size() == 0)
		return;

	for (size_t j = 0, nb2 = listInverseKinematic.size(); j < nb2; j++)
	{
		Emo_IK_Group &group = listInverseKinematic.at(j);
		for (size_t k = 0, nb3 = group.mListIK.size(); k < nb3; k++)
		{
			Emo_IK_Relation &ik = group.mListIK.at(k);
			if (ik.mListBones.size())
				ik.mListBones.at(0).bone->ik_flag = LIBXENOVERSE_EMO_BONE_HAVE_IK;
		}
	}
}

unsigned int EMO_Skeleton::CalculateFileSize(bool addLastPadding) const
{
	if (bones.size() == 0)
		return 0;

	bool isSfiv = false;					//Todo

	unsigned int file_size = sizeof(SkeletonHeader);
	file_size += (bones.size() * sizeof(SkeletonNode));

	if (mHaveExtraBytesOnEachBone)
		file_size += (bones.size() * sizeof(EMO_Ske_Bone_extraInfo));

	file_size += (bones.size() * sizeof(uint32_t)); // Names table

	size_t startNames = file_size;
	for (const EMO_Bone &b : bones)
		file_size += b.name.length() + 1;

	// Padding special , sort of match from the 8 before the names.
	if ((file_size > startNames) && 
		((addLastPadding) || (bones[0].haveAbsoluteMatrix) || (listInverseKinematic.size()) || (listInverseKinematic2.size())))
	{
		/*
		startNames += (size_t)(ceil((double)(startNames) / 8.0) * 8.0) - startNames;

		size_t size_padding = (size_t)(ceil((double)(file_size - startNames) / 16.0) * 16.0) - (file_size - startNames);
		file_size += size_padding;
		*/

		float padding = (isSfiv) ? 4.0f : 16.0f;
		size_t size_padding = (size_t)(ceil((double)file_size / padding) * padding) - file_size;
		file_size += size_padding;
	}

	if (bones[0].haveAbsoluteMatrix)
		file_size += (bones.size() * sizeof(EMO_Ske_Bone_Absolute_Matrix));

	file_size += calculIksize(addLastPadding);

	if ((listInverseKinematic2.size()) && (file_size & 0x7) && (!isSfiv))
		file_size += (0x8 - (file_size & 0x7));

	file_size += calculIksize2(addLastPadding);

	return file_size;
}
/*-------------------------------------------------------------------------------\
|                             calculIksize			                             |
\-------------------------------------------------------------------------------*/
size_t EMO_Skeleton::calculIksize(bool addLastPadding) const
{
	size_t filesize = 0;
	size_t nbGroup = listInverseKinematic.size();
	if (nbGroup == 0)
		return filesize;

	filesize = 0;
	for (size_t i = 0; i < nbGroup; i++)
	{
		const Emo_IK_Group &group = listInverseKinematic.at(i);

		if (group.type == LIBXENOVERSE_EMO_IK_TYPE_WEIGHT)
		{
			filesize += 2 * sizeof(uint16_t);

			size_t nbBones = group.mListIK.at(0).mListBones.size();
			filesize += 2 * sizeof(uint8_t) + nbBones * (sizeof(uint16_t) + sizeof(float));
			if (nbBones % 2 == 0)
				filesize += sizeof(uint16_t) + sizeof(float);

			/*
			if ((filesize & 0x7) && ((addLastPadding) || (listInverseKinematic2.size())))
				filesize += (0x8 - (filesize & 0x7));
			*/

		}else if (group.type == LIBXENOVERSE_EMO_IK_TYPE_EQUAL) {
			
			filesize += 2 * sizeof(uint16_t);

			size_t nbBones = group.mListIK.at(0).mListBones.size();
			filesize += 2 * sizeof(uint8_t) + nbBones * sizeof(uint16_t);
		}
	}

	
	

	return filesize;
}
/*-------------------------------------------------------------------------------\
|                             calculIksize2			                             |
\-------------------------------------------------------------------------------*/
size_t EMO_Skeleton::calculIksize2(bool addLastPadding) const
{
	size_t filesize = 0;
	size_t nbIk2 = listInverseKinematic2.size();
	if (nbIk2 == 0)
		return filesize;

	for (size_t i = 0; i < nbIk2; i++)
	{
		const Emo_IK_Relation2 &ik = listInverseKinematic2.at(i);
		
		size_t nbBones = ik.mListBones.size();
		filesize += sizeof(ESK_IK_2) + nbBones * sizeof(uint16_t);
	}

	if (filesize & 0x3)
		filesize += (0x4 - (filesize & 0x3));

	for (size_t i = 0; i < nbIk2; i++)
	{
		const Emo_IK_Relation2 &ik = listInverseKinematic2.at(i);
		filesize += sizeof(uint32_t) + (ik.name.length() + 1);
	}


	//if ((filesize & 0x3) && (addLastPadding))
	if (filesize & 0x3)
		filesize += (0x4 - (filesize & 0x3));

	return filesize;
}
































/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// load/save the Xml version	/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////





bool EMO_Skeleton::DecompileSkeletonToFile(const std::string &path, bool show_error, bool build_path)
{
	TiXmlDocument *doc = EMO_Skeleton::Decompile();

	if (!doc)
	{
		if (show_error)
		{
			printf("Decompilation of file \"%s\" failed.\n", path.c_str());
			LibXenoverse::notifyError();
		}

		return false;
	}

	if (build_path)
	{
		if (!EMO_BaseFile::CreatePath(path))
		{
			if (show_error)
			{
				printf("Cannot create path for file \"%s\"\n", path.c_str());
				LibXenoverse::notifyError();
			}

			return false;
		}
	}

	bool ret = doc->SaveFile(path);
	delete doc;

	if (!ret && show_error)
	{
		printf("Cannot create/write file \"%s\"\n", path.c_str());
		LibXenoverse::notifyError();
	}

	return ret;
}
TiXmlDocument *EMO_Skeleton::Decompile() const
{
	TiXmlDocument *doc = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "utf-8", "");
	doc->LinkEndChild(decl);

	Decompile(doc);

	return doc;
}
void EMO_Skeleton::Decompile(TiXmlNode *root) const
{
	TiXmlElement *entry_root = new TiXmlElement("Skeleton");

	entry_root->SetAttribute("skeletonUniqueId", Unsigned64ToString(skeletonUniqueId, true));

	entry_root->SetAttribute("unk_0", UnsignedToString(unknow_0, true));
	entry_root->SetAttribute("unk_1", UnsignedToString(unknow_1, true));
	entry_root->SetAttribute("unk_2", UnsignedToString(unknow_2, true));
	entry_root->SetAttribute("unk_3", UnsignedToString(unknow_3, true));
	entry_root->SetAttribute("unk_4", UnsignedToString(unknow_4, true));
	
	

	{							//test Todo remove
		size_t nbbones = bones.size();
		size_t nbIks = listInverseKinematic.size();


		

		bool hyp_ik4_firstIkInGroup = true;						//todo test about the groupType
		for (size_t i = 0; i < nbbones; i++)
		{
			const EMO_Bone &bone = bones.at(i);
			if ((bone.ik_flag & 4) == 0)						//4 or 5
				continue;

			bool isfound = false;
			for (size_t j = 0; j < nbIks; j++)
			{
				const Emo_IK_Group &ik_g = listInverseKinematic.at(j);
				if ((ik_g.mListIK.size() != 0) &&
					(ik_g.mListIK.at(0).mListBones.size() != 0) &&
					(ik_g.mListIK.at(0).mListBones.at(0).bone) &&
					(ik_g.mListIK.at(0).mListBones.at(0).bone->name == bone.name))
				{
					isfound = true;
					break;
				}
			}

			if (!isfound)
			{
				hyp_ik4_firstIkInGroup = false;
				break;
			}
		}
		entry_root->SetAttribute("hyp_ik4_firstIkInGroup", hyp_ik4_firstIkInGroup ? "true" : "false");


		bool hyp_ik4_firstIkInGroup_inv = true;						//todo test about the groupType
		for (size_t j = 0; j < nbIks; j++)
		{
			const Emo_IK_Group &ik_g = listInverseKinematic.at(j);
			if (ik_g.mListIK.size() == 0)
				continue;

			for (size_t k = 0, nb = ik_g.mListIK.size(); k < nb; k++)
			{
				const Emo_IK_Relation ik_r = ik_g.mListIK.at(k);
				if ((ik_r.mListBones.size() == 0) || (!ik_r.mListBones.at(0).bone) )
					continue;

				if ((ik_r.mListBones.at(0).bone->ik_flag & 4) == 0)						//4 or 5
				{
					hyp_ik4_firstIkInGroup_inv = false;
					break;
				}
			}
			if (!hyp_ik4_firstIkInGroup_inv)
				break;
		}
		entry_root->SetAttribute("hyp_ik4_firstIkInGroup_inv", hyp_ik4_firstIkInGroup_inv ? "true" : "false");


		switch (context)
		{
			case ContextUse::Object : 
			{
				bool hyp_ik1_Object_always0 = true;
				for (size_t i = 0; i < nbbones; i++)
				{
					const EMO_Bone &bone = bones.at(i);
					if ((bone.ik_flag & 1) != 0)						//0,2 or 4, not 1 or 5
					{
						hyp_ik1_Object_always0 = false;
						break;
					}
				}
				entry_root->SetAttribute("hyp_ik1_Object_always0", hyp_ik1_Object_always0 ? "true" : "false");

			}
			break;

			case ContextUse::ObjectAnimation:
			{
				EMA* ema = (EMA*)contextPtr;
				std::vector<EmaAnimation> &anims = ema->getAnimations();
				size_t nbAnim = anims.size();

				bool hyp_ik1_ObjectAnim_is_1_IfAnimNode = true;
				for (size_t i = 0; i < nbbones; i++)
				{
					const EMO_Bone &bone = bones.at(i);
					bool shouldHaveAnim = (bone.ik_flag & 1);
				

					bool isfound = false;
					for (size_t j = 0; j < nbAnim; j++)
					{
						EmaAnimation &anim = anims.at(j);
						std::vector<EMAAnimationNode> &nodes = anim.getNodes();

						for (size_t k = 0, nb = nodes.size(); k < nb; k++)
						{
							if((nodes.at(k).GetBone())&&(nodes.at(k).GetBone()->GetName()== bone.name))
							{
								isfound = true;
								break;
							}
						}
						if (isfound)
							break;
					}

					if ( ((!isfound)&&(shouldHaveAnim)) || ((isfound) && (!shouldHaveAnim)) )
					{
						hyp_ik1_ObjectAnim_is_1_IfAnimNode = false;
						break;
					}
				}
				entry_root->SetAttribute("hyp_ik1_ObjectAnim_is_1_IfAnimNode", hyp_ik1_ObjectAnim_is_1_IfAnimNode ? "true" : "false");


			}
			break;

			case ContextUse::MaterialAnimation:
			{
				EMA* ema = (EMA*)contextPtr;
				std::vector<EmaAnimation> &anims = ema->getAnimations();
				size_t nbAnim = anims.size();


				bool hyp_ik1_MaterialAnim_is_1_OnFirst = true;
				for (size_t i = 0; i < nbbones; i++)
				{
					const EMO_Bone &bone = bones.at(i);
					bool haveit = (bone.ik_flag & 1);

					
					if(i != 0)
					{
						if (haveit)									// it's look like having 0 on all notTheFirstBone.
						{
							hyp_ik1_MaterialAnim_is_1_OnFirst = false;
							break;
						}

					}else {												//else look like depend if the bone is animated or not.

						bool isfound = false;
						for (size_t j = 0; j < nbAnim; j++)
						{
							EmaAnimation &anim = anims.at(j);
							std::vector<EMAAnimationNode> &nodes = anim.getNodes();

							for (size_t k = 0, nb = nodes.size(); k < nb; k++)
							{
								if ((nodes.at(k).GetBone()) && (nodes.at(k).GetBone()->GetName() == bone.name))
								{
									isfound = true;
									break;
								}
							}
							if (isfound)
								break;
						}

						if (((!isfound) && (haveit)) || ((isfound) && (!haveit)))
						{
							hyp_ik1_MaterialAnim_is_1_OnFirst = false;
							break;
						}
					}
				}
				entry_root->SetAttribute("hyp_ik1_MaterialAnim_is_1_OnFirst", hyp_ik1_MaterialAnim_is_1_OnFirst ? "true" : "false");




				bool hyp__MaterialAnim_AllboneHavesameParent = true;
				for (size_t i = 0; i < nbbones; i++)
				{
					const EMO_Bone &bone = bones.at(i);
					

					if( (bone.parent->name!=bones.at(0).name) ||
						(bone.child->name != bones.at(0).name) ||
						(bone.sibling->name != bones.at(0).name) )
					{
						hyp__MaterialAnim_AllboneHavesameParent = false;
						break;
					}
				}
				entry_root->SetAttribute("hyp__MaterialAnim_AllboneHavesameParent", hyp__MaterialAnim_AllboneHavesameParent ? "true" : "false");

			}
			break;

		}



	}




	{						//test Todo remove.
		string duplicatedBoneName = "";
		string boneName;
		for (size_t i = 0, nbBone = bones.size(); i < nbBone; i++)
		{
			string boneName = bones.at(i).name;
			for (size_t j = i + 1; j < nbBone; j++)
			{
				if (bones.at(j).name == boneName)
				{
					duplicatedBoneName += boneName + ",";
					continue;
				}
			}
		}
		if (duplicatedBoneName.length())
			entry_root->SetAttribute("duplicatedBoneName", duplicatedBoneName);
	}


	for (const EMO_Bone &b : bones)
		b.Decompile(entry_root, mHaveExtraBytesOnEachBone);


	if (listInverseKinematic.size())
	{
		TiXmlElement* node_IK_list = new TiXmlElement("InverseKinematic");
		entry_root->LinkEndChild(node_IK_list);

		size_t nbBones = listInverseKinematic.size();
		for (size_t i = 0; i < nbBones; i++)
		{
			const Emo_IK_Group &group = listInverseKinematic.at(i);

			TiXmlElement* node_group = new TiXmlElement("Group");
			node_IK_list->LinkEndChild(node_group);
			node_group->SetAttribute("type", (group.type == LIBXENOVERSE_EMO_IK_TYPE_WEIGHT) ? "Weight" : "Equal");
			

			const Emo_IK_Relation &ik = group.mListIK.at(0);
			TiXmlElement* node_ik = new TiXmlElement("IK");
			node_group->LinkEndChild(node_ik);
			node_ik->SetAttribute("unk_0", ik.unknow_0);
			if(group.type == LIBXENOVERSE_EMO_IK_TYPE_EQUAL)
				node_ik->SetAttribute("unk_1", ik.unknow_1);
			
			size_t nbBones = ik.mListBones.size();
			for (size_t k = 0; k < nbBones; k++)
			{
				TiXmlElement* node = new TiXmlElement((k == 0) ? "BoneInfluenced" : "Bone");
				node_ik->LinkEndChild(node);

				node->SetAttribute("name", ik.mListBones.at(k).bone->name);
				if (group.type == LIBXENOVERSE_EMO_IK_TYPE_WEIGHT)
					node->SetDoubleAttribute("value", ik.mListBones.at(k).value);
			}
		}
	}


	if (listInverseKinematic2.size())
	{
		TiXmlElement* node_IK_list = new TiXmlElement("InverseKinematic2");
		entry_root->LinkEndChild(node_IK_list);

		size_t nbIk2 = listInverseKinematic2.size();
		for (size_t i = 0; i < nbIk2; i++)
		{
			const Emo_IK_Relation2 &ik2 = listInverseKinematic2.at(i);

			TiXmlElement* node_ik2 = new TiXmlElement("IK2");
			node_IK_list->LinkEndChild(node_ik2);
			node_ik2->SetAttribute("name", ik2.name);
			node_ik2->SetAttribute("unk_0",  ik2.unknow_0); 

			size_t nbBones = ik2.mListBones.size();
			for (size_t k = 0; k < nbBones; k++)
			{
				TiXmlElement* node = new TiXmlElement("Bone");
				node_ik2->LinkEndChild(node);
				node->SetAttribute("name", ik2.mListBones.at(k).bone->name);
			}
		}
	}



	root->LinkEndChild(entry_root);
}
void EMO_Bone::Decompile(TiXmlNode *root, bool hasExtraData) const
{
	TiXmlElement *entry_root = new TiXmlElement("Bone");
	entry_root->SetAttribute("name", name);

	entry_root->SetAttribute("parent", (parent) ? parent->name : "");					//notice : I didn't use "NULL" as before because I allready found "NULL" as a name. So I prefer "".
	entry_root->SetAttribute("child", (child) ? child->name : "");
	entry_root->SetAttribute("sibling", (sibling) ? sibling->name : "");
	entry_root->SetAttribute("EmgIndex", (emgIndex != 0xFFFF) ? ToString(emgIndex) : "");
	entry_root->SetAttribute("ik_flag", ik_flag);					//to test todo remove.
	
	entry_root->SetAttribute("unk_0", unknow_0);
	entry_root->SetAttribute("unk_1", FloatToString(unknow_1));
	

	entry_root->SetAttribute("debugIndex", EMO_BaseFile::UnsignedToString(debugIndex, true));			//todo remove.
	
	DecompileTransformationMatrix(entry_root, "RelativeTransformMatrix", relativeMatrix);

	if (haveAbsoluteMatrix)
		DecompileTransformationMatrix(entry_root, "AbsoluteTransformMatrix", absoluteMatrix);

	if (hasExtraData)
	{
		entry_root->SetAttribute("exUnk_0", unk_extraInfo_0);
		entry_root->SetAttribute("exUnk_1", unk_extraInfo_1);
		entry_root->SetAttribute("exUnk_2", EMO_BaseFile::UnsignedToString(unk_extraInfo_2,true));
		entry_root->SetAttribute("exUnk_3", unk_extraInfo_3);
	}

	root->LinkEndChild(entry_root);
}

void EMO_Bone::DecompileTransformationMatrix(TiXmlElement *root, const char *name, const float *matrix)
{
	std::vector<float> row;

	row.resize(4);

#ifdef FBX_SUPPORT

	FbxMatrix fbx_matrix(	matrix[0], matrix[1], matrix[2], matrix[3], 
							matrix[4], matrix[5], matrix[6], matrix[7],
							matrix[8], matrix[9], matrix[10], matrix[11],
							matrix[12], matrix[13], matrix[14], matrix[15]);

	FbxVector4 translation, scaling, shearing;
	FbxQuaternion rotation;
	double sign;
	fbx_matrix.GetElements(translation, rotation, shearing, scaling, sign);

	std::string comment = "Translation: " + EMO_BaseFile::FloatToString((float)translation[0]) + ", " + EMO_BaseFile::FloatToString((float)translation[1]) + ", " + EMO_BaseFile::FloatToString((float)translation[2]);
	comment += "	|	Rotation: " + EMO_BaseFile::FloatToString((float)rotation[0]) + ", " + EMO_BaseFile::FloatToString((float)rotation[1]) + ", " + EMO_BaseFile::FloatToString((float)rotation[2]) + ", " + EMO_BaseFile::FloatToString((float)rotation[3]);
	comment += "	|	Scaling: " + EMO_BaseFile::FloatToString((float)scaling[0]) + ", " + EMO_BaseFile::FloatToString((float)scaling[1]) + ", " + EMO_BaseFile::FloatToString((float)scaling[2]);
	comment += "	|	Shearing: " + EMO_BaseFile::FloatToString((float)shearing[0]) + ", " + EMO_BaseFile::FloatToString((float)shearing[1]) + ", " + EMO_BaseFile::FloatToString((float)shearing[2]);
	comment += "	|	Sign: " + EMO_BaseFile::FloatToString((float)sign);
	EMO_BaseFile::WriteComment(root, comment);

#endif

	TiXmlElement* absoluteTransformMatrixNode = new TiXmlElement(name);
	TiXmlElement* lineNode0 = new TiXmlElement("Line");
	TiXmlElement* lineNode1 = new TiXmlElement("Line");
	TiXmlElement* lineNode2 = new TiXmlElement("Line");
	TiXmlElement* lineNode3 = new TiXmlElement("Line");

	lineNode0->SetAttribute("x", FloatToString(matrix[0]));
	lineNode0->SetAttribute("y", FloatToString(matrix[1]));
	lineNode0->SetAttribute("z", FloatToString(matrix[2]));
	lineNode0->SetAttribute("w", FloatToString(matrix[3]));

	lineNode1->SetAttribute("x", FloatToString(matrix[4]));
	lineNode1->SetAttribute("y", FloatToString(matrix[5]));
	lineNode1->SetAttribute("z", FloatToString(matrix[6]));
	lineNode1->SetAttribute("w", FloatToString(matrix[7]));

	lineNode2->SetAttribute("x", FloatToString(matrix[8]));
	lineNode2->SetAttribute("y", FloatToString(matrix[9]));
	lineNode2->SetAttribute("z", FloatToString(matrix[10]));
	lineNode2->SetAttribute("w", FloatToString(matrix[11]));

	lineNode3->SetAttribute("x", FloatToString(matrix[12]));
	lineNode3->SetAttribute("y", FloatToString(matrix[13]));
	lineNode3->SetAttribute("z", FloatToString(matrix[14]));
	lineNode3->SetAttribute("w", FloatToString(matrix[15]));

	absoluteTransformMatrixNode->LinkEndChild(lineNode0);
	absoluteTransformMatrixNode->LinkEndChild(lineNode1);
	absoluteTransformMatrixNode->LinkEndChild(lineNode2);
	absoluteTransformMatrixNode->LinkEndChild(lineNode3);
	root->LinkEndChild(absoluteTransformMatrixNode);
}








bool EMO_Skeleton::Compile(TiXmlDocument *doc, bool big_endian)
{
	Reset();
	this->big_endian = big_endian;

	TiXmlHandle handle(doc);
	const TiXmlElement *root = EMO_BaseFile::FindRoot(&handle, "Skeleton");

	if (!root)
	{
		printf("Cannot find\"Skeleton\" in xml.\n");
		notifyError();
		return false;
	}

	return Compile(root);
}
bool EMO_Skeleton::Compile(const TiXmlElement *root)
{
	if (!root)
		return false;

	string str = "";
	root->QueryStringAttribute("skeletonUniqueId", &str); skeletonUniqueId = EMO_BaseFile::GetUnsigned64(str);
	root->QueryStringAttribute("unk_0", &str); unknow_0 = EMO_BaseFile::GetUnsigned(str);
	root->QueryStringAttribute("unk_1", &str); unknow_1 = EMO_BaseFile::GetUnsigned(str);
	root->QueryStringAttribute("unk_2", &str); unknow_2 = EMO_BaseFile::GetUnsigned(str);
	root->QueryStringAttribute("unk_3", &str); unknow_3 = EMO_BaseFile::GetUnsigned(str);
	root->QueryStringAttribute("unk_4", &str); unknow_4 = EMO_BaseFile::GetUnsigned(str);


	// Skeleton, first pass (all parameters with no relations between all bones)
	std::vector<const TiXmlElement*> listForBones;
	for (const TiXmlElement *elem = root->FirstChildElement("Bone"); elem != NULL; elem = elem->NextSiblingElement("Bone"))
	{
		EMO_Bone b;
		if (!b.Compile(elem, mHaveExtraBytesOnEachBone))
		{
			printf("%s: EMO_Bone compilation failed.\n", FUNCNAME);
			notifyError();
			return false;
		}
		bones.push_back(b);
		listForBones.push_back(elem);
	}

	// Skeleton, second pass (make relations when you have all bones)
	for (size_t i = 0, nb = bones.size(); i < nb; i++)
	{
		EMO_Bone &b = bones.at(i);
		const TiXmlElement *elem = listForBones.at(i);
		
		b.parent = (elem->QueryStringAttribute("parent", &str) == TIXML_SUCCESS) ? GetBone(str) : nullptr;
		b.child = (elem->QueryStringAttribute("child", &str) == TIXML_SUCCESS) ? GetBone(str) : nullptr;
		b.sibling = (elem->QueryStringAttribute("sibling", &str) == TIXML_SUCCESS) ? GetBone(str) : nullptr;
	}



	//read IK
	const TiXmlElement* node_IK_list = root->FirstChildElement("InverseKinematic");
	if (node_IK_list)
	{
		uint32_t tmp = 0;

		for (const TiXmlElement* node_group = node_IK_list->FirstChildElement("Group"); node_group; node_group = node_group->NextSiblingElement("Group"))
		{
			const TiXmlElement* node_ik = node_group->FirstChildElement("IK");
			if (!node_ik)
				continue;

			listInverseKinematic.push_back(Emo_IK_Group());

			node_group->QueryStringAttribute("type", &str);
			if (str == "Weight")
				listInverseKinematic.back().type = LIBXENOVERSE_EMO_IK_TYPE_WEIGHT;
			else if (str == "Equal")
				listInverseKinematic.back().type = LIBXENOVERSE_EMO_IK_TYPE_EQUAL;
			else
				continue;

			
			listInverseKinematic.back().mListIK.push_back(Emo_IK_Relation());
			Emo_IK_Relation &ik = listInverseKinematic.back().mListIK.back();
			node_ik->QueryUnsignedAttribute("unk_0", &tmp); ik.unknow_0 = (uint8_t)tmp;

			ik.unknow_1 = 0;
			if(listInverseKinematic.back().type == LIBXENOVERSE_EMO_IK_TYPE_EQUAL)
				node_ik->QueryUnsignedAttribute("unk_1", &tmp); ik.unknow_1 = (uint8_t)tmp;

			const TiXmlElement* node_influenced = node_ik->FirstChildElement("BoneInfluenced");
			if (!node_influenced)
			{
				printf("No 'BoneInfluenced' tags find. skip.'\n");
				notifyError();
				listInverseKinematic.pop_back();
				continue;
			}

			string name = "";
			double value = 0.0;
			EMO_Bone* eskBone = 0;
			size_t nbBones = bones.size();

			node_influenced->QueryStringAttribute("name", &name);
			node_influenced->QueryStringAttribute("value", &str);
			value = (listInverseKinematic.back().type == LIBXENOVERSE_EMO_IK_TYPE_WEIGHT) ? StringToFloat(str) : 0.0;

			for (size_t i = 0; i < nbBones; i++)
			{
				if (bones.at(i).name == name)
				{
					eskBone = &bones.at(i);
					break;
				}
			}

			if (eskBone)
			{
				ik.mListBones.push_back(Emo_IK_Relation::IKR_Bone(eskBone, (float)value));
			}else {
				printf("'BoneInfluenced' not found in bones declaration. skip.'\n");
				notifyError();
				listInverseKinematic.pop_back();
				continue;
			}


			for (const TiXmlElement* node = node_ik->FirstChildElement("Bone"); node; node = node->NextSiblingElement("Bone"))
			{
				name = "";
				value = 0.0;
				node->QueryStringAttribute("name", &name);
				node->QueryStringAttribute("value", &str);
				value = (listInverseKinematic.back().type == LIBXENOVERSE_EMO_IK_TYPE_WEIGHT) ? StringToFloat(str) : 0.0;

				for (size_t i = 0; i < nbBones; i++)
				{
					if (bones.at(i).name == name)
					{
						ik.mListBones.push_back(Emo_IK_Relation::IKR_Bone(&bones.at(i), (float)value));
						break;
					}
				}
			}
		}
	}







	

	//read IK2 (streetfighters)
	const TiXmlElement* node_IK2_list = root->FirstChildElement("InverseKinematic2");
	if (node_IK2_list)
	{
		uint32_t tmp = 0;
		size_t nbBones = bones.size();
		string name = "";
		EMO_Bone* eskBone = 0;


		for (const TiXmlElement* node_ik = node_IK2_list->FirstChildElement("IK2"); node_ik; node_ik = node_ik->NextSiblingElement("IK2"))
		{
			listInverseKinematic2.push_back(Emo_IK_Relation2());
			Emo_IK_Relation2 &ik = listInverseKinematic2.back();
			node_ik->QueryStringAttribute("name", &ik.name);
			node_ik->QueryUnsignedAttribute("unk_0", &ik.unknow_0);
			
			for (const TiXmlElement* node = node_ik->FirstChildElement("Bone"); node; node = node->NextSiblingElement("Bone"))
			{
				name = "";
				node->QueryStringAttribute("name", &name);

				for (size_t i = 0; i < nbBones; i++)
				{
					if (bones.at(i).name == name)
					{
						ik.mListBones.push_back(Emo_IK_Relation::IKR_Bone(&bones.at(i), 0.0));
						break;
					}
				}
			}
		}
	}

	
	return true;
}
bool EMO_Bone::Compile(const TiXmlElement *root, bool hasExtraData)
{
	if (root->QueryStringAttribute("name", &name) != 0)
	{
		printf("%s: Cannot get name of EMO_Bone.\n", FUNCNAME);
		notifyWarning();
	}

	string str = "";
	root->QueryStringAttribute("EmgIndex", &str);
	emgIndex = (str.length()) ? (uint16_t)std::stoi(str) : 0xFFFF;

	size_t tmp = 0;
	root->QueryUnsignedAttribute("ik_flag", &tmp);	ik_flag = tmp;				//to test perfect rebuild todo remove.
	root->QueryUnsignedAttribute("unk_0", &tmp); unknow_0 = tmp;
	root->QueryStringAttribute("unk_1", &str); unknow_1 = StringToFloat(str);

	
	if (hasExtraData)
	{
		root->QueryUnsignedAttribute("exUnk_0", &unk_extraInfo_0);
		root->QueryUnsignedAttribute("exUnk_1", &unk_extraInfo_1);
		root->QueryStringAttribute("exUnk_2", &str); unk_extraInfo_2 = EMO_BaseFile::GetUnsigned(str);
		root->QueryUnsignedAttribute("exUnk_3", &unk_extraInfo_3);
	}


	const TiXmlElement* relativeTransformMatrixNode = root->FirstChildElement("RelativeTransformMatrix");
	CompileTransformationMatrix(relativeTransformMatrixNode, relativeMatrix);


	const TiXmlElement* absoluteTransformMatrixNode = root->FirstChildElement("AbsoluteTransformMatrix");
	haveAbsoluteMatrix = (absoluteTransformMatrixNode != 0);
	CompileTransformationMatrix(absoluteTransformMatrixNode, absoluteMatrix);

	meta_original_offset = 0xFFFFFFFF;
	return true;
}
bool EMO_Bone::CompileTransformationMatrix(const TiXmlElement *root, float *matrix)
{
	if (!root)
		return false;
	
	const TiXmlElement* lineNode0 = root->FirstChildElement("Line");
	const TiXmlElement* lineNode1 = (lineNode0) ? lineNode0->NextSiblingElement("Line") : 0;
	const TiXmlElement* lineNode2 = (lineNode1) ? lineNode1->NextSiblingElement("Line") : 0;
	const TiXmlElement* lineNode3 = (lineNode2) ? lineNode2->NextSiblingElement("Line") : 0;
	if (!lineNode3)
	{
		printf("AbsoluteTransformMatrix incomplete number of 'Line'. skip.'\n");
		notifyError();
		return false;

	}

	string str = "";
	lineNode0->QueryStringAttribute("x", &str); matrix[0] = StringToFloat(str);
	lineNode0->QueryStringAttribute("y", &str); matrix[1] = StringToFloat(str);
	lineNode0->QueryStringAttribute("z", &str); matrix[2] = StringToFloat(str);
	lineNode0->QueryStringAttribute("w", &str); matrix[3] = StringToFloat(str);

	lineNode1->QueryStringAttribute("x", &str); matrix[4] = StringToFloat(str);
	lineNode1->QueryStringAttribute("y", &str); matrix[5] = StringToFloat(str);
	lineNode1->QueryStringAttribute("z", &str); matrix[6] = StringToFloat(str);
	lineNode1->QueryStringAttribute("w", &str); matrix[7] = StringToFloat(str);

	lineNode2->QueryStringAttribute("x", &str); matrix[8] = StringToFloat(str);
	lineNode2->QueryStringAttribute("y", &str); matrix[9] = StringToFloat(str);
	lineNode2->QueryStringAttribute("z", &str); matrix[10] = StringToFloat(str);
	lineNode2->QueryStringAttribute("w", &str); matrix[11] = StringToFloat(str);

	lineNode3->QueryStringAttribute("x", &str); matrix[12] = StringToFloat(str);
	lineNode3->QueryStringAttribute("y", &str); matrix[13] = StringToFloat(str);
	lineNode3->QueryStringAttribute("z", &str); matrix[14] = StringToFloat(str);
	lineNode3->QueryStringAttribute("w", &str); matrix[15] = StringToFloat(str);

	return true;
}







////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// ESK Convertions /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////



/*-------------------------------------------------------------------------------\
|                             readESKBone			                             |
\-------------------------------------------------------------------------------*/
void EMO_Bone::readESKBone(ESKBone* esk)
{
	name = esk->name;
	ik_flag = esk->ik_flag;
	unk_extraInfo_0 = esk->unk_extraInfo_0;
	unk_extraInfo_1 = esk->unk_extraInfo_1;
	unk_extraInfo_2 = esk->unk_extraInfo_2;
	unk_extraInfo_3 = esk->unk_extraInfo_3;

	haveAbsoluteMatrix = esk->haveAbsoluteMatrix;

	if (haveAbsoluteMatrix)
	{
		for (size_t i = 0; i < 16; i++)
			absoluteMatrix[i] = esk->absoluteMatrix[i];
	}

	double relativeTransform_a[12];									//special tranformation from observation between relativeTransform and transformMatrix
	for (size_t i = 0; i < 12; i++)
		relativeTransform_a[i] = esk->relativeTransform[i];

	double resultTransformMatrix[16];
	ESKBone::makeTransform4x4(&relativeTransform_a[0], &resultTransformMatrix[0]);
	ESKBone::transpose4x4(&resultTransformMatrix[0]);				//in Ogre, position is in last colone, but in Esk, position is on the last row, so we will use Transpose transformation.

	for (size_t i = 0; i < 16; i++)
		relativeMatrix[i] = (float)(resultTransformMatrix[i]);
}

/*-------------------------------------------------------------------------------\
|                             writeEsk				                             |
\-------------------------------------------------------------------------------*/
void EMO_Skeleton::writeEsk(ESK* esk)
{
	esk->name = name;
	esk->skeletonUniqueId = skeletonUniqueId;
	esk->mHaveExtraBytesOnEachBone = mHaveExtraBytesOnEachBone;

	esk->bones.clear();
	EskTreeNode* rootNode = esk->getTreeOrganisation();




	//to secure, we begin to build bones, and after we will made the hierarchy.
	std::vector<EskTreeNode*> listTreeNode;
	ESKBone* bone;
	EskTreeNode* node;
	size_t nbElements = bones.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		bone = new ESKBone();
		bones.at(i).writeESKBone(bone);
		esk->bones.push_back(bone);

		node = new EskTreeNode(esk->bones.at(i), i, rootNode);			//by default, rootnode is the parent of all.
		listTreeNode.push_back(node);
	}


	for (size_t i = 0; i < nbElements; i++)
	{
		EMO_Bone &emoBone = bones.at(i);

		EMO_Bone* emoBone_ptr = &emoBone;
		if (!emoBone_ptr->parent)									//we take care only bone witch have rootNode as parent.
		{
			rootNode->mChildren.push_back(listTreeNode.at(i));
			listTreeNode.at(i)->mParent = rootNode;

			writeEsk__recursive(emoBone_ptr, bones, listTreeNode.at(i), listTreeNode);		//after, we take care of children recursively.
		}
	}


	//all the recursive try to take care of child and sibling informations. saddly, there could have wrong information about child information.
	//saddly, by adding security on it, it could break the link witch could have with the parent information.
	//SO, we have to look after missing bone. if the parent informaiton is good, we could add it in hierarchy, else, it will still on rootNode.
	size_t nbNode = listTreeNode.size();
	for (size_t i = 0; i < nbNode; i++)											//we look at all bones
	{
		if (rootNode->getBoneWithIndex(i))
			continue;

		//if we miss someone in hierarchy.
		EMO_Bone &emoBone = bones.at(i);
		if (emoBone.parent)										//try with parent information (instead of child and sibling)
		{
			bool isGood = true;
			EMO_Bone* emoBone_tmp = emoBone.parent;
			while (emoBone_tmp)
			{
				if (emoBone_tmp == &emoBone)						//try to avoid bone be the parent of the parent, or him self directly.
				{
					isGood = false;
					break;
				}
				emoBone_tmp = emoBone_tmp->parent;
			}

			if (isGood)
			{
				size_t isFound = (size_t)-1;
				size_t nbElements = bones.size();
				for (size_t j = 0; j < nbElements; j++)
				{
					if (&bones.at(j) == emoBone.parent)
					{
						isFound = j;
						break;
					}
				}

				if (isFound != (size_t)-1)					//if we find it, we update the treeNode.
				{
					//another check, if there isn't a fail on sibling, to avoid infinite loop
					size_t nbchildren_tmp = listTreeNode.at(isFound)->mChildren.size();
					for (size_t m = 0; m < nbchildren_tmp; m++)
					{
						if (listTreeNode.at(isFound)->mChildren.at(m) == listTreeNode.at(i))
						{
							printf("warning: There is a strange thing about hierarchy on %s. skipped for avoid infinite loops.\n", listTreeNode.at(i)->mBone->getName().c_str());
							LibXenoverse::notifyWarning();
							isFound = (size_t)-1;
							break;
						}
					}

					if (isFound != (size_t)-1)
					{
						listTreeNode.at(i)->mParent = listTreeNode.at(isFound);
						listTreeNode.at(isFound)->mChildren.push_back(listTreeNode.at(i));
					}
					else {
						rootNode->mChildren.push_back(listTreeNode.at(i));
						listTreeNode.at(i)->mParent = rootNode;
					}

				}
				else {
					rootNode->mChildren.push_back(listTreeNode.at(i));
					listTreeNode.at(i)->mParent = rootNode;
				}

			}
			else {
				rootNode->mChildren.push_back(listTreeNode.at(i));
				listTreeNode.at(i)->mParent = rootNode;
			}

		}
		else {
			rootNode->mChildren.push_back(listTreeNode.at(i));
			listTreeNode.at(i)->mParent = rootNode;
		}
	}


	esk->setTreeOrganisation(rootNode);




	//Ik datas
	esk->listInverseKinematic.clear();
	size_t nbBones_all = esk->bones.size();
	size_t nbGroup = listInverseKinematic.size();
	for (size_t i = 0; i < nbGroup; i++)
	{
		Emo_IK_Group &group = listInverseKinematic.at(i);
		esk->listInverseKinematic.push_back(Esk_IK_Group());

		size_t nbIk = group.mListIK.size();
		for (size_t j = 0; j < nbIk; j++)
		{
			Emo_IK_Relation &ik = group.mListIK.at(j);
			esk->listInverseKinematic.back().mListIK.push_back(Esk_IK_Relation());
			Esk_IK_Relation &ik_b = esk->listInverseKinematic.back().mListIK.back();

			size_t nbBones = ik.mListBones.size();
			for (size_t k = 0; k < nbBones; k++)
			{
				EMO_Bone* bone = ik.mListBones.at(k).bone;
				ESKBone* bone_b = 0;

				for (size_t m = 0; m < nbBones_all; m++)
				{
					if (esk->bones.at(m)->getName() == bone->name)
					{
						bone_b = esk->bones.at(m);
						break;
					}
				}
				if (!bone_b)
					continue;

				ik_b.mListBones.push_back(Esk_IK_Relation::IKR_Bone(bone_b, ik.mListBones.at(k).value));
			}
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             writeEsk__recursive				                 |
\-------------------------------------------------------------------------------*/
void EMO_Skeleton::writeEsk__recursive(EMO_Bone* emoBone, std::vector<EMO_Bone> &bones, EskTreeNode* treeNode, std::vector<EskTreeNode*> &listTreeNode)
{
	//we look after all children (child and sibling), to make the hierarchy.
	EMO_Bone* emoBone_child = emoBone->child;
	size_t inc = 0;

	while (emoBone_child != nullptr)
	{
		//to secure on wrong informations about hierarchy (in some emo), we have to break possible infinite loops.
		if (emoBone_child == emoBone)				//I see a case witch the bone was its parent himself, and the child ...
		{
			printf("warning: There is a strange thing about hierarchy on %s. skipped for avoid infinite loops.\n", emoBone_child->GetName().c_str());
			LibXenoverse::notifyWarning();
			return;
		}

		EMO_Bone* emoBone_tmp = emoBone->parent;
		while (emoBone_tmp)										//case of a bad link between parent/child. lets check if parents are not used as a child of a child.
		{
			if (emoBone_child == emoBone_tmp)
			{
				printf("warning: There is a strange thing about hierarchy on %s. skipped for avoid infinite loops.\n", emoBone_child->GetName().c_str());
				LibXenoverse::notifyWarning();
				return;
			}
			emoBone_tmp = emoBone_tmp->parent;
		}


		//normal case:

		size_t isFound = (size_t)-1;
		size_t nbElements = bones.size();
		for (size_t j = 0; j < nbElements; j++)
		{
			if (&bones.at(j) == emoBone_child)
			{
				isFound = j;
				break;
			}
		}

		if (isFound != (size_t)-1)					//if we find it, we update the treeNode.
		{
			listTreeNode.at(isFound)->mParent = treeNode;


			//another check, if there isn't a fail on sibling, to avoid infinite loop
			size_t nbchildren_tmp = treeNode->mChildren.size();
			for (size_t j = 0; j < nbchildren_tmp; j++)
			{
				if (treeNode->mChildren.at(j) == listTreeNode.at(isFound))
				{
					printf("warning: There is a strange thing about hierarchy on %s. skipped for avoid infinite loops.\n", emoBone_child->GetName().c_str());
					LibXenoverse::notifyWarning();
					return;
				}
			}

			treeNode->mChildren.push_back(listTreeNode.at(isFound));

			writeEsk__recursive(emoBone_child, bones, listTreeNode.at(isFound), listTreeNode);		//same for children
		}

		emoBone_child = emoBone_child->sibling;
	}
}
/*-------------------------------------------------------------------------------\
|                             writeESKBone			                             |
\-------------------------------------------------------------------------------*/
void EMO_Bone::writeESKBone(ESKBone* esk)
{
	esk->name = name;
	esk->ik_flag = ik_flag;
	esk->unk_extraInfo_0 = unk_extraInfo_0;
	esk->unk_extraInfo_1 = unk_extraInfo_1;
	esk->unk_extraInfo_2 = unk_extraInfo_2;
	esk->unk_extraInfo_3 = unk_extraInfo_3;



	esk->haveAbsoluteMatrix = haveAbsoluteMatrix;

	if (esk->haveAbsoluteMatrix)
	{
		for (size_t i = 0; i < 16; i++)
			esk->absoluteMatrix[i] = absoluteMatrix[i];
	}




	double tmpTransformMatrix[16];

	for (size_t i = 0; i < 16; i++)
		tmpTransformMatrix[i] = relativeMatrix[i];

	ESKBone::transpose4x4(&tmpTransformMatrix[0]);		//come back to Ogre matrice way
	double relativeTransform_b[12];						//special tranformation from observation between relativeTransform and relativeMatrix
	ESKBone::decomposition4x4(&tmpTransformMatrix[0], &relativeTransform_b[0]);

	for (size_t i = 0; i < 12; i++)
		esk->relativeTransform[i] = (float)relativeTransform_b[i];
}







////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// Fbx Convertions /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////



#ifdef FBX_SUPPORT

bool EMO_Skeleton::ExportFbxBone(const EMO_Bone *parent, FbxNode *root_node, FbxScene *scene, std::vector<FbxNode *> &fbx_bones) const
{
	for (size_t i = 0; i < bones.size(); i++)
	{
		const EMO_Bone &bone = bones[i];

		if (bone.parent != parent)
			continue;

		FbxSkeleton* skeleton_root_attribute = FbxSkeleton::Create(scene, bone.name.c_str());

		if (!parent)
		{
			skeleton_root_attribute->SetSkeletonType(FbxSkeleton::eRoot);
		}
		else
		{
			skeleton_root_attribute->SetSkeletonType(FbxSkeleton::eLimbNode);
		}

		FbxNode* fbx_bone_node = FbxNode::Create(scene, bone.name.c_str());
		fbx_bone_node->SetNodeAttribute(skeleton_root_attribute);
		root_node->AddChild(fbx_bone_node);

		FbxAMatrix matrix = bone.GetGlobalTransform();

		/*bool invert_yz = false;
		bool change_y = false;

		if (invert_yz)
		{
			FbxVector4 translation, rotation, scaling;
			double temp;

			translation = matrix.GetT();
			rotation = matrix.GetR();
			scaling = matrix.GetS();

			temp = translation[1];
			translation[1] = translation[2];
			translation[2] = temp;

			temp = rotation[1];
			rotation[1] = rotation[2];
			rotation[2] = temp;

			temp = scaling[1];
			scaling[1] = scaling[2];
			scaling[2] = temp;

			if (change_y)
			{
				translation[1] = -translation[1];
				rotation[1] = -rotation[1];
			}

			matrix.SetT(translation);
			matrix.SetR(rotation);
			matrix.SetS(scaling);
		}*/

		EMO_Bone::SetGlobalDefaultPosition(fbx_bone_node, matrix);

		fbx_bones[i] = fbx_bone_node;
		ExportFbxBone(&bone, fbx_bone_node, scene, fbx_bones);
	}

	return true;
}

bool EMO_Skeleton::ExportFbx(FbxScene *scene, std::vector<FbxNode *> &fbx_bones) const
{
	fbx_bones.resize(bones.size());
	return ExportFbxBone(nullptr, scene->GetRootNode(), scene, fbx_bones);
}

#endif







//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


EMO_Bone::EMO_Bone(void)
{
	parent = nullptr;
	child = nullptr;
	sibling = nullptr;
	emgIndex = unknow_0 = 0xffff;
	unknow_0 = ik_flag = unk_extraInfo_0 = unk_extraInfo_1 = unk_extraInfo_3 = 0;
	unknow_1 = 0.0;
	unk_extraInfo_2 = 0xffff;
	debugIndex = 0;

	haveAbsoluteMatrix = false;

	for (size_t i = 0; i < 16; i++)
	{
		relativeMatrix[i] = 0.0f;
		absoluteMatrix[i] = 0.0f;
	}
	relativeMatrix[0] = relativeMatrix[5] = relativeMatrix[10] = relativeMatrix[15] = 1.0f;			//Matrix iddentity
}








#ifdef FBX_SUPPORT

FbxAMatrix EMO_Bone::GetGlobalTransform() const
{
	FbxAMatrix bone_matrix = EMO_Bone::ArrayToFbxAMatrix(relativeMatrix);

    if (parent)
        bone_matrix = parent->GetGlobalTransform() * bone_matrix;

    return bone_matrix;
}

void EMO_Bone::FbxMatrixToArray(float *mo, const FbxMatrix *mi)
{
	for (int i = 0; i < 4; i++)
	{
		FbxVector4 row = mi->GetRow(i);

		mo[i] = (float)row[0];
		mo[i + 4] = (float)row[1];
		mo[i + 8] = (float)row[2];
		mo[i + 12] = (float)row[3];
	}
}

void EMO_Bone::FbxMatrixToArray(double *mo, const FbxMatrix *mi)
{
	for (int i = 0; i < 4; i++)
	{
		FbxVector4 row = mi->GetColumn(i);

		mo[i] = row[0];
		mo[i + 4] = row[1];
		mo[i + 8] = row[2];
		mo[i + 12] = row[3];
	}
}

FbxMatrix EMO_Bone::ArrayToFbxMatrix(const float *mi)
{
	return FbxMatrix(mi[0], mi[1], mi[2], mi[3],
		mi[4], mi[5], mi[6], mi[7],
		mi[8], mi[9], mi[10], mi[11],
		mi[12], mi[13], mi[14], mi[15]);
}

FbxAMatrix EMO_Bone::ArrayToFbxAMatrix(const float *mi)
{
	FbxMatrix m = ArrayToFbxMatrix(mi);
	FbxVector4 translation, rotation, scaling, shearing;
	double sign;

	m.GetElements(translation, rotation, shearing, scaling, sign);
	return FbxAMatrix(translation, rotation, scaling);
}

FbxAMatrix EMO_Bone::GetGlobalDefaultPosition(FbxNode* node)
{
	FbxAMatrix local_position;
	FbxAMatrix global_position;
	FbxAMatrix parent_global_position;

	local_position.SetT(node->LclTranslation.Get());
	local_position.SetR(node->LclRotation.Get());
	local_position.SetS(node->LclScaling.Get());

	if (node->GetParent())
	{
		parent_global_position = GetGlobalDefaultPosition(node->GetParent());
		global_position = parent_global_position * local_position;
	}
	else
	{
		global_position = local_position;
	}

	return global_position;
}

void EMO_Bone::SetGlobalDefaultPosition(FbxNode* node, FbxAMatrix global_position)
{
	FbxAMatrix local_position;
	FbxAMatrix parent_global_position;

	if (node->GetParent())
	{
		parent_global_position = GetGlobalDefaultPosition(node->GetParent());
		local_position = parent_global_position.Inverse() * global_position;
	}
	else
	{
		local_position = global_position;
	}

	node->LclTranslation.Set(local_position.GetT());
	node->LclRotation.Set(local_position.GetR());
	node->LclScaling.Set(local_position.GetS());
}

#endif



bool EMO_Bone::PartialCompare(const EMO_Bone *b1, const EMO_Bone *b2)
{
    if (b1 == b2)
        return true;

    if( (!b1) || (!b2) || 
		(b1->name != b2->name) ||
		(b1->haveAbsoluteMatrix != b2->haveAbsoluteMatrix) ||
		(b1->emgIndex != b2->emgIndex) ||
		(b1->unknow_0 != b2->unknow_0) ||
		(b1->ik_flag != b2->ik_flag) ||
		(b1->unknow_1 != b2->unknow_1) ||
		(b1->unk_extraInfo_0 != b2->unk_extraInfo_0) ||
		(b1->unk_extraInfo_1 != b2->unk_extraInfo_1) ||
		(b1->unk_extraInfo_2 != b2->unk_extraInfo_2) ||
		(b1->unk_extraInfo_3 != b2->unk_extraInfo_3) ||
		((b1->parent != b2->parent) && ((!b1->parent) || (!b2->parent) || (b1->parent->name != b2->parent->name))) ||
		((b1->child != b2->child) && ((!b1->child) || (!b2->child) || (b1->child->name != b2->child->name))) || 
		((b1->sibling != b2->sibling) && ((!b1->sibling) || (!b2->sibling) || (b1->sibling->name != b2->sibling->name))) ||
		(memcmp(b1->relativeMatrix, b2->relativeMatrix, sizeof(b1->relativeMatrix)) != 0) ||
		((b1->haveAbsoluteMatrix) && (memcmp(b1->absoluteMatrix, b2->absoluteMatrix, sizeof(b1->absoluteMatrix)) != 0))
		)
    {
        return false;
    }

    return true;
}

bool EMO_Bone::operator==(const EMO_Bone &rhs) const
{
    if (!PartialCompare(this, &rhs))
        return false;

    if (!PartialCompare(this->parent, rhs.parent))
        return false;

    if (!PartialCompare(this->child, rhs.child))
        return false;

    if (!PartialCompare(this->sibling, rhs.sibling))
        return false;

	return true;
}

EMO_Skeleton::EMO_Skeleton(void)
{
    big_endian = false;
    Reset();
}

EMO_Skeleton::EMO_Skeleton(uint8_t *buf, unsigned int size)
{
    big_endian = false;
	Reset();

    Load(buf, size);
}

EMO_Skeleton::~EMO_Skeleton()
{
    Reset();
}

void EMO_Skeleton::Copy(const EMO_Skeleton &other)
{
	this->listInverseKinematic = other.listInverseKinematic;
	this->listInverseKinematic2 = other.listInverseKinematic2;
	this->unknow_0 = other.unknow_0;
	this->unknow_1 = other.unknow_1;
	this->unknow_2 = other.unknow_2;
	this->unknow_3 = other.unknow_3;
	this->unknow_4 = other.unknow_4;
    this->skeletonUniqueId = other.skeletonUniqueId;
	this->context = context;

    this->bones = other.bones;

    for (EMO_Bone &b : bones)
    {
        if (b.parent)
            b.parent = GetBone(b.parent->name);
		
		if (b.child)
			b.child = GetBone(b.child->name);
		
		if (b.sibling)
			b.sibling = GetBone(b.sibling->name);
    }

    this->big_endian = other.big_endian;
}

void EMO_Skeleton::Reset()
{
	mHaveExtraBytesOnEachBone = true;					//by default object have Extra, but camera light or material animation don't have Extra.
	unknow_0 = unknow_1 = unknow_2 = unknow_3 = unknow_4 = 0;
	skeletonUniqueId = 0;

	context = ContextUse::Object;
	contextPtr = 0;

    bones.clear();
	listInverseKinematic.clear();
}

uint16_t EMO_Skeleton::FindBone(const std::vector<EMO_Bone *> &bones, EMO_Bone *bone, bool assert_if_not_found)
{
    if(bone == nullptr)
		return 0xFFFF;

    for (size_t i = 0; i < bones.size(); i++)
        if (bones.at(i) == bone)
            return i;

    if (assert_if_not_found) { assert(0); }

    return 0xFFFF;
}

void EMO_Skeleton::RebuildSkeleton(const std::vector<EMO_Bone *> &old_bones_ptr)
{
    for (EMO_Bone &b : bones)
    {
        if (b.parent != nullptr)
            b.parent = &bones[FindBone(old_bones_ptr, b.parent, true)];

        if (b.child != nullptr)
            b.child = &bones[FindBone(old_bones_ptr, b.child, true)];

        if (b.sibling != nullptr)
            b.sibling = &bones[FindBone(old_bones_ptr, b.sibling, true)];
    }
}


EMO_Bone* EMO_Skeleton::GetBone(uint16_t idx)
{
	if (idx >= bones.size())
		return nullptr;

	return &bones[idx];
}

EMO_Bone* EMO_Skeleton::GetBone(const std::string &name)
{
	if (name.length() == 0)
		return nullptr;

	for (EMO_Bone &b : bones)
		if (b.name == name)
			return &b;

	return nullptr;
}

const EMO_Bone* EMO_Skeleton::GetBone(const std::string &name) const
{
	for (const EMO_Bone &b : bones)
		if (b.name == name)
			return &b;

	return nullptr;
}

uint16_t EMO_Skeleton::AppendBone(const EMO_Skeleton &other, const std::string &name)
{
    const EMO_Bone *bone = other.GetBone(name);
    if (!bone)
        return (uint16_t)-1;

    return AppendBone(*bone);
}

uint16_t EMO_Skeleton::AppendBone(const EMO_Bone &bone)
{
    std::vector<EMO_Bone *> old_bones_ptr;
    EMO_Bone new_bone = bone;

    new_bone.parent = nullptr;
    new_bone.child = nullptr;
    new_bone.sibling = nullptr;
    new_bone.emgIndex = 0xffff;
    new_bone.unknow_0 = 0xffff;

    for (EMO_Bone &b : bones)
    {
        old_bones_ptr.push_back(&b);
    }

    bones.push_back(new_bone);
    RebuildSkeleton(old_bones_ptr);

    return (bones.size()-1);
}

bool EMO_Skeleton::CloneEMO_BoneParentChild(const EMO_Skeleton &other, const std::string &bone_name, EMO_Bone **not_found)
{
    EMO_Bone *bone_me;
    const EMO_Bone *bone_other;

    //LOG_DEBUG("Clone: %s\n", bone_name.c_str());

    if (not_found)
        *not_found = nullptr;

    bone_me = GetBone(bone_name);
    if (!bone_me)
        return false;

    bone_other = other.GetBone(bone_name);
    if (!bone_other)
        return false;

    EMO_Bone bone_temp = EMO_Bone(*bone_me);

    if (!bone_other->parent)
    {
        bone_temp.parent = nullptr;
    }
    else
    {
        EMO_Bone *parent = GetBone(bone_other->parent->name);

        if (!parent)
        {
            *not_found = bone_other->parent;
            return false;
        }

        bone_temp.parent = parent;
    }

    if (!bone_other->child)
    {
        bone_temp.child = nullptr;
    }
    else
    {
        EMO_Bone *child = GetBone(bone_other->child->name);

        if (!child)
        {
            *not_found = bone_other->child;
            return false;
        }

        bone_temp.child = child;
    }

    if (!bone_other->sibling)
    {
        bone_temp.sibling = nullptr;
    }
    else
    {
        EMO_Bone *sibling = GetBone(bone_other->sibling->name);

        if (!sibling)
        {
            *not_found = bone_other->sibling;
            return false;
        }

        bone_temp.sibling = sibling;
    }

	bone_temp.emgIndex = bone_other->emgIndex;
	bone_temp.unknow_0 = bone_other->unknow_0;

    //bones[BoneToIndex(bone_me)] = bone_temp;
    *bone_me = bone_temp;
    return true;
}

uint16_t EMO_Skeleton::BoneToIndex(EMO_Bone *bone) const
{
    if (!bone)
        return 0xFFFF;

    for (size_t i = 0; i < bones.size(); i++)
    {
        if (bone == &bones[i])
            return i;
    }

	printf("%s: We are about to crash or abort.\n", FUNCNAME);
	printf("Cannot find bone \"%s\" in this skeleton\n", bone->name.c_str());
	LibXenoverse::notifyError();

    //LOG_DEBUG("%s: coding error somewhere. This bone is not in this object (%s)\n", FUNCNAME, bone->name.c_str());
    throw std::runtime_error("Aborting");
    return 0xFFFF;
}



bool EMO_Skeleton::operator==(const EMO_Skeleton &rhs) const
{
	size_t nbIk = listInverseKinematic.size();
	size_t nbIk2 = listInverseKinematic2.size();
	if ((nbIk != rhs.listInverseKinematic.size()) || (nbIk2 != rhs.listInverseKinematic2.size()))
		return false;

	for (size_t i = 0; i < nbIk; i++)
		if (listInverseKinematic.at(i) != rhs.listInverseKinematic.at(i))
			return false;

	for (size_t i = 0; i < nbIk; i++)
		if (listInverseKinematic2.at(i) != rhs.listInverseKinematic2.at(i))
			return false;

	if( (this->unknow_0 != rhs.unknow_0) ||
		(this->unknow_1 != rhs.unknow_1) ||
		(this->unknow_2 != rhs.unknow_2) ||
		(this->unknow_3 != rhs.unknow_3) ||
		(this->unknow_4 != rhs.unknow_4) ||
		(this->skeletonUniqueId != rhs.skeletonUniqueId) ||
		(this->bones != rhs.bones)  )
		return false;

    return true;
}









}