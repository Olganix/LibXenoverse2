
#include "EMM.h"

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
void EMM::save_Coloration(string filename, bool show_error)
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
void EMM::write_Coloration(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	EMM_Header* hdr = (EMM_Header*)buf;
	if (size < sizeof(EMM_Header) || memcmp(hdr->signature, LIBXENOVERSE_EMM_SIGNATURE, 4) != 0)
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
	binCt.write_Coloration_Tag("offset_materials", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + hdr->offset_materials, true) + " EMM_Section", offset, sizeof(uint32_t), "EMM_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

	if (hdr->header_size > 0x10)
	{
		incSection++;
		incParam = 0;

		size_t offset_EMM_Header_Sub = offset;
		EMM_Header_Sub* emm_Header_Sub = (EMM_Header_Sub*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_EMM_Header_Sub));
		
		binCt.write_Coloration_Tag("offset_unknowValues", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + emm_Header_Sub->offset_unknowValues, true) + " EMM_Section", offset, sizeof(uint32_t), "EMM_Header_Sub", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		if(hdr->header_size > 0x14)
			binCt.write_Coloration_Tag("unknown_0", "uint32_t", "padding", offset, sizeof(uint32_t), "EMM_Header_Sub", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		if (hdr->header_size > 0x18)
			binCt.write_Coloration_Tag("unknown_1", "uint32_t", "padding", offset, sizeof(uint32_t), "EMM_Header_Sub", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		if (hdr->header_size > 0x1C)
			binCt.write_Coloration_Tag("unknown_2", "uint32_t", "padding", offset, sizeof(uint32_t), "EMM_Header_Sub", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		if ((emm_Header_Sub->offset_unknowValues)&&(startAdress + emm_Header_Sub->offset_unknowValues < size))
		{
			size_t offset_unknowValues = offset = startAdress + emm_Header_Sub->offset_unknowValues;
			uint8_t* unknowValues = (uint8_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_unknowValues));

			binCt.write_Coloration_Tag("unknowValues", "68 * uint8_t", "", offset, 68 * sizeof(uint8_t), "EMM_unknowValues", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 68 * sizeof(uint8_t);
		}

	}else {
		incSection++;
	}
	
	size_t offset_EMM_Section = offset = hdr->offset_materials;
	uint32_t* number_materials = (uint32_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_EMM_Section));
	incSection++;
	incParam = 0;

	binCt.write_Coloration_Tag("number_materials", "uint32_t", "", offset, sizeof(uint32_t), "EMM_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	

	uint32_t* offset_materials = (uint32_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_EMM_Section + sizeof(uint32_t)));
	for (size_t i = 0; i < *number_materials; i++)
	{
		incSection = 2;
		size_t offset_EMM_Material_Section = offset_EMM_Section + offset_materials[i];
		offset = offset_EMM_Material_Section;

		binCt.write_Coloration_Tag("offset_material", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMM_Material_Section, true) + " Ref to list adress to EMM_Material_Section", offset_EMM_Section + sizeof(uint32_t) + i * sizeof(uint32_t), sizeof(uint32_t), "EMM_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);


		EMM_Material_Section* emm_Material_Section = (EMM_Material_Section*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
		incParam = 0;

		binCt.write_Coloration_Tag("name", "32 x char", "", offset, 32 * sizeof(char), "EMM_Material_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 32 * sizeof(char);
		binCt.write_Coloration_Tag("shaderProgName", "32 x char", "", offset, 32 * sizeof(char), "EMM_Material_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 32 * sizeof(char);
		binCt.write_Coloration_Tag("number_parameters", "uint16_t", "", offset, sizeof(uint16_t), "EMM_Material_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unknown_0", "uint16_t", "", offset, sizeof(uint16_t), "EMM_Material_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);



		incSection = 3;
		incParam = 0;
		size_t nbParams = emm_Material_Section->number_parameters;					//notice: in 64 files, there is andom old memory from a old EMM_Parameter_Section removed (case nb_parameter == 0)
		for (size_t j = 0; j < nbParams; j++)
		{
			size_t offset_EMM_Parameter_Section = offset_EMM_Material_Section + sizeof(EMM_Material_Section) + j * sizeof(EMM_Parameter_Section);
			offset = offset_EMM_Parameter_Section;

			EMM_Parameter_Section* emm_Parameter_Section = (EMM_Parameter_Section*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));

			binCt.write_Coloration_Tag("name", "32 x char", "", offset, 32 * sizeof(char), "EMM_Parameter_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 32 * sizeof(char);
			binCt.write_Coloration_Tag("type", "uint16_t", " 0: Float, 1: uint", offset, sizeof(uint16_t), "EMM_Parameter_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("unknown_0", "uint16_t", "", offset, sizeof(uint16_t), "EMM_Parameter_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("value", "uint32_t or float32", "", offset, sizeof(uint32_t), "EMM_Parameter_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		}
	}

	binCt.write_Coloration_Gaps(0x0, size, sizeof(uint8_t), idTag, "Empty", parent, listBytesAllreadyTagged);		//test Empty/gaps/holes.
}





}