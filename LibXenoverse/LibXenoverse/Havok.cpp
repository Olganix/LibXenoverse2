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
		printf("error: havok's version request : %s\n", Havok_TAG0_SIGNATURE);
		notifyError();
		return false;
	}
	offset += sizeof(Havok_PartHeader);


	Havok_PartHeader* version_hdr = (Havok_PartHeader*)GetOffsetPtr(buf, offset, true);
	size_t version_hdr_size = version_hdr->size;  version_hdr_size = val32(version_hdr_size) & 0x3FFFFFFF;
	if (memcmp(version_hdr->signature, Havok_SDKV_SIGNATURE, 4) != 0)
	{
		printf("error: havok's version request : %s\n", Havok_SDKV_SIGNATURE);
		notifyError();
		return false;
	}
	offset += sizeof(Havok_PartHeader);


	Havok_Version* version_info = (Havok_Version*)GetOffsetPtr(buf, offset, true);
	if ((memcmp(version_info->year, Havok_v2015_SIGNATURE, 4) != 0) || (memcmp(version_info->major, Havok_major01_SIGNATURE, 2) != 0) || (memcmp(version_info->minor, Havok_minor00_SIGNATURE, 2) != 0))
	{
		printf("error: havok's version request : %s.%s.%s\n", Havok_v2015_SIGNATURE, Havok_major01_SIGNATURE, Havok_minor00_SIGNATURE);
		notifyError();
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
		printf("%s: Memory allocation error (0x%x)\n", FUNCNAME, size);
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
		printf("unknow superType->subType() : %s\n", EMO_BaseFile::UnsignedToString(supertype->subType(), true).c_str());
		notifyError();
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
		/*
		//second test for version "hk_2014.1.0-r1"
		rootNode = EMO_BaseFile::FindRoot(&handle, "hktagfile");
		if (rootNode)
		{
			string str = "";
			rootNode->QueryStringAttribute("sdkversion", &str);
			if(str == "hk_2014.1.0-r1")
				return import_Xml_v2014_01_00_r1(rootNode);
		}
		*/

		printf("Cannot find\"Havok\" in xml.\n");
		notifyError();
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
		notifyError();
		return false;
	}
	TiXmlElement* node_Data = rootNode->FirstChildElement("Data");
	if (!node_Data)
	{
		printf("Cannot find\"Data\" in xml.\n");
		notifyError();
		return false;
	}
	TiXmlElement* node_Item = rootNode->FirstChildElement("ListItem");
	if (!node_Item)
	{
		printf("Cannot find\"ListItem\" in xml.\n");
		notifyError();
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
		listType.at(inc++)->importXml_secondkeyframe(node, listType);






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
|                             importXml_secondkeyframe			                     |
\-------------------------------------------------------------------------------*/
void Havok_TagType::importXml_secondkeyframe(TiXmlElement* node, std::vector<Havok_TagType*> &listType)
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
		printf("unknow superType->subType() : %s\n", EMO_BaseFile::UnsignedToString(supertype->subType(), true).c_str());
		notifyError();
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
|                             import_Xml_v2014_01_00_r1                          |
\-------------------------------------------------------------------------------*
bool Havok::import_Xml_v2014_01_00_r1(TiXmlElement* rootNode)
{
	Reset();


	//////////////////////////////  read Types Xml part to make Havok_TagType, Havok_TagMember, ...
	TiXmlElement* node_Types = rootNode->FirstChildElement("class");
	if (!node_Types)
	{
		printf("Cannot find\"class\" in xml.\n");
		notifyError();
		return false;
	}
	TiXmlElement* node_Data = rootNode->FirstChildElement("object");
	if (!node_Data)
	{
		printf("Cannot find\"object\" in xml.\n");
		notifyError();
		return false;
	}
	
	string str;
	string str_type;

	listType.push_back(new Havok_TagType(0, "None"));
	std::vector<string> listMemberTypeNames;

	for (TiXmlElement* node = rootNode->FirstChildElement("class"); node; node = node->NextSiblingElement("class"))
	{
		Havok_TagType* type = new Havok_TagType(listType.size());
		listType.push_back(type);
		
		str = "";
		node->QueryStringAttribute("name", &str);
		type->name = str;

		if (node->Attribute("version"))
		{
			type->flags = type->flags | TagFlag::TF_Version;
			node->QueryUnsignedAttribute("version", &type->version);
		}

		type->subTypeFlags = type->subTypeFlags | TagSubType::TST_Class;


		//a first preparation
		for (TiXmlElement* node_member = node->FirstChildElement("member"); node_member; node_member = node_member->NextSiblingElement("member"))
		{
			type->flags = type->flags | TagFlag::TF_Members;

			str = "";
			node_member->QueryStringAttribute("type", &str);

			bool isfound = false;
			for (size_t i = 0; i < listMemberTypeNames.size(); i++)
			{
				if (listMemberTypeNames.at(i)==str)
				{
					isfound = true;
					break;
				}
			}
			if (!isfound)
				listMemberTypeNames.push_back(str);
		}

	}

	//second time to have parent
	size_t nbTypes = listType.size();
	size_t inc = 1;
	for (TiXmlElement* node = rootNode->FirstChildElement("class"); node; node = node->NextSiblingElement("class"))
	{
		Havok_TagType* type = listType.at(inc++);

		if (node->Attribute("parent"))
		{
			str = "";
			node->QueryStringAttribute("parent", &str);
			if (str == type->name)
				continue;

			type->parent = searchForType(str);
		}
	}



	//before take care of members for build array pointer, ... types, we wil add common types that could simplify/remove some search of type after.
	size_t nbElements = listMemberTypeNames.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		str = listMemberTypeNames.at(i);

		

		if (str == "int")
		{

			if (!searchForType("int"))						//because could be added by another, because of heritage
				listType.push_back(new Havok_TagType(listType.size(), "int", 0, TF_ByteSize | TF_SubType, 33284, 0, 0, 4, 4));

		}else if ((str == "real") || (str == "hkReal") || (str == "float")) {

			Havok_TagType* type = searchForType("float");
			if (!type)
			{
				type = new Havok_TagType(listType.size(), "float", 0, TF_ByteSize | TF_SubType, 1525253, 0, 0, 4, 4);
				listType.push_back(type);
			}
			if (!searchForType("hkReal"))
				listType.push_back(new Havok_TagType(listType.size(), "hkReal", type, 0, 0, 0, 0, 0, 0));

		}else if ((str == "byte") || (str == "unsigned int")) {

			if (!searchForType("unsigned int"))
				listType.push_back(new Havok_TagType(listType.size(), "unsigned int", 0, TF_ByteSize | TF_SubType, 32772, 0, 0, 4, 4));

		}else if ((str == "vec4") || (str == "hkVector4")) {

			Havok_TagType* type = searchForType("float");
			if (!type)
			{
				type = new Havok_TagType(listType.size(), "float", 0, TF_ByteSize | TF_SubType, 1525253, 0, 0, 4, 4);
				listType.push_back(type);
			}
			Havok_TagType* type_b = searchForType("hkVector4f");
			if (!type_b)
			{
				type_b = new Havok_TagType(listType.size(), "hkVector4f", 0, TF_ByteSize | TF_Pointer | TF_SubType, 1064, type, 0, 16, 16);
				listType.push_back(type_b);
			}
			Havok_TagType* type_c = searchForType("hkVector4");
			if (!type_c)
			{
				type_c = new Havok_TagType(listType.size(), "hkVector4", type_b, 0, 0, 0, 0, 0, 0);
				listType.push_back(type_c);
			}
			type_b->pointer = type_c;


		}else if (str == "string") {

			Havok_TagType* type = searchForType("char");
			if (!type)
			{
				type = new Havok_TagType(listType.size(), "char", 0, TF_ByteSize | TF_SubType, 8196, 0, 0, 1, 1);
				listType.push_back(type);
			}
			if (!searchForType("string"))
				listType.push_back(new Havok_TagType(listType.size(), "string", 0, TF_ByteSize | TF_SubType, TST_String, type, 0, 4, 4));



		}else if (str == "vec12") {


			Havok_TagType* type = searchForType("float");
			if (!type)
			{
				type = new Havok_TagType(listType.size(), "float", 0, TF_ByteSize | TF_SubType, 1525253, 0, 0, 4, 4);
				listType.push_back(type);
			}
			if (!searchForType("vec12"))
				listType.push_back(new Havok_TagType(listType.size(), "vec12", 0, TF_ByteSize | TF_SubType, TST_String, type, 0, 48, 16));

		}else if (str == "vec16") {

			Havok_TagType* type = searchForType("float");
			if (!type)
			{
				type = new Havok_TagType(listType.size(), "float", 0, TF_ByteSize | TF_SubType, 1525253, 0, 0, 4, 4);
				listType.push_back(type);
			}
			if (!searchForType("vec16"))
				listType.push_back(new Havok_TagType(listType.size(), "vec16", 0, TF_ByteSize | TF_SubType, TST_String, type, 0, 64, 16));
		}
	}
		
	// add the commmon from version 2015
	if (!searchForType("int"))
		listType.push_back(new Havok_TagType(listType.size(), "int", 0, TF_ByteSize | TF_SubType, 33284, 0, 0, 4, 4));
	if (!searchForType("unsigned int"))						//because could be added by another, because of heritage
		listType.push_back(new Havok_TagType(listType.size(), "unsigned int", 0, TF_ByteSize | TF_SubType, 32772, 0, 0, 4, 4));
	if (!searchForType("hkUint32"))						//because could be added by another, because of heritage
		listType.push_back(new Havok_TagType(listType.size(), "hkUint32", 0, 0, 0, searchForType("unsigned int"), 0, 0, 0));
	if (!searchForType("char"))
		listType.push_back(new Havok_TagType(listType.size(), "char", 0, TF_ByteSize | TF_SubType, 8196, 0, 0, 1, 1));
	if (!searchForType("unsigned char"))
		listType.push_back(new Havok_TagType(listType.size(), "unsigned char", 0, TF_ByteSize | TF_SubType, 8196, 0, 0, 1, 1));
	if (!searchForType("hkUint8"))
		listType.push_back(new Havok_TagType(listType.size(), "hkUint8", searchForType("unsigned char"), 0, 0, 0, 0, 0, 0));

	if (!searchForType("unsigned long long"))
		listType.push_back(new Havok_TagType(listType.size(), "unsigned long long", 0, TF_ByteSize | TF_SubType, 65540, 0, 0, 8, 4));
	if (!searchForType("hkUint64"))
		listType.push_back(new Havok_TagType(listType.size(), "hkUint64", searchForType("unsigned long long"), 0, 0, 0, 0, 0, 0));

	if (!searchForType("unsigned short"))
		listType.push_back(new Havok_TagType(listType.size(), "unsigned short", 0, TF_ByteSize | TF_SubType, 16388, 0, 0, 2, 2));
	if (!searchForType("hkUint16"))
		listType.push_back(new Havok_TagType(listType.size(), "hkUint16", searchForType("unsigned short"), 0, 0, 0, 0, 0, 0));

		
	


	//now we have type class and common types, we can take care of memebers, and also add type array, pointer, tuple, ....
	inc = 1;
	for (TiXmlElement* node = rootNode->FirstChildElement("class"); node; node = node->NextSiblingElement("class"))
	{
		Havok_TagType* type = listType.at(inc++);

		for (TiXmlElement* node_member = node->FirstChildElement("member"); node_member; node_member = node_member->NextSiblingElement("member"))
		{
			type->flags = type->flags | TagFlag::TF_Members;

			Havok_TagMember* member = new Havok_TagMember();
			type->members.push_back(member);

			str = "";
			node_member->QueryStringAttribute("name", &str);
			member->name = str;


			str_type = "";
			bool isArray = false;
			size_t tupleCount = 0;

			node_member->QueryStringAttribute("type", &str_type);
			node_member->QueryBoolAttribute("array", &isArray);
			if (!isArray)
				node_member->QueryUnsignedAttribute("count", &tupleCount);

			if( (str_type == "void")|| (str_type == "None"))
			{
				member->flags = member->flags | TagSubType::TST_Void;
				member->type = listType.at(0);							//None.

			}else if (str_type == "ref"){
				member->flags = member->flags | TagSubType::TST_Pointer;

				str = "";
				node_member->QueryStringAttribute("class", &str);

				Havok_TagType* targetType = searchForType(str);
				if (targetType)
				{
					//we must create a type witch is referenced as a pointer for the target class/type
					member->type = searchForType("T*", targetType);						//first, we search if it's doesn't allready exist.

					if (!member->type)													//else create it.
					{
						Havok_TagType* type_tmp = new Havok_TagType(listType.size(), "T*", 0, TF_ByteSize | TF_Pointer |TF_SubType, 6, targetType, 0, 4, 4);
						listType.push_back(type_tmp);
						member->type = type_tmp;

						type_tmp->listTemplate.push_back(new Havok_TagTemplate("tT", targetType->id));
						type_tmp->listTemplate.back()->value_ptr = targetType;
					}
				}

			}else if (str_type == "string") {
				member->flags = member->flags | TagSubType::TST_String;

				//a string it's a item, with a unknow number of element unsigned char
				member->type = searchForType("string");
				if (!member->type)
				{
					Havok_TagType* targetType = searchForType("unsigned char");							//but there is subtype to search
					member->type = new Havok_TagType(listType.size(), "string", 0, 0, TST_Array, targetType, 0, 0, 1);
					listType.push_back(member->type);
				}

			}else if (str_type == "struct") {
				member->flags = member->flags | TagSubType::TST_Class;

				str = "";
				node_member->QueryStringAttribute("class", &str);

				member->type = searchForType(str);
			}else{

				if (str_type == "real")
					str_type = "hkReal";
				else if (str_type == "byte")
					str_type = "unsigned int";
				else if (str_type == "vec4")
					str_type = "hkVector4";

				member->type = searchForType(str_type);
				member->flags = member->flags | ((member->type) ? member->type->superType()->subTypeFlags : TagSubType::TST_Invalid);
			}



			if (str_type == "isArray")
			{
				member->flags = member->flags | TagSubType::TST_Array;


				Havok_TagType* targetType = member->type;
				if (targetType)
				{
					//we must create a type witch is referenced as a pointer for the target class/type
					member->type = searchForType("hkArray", targetType);						//first, we search if it's doesn't allready exist.

					if (!member->type)													//else create it.
					{
						Havok_TagType* type_tmp = new Havok_TagType(listType.size(), "hkArray", 0, TF_ByteSize | TF_Pointer | TF_SubType | TF_Members, 8, targetType, 0, 12, 4);
						listType.push_back(type_tmp);
						member->type = type_tmp;

						member->type->listTemplate.push_back(new Havok_TagTemplate("tT", targetType->id));
						member->type->listTemplate.back()->value_ptr = targetType;
						member->type->listTemplate.push_back(new Havok_TagTemplate("tAllocator", 31));
						member->type->listTemplate.back()->value_ptr = targetType;



						Havok_TagType* type_tmp_b = new Havok_TagType(listType.size(), "T*", 0, TF_ByteSize | TF_Pointer | TF_SubType, 6, targetType, 0, 4, 4);
						listType.push_back(type_tmp);
						type_tmp->listTemplate.push_back(new Havok_TagTemplate("tT", targetType->id));
						type_tmp->listTemplate.back()->value_ptr = targetType;
						Havok_TagMember* member_tmp = new Havok_TagMember("m_data", 34, 0, type_tmp_b);
						member->type->members.push_back(member_tmp);

						type_tmp_b = searchForType("int");
						member_tmp = new Havok_TagMember("m_size", 34, 4, type_tmp_b);
						member->type->members.push_back(member_tmp);
						member_tmp = new Havok_TagMember("m_capacityAndFlags", 34, 8, type_tmp_b);
						member->type->members.push_back(member_tmp);
					}
				}
			}

			if (tupleCount != 0)
			{
				member->flags = member->flags | TagSubType::TST_Tuple;


				Havok_TagType* targetType = member->type;
				if (targetType)
				{
					member->type = searchForType("T[N]", targetType);

					if (!member->type)													//else create it.
					{
						//strange sometime there is 1064 (=0x428, for hkUint32) instead of 808 (=0x328, for hkUint8), but there isn't 0x100 or 0x400, and also 0x200 is for IsSigned , witch is not the case of hkUint8.
						Havok_TagType* type_tmp = new Havok_TagType(listType.size(), "T[N]", 0, TF_ByteSize | TF_Pointer | TF_SubType, 808, targetType, 0, targetType->byteSize * tupleCount, targetType->alignment);
						listType.push_back(type_tmp);
						member->type = type_tmp;

						member->type->listTemplate.push_back(new Havok_TagTemplate("tT", targetType->id));
						member->type->listTemplate.back()->value_ptr = targetType;
						member->type->listTemplate.push_back(new Havok_TagTemplate("vN", tupleCount));
					}
				}
			}



		}
	}


	//look at for byteSize and offsets for class, and also get some alignement from 2015 version.
	for (size_t i = 1; i < nbTypes + 1; i++)									//only the class
		calculateByteSizeAndAlignmentOfClass(listType.at(i));




	
	//////////////////////////////  read Data Xml part to make Havok_TagObject, and fill items
	listItem.push_back(new Havok_TagItem(0, listType.at(0)));				//None

	std::vector<Havok_TagObject*> linkObj;
	std::vector<size_t> linkIndex;

	std::vector<Havok_TagObject*> listFirstObject;
	Havok_TagType* lastType = 0;
	inc = 1;
	for (TiXmlElement* node = rootNode->FirstChildElement("object"); node; node = node->NextSiblingElement("object"))
	{
		str = "";
		node->QueryStringAttribute("name", &str);
		
		Havok_TagType* type = searchForType(str);

		if (type != lastType)
			listItem.push_back(new Havok_TagItem(0, type));


		Havok_TagObject* obj = objectImportXml_v2014_01_00_r1(node, linkObj, linkIndex, listItem.back());
		listItem.back()->value.push_back(obj);
		listItem.back()->count++;
		obj->attachement = listItem.back();

		if (!rootObject)
			rootObject = obj;

		listFirstObject.push_back(obj);
	}
	

	//linkObj, linkIndex for pointers (designed by a Index)
	size_t nbLink = linkObj.size();
	for (size_t i = 0; i < nbLink; i++)
		linkObj.at(i)->objectPointer = listFirstObject.at(linkIndex.at(i));

	return (rootObject != 0);
}
/*-------------------------------------------------------------------------------\
|                             objectImportXml_v2014_01_00_r1                     |
\-------------------------------------------------------------------------------*
Havok_TagObject* Havok::objectImportXml_v2014_01_00_r1(TiXmlElement* node, std::vector<Havok_TagObject*> &linkObj, std::vector<size_t> &linkIndex, Havok_TagItem* parentAttachement)
{
	string str = "";
	node->QueryStringAttribute("type", &str);


	Havok_TagType* type = searchForType(str);
	if (!type)
	{
		printf("warning: type %s not found. it's not normal. skipped\n", str.c_str());
		notifyError();
		return 0;
	}
	Havok_TagType* supertype = type->superType();


	Havok_TagObject* obj = new Havok_TagObject();
	obj->attachement = parentAttachement;
	obj->type = type;


	bool isPtr = false;
	bool isParentPtr = false;




	if (supertype->subType() == Havok::TST_Bool)
	{
		obj->b_value = (string(node->GetText()) == "true");

	}else if (supertype->subType() == Havok::TST_Int) {

		obj->i_value = (supertype->subTypeFlags & Havok::TST_Int64) ? EMO_BaseFile::GetUnsigned64(node->GetText(), 0) : EMO_BaseFile::GetUnsigned(node->GetText(), 0);

	}else if (supertype->subType() == Havok::TST_Float) {

		obj->f_value = StringToFloat(node->GetText());

	}else if (supertype->subType() == Havok::TST_String) {
		
		obj->s_value = string(node->GetText());

		size_t nbCharac = obj->s_value.length();
		for (size_t i = 0;i<nbCharac;i++)
		{
			Havok_TagObject* obj_tmp = new Havok_TagObject(searchForType("char"));
			obj_tmp->i_value = obj->s_value.at(i);

			obj->listObjectString.push_back(obj_tmp);
		}

		Havok_TagObject* obj_tmp = new Havok_TagObject(searchForType("char"));				//"\0"				//Todo search if it's necessary , but I didn't have concret example.
		obj_tmp->i_value = 0;
		obj->listObjectString.push_back(obj_tmp);


	}else if (supertype->subType() == Havok::TST_Pointer) {

		linkObj.push_back(obj);															// pointer could referenced a Object witch don't exist yet. So we have to do this after.
		linkIndex.push_back( std::stoi(string(node->GetText()).substr(1)) );

	}else if (supertype->subType() == Havok::TST_Class) {

		for (TiXmlElement* node_tmp = node->FirstChildElement(); node_tmp; node_tmp = node_tmp->NextSiblingElement())
			obj->listObjectClass.push_back(objectImportXml_v2014_01_00_r1(node_tmp, linkObj, linkIndex, parentAttachement));

		isParentPtr = true;

	}else if (supertype->subType() == Havok::TST_Array) {

		for (TiXmlElement* node_tmp = node->FirstChildElement(); node_tmp; node_tmp = node_tmp->NextSiblingElement())
			obj->listObjectArray.push_back(objectImportXml_v2014_01_00_r1(node_tmp, linkObj, linkIndex, parentAttachement));

	}else if (supertype->subType() == Havok::TST_Tuple) {

		for (TiXmlElement* node_tmp = node->FirstChildElement(); node_tmp; node_tmp = node_tmp->NextSiblingElement())
			obj->listObjectTuple.push_back(objectImportXml_v2014_01_00_r1(node_tmp, linkObj, linkIndex, parentAttachement));

	}else {
		printf("unknow superType->subType() : %s\n", EMO_BaseFile::UnsignedToString(supertype->subType(), true).c_str());
		notifyError();
	}


	if ((obj->attachement) && (obj->attachement != parentAttachement))
		obj->attachement->isPtr = isPtr;


	return obj;
}
/*-------------------------------------------------------------------------------\
|                             searchForType				                          |
\-------------------------------------------------------------------------------*/
Havok_TagType* Havok::searchForType(string name, Havok_TagType* pointer)
{
	size_t nbTypes = listType.size();
	for (size_t i = 0; i < nbTypes; i++)
	{
		if (listType.at(i)->name == name)
		{
			if ((pointer) && (listType.at(i)->pointer != pointer))
				continue;

			return listType.at(i);
		}
	}
	return 0;
}
/*-------------------------------------------------------------------------------\
|                             calculateByteSizeAndAlignmentOfClass               |
\-------------------------------------------------------------------------------*
void Havok::calculateByteSizeAndAlignmentOfClass(Havok_TagType* type)
{
	if ((!type) || ((type->subTypeFlags & TST_Class) == 0) || (type->byteSize != 0))
		return;

	std::vector<Havok_TagMember*> members = type->allMembers();

	size_t offset = 0;
	size_t nbMembers = members.size();
	for (size_t i = 0; i < nbMembers; i++)
	{
		Havok_TagMember* member = members.at(i);
		member->byteOffset = offset;

		calculateByteSizeAndAlignmentOfClass(member->type);
		offset += member->type->byteSize;

		if (member->type->alignment > type->alignment)
			type->alignment = member->type->alignment;
	}

	type->byteSize = offset;
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
	//if (!exportFBX_CheckHave3dData())							//check if object or child have something knowed. is to avoid useless Node creation. => apparently it's break ingame
	//	return;

	
	
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


	static std::vector<FbxVector4> listVertexPosition_Geometry;
	static std::vector<size_t> listFaceIndex_Geometry;


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
		
		listVertexPosition_Geometry = listVertexPosition;				//to use them after on aabb
		listFaceIndex_Geometry = listFaceIndex;

		stopRecursive = true;

		/*
	}else if ((type->name == "hkcdStaticTree::Tree") && (listObjectClass.size() == 2)) {
		
		
		
		fbxName += "end;" + type->name;
		color = FbxDouble3(0.0, 1.0, 0.0);


		std::vector<Havok_TagObject*> &nodes = listObjectClass.at(0)->listObjectArray;
		Havok_TagObject* domain = listObjectClass.at(1);

		std::vector<Havok_TagObject*> &minObj = domain->listObjectClass.at(0)->listObjectTuple;
		std::vector<Havok_TagObject*> &maxObj = domain->listObjectClass.at(1)->listObjectTuple;
		FbxVector4 minXYZ(minObj.at(0)->f_value, minObj.at(1)->f_value, minObj.at(2)->f_value);
		FbxVector4 maxXYZ(maxObj.at(0)->f_value, maxObj.at(1)->f_value, maxObj.at(2)->f_value);

		printf("aabb: [%f, %f]x[%f, %f]x[%f, %f] \n", minXYZ[0], maxXYZ[0], minXYZ[1], maxXYZ[1], minXYZ[2], maxXYZ[2]);

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


		{
			FbxNode *node = FbxNode::Create(scene, (fbxName + "_AABB").c_str());
			node->LclTranslation.Set(FbxVector4(0, 0, 0));
			node->LclRotation.Set(FbxVector4(0, 0, 0));
			node->LclScaling.Set(FbxVector4(1, 1, 1));
			parentNode->AddChild(node);


			FbxMesh* fbxMesh = FbxMesh::Create(scene, (fbxName + "_AABB").c_str());
			if (!fbxMesh)
				return;
			node->SetNodeAttribute(fbxMesh);					//attach mesh to node.



																// Materials part
			FbxGeometryElementMaterial* lMaterialElement = fbxMesh->CreateElementMaterial();			//add a material use.
			lMaterialElement->SetMappingMode(FbxGeometryElement::eByPolygon);
			lMaterialElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

			FbxSurfacePhong* fbxMaterial_phong = FbxSurfacePhong::Create(scene, FbxString((fbxName + "_AABB").c_str()).Buffer());
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
		listFaceIndex.clear();
		listVertexPosition.clear();


		//now the inside Nodes.
		size_t nbNodes = nodes.size();
		size_t value_tmp = 0;
		size_t value_tmp_b = 0;
		double percentMin = 0.0;
		double percentMax = 0.0;
		size_t unk = 0;
		size_t face = 0;
		FbxVector4 minXYZ_tmp(0, 0, 0, 0);
		FbxVector4 maxXYZ_tmp(0, 0, 0, 0);

		for (size_t i = 0; i < nbNodes; i++)
		{
			std::vector<Havok_TagObject*> &codec3Axis6 = nodes.at(i)->listObjectClass;						//min and max are coded in a single uint8
			std::vector<Havok_TagObject*> &minMaxXYZObj = codec3Axis6.at(0)->listObjectTuple;

			printf("aabb_subpart[%i]: ", i);

			for (size_t j = 0; j < 3; j++)
			{
				value_tmp = (size_t)minMaxXYZObj.at(j)->i_value;
				printf("[%x]x", value_tmp);
			}

			unk = (size_t)codec3Axis6.at(1)->i_value;
			face = (size_t)codec3Axis6.at(2)->i_value;

			printf(" => %d for face %d \n", unk, face);
		}


		for (size_t i = 0; i < nbNodes; i++)
		{
			std::vector<Havok_TagObject*> &codec3Axis6 = nodes.at(i)->listObjectClass;						//min and max are coded in a single uint8
			std::vector<Havok_TagObject*> &minMaxXYZObj = codec3Axis6.at(0)->listObjectTuple;

			for (size_t j = 0; j < 3; j++)
			{
				value_tmp = (size_t)minMaxXYZObj.at(j)->i_value;
				

				//AabbTree : http://allenchou.net/2014/02/game-physics-broadphase-dynamic-aabb-tree/
				//TODO find encoding of values. and after if there is a relation with Simdtree::Node
				//simdTree : https://github.com/tpn/pdfs/blob/master/Binary%20Search%20Tree%20with%20SIMD%20Bandwidth%20Optimizations%20Using%20SSE%20(Preso06-SIMDTree).pdf


				*//*
				if (value_tmp == 0)					//test Todo a virer.
					value_tmp = 255;

				minXYZ_tmp[j] = - ((((double)(value_tmp & 0xF)) / (double(0xF))) * (maxXYZ[j] - minXYZ[j])/2.0 ) + (maxXYZ[j] + minXYZ[j]) / 2.0;
				maxXYZ_tmp[j] = ((((double)( (value_tmp >> 4) & 0xF)) / (double(0xF))) * (maxXYZ[j] - minXYZ[j]) / 2.0) + (maxXYZ[j] + minXYZ[j]) / 2.0;
				*//*


				if ((j == 1) && (value_tmp != 0))
					int aa = 42;

				value_tmp_b = (value_tmp >> 4) & 0xF;
				size_t inc = 0;
				while (value_tmp_b > ((size_t)1 << inc))
				{
					value_tmp_b -= 1 << inc;
					inc++;
				}
				percentMin = (((double)(value_tmp_b)) / ((double)((size_t)1 << inc))) ;


				value_tmp_b = value_tmp & 0xF;
				inc = 0;
				while (value_tmp_b > ((size_t)1 << inc))
				{
					value_tmp_b -= 1 << inc;
					inc++;
				}
				percentMax = (((double)(value_tmp_b + 1))/((double)((size_t)1 << inc)));

				minXYZ_tmp[j] = (percentMin) * (maxXYZ[j] - minXYZ[j]);
				maxXYZ_tmp[j] = (percentMax) * (maxXYZ[j] - minXYZ[j]);

				
				if (j != 1)				//test to solve Y first.
				{
					minXYZ_tmp[j] = -30;
					maxXYZ_tmp[j] = 30;
				}
			}

			unk = (size_t)codec3Axis6.at(1)->i_value;
			face = (size_t)codec3Axis6.at(2)->i_value;

			printf("aabb_subpart[%i]: [%f, %f]x[%f, %f]x[%f, %f] => %d for face %d \n", i, minXYZ_tmp[0], maxXYZ_tmp[0], minXYZ_tmp[1], maxXYZ_tmp[1], minXYZ_tmp[2], maxXYZ_tmp[2], unk, face);

			//make the subpart
			color = (unk!=128) ? FbxDouble3(1.0, 1.0, 0.0) : FbxDouble3(1.0, 0.0, 0.0);

			{
				//to symbilize the AABB (AxixAlignedBoundingBox) area, we will draw a Cube:
				listVertexPosition.push_back(FbxVector4(minXYZ_tmp[0], minXYZ_tmp[1], minXYZ_tmp[2]));
				listVertexPosition.push_back(FbxVector4(maxXYZ_tmp[0], minXYZ_tmp[1], minXYZ_tmp[2]));
				listVertexPosition.push_back(FbxVector4(maxXYZ_tmp[0], minXYZ_tmp[1], maxXYZ_tmp[2]));
				listVertexPosition.push_back(FbxVector4(minXYZ_tmp[0], minXYZ_tmp[1], maxXYZ_tmp[2]));
				listVertexPosition.push_back(FbxVector4(minXYZ_tmp[0], maxXYZ_tmp[1], minXYZ_tmp[2]));
				listVertexPosition.push_back(FbxVector4(maxXYZ_tmp[0], maxXYZ_tmp[1], minXYZ_tmp[2]));
				listVertexPosition.push_back(FbxVector4(maxXYZ_tmp[0], maxXYZ_tmp[1], maxXYZ_tmp[2]));
				listVertexPosition.push_back(FbxVector4(minXYZ_tmp[0], maxXYZ_tmp[1], maxXYZ_tmp[2]));
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



				//add the face concerned
				if (face * 3 + 2 < listFaceIndex_Geometry.size())
				{
					listVertexPosition.push_back(listVertexPosition_Geometry.at(listFaceIndex_Geometry.at(face * 3 + 0)));
					listVertexPosition.push_back(listVertexPosition_Geometry.at(listFaceIndex_Geometry.at(face * 3 + 1)));
					listVertexPosition.push_back(listVertexPosition_Geometry.at(listFaceIndex_Geometry.at(face * 3 + 2)));

					listFaceIndex.push_back(8 + 0); listFaceIndex.push_back(8 + 1); listFaceIndex.push_back(8 + 2);
				}
			}

			
			{
				FbxNode *node = FbxNode::Create(scene, (fbxName + "_AABB__"+ fillStringNumberLeft(std::to_string(i), 3) ).c_str());
				node->SetVisibility(false);
				node->LclTranslation.Set(FbxVector4(0, 0, 0));
				node->LclRotation.Set(FbxVector4(0, 0, 0));
				node->LclScaling.Set(FbxVector4(1, 1, 1));
				parentNode->AddChild(node);


				FbxMesh* fbxMesh = FbxMesh::Create(scene, (fbxName + "_AABB__" + fillStringNumberLeft(std::to_string(i), 3) ).c_str());
				if (!fbxMesh)
					return;
				node->SetNodeAttribute(fbxMesh);					//attach mesh to node.



																	// Materials part
				FbxGeometryElementMaterial* lMaterialElement = fbxMesh->CreateElementMaterial();			//add a material use.
				lMaterialElement->SetMappingMode(FbxGeometryElement::eByPolygon);
				lMaterialElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

				FbxSurfacePhong* fbxMaterial_phong = FbxSurfacePhong::Create(scene, FbxString((fbxName + "_AABB__" + fillStringNumberLeft(std::to_string(i), 3) ).c_str()).Buffer());
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

			listFaceIndex.clear();
			listVertexPosition.clear();
		}




		stopRecursive = true;

		*/
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
|                             exportFBX_CheckHave3dData							 |
\-------------------------------------------------------------------------------*/
bool Havok_TagObject::exportFBX_CheckHave3dData()
{
	Havok_TagType* supertype = type->superType();

	if ((supertype->subType() != Havok::TST_Pointer) &&
		(supertype->subType() != Havok::TST_Class) &&
		(supertype->subType() != Havok::TST_Array) &&
		(supertype->subType() != Havok::TST_Tuple)						//todo check if Tuple could have interressting informations. else remove this
		)
	{
		return false;
	}


	if ((type->name == "hkGeometry") && (listObjectClass.size() == 2))
	{
		return true;
	}else if ((type->name == "hkcdStaticTree::Tree") && (listObjectClass.size() == 2)) {
		return true;

		/*
	}else if ((type->name == "hkAabb") && (listObjectClass.size() == 2)){
		return true;
		*/
	}

	bool haveChild_WithData = false;

	// recursive on children
	if ((supertype->subType() == Havok::TST_Pointer) && (objectPointer))
	{
		haveChild_WithData = objectPointer->exportFBX_CheckHave3dData();

	}else if (supertype->subType() == Havok::TST_Class) {

		std::vector<Havok_TagMember*> listMembers = type->allMembers();
		size_t nbMember = listMembers.size();

		size_t nbObj = listObjectClass.size();
		for (size_t i = 0; i < nbObj; i++)
			haveChild_WithData = haveChild_WithData || listObjectClass.at(i)->exportFBX_CheckHave3dData();

	}else if (supertype->subType() == Havok::TST_Array) {

		size_t nbObj = listObjectArray.size();
		for (size_t i = 0; i < nbObj; i++)
			haveChild_WithData = haveChild_WithData || listObjectArray.at(i)->exportFBX_CheckHave3dData();

	}else if (supertype->subType() == Havok::TST_Tuple) {

		size_t nbObj = listObjectTuple.size();
		for (size_t i = 0; i < nbObj; i++)
			haveChild_WithData = haveChild_WithData || listObjectTuple.at(i)->exportFBX_CheckHave3dData();
	}

	return haveChild_WithData;
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
		printf("Warning: fbx node '%s' fail to be used in havok file\n", name.c_str());
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
				printf("Error hkGeometry : We need minimum one vertice and one triangle in havok as a model for new values.\n");
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











































/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





void Havok::simplifyHavokXml(string filename, string filenameOut)
{

	TiXmlDocument doc_src;

	if (!doc_src.LoadFile(filename))
	{

		if (doc_src.ErrorId() == TiXmlBase::TIXML_ERROR_OPENING_FILE)
			printf("Cannot open file \"%s\"\n", filename.c_str());
		else
			printf("Error parsing file \"%s\". This is what tinyxml has to say: %s. Row=%d, col=%d.\n", filename.c_str(), doc_src.ErrorDesc(), doc_src.ErrorRow(), doc_src.ErrorCol());
		LibXenoverse::notifyError();
		return;
	}
	TiXmlHandle handle_src(&doc_src);
	TiXmlElement* rootNode_src = handle_src.FirstChildElement("Havok").Element();

	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);



	EMD* emd = new EMD();
	emd->setName(filename + ".emd");

	EMDModel* emdModel = new EMDModel();
	emd->getModel().push_back(emdModel);
	emdModel->setName("Havok");


	size_t nbVertices = 0, nbTriangles = 0, nbStaticNodes = 0, nbAABB = 0;
	TiXmlElement* rootNode = simplifyHavokXml_Node(rootNode_src, nbVertices, nbTriangles, nbStaticNodes, nbAABB, emdModel);
	if (rootNode)
	{
		doc.LinkEndChild(rootNode);
		doc.SaveFile(filenameOut);

		emd->save(filenameOut + ".emd");

		{
			bool exportAscii = false;
			string export_filename = filenameOut + ".fbx";

			// Create FBX Manager
			FbxManager *sdk_manager = FbxManager::Create();
			FbxIOSettings *ios = FbxIOSettings::Create(sdk_manager, IOSROOT);
			ios->SetBoolProp(EXP_FBX_EMBEDDED, true);
			if (exportAscii)
				ios->SetBoolProp(EXP_ASCIIFBX, true);										//test to have debug on fbx. Todo comment
			sdk_manager->SetIOSettings(ios);

			// Create Scene
			FbxScene *scene = FbxScene::Create(sdk_manager, "EMDFBXScene");
			scene->GetGlobalSettings().SetCustomFrameRate(60.0);				//specify the frameRate, the number of image by second. (not working well in blender fbx importer. so, I had to modify default value into blender fbx importer).
			FbxTime::EProtocol protocol = FbxTime::GetGlobalTimeProtocol();
			FbxTime::SetGlobalTimeProtocol(FbxTime::EProtocol::eDefaultProtocol);
			protocol = FbxTime::GetGlobalTimeProtocol();
			scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::eOpenGL);	//eOpenGL,			!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded
			scene->GetGlobalSettings().SetSystemUnit(FbxSystemUnit::m);
			FbxNode *lRootNode = scene->GetRootNode();




			std::vector<ESK::FbxBonesInstance_DBxv> global_fbx_bones;
			std::vector<EMB*> mListEmb;
			emd->exportFBX(scene, global_fbx_bones, mListEmb, 0, true);




			int lFileFormat = sdk_manager->GetIOPluginRegistry()->GetNativeWriterFormat();
			FbxExporter* lExporter = FbxExporter::Create(sdk_manager, "");
			bool lExportStatus = lExporter->Initialize(export_filename.c_str(), lFileFormat, sdk_manager->GetIOSettings());
			if (!lExportStatus)
			{
				printf("Call to FbxExporter::Initialize() failed.\n");
				printf("Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
				LibXenoverse::notifyError();
				LibXenoverse::waitOnEnd();
				delete emd;
				return;
			}
			lExporter->Export(scene);												// Export scene
			lExporter->Destroy();


			if (exportAscii)												//test to have the Ascci version, that could help.
			{
				int lFileFormat_Ascii = sdk_manager->GetIOPluginRegistry()->FindWriterIDByDescription("FBX ascii (*.fbx)");

				FbxExporter* lExporter = FbxExporter::Create(sdk_manager, "");
				bool lExportStatus = lExporter->Initialize((export_filename.substr(0, export_filename.length() - 4) + "_Ascii.fbx").c_str(), lFileFormat_Ascii, sdk_manager->GetIOSettings());
				if (lExportStatus)
				{
					lExporter->Export(scene);												// Export scene
					lExporter->Destroy();
				}
			}

		}

	}else {
		printf("Cannot find\"Havok\" in xml.\n");
		LibXenoverse::notifyError();
		return;
	}

	delete emd;
}



