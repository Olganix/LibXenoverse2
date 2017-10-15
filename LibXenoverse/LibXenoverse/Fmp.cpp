#include "Fmp.h"

namespace LibXenoverse
{

/*-------------------------------------------------------------------------------\
|                             FmpFile				                             |
\-------------------------------------------------------------------------------*/
FmpFile::FmpFile()
{
	//section 0 - FMPHeader : black/grey
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#232323");			//background color
	listTagColors.back().back().push_back("#FFFFFF");			//font color (just to possibility read from background color)
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#464646");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 1 - FMPUnk0AEntry : red
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC0000");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC7777");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 2 - FMPUnk0BEntry : green
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#00CC00");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#77CC77");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 3 - FMPUnk0BEntry_names : green
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#009900");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#339933");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 4 - FMPUnk1Entry : bleu
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#0000CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7777CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 5 - FMPUnk2Entry : yellow
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCC00");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCC77");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 6 - FMPUnk3Entry : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#00CCCC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#77CCCC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 7 - FMPUnk3Entry indexes : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#009999");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#339999");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 8 - FMPUnk4Entry : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC00CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC77CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 9 - FMPUnk4Entry_Next : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#990099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#993399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f



	//section 10 - FMPUnk4Entry_Next_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 11 - FMPUnk4Entry_Next_c : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC0099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC3399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 12 - FMPUnk4Entry_Next_d : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900AA");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933AA");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 13 - FMPUnk4Entry_Next_e : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA0099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA3399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f



	//section 14 - FMPUnk4Entry_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#770077");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#773377");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 15 - FMPUnk4Entry_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#550055");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#553355");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f





	//section 16 - FMPUnk4Entry_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA00FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA33FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 17 - FMPUnk4Entry_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 18 - FMPUnk4Entry_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7700FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7733FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 19 - FMPUnk4Entry_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#5500FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#5533FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f




	//section 20 - FMPUnk5Entry : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCCCC");			//bg
	listTagColors.back().back().push_back("#000000");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AAAAAA");			//bg
	listTagColors.back().back().push_back("#000000");			//f


	//section 21 - FMPUnk5Entry_Box : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#999999");			//bg
	listTagColors.back().back().push_back("#000000");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#777777");			//bg
	listTagColors.back().back().push_back("#000000");			//f
}

/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool FmpFile::Load(const uint8_t *buf, size_t size)
{
	Reset();

	FMPHeader *hdr = (FMPHeader *)buf;

	if (size < sizeof(FMPHeader) || memcmp(hdr->signature, FMP_SIGNATURE, 4) != 0)
		return false;

	/*
	for (size_t i = 0; i < 8; i++)
		unk_08[i] = hdr->unk_08[i];
	*/


	FMPUnk0AEntry *unk0A = (FMPUnk0AEntry*)GetOffsetPtr(buf, hdr->unk0A_offset);
	//FMPUnk0BEntry *unk0B = (FMPUnk0BEntry *)GetOffsetPtr(buf, hdr->unk0B_offset);


	FMPUnk1Entry *unk1 = (FMPUnk1Entry*)GetOffsetPtr(buf, hdr->unk1_offset);
	//for (size_t i = 0; i < hdr->num_unk1; i++)
	//	unk1[i];

	FMPUnk2Entry *unk2 = (FMPUnk2Entry*)GetOffsetPtr(buf, hdr->unk2_offset);
	//for (size_t i = 0; i < hdr->num_unk2; i++)
	//	unk2[i];


	FMPUnk3Entry *unk3 = (FMPUnk3Entry*)GetOffsetPtr(buf, hdr->unk3_offset);
	//for (size_t i = 0; i < hdr->num_unk3; i++)
	//	unk3[i];


	FMPUnk4Entry *unk4 = (FMPUnk4Entry*)GetOffsetPtr(buf, hdr->unk4_offset);
	FMPUnk4Entry_Next *unk4_next;
	for (size_t i = 0; i < hdr->num_unk4; i++)
	{
		//todo 

		if (unk4[i].unk1_offset)
		{
			unk4_next = (FMPUnk4Entry_Next*)GetOffsetPtr(buf, unk4[i].unk1_offset);

			//todo 
		}
	}


	FMPUnk5Entry *unk5 = (FMPUnk5Entry*)GetOffsetPtr(buf, hdr->unk5_offset);
	FMPUnk5Entry_Box *unk5_Box;
	for (size_t i = 0; i < hdr->num_unk5; i++)
	{
		unk5_Box = (FMPUnk5Entry_Box*)GetOffsetPtr(buf, unk5[i].unk1_offset);

		size_t nbBox = unk5[i].count;
		//for (size_t j = 0; j < nbBox; j++)
		//	unk5_Box[j];
	}


	//FMP_Dep1 *dep1 = (FMP_Dep1*)GetOffsetPtr(buf, hdr->dep1_table_offset);
	//for (size_t i = 0; i < hdr->num_dep1; i++)
	//	dep1[i];

	//FMP_Dep2 *dep2 = (FMP_Dep2*)GetOffsetPtr(buf, hdr->dep2_table_offset);
	//for (size_t i = 0; i < hdr->num_dep2; i++)
	//	dep2[i];

	//FMP_Dep3 *dep3 = (FMP_Dep3*)GetOffsetPtr(buf, hdr->dep3_table_offset);
	//for (size_t i = 0; i < hdr->num_dep3; i++)
	//	dep3[i];

	//FMP_Dep4 *dep4 = (FMP_Dep4*)GetOffsetPtr(buf, hdr->dep4_table_offset);
	//for (size_t i = 0; i < hdr->num_dep4; i++)
	//	dep4[i];

	//for (size_t i = 0; i < 4; i++)
	//	unk_60[i] = hdr->unk_60[i];



	/*

	FMPUnk1EntryHeader *unk1_entries = (FMPUnk1EntryHeader *)GetOffsetPtr(buf, hdr->unk1e_offset);
	this->unk1_entries.resize(val32(hdr->num_unk1e));

	for (size_t i = 0; i < this->unk1_entries.size(); i++)
	{
		FmpUnk1 &unk1 = this->unk1_entries[i];

		unk1.name = (char *)GetOffsetPtr(buf, unk1_entries[i].name_offset);
		unk1.unsigneds.resize(val32(unk1_entries->num_unsigned));

		for (size_t j = 0; j < unk1.unsigneds.size(); j++)
		{
			uint16_t *p = (uint16_t *)GetOffsetPtr(buf, unk1_entries[i].unsigned_offsets);
			unk1.unsigneds[j] = val16(p[j]);
		}
	}

	FMPUnk2EntryHeader *unk2_entries = (FMPUnk2EntryHeader *)GetOffsetPtr(buf, hdr->unk2e_offset);
	this->unk2_entries.resize(val32(hdr->num_unk2e));

	for (size_t i = 0; i < this->unk2_entries.size(); i++)
	{
		FmpUnk2 &unk2 = this->unk2_entries[i];

		unk2.name = (char *)GetOffsetPtr(buf, unk2_entries[i].name_offset);
		unk2.unk_04 = val16(unk2_entries[i].unk_04);
		unk2.unk_06 = val16(unk2_entries[i].unk_06);
		unk2.unk_08 = val16(unk2_entries[i].unk_08);
		unk2.unk_0A = val16(unk2_entries[i].unk_0A);
		unk2.unk_1C = val32(unk2_entries[i].unk_1C);

		for (int j = 0; j < 16; j++)
		{
			unk2.matrix[j] = val_float(unk2_entries[i].matrix[j]);
		}
	}

	*/

	return true;
}

void FmpFile::Reset()
{

}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Xml First Version for Debug ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*-------------------------------------------------------------------------------\
|                             save_Xml				                             |
\-------------------------------------------------------------------------------*/
void FmpFile::save_Xml(string filename, bool show_error)
{
	uint8_t *buf;
	size_t size;

	buf = ReadFile(filename, &size, show_error);
	if (!buf)
		return;

	TiXmlDocument *doc = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	doc->LinkEndChild(decl);

	TiXmlElement* root = new TiXmlElement("Fmp");
	EMO_BaseFile::WriteParamString(root, "path", filename);
	
	write_Xml(root, buf, size);

	doc->LinkEndChild(root);



	delete[] buf;

	doc->SaveFile(filename + ".xml");

	return;
}




