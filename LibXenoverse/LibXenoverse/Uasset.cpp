
#include "Uasset.h"

#include <sstream>
#include <iomanip>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>



namespace LibXenoverse
{


/*-------------------------------------------------------------------------------\
|                             Uasset							                 |
\-------------------------------------------------------------------------------*/
Uasset::~Uasset(void)
{
	/*
	if (skeleton != NULL)
		delete skeleton;
	skeleton = NULL;
	*/
}
/*-------------------------------------------------------------------------------\
|                             load								                 |
\-------------------------------------------------------------------------------*/
bool Uasset::load(string filename)
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
void Uasset::save(string filename, bool big_endian)
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
void Uasset::read(File *file)
{
	/*
	file->goToAddress(6);

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

	unsigned int skeleton_offset = 0;
	unsigned int offset_animations = 0;
	unsigned int animation_names_offset = 0;
	unsigned short animation_count = 0;

	file->readUChar(&type);
	file->readUChar(&unknow_1);
	file->readInt16E(&animation_count);
	file->readInt32E(&skeleton_offset);
	file->readInt32E(&offset_animations);
	file->readInt32E(&animation_names_offset);

	LOG_DEBUG("--------------- read EAN \n[8] unkTotal : %i, animation_count : %i, SkeletonOffset : [%i], offset_animations : [%i], animation_names_offset : [%i]\n", unknow_0, animation_count, skeleton_offset, offset_animations, animation_names_offset);

	// Read Skeleton
	LOG_DEBUG("----------- Skeleton\n");
	file->goToAddress(skeleton_offset);
	skeleton = new ESK();
	skeleton->read(file);
	skeleton->version = version;

	// Read Animations
	LOG_DEBUG("----------- Animations KeyFrames\n");
	animations.resize(animation_count);
	unsigned int address = 0;
	for (size_t i = 0; i < animation_count; i++)
	{
		// Read Keyframes
		file->goToAddress(offset_animations + i * 4);
		file->readInt32E(&address);
		file->goToAddress(address);

		LOG_DEBUG("------ animation %i : [%i] => [%i]\n", i, offset_animations + i * 4, address);

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
	*/
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void Uasset::write(File *file)
{
	/*
	file->goToAddress(6);

	// Header
	uint16_t header_size = 0x20;
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
	
	unsigned int ean_header_size = 32;

	unsigned short animation_count = animations.size();

	//differents parts of the file
	unsigned int skeleton_offset = ean_header_size;
	unsigned int offset_animations = 0;
	unsigned int animation_names_offset = 0;	//define after.

	file->writeUChar(&type);
	file->writeUChar(&unknow_1);
	file->writeInt16E(&animation_count);
	file->writeInt32E(&skeleton_offset);
	file->writeInt32E(&offset_animations);
	file->writeInt32E(&animation_names_offset);
		
	LOG_DEBUG("--------------- write EAN \n[8] unkTotal : %i, animation_count : %i, SkeletonOffset : [%i], offset_animations : [%i], animation_names_offset : [%i]\n", unknow_0, animation_count, skeleton_offset, offset_animations, animation_names_offset);

	// Write Skeleton
	LOG_DEBUG("----------- Skeleton\n");
	file->goToAddress(skeleton_offset);
	skeleton->write(file, false);

		
	// Write Animations
	offset_animations = file->getCurrentAddress();
	LOG_DEBUG("----------- Animations KeyFrames - offset_animations : [%i] \n", offset_animations);
	size_t keyframe_size = 0;
	size_t current_keyframe_size = 0;
	size_t sizeToFill = 0;
	unsigned int address_start_keyframeDef = (size_t)(ceil((offset_animations + animation_count * 4) / 16.0f) * 16);			//the adresss is always a start of a 16octets lines.
	unsigned int address = 0;

	for (size_t i = 0; i < animation_count; i++)
	{
		// Write Keay frames
		file->goToAddress(offset_animations + i * 4);
		address = address_start_keyframeDef + keyframe_size;
		file->writeInt32E(&address);
		file->goToAddress(address);

		LOG_DEBUG("------ animation %i : [%i] => [%i]\n", i, offset_animations + i * 4, address);
			
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
	file->writeInt32E(&offset_animations);
	file->writeInt32E(&animation_names_offset);


	file->goToEnd();

	LOG_DEBUG("--------------- End writting EAN \n");
	*/
}












/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// wxHexEditor coloration ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////





/*-------------------------------------------------------------------------------\
|                             save_Coloration		                             |
\-------------------------------------------------------------------------------*/
void Uasset::save_Coloration(string filename, bool show_error)
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
void Uasset::write_Coloration(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	/*
	EAN_Header* hdr = (EAN_Header*)buf;
	if (size < sizeof(EAN_Header) || memcmp(hdr->signature, LIBXENOVERSE_EAN_SIGNATURE, 4) != 0)
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

	binCt.write_Coloration_Tag("signature", "string", "", offset, 4 * sizeof(char), "EMM_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(char);
	binCt.write_Coloration_Tag("endian", "uint16_t", "", offset, sizeof(uint16_t), "EMM_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("header_size", "uint16_t", "", offset, sizeof(uint16_t), "EMM_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("version", "4 * uint8_t", "", offset, 4 * sizeof(uint8_t), "EMM_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(uint8_t);
	binCt.write_Coloration_Tag("unknow_0", "uint32_t", "", offset, sizeof(uint32_t), "EMM_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("type", "uint8_t", "", offset, sizeof(uint8_t), "EMM_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
	binCt.write_Coloration_Tag("unknow_1", "uint8_t", "", offset, sizeof(uint8_t), "EMM_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
	binCt.write_Coloration_Tag("number_animations", "uint16_t", "", offset, sizeof(uint16_t), "EMM_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("offset_skeleton", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + hdr->offset_skeleton, true) + " EMM_Section", offset, sizeof(uint32_t), "EMM_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("offset_animations", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + hdr->offset_animations, true) + " EMM_Section", offset, sizeof(uint32_t), "EMM_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("offset_animNames", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + hdr->offset_animNames, true) + " EMM_Section", offset, sizeof(uint32_t), "EMM_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

	incSection++;
	incParam = 0;


	//-------------------------------------------- ESK part -----------------------------------------------------

	if (hdr->offset_skeleton)
	{
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
		/*
		for (size_t i = 0; i < esk_skeleton_Section->number_bones; i++)
		{
			size_t offset_ESK_Bone_Relative_Transform = offset = offset_ESK_Skeleton_Section + esk_skeleton_Section->offset_relTransforms + i * sizeof(ESK_Bone_Relative_Transform);
			incParam = 0;

			binCt.write_Coloration_Tag("position", "4 * float", "", offset, 4 * sizeof(float), "ESK_Bone_Relative_Transform", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);
			binCt.write_Coloration_Tag("orientation", "4 * float", "quaternion", offset, 4 * sizeof(float), "ESK_Bone_Relative_Transform", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);
			binCt.write_Coloration_Tag("scale", "4 * float", "", offset, 4 * sizeof(float), "ESK_Bone_Relative_Transform", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);
		}
		*//*
		binCt.write_Coloration_Tag("pos_orient_scale", ToString(esk_skeleton_Section->number_bones) + " * 3 * 4 * float", "", offset, esk_skeleton_Section->number_bones * 3 * 4 * sizeof(float), "ESK_Bone_Relative_Transform", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += esk_skeleton_Section->number_bones * 3 * 4 * sizeof(float);




		incSection++;
		incParam = 0;
		if (esk_skeleton_Section->offset_absMatrices)
		{
			ESK_Bone_Absolute_Matrix* esk_Bone_Absolute_Matrix = (ESK_Bone_Absolute_Matrix*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_ESK_Skeleton_Section + esk_skeleton_Section->offset_absMatrices));
			/*
			for (size_t i = 0; i < esk_skeleton_Section->number_bones; i++)
			{
				size_t offset_ESK_Bone_Absolute_Matrix = offset = offset_ESK_Skeleton_Section + esk_skeleton_Section->offset_absMatrices + i * sizeof(ESK_Bone_Absolute_Matrix);
				incParam = 0;

				binCt.write_Coloration_Tag("m0", "4 * float", "", offset, 4 * sizeof(float), "ESK_Bone_Absolute_Matrix", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);
				binCt.write_Coloration_Tag("m1", "4 * float", "", offset, 4 * sizeof(float), "ESK_Bone_Absolute_Matrix", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);
				binCt.write_Coloration_Tag("m2", "4 * float", "", offset, 4 * sizeof(float), "ESK_Bone_Absolute_Matrix", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);
				binCt.write_Coloration_Tag("m3", "4 * float", "", offset, 4 * sizeof(float), "ESK_Bone_Absolute_Matrix", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);
			}
			*//*
			binCt.write_Coloration_Tag("m0_1_2_3", ToString(esk_skeleton_Section->number_bones) + " * 4 * 4 * float", "", offset, esk_skeleton_Section->number_bones * 4 * 4 * sizeof(float), "ESK_Bone_Absolute_Matrix", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += esk_skeleton_Section->number_bones * 4 * 4 * sizeof(float);
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
				binCt.write_Coloration_Tag("number_Iks", "uint16_t", "", offset, sizeof(uint16_t), "ESK_IK_Group", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
				binCt.write_Coloration_Tag("sizeGroup", "uint16_t", "number of bytes for the groups", offset, sizeof(uint16_t), "ESK_IK_Group", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);

				for (size_t j = 0; j < esk_IK_Group->number_Iks; j++)
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
			/*
			for (size_t i = 0; i < esk_skeleton_Section->number_bones; i++)
			{
				size_t offset_ESK_Bone_extraInfo = offset = offset_ESK_Skeleton_Section + esk_skeleton_Section->offset_boneExtraInfo + i * sizeof(ESK_Bone_extraInfo);
				incParam = 0;

				binCt.write_Coloration_Tag("unknow_0", "uint16_t", "", offset, sizeof(uint16_t), "ESK_Bone_extraInfo", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
				binCt.write_Coloration_Tag("unknow_1", "uint16_t", "", offset, sizeof(uint16_t), "ESK_Bone_extraInfo", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
				binCt.write_Coloration_Tag("unknow_2", "uint16_t", "", offset, sizeof(uint16_t), "ESK_Bone_extraInfo", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
				binCt.write_Coloration_Tag("unknow_3", "uint16_t", "", offset, sizeof(uint16_t), "ESK_Bone_extraInfo", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			}
			*//*
			binCt.write_Coloration_Tag("unknow_0_1_2_3", ToString(esk_skeleton_Section->number_bones) + " * 4 * uint16_t", "", offset, esk_skeleton_Section->number_bones * 4 * sizeof(uint16_t), "ESK_Bone_extraInfo", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += esk_skeleton_Section->number_bones * 4 * sizeof(uint16_t);
		}

		/*
		size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
		if (size_padding)
			binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "ESK", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged);
		offset += size_padding;
		*//*
	}

	//-----------------------------------------------------------------------------------------------------------




	incSection++;
	incParam = 0;
	size_t incSection_adress = incSection;
	size_t incSection_Anim = incSection + 1;
	size_t incSection_AnimNode = incSection + 2;
	size_t incSection_AnimKf = incSection + 3;
	size_t incSection_Kf_frametime = incSection + 4;
	size_t incSection_Kf_values = incSection + 5;

	std::vector<uint32_t> allready_Kf_offset;

	if (hdr->offset_animations)
	{
		uint32_t* offset_animations = (uint32_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, startAdress + hdr->offset_animations));

		offset = startAdress + hdr->offset_animations + hdr->number_animations * sizeof(uint32_t);
		size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
		if (size_padding)
			binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EAN_Anim_Section", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged);
		offset += size_padding;

		for (size_t i = 0; i < hdr->number_animations; i++)
		{
			size_t offset_EAN_Anim_Section = offset = startAdress + offset_animations[i];
			incSection = incSection_adress;
			binCt.write_Coloration_Tag("offset_animation", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EAN_Anim_Section, true) + " Ref to list adress to EAN_Anim_Section", startAdress + hdr->offset_animations + i * sizeof(uint32_t), sizeof(uint32_t), "EAN_Anim_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);



			EAN_Anim_Section* ean_Anim_Section = (EAN_Anim_Section*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
			incSection = incSection_Anim;
			incParam = 0;

			binCt.write_Coloration_Tag("unknow_0", "uint16_t", "", offset, sizeof(uint16_t), "EAN_Anim_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("indexSize", "uint8_t", "to have the number of bytes for index", offset, sizeof(uint8_t), "EAN_Anim_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
			binCt.write_Coloration_Tag("floatSize", "uint8_t", "to have the number of bytes for float", offset, sizeof(uint8_t), "EAN_Anim_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
			binCt.write_Coloration_Tag("number_frames", "uint32_t", "", offset, sizeof(uint32_t), "EAN_Anim_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			binCt.write_Coloration_Tag("number_animNodes", "uint32_t", "", offset, sizeof(uint32_t), "EAN_Anim_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			binCt.write_Coloration_Tag("offset_animNodes", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EAN_Anim_Section + ean_Anim_Section->offset_animNodes, true) + " EAN_AnimNode_Section", offset, sizeof(uint32_t), "EAN_Anim_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);


			if (ean_Anim_Section->offset_animNodes)
			{
				uint32_t* offset_animNodes = (uint32_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_EAN_Anim_Section + ean_Anim_Section->offset_animNodes));

				/*
				offset = offset_EAN_Anim_Section + ean_Anim_Section->offset_animNodes + ean_Anim_Section->number_animNodes * sizeof(uint32_t);
				size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
				if (size_padding)
					binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EAN_AnimNode_Section", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged);
				offset += size_padding;
				*//*

				for (size_t j = 0; j < ean_Anim_Section->number_animNodes; j++)
				{
					size_t offset_EAN_AnimNode_Section = offset = offset_EAN_Anim_Section + offset_animNodes[j];
					incSection = incSection_adress;
					binCt.write_Coloration_Tag("offset_animNode", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EAN_AnimNode_Section, true) + " Ref to list adress to EAN_AnimNode_Section", offset_EAN_Anim_Section + ean_Anim_Section->offset_animNodes + j * sizeof(uint32_t), sizeof(uint32_t), "EAN_AnimNode_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);


					EAN_AnimNode_Section* ean_AnimNode_Section = (EAN_AnimNode_Section*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
					incSection = incSection_AnimNode;
					incParam = 0;

					binCt.write_Coloration_Tag("bone_index", "uint16_t", "", offset, sizeof(uint16_t), "EAN_AnimNode_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
					binCt.write_Coloration_Tag("number_animKfs", "uint16_t", "", offset, sizeof(uint16_t), "EAN_AnimNode_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
					binCt.write_Coloration_Tag("offset_animKfs", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EAN_AnimNode_Section + ean_AnimNode_Section->offset_animKfs, true) + " EAN_AnimKf_Section", offset, sizeof(uint32_t), "EAN_AnimNode_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);




					if (ean_AnimNode_Section->offset_animKfs)
					{
						uint32_t* offset_animKfs = (uint32_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_EAN_AnimNode_Section + ean_AnimNode_Section->offset_animKfs));
						incSection = incSection_adress;
						binCt.write_Coloration_Tag("list offset_animKf", ToString(ean_AnimNode_Section->number_animKfs) + " * uint32_t", " first => " + EMO_BaseFile::UnsignedToString(offset_EAN_AnimNode_Section + offset_animKfs[0], true) + " Ref to list adress to EAN_AnimKf_Section", offset_EAN_AnimNode_Section + ean_AnimNode_Section->offset_animKfs, ean_AnimNode_Section->number_animKfs * sizeof(uint32_t), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);

						/*
						offset = offset_EAN_AnimNode_Section + ean_AnimNode_Section->offset_animKfs + ean_AnimNode_Section->number_animKfs * sizeof(uint32_t);
						size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
						if (size_padding)
							binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EAN_AnimKf_Section", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged);
						offset += size_padding;
						*//*

						for (size_t k = 0; k < ean_AnimNode_Section->number_animKfs; k++)
						{
							size_t offset_EAN_AnimKf_Section = offset = offset_EAN_AnimNode_Section + offset_animKfs[k];
							//incSection = incSection_adress;
							//binCt.write_Coloration_Tag("offset_animKf", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EAN_AnimKf_Section, true) + " Ref to list adress to EAN_AnimKf_Section", offset_EAN_AnimNode_Section + ean_AnimNode_Section->offset_animKfs + k * sizeof(uint32_t), sizeof(uint32_t), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);


							EAN_AnimKf_Section* ean_AnimKf_Section = (EAN_AnimKf_Section*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
							incSection = incSection_AnimKf;
							incParam = 0;

							binCt.write_Coloration_Tag("type_flag", "uint8_t", "", offset, sizeof(uint8_t), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
							binCt.write_Coloration_Tag("unknow_0", "uint8_t", "", offset, sizeof(uint8_t), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
							binCt.write_Coloration_Tag("unknow_1", "uint16_t", "", offset, sizeof(uint16_t), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
							binCt.write_Coloration_Tag("number_Kfs", "uint32_t", "", offset, sizeof(uint32_t), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
							binCt.write_Coloration_Tag("offset_Kf_frametime", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EAN_AnimKf_Section + ean_AnimKf_Section->offset_Kf_frametime, true) + " frametime", offset, sizeof(uint32_t), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
							binCt.write_Coloration_Tag("offset_Kf_values", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EAN_AnimKf_Section + ean_AnimKf_Section->offset_Kf_values, true) + " Vector4", offset, sizeof(uint32_t), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);



							if (ean_AnimKf_Section->offset_Kf_frametime)
							{
								size_t offset_EAN_AnimKf_Section_frametime = offset = offset_EAN_AnimKf_Section + ean_AnimKf_Section->offset_Kf_frametime;
								incSection = incSection_Kf_frametime;
								incParam = 0;

								bool allreadyTagged = false;
								for (size_t m = 0, nb4 = allready_Kf_offset.size(); m < nb4; m++)
								{
									if (allready_Kf_offset.at(m) == offset_EAN_AnimKf_Section_frametime)
									{
										allreadyTagged = true;
										break;
									}
								}


								if (!allreadyTagged)
								{
									allready_Kf_offset.push_back(offset_EAN_AnimKf_Section_frametime);

									if (ean_Anim_Section->indexSize == 0)
									{
										uint8_t* animKf_frametimes = (uint8_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_EAN_AnimKf_Section_frametime));
										/*
										for (size_t m = 0; m < ean_AnimKf_Section->number_Kfs; m++)
										{
											binCt.write_Coloration_Tag("frametime", "uint8_t", "", offset, sizeof(uint8_t), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
										}
										*//*
										binCt.write_Coloration_Tag("frametime", ToString(ean_AnimKf_Section->number_Kfs) + " * uint8_t", "", offset, ean_AnimKf_Section->number_Kfs * sizeof(uint8_t), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += ean_AnimKf_Section->number_Kfs * sizeof(uint8_t);

									}
									else {

										uint16_t* animKf_frametimes = (uint16_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_EAN_AnimKf_Section_frametime));
										/*
										for (size_t m = 0; m < ean_AnimKf_Section->number_Kfs; m++)
										{
											binCt.write_Coloration_Tag("frametime", "uint16_t", "", offset, sizeof(uint16_t), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
										}
										*//*
										binCt.write_Coloration_Tag("frametime", ToString(ean_AnimKf_Section->number_Kfs) + " * uint16_t", "", offset, ean_AnimKf_Section->number_Kfs * sizeof(uint16_t), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += ean_AnimKf_Section->number_Kfs * sizeof(uint16_t);
									}
								}

								size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
								if (size_padding)
									binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EAN_AnimNode_Section", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged);
								offset += size_padding;
							}



							if (ean_AnimKf_Section->offset_Kf_values)
							{
								size_t offset_EAN_AnimKf_Section_values = offset = offset_EAN_AnimKf_Section + ean_AnimKf_Section->offset_Kf_values;
								incSection = incSection_Kf_values;
								incParam = 0;

								bool allreadyTagged = false;
								for (size_t m = 0, nb4 = allready_Kf_offset.size(); m < nb4; m++)
								{
									if (allready_Kf_offset.at(m) == offset_EAN_AnimKf_Section_values)
									{
										allreadyTagged = true;
										break;
									}
								}


								if (!allreadyTagged)
								{
									allready_Kf_offset.push_back(offset_EAN_AnimKf_Section_values);

									size_t nbBytesValues = 0;

									if (ean_Anim_Section->floatSize != 1)			//Todo search what is 0x2 for (apparently it's flag, the first left bit is for floatSize, but the second left bit is for something else)
									{
										float* animKf_value = (float*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_EAN_AnimKf_Section_values));
										/*
										for (size_t m = 0; m < ean_AnimKf_Section->number_Kfs; m++)
										{
											binCt.write_Coloration_Tag("values_x", "float", "", offset, sizeof(float), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
											binCt.write_Coloration_Tag("values_y", "float", "", offset, sizeof(float), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
											binCt.write_Coloration_Tag("values_z", "float", "", offset, sizeof(float), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
											binCt.write_Coloration_Tag("values_w", "float", "", offset, sizeof(float), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
										}
										*//*
										nbBytesValues = ean_AnimKf_Section->number_Kfs * 4 * sizeof(float);
										binCt.write_Coloration_Tag("values_xyzw", ToString(ean_AnimKf_Section->number_Kfs) + " * 4 * float", "", offset, ean_AnimKf_Section->number_Kfs * 4 * sizeof(float), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += ean_AnimKf_Section->number_Kfs * 4 * sizeof(float);

									}
									else {
										//real is float 16
										uint16_t* animKf_value = (uint16_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_EAN_AnimKf_Section_values));
										/*
										for (size_t m = 0; m < ean_AnimKf_Section->number_Kfs; m++)
										{
											binCt.write_Coloration_Tag("values_x", "float16", "", offset, sizeof(uint16_t), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
											binCt.write_Coloration_Tag("values_y", "float16", "", offset, sizeof(uint16_t), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
											binCt.write_Coloration_Tag("values_z", "float16", "", offset, sizeof(uint16_t), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
											binCt.write_Coloration_Tag("values_w", "float16", "", offset, sizeof(uint16_t), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
										}
										*//*
										nbBytesValues = ean_AnimKf_Section->number_Kfs * 4 * sizeof(uint16_t);
										binCt.write_Coloration_Tag("values_xyzw", ToString(ean_AnimKf_Section->number_Kfs) + " * 4 * float16", "", offset, ean_AnimKf_Section->number_Kfs * 4 * sizeof(uint16_t), "EAN_AnimKf_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += ean_AnimKf_Section->number_Kfs * 4 * sizeof(uint16_t);
									}
								}
							}
						}
					}
				}
			}



			incSection = incSection_Anim;
			/*
			size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
			if (size_padding == 0)
			{
				binCt.write_Coloration_Tag("unknow_1_2_3", "3 * uint32_t", " strange padding", offset, 3 * sizeof(uint32_t), "EAN_Anim_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 3 * sizeof(uint32_t);
				if (i + 1 < hdr->number_animations)
				{
					size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
					if (size_padding)
						binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EAN_Anim_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
					offset += size_padding;
				}
			}
			*//*
			/*
			if (i + 1 < hdr->number_animations)
			{
				size_padding = (size_t)(ceil((double)(offset - offset_EAN_Anim_Section) / 32.0) * 32.0) - (offset - offset_EAN_Anim_Section);
				binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EAN_Anim_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
				offset += size_padding;

			}else {
				size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
				if (size_padding == 0)
				{
					binCt.write_Coloration_Tag("unknow_1_2_3", "3 * uint32_t", " strange padding", offset, 3 * sizeof(uint32_t), "EAN_Anim_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 3 * sizeof(uint32_t);
				}
			}
			*//*
		}
	}
	incSection = incSection_Kf_values;





	////////////////////////////////////
	incSection++;
	incParam = 0;
	offset = startAdress + hdr->offset_animNames;
	size_t endOffset = offset;
	for (size_t i = 0; i < hdr->number_animations; i++)
	{
		name_offset = (uint32_t*)EMO_BaseFile::GetOffsetPtr_native(buf, offset);

		binCt.write_Coloration_Tag("offset_animation_name", "uint32_t", "", offset, sizeof(uint32_t), "EAN_AnimNames", parent, idTag++, incSection, i, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		if (name_offset)
		{
			name_tmp = string((char*)EMO_BaseFile::GetOffsetPtr_native(buf, startAdress + (*name_offset)));
			binCt.write_Coloration_Tag("animation_name", "string", "", startAdress + (*name_offset), (name_tmp.size() + 1) * sizeof(char), "EAN_AnimNames", parent, idTag++, incSection + 1, i, listBytesAllreadyTagged, (size_t)-1, false);

			size_t endoffset_tmp = startAdress + (*name_offset) + (name_tmp.size() + 1) * sizeof(char);
			if (endOffset < endoffset_tmp)
				endOffset = endoffset_tmp;
		}
	}
	offset = endOffset;
	/*
	size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
	if (size_padding)
		binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EAN_AnimNames", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged);
	offset += size_padding;
	*//*
	incSection++;




	binCt.write_Coloration_Gaps(0x0, size, sizeof(uint8_t), idTag, "Empty", parent, listBytesAllreadyTagged);		//test Empty/gaps/holes.
	*/
}








}