TiXmlElement* Havok::simplifyHavokXml_Node(TiXmlElement* currentNode_src, size_t &nbVertices, size_t &nbTriangles, size_t &nbStaticNodes, size_t &nbAABB, EMDModel* emdModel)		//recursive
{
	if (!currentNode_src)
		return 0;


	//parts we try to simplify
	string str = "";
	if ((string(currentNode_src->Value()) == "Object") && (currentNode_src->QueryStringAttribute("Name", &str) == TIXML_SUCCESS))
	{
		if (str == "vertices")
			return simplifyHavokXml_Node_Vertices(currentNode_src, nbVertices, emdModel);

		if (str == "triangles")
			return simplifyHavokXml_Node_Faces(currentNode_src, nbTriangles, emdModel);

		if (str == "aabbTree")
			return simplifyHavokXml_Node_AabbTree(currentNode_src, nbTriangles, nbStaticNodes, emdModel);

		if (str == "simdTree")
			return simplifyHavokXml_Node_SimTree(currentNode_src, nbStaticNodes, nbAABB, emdModel);
	}





	//////////// case only simple copy
	TiXmlElement* currentNode = new TiXmlElement(currentNode_src->Value());

	// Arguments
	for (TiXmlAttribute* node_src = currentNode_src->FirstAttribute(); node_src; node_src = node_src->Next())
		currentNode->SetAttribute(node_src->Name(), node_src->ValueStr());


	// Children
	for (TiXmlElement* node_src = currentNode_src->FirstChildElement(); node_src; node_src = node_src->NextSiblingElement())
	{
		TiXmlElement* node = simplifyHavokXml_Node(node_src, nbVertices, nbTriangles, nbStaticNodes, nbAABB, emdModel);
		currentNode->LinkEndChild(node);
	}

	return currentNode;
}