/*-------------------------------------------------------------------------------\
|                             write_Xml						                     |
\-------------------------------------------------------------------------------*/
void FmpFile::write_Xml(TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	FMPHeader *hdr = (FMPHeader *)buf;
	if (size < sizeof(FMPHeader) || memcmp(hdr->signature, FMP_SIGNATURE, 4) != 0)
		return;


	std::vector<bool> listBytesAllreadyTagged;
	listBytesAllreadyTagged.resize(size, false);				//to check override of the same byte (overflow)


	//node->SetAttribute("value", UnsignedToString(value, hexadecimal));
	
	TiXmlElement* node_FMPHeader = new TiXmlElement("Header");
	TiXmlElement* node;
	node = new TiXmlElement("unk_08"); node->SetAttribute("u32", UnsignedToString(hdr->unk_08, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("unk_0C"); node->SetAttribute("u32", UnsignedToString(hdr->unk_0C, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("Unk0AEntry"); node->SetAttribute("startOffset", UnsignedToString(hdr->unk0A_offset, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("Unk0BEntry"); node->SetAttribute("startOffset", UnsignedToString(hdr->unk0B_offset, true)); node_FMPHeader->LinkEndChild(node);

	node = new TiXmlElement("Unk1Entry"); node->SetAttribute("count", hdr->num_unk1);  node->SetAttribute("startOffset", UnsignedToString(hdr->unk1_offset, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("Unk2Entry"); node->SetAttribute("count", hdr->num_unk2);  node->SetAttribute("startOffset", UnsignedToString(hdr->unk2_offset, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("Unk3Entry"); node->SetAttribute("count", hdr->num_unk3);  node->SetAttribute("startOffset", UnsignedToString(hdr->unk3_offset, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("Unk4Entry"); node->SetAttribute("count", hdr->num_unk4);  node->SetAttribute("startOffset", UnsignedToString(hdr->unk4_offset, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("Unk5Entry"); node->SetAttribute("count", hdr->num_unk5);  node->SetAttribute("startOffset", UnsignedToString(hdr->unk5_offset, true)); node_FMPHeader->LinkEndChild(node);

	node = new TiXmlElement("Unk1Dep"); node->SetAttribute("count", hdr->num_dep1);  node->SetAttribute("startOffset", UnsignedToString(hdr->dep1_table_offset, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("Unk2Dep"); node->SetAttribute("count", hdr->num_dep2);  node->SetAttribute("startOffset", UnsignedToString(hdr->dep2_table_offset, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("Unk3Dep"); node->SetAttribute("count", hdr->num_dep3);  node->SetAttribute("startOffset", UnsignedToString(hdr->dep3_table_offset, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("Unk4Dep"); node->SetAttribute("count", hdr->num_dep4);  node->SetAttribute("startOffset", UnsignedToString(hdr->dep4_table_offset, true)); node_FMPHeader->LinkEndChild(node);
	
	node = new TiXmlElement("unk_60__0_"); node->SetAttribute("u32", UnsignedToString(hdr->unk_60[0], true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("unk_60__1_"); node->SetAttribute("u32", UnsignedToString(hdr->unk_60[1], true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("unk_60__2_"); node->SetAttribute("u32", UnsignedToString(hdr->unk_60[2], true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("unk_60__3_"); node->SetAttribute("u32", UnsignedToString(hdr->unk_60[3], true)); node_FMPHeader->LinkEndChild(node);
	parent->LinkEndChild(node_FMPHeader);
	



	FMPUnk0AEntry *unk0A = (FMPUnk0AEntry*)GetOffsetPtr(buf, hdr->unk0A_offset);
	TiXmlElement* node_FMPUnk0AEntry = new TiXmlElement("Unk0AEntry");
	node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_0, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_1, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_2"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_2, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_3"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_3, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_4"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_4, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_5"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_5, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_6"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_6, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_7"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_7, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_8"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_8, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_9"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_9, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_10"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_10, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_11"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_11, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_12"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_12, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_13"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_13, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_14"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_14, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_15"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_15, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_16"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_16, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_17"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_17, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_18"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_18, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_19"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_19, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_20"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_20, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_21"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_21, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_22"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_22, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_23"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_23, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_24"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_24, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_25"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_25, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_26"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_26, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_27"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_27, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_28"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_28, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_29"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_29, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_30"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_30, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_31"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_31, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_32"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_32, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_33"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_33, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	node = new TiXmlElement("unk_34"); node->SetAttribute("u32", UnsignedToString(unk0A->unk_34, true)); node_FMPUnk0AEntry->LinkEndChild(node);
	
	parent->LinkEndChild(node_FMPUnk0AEntry);


	
	TiXmlElement* node_FMPUnk0BEntry = new TiXmlElement("Unk0BEntry");

	//todo reste

	TiXmlElement* node_FMPUnk0BEntry_names = new TiXmlElement("ItemTypes");	
	uint32_t* name_offset;
	string name;
	for (size_t i = 0; i < 51; i++)
	{
		name_offset = (uint32_t*)GetOffsetPtr(buf, hdr->unk0B_offset + 0x100);
		name = std::string((char *)GetOffsetPtr(buf, name_offset, i));

		node = new TiXmlElement("ItemType"); node->SetAttribute("id", UnsignedToString(i, true)); node->SetAttribute("name", name); node_FMPUnk0BEntry_names->LinkEndChild(node);
	}
	node_FMPUnk0BEntry->LinkEndChild(node_FMPUnk0BEntry_names);

	parent->LinkEndChild(node_FMPUnk0BEntry);


	



	FMPUnk1Entry *unk1 = (FMPUnk1Entry*)GetOffsetPtr(buf, hdr->unk1_offset);
	TiXmlElement* node_FMPUnk1Entrys = new TiXmlElement("Unk1Entrys");
	node_FMPUnk1Entrys->SetAttribute("count", hdr->num_unk1);
	TiXmlElement* node_FMPUnk1Entry;
	for (size_t i = 0; i < hdr->num_unk1; i++)
	{
		node_FMPUnk1Entry = new TiXmlElement("Unk1Entry");
		
		name = std::string((char *)GetOffsetPtr(buf, unk1[i].name_offset));
		node = new TiXmlElement("name"); node->SetAttribute("name", name); node_FMPUnk1Entry->LinkEndChild(node);

		node = new TiXmlElement("unk_04"); node->SetAttribute("u32", UnsignedToString(unk1[i].unk_04, true)); node_FMPUnk1Entry->LinkEndChild(node);
		node = new TiXmlElement("unk_08__0_"); node->SetAttribute("f32", std::to_string(unk1[i].unk_08[0])); node_FMPUnk1Entry->LinkEndChild(node);
		node = new TiXmlElement("unk_08__1_"); node->SetAttribute("f32", std::to_string(unk1[i].unk_08[1])); node_FMPUnk1Entry->LinkEndChild(node);
		node = new TiXmlElement("unk_08__2_"); node->SetAttribute("f32", std::to_string(unk1[i].unk_08[2])); node_FMPUnk1Entry->LinkEndChild(node);
		node = new TiXmlElement("unk_08__3_"); node->SetAttribute("f32", std::to_string(unk1[i].unk_08[3])); node_FMPUnk1Entry->LinkEndChild(node);

		node_FMPUnk1Entrys->LinkEndChild(node_FMPUnk1Entry);
	}
	parent->LinkEndChild(node_FMPUnk1Entrys);






	FMPUnk2Entry *unk2 = (FMPUnk2Entry*)GetOffsetPtr(buf, hdr->unk2_offset);
	TiXmlElement* node_FMPUnk2Entrys = new TiXmlElement("Unk2Entrys");
	node_FMPUnk2Entrys->SetAttribute("count", hdr->num_unk2);
	TiXmlElement* node_FMPUnk2Entry;
	for (size_t i = 0; i < hdr->num_unk2; i++)
	{
		node_FMPUnk2Entry = new TiXmlElement("Unk2Entry");

		name = std::string((char *)GetOffsetPtr(buf, unk2[i].name_offset));
		node = new TiXmlElement("name"); node->SetAttribute("name", name); node_FMPUnk2Entry->LinkEndChild(node);

		node = new TiXmlElement("unk_04__0_"); node->SetAttribute("f32", std::to_string(unk2[i].unk_04[0])); node_FMPUnk2Entry->LinkEndChild(node);
		node = new TiXmlElement("unk_04__1_"); node->SetAttribute("f32", std::to_string(unk2[i].unk_04[1])); node_FMPUnk2Entry->LinkEndChild(node);
		node = new TiXmlElement("unk_04__2_"); node->SetAttribute("f32", std::to_string(unk2[i].unk_04[2])); node_FMPUnk2Entry->LinkEndChild(node);
		node = new TiXmlElement("unk_04__3_"); node->SetAttribute("f32", std::to_string(unk2[i].unk_04[3])); node_FMPUnk2Entry->LinkEndChild(node);
		node = new TiXmlElement("unk_04__4_"); node->SetAttribute("f32", std::to_string(unk2[i].unk_04[4])); node_FMPUnk2Entry->LinkEndChild(node);
		node = new TiXmlElement("unk_04__5_"); node->SetAttribute("f32", std::to_string(unk2[i].unk_04[5])); node_FMPUnk2Entry->LinkEndChild(node);

		node_FMPUnk2Entrys->LinkEndChild(node_FMPUnk2Entry);
	}
	parent->LinkEndChild(node_FMPUnk2Entrys);




	FMPUnk3Entry *unk3 = (FMPUnk3Entry*)GetOffsetPtr(buf, hdr->unk3_offset);
	TiXmlElement* node_FMPUnk3Entrys = new TiXmlElement("Unk3Entrys");
	node_FMPUnk3Entrys->SetAttribute("count", hdr->num_unk3);
	TiXmlElement* node_FMPUnk3Entry;
	for (size_t i = 0; i < hdr->num_unk3; i++)
	{
		node_FMPUnk3Entry = new TiXmlElement("Unk3Entry");

		name = std::string((char *)GetOffsetPtr(buf, unk3[i].name_offset));
		node = new TiXmlElement("name"); node->SetAttribute("name", name); node_FMPUnk3Entry->LinkEndChild(node);


		TiXmlElement* node_Indexes = new TiXmlElement("Indexes");
		size_t nbIndex = unk3[i].num_indexes;
		node_Indexes->SetAttribute("count", nbIndex);

		for (size_t j = 0; j < nbIndex; j++)
		{
			size_t offset_b = unk3[i].indexes_offset + j * sizeof(int16_t);
			int16_t value = *((int16_t*)GetOffsetPtr(buf, offset_b));

			node = new TiXmlElement("Index_Into_Unk4Entrys"); node->SetAttribute("value", value); node_Indexes->LinkEndChild(node);
		}
		node_FMPUnk3Entry->LinkEndChild(node_Indexes);

		node_FMPUnk3Entrys->LinkEndChild(node_FMPUnk3Entry);
	}
	parent->LinkEndChild(node_FMPUnk3Entrys);





	FMPUnk4Entry *unk4 = (FMPUnk4Entry*)GetOffsetPtr(buf, hdr->unk4_offset);
	FMPUnk4Entry_Next *unk4_next;

	FMPUnk4Entry_Next2 *unk4_next2;
	FMPUnk4Entry_Next2_b *unk4_next2_b;
	FMPUnk4Entry_Next2_c *unk4_next2_c;
	
	TiXmlElement* node_FMPUnk4Entrys = new TiXmlElement("Unk4Entrys");
	TiXmlElement* node_FMPUnk4Entry_Next;

	TiXmlElement* node_FMPUnk4Entry_Next2;
	TiXmlElement* node_FMPUnk4Entry_Next2_b;
	TiXmlElement* node_FMPUnk4Entry_Next2_c;

	node_FMPUnk4Entrys->SetAttribute("count", hdr->num_unk4);
	TiXmlElement* node_FMPUnk4Entry;
	for (size_t i = 0; i < hdr->num_unk4; i++)
	{
		node_FMPUnk4Entry = new TiXmlElement("Unk4Entry");

		name = std::string((char *)GetOffsetPtr(buf, unk4[i].name_offset));
		node = new TiXmlElement("name"); node->SetAttribute("name", name); node_FMPUnk4Entry->LinkEndChild(node);

		node = new TiXmlElement("unk_04"); node->SetAttribute("u32", UnsignedToString(unk4[i].unk_04, true)); node_FMPUnk4Entry->LinkEndChild(node);
		node = new TiXmlElement("unk_08"); node->SetAttribute("u16", UnsignedToString(unk4[i].unk_08, true)); node_FMPUnk4Entry->LinkEndChild(node);
		node = new TiXmlElement("unk5_index"); node->SetAttribute("u16", UnsignedToString(unk4[i].unk5_index, true)); node_FMPUnk4Entry->LinkEndChild(node);

		
		node_FMPUnk4Entry_Next = new TiXmlElement("Next");
		node_FMPUnk4Entry_Next->SetAttribute("startOffset", UnsignedToString(unk4[i].unk1_offset, true));
		node_FMPUnk4Entry_Next->SetAttribute("anotherStartOffset", UnsignedToString(unk4[i].unk2_offset, true));
		if (unk4[i].unk1_offset)
		{
			unk4_next = (FMPUnk4Entry_Next*)GetOffsetPtr(buf, unk4[i].unk1_offset);
			
			node = new TiXmlElement("count"); node->SetAttribute("u32", UnsignedToString(unk4_next->count, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);

			node = new TiXmlElement("unk1_offset"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk1_offset, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk2_offset"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk2_offset, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk_0, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk3_offset"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk2_offset, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk_2, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk_4"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk_4, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk_5"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk_5, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk_6"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk_6, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk_7"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk_7, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk_8"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk_8, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk_9"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk_9, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk_10"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk_10, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk_8b"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk_8b, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk_9b"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk_9b, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk_10b"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk_10b, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk_11b"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk_11b, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk_12"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk_12, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk_13"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk_13, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
			node = new TiXmlElement("unk_14"); node->SetAttribute("u32", UnsignedToString(unk4_next->unk_14, true)); node_FMPUnk4Entry_Next->LinkEndChild(node);
		}
		node_FMPUnk4Entry->LinkEndChild(node_FMPUnk4Entry_Next);

		
		
		
		node = new TiXmlElement("unk_14"); node->SetAttribute("u32", UnsignedToString(unk4[i].unk_14, true)); node_FMPUnk4Entry->LinkEndChild(node);

		node_FMPUnk4Entry_Next2 = new TiXmlElement("Next2");
		node_FMPUnk4Entry_Next2->SetAttribute("startOffset", UnsignedToString(unk4[i].unk3_offset, true));
		node_FMPUnk4Entry_Next2->SetAttribute("anotherStartOffset", UnsignedToString(unk4[i].unk4_offset, true));
		if (unk4[i].unk3_offset)
		{
			unk4_next2 = (FMPUnk4Entry_Next2*)GetOffsetPtr(buf, unk4[i].unk3_offset);



			node_FMPUnk4Entry_Next2_b = new TiXmlElement("Next2_b");
			node_FMPUnk4Entry_Next2_b->SetAttribute("startOffset", UnsignedToString(unk4_next2->unk1_offset, true));
			if (unk4_next2->unk1_offset)
			{
				unk4_next2_b = (FMPUnk4Entry_Next2_b*)GetOffsetPtr(buf, unk4_next2->unk1_offset);

				name = std::string((char *)GetOffsetPtr(buf, unk4_next2_b->name_offset));
				node = new TiXmlElement("name"); node->SetAttribute("name", name); node_FMPUnk4Entry_Next2_b->LinkEndChild(node);

				node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(unk4_next2_b->unk_0, true)); node_FMPUnk4Entry_Next2_b->LinkEndChild(node);
				node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(unk4_next2_b->unk_1, true)); node_FMPUnk4Entry_Next2_b->LinkEndChild(node);
				
				if (unk4_next2_b->unk_1 == 2)
				{
					node_FMPUnk4Entry_Next2_c = new TiXmlElement("Next2_c");
					node_FMPUnk4Entry_Next2_c->SetAttribute("startOffset", UnsignedToString(unk4_next2_b->unk_2, true));
					if (unk4_next2_b->unk_2)
					{
						unk4_next2_c = (FMPUnk4Entry_Next2_c*)GetOffsetPtr(buf, unk4_next2_b->unk_2);

						node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(unk4_next2_c->unk_0, true)); node_FMPUnk4Entry_Next2_c->LinkEndChild(node);
						node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(unk4_next2_c->unk_1, true)); node_FMPUnk4Entry_Next2_c->LinkEndChild(node);
					}
					node_FMPUnk4Entry_Next2_b->LinkEndChild(node_FMPUnk4Entry_Next2_c);
				} else {
					node = new TiXmlElement("unk_2"); node->SetAttribute("u32", UnsignedToString(unk4_next2_b->unk_2, true)); node_FMPUnk4Entry_Next2_b->LinkEndChild(node);
				}

				node = new TiXmlElement("unk_3"); node->SetAttribute("u32", UnsignedToString(unk4_next2_b->unk_3, true)); node_FMPUnk4Entry_Next2_b->LinkEndChild(node);
				
				if (unk4_next2_b->unk_1 == 2)
				{
					node_FMPUnk4Entry_Next2_c = new TiXmlElement("Next2_c");
					node_FMPUnk4Entry_Next2_c->SetAttribute("startOffset", UnsignedToString(unk4_next2_b->unk_4, true));
					if (unk4_next2_b->unk_4)
					{
						unk4_next2_c = (FMPUnk4Entry_Next2_c*)GetOffsetPtr(buf, unk4_next2_b->unk_4);

						node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(unk4_next2_c->unk_0, true)); node_FMPUnk4Entry_Next2_c->LinkEndChild(node);
						node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(unk4_next2_c->unk_1, true)); node_FMPUnk4Entry_Next2_c->LinkEndChild(node);
					}
					node_FMPUnk4Entry_Next2_b->LinkEndChild(node_FMPUnk4Entry_Next2_c);
				} else {
					node = new TiXmlElement("unk_4"); node->SetAttribute("u32", UnsignedToString(unk4_next2_b->unk_4, true)); node_FMPUnk4Entry_Next2_b->LinkEndChild(node);
				}

				node = new TiXmlElement("unk_5"); node->SetAttribute("u32", UnsignedToString(unk4_next2_b->unk_5, true)); node_FMPUnk4Entry_Next2_b->LinkEndChild(node);
				node = new TiXmlElement("unk_6"); node->SetAttribute("u32", UnsignedToString(unk4_next2_b->unk_6, true)); node_FMPUnk4Entry_Next2_b->LinkEndChild(node);
				node = new TiXmlElement("unk_7"); node->SetAttribute("u32", UnsignedToString(unk4_next2_b->unk_7, true)); node_FMPUnk4Entry_Next2_b->LinkEndChild(node);
				node = new TiXmlElement("unk_8"); node->SetAttribute("u32", UnsignedToString(unk4_next2_b->unk_8, true)); node_FMPUnk4Entry_Next2_b->LinkEndChild(node);
				node = new TiXmlElement("unk_9"); node->SetAttribute("u32", UnsignedToString(unk4_next2_b->unk_9, true)); node_FMPUnk4Entry_Next2_b->LinkEndChild(node);
				node = new TiXmlElement("unk_10"); node->SetAttribute("u32", UnsignedToString(unk4_next2_b->unk_10, true)); node_FMPUnk4Entry_Next2_b->LinkEndChild(node);
				
				if (unk4_next2_b->unk_1 == 2)
				{
					node_FMPUnk4Entry_Next2_c = new TiXmlElement("Next2_c");
					node_FMPUnk4Entry_Next2_c->SetAttribute("startOffset", UnsignedToString(unk4_next2_b->unk_11, true));
					if(unk4_next2_b->unk_11)
					{
						unk4_next2_c = (FMPUnk4Entry_Next2_c*)GetOffsetPtr(buf, unk4_next2_b->unk_11);

						node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(unk4_next2_c->unk_0, true)); node_FMPUnk4Entry_Next2_c->LinkEndChild(node);
						node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(unk4_next2_c->unk_1, true)); node_FMPUnk4Entry_Next2_c->LinkEndChild(node);
					}
					node_FMPUnk4Entry_Next2_b->LinkEndChild(node_FMPUnk4Entry_Next2_c);

				}else{
					node = new TiXmlElement("unk_11"); node->SetAttribute("u32", UnsignedToString(unk4_next2_b->unk_11, true)); node_FMPUnk4Entry_Next2_b->LinkEndChild(node);
				}
			}
			node_FMPUnk4Entry_Next2->LinkEndChild(node_FMPUnk4Entry_Next2_b);
			


			node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(unk4_next2->unk_0, true)); node_FMPUnk4Entry_Next2->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(unk4_next2->unk_1, true)); node_FMPUnk4Entry_Next2->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("u32", UnsignedToString(unk4_next2->unk_2, true)); node_FMPUnk4Entry_Next2->LinkEndChild(node);
			node = new TiXmlElement("unk_3"); node->SetAttribute("u32", UnsignedToString(unk4_next2->unk_3, true)); node_FMPUnk4Entry_Next2->LinkEndChild(node);
			node = new TiXmlElement("unk_4"); node->SetAttribute("u32", UnsignedToString(unk4_next2->unk_4, true)); node_FMPUnk4Entry_Next2->LinkEndChild(node);
			node = new TiXmlElement("unk_5"); node->SetAttribute("u32", UnsignedToString(unk4_next2->unk_5, true)); node_FMPUnk4Entry_Next2->LinkEndChild(node);
			node = new TiXmlElement("unk_6"); node->SetAttribute("u32", UnsignedToString(unk4_next2->unk_6, true)); node_FMPUnk4Entry_Next2->LinkEndChild(node);
			node = new TiXmlElement("unk_7"); node->SetAttribute("u32", UnsignedToString(unk4_next2->unk_7, true)); node_FMPUnk4Entry_Next2->LinkEndChild(node);
			node = new TiXmlElement("unk_8"); node->SetAttribute("u32", UnsignedToString(unk4_next2->unk_8, true)); node_FMPUnk4Entry_Next2->LinkEndChild(node);
			node = new TiXmlElement("unk_9"); node->SetAttribute("u32", UnsignedToString(unk4_next2->unk_9, true)); node_FMPUnk4Entry_Next2->LinkEndChild(node);
			node = new TiXmlElement("unk_10"); node->SetAttribute("u32", UnsignedToString(unk4_next2->unk_10, true)); node_FMPUnk4Entry_Next2->LinkEndChild(node);
			node = new TiXmlElement("unk_11"); node->SetAttribute("u32", UnsignedToString(unk4_next2->unk_11, true)); node_FMPUnk4Entry_Next2->LinkEndChild(node);
			node = new TiXmlElement("unk_12"); node->SetAttribute("u32", UnsignedToString(unk4_next2->unk_12, true)); node_FMPUnk4Entry_Next2->LinkEndChild(node);
		}
		node_FMPUnk4Entry->LinkEndChild(node_FMPUnk4Entry_Next2);



		node = new TiXmlElement("unk_20"); node->SetAttribute("u32", UnsignedToString(unk4[i].unk_20, true)); node_FMPUnk4Entry->LinkEndChild(node);


		TiXmlElement* node_matrix = new TiXmlElement("TransformMatrix4x3");
		node = new TiXmlElement("L0"); node->SetAttribute("f32s", std::to_string(unk4[i].transformMatrix4x3[0]) +", "+ std::to_string(unk4[i].transformMatrix4x3[1]) + ", " + std::to_string(unk4[i].transformMatrix4x3[2])); node_matrix->LinkEndChild(node);
		node = new TiXmlElement("L1"); node->SetAttribute("f32s", std::to_string(unk4[i].transformMatrix4x3[3]) + ", " + std::to_string(unk4[i].transformMatrix4x3[4]) + ", " + std::to_string(unk4[i].transformMatrix4x3[5])); node_matrix->LinkEndChild(node);
		node = new TiXmlElement("L2"); node->SetAttribute("f32s", std::to_string(unk4[i].transformMatrix4x3[6]) + ", " + std::to_string(unk4[i].transformMatrix4x3[7]) + ", " + std::to_string(unk4[i].transformMatrix4x3[8])); node_matrix->LinkEndChild(node);
		node = new TiXmlElement("L3"); node->SetAttribute("f32s", std::to_string(unk4[i].transformMatrix4x3[9]) + ", " + std::to_string(unk4[i].transformMatrix4x3[10]) + ", " + std::to_string(unk4[i].transformMatrix4x3[11])); node_matrix->LinkEndChild(node);
		node_FMPUnk4Entry->LinkEndChild(node_matrix);

		node_FMPUnk4Entrys->LinkEndChild(node_FMPUnk4Entry);
	}
	parent->LinkEndChild(node_FMPUnk4Entrys);










	FMPUnk5Entry *unk5 = (FMPUnk5Entry*)GetOffsetPtr(buf, hdr->unk5_offset);
	FMPUnk5Entry_Box *unk5_Box;
	TiXmlElement* node_FMPUnk5Entrys = new TiXmlElement("UnkE5ntrys");
	TiXmlElement* node_Boxes;
	TiXmlElement* node_Box;
	node_FMPUnk5Entrys->SetAttribute("count", hdr->num_unk5);
	TiXmlElement* node_FMPUnk5Entry;
	for (size_t i = 0; i < hdr->num_unk5; i++)
	{
		node_FMPUnk5Entry = new TiXmlElement("Unk5Entry");

		name = std::string((char *)GetOffsetPtr(buf, unk5[i].name_offset));
		node = new TiXmlElement("name"); node->SetAttribute("name", name); node_FMPUnk5Entry->LinkEndChild(node);


		node_Boxes = new TiXmlElement("HitBoxes");
		size_t nbBox = unk5[i].count;
		node_Boxes->SetAttribute("count", nbBox);

		if (unk5[i].unk1_offset)
		{
			uint32_t offset = unk5[i].unk1_offset;

			
			for (size_t j = 0; j < nbBox; j++)
			{
				uint32_t* nameOffset_tmp = (uint32_t*)GetOffsetPtr(buf, offset);
				if (*nameOffset_tmp >= (uint32_t)size)
				{
					printf("Error : overflow on HitBox name. skipped all next hitbox from Unk5.\n", name.c_str());
					break;						//we can't know witch size in bytes.
				}
				name = std::string((char *)GetOffsetPtr(buf, *nameOffset_tmp));
				

				//printf("HitBox '%s' found.\n", name.c_str());		//todo remettre


				

				unk5_Box = (FMPUnk5Entry_Box*)GetOffsetPtr(buf, offset);
				offset += sizeof(FMPUnk5Entry_Box);



				node_Box = new TiXmlElement("HitBox");
				node = new TiXmlElement("name"); node->SetAttribute("name", name); node_Box->LinkEndChild(node);
				
				node = new TiXmlElement("child"); node->SetAttribute("u16", UnsignedToString(unk5_Box[0].child, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_a0"); node->SetAttribute("u16", UnsignedToString(unk5_Box[0].unk_a0, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("sibling"); node->SetAttribute("u16", UnsignedToString(unk5_Box[0].sibling, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("parent"); node->SetAttribute("u16", UnsignedToString(unk5_Box[0].parent, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_a1"); node->SetAttribute("u16", UnsignedToString(unk5_Box[0].unk_a1, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_a2"); node->SetAttribute("u16", UnsignedToString(unk5_Box[0].unk_a2, true)); node_Box->LinkEndChild(node);

				node = new TiXmlElement("unk1_offset"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk1_offset, true)); node_Box->LinkEndChild(node);

				node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_0, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("count2"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].count2, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk2_offset"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk2_offset, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("count3"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].count3, true)); node_Box->LinkEndChild(node);

				/*

				//Todo.

				if ((name != "BOX") && (name != "SPHERE"))
				{
					uin32_t	unk_5 = *((uint32_t*)GetOffsetPtr(buf, unk5_Box->unk1_offset));
					node = new TiXmlElement("unk_5"); node->SetAttribute("u32", UnsignedToString(unk5_Col[0].unk_5, true)); node_Box->LinkEndChild(node);
					continue;
				}

				node = new TiXmlElement("unk_5"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_5, true)); node_Box->LinkEndChild(node);

				node = new TiXmlElement("unk_6"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_6, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk2_offset"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk2_offset, true)); node_Box->LinkEndChild(node);
				
				node = new TiXmlElement("unk_7"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_7, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk3_offset"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk3_offset, true)); node_Box->LinkEndChild(node);

				node = new TiXmlElement("unk_8"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_8, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_9"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_9, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_10"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_10, true)); node_Box->LinkEndChild(node);

				node = new TiXmlElement("unk_11"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_11, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk4_offset"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk4_offset, true)); node_Box->LinkEndChild(node);

				node = new TiXmlElement("unk_12"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_12, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_13"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_13, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_14"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_14, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_15"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_15, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_16"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_16, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_17"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_17, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_18"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_18, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_19"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_19, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_20"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_20, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_21"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_21, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_22"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_22, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_23"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_23, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_24"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_24, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_25"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_25, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_26"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_26, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_27"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_27, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_28"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_28, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_29"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_29, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_30"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_30, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_31"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_31, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_32"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_32, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_33"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_33, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_34"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_34, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_35"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_35, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_36"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_36, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_37"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_37, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_38"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_38, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_39"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_39, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_40"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_40, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_41"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_41, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_42"); node->SetAttribute("u32", UnsignedToString(unk5_Box[0].unk_42, true)); node_Box->LinkEndChild(node);
				*/

				node_Boxes->LinkEndChild(node_Box);
			}
		}

		node_FMPUnk5Entry->LinkEndChild(node_Boxes);


		node_FMPUnk5Entrys->LinkEndChild(node_FMPUnk5Entry);
	}
	parent->LinkEndChild(node_FMPUnk5Entrys);


	






	TiXmlElement* node_FMPDep1 = new TiXmlElement("Dep1");
	node_FMPDep1->SetAttribute("count", hdr->num_dep1);
	
	name_offset = (uint32_t*)GetOffsetPtr(buf, hdr->dep1_table_offset);
	for (size_t i = 0; i < hdr->num_dep1; i++)
	{
		name = std::string((char *)GetOffsetPtr(buf, name_offset[i]));
		node = new TiXmlElement("File"); node->SetAttribute("filename", name); node_FMPDep1->LinkEndChild(node);
	}
	
	parent->LinkEndChild(node_FMPDep1);



	
	


	TiXmlElement* node_FMPDep2 = new TiXmlElement("Dep2");
	node_FMPDep2->SetAttribute("count", hdr->num_dep2);
	name_offset = (uint32_t*)GetOffsetPtr(buf, hdr->dep2_table_offset);
	for (size_t i = 0; i < hdr->num_dep2; i++)
	{
		name = std::string((char *)GetOffsetPtr(buf, name_offset[i]));
		node = new TiXmlElement("File"); node->SetAttribute("filename", name); node_FMPDep2->LinkEndChild(node);
	}
	parent->LinkEndChild(node_FMPDep2);

	TiXmlElement* node_FMPDep3 = new TiXmlElement("Dep3");
	node_FMPDep3->SetAttribute("count", hdr->num_dep3);
	name_offset = (uint32_t*)GetOffsetPtr(buf, hdr->dep3_table_offset);
	for (size_t i = 0; i < hdr->num_dep3; i++)
	{
		name = std::string((char *)GetOffsetPtr(buf, name_offset[i]));
		node = new TiXmlElement("File"); node->SetAttribute("filename", name); node_FMPDep3->LinkEndChild(node);
	}
	parent->LinkEndChild(node_FMPDep3);

	TiXmlElement* node_FMPDep4 = new TiXmlElement("Dep4");
	node_FMPDep4->SetAttribute("count", hdr->num_dep4);
	name_offset = (uint32_t*)GetOffsetPtr(buf, hdr->dep4_table_offset);
	for (size_t i = 0; i < hdr->num_dep4; i++)
	{
		name = std::string((char *)GetOffsetPtr(buf, name_offset[i]));
		node = new TiXmlElement("File"); node->SetAttribute("filename", name); node_FMPDep4->LinkEndChild(node);
	}
	parent->LinkEndChild(node_FMPDep4);
}








/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// wxHexEditor coloration ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////




/*-------------------------------------------------------------------------------\
|                             save_Coloration		                             |
\-------------------------------------------------------------------------------*/
void FmpFile::save_Coloration(string filename, bool show_error)
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
void FmpFile::write_Coloration(TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	FMPHeader *hdr = (FMPHeader *)buf;
	if (size < sizeof(FMPHeader) || memcmp(hdr->signature, FMP_SIGNATURE, 4) != 0)
		return;


	std::vector<bool> listBytesAllreadyTagged;
	listBytesAllreadyTagged.resize(size, false);				//to check override of the same byte (overflow)


	size_t idTag = 0;
	size_t offset = 0;
	size_t incSection = 0;
	size_t incParam = 0;

	write_Coloration_Tag("signature", "string", "", offset, 4 * sizeof(char), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
	offset += 4 * sizeof(char);
	write_Coloration_Tag("version", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
	offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_08", "uint32_t", "must be an even number", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_0C", "uint32_t", "Zero", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);	offset += sizeof(uint32_t);
	write_Coloration_Tag("unk0A_offset", "uint32_t", "Offset (always 0x70 because of header Size)", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);	offset += sizeof(uint32_t);
	write_Coloration_Tag("unk0B_offset", "uint32_t", "Offset (always 0xFC)", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("num_unk1", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk1_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("num_unk2", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk2_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("num_unk3", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk3_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("num_unk4", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk4_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("num_unk5", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk5_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("num_dep1", "uint32_t", "=>  dans les Qst et EVe, il n'y en a pas, et dans le BFten.map, il y a des noms ajoute : du style les noms de fichiers, de model, \"Start Frame\" \"Speed Rate\", \"PLAY SOUND 3D\" , etc ...", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("dep1_table_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("num_dep2", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("dep2_table_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("num_dep3", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("dep3_table_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("num_dep4", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("dep4_table_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_60[4]", "uint32_t[4]", "Always 0 (but game code references first two)", offset, 4 * sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
	offset += 4 * sizeof(uint32_t);


	FMPUnk0AEntry *unk0A = (FMPUnk0AEntry*)GetOffsetPtr(buf, hdr->unk0A_offset);
	offset = hdr->unk0A_offset;
	incSection++;
	incParam = 0;

	write_Coloration_Tag("unk_0", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_1", "uint32_t", "always as unk_0", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_2", "uint32_t", "always as unk_0", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_3", "uint32_t", "always 0", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_4", "uint32_t", "always 0", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_5", "uint32_t", "always 0", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_6", "uint32_t", "always as 0xcdcc1cc1", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_7", "uint32_t", "always 4", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_8", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_9", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_10", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_11", "uint32_t", "always 9A 99 99 3E", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_12", "uint32_t", "always 9A 99 99 3E", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_13", "uint32_t", "always 9A 99 99 3E", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_14", "uint32_t", "always 33 33 33 3F", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_15", "uint32_t", "always 33 33 33 3F", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_16", "uint32_t", "always 33 33 33 3F", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_17", "uint32_t", "always 00 00 C8 42", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_18", "uint32_t", "always 00 00 FA 43", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_19", "uint32_t", "always 0", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_20", "uint32_t", "always 0", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_21", "uint32_t", "always 0", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_22", "uint32_t", "always 00 00 80 3F", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_23", "uint32_t", "always 00 00 80 3F", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_24", "uint32_t", "always 00 00 80 3F", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_25", "uint32_t", "always 00 00 C8 42", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_26", "uint32_t", "always 00 00 96 43", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_27", "uint32_t", "always 00 00 96 42", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_28", "uint32_t", "always 00 00 48 42", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_29", "uint32_t", "always 00 00 00 3F", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_30", "uint32_t", "always 00 00 00 3F", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_31", "uint32_t", "always 00 00 00 3F", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_32", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_33", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_34", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0AEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);



	incSection++;
	
	FMPUnk0BEntry *unk0B = (FMPUnk0BEntry*)GetOffsetPtr(buf, hdr->unk0B_offset);
	offset = hdr->unk0B_offset;

	incParam = 0;
	write_Coloration_Tag("unk_0", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_1", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_2", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_3", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_4", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_5", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_6", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_7", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_8", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_9", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_10", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_11", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_12", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_13", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_14", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_15", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_16", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_17", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_18", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_19", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_20", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_21", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_22", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_23", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_24", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_25", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_26", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_27", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_28", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_29", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_30", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_31", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_32", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_33", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_34", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_35", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_36", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_37", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_38", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_39", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_40", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_41", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_42", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_43", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_44", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_45", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_46", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_47", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_48", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_49", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_50", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_51", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_52", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_53", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_54", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	
	

	//start of list of name_offset.
	incSection++;
	offset = hdr->unk0B_offset + 0x100;
	uint32_t* name_offset;
	for (size_t i = 0; i < 51; i++)
	{
		name_offset = (uint32_t*)GetOffsetPtr(buf, offset);

		write_Coloration_Tag("name_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk0BEntry_nameOffsets", parent, idTag++, incSection, i, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		write_Coloration_Tag("name", "string", "", *name_offset, 4 * sizeof(char), "FMPUnk0BEntry_names", parent, idTag++, incSection + 1, i, listBytesAllreadyTagged, (size_t)-1, false);
	}





	FMPUnk1Entry *unk1 = (FMPUnk1Entry*)GetOffsetPtr(buf, hdr->unk1_offset);
	offset = hdr->unk1_offset;
	incSection++;
	for (size_t i = 0; i < hdr->num_unk1; i++)
	{
		incParam = 0;
		write_Coloration_Tag("name_offset", "uint32_t", "TRIGGER_BROAD_PHASE, TRIGGER_NARROW_PHASE, TRIGGER_CONTACT_SOLVER, Default, or some '__X'", offset, sizeof(uint32_t), "FMPUnk1Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_04", "uint32_t", "1, 2, 3 (increment), but at the end we have 0 for Default and '__X'", offset, sizeof(uint32_t), "FMPUnk1Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_08[4]", "float[4]", "for unk_08[0-2] values are in [0, 1.0]. for unk_08[3] = 999936.0, 0.0 or 0.104279 => i'm not sure is a float value", offset, 4 * sizeof(float), "FMPUnk1Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += 4 * sizeof(float);

		//associate name on last block.
		write_Coloration_Tag("name", "string", "", unk1[i].name_offset, 4 * sizeof(char), "FMPUnk1Entry", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}




	FMPUnk2Entry *unk2 = (FMPUnk2Entry*)GetOffsetPtr(buf, hdr->unk2_offset);
	offset = hdr->unk2_offset;
	incSection++;
	for (size_t i = 0; i < hdr->num_unk2; i++)
	{
		incParam = 0;
		write_Coloration_Tag("name_offset", "uint32_t", "Default or some '__X'", offset, sizeof(uint32_t), "FMPUnk2Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_04[6]", "float[6]", "we feel 0 and 1, 2 and 3, 4 and 5, are together. for unk_04[0] = 0.0, 1.0, 2.0 or 3.0. for unk_04[1] is always 1.0. for unk_04[2] = 200.0 or 300.0. for unk_04[3] = 100.0, or 300.0. for unk_04[4] is in [0,2.0]. for unk_04[5] is in [0, 10.0]. ", offset, 6 * sizeof(float), "FMPUnk2Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += 6 * sizeof(float);

		//associate name on last block.
		write_Coloration_Tag("name", "string", "", unk2[i].name_offset, 4 * sizeof(char), "FMPUnk2Entry", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}



	FMPUnk3Entry *unk3 = (FMPUnk3Entry*)GetOffsetPtr(buf, hdr->unk3_offset);
	offset = hdr->unk3_offset;
	incSection++;
	for (size_t i = 0; i < hdr->num_unk3; i++)
	{
		incParam = 0;
		write_Coloration_Tag("name_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk3Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("num_indexes", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk3Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("indexes_offset", "uint32_t", "Points to table of 16 bits integers. Indexes are relative to Unk4Entrys", offset, sizeof(uint32_t), "FMPUnk3Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);

		size_t nbIndex = unk3[i].num_indexes;
		for (size_t j = 0; j < nbIndex; j++)
		{
			incParam = 0;
			size_t offset_b = unk3[i].indexes_offset + j * sizeof(int16_t);

			write_Coloration_Tag("index", "int16_t", "index into Unk4Entrys", offset_b, sizeof(int16_t), "FMPUnk3Entry_Indexes" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j);
		}

		//associate name on last block.
		write_Coloration_Tag("name", "string", "", unk3[i].name_offset, 4 * sizeof(char), "FMPUnk3Entry", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}







	FMPUnk4Entry* unk4 = (FMPUnk4Entry*)GetOffsetPtr(buf, hdr->unk4_offset);
	FMPUnk4Entry_Next* unk4_next;
	FMPUnk4Entry_Next_b* unk4_next_b;
	FMPUnk4Entry_Next_c* unk4_next_c;
	FMPUnk4Entry_Next_d* unk4_next_d;
	FMPUnk4Entry_Next_e* unk4_next_e;
	FMPUnk4Entry_Next_f* unk4_next_f;

	FMPUnk4Entry_Next3* unk4_next3;
	FMPUnk4Entry_Next3_b* unk4_next3_b;
	FMPUnk4Entry_Next3_c* unk4_next3_c;

	FMPUnk4Entry_Next2* unk4_next2;
	FMPUnk4Entry_Next2_b* unk4_next2_b;
	FMPUnk4Entry_Next2_c* unk4_next2_c;
	
	FMPUnk4Entry_Next4* unk4_next4;
	FMPUnk4Entry_Next4_b* unk4_next4_b;
	FMPUnk4Entry_Next4_c* unk4_next4_c;
	FMPUnk4Entry_Next4_d* unk4_next4_d;
	FMPUnk4Entry_Next4_e* unk4_next4_e;

	std::vector<uint32_t> listAllreadyUsedOffset_byUnk4Entrys;		//many Unk4Entry could use the same unk1_offset, unk2_offset, unk3_offset or unk4_offset. So it's break the tools witch look after rewriten tags. So we have to skip if allready see it.

	offset = hdr->unk4_offset;
	incSection += 2;
	for (size_t i = 0; i < hdr->num_unk4; i++)
	{
		incParam = 0;
		write_Coloration_Tag("name_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk4Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_04", "uint32_t", "0, 1, 0x10001 =>  sort of flag. for the right bit, may be 0 for instanced objects and particules. 1 for the rest (so could be a bool 'instanced')", offset, sizeof(uint32_t), "FMPUnk4Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_08", "int16_t", "always 0xffff if unk_04==0. else it 'increment' but not clear (loop, repetition). it could go very high, specialy on free world (-> 0x55e)", offset, sizeof(int16_t), "FMPUnk4Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(int16_t);
		
		write_Coloration_Tag("unk5_index", "int16_t", "Signed, compared against < 0", offset, sizeof(int16_t), "FMPUnk4Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(int16_t);
		write_Coloration_Tag("unk1_offset", "uint32_t", " => To FMPUnk4Entry_Next (0x50*n, n is get somehow from FMPUnk5Entry) (Notice many Unk4Entry could use the same offset)", offset, sizeof(uint32_t), "FMPUnk4Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk2_offset", "uint32_t", " => To FMPUnk4Entry_Next3 (Notice many Unk4Entry could use the same offset)", offset, sizeof(uint32_t), "FMPUnk4Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		
		write_Coloration_Tag("unk_14", "uint32_t", "0 1 or 2, so it's could be a type", offset, sizeof(uint32_t), "FMPUnk4Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk3_offset", "uint32_t", " => Ref to FMPUnk4Entry_Next2 (Notice many Unk4Entry could use the same offset)", offset, sizeof(uint32_t), "FMPUnk4Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk4_offset", "uint32_t", " => Ref to FMPUnk4Entry_Next4 (Notice many Unk4Entry could use the same offset)", offset, sizeof(uint32_t), "FMPUnk4Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);

		write_Coloration_Tag("unk_20", "uint32_t", "only 0x3fffffff, 0x0066cccc, 0x00ff00cc, 0x35ffffff => could be flags", offset, sizeof(uint32_t), "FMPUnk4Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("transformMatrix4x3", "float[12]", "", offset, 12 * sizeof(float), "FMPUnk4Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += 12 * sizeof(float);


		bool isfound = true;
		if (unk4[i].unk1_offset)					//to remove FMPUnk4Entry_Next allready saw
		{
			uint32_t offset_tmp = unk4[i].unk1_offset;
			isfound = false;
			size_t nbOffset = listAllreadyUsedOffset_byUnk4Entrys.size();
			for (size_t j = 0; j < nbOffset; j++)
			{
				if (listAllreadyUsedOffset_byUnk4Entrys.at(j) == offset_tmp)
				{
					isfound = true;
					break;
				}
			}
			if (!isfound)
				listAllreadyUsedOffset_byUnk4Entrys.push_back(offset_tmp);
		}
		
		if((unk4[i].unk1_offset)&&(!isfound))
		{
			unk4_next = (FMPUnk4Entry_Next*)GetOffsetPtr(buf, unk4[i].unk1_offset);
			size_t offset_b = unk4[i].unk1_offset;
			incParam = 0;

			write_Coloration_Tag("count", "uint32_t", "[A] 0, 1 or 2 (may be a type)", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk1_offset", "uint32_t", "=> ref to FMPUnk4Entry_Next_b", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk2_offset", "uint32_t", "=> ref to FMPUnk4Entry_Next_c", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk3_offset", "uint32_t", "=> Ref to FMPUnk4Entry_Next_d", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_2", "uint32_t", "only 0xffff0000, 1 or 0", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_4", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_5", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_6", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_7", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_8", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_9", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_10", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_8b", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_9b", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_10b", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_11b", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_12", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_13", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_14", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);

			if (unk4_next->unk1_offset)
			{
				unk4_next_b = (FMPUnk4Entry_Next_b*)GetOffsetPtr(buf, unk4_next->unk1_offset);
				offset_b = unk4_next->unk1_offset;
				for (size_t j = 0; j < unk4_next->count; j++)
				{
					incParam = 0;
					write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_1", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
				}
			}

			if (unk4_next->unk2_offset)
			{
				unk4_next_c = (FMPUnk4Entry_Next_c*)GetOffsetPtr(buf, unk4_next->unk2_offset);
				offset_b = unk4_next->unk2_offset;
				{
					write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_1", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_2", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_3", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_4", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_5", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_6", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_7", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_8", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_9", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_10", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_11", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_12", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_13", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_14", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_15", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_16", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_17", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_18", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_19", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_20", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
				}
			}

			if (unk4_next->unk3_offset)
			{
				unk4_next_d = (FMPUnk4Entry_Next_d*)GetOffsetPtr(buf, unk4_next->unk3_offset);
				offset_b = unk4_next->unk3_offset;
				{
					write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_d[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_1", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_d[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("count", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_d[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk1_offset", "uint32_t", "=> Ref to FMPUnk4Entry_Next_e", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_d[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);


					if (unk4_next_d->unk1_offset)
					{
						unk4_next_e = (FMPUnk4Entry_Next_e*)GetOffsetPtr(buf, unk4_next_d->unk1_offset);
						offset_b = unk4_next_d->unk1_offset;
						for (size_t j = 0; j < unk4_next_d->count; j++)
						{
							incParam = 0;
							
							write_Coloration_Tag("name_offset", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("count", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk1_offset", "uint32_t", "=> Ref to FMPUnk4Entry_Next_f", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);

							write_Coloration_Tag("name", "string", "", unk4_next_e[j].name_offset, 4 * sizeof(char), "FMPUnk4Entry_Next3_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 5, i, listBytesAllreadyTagged, j, false);

							if (unk4_next_e[j].unk1_offset)
							{
								unk4_next_f = (FMPUnk4Entry_Next_f*)GetOffsetPtr(buf, unk4_next_e[j].unk1_offset);
								size_t offset_c = unk4_next_e[j].unk1_offset;
								for (size_t k = 0; k < unk4_next_e[j].count; k++)
								{
									incParam = 0;

									write_Coloration_Tag("name_offset", "uint32_t", "", offset_c, sizeof(uint32_t), "FMPUnk4Entry_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); offset_c += sizeof(uint32_t);
									write_Coloration_Tag("unk_0", "uint32_t", "if ==6, unk1_offset is a nameOffset. so it's a type.", offset_c, sizeof(uint32_t), "FMPUnk4Entry_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); offset_c += sizeof(uint32_t);
									write_Coloration_Tag("unk1_offset", "uint32_t", "=> Ref to a unique Value. it's a nameOffset if unk_0 == 6", offset_c, sizeof(uint32_t), "FMPUnk4Entry_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); offset_c += sizeof(uint32_t);

									write_Coloration_Tag("name", "string", "", unk4_next_f[k].name_offset, 4 * sizeof(char), "FMPUnk4Entry_Next3_b[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, i, listBytesAllreadyTagged, k, false);

									if (unk4_next_f[k].unk1_offset)
									{
										if (unk4_next_f[k].unk_0 != 6)
										{
											write_Coloration_Tag("unk1_offset", "uint32_t", "=> Ref to a unique Value. it's a nameOffset if unk_0 == 6", unk4_next_f[k].unk1_offset, sizeof(uint32_t), "FMPUnk4Entry_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k);
										}else {
											write_Coloration_Tag("name2 unk1_offset", "string", "=> Ref to a unique Value. it's a nameOffset if unk_0 == 6", unk4_next_f[k].unk1_offset, 4 * sizeof(char), "FMPUnk4Entry_Next3_b[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, i, listBytesAllreadyTagged, k, false);
										}
									}
								}
							}
						}
					}

				}
			}
		}







		isfound = true;
		if (unk4[i].unk2_offset)					//to remove FMPUnk4Entry_Next allready saw
		{
			uint32_t offset_tmp = unk4[i].unk2_offset;
			isfound = false;
			size_t nbOffset = listAllreadyUsedOffset_byUnk4Entrys.size();
			for (size_t j = 0; j < nbOffset; j++)
			{
				if (listAllreadyUsedOffset_byUnk4Entrys.at(j) == offset_tmp)
				{
					isfound = true;
					break;
				}
			}
			if (!isfound)
				listAllreadyUsedOffset_byUnk4Entrys.push_back(offset_tmp);
		}

		if ((unk4[i].unk2_offset)&&(!isfound))
		{
			unk4_next3 = (FMPUnk4Entry_Next3*)GetOffsetPtr(buf, unk4[i].unk2_offset);
			size_t offset_b = unk4[i].unk2_offset;
			incParam = 0;

			write_Coloration_Tag("unk_0", "uint32_t", "[A] 0, 1, 2, 3 or 4 (may be a type)", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next3", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_1", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next3", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("count", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next3", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk1_offset", "uint32_t", "=> ref to FMPUnk4Entry_Next3_b", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next3", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);

			if (unk4_next3->unk1_offset)
			{
				unk4_next3_b = (FMPUnk4Entry_Next3_b*)GetOffsetPtr(buf, unk4_next3->unk1_offset);
				offset_b = unk4_next3->unk1_offset;
				for (size_t j = 0; j < unk4_next3->count; j++)
				{
					incParam = 0;

					write_Coloration_Tag("name_offset", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next3_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next3_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("count", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next3_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk1_offset", "uint32_t", " => ref to FMPUnk4Entry_Next3_c", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next3_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);

					write_Coloration_Tag("name", "string", "", unk4_next3_b[j].name_offset, 4 * sizeof(char), "FMPUnk4Entry_Next3_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, i, listBytesAllreadyTagged, j, false);

					if (unk4_next3_b[j].unk1_offset)
					{
						unk4_next3_c = (FMPUnk4Entry_Next3_c*)GetOffsetPtr(buf, unk4_next3_b[j].unk1_offset);
						size_t offset_c = unk4_next3_b[j].unk1_offset;
						for (size_t k = 0; k < unk4_next3_b[j].count; k++)
						{
							incParam = 0;

							write_Coloration_Tag("name_offset", "uint32_t", "", offset_c, sizeof(uint32_t), "FMPUnk4Entry_Next3_c[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, k); offset_c += sizeof(uint32_t);
							write_Coloration_Tag("unk_0", "uint32_t", "", offset_c, sizeof(uint32_t), "FMPUnk4Entry_Next3_c[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, k); offset_c += sizeof(uint32_t);
							write_Coloration_Tag("name_offset2", "uint32_t", "", offset_c, sizeof(uint32_t), "FMPUnk4Entry_Next3_c[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, k); offset_c += sizeof(uint32_t);

							write_Coloration_Tag("name", "string", "", unk4_next3_c[k].name_offset, 4 * sizeof(char), "FMPUnk4Entry_Next3_c[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 5, i, listBytesAllreadyTagged, k, false);
							write_Coloration_Tag("name2", "string", "", unk4_next3_c[k].name_offset2, 4 * sizeof(char), "FMPUnk4Entry_Next3_c[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 5, i, listBytesAllreadyTagged, k, false);
						}
					}
				}
			}
		}









		isfound = true;
		if (unk4[i].unk3_offset)					//to remove FMPUnk4Entry_Next allready saw
		{
			uint32_t offset_tmp = unk4[i].unk3_offset;
			isfound = false;
			size_t nbOffset = listAllreadyUsedOffset_byUnk4Entrys.size();
			for (size_t j = 0; j < nbOffset; j++)
			{
				if (listAllreadyUsedOffset_byUnk4Entrys.at(j) == offset_tmp)
				{
					isfound = true;
					break;
				}
			}
			if (!isfound)
				listAllreadyUsedOffset_byUnk4Entrys.push_back(offset_tmp);
		}


		if ((unk4[i].unk3_offset) && (!isfound))
		{
			unk4_next2 = (FMPUnk4Entry_Next2*)GetOffsetPtr(buf, unk4[i].unk3_offset);
			size_t offset_b = unk4[i].unk3_offset;
			incParam = 0;

			write_Coloration_Tag("unk1_offset", "uint32_t", "=> ref to FMPUnk4Entry_Next2_b", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_1", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_2", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_3", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_4", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_5", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_6", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_7", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_8", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_9", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_10", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_11", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_12", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			
			if (unk4_next2[0].unk1_offset)
			{
				unk4_next2_b = (FMPUnk4Entry_Next2_b*)GetOffsetPtr(buf, unk4_next2[0].unk1_offset);
				offset_b = unk4_next2[0].unk1_offset;
				incParam = 0;

				write_Coloration_Tag("name_offset", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("unk_1", "uint32_t", "=> type witch influence unk_2, unk_4 unk_11 is a ref offset (if ==2).", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("unk_2", "uint32_t", "=> if unk_1==2, Ref to a FMPUnk4Entry_Next2_c", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("unk_3", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("unk_4", "uint32_t", "=> if unk_1==2, Ref to a FMPUnk4Entry_Next2_c", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("unk_5", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("unk_6", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("unk_7", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("unk_8", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("unk_9", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("unk_10", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("unk_11", "uint32_t", "if unk_1==2, Ref to a FMPUnk4Entry_Next2_c (may be it's another things because don't look like values from others).", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);

				//associate name on last block.
				write_Coloration_Tag("name", "string", "", unk4_next2_b[0].name_offset, 4 * sizeof(char), "FMPUnk4Entry_Next2_b", parent, idTag++, incSection + 7, i, listBytesAllreadyTagged, i, false);

				if (unk4_next2_b[0].unk_1 == 2)
				{
					if (unk4_next2_b[0].unk_2)
					{
						unk4_next2_c = (FMPUnk4Entry_Next2_c*)GetOffsetPtr(buf, unk4_next2_b[0].unk_2);
						offset_b = unk4_next2_b[0].unk_2;
						incParam = 0;

						write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_c[0]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_1", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_c[0]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					}

					if (unk4_next2_b[0].unk_4)
					{
						unk4_next2_c = (FMPUnk4Entry_Next2_c*)GetOffsetPtr(buf, unk4_next2_b[0].unk_4);
						offset_b = unk4_next2_b[0].unk_4;
						incParam = 0;

						write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_c[1]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_1", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_c[1]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					}

					if (unk4_next2_b[0].unk_11)
					{
						unk4_next2_c = (FMPUnk4Entry_Next2_c*)GetOffsetPtr(buf, unk4_next2_b[0].unk_11);
						offset_b = unk4_next2_b[0].unk_11;
						incParam = 0;

						write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_c[2]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_1", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next2_c[2]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					}
				}
			}
		}










		isfound = true;
		if (unk4[i].unk4_offset)					//to remove FMPUnk4Entry_Next allready saw
		{
			uint32_t offset_tmp = unk4[i].unk4_offset;
			isfound = false;
			size_t nbOffset = listAllreadyUsedOffset_byUnk4Entrys.size();
			for (size_t j = 0; j < nbOffset; j++)
			{
				if (listAllreadyUsedOffset_byUnk4Entrys.at(j) == offset_tmp)
				{
					isfound = true;
					break;
				}
			}
			if (!isfound)
				listAllreadyUsedOffset_byUnk4Entrys.push_back(offset_tmp);
		}

		if ((unk4[i].unk4_offset)&&(!isfound))
		{
			unk4_next4 = (FMPUnk4Entry_Next4*)GetOffsetPtr(buf, unk4[i].unk4_offset);
			size_t offset_b = unk4[i].unk4_offset;
			incParam = 0;

			write_Coloration_Tag("count", "uint32_t", "=> count for FMPUnk4Entry_Next4_c", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4", parent, idTag++, incSection + 8, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("count2", "uint32_t", "=> count for FMPUnk4Entry_Next4_b", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4", parent, idTag++, incSection + 8, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);

			write_Coloration_Tag("unk1_offset", "uint32_t", "=> ref to FMPUnk4Entry_Next4_b", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4", parent, idTag++, incSection + 8, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk2_offset", "uint32_t", "=> ref to FMPUnk4Entry_Next4_d", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4", parent, idTag++, incSection + 8, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk3_offset", "uint32_t", "=> ref to FMPUnk4Entry_Next4_c", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4", parent, idTag++, incSection + 8, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			

			if (unk4_next4->unk1_offset)
			{
				unk4_next4_b = (FMPUnk4Entry_Next4_b*)GetOffsetPtr(buf, unk4_next4->unk1_offset);
				offset_b = unk4_next4->unk1_offset;
				for (size_t j = 0; j < unk4_next4->count2; j++)
				{
					incParam = 0;

					write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_1", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_2", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_3", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_4", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_5", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_6", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_7", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_8", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_9", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_10", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_11", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
				}
			}

			if (unk4_next4->unk3_offset)
			{
				unk4_next4_c = (FMPUnk4Entry_Next4_c*)GetOffsetPtr(buf, unk4_next4->unk3_offset);
				offset_b = unk4_next4->unk3_offset;
				for (size_t j = 0; j < unk4_next4->count; j++)
				{
					incParam = 0;

					write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_1", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_2", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_3", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_4", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_5", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_6", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_7", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_8", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
				}
			}

			if (unk4_next4->unk2_offset)
			{
				
				std::vector<uint32_t> adressesToCheck;
				adressesToCheck.push_back(unk4_next4->unk2_offset);

				for (size_t j = 0; j < adressesToCheck.size(); j++)		//the list will grow in process
				{
					offset_b = adressesToCheck.at(j);
					unk4_next4_d = (FMPUnk4Entry_Next4_d*)GetOffsetPtr(buf, offset_b);
					incParam = 0;

					write_Coloration_Tag("unk_0", "uint16_t", "if 0020 -> followed by FMPUnk4Entry_Next4_e else is Indexes", offset_b, sizeof(uint16_t), "FMPUnk4Entry_Next4_d[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint16_t);
					write_Coloration_Tag("count", "uint16_t", "number of followed Index (with padding to match uint32_t)", offset_b, sizeof(uint16_t), "FMPUnk4Entry_Next4_d[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint16_t);

					if (unk4_next4_d->unk_0 == 0x2000)
					{
						unk4_next4_e = (FMPUnk4Entry_Next4_e*)GetOffsetPtr(buf, offset_b);
						incParam = 0;

						write_Coloration_Tag("unk_1", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_2", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_3", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);

						write_Coloration_Tag("unk1_offset", "uint32_t", "for recursive", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk2_offset", "uint32_t", "for recursive", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk3_offset", "uint32_t", "for recursive", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk4_offset", "uint32_t", "for recursive", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);

						write_Coloration_Tag("unk_4", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_5", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_6", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_7", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk4Entry_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);

						if (unk4_next4_e->unk1_offset) adressesToCheck.push_back(unk4_next4_e->unk1_offset);
						if (unk4_next4_e->unk2_offset) adressesToCheck.push_back(unk4_next4_e->unk2_offset);
						if (unk4_next4_e->unk3_offset) adressesToCheck.push_back(unk4_next4_e->unk3_offset);
						if (unk4_next4_e->unk4_offset) adressesToCheck.push_back(unk4_next4_e->unk4_offset);
						
					}else {
						
						for (size_t k = 0; k < unk4_next4_d->count; k++)
						{
							write_Coloration_Tag("unk_2", "uint16_t", "Indexes with padding to Uint32_t", offset_b, sizeof(uint16_t), "FMPUnk4Entry_Next4_d[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, k); offset_b += sizeof(uint16_t);
						}
					}
				}
				


			}

		}



		//associate name on last block.
		write_Coloration_Tag("name", "string", "", unk4[i].name_offset, 4 * sizeof(char), "FMPUnk4Entry", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}







	FMPUnk5Entry *unk5 = (FMPUnk5Entry*)GetOffsetPtr(buf, hdr->unk5_offset);
	FMPUnk5Entry_Box *unk5_Box;
	FMPUnk5Entry_Box_b *unk5_Box_b;
	FMPUnk5Entry_Box_c *unk5_Box_c;
	FMPUnk5Entry_Box_d *unk5_Box_d;
	FMPUnk5Entry_Box_e *unk5_Box_e;
	offset = hdr->unk5_offset;
	incSection += 12;
	for (size_t i = 0; i < hdr->num_unk5; i++)
	{
		incParam = 0;
		write_Coloration_Tag("name_offset", "uint32_t", "sound like FMPUnk4Entry's names, witch do the link between. notice there is more names on Unk4Entry", offset, sizeof(uint32_t), "FMPUnk5Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("count", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk5Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk1_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMPUnk5Entry", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);

		if (unk5[i].unk1_offset)
		{
			uint32_t startOffset_tmp = unk5[i].unk1_offset;

			size_t nbBox = unk5[i].count;
			for (size_t j = 0; j < nbBox; j++)
			{
				incParam = 0;

				uint32_t* nameOffset_tmp = (uint32_t*)GetOffsetPtr(buf, startOffset_tmp);
				if (*nameOffset_tmp >= (uint32_t)size)
				{
					printf("Error : overflow on HitBox name. skipped all next hitbox from Unk5.\n", name.c_str());
					break;						//we can't know witch size in bytes.
				}
				name = std::string((char *)GetOffsetPtr(buf, *nameOffset_tmp));
				write_Coloration_Tag("name", "string", "", *nameOffset_tmp, 4 * sizeof(char), "FMPUnk5Entry_Box", parent, idTag++, incSection + 1, j, listBytesAllreadyTagged, j, false);


				unk5_Box = (FMPUnk5Entry_Box*)GetOffsetPtr(buf, startOffset_tmp);
				uint32_t offset_b = startOffset_tmp;
				startOffset_tmp += sizeof(FMPUnk5Entry_Box);

				write_Coloration_Tag("name_offset", "uint32_t", "BOX, SPHERE, and a lot of names (sometime the same as Unk5Entry's names, but there is more name in this list).", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("child", "uint16_t", "0xffff for null", offset_b, sizeof(uint16_t), "FMPUnk5Entry_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint16_t);
				write_Coloration_Tag("unk_a0", "uint16_t", "only 0 of 0xffff, may be a Bool (may be for size)", offset_b, sizeof(uint16_t), "FMPUnk5Entry_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint16_t);
				write_Coloration_Tag("sibling", "uint16_t", "0xffff for null", offset_b, sizeof(uint16_t), "FMPUnk5Entry_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint16_t);
				write_Coloration_Tag("parent", "uint16_t", "only 0 of 0xffff", offset_b, sizeof(uint16_t), "FMPUnk5Entry_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint16_t);
				write_Coloration_Tag("unk_a1", "uint16_t", "only 0, 1, 2, 3 or 4", offset_b, sizeof(uint16_t), "FMPUnk5Entry_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint16_t);
				write_Coloration_Tag("unk_a2", "uint16_t", "always 0", offset_b, sizeof(uint16_t), "FMPUnk5Entry_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint16_t);

				write_Coloration_Tag("unk1_offset", "uint32_t", "=> ref to FMPUnk5Entry_Box_b", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("count2", "uint32_t", " value in [0, 0x53da], number of FMPUnk5Entry_Box_e", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("unk2_offset", "uint32_t", "=> Ref to FMPUnk5Entry_Box_e", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("count3", "uint32_t", "count of series of uint16_t values (list of Id, may the one of each FMPUnk5Entry_Box_e (same number)). if not null (and other unknowed cases), the next value is the unk3_offset", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
				
				uint32_t unk3_offset = 0;
				if ((name != "BOX") && (name != "SPHERE") && (name != "CYLINDER"))
				{
					unk3_offset = *((uint32_t*)GetOffsetPtr(buf, startOffset_tmp));
					
					startOffset_tmp += sizeof(uint32_t);
					write_Coloration_Tag("unk_5", "uint32_t", "in version Col, is about only unk_5", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Col" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
				}


				
				//if (unk5_Box->unk1_offset == 0x000077B0)		//test Todo a virer
				if((i==1)&&(j==1))
					int aa = 42;
				
				if (unk5_Box->unk1_offset)
				{
					unk5_Box_b = (FMPUnk5Entry_Box_b*)GetOffsetPtr(buf, unk5_Box->unk1_offset);
					uint32_t offset_b = unk5_Box->unk1_offset;
					startOffset_tmp = unk5_Box->unk1_offset + sizeof(FMPUnk5Entry_Box_b);


					write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_b" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk1_offset", "uint32_t", "=> ref to FMPUnk5Entry_Box_c ", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_b" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);

					if (unk5_Box_b->unk1_offset)
					{
						unk5_Box_c = (FMPUnk5Entry_Box_c*)GetOffsetPtr(buf, unk5_Box_b->unk1_offset);
						uint32_t offset_b = unk5_Box_b->unk1_offset;
						startOffset_tmp = unk5_Box_b->unk1_offset + sizeof(FMPUnk5Entry_Box_c);


						write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk1_offset", "uint32_t", "=> ref to a FMPUnk5Entry_Box_d", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);

						write_Coloration_Tag("unk_1", "uint32_t", "always 0", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_2", "uint32_t", "always 0", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("size3", "uint32_t", "size in octets of part started by unk3_offset (seam to be a file inserted)", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk3_offset", "uint32_t", "0 start of a part (seam to be a file inserted)", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk2_offset", "uint32_t", "=> ref to a FMPUnk5Entry_Box_d", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);

						write_Coloration_Tag("unk_5", "uint32_t", "always 0", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_6", "uint32_t", "only 0 or 0x3c23d70a", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_7", "uint32_t", "only 0, 1, 2 or 0x3c23d70a", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);


						if (unk5_Box_c->unk1_offset)
						{
							unk5_Box_d = (FMPUnk5Entry_Box_d*)GetOffsetPtr(buf, unk5_Box_c->unk1_offset);
							uint32_t offset_b = unk5_Box_c->unk1_offset;
							startOffset_tmp = unk5_Box_c->unk1_offset + sizeof(FMPUnk5Entry_Box_d);

							write_Coloration_Tag("unk_0", "uint32_t", "only 0, 1, 2, 3 or 0xb6800000", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_1", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_2", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_3", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_4", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_5", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_6", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_7", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_8", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_9", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_10", "uint32_t", "always 0", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_11", "uint32_t", "always 0", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_12", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_13", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);

						}

						if (unk5_Box_c->unk2_offset)
						{
							unk5_Box_d = (FMPUnk5Entry_Box_d*)GetOffsetPtr(buf, unk5_Box_c->unk2_offset);
							uint32_t offset_b = unk5_Box_c->unk2_offset;
							startOffset_tmp = unk5_Box_c->unk2_offset + sizeof(FMPUnk5Entry_Box_d);

							write_Coloration_Tag("unk_0", "uint32_t", "only 0, 1, 2, 3 or 0xb6800000", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_1", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_2", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_3", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_4", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_5", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_6", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_7", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_8", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_9", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_10", "uint32_t", "always 0", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_11", "uint32_t", "always 0", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_12", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
							write_Coloration_Tag("unk_13", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						}

						if ((unk5_Box_c->unk3_offset) && (unk5_Box_c->size3))
						{
							write_Coloration_Tag("sort of inserted Files", "uint32_t", "", unk5_Box_c->unk3_offset, unk5_Box_c->size3, "FMPUnk5Entry_Box_c__" + std::to_string(i), parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j);
						}
					}
				}

				if (unk5_Box->unk2_offset)
				{
					unk5_Box_e = (FMPUnk5Entry_Box_e*)GetOffsetPtr(buf, unk5_Box->unk2_offset);
					uint32_t offset_b = unk5_Box->unk2_offset;
					//startOffset_tmp = unk5_Box->unk2_offset + sizeof(FMPUnk5Entry_Box_e);		//FMPUnk5Entry_Box_e is out of size grow, so we don't add to startOffset_tmp.

					for (size_t k = 0; k < unk5_Box->count2; k++)
					{
						incParam = 0;

						//todo check the values.
						write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_1", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_2", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_3", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_4", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_5", "uint32_t", "", offset_b, sizeof(uint32_t), "FMPUnk5Entry_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset_b += sizeof(uint32_t);
					}
				}

				if ((unk3_offset) && (unk5_Box->count3))
				{
					uint32_t offset_b = unk3_offset;
					incParam = 0;
					//startOffset_tmp = sizeof(uint16_t) * unk5_Box->count3 + sizeof(unk3_offset);		//FMPUnk5Entry_Box_e is out of size grow, so we don't add to startOffset_tmp.

					for (size_t k = 0; k < unk5_Box->count3; k++)
					{
						write_Coloration_Tag("values", "uint16_t", "", offset_b, sizeof(uint16_t), "FMPUnk5Entry_Box__[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, k); offset_b += sizeof(uint16_t);
					}
				}
			}
		}


		//associate name on last block.
		write_Coloration_Tag("name", "string", "", unk5[i].name_offset, 4 * sizeof(char), "FMPUnk5Entry", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}







	offset = hdr->dep1_table_offset;
	incSection += 2;
	incParam = 0;
	for (size_t i = 0; i < hdr->num_dep1; i++)
	{
		uint32_t name_offset = *(uint32_t*)GetOffsetPtr(buf, offset);

		write_Coloration_Tag("name_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMP_Dep1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		
		
		write_Coloration_Tag("name", "string", "", name_offset, 4 * sizeof(char), "FMP_Dep1", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}

	
	offset = hdr->dep2_table_offset;
	incSection++;
	incParam = 0;
	for (size_t i = 0; i < hdr->num_dep2; i++)
	{
		uint32_t name_offset = *(uint32_t*)GetOffsetPtr(buf, offset);
		write_Coloration_Tag("name_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMP_Dep2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("name", "string", "", name_offset, 4 * sizeof(char), "FMP_Dep2", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}

	offset = hdr->dep3_table_offset;
	incSection++;
	incParam = 0;
	for (size_t i = 0; i < hdr->num_dep3; i++)
	{
		uint32_t name_offset = *(uint32_t*)GetOffsetPtr(buf, offset);
		write_Coloration_Tag("name_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMP_Dep3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("name", "string", "", name_offset, 4 * sizeof(char), "FMP_Dep3", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}

	offset = hdr->dep4_table_offset;
	incSection++;
	incParam = 0;
	for (size_t i = 0; i < hdr->num_dep4; i++)
	{
		uint32_t name_offset = *(uint32_t*)GetOffsetPtr(buf, offset);
		write_Coloration_Tag("name_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMP_Dep4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("name", "string", "", name_offset, 4 * sizeof(char), "FMP_Dep4", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}
}





/*-------------------------------------------------------------------------------\
|                             write_Coloration_Tag			                     |
\-------------------------------------------------------------------------------*/
void FmpFile::write_Coloration_Tag(string paramName, string paramType, string paramComment, size_t startOffset, size_t size, string sectionName, TiXmlElement* parent, size_t idTag, size_t sectionIndex, size_t paramIndex, std::vector<bool> &listBytesAllreadyTagged, size_t sectionIndexInList, bool checkAllreadyTaggued)
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
			printf("Error on tagID %i : overflow %i >= %i.\n", idTag, index, limit);
			continue;
		}

		if (index == 0x11ac)			//for test
			int aa = 42;

		if ((checkAllreadyTaggued) && (listBytesAllreadyTagged.at(index)))
			printf("warning on tagID %i : the byte %i allready taggued, may be a overflow between blocks. Infos : %s. \n", idTag, index, (sectionName + ((sectionIndexInList != (size_t)-1) ? "[" + std::to_string(sectionIndexInList) + "]" : "") + "." + paramName + " (" + paramType + ") : " + paramComment).c_str() );

		listBytesAllreadyTagged.at(index) = true;
	}
}


}