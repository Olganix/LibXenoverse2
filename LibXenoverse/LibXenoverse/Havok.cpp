#include "Havok.h"

#include <sstream>
#include <iomanip>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

namespace LibXenoverse
{

/*-------------------------------------------------------------------------------\
|                             Havok					                             |
\-------------------------------------------------------------------------------*/
Havok::Havok()
{
	Reset();
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void Havok::Reset()
{
	version = "2015.01.00";

	size_t nbElement = listType.size();
	for (size_t i = 0; i < nbElement; i++)
		delete listType.at(i);
	listType.clear();

	nbElement = listItem.size();
	for (size_t i = 0; i < nbElement; i++)
		delete listItem.at(i);
	listItem.clear();

	if (rootObject)
		delete rootObject;
	rootObject = 0;
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void Havok_TagType::Reset()
{ 
	size_t nbTemp = listTemplate.size(); 
	for (size_t i = 0; i < nbTemp; i++) 
		delete listTemplate.at(i); 
	listTemplate.clear();

	nbTemp = members.size(); 
	for (size_t i = 0; i < nbTemp; i++) 
		delete members.at(i); 
	members.clear();

	interfaces.clear();
	parent = 0;
	pointer = 0;
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void Havok_TagObject::Reset()
{
	size_t nbElement = listObjectClass.size();
	for (size_t i = 0; i < nbElement; i++)
		delete listObjectClass.at(i);

	nbElement = listObjectArray.size();
	for (size_t i = 0; i < nbElement; i++)
		delete listObjectArray.at(i);

	nbElement = listObjectTuple.size();
	for (size_t i = 0; i < nbElement; i++)
		delete listObjectTuple.at(i);

	if (objectPointer)
		delete objectPointer;
	objectPointer = 0;
}
/*-------------------------------------------------------------------------------\
|                             clone					                             |
\-------------------------------------------------------------------------------*/
Havok_TagObject* Havok_TagObject::clone()
{
	Havok_TagObject* obj = new Havok_TagObject(type);

	obj->b_value = this->b_value;
	obj->s_value = this->s_value;
	obj->i_value = this->i_value;
	obj->f_value = this->f_value;
	obj->attachement = this->attachement;

	if(this->objectPointer)
		obj->objectPointer = this->objectPointer->clone();

	size_t nbElements = this->listObjectString.size();
	for (size_t i = 0; i < nbElements; i++)
		obj->listObjectString.push_back(this->listObjectString.at(i)->clone());

	nbElements = this->listObjectClass.size();
	for (size_t i = 0; i < nbElements; i++)
		obj->listObjectClass.push_back(this->listObjectClass.at(i)->clone());

	nbElements = this->listObjectArray.size();
	for (size_t i = 0; i < nbElements; i++)
		obj->listObjectArray.push_back(this->listObjectArray.at(i)->clone());

	nbElements = this->listObjectTuple.size();
	for (size_t i = 0; i < nbElements; i++)
		obj->listObjectTuple.push_back(this->listObjectTuple.at(i)->clone());

	return obj;
}
/*-------------------------------------------------------------------------------\
|                             remove					                         |
\-------------------------------------------------------------------------------*/
void Havok_TagItem::remove(Havok_TagObject* obj)
{
	size_t nbElements = value.size();
	for (size_t i=0;i<nbElements;i++)
	{
		if (value.at(i) == obj)
		{
			value.erase(value.begin() + i);
			break;
		}
	}
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*-------------------------------------------------------------------------------\
|                             superType				                             |
\-------------------------------------------------------------------------------*/
Havok_TagType* Havok_TagType::superType() 
{ 
	return ((flags & Havok::TagFlag::TF_SubType) ? this : parent->superType());
}
/*-------------------------------------------------------------------------------\
|                             subType				                             |
\-------------------------------------------------------------------------------*/
size_t Havok_TagType::subType()
{
	return subTypeFlags & Havok::TagSubType::TST_TypeMask;
}
/*-------------------------------------------------------------------------------\
|                             allMembers			                             |
\-------------------------------------------------------------------------------*/
std::vector<Havok_TagMember*> Havok_TagType::allMembers()
{
	std::vector<Havok_TagMember*> ret;
	if (parent)
		ret = parent->allMembers();
	size_t nbMembers = members.size();
	for (size_t i = 0; i < nbMembers; i++)
		ret.push_back(members.at(i));
	return ret;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool Havok::Load(const uint8_t *buf, size_t size)
{
	Reset();

	this->big_endian = true;

	size_t offset = 0;

	Havok_PartHeader* hdr = (Havok_PartHeader*)buf;
	size_t hdr_size = hdr->size;  hdr_size = val32(hdr_size) & 0x3FFFFFFF;
	if (size < sizeof(Havok_PartHeader) || (memcmp(hdr->signature, Havok_TAG0_SIGNATURE, 4) != 0) || (size < hdr_size))
	{
		printf("havok's version request : %s\n", Havok_TAG0_SIGNATURE);
		return false;
	}
	offset += sizeof(Havok_PartHeader);


	Havok_PartHeader* version_hdr = (Havok_PartHeader*)GetOffsetPtr(buf, offset, true);
	size_t version_hdr_size = version_hdr->size;  version_hdr_size = val32(version_hdr_size) & 0x3FFFFFFF;
	if (memcmp(version_hdr->signature, Havok_SDKV_SIGNATURE, 4) != 0)
	{
		printf("havok's version request : %s\n", Havok_SDKV_SIGNATURE);
		return false;
	}
	offset += sizeof(Havok_PartHeader);


	Havok_Version* version_info = (Havok_Version*)GetOffsetPtr(buf, offset, true);
	if ((memcmp(version_info->year, Havok_v2015_SIGNATURE, 4) != 0) || (memcmp(version_info->major, Havok_major01_SIGNATURE, 2) != 0) || (memcmp(version_info->minor, Havok_minor00_SIGNATURE, 2) != 0))
	{
		printf("havok's version request : %s.%s.%s\n", Havok_v2015_SIGNATURE, Havok_major01_SIGNATURE, Havok_minor00_SIGNATURE);
		return false;
	}
	offset += sizeof(Havok_Version);



	
	size_t offsetData = (size_t)-1;
	size_t sizeOfData = 0;
	std::vector<string> listTSTR;
	std::vector<string> listFSTR;


	while (offset < hdr_size)
	{
		Havok_PartHeader* part_hdr = (Havok_PartHeader*)GetOffsetPtr(buf, offset, true);
		size_t part_id = (val32(part_hdr->size) & 0xC0000000) >> 30;
		size_t part_hdr_size = val32(part_hdr->size) & 0x3FFFFFFF;
		offset += sizeof(Havok_PartHeader);


		if (memcmp(part_hdr->signature, Havok_DATA_SIGNATURE, 4) == 0)
		{
			offsetData = offset;				// will be colored after getting the item list.
			sizeOfData = part_hdr_size - sizeof(Havok_PartHeader);
			offset += part_hdr_size - sizeof(Havok_PartHeader);

		}
		else if (memcmp(part_hdr->signature, Havok_TYPE_SIGNATURE, 4) == 0) {

			size_t startoffset_Type = offset - sizeof(Havok_PartHeader);

			while (offset < startoffset_Type + part_hdr_size)
			{
				Havok_PartHeader* type_subpart_hdr = (Havok_PartHeader*)GetOffsetPtr(buf, offset, true);
				size_t type_subpart_id = (val32(type_subpart_hdr->size) & 0xC0000000) >> 30;
				size_t type_subpart_hdr_size = val32(type_subpart_hdr->size) & 0x3FFFFFFF;
				offset += sizeof(Havok_PartHeader);



				if (memcmp(type_subpart_hdr->signature, Havok_TSTR_SIGNATURE, 4) == 0)
				{
					size_t offset_tmp = 0;
					size_t inc = 0;
					while (offset_tmp < type_subpart_hdr_size - sizeof(Havok_PartHeader))
					{
						char* char_ptr = (char*)GetOffsetPtr(buf, offset + offset_tmp, true);
						string str = string(char_ptr);
						listTSTR.push_back(string(str));
						offset_tmp += str.length() + 1;
					}
					offset += type_subpart_hdr_size - sizeof(Havok_PartHeader);



				}else if (memcmp(type_subpart_hdr->signature, Havok_TNAM_SIGNATURE, 4) == 0) {

					
					size_t nbBytes = 0;
					size_t nbTypes = readPacked(buf + offset, size - offset, nbBytes);

					for (size_t i = 0; i < nbTypes; i++)
						listType.push_back(new Havok_TagType(i, "None"));

					size_t offset_tmp = offset + nbBytes;
					for (size_t i = 1; i < nbTypes; i++)
					{
						Havok_TagType* tagType = listType.at(i);

						tagType->name = listTSTR.at(readPacked(buf + offset_tmp, size - offset_tmp, nbBytes));
						offset_tmp += nbBytes;

						size_t nbNextValues = readPacked(buf + offset_tmp, size - offset_tmp, nbBytes);
						offset_tmp += nbBytes;

						for (size_t j = 0; j < nbNextValues; j++)
						{
							size_t index = readPacked(buf + offset_tmp, size - offset_tmp, nbBytes);
							offset_tmp += nbBytes;
							string str = listTSTR.at(index);


							Havok_TagTemplate* template_tmp = new Havok_TagTemplate(str, readPacked(buf + offset_tmp, size - offset_tmp, nbBytes));
							offset_tmp += nbBytes;

							if (template_tmp->isType())
								template_tmp->value_ptr = listType.at(template_tmp->value);

							tagType->listTemplate.push_back(template_tmp);
						}
					}

					offset += type_subpart_hdr_size - sizeof(Havok_PartHeader);


				}else if (memcmp(type_subpart_hdr->signature, Havok_FSTR_SIGNATURE, 4) == 0) {

					size_t offset_tmp = 0;
					size_t inc = 0;
					while (offset_tmp < type_subpart_hdr_size - sizeof(Havok_PartHeader))
					{
						char* char_ptr = (char*)GetOffsetPtr(buf, offset + offset_tmp, true);
						string str = string(char_ptr);
						listFSTR.push_back(string(str));
						offset_tmp += str.length() + 1;
					}

					offset += type_subpart_hdr_size - sizeof(Havok_PartHeader);



				}
				else if (memcmp(type_subpart_hdr->signature, Havok_TBOD_SIGNATURE, 4) == 0) {

					size_t offset_tmp = 0;
					size_t nbBytes = 0;
					while (offset_tmp<type_subpart_hdr_size - sizeof(Havok_PartHeader))
					{
						size_t typeIndex = readPacked(buf + offset + offset_tmp, size - offset_tmp, nbBytes);
						offset_tmp += nbBytes;

						if (typeIndex == 0)						//sequence of 0 padding
							continue;

						Havok_TagType* tagType = listType.at(typeIndex);

						size_t typeIndexParent = readPacked(buf + offset + offset_tmp, size - offset_tmp, nbBytes);
						offset_tmp += nbBytes;
						tagType->parent = listType.at(typeIndexParent);

						size_t flags = readPacked(buf + offset + offset_tmp, size - offset_tmp, nbBytes);
						offset_tmp += nbBytes;
						tagType->flags = flags;




						if (tagType->flags & TagFlag::TF_SubType)
						{
							size_t value = readPacked(buf + offset + offset_tmp, size - offset_tmp, nbBytes);
							offset_tmp += nbBytes;

							tagType->subTypeFlags = value;
						}

						if ((tagType->flags & TagFlag::TF_Pointer) && ((tagType->subTypeFlags & 0xF) >= 6))
						{
							size_t value = readPacked(buf + offset + offset_tmp, size - offset_tmp, nbBytes);
							offset_tmp += nbBytes;

							tagType->pointer = listType.at(value);
						}

						if (tagType->flags & TagFlag::TF_Version)
						{
							size_t value = readPacked(buf + offset + offset_tmp, size - offset_tmp, nbBytes);
							offset_tmp += nbBytes;

							tagType->version = value;
						}

						if (tagType->flags & TagFlag::TF_ByteSize)
						{
							size_t value = readPacked(buf + offset + offset_tmp, size - offset_tmp, nbBytes);
							offset_tmp += nbBytes;

							tagType->byteSize = value;

							value = readPacked(buf + offset + offset_tmp, size - offset_tmp, nbBytes);
							offset_tmp += nbBytes;

							tagType->alignment = value;
						}

						if (tagType->flags & TagFlag::TF_AbstractValue)
						{
							size_t value = readPacked(buf + offset + offset_tmp, size - offset_tmp, nbBytes);
							offset_tmp += nbBytes;

							tagType->abstractValue = value;
						}

						if (tagType->flags & TagFlag::TF_Members)
						{
							size_t nbMembers = readPacked(buf + offset + offset_tmp, size - offset_tmp, nbBytes);
							offset_tmp += nbBytes;

							for (size_t i = 0; i < nbMembers; i++)
							{
								Havok_TagMember* tagMember = new Havok_TagMember(listFSTR.at(readPacked(buf + offset + offset_tmp, size - offset_tmp, nbBytes)));
								offset_tmp += nbBytes;

								tagMember->flags = readPacked(buf + offset + offset_tmp, size - offset_tmp, nbBytes);
								offset_tmp += nbBytes;

								tagMember->byteOffset = readPacked(buf + offset + offset_tmp, size - offset_tmp, nbBytes);
								offset_tmp += nbBytes;

								tagMember->type = listType.at(readPacked(buf + offset + offset_tmp, size - offset_tmp, nbBytes));
								offset_tmp += nbBytes;

								tagType->members.push_back(tagMember);
							}
						}


						if (tagType->flags & TagFlag::TF_Interfaces)
						{
							size_t nbTypeArray = readPacked(buf + offset + offset_tmp, size - offset_tmp, nbBytes);
							offset_tmp += nbBytes;

							for (size_t i = 0; i < nbTypeArray; i++)
							{
								size_t index_tmp = readPacked(buf + offset + offset_tmp, size - offset_tmp, nbBytes);
								offset_tmp += nbBytes;
								Havok_TagType* type_temp = listType.at(index_tmp);


								size_t value = readPacked(buf + offset + offset_tmp, size - offset_tmp, nbBytes);
								offset_tmp += nbBytes;

								tagType->interfaces.push_back(Havok_TagInterface(type_temp, value));
							}
						}


						if (tagType->flags & TagFlag::TF_Unknown)
						{
							printf("Error: in TBOD, Flag 0x80 exists, handle it!\n");
							LibXenoverse::notifyError();
						}
					}

					offset += type_subpart_hdr_size - sizeof(Havok_PartHeader);



				}else if (memcmp(type_subpart_hdr->signature, Havok_TPAD_SIGNATURE, 4) == 0) {

					offset += type_subpart_hdr_size - sizeof(Havok_PartHeader);


				}else {

					printf("Warning : Type_subPartHeader signature unknow : %s at %s\n", type_subpart_hdr->signature, UnsignedToString(offset, true).c_str());
					LibXenoverse::notifyWarning();
				}
			}


		}else if (memcmp(part_hdr->signature, Havok_INDX_SIGNATURE, 4) == 0) {

			size_t startIndexPart = offset - sizeof(Havok_PartHeader);

			while (offset< startIndexPart + part_hdr_size)
			{
				Havok_PartHeader* indexation_subpart_hdr = (Havok_PartHeader*)GetOffsetPtr(buf, offset, true);
				size_t index_subpart_id = (val32(indexation_subpart_hdr->size) & 0xC0000000) >> 30;
				size_t index_subpart_hdr_size = val32(indexation_subpart_hdr->size) & 0x3FFFFFFF;
				offset += sizeof(Havok_PartHeader);

				if (memcmp(indexation_subpart_hdr->signature, Havok_ITEM_SIGNATURE, 4) == 0)
				{
					size_t startoffset_itemList = offset - sizeof(Havok_PartHeader);
					size_t nbBytes = 0;

					while (offset < startoffset_itemList + index_subpart_hdr_size)
					{
						uint32_t* values = (uint32_t*)(buf + offset);
						offset += 3 * sizeof(uint32_t);

						Havok_TagItem* item = new Havok_TagItem();
						listItem.push_back(item);

						size_t flag = values[0];
						item->type = listType.at(flag & 0xFFFFFF);
						item->isPtr = ((flag & 0x10000000) != 0);

						item->offset = offsetData + values[1];
						item->count = values[2];
					}

				}else {
					printf("Warning : indexation PartHeader signature unknow : %s at %s\n", indexation_subpart_hdr->signature, UnsignedToString(offset, true).c_str());
					LibXenoverse::notifyWarning();
					offset += index_subpart_hdr_size - sizeof(Havok_PartHeader);
				}
			}




		}else {
			printf("Warning : PartHeader signature unknow : %s at %s\n", part_hdr->signature, UnsignedToString(offset, true).c_str());
			LibXenoverse::notifyWarning();
			offset += part_hdr_size - sizeof(Havok_PartHeader);
		}
	}



	if ((offsetData != (size_t)-1) && (listItem.size() != 0))			//now we have the data and the informations about it.
	{
		offset = offsetData;

		size_t nbElements = 0;
		size_t nbItem = listItem.size();
		for (size_t i = 0; i < nbItem; i++)
		{
			Havok_TagItem* item = listItem.at(i);

			if (((item->type) && (item->type->name == "None")) || (item->value.size() != 0))
				continue;



			Havok_TagType* type = item->type;

			for (size_t j = 0; j < item->count; j++)
			{
				offset = item->offset + j * type->superType()->byteSize;
				item->value.push_back(readObject(j, buf, size, type, offset, listItem, listType, item));

				if (!rootObject)
					rootObject = (Havok_TagObject*)item->value.back();
			}

			break;				//normally the first element is enought because of recursivity
		}
	}





	return (rootObject!=0);
}
















/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*-------------------------------------------------------------------------------\
|                             CreateFile			                             |
\-------------------------------------------------------------------------------*/
uint8_t* Havok::CreateFile(unsigned int *psize)
{
	this->big_endian = true;


	////////////////////////////// preparation for Type binary parts.
	std::vector<string> listTSTR;
	size_t nbTemplate;
	string str;

	size_t nbTypes = listType.size();
	for (size_t i = 1; i < nbTypes; i++)						//it's all names of type + template
	{
		str = listType.at(i)->name;

		bool isfound = false;
		size_t nbNames = listTSTR.size();
		for (size_t k = 0; k < nbNames; k++)
		{
			if (listTSTR.at(k) == str)
			{
				isfound = true;
				break;
			}
		}
		if (!isfound)
			listTSTR.push_back(str);


		std::vector<Havok_TagTemplate*> &listTemplate = listType.at(i)->listTemplate;
		nbTemplate = listTemplate.size();
		for (size_t j = 0; j < nbTemplate; j++)
		{
			str = listTemplate.at(j)->name;

			bool isfound = false;
			size_t nbNames = listTSTR.size();
			for (size_t k = 0; k < nbNames; k++)
			{
				if (listTSTR.at(k) == str)
				{
					isfound = true;
					break;
				}
			}
			if (!isfound)
				listTSTR.push_back(str);
		}
	}



	std::vector<string> listFSTR;
	size_t nbMembers;

	for (size_t i = 1; i < nbTypes; i++)						//it's all names of members
	{
		std::vector<Havok_TagMember*> &members = listType.at(i)->members;
		nbMembers = members.size();
		for (size_t j = 0; j < nbMembers; j++)
		{
			str = members.at(j)->name;

			bool isfound = false;
			size_t nbNames = listFSTR.size();
			for (size_t k = 0; k < nbNames; k++)
			{
				if (listFSTR.at(k) == str)
				{
					isfound = true;
					break;
				}
			}
			if (!isfound)
				listFSTR.push_back(str);
		}
	}

	//TNAM
	std::vector<byte> listBytesTNam;
	size_t nbNames = listTSTR.size();
	Havok_TagType* type;

	writePacked(listBytesTNam, nbTypes);						//first the number of types

	for (size_t i = 1; i < nbTypes; i++)
	{
		type = listType.at(i);
		str = type->name;

		size_t isfound = (size_t)-1;
		for (size_t k = 0; k < nbNames; k++)
		{
			if (listTSTR.at(k) == str)
			{
				isfound = k;
				break;
			}
		}
		if (isfound == (size_t)-1)
		{
			printf("Error: Name '%s' not found into TSTR. Skipped.\n", str.c_str());
			LibXenoverse::notifyError();
			isfound = 0;
		}

		writePacked(listBytesTNam, isfound);


		size_t nbTemplate = type->listTemplate.size();
		writePacked(listBytesTNam, nbTemplate);

		for (size_t j = 0; j < nbTemplate; j++)
		{
			Havok_TagTemplate* template_tmp = type->listTemplate.at(j);
			str = template_tmp->name;

			size_t isfound = (size_t)-1;
			for (size_t k = 0; k < nbNames; k++)
			{
				if (listTSTR.at(k) == str)
				{
					isfound = k;
					break;
				}
			}
			if (isfound == (size_t)-1)
			{
				printf("Error: Name '%s' not found into TSTR. Skipped.\n", str.c_str());
				LibXenoverse::notifyError();
				isfound = 0;
			}
			writePacked(listBytesTNam, isfound);

			writePacked(listBytesTNam, template_tmp->value);
		}
	}



	//TBOD
	std::vector<byte> listBytesTBod;
	size_t nbFunctNames = listFSTR.size();

	for (size_t i = 1; i < nbTypes; i++)
	{
		type = listType.at(i);

		writePacked(listBytesTBod, type->id);
		writePacked(listBytesTBod, type->parent ? type->parent->id : 0);
		writePacked(listBytesTBod, type->flags);

		if (type->flags & TagFlag::TF_SubType)
			writePacked(listBytesTBod, type->subTypeFlags);

		if ((type->flags & TagFlag::TF_Pointer) && ((type->subTypeFlags & 0xF) >= 6))
			writePacked(listBytesTBod, type->pointer ? type->pointer->id : 0);

		if (type->flags & TagFlag::TF_Version)
			writePacked(listBytesTBod, type->version);

		if (type->flags & TagFlag::TF_ByteSize)
		{
			writePacked(listBytesTBod, type->byteSize);
			writePacked(listBytesTBod, type->alignment);
		}

		if (type->flags & TagFlag::TF_AbstractValue)
			writePacked(listBytesTBod, type->abstractValue);


		if (type->flags & TagFlag::TF_Members)
		{
			size_t nbMembers = type->members.size();
			writePacked(listBytesTBod, nbMembers);

			for (size_t j = 0; j < nbMembers; j++)
			{
				Havok_TagMember* tagMember = type->members.at(j);
				str = tagMember->name;

				size_t isfound = (size_t)-1;
				for (size_t k = 0; k < nbFunctNames; k++)
				{
					if (listFSTR.at(k) == str)
					{
						isfound = k;
						break;
					}
				}
				if (isfound == (size_t)-1)
				{
					printf("Error: Member Name '%s' not found into FSTR. Skipped.\n", str.c_str());
					LibXenoverse::notifyError();
					isfound = 0;
				}
				writePacked(listBytesTBod, isfound);
				writePacked(listBytesTBod, tagMember->flags);
				writePacked(listBytesTBod, tagMember->byteOffset);
				writePacked(listBytesTBod, ((Havok_TagType*)tagMember->type)->id);
			}
		}


		if (type->flags & TagFlag::TF_Interfaces)
		{
			size_t nbTypeArray = type->interfaces.size();
			writePacked(listBytesTBod, nbTypeArray);

			for (size_t j = 0; j < nbTypeArray; j++)
			{
				Havok_TagInterface &inter = type->interfaces.at(j);
				writePacked(listBytesTBod, ((Havok_TagType*)inter.type)->id);
				writePacked(listBytesTBod, inter.value);
			}
		}

		//if (tagType->flags & TagFlag::TF_Unknown)
	}



	////////////////////////////// calculSize Type binary part 
	size_t size_Type = sizeof(Havok_PartHeader) * 2;
	for (size_t i = 0; i < nbNames; i++)
		size_Type += listTSTR.at(i).length() + 1;
	if (size_Type & 0x3) { size_Type += (0x4 - (size_Type & 0x3)); };		//padding 0

	size_Type += sizeof(Havok_PartHeader) + listBytesTNam.size();
	if (size_Type & 0x3) { size_Type += (0x4 - (size_Type & 0x3)); };		//padding 0

	size_Type += sizeof(Havok_PartHeader);
	for (size_t i = 0; i < nbFunctNames; i++)
		size_Type += listFSTR.at(i).length() + 1;
	if (size_Type & 0x3) { size_Type += (0x4 - (size_Type & 0x3)); };		//padding 0

	size_Type += sizeof(Havok_PartHeader) + listBytesTBod.size();
	if (size_Type & 0x3) { size_Type += (0x4 - (size_Type & 0x3)); };		//padding 0

	size_Type += sizeof(Havok_PartHeader);
	//if (size_Type & 0xf) { size_Type += (0x10 - (size_Type & 0xf)); };		//padding 0 by TPAD







	////////////////////////////// make type binary part 

	uint8_t* buf_type = new uint8_t[size_Type];
	memset(buf_type, 0, size_Type);				//fill by 0 to secure, and not having random memory.

	size_t offset = 0;

	Havok_PartHeader* part_hdr = (Havok_PartHeader*)(buf_type + offset);
	offset += sizeof(Havok_PartHeader);

	part_hdr->size = val32(size_Type);
	strcpy(&(part_hdr->signature[0]), Havok_TYPE_SIGNATURE);






	part_hdr = (Havok_PartHeader*)(buf_type + offset);
	offset += sizeof(Havok_PartHeader);

	size_t size_tmp = 0;
	for (size_t i = 0; i < nbNames; i++)
		size_tmp += listTSTR.at(i).length() + 1;
	if ((size_tmp + offset) & 0x3) { size_tmp += (0x4 - ((size_tmp + offset) & 0x3)); };		//padding 0
	size_tmp += sizeof(Havok_PartHeader);

	part_hdr->size = val32(size_tmp + 0x40000000);
	strcpy(&(part_hdr->signature[0]), Havok_TSTR_SIGNATURE);

	for (size_t i = 0; i < nbNames; i++)
	{
		strcpy((char*)(buf_type + offset), listTSTR.at(i).c_str());
		offset += listTSTR.at(i).length() + 1;
	}
	if (offset & 0x3) { offset += (0x4 - (offset & 0x3)); };		//padding 0



	part_hdr = (Havok_PartHeader*)(buf_type + offset);
	offset += sizeof(Havok_PartHeader);

	size_tmp = listBytesTNam.size();
	if ((size_tmp + offset) & 0x3) { size_tmp += (0x4 - ((size_tmp + offset) & 0x3)); };		//padding 0
	size_tmp += sizeof(Havok_PartHeader);

	part_hdr->size = val32(size_tmp + 0x40000000);
	strcpy(&(part_hdr->signature[0]), Havok_TNAM_SIGNATURE);

	size_t nbElements = listBytesTNam.size();
	for (size_t i = 0; i < nbElements; i++)
		buf_type[offset++] = listBytesTNam.at(i);

	if (offset & 0x3) { offset += (0x4 - (offset & 0x3)); };		//padding 0




	part_hdr = (Havok_PartHeader*)(buf_type + offset);
	offset += sizeof(Havok_PartHeader);

	size_tmp = 0;
	for (size_t i = 0; i < nbFunctNames; i++)
		size_tmp += listFSTR.at(i).length() + 1;
	if ((size_tmp + offset) & 0x3) { size_tmp += (0x4 - ((size_tmp + offset) & 0x3)); };		//padding 0
	size_tmp += sizeof(Havok_PartHeader);

	part_hdr->size = val32(size_tmp + 0x40000000);
	strcpy(&(part_hdr->signature[0]), Havok_FSTR_SIGNATURE);

	for (size_t i = 0; i < nbFunctNames; i++)
	{
		strcpy((char*)(buf_type + offset), listFSTR.at(i).c_str());
		offset += listFSTR.at(i).length() + 1;
	}
	if (offset & 0x3) { offset += (0x4 - (offset & 0x3)); };		//padding 0





	part_hdr = (Havok_PartHeader*)(buf_type + offset);
	offset += sizeof(Havok_PartHeader);

	size_tmp = listBytesTBod.size();
	if ((size_tmp + offset) & 0x3) { size_tmp += (0x4 - ((size_tmp + offset) & 0x3)); };		//padding 0
	size_tmp += sizeof(Havok_PartHeader);

	part_hdr->size = val32(size_tmp + 0x40000000);
	strcpy(&(part_hdr->signature[0]), Havok_TBOD_SIGNATURE);

	nbElements = listBytesTBod.size();
	for (size_t i = 0; i < nbElements; i++)
		buf_type[offset++] = listBytesTBod.at(i);
	if (offset & 0x3) { offset += (0x4 - (offset & 0x3)); };		//padding 0





	part_hdr = (Havok_PartHeader*)(buf_type + offset);
	offset += sizeof(Havok_PartHeader);

	size_tmp = 0;
	//if (offset & 0xf)
	//	size_tmp = (0x10 - (offset & 0xf));
	//offset += size_tmp;
	size_tmp += sizeof(Havok_PartHeader);

	part_hdr->size = val32(size_tmp + 0x40000000);
	memcpy(&(part_hdr->signature[0]), Havok_TPAD_SIGNATURE, 4);




















	

	//////////////////////////////  make data part
	std::vector<byte> listBytesData;

	size_t nbItems = listItem.size();
	std::vector<Havok_TagItem*> listOrdered = listItem;				// for having the same order as before, (as I didn't find the logique behind),
	std::sort(listOrdered.begin(), listOrdered.end(), &Havok_TagItem::offsetOrder);			//todo find

	for (size_t i = 0; i < nbItems; i++)
	{
		Havok_TagItem* item = listOrdered.at(i);
		item->offset = listBytesData.size();
		item->count = item->value.size();

		printf("---- writeItem(%s) at %s \n", UnsignedToString(item->type->id, false).c_str(), UnsignedToString(listBytesData.size(), true).c_str());

		for (size_t j = 0; j < item->count; j++)
			writeObject((Havok_TagObject*)item->value.at(j), listBytesData, listItem, listType);


		//padding 0, matching alignement
		{
			offset = listBytesData.size();

			size_t alignement = 16;
			if (i + 1 < nbItems)
			{
				type = listOrdered.at(i + 1)->type;
				alignement = type->alignment;
				while (type->parent)
				{
					type = type->parent;
					alignement = (type->alignment>alignement) ? type->alignment : alignement;
				}
			}

			if ((alignement > 0) && (offset & (alignement - 1)))
			{
				offset = (alignement - (offset & (alignement - 1)));
				for (size_t j = 0; j < offset; j++)
					listBytesData.push_back(0);
			};
		}

		printf("---- \n\n");
	}



	size_t nbBytes_Data = listBytesData.size();
	size_t size_Data = sizeof(Havok_PartHeader) + nbBytes_Data;

	uint8_t* buf_Data = new uint8_t[size_Data];
	memset(buf_Data, 0, size_Data);				//fill by 0 to secure, and not having random memory.


	part_hdr = (Havok_PartHeader*)(buf_Data);
	offset = sizeof(Havok_PartHeader);

	part_hdr->size = val32(size_Data + 0x40000000);
	strcpy(&(part_hdr->signature[0]), Havok_DATA_SIGNATURE);


	for (size_t i = 0; i < nbBytes_Data; i++)
		buf_Data[offset + i] = listBytesData.at(i);








	//////////////////////////////  make item part.

	std::vector<byte> listBytesItem;
	size_t flags = 0;

	for (size_t i = 0; i < nbItems; i++)
	{
		Havok_TagItem* item = listItem.at(i);

		flags = (item->type->id & 0xFFFFFF) + (item->type->id != 0 ? ((item->isPtr) ? 0x10000000 : 0x20000000) : 0);
		writeFormat(flags, listBytesItem, TST_Int32);
		writeFormat(item->offset, listBytesItem, TST_Int32);
		writeFormat(item->value.size(), listBytesItem, TST_Int32);
	}


	size_t nbBytesItem = listBytesItem.size();
	size_t size_Item = sizeof(Havok_PartHeader) * 2 + nbBytesItem;

	uint8_t* buf_Item = new uint8_t[size_Item];
	memset(buf_Item, 0, size_Item);				//fill by 0 to secure, and not having random memory.

	part_hdr = (Havok_PartHeader*)(buf_Item);
	offset = sizeof(Havok_PartHeader);

	part_hdr->size = val32(size_Item);
	strcpy(&(part_hdr->signature[0]), Havok_INDX_SIGNATURE);


	part_hdr = (Havok_PartHeader*)(buf_Item + offset);
	offset += sizeof(Havok_PartHeader);

	part_hdr->size = val32(nbBytesItem + sizeof(Havok_PartHeader) + 0x40000000);
	strcpy(&(part_hdr->signature[0]), Havok_ITEM_SIGNATURE);


	for (size_t i = 0; i < nbBytesItem; i++)
		buf_Item[offset + i] = listBytesItem.at(i);








	////////////////////////////// fuuuuuuuuusion  yaaaaahhh

	size_t size = sizeof(Havok_PartHeader) * 2 + sizeof(Havok_Version) + size_Data + size_Type + size_Item;
	uint8_t* buf = new uint8_t[size];
	if (!buf)
	{
		LOG_DEBUG("%s: Memory allocation error (0x%x)\n", FUNCNAME, size);
		LibXenoverse::notifyError();
		return nullptr;
	}
	*psize = size;
	offset = 0;

	part_hdr = (Havok_PartHeader*)(buf + offset);					//header
	offset += sizeof(Havok_PartHeader);
	part_hdr->size = val32(size);
	strcpy(&(part_hdr->signature[0]), Havok_TAG0_SIGNATURE);

	part_hdr = (Havok_PartHeader*)(buf + offset);					//version
	offset += sizeof(Havok_PartHeader);
	part_hdr->size = val32(sizeof(Havok_PartHeader) + sizeof(Havok_Version) + 0x40000000);
	strcpy(&(part_hdr->signature[0]), Havok_SDKV_SIGNATURE);

	Havok_Version* version = (Havok_Version*)(buf + offset);
	offset += sizeof(Havok_Version);
	strcpy(&(version->year[0]), Havok_v2015_SIGNATURE);
	strcpy(&(version->major[0]), Havok_major01_SIGNATURE);
	strcpy(&(version->minor[0]), Havok_minor00_SIGNATURE);


	memcpy(buf + offset, buf_Data, size_Data);
	offset += size_Data;

	memcpy(buf + offset, buf_type, size_Type);
	offset += size_Type;

	memcpy(buf + offset, buf_Item, size_Item);
	offset += size_Item;



	////////////////////////////// Clean
	delete[] buf_type;
	delete[] buf_Data;
	delete[] buf_Item;


	return buf;
}












/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/*-------------------------------------------------------------------------------\
|                             saveXml											 |
\-------------------------------------------------------------------------------*/
void Havok::saveXml(string filename)
{
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement* rootNode = export_Xml(filename);
	doc.LinkEndChild(rootNode);

	doc.SaveFile(filename);
}
/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* Havok::export_Xml(string filename)
{
	TiXmlElement* parent = new TiXmlElement("Havok");

	parent->SetAttribute("version", string(Havok_v2015_SIGNATURE) + "." + Havok_major01_SIGNATURE + "." + Havok_minor00_SIGNATURE);

	
	TiXmlElement* node_data = new TiXmlElement("Data");
	parent->LinkEndChild(node_data);

	if (rootObject)
		node_data->LinkEndChild(rootObject->exportXml());
	

	TiXmlElement* node_Type = new TiXmlElement("ListType");
	parent->LinkEndChild(node_Type);
	
	size_t nbElements = listType.size();
	for (size_t i = 0; i<nbElements; i++)
		node_Type->LinkEndChild(listType.at(i)->exportXml(listType));


	TiXmlElement* node_ItemList = new TiXmlElement("ListItem");
	parent->LinkEndChild(node_ItemList);

	node_ItemList->LinkEndChild(new TiXmlComment("ListItem is not necessary, it's just to have the same order in binary."));
	
	nbElements = listItem.size();
	for (size_t i = 0; i<nbElements; i++)
		node_ItemList->LinkEndChild(listItem.at(i)->exportXml());
	

	return parent;
}
/*-------------------------------------------------------------------------------\
|                             exportXml						                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* Havok_TagType::exportXml(std::vector<Havok_TagType*> &listType)
{
	size_t nbTypes = listType.size();

	TiXmlElement* node = new TiXmlElement("Type");
	node->SetAttribute("id", id);
	node->SetAttribute("name", name);

	if (parent != 0)
	{
		for (size_t i = 0; i<nbTypes; i++)
			if (parent == listType.at(i))
				node->SetAttribute("parent", i);
	}

	if (pointer != 0)
	{
		for (size_t i = 0; i<nbTypes; i++)
			if (pointer == listType.at(i))
				node->SetAttribute("pointer", i);
	}

	node->SetAttribute("flags", flags);
	node->SetAttribute("subTypeFlags", subTypeFlags);
	if (version != 0)
		node->SetAttribute("version", version);

	node->SetAttribute("byteSize", byteSize);
	node->SetAttribute("alignment", alignment);
	if (abstractValue != 0)
		node->SetAttribute("abstractValue", abstractValue);

	size_t nbElements = listTemplate.size();
	for (size_t i = 0; i < nbElements; i++)
		node->LinkEndChild(listTemplate.at(i)->exportXml());

	nbElements = members.size();
	for (size_t i = 0; i < nbElements; i++)
		node->LinkEndChild(members.at(i)->exportXml(listType));

	nbElements = interfaces.size();
	for (size_t i = 0; i < nbElements; i++)
		node->LinkEndChild(interfaces.at(i).exportXml(listType));

	return node;
}
/*-------------------------------------------------------------------------------\
|                             exportXml						                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* Havok_TagTemplate::exportXml()
{
	TiXmlElement* node = new TiXmlElement("template");
	node->SetAttribute("name", name);
	node->SetAttribute("value", value);
	return node;
}
/*-------------------------------------------------------------------------------\
|                             exportXml						                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* Havok_TagMember::exportXml(std::vector<Havok_TagType*> &listType)
{
	size_t nbTypes = listType.size();

	TiXmlElement* node = new TiXmlElement("member");
	node->SetAttribute("name", name);
	node->SetAttribute("flags", flags);
	node->SetAttribute("offset", byteOffset);

	if (type != 0)
	{
		for (size_t i = 0; i<nbTypes; i++)
			if (type == listType.at(i))
				node->SetAttribute("type", i);
	}
	return node;
}
/*-------------------------------------------------------------------------------\
|                             exportXml						                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* Havok_TagInterface::exportXml(std::vector<Havok_TagType*> &listType)
{
	size_t nbTypes = listType.size();

	TiXmlElement* node = new TiXmlElement("interface");
	node->SetAttribute("flags", value);

	if (type != 0)
	{
		for (size_t i = 0; i<nbTypes; i++)
			if (type == listType.at(i))
				node->SetAttribute("type", i);
	}
	return node;
}
/*-------------------------------------------------------------------------------\
|                             exportXml						                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* Havok_TagObject::exportXml(string name)
{
	TiXmlElement* node = new TiXmlElement("Object");
	if (name.length())
		node->SetAttribute("Name", name);

	node->SetAttribute("TypeId", type->id);

	Havok_TagType* supertype = type->superType();
	//node->SetAttribute("supTNam", std::to_string(supertype->id) + "_" + supertype->name);

	if (supertype->subType() == Havok::TST_Bool)
	{
		node->SetAttribute("type", "bool");
		node->SetAttribute("value", b_value ? "true" : "false");

	}else if (supertype->subType() == Havok::TST_Int) {
		node->SetAttribute("type", "int");
		node->SetAttribute("value", std::to_string(i_value));

	}else if (supertype->subType() == Havok::TST_Float) {
		node->SetAttribute("type", "float");
		node->SetAttribute("value", FloatToString(f_value));

	}else if (supertype->subType() == Havok::TST_String) {
		node->SetAttribute("type", "string");
		node->SetAttribute("value", s_value);

		size_t nbObj = listObjectString.size();
		for (size_t i = 0; i<nbObj; i++)
			node->LinkEndChild(listObjectString.at(i)->exportXml());


	}else if (supertype->subType() == Havok::TST_Pointer) {
		node->SetAttribute("type", "pointer");
		if (objectPointer)
			node->LinkEndChild(objectPointer->exportXml());

	}else if (supertype->subType() == Havok::TST_Class) {

		std::vector<Havok_TagMember*> listMembers = type->allMembers();
		size_t nbMember = listMembers.size();

		node->SetAttribute("TName", type->name);
		node->SetAttribute("type", "Class");
		size_t nbObj = listObjectClass.size();
		for (size_t i = 0; i<nbObj; i++)
			node->LinkEndChild(listObjectClass.at(i)->exportXml((i <= nbMember) ? listMembers.at(i)->name : ""));

	}else if (supertype->subType() == Havok::TST_Array) {

		node->SetAttribute("type", "Array");
		size_t nbObj = listObjectArray.size();
		for (size_t i = 0; i<nbObj; i++)
			node->LinkEndChild(listObjectArray.at(i)->exportXml());

	}else if (supertype->subType() == Havok::TST_Tuple) {

		node->SetAttribute("type", "Tuple");
		size_t nbObj = listObjectTuple.size();
		for (size_t i = 0; i<nbObj; i++)
			node->LinkEndChild(listObjectTuple.at(i)->exportXml());
	}else {
		printf("unknow superType->subType() : %s", EMO_BaseFile::UnsignedToString(supertype->subType(), true).c_str());
	}

	return node;
}
/*-------------------------------------------------------------------------------\
|                             exportXml						                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* Havok_TagItem::exportXml()
{
	TiXmlElement* node_Item = new TiXmlElement("Item");
	node_Item->SetAttribute("type", type ? type->id : 0);
	node_Item->SetAttribute("isPtr", isPtr ? "true" : "false");
	node_Item->SetAttribute("offset", EMO_BaseFile::UnsignedToString(offset, true));
	node_Item->SetAttribute("count", count);
	return node_Item;
}























//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------\
|                             Compile				                             |
\-------------------------------------------------------------------------------*/
bool Havok::Compile(TiXmlDocument *doc, bool big_endian)
{
	this->big_endian = big_endian;

	TiXmlHandle handle(doc);
	TiXmlElement* rootNode = EMO_BaseFile::FindRoot(&handle, "Havok");

	if (!rootNode)
	{
		LOG_DEBUG("Cannot find\"Havok\" in xml.\n");
		return false;
	}

	return import_Xml(rootNode);
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool Havok::import_Xml(TiXmlElement* rootNode)
{
	Reset();


	//////////////////////////////  read Types Xml part to make Havok_TagType, Havok_TagMember, ...
	TiXmlElement* node_Types = rootNode->FirstChildElement("ListType");
	if (!node_Types)
	{
		printf("Cannot find\"ListType\" in xml.\n");
		return false;
	}
	TiXmlElement* node_Data = rootNode->FirstChildElement("Data");
	if (!node_Data)
	{
		printf("Cannot find\"Data\" in xml.\n");
		return false;
	}
	TiXmlElement* node_Item = rootNode->FirstChildElement("ListItem");
	if (!node_Item)
	{
		printf("Cannot find\"ListItem\" in xml.\n");
		return false;
	}

	for (TiXmlElement* node = node_Types->FirstChildElement("Type"); node; node = node->NextSiblingElement("Type"))
	{
		listType.push_back(new Havok_TagType(0));
		listType.back()->importXml(node);
	}

	//second time to have pointer references
	size_t inc = 0;
	for (TiXmlElement* node = node_Types->FirstChildElement("Type"); node; node = node->NextSiblingElement("Type"))
		listType.at(inc++)->importXml_secondStep(node, listType);






	//////////////////////////////  read Data Xml part to make Havok_TagObject, and fill items
	listItem.push_back(new Havok_TagItem(0, listType.at(0)));				//None
	for (TiXmlElement* node = node_Data->FirstChildElement("Object"); node; node = node->NextSiblingElement("Object"))
	{
		Havok_TagObject* obj = new Havok_TagObject();
		obj->importXml(node, listType, listItem, 0, listItem.size(), 0);

		if (!rootObject)
			rootObject = obj;
	}
	std::sort(listItem.begin(), listItem.end(), &Havok_TagItem::levelOrder);







	//////////////////////////////  read Item Xml part to Keep parts in the same order as before (and having the same order help to have 0 differences on all the 16xxx havok files. and I didn't see any way to find this offsetData order)
	//todo find the solution to get back the offsetData order wihtout reading the Item Xml part 
	size_t nbItems = listItem.size();
	size_t nbTypes = listType.size();
	std::vector<Havok_TagItem*> listNoOrderer = listItem;
	listItem.clear();
	std::vector<bool> listIsCopyed;
	listIsCopyed.resize(nbItems, false);
	std::vector<size_t> listStart_forEachId;			//to avoid to have 2 same items
	listStart_forEachId.resize(nbTypes, 0);
	size_t typeId = 0;
	string str;
	

	for (TiXmlElement* node = node_Item->FirstChildElement("Item"); node; node = node->NextSiblingElement("Item"))
	{
		node->QueryUnsignedAttribute("type", &typeId);

		if (typeId >= nbTypes)
			continue;

		for (size_t i = listStart_forEachId.at(typeId); i < nbItems; i++)
		{
			if (listNoOrderer.at(i)->type->id== typeId)
			{
				listItem.push_back(listNoOrderer.at(i));
				listIsCopyed.at(i) = true;
				listStart_forEachId.at(typeId) = i + 1;						//to get another (same if same id)

				listItem.back()->importXml(node);
				break;
			}
		}
	}

	for (size_t i = 0; i < nbItems; i++)									//new part at the end.
		if (listIsCopyed.at(i) == false)
			listItem.push_back(listNoOrderer.at(i));


	return (rootObject != 0);
}
/*-------------------------------------------------------------------------------\
|                             importXml						                     |
\-------------------------------------------------------------------------------*/
void Havok_TagType::importXml(TiXmlElement* node)
{
	name = "None";
	version = 0;
	abstractValue = 0;

	node->QueryUnsignedAttribute("id", &id);
	node->QueryStringAttribute("name", &name);
	node->QueryUnsignedAttribute("flags", &flags);
	node->QueryUnsignedAttribute("subTypeFlags", &subTypeFlags);
	node->QueryUnsignedAttribute("byteSize", &byteSize);
	node->QueryUnsignedAttribute("alignment", &alignment);

	if (node->Attribute("version"))
		node->QueryUnsignedAttribute("version", &version);

	if (node->Attribute("abstractValue"))
		node->QueryUnsignedAttribute("abstractValue", &abstractValue);

	return;
}
/*-------------------------------------------------------------------------------\
|                             importXml_secondStep			                     |
\-------------------------------------------------------------------------------*/
void Havok_TagType::importXml_secondStep(TiXmlElement* node, std::vector<Havok_TagType*> &listType)
{
	size_t nbTypes = listType.size();

	if (node->Attribute("parent"))
	{
		size_t id_tmp = 0;
		node->QueryUnsignedAttribute("parent", &id_tmp);

		for (size_t i = 0; i < nbTypes; i++)
			if (listType.at(i)->id == id_tmp)
				parent = listType.at(i);
	}

	if (node->Attribute("pointer"))
	{
		size_t id_tmp = 0;
		node->QueryUnsignedAttribute("pointer", &id_tmp);

		for (size_t i = 0; i < nbTypes; i++)
			if (listType.at(i)->id == id_tmp)
				pointer = listType.at(i);
	}


	for (TiXmlElement* node_tmp = node->FirstChildElement("template"); node_tmp; node_tmp = node_tmp->NextSiblingElement("template"))
	{
		listTemplate.push_back(new Havok_TagTemplate());
		listTemplate.back()->importXml(node_tmp);

		size_t id_tmp = listTemplate.back()->value;

		if (listTemplate.back()->isType())
		{
			for (size_t i = 0; i < nbTypes; i++)
				if (listType.at(i)->id == id_tmp)
					listTemplate.back()->value_ptr = listType.at(i);
		}
	}


	for (TiXmlElement* node_tmp = node->FirstChildElement("member"); node_tmp; node_tmp = node_tmp->NextSiblingElement("member"))
	{
		members.push_back(new Havok_TagMember());
		size_t id_tmp = members.back()->importXml(node_tmp);

		if (id_tmp != (size_t)-1)
		{
			for (size_t i = 0; i < nbTypes; i++)
				if (listType.at(i)->id == id_tmp)
					members.back()->type = listType.at(i);
		}
	}

	for (TiXmlElement* node_tmp = node->FirstChildElement("interface"); node_tmp; node_tmp = node_tmp->NextSiblingElement("interface"))
	{
		interfaces.push_back(Havok_TagInterface());
		size_t id_tmp = interfaces.back().importXml(node_tmp);

		if (id_tmp != (size_t)-1)
		{
			for (size_t i = 0; i < nbTypes; i++)
				if (listType.at(i)->id == id_tmp)
					interfaces.back().type = listType.at(i);
		}
	}

	return;
}
/*-------------------------------------------------------------------------------\
|                             importXml						                     |
\-------------------------------------------------------------------------------*/
void Havok_TagTemplate::importXml(TiXmlElement* node)
{
	node->QueryStringAttribute("name", &name);
	node->QueryUnsignedAttribute("value", &value);
}
/*-------------------------------------------------------------------------------\
|                             importXml						                     |
\-------------------------------------------------------------------------------*/
size_t Havok_TagMember::importXml(TiXmlElement* node)
{
	node->QueryStringAttribute("name", &name);
	node->QueryUnsignedAttribute("flags", &flags);
	node->QueryUnsignedAttribute("offset", &byteOffset);
	size_t type_id = (size_t)-1;
	if (node->Attribute("type"))
		node->QueryUnsignedAttribute("type", &type_id);
	return type_id;
}
/*-------------------------------------------------------------------------------\
|                             importXml						                     |
\-------------------------------------------------------------------------------*/
size_t Havok_TagInterface::importXml(TiXmlElement* node)
{
	node->QueryUnsignedAttribute("flags", &value);
	size_t type_id = (size_t)-1;
	if (node->Attribute("type"))
		node->QueryUnsignedAttribute("type", &type_id);
	return type_id;
}
/*-------------------------------------------------------------------------------\
|                             importXml						                     |
\-------------------------------------------------------------------------------*/
bool Havok_TagObject::importXml(TiXmlElement* node, std::vector<Havok_TagType*> &listType, std::vector<Havok_TagItem*> &listItems, Havok_TagItem* parentAttachement, size_t addToItemList, size_t level)
{
	string str = "";
	size_t type_id = 0;
	node->QueryUnsignedAttribute("TypeId", &type_id);


	type = 0;
	size_t nbTypes = listType.size();
	for (size_t i = 0; i < nbTypes; i++)
	{
		if (listType.at(i)->id == type_id)
		{
			type = listType.at(i);
			break;
		}
	}
	if (type == 0)
		return false;


	Havok_TagType* supertype = type->superType();



	attachement = parentAttachement;


	if (addToItemList != 0)
	{
		if (addToItemList >= listItems.size())
		{
			for (size_t i = listItems.size(); i <= addToItemList; i++)
				listItems.push_back(new Havok_TagItem(level, listType.at(0)));

			listItems.at(addToItemList)->type = type;
		}

		listItems.at(addToItemList)->value.push_back(this);
		listItems.at(addToItemList)->count++;
		attachement = listItems.at(addToItemList);
	}


	bool isPtr = false;
	bool isParentPtr = false;


	if (supertype->subType() == Havok::TST_Bool)
	{
		node->QueryStringAttribute("value", &str);
		b_value = (str == "true");

	}else if (supertype->subType() == Havok::TST_Int) {

		node->QueryStringAttribute("value", &str);
		i_value = (supertype->subTypeFlags & Havok::TST_Int64) ? EMO_BaseFile::GetUnsigned64(str, 0) : EMO_BaseFile::GetUnsigned(str, 0);

	}else if (supertype->subType() == Havok::TST_Float) {

		node->QueryStringAttribute("value", &str);
		f_value = StringToFloat(str);

	}else if (supertype->subType() == Havok::TST_String) {
		node->QueryStringAttribute("value", &s_value);

		for (TiXmlElement* node_tmp = node->FirstChildElement("Object"); node_tmp; node_tmp = node_tmp->NextSiblingElement("Object"))
		{
			Havok_TagObject* obj = new Havok_TagObject();
			isPtr = obj->importXml(node_tmp, listType, listItems, 0, 0, level + 1) || isPtr;
			isParentPtr = true;
			listObjectString.push_back(obj);
		}

	}else if (supertype->subType() == Havok::TST_Pointer) {

		size_t itemIndex = listItems.size();
		for (TiXmlElement* node_tmp = node->FirstChildElement("Object"); node_tmp; node_tmp = node_tmp->NextSiblingElement("Object"))
		{
			objectPointer = new Havok_TagObject();
			isPtr = objectPointer->importXml(node_tmp, listType, listItems, 0, itemIndex, level + 1) || isPtr;
			isParentPtr = true;
			break;
		}

	}else if (supertype->subType() == Havok::TST_Class) {

		for (TiXmlElement* node_tmp = node->FirstChildElement("Object"); node_tmp; node_tmp = node_tmp->NextSiblingElement("Object"))
		{
			Havok_TagObject* obj = new Havok_TagObject();
			isPtr = obj->importXml(node_tmp, listType, listItems, 0, 0, level + 1) || isPtr;
			isParentPtr = true;
			listObjectClass.push_back(obj);
		}

	}else if (supertype->subType() == Havok::TST_Array) {

		size_t itemIndex = listItems.size();
		for (TiXmlElement* node_tmp = node->FirstChildElement("Object"); node_tmp; node_tmp = node_tmp->NextSiblingElement("Object"))
		{
			Havok_TagObject* obj = new Havok_TagObject();
			isPtr = obj->importXml(node_tmp, listType, listItems, 0, itemIndex, level + 1) || isPtr;
			isParentPtr = true;
			listObjectArray.push_back(obj);
		}

	}else if (supertype->subType() == Havok::TST_Tuple) {

		for (TiXmlElement* node_tmp = node->FirstChildElement("Object"); node_tmp; node_tmp = node_tmp->NextSiblingElement("Object"))
		{
			Havok_TagObject* obj = new Havok_TagObject();
			obj->importXml(node_tmp, listType, listItems, 0, 0, level + 1);
			listObjectTuple.push_back(obj);
		}

	}else {
		printf("unknow superType->subType() : %s", EMO_BaseFile::UnsignedToString(supertype->subType(), true).c_str());
	}


	if ((attachement) && (attachement != parentAttachement))
		attachement->isPtr = isPtr;


	return isParentPtr;
}
/*-------------------------------------------------------------------------------\
|                             importXml						                     |
\-------------------------------------------------------------------------------*/
bool Havok_TagItem::importXml(TiXmlElement* node)
{
	string str = "";
	node->QueryStringAttribute("offset", &str);
	offset = EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFFFFFF;

	node->QueryStringAttribute("isPtr", &str);
	isPtr = str == "true";

	return true;
}









//////////////////////////////////////////////////////////////////////////////////////////////////////////////////












/*-------------------------------------------------------------------------------\
|                             readObject					                     |
\-------------------------------------------------------------------------------*/
Havok_TagObject* Havok::readObject(size_t index, const uint8_t *buf, size_t size, Havok_TagType* type, size_t &offset, std::vector<Havok_TagItem*> &listItem, std::vector<Havok_TagType*> &listType, Havok_TagItem* parentAttachement)
{
	printf("**** readObject(%i, %s) at %s \n", type->id, type->name.c_str(), UnsignedToString(offset - 0x20, true).c_str());

	Havok_TagType* typeOrigin = type;
	type = type->superType();

	Havok_TagObject* obj = new Havok_TagObject();
	obj->type = typeOrigin;
	obj->attachement = parentAttachement;


	size_t nbItem = listItem.size();
	for (size_t i = 0; i < nbItem; i++)
	{
		if (listItem.at(i)->type->id == typeOrigin->id)
		{
			obj->attachement = listItem.at(i);
			break;
		}
	}


	string type_str = "";
	if (type->subType() == TST_Bool)
	{
		printf("-> Bool at %s\n", UnsignedToString(offset - 0x20, true).c_str());

		obj->b_value = (readFormat(buf + offset, size - offset, type->subTypeFlags, type_str) > 0);

	}else if (type->subType() == TST_Int) {

		printf("-> Int at %s\n", UnsignedToString(offset - 0x20, true).c_str());

		obj->i_value = readFormat(buf + offset, size - offset, type->subTypeFlags, type_str);

	}else if (type->subType() == TST_Float) {

		printf("-> Float at %s\n", UnsignedToString(offset - 0x20, true).c_str());

		float* values = (float*)(buf + offset);
		obj->f_value = values[0];

	}else if (type->subType() == TST_String) {

		printf("-> String at %s\n", UnsignedToString(offset - 0x20, true).c_str());

		obj->listObjectString = readItemPtr(buf, size, offset, listItem, listType);

		obj->s_value = "";
		size_t nbObj = obj->listObjectString.size();
		for (size_t i = 0; i < nbObj; i++)
			obj->s_value += (char)(obj->listObjectString.at(i)->i_value);

	}else if (type->subType() == TST_Pointer) {

		printf("-> Pointer at %s\n", UnsignedToString(offset - 0x20, true).c_str());

		std::vector<Havok_TagObject*> listObj = readItemPtr(buf, size, offset, listItem, listType);


		obj->objectPointer = 0;
		if (listObj.size() == 1)
		{
			obj->objectPointer = listObj.at(0);
		}else {
			size_t nbObj = listObj.size();
			for (size_t i = 0; i < nbObj; i++)
				delete listObj.at(i);
		}

	}else if (type->subType() == TST_Class) {

		std::vector<Havok_TagMember*> listMembers = type->allMembers();

		size_t nbMember = listMembers.size();
		size_t offset_tmp = 0;
		for (size_t i = 0; i < nbMember; i++)
		{
			offset_tmp = offset + listMembers.at(i)->byteOffset;
			printf("-> Member %s at %s\n", listMembers.at(i)->name.c_str(), UnsignedToString(offset_tmp - 0x20, true).c_str());
			obj->listObjectClass.push_back(readObject(i, buf, size, (Havok_TagType*)listMembers.at(i)->type, offset_tmp, listItem, listType, obj->attachement));
		}


	}else if (type->subType() == TST_Array) {

		printf("-> Array at %s\n", UnsignedToString(offset - 0x20, true).c_str());
		obj->listObjectArray = readItemPtr(buf, size, offset, listItem, listType);

	}else if (type->subType() == TST_Tuple) {

		printf("-> Tuple at %s\n", UnsignedToString(offset - 0x20, true).c_str());

		size_t nbTuple = type->tupleSize();
		for (size_t i = 0; i < nbTuple; i++)
		{
			size_t offset_tmp = offset + i * type->pointer->superType()->byteSize;
			obj->listObjectTuple.push_back(readObject(i, buf, size, type->pointer, offset_tmp, listItem, listType, obj->attachement));
		}
	}



	offset += type->byteSize;
	printf("**** offset + type->byteSize (%s) -> %s\n", UnsignedToString(type->byteSize, true).c_str(), UnsignedToString(offset - 0x20, true).c_str());
	return obj;
}
/*-------------------------------------------------------------------------------\
|                             writeObject					                     |
\-------------------------------------------------------------------------------*/
void Havok::writeObject(Havok_TagObject* obj, std::vector<byte> &listBytesData, std::vector<Havok_TagItem*> &listItem, std::vector<Havok_TagType*> &listType)
{
	Havok_TagType* type = obj->type;

	printf("**** writeObject(%i, %s) at %s \n", type->id, type->name.c_str(), UnsignedToString(listBytesData.size(), true).c_str());

	Havok_TagType* typeOrigin = type;
	type = type->superType();


	if (typeOrigin->id == 3)
		int aa = 42;


	size_t statOffset = listBytesData.size();

	if (type->subType() == TST_Bool)
	{
		printf("-> Bool at %s\n", UnsignedToString(listBytesData.size(), true).c_str());

		writeFormat(obj->b_value ? 1 : 0, listBytesData, type->subTypeFlags);

	}
	else if (type->subType() == TST_Int) {

		printf("-> Int at %s\n", UnsignedToString(listBytesData.size(), true).c_str());
		writeFormat(obj->i_value, listBytesData, type->subTypeFlags);

	}
	else if (type->subType() == TST_Float) {

		printf("-> Float at %s\n", UnsignedToString(listBytesData.size(), true).c_str());
		writeFormat(*((uint32_t*)(&obj->f_value)), listBytesData, TST_Int32);

	}
	else if (type->subType() == TST_String) {

		printf("-> String at %s\n", UnsignedToString(listBytesData.size(), true).c_str());

		if (obj->listObjectString.size() == 0)
		{
			writeFormat(0, listBytesData, TST_Int32);						//index 0 to target item[0] with type None
		}
		else {

			Havok_TagItem* item = obj->listObjectString.at(0)->attachement;

			bool isfound = false;
			size_t nbItems = listItem.size();
			for (size_t i = 0; i < nbItems; i++)
			{
				if (listItem.at(i) == item)
				{
					isfound = true;

					writeFormat(i, listBytesData, TST_Int32);						//index 0 to target item[0] with type None

					break;
				}
			}
			if (!isfound)
				writeFormat(0, listBytesData, TST_Int32);						//index 0 to target item[0] with type None
		}

	}
	else if (type->subType() == TST_Pointer) {

		printf("-> Pointer at %s\n", UnsignedToString(listBytesData.size(), true).c_str());

		if (obj->objectPointer == 0)
		{
			writeFormat(0, listBytesData, TST_Int32);						//index 0 to target item[0] with type None

		}
		else {
			Havok_TagItem* item = obj->objectPointer->attachement;

			bool isfound = false;
			size_t nbItems = listItem.size();
			for (size_t i = 0; i < nbItems; i++)
			{
				if (listItem.at(i) == item)
				{
					isfound = true;

					writeFormat(i, listBytesData, TST_Int32);						//index 0 to target item[0] with type None

					break;
				}
			}
			if (!isfound)
				writeFormat(0, listBytesData, TST_Int32);						//index 0 to target item[0] with type None
		}




	}
	else if (type->subType() == TST_Class) {


		size_t offset_tmp = listBytesData.size();

		std::vector<Havok_TagMember*> listMembers = type->allMembers();
		size_t nbMember = listMembers.size();
		for (size_t i = 0; i < nbMember; i++)
		{
			if (listBytesData.size() - offset_tmp < listMembers.at(i)->byteOffset)			//sometime byteSize is not good enougth between member (because wrong interpretation of template), So we have to deal with to have the same padding 0
			{
				for (size_t j = listBytesData.size() - offset_tmp; j < listMembers.at(i)->byteOffset; j++)
					listBytesData.push_back(0);
			}

			printf("-> Member %s at %s\n", listMembers.at(i)->name.c_str(), UnsignedToString(listBytesData.size(), true).c_str());

			writeObject(obj->listObjectClass.at(i), listBytesData, listItem, listType);
		}



	}
	else if (type->subType() == TST_Array) {

		printf("-> Array at %s\n", UnsignedToString(listBytesData.size(), true).c_str());

		if (obj->listObjectArray.size() == 0)
		{
			writeFormat(0, listBytesData, TST_Int32);						//index 0 to target item[0] with type None
		}
		else {

			Havok_TagItem* item = obj->listObjectArray.at(0)->attachement;

			bool isfound = false;
			size_t nbItems = listItem.size();
			for (size_t i = 0; i < nbItems; i++)
			{
				if (listItem.at(i) == item)
				{
					isfound = true;

					writeFormat(i, listBytesData, TST_Int32);						//index 0 to target item[0] with type None

					break;
				}
			}
			if (!isfound)
				writeFormat(0, listBytesData, TST_Int32);						//index 0 to target item[0] with type None
		}


	}
	else if (type->subType() == TST_Tuple) {

		printf("-> Tuple at %s\n", UnsignedToString(listBytesData.size(), true).c_str());

		size_t nbTuple = type->tupleSize();
		for (size_t i = 0; i < nbTuple; i++)
			writeObject(obj->listObjectTuple.at(i), listBytesData, listItem, listType);
	}


	if (listBytesData.size() - statOffset != type->byteSize)
	{
		printf("warning : dataSize = %i is different of %i.\n", listBytesData.size() - statOffset, type->byteSize);
		LibXenoverse::notifyWarning();

		for (size_t i = listBytesData.size() - statOffset; i < type->byteSize; i++)
			listBytesData.push_back(0);							//padding  0
	}

	printf("**** End writeObject(%i) at %s \n", type->id, UnsignedToString(listBytesData.size(), true).c_str());
}

/*-------------------------------------------------------------------------------\
|                             readItemPtr					                     |
\-------------------------------------------------------------------------------*/
std::vector<Havok_TagObject*> Havok::readItemPtr(const uint8_t *buf, size_t size, size_t &offset, std::vector<Havok_TagItem*> &listItem, std::vector<Havok_TagType*> &listType, bool indexInversed)
{
	printf("---- readItemPtr() at %s \n", UnsignedToString(offset - 0x20, true).c_str());

	std::vector<Havok_TagObject*> ret;
	string str;
	size_t index = (size_t)readFormat(buf + offset, size, TST_Int32, str, indexInversed);
	if ((index == 0) || (index >= listItem.size()))
		return ret;

	Havok_TagItem* item = listItem.at(index);
	if (item->value.size() == 0)
	{
		size_t offset_tmp = 0;
		for (size_t i = 0; i < item->count; i++)
		{
			offset_tmp = item->offset + i * item->type->superType()->byteSize;
			item->value.push_back(readObject(i, buf, size, item->type, offset_tmp, listItem, listType, item));
		}
	}

	printf("---- \n");

	return item->value;
}





/*-------------------------------------------------------------------------------\
|                             readPacked			                             |
\-------------------------------------------------------------------------------*/
uint32_t Havok::readPacked(const uint8_t *buf, size_t size, size_t &nbBytes)				//apparently the 3 first left bits is for definied the size (in bytes) of the value:
{
	uint32_t value = buf[0];
	nbBytes = 1;

	if ((value & 0x80) == 0)				//uint8
		return value;

	if ((value & 0x40) == 0)				//uint16
	{
		nbBytes = 2;
		return (((value << 8) | (buf[1])) & 0x3fff);
	}

	if ((value & 0x20) == 0)				//uint24
	{
		nbBytes = 3;
		return (((value << 16) | (buf[1] << 8) | (buf[2])) & 0x1fffff);
	}

	nbBytes = 4;						//uint32
	return (((value << 24) | (buf[1] << 16) | (buf[2] << 8) | (buf[3])) & 0x1fffffff);
}
/*-------------------------------------------------------------------------------\
|                             writePacked			                             |
\-------------------------------------------------------------------------------*/
void Havok::writePacked(std::vector<byte> &listBytesTNam, uint32_t value)				//apparently the 3 first left bits is for definied the size (in bytes) of the value:
{
	if (value > 0x1fffffff)
		value = value & 0x1fffffff;

	if (value < 0x80)						//need a uin8
	{
		listBytesTNam.push_back(value);
		return;
	}

	if (value < 0x4000)						//need a uin16
	{
		listBytesTNam.push_back(((value >> 8) & 0xFF) | 0x80);
		listBytesTNam.push_back(value & 0xFF);
		return;
	}

	if (value < 0x200000)					//need a uin24
	{
		listBytesTNam.push_back(((value >> 16) & 0xFF) | 0x80 | 0x40);
		listBytesTNam.push_back((value >> 8) & 0xFF);
		listBytesTNam.push_back(value & 0xFF);
		return;
	}

	//need a uin32
	listBytesTNam.push_back(((value >> 24) & 0xFF) | 0x80 | 0x40 | 0x20);
	listBytesTNam.push_back((value >> 16) & 0xFF);
	listBytesTNam.push_back((value >> 8) & 0xFF);
	listBytesTNam.push_back(value & 0xFF);
}
/*-------------------------------------------------------------------------------\
|                             readFormat			                             |
\-------------------------------------------------------------------------------*/
long long Havok::readFormat(const uint8_t *buf, size_t size, uint32_t flags, string &type_str, bool bigEndian, bool isSigned)
{
	if (flags & TST_IsSigned)
		isSigned = true;

	if (flags & TST_Int8)
	{
		type_str = (isSigned ? "i" : "u") + string("8");
		return ((isSigned) ? (*((int8_t*)buf)) : (*((uint8_t*)buf)));

	}
	else if (flags & TST_Int16) {
		type_str = (isSigned ? "i" : "u") + string("16");

		uint16_t value = (isSigned) ? (*((int16_t*)buf)) : (*((uint16_t*)buf));
		return (bigEndian) ? BE16(value) : value;

	}
	else if (flags & TST_Int32) {
		type_str = (isSigned ? "i" : "u") + string("32");
		uint32_t value = (isSigned) ? (*((int32_t*)buf)) : (*((uint32_t*)buf));
		return (bigEndian) ? BE32(value) : value;

	}
	else if (flags & TST_Int64) {
		type_str = (isSigned ? "i" : "u") + string("64");
		uint64_t value = (isSigned) ? (*((int64_t*)buf)) : (*((uint64_t*)buf));
		return (bigEndian) ? BE64(value) : value;
	}
	return 0;
}
/*-------------------------------------------------------------------------------\
|                             writeFormat			                             |
\-------------------------------------------------------------------------------*/
void Havok::writeFormat(long long value, std::vector<byte> &listBytesData, uint32_t flags, bool isSigned)
{
	if (flags & TST_IsSigned)
		isSigned = true;

	if (flags & TST_Int8)
	{
		listBytesData.push_back((isSigned) ? (*((int8_t*)&value)) : (*((uint8_t*)&value)));

	}
	else if (flags & TST_Int16) {
		uint16_t value_tmp = (isSigned) ? (*((int16_t*)&value)) : (*((uint16_t*)&value));

		listBytesData.push_back(value_tmp & 0xFF);
		listBytesData.push_back((value_tmp >> 8) & 0xFF);

	}
	else if (flags & TST_Int32) {
		uint32_t value_tmp = (isSigned) ? (*((int32_t*)&value)) : (*((uint32_t*)&value));

		listBytesData.push_back(value_tmp & 0xFF);
		listBytesData.push_back((value_tmp >> 8) & 0xFF);
		listBytesData.push_back((value_tmp >> 16) & 0xFF);
		listBytesData.push_back((value_tmp >> 24) & 0xFF);

	}
	else if (flags & TST_Int64) {
		uint64_t value_tmp = (isSigned) ? (*((int64_t*)&value)) : (*((uint64_t*)&value));

		listBytesData.push_back(value_tmp & 0xFF);
		listBytesData.push_back((value_tmp >> 8) & 0xFF);
		listBytesData.push_back((value_tmp >> 16) & 0xFF);
		listBytesData.push_back((value_tmp >> 24) & 0xFF);
		listBytesData.push_back((value_tmp >> 32) & 0xFF);
		listBytesData.push_back((value_tmp >> 40) & 0xFF);
		listBytesData.push_back((value_tmp >> 48) & 0xFF);
		listBytesData.push_back((value_tmp >> 56) & 0xFF);
	}
}





#ifdef LIBXENOVERSE_FBX_SUPPORT


/*-------------------------------------------------------------------------------\
|                             exportFBX											 |
\-------------------------------------------------------------------------------*/
void Havok::exportFBX(FbxScene *scene)
{
	if (!rootObject)
		return;
	
	rootObject->exportFBX("Havok", scene);
}
/*-------------------------------------------------------------------------------\
|                             exportFBX											 |
\-------------------------------------------------------------------------------*/
void Havok_TagObject::exportFBX(string basename, FbxScene *scene, FbxNode* parentNode)
{
	FbxNode *node = FbxNode::Create(scene, basename.c_str());
	node->LclTranslation.Set(FbxVector4(0, 0, 0));
	node->LclRotation.Set(FbxVector4(0, 0, 0, 1));
	node->LclScaling.Set(FbxVector4(1, 1, 1));

	if(!parentNode)
		scene->GetRootNode()->AddChild(node);
	else
		parentNode->AddChild(node);
	
	parentNode = node;
	

	Havok_TagType* supertype = type->superType();
	

	if ((supertype->subType() != Havok::TST_Pointer) &&
		(supertype->subType() != Havok::TST_Class) &&
		(supertype->subType() != Havok::TST_Array) &&
		(supertype->subType() != Havok::TST_Tuple)						//todo check if Tuple could have interressting informations. else remove this
		)
	{
		return;
	}
	

	
	string fbxName = "";
	FbxDouble3 color = FbxDouble3(0.0, 0.0, 0.0);
	std::vector<FbxVector4> listVertexPosition;
	std::vector<size_t> listFaceIndex;
	bool stopRecursive = false;


	if ((type->name == "hkGeometry") && (listObjectClass.size() == 2))
	{
		fbxName += "end;" + type->name;
		color = FbxDouble3(0.0, 0.0, 1.0);


		std::vector<Havok_TagObject*> &listVertex = listObjectClass.at(0)->listObjectArray;
		size_t nbVertex = listVertex.size();
		for (size_t i = 0; i < nbVertex; i++)
		{
			Havok_TagObject* vertexObj = listVertex.at(i);

			std::vector<Havok_TagObject*> &channels = vertexObj->listObjectTuple;
			listVertexPosition.push_back(FbxVector4(channels.at(0)->f_value, channels.at(1)->f_value, channels.at(2)->f_value));
		}
		
		std::vector<Havok_TagObject*> &listTriangle = listObjectClass.at(1)->listObjectArray;
		size_t nbTriangle = listTriangle.size();
		for (size_t i = 0; i < nbTriangle; i++)
		{
			Havok_TagObject* triangleObj = listTriangle.at(i);

			std::vector<Havok_TagObject*> &index = triangleObj->listObjectClass;
			listFaceIndex.push_back((size_t)index.at(0)->i_value);
			listFaceIndex.push_back((size_t)index.at(1)->i_value);
			listFaceIndex.push_back((size_t)index.at(2)->i_value);
			//todo see for Material by face ?
		}
		
		stopRecursive = true;




	}else if ((type->name == "hkAabb") && (listObjectClass.size() == 2)){
		fbxName += "end;"+ type->name;
		color = FbxDouble3(0.0, 1.0, 0.0);


		std::vector<Havok_TagObject*> &minObj = listObjectClass.at(0)->listObjectTuple;
		std::vector<Havok_TagObject*> &maxObj = listObjectClass.at(1)->listObjectTuple;
		FbxVector4 minXYZ(minObj.at(0)->f_value, minObj.at(1)->f_value, minObj.at(2)->f_value);
		FbxVector4 maxXYZ(maxObj.at(0)->f_value, maxObj.at(1)->f_value, maxObj.at(2)->f_value);

		//to symbilize the AABB (AxixAlignedBoundingBox) area, we will draw a Cube:
		listVertexPosition.push_back(FbxVector4(minXYZ[0], minXYZ[1], minXYZ[2]));
		listVertexPosition.push_back(FbxVector4(maxXYZ[0], minXYZ[1], minXYZ[2]));
		listVertexPosition.push_back(FbxVector4(maxXYZ[0], minXYZ[1], maxXYZ[2]));
		listVertexPosition.push_back(FbxVector4(minXYZ[0], minXYZ[1], maxXYZ[2]));

		listVertexPosition.push_back(FbxVector4(minXYZ[0], maxXYZ[1], minXYZ[2]));
		listVertexPosition.push_back(FbxVector4(maxXYZ[0], maxXYZ[1], minXYZ[2]));
		listVertexPosition.push_back(FbxVector4(maxXYZ[0], maxXYZ[1], maxXYZ[2]));
		listVertexPosition.push_back(FbxVector4(minXYZ[0], maxXYZ[1], maxXYZ[2]));

		listFaceIndex.push_back(0); listFaceIndex.push_back(1); listFaceIndex.push_back(2);
		listFaceIndex.push_back(0); listFaceIndex.push_back(2); listFaceIndex.push_back(3);

		listFaceIndex.push_back(0); listFaceIndex.push_back(1); listFaceIndex.push_back(5);
		listFaceIndex.push_back(0); listFaceIndex.push_back(5); listFaceIndex.push_back(4);

		listFaceIndex.push_back(1); listFaceIndex.push_back(2); listFaceIndex.push_back(6);
		listFaceIndex.push_back(1); listFaceIndex.push_back(6); listFaceIndex.push_back(5);

		listFaceIndex.push_back(2); listFaceIndex.push_back(3); listFaceIndex.push_back(7);
		listFaceIndex.push_back(2); listFaceIndex.push_back(7); listFaceIndex.push_back(6);

		listFaceIndex.push_back(3); listFaceIndex.push_back(0); listFaceIndex.push_back(4);
		listFaceIndex.push_back(3); listFaceIndex.push_back(4); listFaceIndex.push_back(7);

		listFaceIndex.push_back(4); listFaceIndex.push_back(5); listFaceIndex.push_back(6);
		listFaceIndex.push_back(4); listFaceIndex.push_back(6); listFaceIndex.push_back(7);

		stopRecursive = true;



	}else if ((type->name == "hkcdSimdTree::Node") && (listObjectClass.size() == 7)) {
		fbxName += "end;" + type->name;
		color = FbxDouble3(1.0, 0.0, 0.0);


		std::vector<Havok_TagObject*> &lxObj = listObjectClass.at(0)->listObjectTuple;
		std::vector<Havok_TagObject*> &hxObj = listObjectClass.at(1)->listObjectTuple;
		std::vector<Havok_TagObject*> &lyObj = listObjectClass.at(2)->listObjectTuple;
		std::vector<Havok_TagObject*> &hyObj = listObjectClass.at(3)->listObjectTuple;
		std::vector<Havok_TagObject*> &lzObj = listObjectClass.at(4)->listObjectTuple;
		std::vector<Havok_TagObject*> &hzObj = listObjectClass.at(5)->listObjectTuple;


		size_t offset = 0;
		for (size_t i = 0; i < 4; i++)						//seam to have 4 x (low + hih + data)
		{
			FbxVector4 minXYZ(lxObj.at(i)->f_value, lyObj.at(i)->f_value, lzObj.at(i)->f_value);
			FbxVector4 maxXYZ(hxObj.at(i)->f_value, hyObj.at(i)->f_value, hzObj.at(i)->f_value);

			for (size_t j = 0; j < 3; j++)					//take care of infinite, to work with blender
			{
				if (minXYZ[i] > 100000.0)
					minXYZ[i] = 100000.0;
				if (minXYZ[i] < -100000.0)
					minXYZ[i] = -100000.0;

				if (maxXYZ[i] > 100000.0)
					maxXYZ[i] = 100000.0;
				if (maxXYZ[i] < -100000.0)
					maxXYZ[i] = -100000.0;
			}

			//to symbilize the AABB (AxixAlignedBoundingBox) area, we will draw a Cube:
			listVertexPosition.push_back(FbxVector4(minXYZ[0], minXYZ[1], minXYZ[2]));
			listVertexPosition.push_back(FbxVector4(maxXYZ[0], minXYZ[1], minXYZ[2]));
			listVertexPosition.push_back(FbxVector4(maxXYZ[0], minXYZ[1], maxXYZ[2]));
			listVertexPosition.push_back(FbxVector4(minXYZ[0], minXYZ[1], maxXYZ[2]));
			listVertexPosition.push_back(FbxVector4(minXYZ[0], maxXYZ[1], minXYZ[2]));
			listVertexPosition.push_back(FbxVector4(maxXYZ[0], maxXYZ[1], minXYZ[2]));
			listVertexPosition.push_back(FbxVector4(maxXYZ[0], maxXYZ[1], maxXYZ[2]));
			listVertexPosition.push_back(FbxVector4(minXYZ[0], maxXYZ[1], maxXYZ[2]));

			offset = 8 * i;
			listFaceIndex.push_back(offset + 0); listFaceIndex.push_back(offset + 1); listFaceIndex.push_back(offset + 2);
			listFaceIndex.push_back(offset + 0); listFaceIndex.push_back(offset + 2); listFaceIndex.push_back(offset + 3);
			listFaceIndex.push_back(offset + 0); listFaceIndex.push_back(offset + 1); listFaceIndex.push_back(offset + 5);
			listFaceIndex.push_back(offset + 0); listFaceIndex.push_back(offset + 5); listFaceIndex.push_back(offset + 4);
			listFaceIndex.push_back(offset + 1); listFaceIndex.push_back(offset + 2); listFaceIndex.push_back(offset + 6);
			listFaceIndex.push_back(offset + 1); listFaceIndex.push_back(offset + 6); listFaceIndex.push_back(offset + 5);
			listFaceIndex.push_back(offset + 2); listFaceIndex.push_back(offset + 3); listFaceIndex.push_back(offset + 7);
			listFaceIndex.push_back(offset + 2); listFaceIndex.push_back(offset + 7); listFaceIndex.push_back(offset + 6);
			listFaceIndex.push_back(offset + 3); listFaceIndex.push_back(offset + 0); listFaceIndex.push_back(offset + 4);
			listFaceIndex.push_back(offset + 3); listFaceIndex.push_back(offset + 4); listFaceIndex.push_back(offset + 7);
			listFaceIndex.push_back(offset + 4); listFaceIndex.push_back(offset + 5); listFaceIndex.push_back(offset + 6);
			listFaceIndex.push_back(offset + 4); listFaceIndex.push_back(offset + 6); listFaceIndex.push_back(offset + 7);
		}

		stopRecursive = true;
	}


	

	//todo see others informations to edit with blender/3dsmax like area or boundingbox, and change Color
	//Todo "hknpConvexPolytopeShape" : 
	// https://en.wikipedia.org/wiki/Convex_polytope
	// https://en.wikipedia.org/wiki/Polytope
	// https://www.csun.edu/~ctoth/Handbook/chap15.pdf
	// https://wiki.blender.org/index.php/Doc:FR/2.4/Tutorials/Game_Engine/BSoD/Moving_the_cube_using_physics





	//build Fbx mesh
	if (listVertexPosition.size())
	{
		FbxNode *node = FbxNode::Create(scene, fbxName.c_str());
		node->LclTranslation.Set(FbxVector4(0, 0, 0));
		node->LclRotation.Set(FbxVector4(0, 0, 0));
		node->LclScaling.Set(FbxVector4(1, 1, 1));
		parentNode->AddChild(node);


		FbxMesh* fbxMesh = FbxMesh::Create(scene, fbxName.c_str());
		if (!fbxMesh)
			return;
		node->SetNodeAttribute(fbxMesh);					//attach mesh to node.



		// Materials part
		FbxGeometryElementMaterial* lMaterialElement = fbxMesh->CreateElementMaterial();			//add a material use.
		lMaterialElement->SetMappingMode(FbxGeometryElement::eByPolygon);
		lMaterialElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);
		
		FbxSurfacePhong* fbxMaterial_phong = FbxSurfacePhong::Create(scene, FbxString(fbxName.c_str()).Buffer());
		fbxMaterial_phong->ShadingModel.Set(FbxString("Phong"));
		fbxMaterial_phong->Emissive.Set(color);							//to differ from classic Emd/emo
		fbxMaterial_phong->Ambient.Set(color);
		fbxMaterial_phong->AmbientFactor.Set(1.0);
		fbxMaterial_phong->Diffuse.Set(color);
		fbxMaterial_phong->DiffuseFactor.Set(1.0);
		fbxMaterial_phong->Shininess.Set(0.0);
		fbxMaterial_phong->Specular.Set(color);
		fbxMaterial_phong->SpecularFactor.Set(0.0);
		node->AddMaterial(fbxMaterial_phong);




		fbxMesh->InitControlPoints(listVertexPosition.size());		//resize by the numbers of Vertex.

		FbxVector4* fbxControlPointsPosition = fbxMesh->GetControlPoints();						//pointer of array , for position only

		//Export Vertices.
		size_t nbVertex = listVertexPosition.size();
		for (size_t i = 0; i < nbVertex; i++)
			fbxControlPointsPosition[i] = listVertexPosition.at(i);


		//Export Faces/triangle/IndexBuffer.
		{
			size_t nbFaces = listFaceIndex.size();
			for (size_t j = 0; j + 2 < nbFaces; j += 3)
			{
				if ((listFaceIndex.at(j) >= nbVertex) || (listFaceIndex.at(j + 1) >= nbVertex) || (listFaceIndex.at(j + 2) >= nbVertex))
					continue;

				//make polygone
				fbxMesh->BeginPolygon(0);

				fbxMesh->AddPolygon(listFaceIndex.at(j));
				fbxMesh->AddPolygon(listFaceIndex.at(j + 1));
				fbxMesh->AddPolygon(listFaceIndex.at(j + 2));

				fbxMesh->EndPolygon();
			}
		}
	}








	if (stopRecursive)
		return;





	// recursive on children
	if ((supertype->subType() == Havok::TST_Pointer) && (objectPointer))
	{
		objectPointer->exportFBX("p", scene, parentNode);


	}else if (supertype->subType() == Havok::TST_Class) {

		std::vector<Havok_TagMember*> listMembers = type->allMembers();
		size_t nbMember = listMembers.size();

		size_t nbObj = listObjectClass.size();
		for (size_t i = 0; i < nbObj; i++)
			listObjectClass.at(i)->exportFBX(((i <= nbMember) ? listMembers.at(i)->name : std::to_string(i)), scene, parentNode);

	}else if (supertype->subType() == Havok::TST_Array) {

		size_t nbObj = listObjectArray.size();
		for (size_t i = 0; i < nbObj; i++)
			listObjectArray.at(i)->exportFBX(std::to_string(i), scene, parentNode);

	}else if (supertype->subType() == Havok::TST_Tuple) {

		size_t nbObj = listObjectTuple.size();
		for (size_t i = 0; i < nbObj; i++)
			listObjectTuple.at(i)->exportFBX(std::to_string(i), scene, parentNode);
	}
}










/*-------------------------------------------------------------------------------\
|                             importFBX											 |
\-------------------------------------------------------------------------------*/
bool Havok::importFBX(FbxNode *fbxNode)
{
	if ((!fbxNode) || (!fbxNode->GetMesh()) || (!rootObject))
		return false;
	name = ToString(fbxNode->GetName());


	std::vector<string> sv = split(name, ';');
	if (sv.size() != 2)
		return false;
	
	string typeName = sv.at(1);
	
	string path = sv.at(0);
	FbxNode* node = fbxNode->GetParent();
	while (node)
	{
		path = ToString(node->GetName()) +"," + path;
		node = node->GetParent();
	}

	sv = split(path, ',');						//list tag to get hierarchy's path;

	if ((sv.size() <2) || (sv.at(0) != "RootNode") || (sv.at(1) != "Havok"))
		return false;

	sv.erase(sv.begin());
	sv.erase(sv.begin());

	bool isSuccess = rootObject->importFBX(fbxNode, sv, typeName);

	if (!isSuccess)
	{
		printf("Warning: fbx node '%s' fail to be used in havok file", name.c_str());
		LibXenoverse::notifyWarning();
	}

	return isSuccess;
}

/*-------------------------------------------------------------------------------\
|                             importFBX											 |
\-------------------------------------------------------------------------------*/
bool Havok_TagObject::importFBX(FbxNode *fbxNode, std::vector<string> &xmlPath, string typeName)
{
	if (xmlPath.size() == 0)
		return false;
	
	Havok_TagType* supertype = type->superType();

	if ((supertype->subType() != Havok::TST_Pointer) &&
		(supertype->subType() != Havok::TST_Class) &&
		(supertype->subType() != Havok::TST_Array) &&
		(supertype->subType() != Havok::TST_Tuple)						//todo check if Tuple could have interressting informations. else remove this
		)
	{
		return false;
	}





	if ((xmlPath.size()==1)&&(type->name == typeName))					//normally, we just have the last xmlPath to match.
	{

		/////////////////// first we have to take the vertex and faces index from fbx
		std::vector<FbxVector4> listVertexPosition;
		std::vector<size_t> listFaceIndex;


		FbxMesh* fbxMesh = fbxNode->GetMesh();
		int control_points_count = fbxMesh->GetControlPointsCount();				//number of vertices.
		FbxAMatrix transform_matrix = fbxNode->EvaluateGlobalTransform();
		
		
		FbxVector4 nodeScale = fbxNode->LclScaling.Get();
		if (nodeScale[0] == 0.0) nodeScale[0] = 1.0f;
		if (nodeScale[1] == 0.0) nodeScale[1] = 1.0f;
		if (nodeScale[2] == 0.0) nodeScale[2] = 1.0f;
		

		int material_count = fbxNode->GetMaterialCount();
		for (int m = 0; m < material_count; m++)
		{
			size_t offset = listVertexPosition.size();

			
			FbxAMatrix transform_matrix = fbxNode->EvaluateGlobalTransform();
			FbxAMatrix rotation_matrix = transform_matrix;
			rotation_matrix.SetT(FbxVector4(0.0, 0.0, 0.0, 0.0));
			rotation_matrix.SetS(FbxVector4(1.0, 1.0, 1.0, 1.0));

			FbxVector4* listPositions = fbxMesh->GetControlPoints();
			size_t nbVertex = fbxMesh->GetControlPointsCount();

			for (size_t i = 0; i < nbVertex; i++)
			{
				listVertexPosition.push_back(transform_matrix.MultT(listPositions[i]));
				listVertexPosition.back()[0] /= nodeScale[0];				// because of apply the hierarchy. may be we have to consider positoin rotation, and also in absolute.
				listVertexPosition.back()[1] /= nodeScale[1];
				listVertexPosition.back()[2] /= nodeScale[2];
			}



			size_t lPolygonCount = fbxMesh->GetPolygonCount();
			for (size_t i = 0; i < lPolygonCount; ++i)
			{
				if (fbxMesh->GetPolygonSize(i) != 3)
					continue;

				for (int j = 0; j < 3; j++)
				{
					size_t index_tmp = fbxMesh->GetPolygonVertex(i, j);
					if (index_tmp >= nbVertex)
					{
						printf("error: vertex index out of list. use 0.\n");
						LibXenoverse::notifyError();
						index_tmp = 0;
					}
					listFaceIndex.push_back(index_tmp + offset);						//fill faces
				}
			}
		}
		
		


		
		/////////////////// second part try to match the different case.


		if ((type->name == "hkGeometry") && (listObjectClass.size() == 2))
		{
			size_t nbNewVertex = listVertexPosition.size();
			size_t nbNewFaces = listFaceIndex.size();
			size_t nbNewTriangle = nbNewFaces / 3;

			if ((nbNewVertex == 0) || (nbNewFaces == 0) || (nbNewTriangle == 0))
			{
				printf("Error hkGeometry : number of vertices or faces into fbx is 0. skipped.\n");
				LibXenoverse::notifyError();
				return false;
			}
			

			std::vector<Havok_TagObject*> &listVertex = listObjectClass.at(0)->listObjectArray;
			std::vector<Havok_TagObject*> &listTriangle = listObjectClass.at(1)->listObjectArray;
			size_t nbVertex = listVertex.size();
			size_t nbTriangle = listTriangle.size();
			if ((nbVertex == 0)||(nbTriangle==0))
			{
				printf("Error hkGeometry : We need minimum one vertice and one triangle in havok as a model for new values.");
				LibXenoverse::notifyError();
				return false;
			}

			Havok_TagObject* model_vertexObj = listVertex.at(0);
			Havok_TagObject* model_triangleObj = listTriangle.at(0);

			
			

			if (nbVertex > nbNewVertex)
			{
				for (size_t i = nbVertex - 1; i >= nbNewVertex; i--)
				{
					if (listVertex.at(i)->attachement)
						listVertex.at(i)->attachement->remove(listVertex.at(i));
					delete listVertex.at(i);
					listVertex.erase(listVertex.begin() + i);
				}
			}else {
				for (size_t i = nbVertex; i < nbNewVertex; i++)
				{
					listVertex.push_back(model_vertexObj->clone());
					if (model_vertexObj->attachement)
						model_vertexObj->attachement->value.push_back(listVertex.back());
				}
			}

			
			if (nbTriangle > nbNewTriangle)
			{
				for (size_t i = nbTriangle - 1; i >= nbNewTriangle; i--)
				{
					if (listTriangle.at(i)->attachement)
						listTriangle.at(i)->attachement->remove(listTriangle.at(i));
					delete listTriangle.at(i);
					listTriangle.erase(listTriangle.begin() + i);
				}
			}
			else {
				for (size_t i = nbTriangle; i < nbNewTriangle; i++)
				{
					listTriangle.push_back(model_triangleObj->clone());
					if (model_triangleObj->attachement)
						model_triangleObj->attachement->value.push_back(listTriangle.back());
				}
			}





			for (size_t i = 0; i < nbVertex; i++)
			{
				Havok_TagObject* vertexObj = listVertex.at(i);
				std::vector<Havok_TagObject*> &channels = vertexObj->listObjectTuple;

				channels.at(0)->f_value = (float)listVertexPosition.at(i)[0];
				channels.at(1)->f_value = (float)listVertexPosition.at(i)[1];
				channels.at(2)->f_value = (float)listVertexPosition.at(i)[2];
			}
		
			for (size_t i = 0; i < nbTriangle; i++)
			{
				Havok_TagObject* triangleObj = listTriangle.at(i);

				std::vector<Havok_TagObject*> &index = triangleObj->listObjectClass;
				index.at(0)->i_value = (long long)listFaceIndex.at(i * 3 + 0);
				index.at(1)->i_value = (long long)listFaceIndex.at(i * 3 + 1);
				index.at(2)->i_value = (long long)listFaceIndex.at(i * 3 + 2);

				//todo see for Material by face ?
			}





		}else if ((type->name == "hkAabb") && (listObjectClass.size() == 2)){

			
			size_t nbNewVertex = listVertexPosition.size();
			if (nbNewVertex != 8)
			{
				printf("Error hkAabb: number of vertices must be 8. skipped.\n");
				LibXenoverse::notifyError();
				return false;
			}


			std::vector<Havok_TagObject*> &minObj = listObjectClass.at(0)->listObjectTuple;
			std::vector<Havok_TagObject*> &maxObj = listObjectClass.at(1)->listObjectTuple;
			
			minObj.at(0)->f_value = (float)listVertexPosition.at(0)[0];
			minObj.at(1)->f_value = (float)listVertexPosition.at(0)[1];
			minObj.at(2)->f_value = (float)listVertexPosition.at(0)[2];
			
			maxObj.at(0)->f_value = (float)listVertexPosition.at(6)[0];
			maxObj.at(1)->f_value = (float)listVertexPosition.at(6)[1];
			maxObj.at(2)->f_value = (float)listVertexPosition.at(6)[2];



		}else if ((type->name == "hkcdSimdTree::Node") && (listObjectClass.size() == 7)) {
			

			size_t nbNewVertex = listVertexPosition.size();
			if (nbNewVertex != 4 * 8)
			{
				printf("Error hkAabb: number of vertices must be 4 * 8. skipped.\n");
				LibXenoverse::notifyError();
				return false;
			}


			for (size_t i = 0; i < nbNewVertex; i++)							//take care of infinite
			{
				for (size_t j = 0; j < 3; j++)
				{
					if (listVertexPosition.at(i)[j] >= 100000.0)
						listVertexPosition.at(i)[j] = 3.40282002e+038;
					if (listVertexPosition.at(i)[j] <= -100000.0)
						listVertexPosition.at(i)[j] = -3.40282002e+038;
				}
			}


			std::vector<Havok_TagObject*> &lxObj = listObjectClass.at(0)->listObjectTuple;
			std::vector<Havok_TagObject*> &hxObj = listObjectClass.at(1)->listObjectTuple;
			std::vector<Havok_TagObject*> &lyObj = listObjectClass.at(2)->listObjectTuple;
			std::vector<Havok_TagObject*> &hyObj = listObjectClass.at(3)->listObjectTuple;
			std::vector<Havok_TagObject*> &lzObj = listObjectClass.at(4)->listObjectTuple;
			std::vector<Havok_TagObject*> &hzObj = listObjectClass.at(5)->listObjectTuple;


			size_t offset = 0;
			for (size_t i = 0; i < 4; i++)						//seam to have 4 x (low + hih + data)
			{
				lxObj.at(i)->f_value = (float)listVertexPosition.at(i * 8 + 0)[0];
				lyObj.at(i)->f_value = (float)listVertexPosition.at(i * 8 + 0)[1];
				lzObj.at(i)->f_value = (float)listVertexPosition.at(i * 8 + 0)[2];

				hxObj.at(i)->f_value = (float)listVertexPosition.at(i * 8 + 6)[0];
				hyObj.at(i)->f_value = (float)listVertexPosition.at(i * 8 + 6)[1];
				hzObj.at(i)->f_value = (float)listVertexPosition.at(i * 8 + 6)[2];
			}
		}



		//todo see others informations to edit with blender/3dsmax like area or boundingbox, and change Color
		//Todo "hknpConvexPolytopeShape" :
		// https://en.wikipedia.org/wiki/Convex_polytope
		// https://en.wikipedia.org/wiki/Polytope
		// https://www.csun.edu/~ctoth/Handbook/chap15.pdf
		// https://wiki.blender.org/index.php/Doc:FR/2.4/Tutorials/Game_Engine/BSoD/Moving_the_cube_using_physics

		return true;
	}





	// recursive on children

	if ((supertype->subType() == Havok::TST_Pointer) && (objectPointer) && (xmlPath.at(0)=="p"))
	{
		xmlPath.erase(xmlPath.begin());
		return objectPointer->importFBX(fbxNode, xmlPath, typeName);

	}else if (supertype->subType() == Havok::TST_Class) {

		std::vector<Havok_TagMember*> listMembers = type->allMembers();
		size_t nbMember = listMembers.size();

		string name_tmp;
		size_t nbObj = listObjectClass.size();
		for (size_t i = 0; i < nbObj; i++)
		{
			name_tmp = (i <= nbMember) ? listMembers.at(i)->name : std::to_string(i);
			if (xmlPath.at(0) == name_tmp)
			{
				xmlPath.erase(xmlPath.begin());
				return listObjectClass.at(i)->importFBX(fbxNode, xmlPath, typeName);
			}
		}

	}else if (supertype->subType() == Havok::TST_Array) {

		size_t nbObj = listObjectArray.size();
		for (size_t i = 0; i < nbObj; i++)
		{
			if (xmlPath.at(0) == std::to_string(i))
			{
				xmlPath.erase(xmlPath.begin());
				return listObjectArray.at(i)->importFBX(fbxNode, xmlPath, typeName);
			}
		}

	}else if (supertype->subType() == Havok::TST_Tuple) {

		size_t nbObj = listObjectTuple.size();
		for (size_t i = 0; i < nbObj; i++)
		{
			if (xmlPath.at(0) == std::to_string(i))
			{
				xmlPath.erase(xmlPath.begin());
				return listObjectTuple.at(i)->importFBX(fbxNode, xmlPath, typeName);
			}
		}
	}

	return false;
}


#endif


}