TiXmlElement* Havok::simplifyHavokXml_Node_Vertices(TiXmlElement* currentNode_src, size_t &nbVertices, EMDModel* emdModel)
{
	if (!currentNode_src)
		return 0;

	TiXmlElement* currentNode = new TiXmlElement("Vertices");


	EMDMesh* emdMesh = new EMDMesh();
	emdModel->getMeshes().push_back(emdMesh);
	emdMesh->setName("Geometry");

	EMDSubmesh* emdSubMesh = new EMDSubmesh();
	emdMesh->getSubmeshes().push_back(emdSubMesh);
	emdSubMesh->setMaterialName("Geometry");
	emdSubMesh->setVertexTypeFlags(EMD_VTX_FLAG_POS | EMD_VTX_FLAG_COLOR );


	nbVertices = 0;
	size_t inc = 0;
	string str = "";
	float x, y, z, w;
	for (TiXmlElement* node_src = currentNode_src->FirstChildElement("Object"); node_src; node_src = node_src->NextSiblingElement("Object"))
	{
		TiXmlElement* node = new TiXmlElement("position");
		currentNode->LinkEndChild(node);
		node->SetAttribute("index", nbVertices++);

		inc = 0;
		for (TiXmlElement* node_src_p = node_src->FirstChildElement("Object"); node_src_p; node_src_p = node_src_p->NextSiblingElement("Object"))
		{
			str = "";
			node_src_p->QueryStringAttribute("value", &str);

			switch (inc)
			{
			case 0:	node->SetAttribute("x", str);	x = StringToFloat(str);  break;
			case 1:	node->SetAttribute("y", str);	y = StringToFloat(str); break;
			case 2:	node->SetAttribute("z", str);	z = StringToFloat(str);  break;
			case 3:	node->SetAttribute("w", str);	w = StringToFloat(str);  break;
			}

			if (++inc >= 4)
				break;
		}

		EMDVertex v;
		v.flags = emdSubMesh->getVertexTypeFlags();
		v.pos_x = x;	v.pos_y = y;	v.pos_z = z;	v.color = 0xFF0000FF;
		emdSubMesh->getVertices().push_back(EMDVertex(&v));
	}
	currentNode->SetAttribute("nbVertices", nbVertices);

	return currentNode;
}


