#include "Emp.h"

namespace LibXenoverse
{

/*-------------------------------------------------------------------------------\
|                             Emp												 |
\-------------------------------------------------------------------------------*/
EMP::EMP()
{
	//section 0 - EMPHeader : black/grey
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#232323");			//background color
	listTagColors.back().back().push_back("#FFFFFF");			//font color (just to possibility read from background color)
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#464646");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 1 - EMPUnk0AEntry : red
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC0000");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC7777");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 2 - EMPUnk0BEntry : green
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#00CC00");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#77CC77");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 3 - EMPUnk1Entry : bleu
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#0000CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7777CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f




	

	//section 5 - EMPUnk2Entry : yellow
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCC00");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCC77");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 6 - EMPUnk3Entry : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#00CCCC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#77CCCC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 7 - EMPUnk3Entry indexes : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#009999");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#339999");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 8 - EMPUnk4Entry : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC00CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC77CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


																
	//section 9 - EMPUnk4Entry_Next : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#990099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#993399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f



	//section 10 - EMPUnk4Entry_Next_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 11 - EMPUnk4Entry_Next_c : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC0099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC3399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 12 - EMPUnk4Entry_Next_d : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900AA");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933AA");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 13 - EMPUnk4Entry_Next_e : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA0099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA3399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f



	//section 14 - EMPUnk4Entry_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#770077");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#773377");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 15 - EMPUnk4Entry_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#550055");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#553355");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f





	//section 16 - EMPUnk4Entry_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA00FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA33FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 17 - EMPUnk4Entry_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 18 - EMPUnk4Entry_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7700FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7733FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 19 - EMPUnk4Entry_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#5500FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#5533FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f




	//section 20 - EMPUnk5Entry : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCCCC");			//bg
	listTagColors.back().back().push_back("#000000");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AAAAAA");			//bg
	listTagColors.back().back().push_back("#000000");			//f


	//section 21 - EMPUnk5Entry_Box : 
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
bool EMP::Load(const uint8_t *buf, size_t size)
{
	Reset();

	EMPHeader *hdr = (EMPHeader *)buf;

	if (size < sizeof(EMPHeader) || memcmp(hdr->signature, EMP_SIGNATURE, 4) != 0)
		return false;

	
	/*
	//Section1 make reference to Section2. so we begin by section2.
	EMP_Section2* section2 = (EMP_Section2*)GetOffsetPtr(buf, hdr->offsetSection2);
	for (size_t i = 0; i < hdr->nbSection2; i++)
	{
		//todo section2[i];
	}

	EMP_Section1* section1 = (EMP_Section1*)GetOffsetPtr(buf, hdr->offsetSection1);
	for (size_t i = 0; i < hdr->nbSection1; i++)
	{
		//todo section1[i];

		//todo link to the section2 by adresss matching		// ( ... seriously )
	}
	*/

	return true;
}

