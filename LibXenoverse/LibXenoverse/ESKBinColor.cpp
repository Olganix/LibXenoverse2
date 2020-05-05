
#include "ESK.h"

#include <sstream>
#include <iomanip>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>



namespace LibXenoverse
{



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// wxHexEditor coloration ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////





/*-------------------------------------------------------------------------------\
|                             save_Coloration		                             |
\-------------------------------------------------------------------------------*/
void ESK::save_Coloration(string filename, bool show_error)
{
	uint8_t *buf;
	size_t size;

	buf = EMO_BaseFile::ReadFile(filename, &size, show_error);
	if (!buf)
		return;


	BinColorTag binCt;

	write_Coloration(binCt, binCt.startColoration(filename), buf, size);

	binCt.endColoration(filename);

	return;
}




/*-------------------------------------------------------------------------------\
|                             write_Coloration				                     |
\-------------------------------------------------------------------------------*/
void ESK::write_Coloration(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	ESK_Header* hdr = (ESK_Header*)buf;
	if (size < sizeof(ESK_Header) || memcmp(hdr->signature, LIBXENOVERSE_ESK_SIGNATURE, 4) != 0)
		return;


	std::vector<bool> listBytesAllreadyTagged;
	listBytesAllreadyTagged.resize(size, false);				//to check override of the same byte (overflow)


	size_t idTag = 0;
	size_t offset = 0;
	size_t size_padding = 0;
	bool isBigFile = false;														//to reduce number of tag on vertex and faces, avoid too much memory
	size_t incSection = 0;
	size_t incParam = 0;

	size_t startAdress = 0;
	uint32_t *name_offset = 0;
	string name_tmp = "";

	binCt.write_Coloration_Tag("signature", "string", "", offset, 4 * sizeof(char), "ESK_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(char);
	binCt.write_Coloration_Tag("endian", "uint16_t", "", offset, sizeof(uint16_t), "ESK_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("header_size", "uint16_t", "", offset, sizeof(uint16_t), "ESK_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("version", "4 * uint8_t", "", offset, 4 * sizeof(uint8_t), "ESK_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(uint8_t);
	binCt.write_Coloration_Tag("unknow_0", "uint32_t", "", offset, sizeof(uint32_t), "ESK_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("offset_skeleton", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + hdr->offset_skeleton, true) + " ESK_Skeleton_Section", offset, sizeof(uint32_t), "ESK_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("offset_nextPart", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + hdr->offset_nextPart, true) + " Next Part (like Emd  for Nsk)", offset, sizeof(uint32_t), "ESK_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow_1", "uint32_t", "", offset, sizeof(uint32_t), "ESK_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow_2", "uint32_t", "", offset, sizeof(uint32_t), "ESK_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

	size_t offset_ESK_Skeleton_Section = offset = hdr->offset_skeleton;
	ESK_Skeleton_Section* esk_skeleton_Section = (ESK_Skeleton_Section*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_ESK_Skeleton_Section));
	incSection++;
	incParam = 0;

	binCt.write_Coloration_Tag("number_bones", "uint16_t", "", offset, sizeof(uint16_t), "ESK_Skeleton_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("unknow_flag", "uint16_t", "", offset, sizeof(uint16_t), "ESK_Skeleton_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("offset_bonesHierarchy", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + esk_skeleton_Section->offset_bonesHierarchy, true) + " ESK_Bone_Hierarchy", offset, sizeof(uint32_t), "ESK_Skeleton_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("offset_boneNames", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + esk_skeleton_Section->offset_boneNames, true) + " Bones Names", offset, sizeof(uint32_t), "ESK_Skeleton_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("offset_relTransforms", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + esk_skeleton_Section->offset_relTransforms, true) + " ESK_Bone_Relative_Transform", offset, sizeof(uint32_t), "ESK_Skeleton_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("offset_absMatrices", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + esk_skeleton_Section->offset_absMatrices, true) + " ESK_Bone_Absolute_Matrix", offset, sizeof(uint32_t), "ESK_Skeleton_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("offset_IKs", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + esk_skeleton_Section->offset_IKs, true) + " ESK_IK_Section", offset, sizeof(uint32_t), "ESK_Skeleton_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("offset_boneExtraInfo", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + esk_skeleton_Section->offset_boneExtraInfo, true) + " ESK_Bone_extraInfo", offset, sizeof(uint32_t), "ESK_Skeleton_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow_0", "uint32_t", "", offset, sizeof(uint32_t), "ESK_Skeleton_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow_1", "uint32_t", "", offset, sizeof(uint32_t), "ESK_Skeleton_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);


	incSection++;
	incParam = 0;
	ESK_Bone_Hierarchy* esk_Bone_Hierarchy = (ESK_Bone_Hierarchy*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_ESK_Skeleton_Section + esk_skeleton_Section->offset_bonesHierarchy));
	for (size_t i = 0; i < esk_skeleton_Section->number_bones; i++)
	{
		size_t offset_ESK_Bone_Hierarchy = offset = offset_ESK_Skeleton_Section + esk_skeleton_Section->offset_bonesHierarchy + i * sizeof(ESK_Bone_Hierarchy);
		incParam = 0;

		binCt.write_Coloration_Tag("parent_index", "uint16_t", "", offset, sizeof(uint16_t), "ESK_Bone_Hierarchy", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("child_index", "uint16_t", "", offset, sizeof(uint16_t), "ESK_Bone_Hierarchy", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("sibling_index", "uint16_t", "", offset, sizeof(uint16_t), "ESK_Bone_Hierarchy", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("ik_flag", "uint16_t", "4 if Bone is Start of one IK, else 0", offset, sizeof(uint16_t), "ESK_Bone_Hierarchy", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	}



	incSection++;
	incParam = 0;
	offset = offset_ESK_Skeleton_Section + esk_skeleton_Section->offset_boneNames;
	size_t endOffset = offset;
	for (size_t i = 0; i < esk_skeleton_Section->number_bones; i++)
	{
		name_offset = (uint32_t*)EMO_BaseFile::GetOffsetPtr_native(buf, offset);

		binCt.write_Coloration_Tag("offset_bone_name", "uint32_t", "", offset, sizeof(uint32_t), "ESK_BoneNames", parent, idTag++, incSection, i, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		if (name_offset)
		{
			name_tmp = string((char*)EMO_BaseFile::GetOffsetPtr_native(buf, offset_ESK_Skeleton_Section + (*name_offset)));
			binCt.write_Coloration_Tag("bone_name", "string", "", offset_ESK_Skeleton_Section + (*name_offset), (name_tmp.size() + 1) * sizeof(char), "ESK_BoneNames", parent, idTag++, incSection + 1, i, listBytesAllreadyTagged, (size_t)-1, false);

			size_t endoffset_tmp = offset_ESK_Skeleton_Section + (*name_offset) + (name_tmp.size() + 1) * sizeof(char);
			if (endOffset < endoffset_tmp)
				endOffset = endoffset_tmp;
		}
	}
	offset = endOffset;
	size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
	if (size_padding)
		binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "ESK_BoneNames", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged);
	offset += size_padding;

	incSection++;


	incSection++;
	incParam = 0;
	ESK_Bone_Relative_Transform* esk_Bone_Relative_Transform = (ESK_Bone_Relative_Transform*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_ESK_Skeleton_Section + esk_skeleton_Section->offset_bonesHierarchy));
	for (size_t i = 0; i < esk_skeleton_Section->number_bones; i++)
	{
		size_t offset_ESK_Bone_Relative_Transform = offset = offset_ESK_Skeleton_Section + esk_skeleton_Section->offset_relTransforms + i * sizeof(ESK_Bone_Relative_Transform);
		incParam = 0;

		binCt.write_Coloration_Tag("position", "4 * float", "", offset, 4 * sizeof(float), "ESK_Bone_Relative_Transform", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);
		binCt.write_Coloration_Tag("orientation", "4 * float", "quaternion", offset, 4 * sizeof(float), "ESK_Bone_Relative_Transform", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);
		binCt.write_Coloration_Tag("scale", "4 * float", "", offset, 4 * sizeof(float), "ESK_Bone_Relative_Transform", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);
	}


	incSection++;
	incParam = 0;
	if (esk_skeleton_Section->offset_absMatrices)
	{
		ESK_Bone_Absolute_Matrix* esk_Bone_Absolute_Matrix = (ESK_Bone_Absolute_Matrix*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_ESK_Skeleton_Section + esk_skeleton_Section->offset_absMatrices));
		for (size_t i = 0; i < esk_skeleton_Section->number_bones; i++)
		{
			size_t offset_ESK_Bone_Absolute_Matrix = offset = offset_ESK_Skeleton_Section + esk_skeleton_Section->offset_absMatrices + i * sizeof(ESK_Bone_Absolute_Matrix);
			incParam = 0;

			binCt.write_Coloration_Tag("m0", "4 * float", "", offset, 4 * sizeof(float), "ESK_Bone_Absolute_Matrix", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);
			binCt.write_Coloration_Tag("m1", "4 * float", "", offset, 4 * sizeof(float), "ESK_Bone_Absolute_Matrix", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);
			binCt.write_Coloration_Tag("m2", "4 * float", "", offset, 4 * sizeof(float), "ESK_Bone_Absolute_Matrix", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);
			binCt.write_Coloration_Tag("m3", "4 * float", "", offset, 4 * sizeof(float), "ESK_Bone_Absolute_Matrix", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);
		}
	}


	incSection++;
	incParam = 0;
	size_t incSection_IKs = incSection;
	size_t incSection_IkGroup = incSection + 1;
	size_t incSection_Ik = incSection + 2;
	size_t incSection_IkRelation = incSection + 3;
	size_t incSection_IkRelationB = incSection + 4;
	if (esk_skeleton_Section->offset_IKs)
	{
		incSection = incSection_IKs;
		incParam = 0;
		offset = offset_ESK_Skeleton_Section + esk_skeleton_Section->offset_IKs;
		ESK_IK_Section* esk_IK_Section = (ESK_IK_Section*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
		binCt.write_Coloration_Tag("number_IkGroups", "uint32_t", "", offset, sizeof(uint32_t), "ESK_IK_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		for (size_t i = 0; i < esk_IK_Section->number_IkGroups; i++)
		{
			incSection = incSection_IkGroup;
			incParam = 0;

			size_t offset_ESK_IK_Group = offset;

			ESK_IK_Group* esk_IK_Group = (ESK_IK_Group*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
			binCt.write_Coloration_Tag("typeGroup", "uint16_t", "", offset, sizeof(uint16_t), "ESK_IK_Group", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("sizeGroup", "uint16_t", "number of bytes for the groups", offset, sizeof(uint16_t), "ESK_IK_Group", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);

			if (esk_IK_Group->typeGroup == 1)
			{
				incSection = incSection_Ik;
				incParam = 0;

				ESK_IK* esk_IK = (ESK_IK*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
				binCt.write_Coloration_Tag("boneIndex", "uint8_t", "", offset, sizeof(uint8_t), "ESK_IK", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
				binCt.write_Coloration_Tag("number_relations", "uint8_t", "", offset, sizeof(uint8_t), "ESK_IK", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);


				incSection = incSection_IkRelation;
				incParam = 0;

				for (size_t k = 0; k <= esk_IK->number_relations; k++)
				{
					binCt.write_Coloration_Tag("boneIndex", "uint16_t", "", offset, sizeof(uint16_t), "ESK_IK_Relation", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
				}
				if (esk_IK->number_relations % 2 != 0)					//padding 32 bits
				{
					binCt.write_Coloration_Tag("padding", "uint16_t", "", offset, sizeof(uint16_t), "ESK_IK_Relation", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
				}


				incSection = incSection_IkRelationB;
				incParam = 0;

				for (size_t k = 0; k <= esk_IK->number_relations; k++)
				{
					binCt.write_Coloration_Tag("boneRelationWeight", "float", "", offset, sizeof(float), "ESK_IK_Relation", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
				}
				if (esk_IK->number_relations % 2 != 0)					//padding
				{
					binCt.write_Coloration_Tag("padding", "float", "", offset, sizeof(float), "ESK_IK_Relation", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
				}
			}else if (esk_IK_Group->typeGroup == 0){
				assert(false);				//todo
			}else {
				assert(false);				//todo
			}

			offset = offset_ESK_IK_Group + esk_IK_Group->sizeGroup;
		}
	}
	incSection = incSection_IkRelationB;

	incSection++;
	incParam = 0;
	if (esk_skeleton_Section->offset_boneExtraInfo)
	{
		ESK_Bone_extraInfo* esk_Bone_extraInfo = (ESK_Bone_extraInfo*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_ESK_Skeleton_Section + esk_skeleton_Section->offset_boneExtraInfo));
		for (size_t i = 0; i < esk_skeleton_Section->number_bones; i++)
		{
			size_t offset_ESK_Bone_extraInfo = offset = offset_ESK_Skeleton_Section + esk_skeleton_Section->offset_boneExtraInfo + i * sizeof(ESK_Bone_extraInfo);
			incParam = 0;

			binCt.write_Coloration_Tag("unknow_0", "uint16_t", "", offset, sizeof(uint16_t), "ESK_Bone_extraInfo", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("unknow_1", "uint16_t", "", offset, sizeof(uint16_t), "ESK_Bone_extraInfo", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("unknow_2", "uint16_t", "", offset, sizeof(uint16_t), "ESK_Bone_extraInfo", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("unknow_3", "uint16_t", "", offset, sizeof(uint16_t), "ESK_Bone_extraInfo", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		}
	}

	binCt.write_Coloration_Gaps(0x0, size, sizeof(uint8_t), idTag, "Empty", parent, listBytesAllreadyTagged);		//test Empty/gaps/holes.
}





}