TiXmlElement* Havok::simplifyHavokXml_Node_Faces(TiXmlElement* currentNode_src, size_t &nbTriangles, EMDModel* emdModel)
{
	if (!currentNode_src)
		return 0;

	TiXmlElement* currentNode = new TiXmlElement("TriangleList");
	TiXmlElement* currentNode_Triangles = new TiXmlElement("Triangles");	currentNode->LinkEndChild(currentNode_Triangles);
	TiXmlElement* currentNode_faces = new TiXmlElement("Faces");			currentNode_Triangles->LinkEndChild(currentNode_faces);

	EMDSubmesh* emdSubMesh = emdModel->getMeshes().back()->getSubmeshes().back();
	EMDTriangles triangle;

	nbTriangles = 0;
	size_t inc = 0;
	string str = "";
	for (TiXmlElement* node_src = currentNode_src->FirstChildElement("Object"); node_src; node_src = node_src->NextSiblingElement("Object"))
	{
		TiXmlElement* node = new TiXmlElement("face");
		currentNode_faces->LinkEndChild(node);
		node->SetAttribute("index", nbTriangles++);


		inc = 0;
		for (TiXmlElement* node_src_p = node_src->FirstChildElement("Object"); node_src_p; node_src_p = node_src_p->NextSiblingElement("Object"))
		{
			str = "";
			node_src_p->QueryStringAttribute("value", &str);

			switch (inc)
			{
			case 0:	node->SetAttribute("v1", str);  triangle.faces.push_back(GetUnsigned(str));	break;
			case 1:	node->SetAttribute("v2", str);  triangle.faces.push_back(GetUnsigned(str));  break;
			case 2:	node->SetAttribute("v3", str);  triangle.faces.push_back(GetUnsigned(str));  break;
			}

			if (++inc >= 3)
				break;
		}

	}

	emdSubMesh->getTriangles().push_back(triangle);

	currentNode_faces->SetAttribute("nbTriangles", nbTriangles);

	return currentNode;
}