void EMP::Reset()
{

}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Xml First Version for Debug ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*-------------------------------------------------------------------------------\
|                             save_Xml				                             |
\-------------------------------------------------------------------------------*/
void EMP::save_Xml(string filename, bool show_error)
{
	uint8_t *buf;
	size_t size;

	buf = ReadFile(filename, &size, show_error);
	if (!buf)
		return;

	TiXmlDocument *doc = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	doc->LinkEndChild(decl);

	TiXmlElement* root = new TiXmlElement("Emp");
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
void EMP::write_Xml(TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	EMPHeader *hdr = (EMPHeader *)buf;
	if (size < sizeof(EMPHeader) || memcmp(hdr->signature, EMP_SIGNATURE, 4) != 0)
		return;


	std::vector<bool> listBytesAllreadyTagged;
	listBytesAllreadyTagged.resize(size, false);				//to check override of the same byte (overflow)


	//node->SetAttribute("value", UnsignedToString(value, hexadecimal));
	
	TiXmlElement* node_EMPHeader = new TiXmlElement("Header");
	TiXmlElement* node;
	node = new TiXmlElement("numberSection1"); node->SetAttribute("u16", UnsignedToString(hdr->numberSection1, false)); node_EMPHeader->LinkEndChild(node);
	node = new TiXmlElement("numberSection2"); node->SetAttribute("u16", UnsignedToString(hdr->numberSection2, false)); node_EMPHeader->LinkEndChild(node);
	node = new TiXmlElement("offsetSection1"); node->SetAttribute("startOffset", UnsignedToString(hdr->offsetSection1, true)); node_EMPHeader->LinkEndChild(node);
	node = new TiXmlElement("offsetSection2"); node->SetAttribute("startOffset", UnsignedToString(hdr->offsetSection2, true)); node_EMPHeader->LinkEndChild(node);

	/*
	node = new TiXmlElement("unknow3"); node->SetAttribute("u16", UnsignedToString(hdr->unknow3, false)); node_EMPHeader->LinkEndChild(node);
	node = new TiXmlElement("unknow4"); node->SetAttribute("u16", UnsignedToString(hdr->unknow4, false)); node_EMPHeader->LinkEndChild(node);
	node = new TiXmlElement("unknow5"); node->SetAttribute("u16", UnsignedToString(hdr->unknow5, false)); node_EMPHeader->LinkEndChild(node);
	node = new TiXmlElement("unknow6"); node->SetAttribute("u16", UnsignedToString(hdr->unknow6, false)); node_EMPHeader->LinkEndChild(node);
	*/
	parent->LinkEndChild(node_EMPHeader);
	

	TiXmlElement* node_EMPSection1_Root = new TiXmlElement("Section1_Root");
	parent->LinkEndChild(node_EMPSection1_Root);
	


	std::vector<uint32_t> listOffsetForNextSection1;
	std::vector<TiXmlElement*> listOffsetForNextSection1_Parent;

	listOffsetForNextSection1.push_back(hdr->offsetSection1);
	listOffsetForNextSection1_Parent.push_back(node_EMPSection1_Root);


	std::vector<uint32_t> listOffsetfromSection1ToSection2;
	EMP_Section1* section1;
	size_t offset = 0;
	size_t offset_tmp = 0;
	for (size_t i = 0; i < listOffsetForNextSection1.size(); i++)			//listOffsetForNextSection1.size could increase
	{
		if (listOffsetForNextSection1.at(i) == 0)
			continue;

		section1 = (EMP_Section1*)GetOffsetPtr(buf, listOffsetForNextSection1.at(i));
		offset = listOffsetForNextSection1.at(i);

		TiXmlElement* node_EMPSection1 = new TiXmlElement("Section1");
		listOffsetForNextSection1_Parent.at(i)->LinkEndChild(node_EMPSection1);

		TiXmlElement* node_EMPSection1_Child = new TiXmlElement("Child");
		

		


		//add the 2 next Section1.
		offset_tmp = (*section1).offsetSiblingSection1;
		if (offset_tmp)
		{
			listOffsetForNextSection1.push_back(offset_tmp + listOffsetForNextSection1.at(i));
			listOffsetForNextSection1_Parent.push_back(listOffsetForNextSection1_Parent.at(i));			//same parent for sibling
		}

		offset_tmp = (*section1).offsetChildSection1;
		if (offset_tmp)
		{
			listOffsetForNextSection1.push_back(offset_tmp + listOffsetForNextSection1.at(i));
			listOffsetForNextSection1_Parent.push_back(node_EMPSection1_Child);			//this for child's parent
		}
		

		node = new TiXmlElement("name"); node->SetAttribute("string", string(section1[0].name)); node_EMPSection1->LinkEndChild(node);
		
		node = new TiXmlElement("unk_0"); node->SetAttribute("u8", UnsignedToString(section1[0].unk_0, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_0b"); node->SetAttribute("u8", UnsignedToString(section1[0].unk_0b, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_1"); node->SetAttribute("u16", UnsignedToString(section1[0].unk_1, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("section3_type"); node->SetAttribute("u8", UnsignedToString(section1[0].section3_type, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("section3_type_b"); node->SetAttribute("u8", UnsignedToString(section1[0].section3_type_b, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_3"); node->SetAttribute("u16", UnsignedToString(section1[0].unk_3, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_4"); node->SetAttribute("u8", UnsignedToString(section1[0].unk_4, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_4b"); node->SetAttribute("u8", UnsignedToString(section1[0].unk_4b, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_5"); node->SetAttribute("u16", UnsignedToString(section1[0].unk_5, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_6"); node->SetAttribute("u8", UnsignedToString(section1[0].unk_6, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_6b"); node->SetAttribute("u8", UnsignedToString(section1[0].unk_6b, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_7"); node->SetAttribute("u8", UnsignedToString(section1[0].unk_7, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_7b"); node->SetAttribute("u8", UnsignedToString(section1[0].unk_7b, true)); node_EMPSection1->LinkEndChild(node);


		node = new TiXmlElement("unk_8"); node->SetAttribute("u32", UnsignedToString(section1[0].unk_8, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_9"); node->SetAttribute("u16", UnsignedToString(section1[0].unk_9, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_9b"); node->SetAttribute("u16", UnsignedToString(section1[0].unk_9b, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_10"); node->SetAttribute("u32", UnsignedToString(section1[0].unk_10, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_11"); node->SetAttribute("u32", UnsignedToString(section1[0].unk_11, true)); node_EMPSection1->LinkEndChild(node);

		node = new TiXmlElement("unk_12"); node->SetAttribute("float", std::to_string(section1[0].unk_12)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_13"); node->SetAttribute("float", std::to_string(section1[0].unk_13)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_14"); node->SetAttribute("float", std::to_string(section1[0].unk_14)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_15"); node->SetAttribute("u32", UnsignedToString(section1[0].unk_15, true)); node_EMPSection1->LinkEndChild(node);
		
		node = new TiXmlElement("unk_16"); node->SetAttribute("float", std::to_string(section1[0].unk_16)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_17"); node->SetAttribute("float", std::to_string(section1[0].unk_17)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_18"); node->SetAttribute("float", std::to_string(section1[0].unk_18)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_19"); node->SetAttribute("u32", UnsignedToString(section1[0].unk_19, true)); node_EMPSection1->LinkEndChild(node);

		node = new TiXmlElement("unk_20"); node->SetAttribute("float", std::to_string(section1[0].unk_20)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_21"); node->SetAttribute("float", std::to_string(section1[0].unk_21)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_22"); node->SetAttribute("float", std::to_string(section1[0].unk_22)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_23"); node->SetAttribute("float", std::to_string(section1[0].unk_23)); node_EMPSection1->LinkEndChild(node);

		node = new TiXmlElement("unk_24"); node->SetAttribute("float", std::to_string(section1[0].unk_24)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_24b"); node->SetAttribute("float", std::to_string(section1[0].unk_24b)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_25"); node->SetAttribute("float", std::to_string(section1[0].unk_25)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_25b"); node->SetAttribute("u32", UnsignedToString(section1[0].unk_25b, true)); node_EMPSection1->LinkEndChild(node);

		node = new TiXmlElement("unk_26"); node->SetAttribute("u32", UnsignedToString(section1[0].unk_26, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_27"); node->SetAttribute("float", std::to_string(section1[0].unk_27)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_28"); node->SetAttribute("u16", UnsignedToString(section1[0].unk_28, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("numberSection4"); node->SetAttribute("u16", UnsignedToString(section1[0].numberSection4, false)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("offsetSection4"); node->SetAttribute("offset", UnsignedToString(section1[0].offsetSection4, true)); node_EMPSection1->LinkEndChild(node);

		node = new TiXmlElement("numberSection5"); node->SetAttribute("u16", UnsignedToString(section1[0].numberSection5, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("numberChildSection1"); node->SetAttribute("u16", UnsignedToString(section1[0].numberChildSection1, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("offsetSection5"); node->SetAttribute("offset", UnsignedToString(section1[0].offsetSection5, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("offsetNextSection1"); node->SetAttribute("offset", UnsignedToString(section1[0].offsetSiblingSection1, true)); node_EMPSection1->LinkEndChild(node);
		node = new TiXmlElement("offsetNextSection1_b"); node->SetAttribute("offset", UnsignedToString(section1[0].offsetChildSection1, true)); node_EMPSection1->LinkEndChild(node);

		



		//extended parts
		offset += sizeof(EMP_Section1);

		size_t offset_Section3 = offset;



		//Texture part
		if ((section1[0].section3_type_b == 0x02))
		{
			TiXmlElement* node_EMPSection3 = new TiXmlElement("Section3_TexturePart");

			EMP_Section3_TexturePart* textPart = (EMP_Section3_TexturePart*)GetOffsetPtr(buf, offset_Section3);

			node = new TiXmlElement("unk_0"); node->SetAttribute("u16", UnsignedToString(textPart[0].unk_0, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("u16", UnsignedToString(textPart[0].unk_1, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_2a"); node->SetAttribute("u32", UnsignedToString(textPart[0].unk_2a, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("u32", UnsignedToString(textPart[0].unk_2, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_3"); node->SetAttribute("u32", UnsignedToString(textPart[0].unk_3, true)); node_EMPSection3->LinkEndChild(node);

			node = new TiXmlElement("emmValue"); node->SetAttribute("u16", UnsignedToString(textPart[0].emmValue, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_4"); node->SetAttribute("u16", UnsignedToString(textPart[0].unk_4, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("offsetToOffsetToSection2"); node->SetAttribute("offset", UnsignedToString(textPart[0].offsetToOffsetToSection2, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_5"); node->SetAttribute("float", std::to_string(textPart[0].unk_5)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_6"); node->SetAttribute("float", std::to_string(textPart[0].unk_6)); node_EMPSection3->LinkEndChild(node);

			node = new TiXmlElement("unk_7"); node->SetAttribute("float", std::to_string(textPart[0].unk_7)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_8"); node->SetAttribute("float", std::to_string(textPart[0].unk_8)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_9"); node->SetAttribute("float", std::to_string(textPart[0].unk_9)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_10"); node->SetAttribute("float", std::to_string(textPart[0].unk_10)); node_EMPSection3->LinkEndChild(node);

			node = new TiXmlElement("color0"); node->SetAttribute("R", FloatToString(textPart[0].color0[0])); node->SetAttribute("G", FloatToString(textPart[0].color0[1])); node->SetAttribute("B", FloatToString(textPart[0].color0[2])); node->SetAttribute("A", FloatToString(textPart[0].color0[3]));  node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("color1"); node->SetAttribute("R", FloatToString(textPart[0].color1[0])); node->SetAttribute("G", FloatToString(textPart[0].color1[1])); node->SetAttribute("B", FloatToString(textPart[0].color1[2])); node->SetAttribute("A", FloatToString(textPart[0].color1[3]));  node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("color2"); node->SetAttribute("R", FloatToString(textPart[0].color2[0])); node->SetAttribute("G", FloatToString(textPart[0].color2[1])); node->SetAttribute("B", FloatToString(textPart[0].color2[2])); node->SetAttribute("A", FloatToString(textPart[0].color2[3]));  node_EMPSection3->LinkEndChild(node);

			node = new TiXmlElement("unk_11"); node->SetAttribute("u32", UnsignedToString(textPart[0].unk_11, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_12"); node->SetAttribute("u32", UnsignedToString(textPart[0].unk_12, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_13"); node->SetAttribute("u32", UnsignedToString(textPart[0].unk_13, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_14"); node->SetAttribute("u32", UnsignedToString(textPart[0].unk_14, true)); node_EMPSection3->LinkEndChild(node);


			node_EMPSection1->LinkEndChild(node_EMPSection3);

		}else {

			//todo look if after other type like 3, 4, etc ...;
		}




		if ((section1[0].section3_type == 0x00) && (section1[0].section3_type_b == 0x01))
		{
			EMP_Section3_00_01* section3 = (EMP_Section3_00_01*)GetOffsetPtr(buf, offset_Section3);

			TiXmlElement* node_EMPSection3 = new TiXmlElement("Section3_00_01");

			node = new TiXmlElement("unk_0"); node->SetAttribute("float", std::to_string(section3[0].unk_0)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("float", std::to_string(section3[0].unk_1)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("float", std::to_string(section3[0].unk_2)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_3"); node->SetAttribute("float", std::to_string(section3[0].unk_3)); node_EMPSection3->LinkEndChild(node);

			node = new TiXmlElement("unk_4"); node->SetAttribute("float", std::to_string(section3[0].unk_4)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_5"); node->SetAttribute("float", std::to_string(section3[0].unk_5)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_6"); node->SetAttribute("u32", UnsignedToString(section3[0].unk_6, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_7"); node->SetAttribute("u32", UnsignedToString(section3[0].unk_7, true)); node_EMPSection3->LinkEndChild(node);

			node_EMPSection1->LinkEndChild(node_EMPSection3);


		}else if ((section1[0].section3_type == 0x01) && (section1[0].section3_type_b == 0x01)) {

			EMP_Section3_01_01* section3 = (EMP_Section3_01_01*)GetOffsetPtr(buf, offset_Section3);

			TiXmlElement* node_EMPSection3 = new TiXmlElement("Section3_01_01");

			node = new TiXmlElement("unk_0"); node->SetAttribute("float", std::to_string(section3[0].unk_0)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("float", std::to_string(section3[0].unk_1)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("float", std::to_string(section3[0].unk_2)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_3"); node->SetAttribute("float", std::to_string(section3[0].unk_3)); node_EMPSection3->LinkEndChild(node);

			node_EMPSection1->LinkEndChild(node_EMPSection3);


		}else if ((section1[0].section3_type == 0x02) && (section1[0].section3_type_b == 0x01)) {

			EMP_Section3_02_01* section3 = (EMP_Section3_02_01*)GetOffsetPtr(buf, offset_Section3);

			TiXmlElement* node_EMPSection3 = new TiXmlElement("Section3_02_01");

			node = new TiXmlElement("unk_0"); node->SetAttribute("float", std::to_string(section3[0].unk_0)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("float", std::to_string(section3[0].unk_1)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("float", std::to_string(section3[0].unk_2)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_3"); node->SetAttribute("float", std::to_string(section3[0].unk_3)); node_EMPSection3->LinkEndChild(node);

			node = new TiXmlElement("unk_4"); node->SetAttribute("float", std::to_string(section3[0].unk_4)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_5"); node->SetAttribute("float", std::to_string(section3[0].unk_5)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_6"); node->SetAttribute("float", std::to_string(section3[0].unk_6)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_7"); node->SetAttribute("float", std::to_string(section3[0].unk_7)); node_EMPSection3->LinkEndChild(node);

			node = new TiXmlElement("unk_8"); node->SetAttribute("float", std::to_string(section3[0].unk_8)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_9"); node->SetAttribute("float", std::to_string(section3[0].unk_9)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_10"); node->SetAttribute("u32", UnsignedToString(section3[0].unk_10, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_11"); node->SetAttribute("float", std::to_string(section3[0].unk_11)); node_EMPSection3->LinkEndChild(node);

			node_EMPSection1->LinkEndChild(node_EMPSection3);


		}else if ((section1[0].section3_type == 0x03) && (section1[0].section3_type_b == 0x01)) {

			EMP_Section3_02_01* section3 = (EMP_Section3_02_01*)GetOffsetPtr(buf, offset_Section3);

			TiXmlElement* node_EMPSection3 = new TiXmlElement("Section3_03_01");

			node = new TiXmlElement("unk_0"); node->SetAttribute("float", std::to_string(section3[0].unk_0)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("float", std::to_string(section3[0].unk_1)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("float", std::to_string(section3[0].unk_2)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_3"); node->SetAttribute("float", std::to_string(section3[0].unk_3)); node_EMPSection3->LinkEndChild(node);

			node = new TiXmlElement("unk_4"); node->SetAttribute("float", std::to_string(section3[0].unk_4)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_5"); node->SetAttribute("float", std::to_string(section3[0].unk_5)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_6"); node->SetAttribute("float", std::to_string(section3[0].unk_6)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_7"); node->SetAttribute("float", std::to_string(section3[0].unk_7)); node_EMPSection3->LinkEndChild(node);

			node = new TiXmlElement("unk_8"); node->SetAttribute("float", std::to_string(section3[0].unk_8)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_9"); node->SetAttribute("float", std::to_string(section3[0].unk_9)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_10"); node->SetAttribute("u32", UnsignedToString(section3[0].unk_10, false)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_11"); node->SetAttribute("float", std::to_string(section3[0].unk_11)); node_EMPSection3->LinkEndChild(node);

			node_EMPSection1->LinkEndChild(node_EMPSection3);







		}else if ((section1[0].section3_type == 0x00) && (section1[0].section3_type_b == 0x02)) {

			EMP_Section3_00_02* section3 = (EMP_Section3_00_02*)GetOffsetPtr(buf, offset_Section3);
			TiXmlElement* node_EMPSection3 = new TiXmlElement("Section3_00_02");
			node = new TiXmlElement("unk_0"); node->SetAttribute("float", std::to_string(section3[0].unk_0)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("float", std::to_string(section3[0].unk_1)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("float", std::to_string(section3[0].unk_2)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_3"); node->SetAttribute("float", std::to_string(section3[0].unk_3)); node_EMPSection3->LinkEndChild(node);
			node_EMPSection1->LinkEndChild(node_EMPSection3);


		// ((section1[0].section3_type == 0x10) && (section1[0].section3_type_b == 0x02)) { is only texturePart.


		}else if ((section1[0].section3_type == 0x02) && (section1[0].section3_type_b == 0x02)) {

			EMP_Section3_02_02* section3 = (EMP_Section3_02_02*)GetOffsetPtr(buf, offset_Section3);
			TiXmlElement* node_EMPSection3 = new TiXmlElement("Section3_02_02");
			node = new TiXmlElement("unk_0"); node->SetAttribute("float", std::to_string(section3[0].unk_0)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("float", std::to_string(section3[0].unk_1)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("float", std::to_string(section3[0].unk_2)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_3"); node->SetAttribute("float", std::to_string(section3[0].unk_3)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_4"); node->SetAttribute("float", std::to_string(section3[0].unk_4)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_5"); node->SetAttribute("float", std::to_string(section3[0].unk_5)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_6"); node->SetAttribute("float", std::to_string(section3[0].unk_6)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_7"); node->SetAttribute("u32", UnsignedToString(section3[0].unk_7, true)); node_EMPSection3->LinkEndChild(node);
			node_EMPSection1->LinkEndChild(node_EMPSection3);



		}else if ((section1[0].section3_type == 0x03) && (section1[0].section3_type_b == 0x02)) {

			EMP_Section3_03_02* section3 = (EMP_Section3_03_02*)GetOffsetPtr(buf, offset_Section3);
			TiXmlElement* node_EMPSection3 = new TiXmlElement("Section3_03_02");
			node = new TiXmlElement("unk_0"); node->SetAttribute("u16", UnsignedToString(section3[0].unk_0, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("u16", UnsignedToString(section3[0].unk_1, false)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("u16", UnsignedToString(section3[0].unk_2, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("lastIndexColors"); node->SetAttribute("u16", UnsignedToString(section3[0].lastIndexColors, false)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_4"); node->SetAttribute("u32", UnsignedToString(section3[0].unk_4, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("offsetSection3_03_02_b"); node->SetAttribute("offset", UnsignedToString(section3[0].offsetSection3_03_02_b, true)); node_EMPSection3->LinkEndChild(node);
			
			

			//extention List Floats
			offset_tmp = section3[0].offsetSection3_03_02_b;
			if(offset_tmp)
			{
				offset_tmp += listOffsetForNextSection1.at(i);
				offset = offset_tmp;

				TiXmlElement* node_EMPSection3_b_Floats = new TiXmlElement("Colors");

				float* section3_b_Floats = (float*)GetOffsetPtr(buf, offset_tmp);

				for (size_t j = 0; j <= (*section3).lastIndexColors; j++)
				{
					node = new TiXmlElement("Color");  node->SetAttribute("R", std::to_string(section3_b_Floats[4 * j + 0])); node->SetAttribute("G", std::to_string(section3_b_Floats[4 * j + 1])); node->SetAttribute("B", std::to_string(section3_b_Floats[4 * j + 2])); node->SetAttribute("A", std::to_string(section3_b_Floats[4 * j + 3])); node_EMPSection3_b_Floats->LinkEndChild(node);
				}
				node_EMPSection3->LinkEndChild(node_EMPSection3_b_Floats);
			}
			
			node_EMPSection1->LinkEndChild(node_EMPSection3);





		}else if ((section1[0].section3_type == 0x04) && (section1[0].section3_type_b == 0x02)) {

			EMP_Section3_04_02* section3 = (EMP_Section3_04_02*)GetOffsetPtr(buf, offset_Section3);
			TiXmlElement* node_EMPSection3 = new TiXmlElement("Section3_04_02");
			node = new TiXmlElement("unk_0"); node->SetAttribute("float", std::to_string(section3[0].unk_0)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("float", std::to_string(section3[0].unk_1)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("float", std::to_string(section3[0].unk_2)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_3"); node->SetAttribute("float", std::to_string(section3[0].unk_3)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_4"); node->SetAttribute("u32", UnsignedToString(section3[0].unk_4, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_5"); node->SetAttribute("u32", UnsignedToString(section3[0].unk_5, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_6"); node->SetAttribute("u32", UnsignedToString(section3[0].unk_6, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_7"); node->SetAttribute("u32", UnsignedToString(section3[0].unk_7, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_8"); node->SetAttribute("u32", UnsignedToString(section3[0].unk_8, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("offsetEmg"); node->SetAttribute("offset", UnsignedToString(section3[0].offsetEmg, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_9"); node->SetAttribute("u32", UnsignedToString(section3[0].unk_9, true)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_10"); node->SetAttribute("u32", UnsignedToString(section3[0].unk_10, true)); node_EMPSection3->LinkEndChild(node);
			
			
			//Todo Emg conversion emd , convertion to Xml
			offset_tmp = section3[0].offsetEmg;
			if(offset_tmp)
			{
				offset_tmp += listOffsetForNextSection1.at(i);
				offset = offset_tmp;

				//test extract Emg, todo make a conversion for emd and after in Xml.  (think it could be better to extract into emd file, but we have to keep the name , and hierarchy).
				static size_t emgIndex = 0;

				LibXenoverse::EMG* emg = new LibXenoverse::EMG();
				emg->Load((uint8_t*)GetOffsetPtr(buf, offset), 0, 0);

				LibXenoverse::EMDMesh* emdMesh = new LibXenoverse::EMDMesh();
				emg->writeEmdMesh(emdMesh, emg->getName());

				TiXmlElement* node_EmdMesh = emdMesh->exportXml();
				node_EMPSection3->LinkEndChild(node_EmdMesh);

				delete emg;
				delete emdMesh;
			}
			
			node_EMPSection1->LinkEndChild(node_EMPSection3);


		}else if ((section1[0].section3_type == 0x05) && (section1[0].section3_type_b == 0x02)) {

			EMP_Section3_05_02* section3 = (EMP_Section3_05_02*)GetOffsetPtr(buf, offset_Section3);

			TiXmlElement* node_EMPSection3 = new TiXmlElement("Section3_05_02");

			node = new TiXmlElement("unk_0"); node->SetAttribute("float", std::to_string(section3[0].unk_0)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("float", std::to_string(section3[0].unk_1)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("float", std::to_string(section3[0].unk_2)); node_EMPSection3->LinkEndChild(node);
			node = new TiXmlElement("unk_3"); node->SetAttribute("float", std::to_string(section3[0].unk_3)); node_EMPSection3->LinkEndChild(node);



			//extension EMP_Section3_05_02_b
			offset = offset_Section3 + sizeof(EMP_Section3_05_02);
			{
				EMP_Section3_05_02_b* section3_b = (EMP_Section3_05_02_b*)GetOffsetPtr(buf, offset);

				TiXmlElement* node_EMPSection3_b = new TiXmlElement("Section3_05_02_b");

				node = new TiXmlElement("lastIndexForFloatGroups"); node->SetAttribute("u16", UnsignedToString(section3_b[0].lastIndexForFloatGroups, true)); node_EMPSection3->LinkEndChild(node);
				node = new TiXmlElement("numberFloatByGroup"); node->SetAttribute("u16", UnsignedToString(section3_b[0].numberFloatByGroup, true)); node_EMPSection3->LinkEndChild(node);
				node = new TiXmlElement("offsetSection3_05_02_c"); node->SetAttribute("offset", UnsignedToString(section3_b[0].offsetSection3_05_02_c, true)); node_EMPSection3->LinkEndChild(node);
				
				{
					TiXmlElement* node_EMPSection3_b_Floats = new TiXmlElement("Floats");

					//extention List Floats
					offset_tmp = (*section3_b).offsetSection3_05_02_c;
					if (offset_tmp)
					{
						offset = offset_tmp + listOffsetForNextSection1.at(i);

						float* section3_b_Floats = (float*)GetOffsetPtr(buf, offset_tmp);

						for (size_t j = 0; j <= (*section3_b).lastIndexForFloatGroups; j++)
						{
							for (size_t k = 0; k < (*section3_b).numberFloatByGroup; k++)
							{
								node = new TiXmlElement("Float"); node->SetAttribute("f32", std::to_string(section3_b_Floats[(*section3_b).numberFloatByGroup * j +  k])); node_EMPSection3_b_Floats->LinkEndChild(node);
							}
						}
					}

					node_EMPSection3_b->LinkEndChild(node_EMPSection3_b_Floats);
				}
				
				node_EMPSection3->LinkEndChild(node_EMPSection3_b);

			}

			node_EMPSection1->LinkEndChild(node_EMPSection3);
		}

	


		if ((section1[0].section3_type_b == 0x02))
		{
			EMP_Section3_TexturePart* textPart = (EMP_Section3_TexturePart*)GetOffsetPtr(buf, offset_Section3);

			//extention link to Section2.
			offset_tmp = (*textPart).offsetToOffsetToSection2;
			if (offset_tmp)
			{
				offset_tmp += listOffsetForNextSection1.at(i);
				offset = offset_tmp;

				uint32_t* section3_b = (uint32_t*)GetOffsetPtr(buf, offset);

				offset_tmp = section3_b[0];				// Todo test if EMP_Section3_05_02.unk_4 != 1 => more than one refence
				if (offset_tmp)
				{
					offset_tmp += listOffsetForNextSection1.at(i);
				
					listOffsetfromSection1ToSection2.push_back((*((uint32_t*)GetOffsetPtr(buf, offset_tmp))));			// we will made the link on header of Section2
				}

			}
		}





		//Section4
		offset_tmp = (*section1).offsetSection4;
		if (offset_tmp)
		{
			TiXmlElement* node_EMPSection4_list = new TiXmlElement("Section4_list");
			
			size_t offset_section4_declaration = listOffsetForNextSection1.at(i) + 0x88;
			size_t startSection4_series = offset_tmp;

			for (size_t j = 0; j < (*section1).numberSection4; j++)
			{
				offset = startSection4_series + offset_section4_declaration + j * sizeof(EMP_Section4);

				size_t offset_Section4 = offset;
				EMP_Section4* section4 = (EMP_Section4*)GetOffsetPtr(buf, offset_Section4);

				TiXmlElement* node_EMPSection4 = new TiXmlElement("Section4");

				node = new TiXmlElement("unk_0"); node->SetAttribute("u8", UnsignedToString(section4[0].unk_0, true)); node_EMPSection4->LinkEndChild(node);
				node = new TiXmlElement("unk_1"); node->SetAttribute("u8", UnsignedToString(section4[0].unk_1, true)); node_EMPSection4->LinkEndChild(node);
				node = new TiXmlElement("unk_2"); node->SetAttribute("u16", UnsignedToString(section4[0].unk_2, true)); node_EMPSection4->LinkEndChild(node);
				node = new TiXmlElement("unk_3"); node->SetAttribute("u16", UnsignedToString(section4[0].unk_3, true)); node_EMPSection4->LinkEndChild(node);
				node = new TiXmlElement("unk_4"); node->SetAttribute("u16", UnsignedToString(section4[0].unk_4, true)); node_EMPSection4->LinkEndChild(node);
				
				node = new TiXmlElement("numberIndexIntoSection4_b"); node->SetAttribute("u16", UnsignedToString(section4[0].numberIndexIntoSection4_b, false)); node_EMPSection4->LinkEndChild(node);
				node = new TiXmlElement("numberFloatIntoSection4_b"); node->SetAttribute("u16", UnsignedToString(section4[0].numberFloatIntoSection4_b, false)); node_EMPSection4->LinkEndChild(node);
				node = new TiXmlElement("offsetSection4_b"); node->SetAttribute("offset", UnsignedToString(section4[0].offsetSection4_b, true)); node_EMPSection4->LinkEndChild(node);


				//Section4_b
				offset_tmp = (*section4).offsetSection4_b;
				if (offset_tmp)
					offset_tmp += offset_Section4;

				if (offset_tmp)
				{
					offset = offset_tmp;

					size_t offset_Section4_b = offset;
					uint32_t* section4_b = (uint32_t*)GetOffsetPtr(buf, offset_Section4_b);

					

					std::vector<uint16_t> keyframeTimeIndex;
					uint16_t* section4_b_uint16 = (uint16_t*)GetOffsetPtr(buf, offset_Section4_b);

					for (size_t k = 0; k < (*section4).numberFloatIntoSection4_b; k++)
					{
						keyframeTimeIndex.push_back(section4_b_uint16[k]);
						offset += sizeof(uint16_t);
					}
					if(offset % sizeof(uint32_t) != 0)
						offset += sizeof(uint16_t);


					std::vector<float> keyframeValues;
					float* floatlist = (float*)GetOffsetPtr(buf, offset);

					for (size_t k = 0; k < (*section4).numberFloatIntoSection4_b; k++)
						keyframeValues.push_back(floatlist[k]);
					


					TiXmlElement* node_EMPSection4_b = new TiXmlElement("Section4_b");

					for (size_t k = 0; k < (*section4).numberFloatIntoSection4_b; k++)
					{
						node = new TiXmlElement("Keyframe"); node->SetAttribute("index", std::to_string(keyframeTimeIndex.at(k))); node->SetAttribute("value", std::to_string(keyframeValues.at(k))); node_EMPSection4_b->LinkEndChild(node);
					}

					node_EMPSection4->LinkEndChild(node_EMPSection4_b);
				}


				node_EMPSection4_list->LinkEndChild(node_EMPSection4);
			}
			node_EMPSection1->LinkEndChild(node_EMPSection4_list);
		}





		//Section5
		offset_tmp = (*section1).offsetSection5;
		if (offset_tmp)
		{
			size_t startSection5_seeries = offset_tmp + listOffsetForNextSection1.at(i);

			TiXmlElement* node_EMPSection5_list = new TiXmlElement("Section5_list");

			for (size_t k = 0; k < (*section1).numberSection5; k++)
			{

				offset = startSection5_seeries + k * sizeof(EMP_Section5);

				size_t offset_Section5 = offset;
				EMP_Section5* section5 = (EMP_Section5*)GetOffsetPtr(buf, offset_Section5);

				TiXmlElement* node_EMPSection5 = new TiXmlElement("Section5");

				node = new TiXmlElement("unk_0"); node->SetAttribute("u16", UnsignedToString(section5[0].unk_0, true)); node_EMPSection5->LinkEndChild(node);
				node = new TiXmlElement("numberSection5_b"); node->SetAttribute("u16", UnsignedToString(section5[0].numberSection5_b, false)); node_EMPSection5->LinkEndChild(node);
				node = new TiXmlElement("offsetSection5_b"); node->SetAttribute("offset", UnsignedToString(section5[0].offsetSection5_b, true)); node_EMPSection5->LinkEndChild(node);


				offset_tmp = (*section5).offsetSection5_b;
				if (offset_tmp)
				{
					TiXmlElement* node_EMPSection5_b_list = new TiXmlElement("Section5_b_list");
					size_t startSection5_b_series = offset_tmp;

					for (size_t j = 0; j < (*section5).numberSection5_b; j++)
					{
						offset = startSection5_b_series + offset_Section5 + j * sizeof(EMP_Section5_b);

						size_t offset_Section5_b = offset;
						EMP_Section5_b* section5_b = (EMP_Section5_b*)GetOffsetPtr(buf, offset_Section5_b);


						TiXmlElement* node_EMPSection5_b = new TiXmlElement("Section5_b");

						node = new TiXmlElement("unk_0"); node->SetAttribute("u8", UnsignedToString(section5_b[0].unk_0, true)); node_EMPSection5_b->LinkEndChild(node);
						node = new TiXmlElement("unk_1"); node->SetAttribute("u8", UnsignedToString(section5_b[0].unk_1, true)); node_EMPSection5_b->LinkEndChild(node);
						node = new TiXmlElement("unk_2"); node->SetAttribute("u16", UnsignedToString(section5_b[0].unk_2, true)); node_EMPSection5_b->LinkEndChild(node);
						node = new TiXmlElement("unk_3"); node->SetAttribute("u16", UnsignedToString(section5_b[0].unk_3, true)); node_EMPSection5_b->LinkEndChild(node);
						node = new TiXmlElement("unk_4"); node->SetAttribute("u16", UnsignedToString(section5_b[0].unk_4, true)); node_EMPSection5_b->LinkEndChild(node);

						node = new TiXmlElement("numberIndexIntoSection5_c"); node->SetAttribute("u16", UnsignedToString(section5_b[0].numberIndexIntoSection5_c, false)); node_EMPSection5_b->LinkEndChild(node);
						node = new TiXmlElement("numberFloatIntoSection5_c"); node->SetAttribute("u16", UnsignedToString(section5_b[0].numberFloatIntoSection5_c, false)); node_EMPSection5_b->LinkEndChild(node);
						node = new TiXmlElement("offsetSection5_c"); node->SetAttribute("offset", UnsignedToString(section5_b[0].offsetSection5_c, true)); node_EMPSection5_b->LinkEndChild(node);


						//Section5_c
						offset_tmp = (*section5_b).offsetSection5_c;
						if (offset_tmp)
							offset_tmp += offset_Section5;

						if (offset_tmp)
						{
							offset = offset_tmp;

							size_t offset_Section5_c = offset;
							uint32_t* section5_c = (uint32_t*)GetOffsetPtr(buf, offset_Section5_c);




							std::vector<uint16_t> keyframeTimeIndex;
							uint16_t* section5_c_uint16 = (uint16_t*)GetOffsetPtr(buf, offset_Section5_c);

							for (size_t k = 0; k < (*section5_b).numberFloatIntoSection5_c; k++)
							{
								keyframeTimeIndex.push_back(section5_c_uint16[k]);
								offset += sizeof(uint16_t);
							}
							if (offset % sizeof(uint32_t) != 0)
								offset += sizeof(uint16_t);


							std::vector<float> keyframeValues;
							float* floatlist = (float*)GetOffsetPtr(buf, offset);

							for (size_t k = 0; k < (*section5_b).numberFloatIntoSection5_c; k++)
								keyframeValues.push_back(floatlist[k]);



							TiXmlElement* node_EMPSection5_c = new TiXmlElement("Section5_c");

							for (size_t k = 0; k < (*section5_b).numberFloatIntoSection5_c; k++)
							{
								node = new TiXmlElement("Keyframe"); node->SetAttribute("index", std::to_string(keyframeTimeIndex.at(k))); node->SetAttribute("value", std::to_string(keyframeValues.at(k))); node_EMPSection5_c->LinkEndChild(node);
							}

							node_EMPSection5_b->LinkEndChild(node_EMPSection5_c);


						}

						node_EMPSection5_b_list->LinkEndChild(node_EMPSection5_b);
					}

					node_EMPSection5->LinkEndChild(node_EMPSection5_b_list);

				}

				node_EMPSection5_list->LinkEndChild(node_EMPSection5);
			}

			node_EMPSection1->LinkEndChild(node_EMPSection5_list);
		}

		node_EMPSection1->LinkEndChild(node_EMPSection1_Child);
	}


	
	EMP_Section2* section2 = (EMP_Section2*)GetOffsetPtr(buf, hdr->offsetSection2);
	for (size_t i = 0; i < hdr->numberSection2; i++)
	{
		size_t startSection2 = hdr->offsetSection2 + i * sizeof(EMP_Section2);
		offset = startSection2;

		string linkFromSection1 = "";
		size_t nbLink = listOffsetfromSection1ToSection2.size();
		for (size_t j = 0; j < nbLink; j++)
		{
			if (listOffsetfromSection1ToSection2.at(j) == offset)
				linkFromSection1 += ((linkFromSection1.size() != 0) ? "," : "") + std::to_string(j);
		}
		
		
		TiXmlElement* node_EMPSection2 = new TiXmlElement("Section2");
		node_EMPSection2->SetAttribute("linkToSection1", string(linkFromSection1));
		
		node = new TiXmlElement("unk_0"); node->SetAttribute("u8", UnsignedToString(section2[i].unk_0, true)); node_EMPSection2->LinkEndChild(node);
		node = new TiXmlElement("EmbTextureId"); node->SetAttribute("u8", UnsignedToString(section2[i].EmbTextureId, false)); node_EMPSection2->LinkEndChild(node);
		node = new TiXmlElement("unk_0b"); node->SetAttribute("u8", UnsignedToString(section2[i].unk_0b, true)); node_EMPSection2->LinkEndChild(node);

		node = new TiXmlElement("unk_1"); node->SetAttribute("u8", UnsignedToString(section2[i].unk_1, true)); node_EMPSection2->LinkEndChild(node);
		node = new TiXmlElement("unk_2"); node->SetAttribute("u8", UnsignedToString(section2[i].unk_2, true)); node_EMPSection2->LinkEndChild(node);
		node = new TiXmlElement("unk_3"); node->SetAttribute("u8", UnsignedToString(section2[i].unk_3, true)); node_EMPSection2->LinkEndChild(node);
		node = new TiXmlElement("unk_3b"); node->SetAttribute("u8", UnsignedToString(section2[i].unk_3b, true)); node_EMPSection2->LinkEndChild(node);

		node = new TiXmlElement("unk_4"); node->SetAttribute("u8", UnsignedToString(section2[i].unk_4, true)); node_EMPSection2->LinkEndChild(node);
		node = new TiXmlElement("unk_5"); node->SetAttribute("u8", UnsignedToString(section2[i].unk_5, true)); node_EMPSection2->LinkEndChild(node);
		node = new TiXmlElement("typeSection2_b"); node->SetAttribute("u16", UnsignedToString(section2[i].typeSection2_b, false)); node_EMPSection2->LinkEndChild(node);


		if (section2[i].typeSection2_b == 0)
		{
			EMP_Section2_b_00* section2b = (EMP_Section2_b_00*)GetOffsetPtr(buf, startSection2 + 0xC);
			TiXmlElement* node_EMPSection2b = new TiXmlElement("Section2b_00");
			node = new TiXmlElement("color"); node->SetAttribute("R", FloatToString(section2b[0].color[0])); node->SetAttribute("G", FloatToString(section2b[0].color[1])); node->SetAttribute("B", FloatToString(section2b[0].color[2])); node->SetAttribute("A", FloatToString(section2b[0].color[3]));  node_EMPSection2b->LinkEndChild(node);
			node_EMPSection2->LinkEndChild(node_EMPSection2b);

		}else if (section2[i].typeSection2_b == 1) {

			EMP_Section2_b_01* section2b = (EMP_Section2_b_01*)GetOffsetPtr(buf, startSection2 + 0xC);
			TiXmlElement* node_EMPSection2b = new TiXmlElement("Section2b_01");
			node = new TiXmlElement("color"); node->SetAttribute("R", FloatToString(section2b[0].color[0])); node->SetAttribute("G", FloatToString(section2b[0].color[1])); node->SetAttribute("B", FloatToString(section2b[0].color[2])); node->SetAttribute("A", FloatToString(section2b[0].color[3]));  node_EMPSection2b->LinkEndChild(node);
			node_EMPSection2->LinkEndChild(node_EMPSection2b);


		}else if (section2[i].typeSection2_b == 2) {

			EMP_Section2_b_02* section2b = (EMP_Section2_b_02*)GetOffsetPtr(buf, startSection2 + 0xC);
			TiXmlElement* node_EMPSection2b = new TiXmlElement("Section2b_02");
			
			node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(section2b[0].unk_0, true)); node_EMPSection2b->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(section2b[0].unk_1, true)); node_EMPSection2b->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("u16", UnsignedToString(section2b[0].unk_2, true)); node_EMPSection2b->LinkEndChild(node);
			node = new TiXmlElement("numberSection2_02_c"); node->SetAttribute("u16", UnsignedToString(section2b[0].numberSection2_02_c, false)); node_EMPSection2b->LinkEndChild(node);
			node = new TiXmlElement("offsetSection2_02_c"); node->SetAttribute("offset", UnsignedToString(section2b[0].offsetSection2_02_c, true)); node_EMPSection2b->LinkEndChild(node);


			offset_tmp = section2b[0].offsetSection2_02_c;
			if (offset_tmp)
			{
				offset_tmp += startSection2 + 0xC;
				size_t startSection2c = offset_tmp;
				EMP_Section2_b_02_c* section2c = (EMP_Section2_b_02_c*)GetOffsetPtr(buf, startSection2c);


				TiXmlElement* node_EMPSection2c_list = new TiXmlElement("Section2b_02_c_list");

				for (size_t j = 0; j < section2b[0].numberSection2_02_c; j++)
				{
					TiXmlElement* node_EMPSection2c = new TiXmlElement("Section2b_02_c");

					node = new TiXmlElement("unk_0"); node->SetAttribute("u16", UnsignedToString(section2c[j].unk_0, true)); node_EMPSection2c->LinkEndChild(node);
					node = new TiXmlElement("unk_1"); node->SetAttribute("u16", UnsignedToString(section2c[j].unk_1, true)); node_EMPSection2c->LinkEndChild(node);
					node = new TiXmlElement("color"); node->SetAttribute("R", FloatToString(section2c[j].color[0])); node->SetAttribute("G", FloatToString(section2c[j].color[1])); node->SetAttribute("B", FloatToString(section2c[j].color[2])); node->SetAttribute("A", FloatToString(section2c[j].color[3]));  node_EMPSection2c->LinkEndChild(node);

					node_EMPSection2c_list->LinkEndChild(node_EMPSection2c);
				}

				node_EMPSection2b->LinkEndChild(node_EMPSection2c_list);
			}
			
			node_EMPSection2->LinkEndChild(node_EMPSection2b);
		}
		parent->LinkEndChild(node_EMPSection2);
	}
}








/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// wxHexEditor coloration ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////




/*-------------------------------------------------------------------------------\
|                             save_Coloration		                             |
\-------------------------------------------------------------------------------*/
void EMP::save_Coloration(string filename, bool show_error)
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
void EMP::write_Coloration(TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	EMPHeader *hdr = (EMPHeader *)buf;
	if (size < sizeof(EMPHeader) || memcmp(hdr->signature, EMP_SIGNATURE, 4) != 0)
		return;


	std::vector<bool> listBytesAllreadyTagged;
	listBytesAllreadyTagged.resize(size, false);				//to check override of the same byte (overflow)


	size_t idTag = 0;
	size_t offset = 0;
	size_t incSection = 0;
	size_t incParam = 0;

	write_Coloration_Tag("signature", "string", "", offset, 4 * sizeof(char), "EMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);	offset += 4 * sizeof(char);
	write_Coloration_Tag("endian", "uint16_t", "", offset, sizeof(uint16_t), "EMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("version", "uint32_t", "", offset, sizeof(uint32_t), "EMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_00", "uint16_t", "always 0", offset, sizeof(uint16_t), "EMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);

	write_Coloration_Tag("numberSection1", "uint16_t", "", offset, sizeof(uint16_t), "EMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("numberSection2", "uint16_t", "", offset, sizeof(uint16_t), "EMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("offsetSection1", "uint32_t", "Offset", offset, sizeof(uint32_t), "EMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);	offset += sizeof(uint32_t);
	write_Coloration_Tag("offsetSection2", "uint32_t", "Offset", offset, sizeof(uint32_t), "EMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);	offset += sizeof(uint32_t);

	/*
	//it's sound like random memory values, and there is a file witha a header witch take only 0x18
	write_Coloration_Tag("unknow3", "uint16_t", "", offset, sizeof(uint16_t), "EMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("unknow4", "uint16_t", "", offset, sizeof(uint16_t), "EMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("unknow5", "uint16_t", "", offset, sizeof(uint16_t), "EMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("unknow6", "uint16_t", "", offset, sizeof(uint16_t), "EMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	*/


	std::vector<uint32_t> listOffsetForNextSection1;
	listOffsetForNextSection1.push_back(hdr->offsetSection1);


	std::vector<uint32_t> listOffsetfromSection1ToSection2;
	EMP_Section1* section1;
	size_t offset_tmp = 0;
	incSection++;
	for (size_t i = 0; i < listOffsetForNextSection1.size(); i++)			//listOffsetForNextSection1.size could increase
	{
		if (listOffsetForNextSection1.at(i) == 0)
			continue;
		
		section1 = (EMP_Section1*)GetOffsetPtr(buf, listOffsetForNextSection1.at(i));
		offset = listOffsetForNextSection1.at(i);
		
		//add the 2 next Section1.
		offset_tmp = (*section1).offsetSiblingSection1;
		if (offset_tmp)
			listOffsetForNextSection1.push_back(offset_tmp + listOffsetForNextSection1.at(i));
		offset_tmp = (*section1).offsetChildSection1;
		if (offset_tmp)
			listOffsetForNextSection1.push_back(offset_tmp + listOffsetForNextSection1.at(i));


		incParam = 0;
		write_Coloration_Tag("name", "string", "", offset, 32 * sizeof(char), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += 32 * sizeof(char);
		
		write_Coloration_Tag("unk_0", "uint8_t", "values are 0, 2, 4, 8 (most), a, 10, 12, sema type composed", offset, sizeof(uint8_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_0b", "uint8_t", "if you just consider the 4 first right bits you have all value [0, 0xF], 0 is the most", offset, sizeof(uint8_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_1", "uint16_t", "values are 0, 1, 2, 3 and 0x10, 0x11, 0x13 (so it seam to be a composed type with first 4 bits), 0x21, 0x23, 0x41, 0x43, 0x50, 0x51, 0x53, 0x101, 0x103", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		write_Coloration_Tag("section3_type", "uint8_t", "only in [0,5], 0 is the most seam to get the type , so the size of extended part of Section1 , named Section3", offset, sizeof(uint8_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("section3_type_b", "uint8_t","only 0 1 or 2. 2 is the most value", offset, sizeof(uint8_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_3", "uint16_t", "have all value in[0, 0x1c], and non continue value until 0x190. 0xFFFF (==none) is the most value", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_4", "uint8_t", "few holes until 0x60, go to ff (not a none value). 0xA is the most", offset, sizeof(uint8_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_4b", "uint8_t", "values found are 0 (most), 1, 2, 3, 4, 7, 0x13, 0x1b, 0x1d, 0x1f, 0x21, 0x23, 0x27, 0x2a", offset, sizeof(uint8_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_5", "uint16_t", "have all value in[0, 0x10], and non continue value until 0x12C. 0 is the most", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_6", "uint8_t", "few holes until 0x56, go to ff (not a none value). 0 is the most", offset, sizeof(uint8_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_6b", "uint8_t", "have all value in [0, 8], and non continue value until 0x64. 0 is the most", offset, sizeof(uint8_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_7", "uint8_t", "have all value in [0, 0x12], and no continue until 0xc8, (there is a FF, may be for none), 0 most of the time", offset, sizeof(uint8_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_7b", "uint8_t", "have all value in [0, 8], and no continue until 0x32, 0 most of the time", offset, sizeof(uint8_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);

		write_Coloration_Tag("unk_8", "uint32_t", "always 1", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_9", "uint16_t", "have all value in [0, 0x14], and non continue value until 0x64. 1 is most of the time.", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_9b", "uint16_t", "values found : 0, 1 ,2 ,3, 4, 5, 6, 8, 0xA, 0x64, so it's look ate combined type.", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_10", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_11", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		write_Coloration_Tag("unk_12", "float", "range found [-200.0, 200.0], 0.0 is most of the time, a lot near of 0", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
		write_Coloration_Tag("unk_13", "float", "range found [-40.0, 150.0], 0.0 is most of the time, a lot near of 0", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
		write_Coloration_Tag("unk_14", "float", "range found [-100.0, 200.0], 0.0 is most of the time, a lot near of 0", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
		write_Coloration_Tag("unk_15", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		write_Coloration_Tag("unk_16", "float", "range found [-1.0, 5.2], 0.0 is most of the time", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
		write_Coloration_Tag("unk_17", "float", "range found [-10.0, 20.0], 0.0 is most of the time", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
		write_Coloration_Tag("unk_18", "float", "range found [-12.0, 5.0], 0.0 is most of the time", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
		write_Coloration_Tag("unk_19", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		write_Coloration_Tag("unk_20", "float", "range found [-270.0, 320.0], 0.0 is most of the time, it's clearly a rotation in degrees", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
		write_Coloration_Tag("unk_21", "float", "range found [-270.0, 360.0], 0.0 is most of the time, it's clearly a rotation in degrees", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
		write_Coloration_Tag("unk_22", "float", "range found [-180.0, 360.0], 0.0 is most of the time, it's clearly a rotation in degrees", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
		write_Coloration_Tag("unk_23", "float", "range found [-180.0, 405.0], 0.0 is most of the time, it's clearly a rotation in degrees", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);

		write_Coloration_Tag("unk_24", "float", "range found [-200.0, 360.0], 0.0 is most of the time, it's clearly a rotation in degrees", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
		write_Coloration_Tag("unk_24_b", "float", "range found [-180.0, 810.0], 0.0 is most of the time, it's clearly a rotation in degrees", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
		write_Coloration_Tag("unk_25", "float", "range found [-180.0, 810.0], 0.0 is most of the time, it's clearly a rotation in degrees", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
		write_Coloration_Tag("unk_25_b", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		
		write_Coloration_Tag("unk_26", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_27", "float", "range found [0.0, 90.0], 0.0 is most of the time", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
		
		
		size_t offset_section4_declaration = offset;
		write_Coloration_Tag("unk_28", "uint16_t", "only 0, 1 or 2 found", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		write_Coloration_Tag("numberSection4", "uint16_t", "lazybone Type0, maximum found is 12", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		
		offset_tmp = (*section1).offsetSection4;
		if (offset_tmp)
			offset_tmp += offset_section4_declaration;
		write_Coloration_Tag("offsetSection4", "uint32_t", "to Section4 => " + UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		
		

		write_Coloration_Tag("numberSection5", "uint16_t", "Section5 == lazybone Type1, maximum found is 4", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		write_Coloration_Tag("numberChildSection1", "uint16_t", "", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		offset_tmp = (*section1).offsetSection5;
		if (offset_tmp)
			offset_tmp += listOffsetForNextSection1.at(i);
		write_Coloration_Tag("offsetSection5", "uint32_t", "to Section5 (== lazybone Type1) => " + UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		

		offset_tmp = (*section1).offsetSiblingSection1;
		if (offset_tmp)
			offset_tmp += listOffsetForNextSection1.at(i);
		write_Coloration_Tag("offsetNextSection1", "uint32_t", "to a next Section1 => "+ UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		offset_tmp = (*section1).offsetChildSection1;
		if (offset_tmp)
			offset_tmp += listOffsetForNextSection1.at(i);
		write_Coloration_Tag("offsetNextSection1", "uint32_t", "to a next Section1 => " + UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);		




		//extended parts

		size_t offset_Section3 = offset;

		if ((section1[0].section3_type_b == 0x02))
		{
			EMP_Section3_TexturePart* textPart = (EMP_Section3_TexturePart*)GetOffsetPtr(buf, offset_Section3);


			incParam = 0;
			write_Coloration_Tag("unk_0", "uint16_t", "", offset, sizeof(uint16_t), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			write_Coloration_Tag("unk_1", "uint16_t", "", offset, sizeof(uint16_t), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			write_Coloration_Tag("unk_2a", "uint32_t", "could be a float between [-3,3] but a ~10 values are Nan or wrong number", offset, sizeof(uint32_t), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_2", "uint32_t", "only 0x45 (most), 0xc7, 0x41, 0x47, seam to be a combined type", offset, sizeof(uint32_t), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_3", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);

			write_Coloration_Tag("emmValue", "uint16_t", "emmId in emb, last found is 0x39. there is a 0xffff for none.", offset, sizeof(uint16_t), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			write_Coloration_Tag("unk_4", "uint16_t", "only 0, 1, or 2", offset, sizeof(uint16_t), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);


			offset_tmp = (*textPart).offsetToOffsetToSection2;
			if (offset_tmp)
				offset_tmp += listOffsetForNextSection1.at(i);
			write_Coloration_Tag("offsetToOffsetToSection2", "uint32_t", " (add start Section1) => " + UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);

			write_Coloration_Tag("unk_5", "float", "ange found [-0.75, 750.0], lot of time values are near of +0.0", offset, sizeof(float), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_6", "float", "range found [-5.0, 25.0], 0.0 is most of the time", offset, sizeof(float), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

			write_Coloration_Tag("unk_7", "float", "range found [-1.05853653, 750.0], 0.5 is most of the time", offset, sizeof(float), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_8", "float", "range found [-1.5, 17.5], 0.0 is most of the time", offset, sizeof(float), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_9", "float", "range found [-0.98, 250.0], 0.5 is most of the time, lot of value is near +0.0.", offset, sizeof(float), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_10", "float", "range found [-5.0, 15.0], 0.0 is most of the time", offset, sizeof(float), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

			write_Coloration_Tag("color0", "float[4]", "it's a RGBA color", offset, 4 * sizeof(float), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += 4 * sizeof(float);
			write_Coloration_Tag("color1", "float[4]", "it's a RGBA color", offset, 4 * sizeof(float), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += 4 * sizeof(float);
			write_Coloration_Tag("color2", "float[4]", "it's a RGBA color", offset, 4 * sizeof(float), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += 4 * sizeof(float);

			write_Coloration_Tag("unk_11", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_12", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_13", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_14", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section3_TexturePart", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);

		}else {

			//todo look if after other type like 3, 4, etc ...;
		}




		if ((section1[0].section3_type==0x00)&& (section1[0].section3_type_b == 0x01) )
		{
			incParam = 0;
			write_Coloration_Tag("unk_0", "float", "range found [-15.0, 13.0], 0.0 is most of the time", offset, sizeof(float), "Section3_00_01", parent, idTag++, incSection+2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_1", "float", "range found [-11.0, 25.0], 0.0 is most of the time", offset, sizeof(float), "Section3_00_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_2", "float", "range found [-70.0, 600.0], 0.0 is most of the time", offset, sizeof(float), "Section3_00_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_3", "float", "range found [-50.0, 50.0], 0.0 is most of the time", offset, sizeof(float), "Section3_00_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_4", "float", "range found [-5.0, 180.0], 1.0 is most of the time, seam to be degrees", offset, sizeof(float), "Section3_00_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_5", "float", "range found [0.0, 180.0], 0.0 is most of the time, seam to be degrees", offset, sizeof(float), "Section3_00_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_6", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section3_00_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_7", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section3_00_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);

		}else if ((section1[0].section3_type == 0x01) && (section1[0].section3_type_b == 0x01)) {
			
			incParam = 0;
			write_Coloration_Tag("unk_0", "float", "range found [-3.0, 40.0], 0.0 is most of the time", offset, sizeof(float), "Section3_01_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_1", "float", "range found [-2.0, 20.0], 0.0 is most of the time", offset, sizeof(float), "Section3_01_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_2", "float", "range found [-50.0, 80.0], 1.0 is most of the time", offset, sizeof(float), "Section3_01_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_3", "float", "range found [-30.0, 40.0], 0.0 is most of the time", offset, sizeof(float), "Section3_01_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		}else if ((section1[0].section3_type == 0x02) && (section1[0].section3_type_b == 0x01)) {

			incParam = 0;
			write_Coloration_Tag("unk_0", "float", "range found [-20.0, 10.0], 0.0 is most of the time", offset, sizeof(float), "Section3_02_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_1", "float", "range found [-2.3, 40.0], 0.0 is most of the time", offset, sizeof(float), "Section3_02_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_2", "float", "range found [-120.0, 250.0], 2.0 is most of the time", offset, sizeof(float), "Section3_02_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_3", "float", "range found [-60.0, 200.0], 0.0 is most of the time", offset, sizeof(float), "Section3_02_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_4", "float", "range found [-100.0, 150.0], 0.0 is most of the time, seam to be degrees", offset, sizeof(float), "Section3_02_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_5", "float", "range found [-90.0, 180.0], 0.0 is most of the time, seam to be degrees", offset, sizeof(float), "Section3_02_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_6", "float", "range found [0.0, 90.0], 0.5 is most of the time", offset, sizeof(float), "Section3_02_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_7", "float", "range found [-0.01, 1.0], 0.0 is most of the time", offset, sizeof(float), "Section3_02_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_8", "float", "range found [0.0, 90.0], 0.5 is most of the time", offset, sizeof(float), "Section3_02_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_9", "float", "range found [0.0, 1.0], 0.0 is most of the time", offset, sizeof(float), "Section3_02_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_10", "uint32_t", "only 0 or 1", offset, sizeof(uint32_t), "Section3_02_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_11", "float", "range found [0.0, 1.0], 0.1 is most of the time", offset, sizeof(float), "Section3_02_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		}
		else if ((section1[0].section3_type == 0x03) && (section1[0].section3_type_b == 0x01)) {

			incParam = 0;
			write_Coloration_Tag("unk_0", "float", "range found [-13.0, 70.0], 0.0 is most of the time", offset, sizeof(float), "Section3_03_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_1", "float", "range found [-5.0, 80.0], 0.0 is most of the time", offset, sizeof(float), "Section3_03_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_2", "float", "range found [-40.0, 120.0], 1.0 is most of the time", offset, sizeof(float), "Section3_03_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_3", "float", "range found [-10.0, 50.0], 0.0 is most of the time", offset, sizeof(float), "Section3_03_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_4", "float", "range found [-90.0, 90.0], 0.0 is most of the time", offset, sizeof(float), "Section3_03_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_5", "float", "range found [-50.0, 100.0], 0.0 is most of the time", offset, sizeof(float), "Section3_03_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_6", "float", "could be some floats all = todo check", offset, sizeof(float), "Section3_03_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_7", "float", "0.0 most of the time, 1.5, 0.1, 0.05, 1.0", offset, sizeof(float), "Section3_03_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_8", "float", "could be some floats all = todo check", offset, sizeof(float), "Section3_03_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_9", "float", "0.0 most of the time, 0.4, 0.1, 0.05, 0.25, 1.5 ", offset, sizeof(float), "Section3_03_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_10", "uint32_t", "only 0 or 1", offset, sizeof(uint32_t), "Section3_03_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_11", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section3_03_01", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);











		}else if ((section1[0].section3_type == 0x00) && (section1[0].section3_type_b == 0x02)) {

			EMP_Section3_00_02* section3 = (EMP_Section3_00_02*)GetOffsetPtr(buf, offset_Section3);
			incParam = 0;
			write_Coloration_Tag("unk_0", "float", "range found [-360.0, 360.0], 0.0 is the most, values look as degrees", offset, sizeof(float), "Section3_00_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_1", "float", "range found [-180.0, 360.0], 0.0 is the most, values look as degrees", offset, sizeof(float), "Section3_00_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_2", "float", "range found [-40.0, 120.0], 0.0 is the most", offset, sizeof(float), "Section3_00_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_3", "float", "range found [-20.0, 50.0], 0.0 is the most", offset, sizeof(float), "Section3_00_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);


		// ((section1[0].section3_type == 0x10) && (section1[0].section3_type_b == 0x02)) { is only texturePart.



		}else if ((section1[0].section3_type == 0x02) && (section1[0].section3_type_b == 0x02)) {

			EMP_Section3_02_02* section3 = (EMP_Section3_02_02*)GetOffsetPtr(buf, offset_Section3);
			incParam = 0;
			write_Coloration_Tag("unk_0", "float", "range found [0.0, 180.0], 0.0 is most of the time, all values look like degrees, could be pitch", offset, sizeof(float), "Section3_02_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_1", "float", "range found [-20.0, 360.0], 0.0 is most of the time, all values look like degrees, could be yaw", offset, sizeof(float), "Section3_02_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_2", "float", "range found [-10.0, 360.0], 0.0 is most of the time, could be roll", offset, sizeof(float), "Section3_02_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_3", "float", "range found [-10.0, 60.0], 0.0 is most of the time", offset, sizeof(float), "Section3_02_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_4", "float", "0.0 most of the time, 0.57735, 0.240772", offset, sizeof(float), "Section3_02_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_5", "float", "0.0 most of the time, 0.57735, 0.447214, 0.481543, 0.316228", offset, sizeof(float), "Section3_02_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_6", "float", "0.0 most of the time, 0.57735, 1.0, 0.894427, 0.842701, 0.948683", offset, sizeof(float), "Section3_02_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_7", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section3_02_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);


		}else if ((section1[0].section3_type == 0x03) && (section1[0].section3_type_b == 0x02)) {
			

			EMP_Section3_03_02* section3 = (EMP_Section3_03_02*)GetOffsetPtr(buf, offset_Section3);
			incParam = 0;
			write_Coloration_Tag("unk_0", "uint16_t", " continuity from 1 to 0x10, after it's not continue and go to 0x96", offset, sizeof(uint16_t), "Section3_03_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			write_Coloration_Tag("unk_1", "uint16_t", "from 0 to 10, 12, 13, 14, 20, 30, 40, 50", offset, sizeof(uint16_t), "Section3_03_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			write_Coloration_Tag("unk_2", "uint16_t", "only 1, 2, 3, 4 or 5", offset, sizeof(uint16_t), "Section3_03_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			write_Coloration_Tag("lastIndexColors", "uint16_t", "", offset, sizeof(uint16_t), "Section3_03_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			write_Coloration_Tag("unk_4", "uint32_t", "only 0 or 1", offset, sizeof(uint32_t), "Section3_03_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			
			offset_tmp = (*section3).offsetSection3_03_02_b;
			if (offset_tmp)
				offset_tmp += listOffsetForNextSection1.at(i);
			write_Coloration_Tag("offsetSection3_03_02_b", "uint32_t", "offset (add start Section1) => "+ UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section3_03_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			
			if (offset_tmp)
			{
				offset = offset_tmp;				
				incParam = 0;
				for (size_t j = 0; j <= (*section3).lastIndexColors; j++)
				{
					write_Coloration_Tag("Color[" + std::to_string(j) + "].R", "float", "", offset, sizeof(float), "Section3_03_02b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
					write_Coloration_Tag("Color[" + std::to_string(j) + "].G", "float", "", offset, sizeof(float), "Section3_03_02b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
					write_Coloration_Tag("Color[" + std::to_string(j) + "].B", "float", "", offset, sizeof(float), "Section3_03_02b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
					write_Coloration_Tag("Color[" + std::to_string(j) + "].A", "float", "strange : lot of values are negatives. may be it's not Color ?", offset, sizeof(float), "Section3_03_02b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
				}
			}



		}else if ((section1[0].section3_type == 0x04) && (section1[0].section3_type_b == 0x02)) {
			

			EMP_Section3_04_02* section3 = (EMP_Section3_04_02*)GetOffsetPtr(buf, offset_Section3);
			incParam = 0;
			write_Coloration_Tag("unk_0", "float", "0.0 most of the time , or 90.0", offset, sizeof(float), "Section3_04_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_1", "float", "360.0 most of the time, or 0.0", offset, sizeof(float), "Section3_04_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_2", "float", "1.0 most of the time, or 0.0, 0.2, 0.5, 2.0, 10.0, 20.0 ", offset, sizeof(float), "Section3_04_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_3", "float", "0.0, 0.2, 1.0, 2.0, 3.0, 10.0", offset, sizeof(float), "Section3_04_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_4", "uint32_t", "0 or 0.57735002040863 (if unk_5 is 0.57735002040863, unk_4 always the same)", offset, sizeof(uint32_t), "Section3_04_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_5", "uint32_t", "0 or 0.70710700750351 or 0.57735002040863", offset, sizeof(uint32_t), "Section3_04_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_6", "uint32_t", "always the same value as unk_5", offset, sizeof(uint32_t), "Section3_04_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_7", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section3_04_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_8", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section3_04_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);

			offset_tmp = (*section3).offsetEmg;
			if (offset_tmp)
				offset_tmp += listOffsetForNextSection1.at(i);
			write_Coloration_Tag("offsetEmg", "uint32_t", "offset (add start Section1) => " + UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section3_04_02", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_9", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section3_04_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_10", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section3_04_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);

			if (offset_tmp)
			{
				write_Coloration_Tag("start EMG", "uint32_t", "", offset_tmp, 4 * sizeof(uint32_t), "Section3_04_02", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i);
			}



		}else if ((section1[0].section3_type == 0x05) && (section1[0].section3_type_b == 0x02)) {

			
			EMP_Section3_05_02* section3 = (EMP_Section3_05_02*)GetOffsetPtr(buf, offset_Section3);

			incParam = 0;
			write_Coloration_Tag("unk_0", "float", "range found [-180.0, 300.0], 0.0 is most of the time, seam to be degrees", offset, sizeof(float), "Section3_05_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_1", "float", "range found [-150.0, 360.0], 0.0 is most of the time, seam to be degrees", offset, sizeof(float), "Section3_05_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_2", "float", "range found [-80.0, 180.0], 0.0 is most of the time", offset, sizeof(float), "Section3_05_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			write_Coloration_Tag("unk_3", "float", "range found [-60.0, 360.0], 0.0 is most of the time", offset, sizeof(float), "Section3_05_02", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);


			//extention EMP_Section3_05_02_b
			{
				offset = offset_Section3 + sizeof(EMP_Section3_05_02);

				size_t offset_Section3_b = offset;
				EMP_Section3_05_02_b* section3_b = (EMP_Section3_05_02_b*)GetOffsetPtr(buf, offset_Section3_b);
				
				incParam = 0;
				write_Coloration_Tag("lastIndexForFloatGroups", "uint16_t", "", offset, sizeof(uint16_t), "Section3_05_02_b", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
				write_Coloration_Tag("numberFloatByGroup", "uint16_t", "only 0 (may the offset also 0 in this case), 1 or 2", offset, sizeof(uint16_t), "Section3_05_02_b", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);

				offset_tmp = (*section3_b).offsetSection3_05_02_c;
				if (offset_tmp)
					offset_tmp += listOffsetForNextSection1.at(i);
				write_Coloration_Tag("offsetSection3_05_02_c", "uint32_t", " (add start Section1) To floats series => " + UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section3_05_02", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);


				//extention List Floats
				if (offset_tmp)
				{
					offset = offset_tmp;
					incParam = 0;
					for (size_t j = 0; j <= (*section3_b).lastIndexForFloatGroups; j++)
					{
						for (size_t k = 0; k < (*section3_b).numberFloatByGroup; k++)
						{
							write_Coloration_Tag("float[" + std::to_string(j) + "][" + std::to_string(k) + "]", "float", " values for [0,1], [-1, 1] and degrees angles ", offset, sizeof(float), "Section3_05_02_b_floats", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
						}
					}
				}
			}
		}



		if ((section1[0].section3_type_b == 0x02))
		{
			EMP_Section3_TexturePart* textPart = (EMP_Section3_TexturePart*)GetOffsetPtr(buf, offset_Section3);

			//extention link to Section2.
			offset_tmp = (*textPart).offsetToOffsetToSection2;
			if (offset_tmp)
			{
				offset_tmp += listOffsetForNextSection1.at(i);
				offset = offset_tmp;

				incParam = 0;
				uint32_t* section3_b = (uint32_t*)GetOffsetPtr(buf, offset);

				offset_tmp = section3_b[0];				// Todo test if EMP_Section3_05_02.unk_4 != 1 => more than one refence
				if (offset_tmp)
					offset_tmp += listOffsetForNextSection1.at(i);
				write_Coloration_Tag("offsetToSection2", "uint32_t", " (add start Section1) To a Section2 => " + UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section3_TexturePart_b", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);


				//extention in Section2 series
				if (offset_tmp)
				{
					//write_Coloration_Tag("startSection2", "uint16_t", " link from Section1[" + std::to_string(i) + "]", offset_tmp, sizeof(uint16_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i, false);
					listOffsetfromSection1ToSection2.push_back((*((uint32_t*)GetOffsetPtr(buf, offset_tmp))));			// we will made the link on header of Section2
				}
					
			}
		}



		//Section4
		offset_tmp = (*section1).offsetSection4;
		if (offset_tmp)
		{
			size_t startSection4_series = offset_tmp;

			for (size_t j = 0; j < (*section1).numberSection4; j++)
			{
				offset = startSection4_series + j * sizeof(EMP_Section4) + offset_section4_declaration;

				size_t offset_Section4 = offset;
				EMP_Section4* section4 = (EMP_Section4*)GetOffsetPtr(buf, offset_Section4);

				incParam = 0;
				write_Coloration_Tag("unk_0", "uint8_t", ", only 0, 1, 2, 3 or 4. start of Section4, lazybone Type0", offset, sizeof(uint8_t), "Section4", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint8_t);
				write_Coloration_Tag("unk_1", "uint8_t", "only 0, 1, 2, 3, 0x10, 0x11, 0x12, 0x13, seam type combined", offset, sizeof(uint8_t), "Section4", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint8_t);
				write_Coloration_Tag("unk_2", "uint16_t", "only 0 or 1", offset, sizeof(uint16_t), "Section4", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
				write_Coloration_Tag("unk_3", "uint16_t", "seam always 0", offset, sizeof(uint16_t), "Section4", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
				write_Coloration_Tag("unk_4", "uint16_t", "seam always 0", offset, sizeof(uint16_t), "Section4", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);

				write_Coloration_Tag("numberIndexIntoSection4_b", "uint16_t", "", offset, sizeof(uint16_t), "Section4", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
				write_Coloration_Tag("numberFloatIntoSection4_b", "uint16_t", "", offset, sizeof(uint16_t), "Section4", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);

				offset_tmp = (*section4).offsetSection4_b;
				if (offset_tmp)
					offset_tmp += offset_Section4;

				write_Coloration_Tag("offsetSection4_b", "uint32_t", " (add start Section1) To floats series => " + UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section4", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);

				//Section4_b
				if (offset_tmp)
				{
					offset = offset_tmp;
					

					incParam = 0;
					for (size_t k = 0; k < (*section4).numberFloatIntoSection4_b; k++)
					{
						write_Coloration_Tag("keyframeTimeIndex[" + std::to_string(k) + "]", "uint16_t", "maximum found 6040", offset, sizeof(uint16_t), "Section4_b", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
					}
					if (offset % sizeof(uint32_t) != 0)
						offset += sizeof(uint16_t);

					incParam = 0;
					for (size_t k = 0; k < (*section4).numberFloatIntoSection4_b; k++)
					{
						write_Coloration_Tag("keyframeValues["+ std::to_string(k) +"]", "float", "range found [-630.0, 660.41], but most of value are near 0 : 65% [-2,2], 30% [-10, 10]", offset, sizeof(float), "Section4_b", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
					}
					
					incParam = 0;
					size_t nbIndex = (((*section4).numberFloatIntoSection4_b != 1) ? ((*section4).numberIndexIntoSection4_b) : 0);

					for (size_t k = 0; k < nbIndex; k++)
					{
						write_Coloration_Tag("index["+ std::to_string(k) +"]", "uint16_t", "refere to the list of floats", offset, sizeof(uint16_t), "Section4_b", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
					}
				}
			}
		}




		//Section5
		offset_tmp = (*section1).offsetSection5;
		if (offset_tmp)
		{
			size_t startSection5_seeries = offset_tmp + listOffsetForNextSection1.at(i);

			for (size_t k = 0; k < (*section1).numberSection5; k++)
			{
				offset = startSection5_seeries + k * sizeof(EMP_Section5);

				size_t offset_Section5 = offset;
				EMP_Section5* section5 = (EMP_Section5*)GetOffsetPtr(buf, offset_Section5);

				incParam = 0;
				write_Coloration_Tag("unk_0", "uint16_t", "start of Section5, lazybone Type1", offset, sizeof(uint16_t), "Section5", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
				write_Coloration_Tag("numberSection5_b", "uint16_t", "maximum found 8 (2 is the most)", offset, sizeof(uint16_t), "Section5", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);

				offset_tmp = (*section5).offsetSection5_b;
				if (offset_tmp)
					offset_tmp += offset_Section5;

				write_Coloration_Tag("offsetSection5_b", "uint32_t", " (add start Section5) => " + UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section5", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);

				//Section5_b
				if (offset_tmp)
				{
					size_t startSection5_b_series = offset_tmp;

					for (size_t j = 0; j < (*section5).numberSection5_b; j++)
					{
						offset = startSection5_b_series + j * sizeof(EMP_Section5_b);

						size_t offset_Section5_b = offset;
						EMP_Section5_b* section5_b = (EMP_Section5_b*)GetOffsetPtr(buf, offset_Section5_b);

						incParam = 0;
						write_Coloration_Tag("unk_0", "uint8_t", "only 0, 1 or 2, start of Section5_b, lazybone Type1 Entry", offset, sizeof(uint8_t), "Section5_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint8_t);
						write_Coloration_Tag("unk_1", "uint8_t", "only 0, 16 (most), 17, 18", offset, sizeof(uint8_t), "Section5_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint8_t);
						write_Coloration_Tag("unk_2", "uint16_t", "only 0 or 1", offset, sizeof(uint16_t), "Section5_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
						write_Coloration_Tag("unk_3", "uint16_t", "", offset, sizeof(uint16_t), "Section5_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
						write_Coloration_Tag("unk_4", "uint16_t", "", offset, sizeof(uint16_t), "Section5_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);

						write_Coloration_Tag("numberIndexIntoSection5_c", "uint16_t", "", offset, sizeof(uint16_t), "Section5_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
						write_Coloration_Tag("numberFloatIntoSection5_c", "uint16_t", "", offset, sizeof(uint16_t), "Section5_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);

						offset_tmp = (*section5_b).offsetSection5_c;
						if (offset_tmp)
							offset_tmp += offset_Section5_b;

						write_Coloration_Tag("offsetSection5_c", "uint32_t", " (add start Section5_b) section with floats (lazybone Type1 Data) => " + UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section5_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);



						//Section5_c
						if (offset_tmp)
						{
							offset = offset_tmp;


							incParam = 0;
							for (size_t k = 0; k < (*section5_b).numberFloatIntoSection5_c; k++)
							{
								write_Coloration_Tag("keyframeTimeIndex[" + std::to_string(k) + "]", "uint16_t", "maximum value found = 59578, isn't to much ?", offset, sizeof(uint16_t), "Section5_c", parent, idTag++, incSection + 8, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
							}
							if (offset % sizeof(uint32_t) != 0)
								offset += sizeof(uint16_t);

							incParam = 0;
							for (size_t k = 0; k < (*section5_b).numberFloatIntoSection5_c; k++)
							{
								write_Coloration_Tag("keyframeValues[" + std::to_string(k) + "]", "float", "range found [-2500.0, 5000.0], 0.0 is most of time, it's look like have some animated position", offset, sizeof(float), "Section5_c", parent, idTag++, incSection + 8, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
							}

							incParam = 0;
							size_t nbIndex = (((*section5_b).numberFloatIntoSection5_c != 1) ? (*section5_b).numberIndexIntoSection5_c : 0);

							for (size_t k = 0; k < nbIndex; k++)
							{
								write_Coloration_Tag("index[" + std::to_string(k) + "]", "uint16_t", "refere to the list of floats", offset, sizeof(uint16_t), "Section5_c", parent, idTag++, incSection + 8, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
							}
						}

					}
				}
			}
		}


	}



	
	
	
	EMP_Section2 *section2 = (EMP_Section2*)GetOffsetPtr(buf, hdr->offsetSection2);
	incSection++;
	for (size_t i = 0; i < hdr->numberSection2; i++)
	{
		offset = hdr->offsetSection2 + i * (sizeof(EMP_Section2));

		string linkFromSection1 = "";
		size_t nbLink = listOffsetfromSection1ToSection2.size();
		for (size_t j = 0; j < nbLink; j++)
		{
			if (listOffsetfromSection1ToSection2.at(j)==offset)
				linkFromSection1 += ((linkFromSection1.size()!=0) ? "," : "") + string("Section1[") + std::to_string(j) +"]";
		}

		incParam = 0;
		write_Coloration_Tag("unk_0", "uint8_t", "always 0, Start of Section2["+ std::to_string(i) +"]" + ((linkFromSection1.size()) ? (" with link to "+ linkFromSection1) : ""), offset, sizeof(uint8_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("EmbTextureId", "uint8_t", "until 69", offset, sizeof(uint8_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_0b", "uint16_t", " 0, 1, 2, 3, 16, 17, 18, it may be the samplerID in entries of shaders", offset, sizeof(uint16_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);

		write_Coloration_Tag("unk_1", "uint8_t", " only 1 or 2", offset, sizeof(uint8_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_2", "uint8_t", " only 1 or 2", offset, sizeof(uint8_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_3", "uint8_t", "only 0, 1 or 2", offset, sizeof(uint8_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_3b", "uint8_t", "only 0, 1 or 2", offset, sizeof(uint8_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);

		write_Coloration_Tag("unk_4", "uint8_t", "only 0, 1 or 2", offset, sizeof(uint8_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_5", "uint8_t", "only 0, 1 or 2", offset, sizeof(uint8_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("typeSection2_b", "uint16_t", " influence the next 16 bytes, only 0, 1 or 2", offset, sizeof(uint16_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);


		if (section2[i].typeSection2_b == 0)
		{
			size_t startOffsetSection2b = offset;
			EMP_Section2_b_00 *section2b = (EMP_Section2_b_00*)GetOffsetPtr(buf, startOffsetSection2b);

			write_Coloration_Tag("Color", "float[4]", "R: [0.0, 1.0] 0.0 most of Time. G: [0.0, 1.0] 0.0 most of Time. B : [0.00100000005, 40.0] 1.0 most of Time. A : [0.00999999978, 50.0] 1.0 most of Time. it's TextureScrollScale", offset, 4 * sizeof(float), "Section2b_00", parent, idTag++, incSection +1, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);

		}else if (section2[i].typeSection2_b == 1) {

			size_t startOffsetSection2b = offset;
			EMP_Section2_b_01 *section2b = (EMP_Section2_b_01*)GetOffsetPtr(buf, startOffsetSection2b);

			write_Coloration_Tag("Color", "float[4]", "R is in [-0.0500000007, 2.0], 0 most of time. G is in [-0.0799999982, 1.0], 0 most of Time. B and A alwaus at 0. may be it's not a color ?", offset, 4 * sizeof(float), "Section2b_01", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);


		}else if (section2[i].typeSection2_b == 2) {

			size_t startOffsetSection2b = offset;
			EMP_Section2_b_02 *section2b = (EMP_Section2_b_02*)GetOffsetPtr(buf, startOffsetSection2b);
			
			incParam = 0;
			write_Coloration_Tag("unk_0", "uint32_t", "all value in [1, 0x10], go no continue to 0x7d0, 2 is the most", offset, sizeof(uint32_t), "Section2b_02", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_1", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section2b_02", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_2", "uint16_t", "always 0", offset, sizeof(uint16_t), "Section2b_02", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			write_Coloration_Tag("numberSection2_02_c", "uint16_t", "maximum found 64", offset, sizeof(uint16_t), "Section2b_02", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			write_Coloration_Tag("offsetSection2_02_c", "uint32_t", "", offset, sizeof(uint32_t), "Section2b_02", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

			offset_tmp = (*section2b).offsetSection2_02_c;
			if (offset_tmp)
			{
				offset_tmp += startOffsetSection2b;
				size_t startOffsetSection2c = offset_tmp;
				offset = offset_tmp;

				EMP_Section2_b_02_c *section2c = (EMP_Section2_b_02_c*)GetOffsetPtr(buf, startOffsetSection2c);
				for (size_t j = 0; j < (*section2b).numberSection2_02_c; j++)
				{
					incParam = 0;
					write_Coloration_Tag("unk_0", "uint16_t", "continue from 1 to 0x10, and after could go until 0x7d0", offset, sizeof(uint16_t), "Section2b_02_c", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
					write_Coloration_Tag("unk_1", "uint16_t", "always 0", offset, sizeof(uint16_t), "Section2b_02_c", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
					write_Coloration_Tag("Color", "float[4]", "R : [-0.8, 5.0] 0.0 is the most. G : [-2.0, 1.0] 0.0 is the most. B : [-1.0, 8.0] 0.125 is the most. A : [-1.0, 4.0] 0.5 and 0.25 are the most. it's TextureScrollScale", offset, 4 * sizeof(float), "Section2b_02_c", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);
				}
			}

		}
	}
}





/*-------------------------------------------------------------------------------\
|                             write_Coloration_Tag			                     |
\-------------------------------------------------------------------------------*/
void EMP::write_Coloration_Tag(string paramName, string paramType, string paramComment, size_t startOffset, size_t size, string sectionName, TiXmlElement* parent, size_t idTag, size_t sectionIndex, size_t paramIndex, std::vector<bool> &listBytesAllreadyTagged, size_t sectionIndexInList, bool checkAllreadyTaggued)
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
			LibXenoverse::notifyError();
			continue;
		}

		if (index == 0x11ac)			//for test
			int aa = 42;

		if ((checkAllreadyTaggued) && (listBytesAllreadyTagged.at(index)))
		{
			printf("warning on tagID %i : the byte %i allready taggued, may be a overflow between blocks. Infos : %s. \n", idTag, index, (sectionName + ((sectionIndexInList != (size_t)-1) ? "[" + std::to_string(sectionIndexInList) + "]" : "") + "." + paramName + " (" + paramType + ") : " + paramComment).c_str());
			LibXenoverse::notifyError();
		}

		listBytesAllreadyTagged.at(index) = true;
	}
}
























/*-------------------------------------------------------------------------------\
|                             extractEmd					                     |
\-------------------------------------------------------------------------------*/
void EMP::extractEmd(string filename, EMD* emd)
{
	uint8_t *buf;
	size_t size;

	buf = ReadFile(filename, &size, true);
	if (!buf)
		return;


	string ext = LibXenoverse::extensionFromFilename(filename, true);
	string name_file = LibXenoverse::nameFromFilenameNoExtension(filename, true);
	string folder = LibXenoverse::folderFromFilename(filename);

	

	EMPHeader *hdr = (EMPHeader *)buf;
	if (size < sizeof(EMPHeader) || memcmp(hdr->signature, EMP_SIGNATURE, 4) != 0)
	{
		delete[] buf;
		return;
	}


	std::vector<uint32_t> listOffsetForNextSection1;
	listOffsetForNextSection1.push_back(hdr->offsetSection1);
	

	EMP_Section1* section1;
	size_t offset = 0;
	size_t offset_tmp = 0;
	for (size_t i = 0; i < listOffsetForNextSection1.size(); i++)			//listOffsetForNextSection1.size could increase
	{
		if (listOffsetForNextSection1.at(i) == 0)
			continue;

		section1 = (EMP_Section1*)GetOffsetPtr(buf, listOffsetForNextSection1.at(i));
		offset = listOffsetForNextSection1.at(i);


		//add the 2 next Section1.
		offset_tmp = (*section1).offsetSiblingSection1;
		if (offset_tmp)
		{
			listOffsetForNextSection1.push_back(offset_tmp + listOffsetForNextSection1.at(i));
		}
		offset_tmp = (*section1).offsetChildSection1;
		if (offset_tmp)
		{
			listOffsetForNextSection1.push_back(offset_tmp + listOffsetForNextSection1.at(i));
		}

		



		//extended parts
		offset += sizeof(EMP_Section1);

		size_t offset_Section3 = offset;

		if ((section1[0].section3_type == 0x04) && (section1[0].section3_type_b == 0x02))
		{
			EMP_Section3_04_02* section3 = (EMP_Section3_04_02*)GetOffsetPtr(buf, offset_Section3);
			
			//Todo Emg conversion emd , convertion to Xml
			offset_tmp = section3[0].offsetEmg;
			if (offset_tmp)
			{
				offset_tmp += listOffsetForNextSection1.at(i);
				offset = offset_tmp;

				LibXenoverse::EMG* emg = new LibXenoverse::EMG();
				emg->Load((uint8_t*)GetOffsetPtr(buf, offset), 0, 0);


				size_t file_size = emg->CalculatePartSize();
				if (file_size & 0xF)
					file_size += (0x10 - (file_size & 0xF));
				offset += file_size;

				uint32_t name_offset = *((uint32_t*)GetOffsetPtr(buf, offset));
				emg->name = std::string((char*)GetOffsetPtr(buf, name_offset + offset_tmp - 0x20));



				LibXenoverse::EMDMesh* emdMesh = new LibXenoverse::EMDMesh();
				emg->writeEmdMesh(emdMesh, emg->name);

				
				//string(section1[0].name)
				EMDModel* emdModel = new EMDModel();
				emdModel->name = string(section1[0].name);
				emdModel->meshes.push_back(emdMesh);

				emd->models.push_back(emdModel);

				delete emg;
			}
		}
	}


	delete[] buf;
}




/*-------------------------------------------------------------------------------\
|                             replaceEmgFromEmd					                 |
\-------------------------------------------------------------------------------*/
void EMP::replaceEmgFromEmd(string filename, EMD* emd)
{
	uint8_t *buf;
	size_t size;

	buf = ReadFile(filename, &size, true);
	if (!buf)
		return;


	EMPHeader *hdr = (EMPHeader *)buf;
	if (size < sizeof(EMPHeader) || memcmp(hdr->signature, EMP_SIGNATURE, 4) != 0)
	{
		delete[] buf;
		return;
	}



	
	//first, we need to analyze the emp file.

	struct Section1BinaryInformations
	{
		size_t index;
		uint32_t startOffset;
		uint32_t sizeWithoutEmg;
		string name;

		bool couldHaveEmg;							//if 04 02
		uint32_t startOffset_Emg;
		uint32_t sizeEmg;

		size_t indexSibling;
		size_t indexChild;

		Section1BinaryInformations(size_t index, uint32_t startOffset)
		{
			this->index = index;
			this->startOffset = startOffset;
			sizeWithoutEmg = 0;
			name = "";

			couldHaveEmg = false;							//if 04 02
			startOffset_Emg = 0;
			sizeEmg = 0;

			indexSibling = (size_t)-1;
			indexChild = (size_t)-1;
		}

		void operator=(Section1BinaryInformations &other)
		{
			this->index = other.index;
			this->startOffset = other.startOffset;
			this->sizeWithoutEmg = other.sizeWithoutEmg;
			this->name = other.name;

			this->couldHaveEmg = other.couldHaveEmg;							//if 04 02
			this->startOffset_Emg = other.startOffset_Emg;
			this->sizeEmg = other.sizeEmg;

			this->indexSibling = other.indexSibling;
			this->indexChild = other.indexChild;
		}
	};
	struct Section1BinaryInformations_Sorter
	{
		bool operator()(Section1BinaryInformations &a, Section1BinaryInformations &b) { return (a.startOffset < b.startOffset); }
	};



	std::vector<Section1BinaryInformations> listOffsetForNextSection1;
	listOffsetForNextSection1.push_back(Section1BinaryInformations(0, hdr->offsetSection1));


	EMP_Section1* section1;
	size_t offset = 0;
	size_t offset_tmp = 0;
	for (size_t i = 0; i < listOffsetForNextSection1.size(); i++)			//listOffsetForNextSection1.size could increase
	{
		if (listOffsetForNextSection1.at(i).startOffset == 0)
			continue;

		section1 = (EMP_Section1*)GetOffsetPtr(buf, listOffsetForNextSection1.at(i).startOffset);
		offset = listOffsetForNextSection1.at(i).startOffset;

		listOffsetForNextSection1.at(i).name = section1[0].name;


		//add the 2 next Section1.
		offset_tmp = (*section1).offsetSiblingSection1;
		if (offset_tmp)
		{
			listOffsetForNextSection1.at(i).indexSibling = listOffsetForNextSection1.size();
			listOffsetForNextSection1.push_back(Section1BinaryInformations(listOffsetForNextSection1.size(), offset_tmp + listOffsetForNextSection1.at(i).startOffset));
			
		}
		offset_tmp = (*section1).offsetChildSection1;
		if (offset_tmp)
		{
			listOffsetForNextSection1.at(i).indexChild = listOffsetForNextSection1.size();
			listOffsetForNextSection1.push_back(Section1BinaryInformations(listOffsetForNextSection1.size(), offset_tmp + listOffsetForNextSection1.at(i).startOffset));
		}


		


		//extended parts
		offset += sizeof(EMP_Section1);

		size_t offset_Section3 = offset;

		if ((section1[0].section3_type == 0x04) && (section1[0].section3_type_b == 0x02))
		{
			
			listOffsetForNextSection1.at(i).couldHaveEmg = true;
			
			EMP_Section3_04_02* section3 = (EMP_Section3_04_02*)GetOffsetPtr(buf, offset_Section3);

			//Todo Emg conversion emd , convertion to Xml
			offset_tmp = section3[0].offsetEmg;
			if (offset_tmp)
			{
				listOffsetForNextSection1.at(i).startOffset_Emg = offset_tmp;
				
				offset_tmp += listOffsetForNextSection1.at(i).startOffset;

				offset = offset_tmp;

				LibXenoverse::EMG* emg = new LibXenoverse::EMG();
				emg->Load((uint8_t*)GetOffsetPtr(buf, offset), 0, 0);


				size_t file_size = emg->CalculatePartSize();
				if (file_size & 0xF)
					file_size += (0x10 - (file_size & 0xF));

				// name_offset
				offset += file_size;
				uint32_t name_offset = *((uint32_t*)GetOffsetPtr(buf, offset));
				emg->name = std::string((char*)GetOffsetPtr(buf, name_offset + offset_tmp - 0x20));
				file_size += sizeof(uint32_t) + (emg->name.length() + 1) * sizeof(char);	
				if (file_size & 0xF)
					file_size += (0x10 - (file_size & 0xF));

				file_size += emg->CalculateVertexSize();
				if (file_size & 0xF)
					file_size += (0x10 - (file_size & 0xF));

				listOffsetForNextSection1.at(i).sizeEmg = file_size;

				delete emg;
			}
		}
	}

	//we check the order to have all startOffset ordered
	std::sort(listOffsetForNextSection1.begin(), listOffsetForNextSection1.end(), Section1BinaryInformations_Sorter());


	//we copy all information because we will change them for the new file
	std::vector<Section1BinaryInformations> listNewSection1 = listOffsetForNextSection1;

	//now, I check what we have to replace and update information of the Section1
	std::vector<EMDModel*> &models = emd->models;
	size_t nbModels = models.size();


	std::vector<EMG*> listEmg;
	EMG* emg = 0;
	bool haveToReplace = false;
	long sizeToAddAfterAllChage = 0;
	size_t nbSection1 = listNewSection1.size();
	
	for (size_t i = 0; i < nbSection1; i++)
	{
		Section1BinaryInformations &info = listNewSection1.at(i);
		info.sizeWithoutEmg = ((i + 1 != nbSection1) ? listNewSection1.at(i + 1).startOffset : hdr->offsetSection2) - info.startOffset;		//if it's the last, Section1, we take the end of Section1 to have the size. else the next Section  (we will considere the emg after, if it's present)

		if (info.startOffset_Emg)						//if there is a previous Emg
			info.sizeWithoutEmg = info.startOffset_Emg;
	}

	for (size_t i = 0; i < nbSection1; i++)
	{
		Section1BinaryInformations &info = listNewSection1.at(i);
		listEmg.push_back(0);
		
		if (!info.couldHaveEmg)
			continue;

		emg = 0;
		bool replaceByEmpty = false;
		for (size_t j = 0; j < nbModels; j++)
		{
			if (models.at(j)->name == info.name)
			{
				if(models.at(j)->meshes.size())
				{
					emg = new EMG();
					emg->readEmdMesh(models.at(j)->meshes.at(0), 0);
					replaceByEmpty = false;
				}else {
					replaceByEmpty = true;
				}
				
				haveToReplace = true;
				break;
			}
		}

		

		if ((!emg) && (!replaceByEmpty))
		{
			info.sizeWithoutEmg += info.sizeEmg;
			continue;
		}
			

		listEmg.back() = emg;

		size_t file_size = 0;
		if (emg)
		{
			file_size = emg->CalculatePartSize();
			if (file_size & 0xF)
				file_size += (0x10 - (file_size & 0xF));

			file_size += sizeof(uint32_t) + (emg->name.length() + 1) * sizeof(char);
			if (file_size & 0xF)
				file_size += (0x10 - (file_size & 0xF));


			file_size += emg->CalculateVertexSize();
			if (file_size & 0xF)
				file_size += (0x10 - (file_size & 0xF));
		}

		long sizeToAddAfterEmg = 0;
		if(info.startOffset_Emg)						//if there is a previous Emg
		{
			sizeToAddAfterEmg -= info.sizeEmg;
			if (replaceByEmpty)
				info.startOffset_Emg = 0;

		}else if(!replaceByEmpty){

			if (i + 1 != nbSection1)			//if it's not the last Section1
				info.startOffset_Emg = listNewSection1.at(i + 1).startOffset - listNewSection1.at(i).startOffset;
			else
				info.startOffset_Emg = hdr->offsetSection2 - listNewSection1.at(i).startOffset;		//if it's the last, Section1, we take the end of the Section1s.
		}
		info.sizeEmg = file_size;

		sizeToAddAfterEmg += file_size;
		sizeToAddAfterAllChage += sizeToAddAfterEmg;

		for (size_t j = i + 1; j < nbSection1; j++)			//for the next, we need to update all offset
			listNewSection1.at(j).startOffset += sizeToAddAfterEmg;
	}





	//if there isn't any replace, just skip
	if (!haveToReplace)
	{
		printf("There is nothing to replace. If your are sure, check EmdModel's name match with EmpEntry's name, and there is a type 04 02 (witch could accept a Emg part). Skipped.\n");
		delete[] buf;
		return;
	}



	//else, we will rebuild the file with replacement.
	
	size_t newFileSize = size + sizeToAddAfterAllChage;
	uint8_t* newBuf = (uint8_t*)malloc(newFileSize );
	memset(newBuf, 0, newFileSize);												//clean 0 filling

	//copy of the header, and update
	memcpy(newBuf, buf, sizeof(EMPHeader));
	
	EMPHeader* newHdr = (EMPHeader*)GetOffsetPtr(newBuf, 0);
	(*newHdr).offsetSection2 += sizeToAddAfterAllChage;


	//copy Section2
	memcpy(newBuf + (*newHdr).offsetSection2, buf + (*hdr).offsetSection2, size - 1 - (*hdr).offsetSection2);




	for (size_t i = 0; i < nbSection1; i++)
	{
		Section1BinaryInformations &newInfo = listNewSection1.at(i);
		
		size_t isFound = (size_t)-1;
		for (size_t j = 0; j < nbSection1; j++)							//first, we have to find the linked old Section1
		{
			if (listOffsetForNextSection1.at(j).index == newInfo.index)
			{
				isFound = j;
				break;
			}
		}
		if (isFound == (size_t)-1)							//security
			continue;

		Section1BinaryInformations &oldInfo = listOffsetForNextSection1.at(isFound);

		memcpy(newBuf + newInfo.startOffset, buf + oldInfo.startOffset, newInfo.sizeWithoutEmg);					//copy of the common part;


		//update offsets
		section1 = (EMP_Section1*)GetOffsetPtr(newBuf, newInfo.startOffset);

		
		if (newInfo.indexSibling != (size_t)-1)
		{
			isFound = (size_t)-1;
			for (size_t j = 0; j < nbSection1; j++)							//first, we have to find the linked old Section1
			{
				if (listNewSection1.at(j).index == newInfo.indexSibling)
				{
					isFound = j;
					break;
				}
			}
			(*section1).offsetSiblingSection1 = (isFound != (size_t)-1) ? (listNewSection1.at(isFound).startOffset - newInfo.startOffset) : 0;
		}

		if (newInfo.indexChild != (size_t)-1)
		{
			isFound = (size_t)-1;
			for (size_t j = 0; j < nbSection1; j++)							//first, we have to find the linked old Section1
			{
				if (listNewSection1.at(j).index == newInfo.indexChild)
				{
					isFound = j;
					break;
				}
			}
			(*section1).offsetChildSection1 = (isFound != (size_t)-1) ? (listNewSection1.at(isFound).startOffset - newInfo.startOffset) : 0;
		}

		
		if ((section1[0].section3_type_b == 0x02))						//offset Section2
		{
			size_t offset_Section3 = newInfo.startOffset + sizeof(EMP_Section1);

			EMP_Section3_TexturePart* textPart = (EMP_Section3_TexturePart*)GetOffsetPtr(newBuf, offset_Section3);

			offset_tmp = (*textPart).offsetToOffsetToSection2;
			if (offset_tmp)
			{
				offset_tmp += newInfo.startOffset;

				uint32_t* section3_b = (uint32_t*)GetOffsetPtr(newBuf, offset_tmp);
				offset_tmp = section3_b[0];
				if (offset_tmp)
					section3_b[0] += sizeToAddAfterAllChage;
			}
		}
		

		


		if(!listEmg.at(i))
			continue;

		emg = listEmg.at(i);
		
		size_t emg_size = emg->CalculatePartSize();
		if (emg_size & 0xF)
			emg_size += (0x10 - (emg_size & 0xF));

		uint32_t startOffset_Emg = newInfo.startOffset_Emg + newInfo.startOffset;
		uint32_t tmp = 0;
		
		uint32_t offsetTmp = emg->CreatePart(newBuf + startOffset_Emg, 0, &tmp);
		if (offsetTmp & 0xf)
			offsetTmp += (0x10 - (offsetTmp & 0xF));

		
		//name_offset
		*((uint32_t*)GetOffsetPtr(newBuf, offsetTmp + startOffset_Emg)) = offsetTmp + sizeof(uint32_t) + 0x20;
		offsetTmp += sizeof(uint32_t);


		strcpy((char *)newBuf + startOffset_Emg + offsetTmp, emg->name.c_str());
		offsetTmp += (emg->name.length() + 1) * sizeof(char);
		if (offsetTmp & 0xF)
			offsetTmp += (0x10 - (offsetTmp & 0xF));


		
		*((uint32_t*)GetOffsetPtr(newBuf, startOffset_Emg + 0x24)) = offsetTmp - 0x10;			//not the totaly start of the header, the first entry.
		offsetTmp  += emg->CreateVertex(newBuf + startOffset_Emg + offsetTmp);
		if (offsetTmp & 0xF)
			offsetTmp += (0x10 - (offsetTmp & 0xF));



		{
			size_t offset_Section3 = newInfo.startOffset + sizeof(EMP_Section1);

			EMP_Section3_04_02* section3 = (EMP_Section3_04_02*)GetOffsetPtr(newBuf, offset_Section3);

			(*section3).offsetEmg = newInfo.startOffset_Emg;
		}

		delete emg;
		listEmg.at(i) = 0;
	}




	
	//write the buffer.
	string ext = LibXenoverse::extensionFromFilename(filename, true);
	string name = LibXenoverse::nameFromFilenameNoExtension(filename, true);
	string folder = LibXenoverse::folderFromFilename(filename);
	
	bool ret = WriteFileBool(folder + name +"_modified."+ ext, newBuf, newFileSize, true, false);
	delete[] buf;
	delete[] newBuf;
}





}