TiXmlElement* Havok::simplifyHavokXml_Node_AabbTree(TiXmlElement* currentNode_src, size_t nbTriangles, size_t &nbStaticNodes, EMDModel* emdModel)
{
	if (!currentNode_src)
		return 0;

	//Todo on hidata != 0 , follow making hierarchy https://en.wikipedia.org/wiki/AA_tree
	//hyp_0_loData_indexFace

	TiXmlElement* currentNode = new TiXmlElement("hkcdStaticTree");
	TiXmlElement* currentNode_Nodes = new TiXmlElement("Nodes");	currentNode->LinkEndChild(currentNode_Nodes);
	TiXmlElement* currentNode_Aabb = new TiXmlElement("AABB");		currentNode->LinkEndChild(currentNode_Aabb);

	TiXmlElement* currentNode_Nodes_src = currentNode_src->FirstChildElement("Object");
	TiXmlElement* currentNode_Aabb_src = currentNode_Nodes_src->NextSiblingElement("Object");



	/////////////////////////////////////////// AABB

	TiXmlElement* node_src_min = currentNode_Aabb_src->FirstChildElement("Object");
	TiXmlElement* node = new TiXmlElement("Min");
	currentNode_Aabb->LinkEndChild(node);

	size_t inc = 0;
	string str = "";
	float minX, minY, minZ, maxX, maxY, maxZ;
	for (TiXmlElement* node_src_p = node_src_min->FirstChildElement("Object"); node_src_p; node_src_p = node_src_p->NextSiblingElement("Object"))
	{
		str = "";
		node_src_p->QueryStringAttribute("value", &str);

		switch (inc)
		{
		case 0:	node->SetAttribute("x", str);  minX = StringToFloat(str);	break;
		case 1:	node->SetAttribute("y", str);  minY = StringToFloat(str);  break;
		case 2:	node->SetAttribute("z", str);  minZ = StringToFloat(str);  break;
		case 3:	node->SetAttribute("w", str);  break;
		}

		if (++inc >= 4)
			break;
	}


	TiXmlElement* node_src_max = node_src_min->NextSiblingElement("Object");
	node = new TiXmlElement("Max");
	currentNode_Aabb->LinkEndChild(node);

	inc = 0;
	for (TiXmlElement* node_src_p = node_src_max->FirstChildElement("Object"); node_src_p; node_src_p = node_src_p->NextSiblingElement("Object"))
	{
		str = "";
		node_src_p->QueryStringAttribute("value", &str);

		switch (inc)
		{
		case 0:	node->SetAttribute("x", str);  maxX = StringToFloat(str);  break;
		case 1:	node->SetAttribute("y", str);  maxY = StringToFloat(str);  break;
		case 2:	node->SetAttribute("z", str);  maxZ = StringToFloat(str);  break;
		case 3:	node->SetAttribute("w", str);  break;
		}

		if (++inc >= 4)
			break;
	}
	float aabbSizeX = maxX - minX, aabbSizeY = maxY - minY, aabbSizeZ = maxZ - minZ;
	float aabbSizemax = max(max(aabbSizeX, aabbSizeY), aabbSizeZ);

	////////////////////////////////  <Codec3Axis6 x="0"    y="0"   z="0"   hiData="128" loData="2"  /> 


	EMDSubmesh* emdSubMesh_geometry = emdModel->getMeshes().at(0)->getSubmeshes().at(0);
	EMDTriangles &triangle_geometry = emdSubMesh_geometry->getTriangles().back();
	


	bool hyp_0_loData_indexFace = true;
	size_t maxLoData = 0;
	size_t maxLoData_hidataNot0 = 0;

	std::vector<string> listHiData;					//test todo remove
	std::vector<bool> listFaceUsed;					//test todo remove
	for (size_t i = 0; i < nbTriangles; i++)
		listFaceUsed.push_back(false);

	nbStaticNodes = 0;
	inc = 0;
	str = "";
	int x, y, z;
	for (TiXmlElement* node_src = currentNode_Nodes_src->FirstChildElement("Object"); node_src; node_src = node_src->NextSiblingElement("Object"))
	{
		TiXmlElement* node = new TiXmlElement("Codec3Axis6");
		currentNode_Nodes->LinkEndChild(node);
		node->SetAttribute("index", nbStaticNodes++);

		TiXmlElement* node_src_xyz = node_src->FirstChildElement("Object");

		inc = x = y = z = 0;
		for (TiXmlElement* node_src_p = node_src_xyz->FirstChildElement("Object"); node_src_p; node_src_p = node_src_p->NextSiblingElement("Object"))
		{
			str = "";
			node_src_p->QueryStringAttribute("value", &str);

			switch (inc)
			{
			case 0:	node->SetAttribute("x", str);	x = GetUnsigned(str);  break;
			case 1:	node->SetAttribute("y", str);	y = GetUnsigned(str);  break;
			case 2:	node->SetAttribute("z", str);	z = GetUnsigned(str);  break;
			}
			if (++inc >= 3)
				break;
		}
		
		float xf = (float)((x > 127) ? (x - 255) : x) / 127.0f;
		float yf = (float)((y > 127) ? (y - 255) : y) / 127.0f;
		float zf = (float)((z > 127) ? (z - 255) : z) / 127.0f;
		/*
		float xf = (float)(x - 127) / 127.0f;		if (xf > 1.0f) xf = 1.0f; if (xf < -1.0f) xf = -1.0f;
		float yf = (float)(y - 127) / 127.0f;		if (yf > 1.0f) yf = 1.0f; if (yf < -1.0f) yf = -1.0f;
		float zf = (float)(z - 127) / 127.0f;		if (zf > 1.0f) zf = 1.0f; if (zf < -1.0f) zf = -1.0f;
		*/
		node->SetAttribute("xyz", FloatToString(xf) +", "+ FloatToString(yf) +", "+ FloatToString(zf) );			// [0,255] cut in two -> [-127,127] -> [-1.0, 1.0]


		TiXmlElement* node_src_p = node_src_xyz->NextSiblingElement("Object");
		str = "";
		node_src_p->QueryStringAttribute("value", &str);
		//node->SetAttribute("hiData", str);							//no need because there is the two case of loData displayed different.
		size_t hiData = EMO_BaseFile::GetUnsigned(str);

		bool isfound = false;
		for (size_t i = 0, nbHidAta = listHiData.size(); i < nbHidAta; i++)
		{
			if (listHiData.at(i) == str)
			{
				isfound = true;
				break;
			}
		}
		if (!isfound)
			listHiData.push_back(str);

		node_src_p = node_src_p->NextSiblingElement("Object");
		str = "";
		node_src_p->QueryStringAttribute("value", &str);
		node->SetAttribute((hiData == 0) ? "face" : "loData", str);
		size_t loData = EMO_BaseFile::GetUnsigned(str);

		if (hiData == 0)										//it's a faceIndex
		{
			if (loData > maxLoData)
				maxLoData = loData;

			if(loData < nbTriangles)
				listFaceUsed.at(loData) = true;
		}else {													//it's 128, a node tree, may be ?
			if (loData > maxLoData_hidataNot0)
				maxLoData_hidataNot0 = loData;
		}




		//visual debug for emd - fbx
		{
			EMDMesh* emdMesh = new EMDMesh();
			emdModel->getMeshes().push_back(emdMesh);
			emdMesh->setName("codec3Axis6_" + ToString(nbStaticNodes - 1) + "__" + ((hiData == 0) ? "Face_" : "Node_") + ToString(loData));

			EMDSubmesh* emdSubMesh = new EMDSubmesh();
			emdMesh->getSubmeshes().push_back(emdSubMesh);
			//emdSubMesh->setMaterialName("codec3Axis6");
			emdSubMesh->setMaterialName("codec3Axis6_" + ToString(nbStaticNodes - 1) + "__" + ((hiData == 0) ? "Face_" : "Node_") + ToString(loData));
			emdSubMesh->setVertexTypeFlags(EMD_VTX_FLAG_POS | EMD_VTX_FLAG_COLOR);
			EMDTriangles triangle;

			
			EMDVertex v; v.flags = emdSubMesh->getVertexTypeFlags();
			size_t offsetVertex = 0;
			if (hiData == 0)							// first we copy the face
			{
				emdSubMesh->getVertices().push_back(EMDVertex(&emdSubMesh_geometry->getVertices().at(triangle_geometry.faces[loData * 3 + 0]))); emdSubMesh->getVertices().back().color = 0xFFFF00FF;
				emdSubMesh->getVertices().push_back(EMDVertex(&emdSubMesh_geometry->getVertices().at(triangle_geometry.faces[loData * 3 + 1]))); emdSubMesh->getVertices().back().color = 0xFFFF00FF;
				emdSubMesh->getVertices().push_back(EMDVertex(&emdSubMesh_geometry->getVertices().at(triangle_geometry.faces[loData * 3 + 2]))); emdSubMesh->getVertices().back().color = 0xFFFF00FF;
				triangle.faces.push_back(0);
				triangle.faces.push_back(1);
				triangle.faces.push_back(2);
				offsetVertex += 3;
			}
			
			//second, representation of the XYZ
			double length = sqrt(xf * xf + yf * yf + zf * zf);
			double xf_n = ((length != 0) ? (xf / length) : 0), yf_n = ((length!=0) ? (yf / length) : 0), zf_n = ((length!=0) ? (zf / length) : 0);
			
			double start_x = -(1.0 + length * 10.0) * (aabbSizemax / 2.0) * xf_n, start_y = -(1.0 + length * 10.0) * (aabbSizemax / 2.0) * yf_n, start_z = -(1.0 + length * 10.0) * (aabbSizemax / 2.0) * zf_n;
			double end_x = - (aabbSizemax / 2.0) * xf_n, end_y = -(aabbSizemax / 2.0) * yf_n, end_z = -(aabbSizemax / 2.0) * zf_n;	//vector to display , outside of AABB

			double bnx = 0, bny = 1.0, bnz = 0.0;			//binormal
			if ( (abs(xf_n) < 0.01) && (abs(zf_n) < 0.01))
			{
				bny = 0.0;
				if (abs(yf_n) > 0.01)
					bnz = 1.0;
			}

			double nx = bny * zf_n - bnz * yf_n;	// normal = cross(binormal, vctorDir)
			double ny = bnz * xf_n - bnx * zf_n;
			double nz = bnx * yf_n - bny * xf_n;

			double width = 0.1;
			nx *= width, ny *= width, nz *= width;

			v.color = 0x00FF00FF;
			
			v.pos_x = (float)(start_x - nx); v.pos_y = (float)(start_y - ny); v.pos_z = (float)(start_z - nz); emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = (float)(start_x + nx); v.pos_y = (float)(start_y + ny); v.pos_z = (float)(start_z + nz); emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = (float)(end_x + nx); v.pos_y = (float)(end_y + ny); v.pos_z = (float)(end_z + nz); emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = (float)(end_x - nx); v.pos_y = (float)(end_y - ny); v.pos_z = (float)(end_z - nz); emdSubMesh->getVertices().push_back(EMDVertex(&v));

			triangle.faces.push_back(offsetVertex + 0); triangle.faces.push_back(offsetVertex + 1); triangle.faces.push_back(offsetVertex + 2);
			triangle.faces.push_back(offsetVertex + 0); triangle.faces.push_back(offsetVertex + 2); triangle.faces.push_back(offsetVertex + 3);
			triangle.faces.push_back(offsetVertex + 0); triangle.faces.push_back(offsetVertex + 2); triangle.faces.push_back(offsetVertex + 1);
			triangle.faces.push_back(offsetVertex + 0); triangle.faces.push_back(offsetVertex + 3); triangle.faces.push_back(offsetVertex + 2);
			offsetVertex += 4;

			emdSubMesh->getTriangles().push_back(triangle);
		}
	}
	hyp_0_loData_indexFace = (maxLoData + 1 <= nbTriangles);



	currentNode->SetAttribute("nbStaticNodes", nbStaticNodes);
	//currentNode->SetAttribute("maxLoData_hiData0", maxLoData);
	currentNode->SetAttribute("maxLoData_hiDataNot0", maxLoData_hidataNot0);
	//currentNode->SetAttribute("hyp_0_loData_indexFace", hyp_0_loData_indexFace ? "true" : "false");

	/*
	str = "";
	for (size_t i = 0, nbHidAta = listHiData.size(); i < nbHidAta; i++)
		str += listHiData.at(i) + ", ";
	currentNode->SetAttribute("listHiData", str);
	*/

	str = "";
	for (size_t i = 0; i < nbTriangles; i++)
		if(listFaceUsed.at(i)==false)
			str += ToString(listFaceUsed.at(i)) + ", ";
	currentNode->SetAttribute("listFaceNOTUsed", str);




	return currentNode;
}








TiXmlElement* Havok::simplifyHavokXml_Node_SimTree(TiXmlElement* currentNode_src, size_t nbStaticNodes, size_t &nbAABB, EMDModel* emdModel)
{
	if (!currentNode_src)
		return 0;

	TiXmlElement* currentNode = new TiXmlElement("SimdTree");

	TiXmlElement* currentNode_Nodes_src = currentNode_src->FirstChildElement("Object");

	//string name_ad = ""; currentNode_src->QueryStringAttribute("Name", &name_ad);				//todo remove
	//string name_ae = ""; currentNode_Nodes_src->QueryStringAttribute("Name", &name_ae);				//todo remove

	bool hyp_data_isStaticNodeIndex = true;
	size_t maxData = 0;
	nbAABB = 0;
	size_t inc = 0;
	size_t inc_b = 0;
	string str = "";
	for (TiXmlElement* node_src = currentNode_Nodes_src->FirstChildElement("Object"); node_src; node_src = node_src->NextSiblingElement("Object"))
	{
		//string name_aa = ""; node_src->QueryStringAttribute("TypeId", &name_aa);				//todo remove

		TiXmlElement* node = new TiXmlElement("Node");
		currentNode->LinkEndChild(node);

		TiXmlElement* aabb_0 = new TiXmlElement("AABB");	node->LinkEndChild(aabb_0); TiXmlElement* aabb_0_min = new TiXmlElement("Min"); TiXmlElement* aabb_0_max = new TiXmlElement("Max"); aabb_0->LinkEndChild(aabb_0_min); aabb_0->LinkEndChild(aabb_0_max);
		TiXmlElement* aabb_1 = new TiXmlElement("AABB");	node->LinkEndChild(aabb_1); TiXmlElement* aabb_1_min = new TiXmlElement("Min"); TiXmlElement* aabb_1_max = new TiXmlElement("Max"); aabb_1->LinkEndChild(aabb_1_min); aabb_1->LinkEndChild(aabb_1_max);
		TiXmlElement* aabb_2 = new TiXmlElement("AABB");	node->LinkEndChild(aabb_2); TiXmlElement* aabb_2_min = new TiXmlElement("Min"); TiXmlElement* aabb_2_max = new TiXmlElement("Max"); aabb_2->LinkEndChild(aabb_2_min); aabb_2->LinkEndChild(aabb_2_max);
		TiXmlElement* aabb_3 = new TiXmlElement("AABB");	node->LinkEndChild(aabb_3); TiXmlElement* aabb_3_min = new TiXmlElement("Min"); TiXmlElement* aabb_3_max = new TiXmlElement("Max"); aabb_3->LinkEndChild(aabb_3_min); aabb_3->LinkEndChild(aabb_3_max);
		aabb_0->SetAttribute("index", nbAABB++); aabb_1->SetAttribute("index", nbAABB++); aabb_2->SetAttribute("index", nbAABB++); aabb_3->SetAttribute("index", nbAABB++);


		std::vector<TiXmlElement*> listAABB; listAABB.push_back(aabb_0); listAABB.push_back(aabb_1);  listAABB.push_back(aabb_2);  listAABB.push_back(aabb_3);
		std::vector<TiXmlElement*> listMin; listMin.push_back(aabb_0_min); listMin.push_back(aabb_1_min);  listMin.push_back(aabb_2_min);  listMin.push_back(aabb_3_min);
		std::vector<TiXmlElement*> listMax; listMax.push_back(aabb_0_max); listMax.push_back(aabb_1_max);  listMax.push_back(aabb_2_max);  listMax.push_back(aabb_3_max);

		inc = 0;
		for (TiXmlElement* node_src_p = node_src->FirstChildElement("Object"); node_src_p; node_src_p = node_src_p->NextSiblingElement("Object"))
		{
			//string name_ab = ""; node_src_p->QueryStringAttribute("Name", &name_ab);				//todo remove

			inc_b = 0;
			for (TiXmlElement* node_src_p2 = node_src_p->FirstChildElement("Object"); node_src_p2; node_src_p2 = node_src_p2->NextSiblingElement("Object"))
			{
				//string name_ac = ""; node_src_p2->QueryStringAttribute("TypeId", &name_ac);				//todo remove

				str = "";
				node_src_p2->QueryStringAttribute("value", &str);

				string attr = "";
				std::vector<TiXmlElement*>* list = 0;
				switch (inc)
				{
				case 0: list = &listMin; attr = "x"; break;			//lx
				case 1: list = &listMax; attr = "x"; break;			//hx
				case 2: list = &listMin; attr = "y"; break;			//ly
				case 3: list = &listMax; attr = "y"; break;			//hy
				case 4: list = &listMin; attr = "z"; break;			//lz
				case 5: list = &listMax; attr = "z"; break;			//hz
				case 6: list = &listAABB; attr = "data";
					size_t data = EMO_BaseFile::GetUnsigned(str);
					if (data > maxData)
						maxData = data;
					break;		//data
				}

				if (list)
					list->at(inc_b)->SetAttribute(attr, str);

				if (++inc_b >= 4)
					break;
			}
			if (++inc >= 7)
				break;
		}




		//visual
		float minX, minY, minZ, maxX, maxY, maxZ, tmp;
		for (size_t i=0;i<4;i++)
		{
			
			str = "";
			listAABB.at(i)->QueryStringAttribute("data", &str);
			if (str == "0")							//no interested by infinite
				continue;

			EMDMesh* emdMesh = new EMDMesh();
			emdModel->getMeshes().push_back(emdMesh);
			emdMesh->setName("AABB_" + ToString(nbAABB - 4 + i) + "__" + str);

			EMDSubmesh* emdSubMesh = new EMDSubmesh();
			emdMesh->getSubmeshes().push_back(emdSubMesh);
			//emdSubMesh->setMaterialName("AABB");
			emdSubMesh->setMaterialName("AABB_" + ToString(nbAABB - 4 + i) + "__" + str);
			emdSubMesh->setVertexTypeFlags(EMD_VTX_FLAG_POS | EMD_VTX_FLAG_COLOR);
			EMDTriangles triangle;

			
			listMin.at(i)->QueryStringAttribute("x", &str); tmp = StringToFloat(str); if (tmp < -10000) tmp = -10000; if (tmp > 10000) tmp = 10000; minX = tmp;	//check infinit => 10 000
			listMin.at(i)->QueryStringAttribute("y", &str); tmp = StringToFloat(str); if (tmp < -10000) tmp = -10000; if (tmp > 10000) tmp = 10000; minY = tmp;
			listMin.at(i)->QueryStringAttribute("z", &str); tmp = StringToFloat(str); if (tmp < -10000) tmp = -10000; if (tmp > 10000) tmp = 10000; minZ = tmp;
			listMax.at(i)->QueryStringAttribute("x", &str); tmp = StringToFloat(str); if (tmp < -10000) tmp = -10000; if (tmp > 10000) tmp = 10000; maxX = tmp;
			listMax.at(i)->QueryStringAttribute("y", &str); tmp = StringToFloat(str); if (tmp < -10000) tmp = -10000; if (tmp > 10000) tmp = 10000; maxY = tmp;
			listMax.at(i)->QueryStringAttribute("z", &str); tmp = StringToFloat(str); if (tmp < -10000) tmp = -10000; if (tmp > 10000) tmp = 10000; maxZ = tmp;
			
			

			EMDVertex v; v.flags = emdSubMesh->getVertexTypeFlags(); v.color = 0x0000FFFF;
			v.pos_x = minX;	v.pos_y = minY;	v.pos_z = maxZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = minX;	v.pos_y = minY;	v.pos_z = minZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = maxX;	v.pos_y = minY;	v.pos_z = minZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = maxX;	v.pos_y = minY;	v.pos_z = maxZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));

			v.pos_x = minX;	v.pos_y = maxY;	v.pos_z = maxZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = maxX;	v.pos_y = maxY;	v.pos_z = maxZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = maxX;	v.pos_y = maxY;	v.pos_z = minZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = minX;	v.pos_y = maxY;	v.pos_z = minZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));

			v.pos_x = minX;	v.pos_y = minY;	v.pos_z = maxZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = maxX;	v.pos_y = minY;	v.pos_z = maxZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = maxX;	v.pos_y = maxY;	v.pos_z = maxZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = minX;	v.pos_y = maxY;	v.pos_z = maxZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));

			v.pos_x = maxX;	v.pos_y = minY;	v.pos_z = maxZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = maxX;	v.pos_y = minY;	v.pos_z = minZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = maxX;	v.pos_y = maxY;	v.pos_z = minZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = maxX;	v.pos_y = maxY;	v.pos_z = maxZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));

			v.pos_x = maxX;	v.pos_y = minY;	v.pos_z = minZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = minX;	v.pos_y = minY;	v.pos_z = minZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = minX;	v.pos_y = maxY;	v.pos_z = minZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = maxX;	v.pos_y = maxY;	v.pos_z = minZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));

			v.pos_x = minX;	v.pos_y = minY;	v.pos_z = minZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = minX;	v.pos_y = minY;	v.pos_z = maxZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = minX;	v.pos_y = maxY;	v.pos_z = maxZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
			v.pos_x = minX;	v.pos_y = maxY;	v.pos_z = minZ;	emdSubMesh->getVertices().push_back(EMDVertex(&v));

			triangle.faces.push_back(0); triangle.faces.push_back(1); triangle.faces.push_back(2);
			triangle.faces.push_back(2); triangle.faces.push_back(3); triangle.faces.push_back(0);
			triangle.faces.push_back(4); triangle.faces.push_back(5); triangle.faces.push_back(6);
			triangle.faces.push_back(6); triangle.faces.push_back(7); triangle.faces.push_back(4);
			triangle.faces.push_back(8); triangle.faces.push_back(9); triangle.faces.push_back(10);
			triangle.faces.push_back(10); triangle.faces.push_back(11); triangle.faces.push_back(8);
			triangle.faces.push_back(12); triangle.faces.push_back(13); triangle.faces.push_back(14);
			triangle.faces.push_back(14); triangle.faces.push_back(15); triangle.faces.push_back(12);
			triangle.faces.push_back(16); triangle.faces.push_back(17); triangle.faces.push_back(18);
			triangle.faces.push_back(18); triangle.faces.push_back(19); triangle.faces.push_back(16);
			triangle.faces.push_back(20); triangle.faces.push_back(21); triangle.faces.push_back(22);
			triangle.faces.push_back(22); triangle.faces.push_back(23); triangle.faces.push_back(20);

			emdSubMesh->getTriangles().push_back(triangle);
		}

	}

	hyp_data_isStaticNodeIndex = maxData <= nbStaticNodes;				//because is index+1, because of 0 for inf aabb

	currentNode->SetAttribute("nbAABB", nbAABB);
	currentNode->SetAttribute("maxData", maxData);
	currentNode->SetAttribute("hyp_data_isStaticNodeIndex_plus1", hyp_data_isStaticNodeIndex ? "true" : "false");

	return currentNode;
}








#endif


}