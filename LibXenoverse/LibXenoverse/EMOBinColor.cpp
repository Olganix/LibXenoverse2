
#include "EMO.h"

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
void EMO::save_Coloration(string filename, bool show_error)
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
|                             save_Coloration		                             |
\-------------------------------------------------------------------------------*/
void EMA::save_Coloration(string filename, bool show_error)
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
void EMO::write_Coloration(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	EMO_Header* hdr = (EMO_Header*)buf;
	if (size < sizeof(EMO_Header) || hdr->signature != EMO_SIGNATURE)
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
	bool vertexInside = (hdr->vertex_offset == 0);

	binCt.write_Coloration_Tag("signature", "string", "", offset, 4 * sizeof(char), "EMO_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(char);
	binCt.write_Coloration_Tag("endian", "uint16_t", "", offset, sizeof(uint16_t), "EMO_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("header_size", "uint16_t", "", offset, sizeof(uint16_t), "EMO_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("version", "4 * uint8_t", "", offset, 4 * sizeof(uint8_t), "EMO_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(uint8_t);
	binCt.write_Coloration_Tag("parts_offset", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + val32(hdr->parts_offset), true) + " EMO_Section", offset, sizeof(uint32_t), "EMO_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("skeleton_offset", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + val32(hdr->skeleton_offset), true) + " EMO_Section", offset, sizeof(uint32_t), "EMO_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("vertex_offset", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + val32(hdr->vertex_offset), true) + " EMO_Section", offset, sizeof(uint32_t), "EMO_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow_0", "uint32_t", "", offset, sizeof(uint32_t), "EMO_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow_1", "uint32_t", "", offset, sizeof(uint32_t), "EMO_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

	size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
	if (size_padding)
		binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMO_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
	offset += size_padding;

	


	incSection++;
	incParam = 0;
	size_t offset_EMO_PartsGroupHeader = offset = startAdress + val32(hdr->parts_offset);
	EMO_PartsGroupHeader* emo_PartsGroupHeader = (EMO_PartsGroupHeader*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));

	binCt.write_Coloration_Tag("groups_count", "uint16_t", "", offset, sizeof(uint16_t), "EMO_PartsGroupHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("material_count", "uint16_t", "", offset, sizeof(uint16_t), "EMO_PartsGroupHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("names_offset", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + val32(emo_PartsGroupHeader->names_offset), true) + " Names", offset, sizeof(uint32_t), "EMO_PartsGroupHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);


	uint32_t* offset_EMO_PartHeaders = (uint32_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));

	offset += emo_PartsGroupHeader->groups_count * sizeof(uint32_t);
	size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
	if (size_padding)
		binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMO_PartHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
	offset += size_padding;


	for (size_t i = 0; i < emo_PartsGroupHeader->groups_count; i++)
	{
		incSection = 2;
		size_t offset_EMO_PartHeader = offset = offset_EMO_PartsGroupHeader + offset_EMO_PartHeaders[i];

		binCt.write_Coloration_Tag("offset_EMO_PartHeader", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMO_PartHeader, true) + " Ref to list adress to EMO_Material_Section", offset_EMO_PartsGroupHeader + 0x8 + i * sizeof(uint32_t), sizeof(uint32_t), "EMO_PartsGroupHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);


		EMO_PartHeader* emo_PartHeader = (EMO_PartHeader*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
		incParam = 0;

		binCt.write_Coloration_Tag("emg_count", "uint32_t", "", offset, sizeof(uint32_t), "EMO_PartHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		incSection = 3;
		incParam = 0;
		uint32_t* offset_EMGs = (uint32_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));

		offset += emo_PartHeader->emg_count * sizeof(uint32_t);
		size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
		if (size_padding)
			binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMO_PartHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
		offset += size_padding;


		for (size_t j = 0; j < emo_PartHeader->emg_count; j++)
		{
			incSection = 2;
			size_t offset_EMG = offset = offset_EMO_PartHeader + offset_EMGs[j];

			binCt.write_Coloration_Tag("offset_EMG", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMG, true) + " Ref to list adress to Emg", offset_EMO_PartHeader + 0x4 + j * sizeof(uint32_t), sizeof(uint32_t), "EMO_PartHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);

			if (offset_EMGs[j])
			{
				EMG emg;
				size_t end_Emg_vertex_offset = emg.write_Coloration_EMG(binCt, parent, GetOffsetPtr(buf, offset_EMG), size - val32(offset_EMG), offset_EMG, listBytesAllreadyTagged, vertexInside);

				if ((i + 1 != emo_PartsGroupHeader->groups_count) || (j + 1 != emo_PartHeader->emg_count))						//not on the last.
				{
					offset = end_Emg_vertex_offset;
					size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
					if (size_padding)
						binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMG", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
					offset += size_padding;
				}
				
			}
		}
	}

	incSection++;
	incParam = 0;
	if (emo_PartsGroupHeader->names_offset)
	{
		offset = offset_EMO_PartsGroupHeader + emo_PartsGroupHeader->names_offset;
		size_t endOffset = offset;
		for (size_t i = 0; i < emo_PartsGroupHeader->groups_count; i++)
		{
			name_offset = (uint32_t*)EMO_BaseFile::GetOffsetPtr_native(buf, offset);

			binCt.write_Coloration_Tag("offset_group_name", "uint32_t", "", offset, sizeof(uint32_t), "EMO_GroupNames", parent, idTag++, incSection, i, listBytesAllreadyTagged); offset += sizeof(uint32_t);

			if (name_offset)
			{
				name_tmp = string((char*)EMO_BaseFile::GetOffsetPtr_native(buf, offset_EMO_PartsGroupHeader + (*name_offset)));
				binCt.write_Coloration_Tag("group_name", "string", "", offset_EMO_PartsGroupHeader + (*name_offset), (name_tmp.size() + 1) * sizeof(char), "EMO_GroupNames", parent, idTag++, incSection, i, listBytesAllreadyTagged, (size_t)-1, false);

				size_t endoffset_tmp = offset_EMO_PartsGroupHeader + (*name_offset) + (name_tmp.size() + 1) * sizeof(char);
				if (endOffset < endoffset_tmp)
					endOffset = endoffset_tmp;
			}
		}
		offset = endOffset;
		size_padding = (size_t)(ceil((double)offset / 64.0) * 64.0) - offset;
		if (size_padding)
			binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMO_GroupNames", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
		offset += size_padding;


	}else {						//case dbxv1 Ibm_Crg.light.emo
		size_padding = (size_t)(ceil((double)offset / 32.0) * 32.0) - offset;
		if (size_padding)
			binCt.write_Coloration_Tag("padding2", "padding", "", offset, size_padding, "EMO_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
		offset += size_padding;
	}


	incParam = 0;
	incSection++;
	if (hdr->skeleton_offset)
	{
		write_Coloration_Skeleton(binCt, parent, GetOffsetPtr(buf, hdr->skeleton_offset), size - val32(hdr->skeleton_offset), hdr->skeleton_offset, ((hdr->vertex_offset!=0)&&((emo_PartsGroupHeader->names_offset!=0))), listBytesAllreadyTagged);
	}


	binCt.write_Coloration_Gaps(0x0, size, sizeof(uint8_t), idTag, "Empty", parent, listBytesAllreadyTagged);		//test Empty/gaps/holes.
}




/*-------------------------------------------------------------------------------\
|                             write_Coloration_EMG			                     |
\-------------------------------------------------------------------------------*/
size_t EMG::write_Coloration_EMG(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size, size_t startOffset, std::vector<bool> &listBytesAllreadyTagged, bool vertexInside)
{
	size_t idTag = 0;
	size_t offset = 0;
	size_t incSection = 0;
	size_t incParam = 0;

	EMG_ChunkHeader *hdr = (EMG_ChunkHeader *)buf;
	offset = startOffset;
	binCt.write_Coloration_Tag("signature", "string", "", offset, 4 * sizeof(char), "EMG_ChunkHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(char);
	binCt.write_Coloration_Tag("unknow_inc", "uint16_t", "", offset, sizeof(uint16_t), "EMG_ChunkHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("subparts_count", "uint16_t", "", offset, sizeof(uint16_t), "EMG_ChunkHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);


	incSection++;
	size_t incSection_EMG_SubPart_Section = incSection;
	size_t incSection_textures_lists = incSection + 1;
	size_t incSection_EMGTextureDef = incSection + 2;
	size_t incSection_EMG_SubMesh = incSection + 3;
	size_t incSection_EMG_Face = incSection + 4;
	size_t incSection_EMG_Bone = incSection + 5;
	size_t incSection_EMG_Vertex = incSection + 6;
	incParam = 0;
	uint32_t* offset_EMG_SubPart_Sections = (uint32_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset - startOffset));


	offset += hdr->subparts_count * sizeof(uint32_t);
	size_t size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
	if (size_padding)
		binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMG_ChunkHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
	offset += size_padding;

	size_t end_Emg_offset = offset;
	size_t end_Emg_vertex_offset = offset;
	for (size_t i = 0; i < hdr->subparts_count; i++)
	{
		incSection = incSection_EMG_SubPart_Section;
		size_t offset_EMG_SubPart_Section = offset = startOffset + offset_EMG_SubPart_Sections[i];

		binCt.write_Coloration_Tag("offset_EMG_SubPart_Section", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMG_SubPart_Section, true) + " Ref to list adress to EMG_SubPart_Section", startOffset + 0x8 + i * sizeof(uint32_t), sizeof(uint32_t), "EMG_ChunkHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);


		EMG_SubPart_Section* emg_SubPart = (EMG_SubPart_Section*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset - startOffset));
		incParam = 0;

		binCt.write_Coloration_Tag("vertex_type_flag", "uint32_t", "", offset, sizeof(uint32_t), "EMG_SubPart_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("textures_lists_count", "uint32_t", "", offset, sizeof(uint32_t), "EMG_SubPart_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("unknow_0", "uint32_t", "", offset, sizeof(uint32_t), "EMG_SubPart_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("textures_lists_offset", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMG_SubPart_Section + val32(emg_SubPart->textures_lists_offset), true) + " EMGTextureDef_Section", offset, sizeof(uint32_t), "EMG_SubPart_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		binCt.write_Coloration_Tag("vertex_count", "uint16_t", "", offset, sizeof(uint16_t), "EMG_SubPart_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("vertex_size", "uint16_t", "", offset, sizeof(uint16_t), "EMG_SubPart_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("vertex_offset", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMG_SubPart_Section + val32(emg_SubPart->vertex_offset), true) + " Vertices", offset, sizeof(uint32_t), "EMG_SubPart_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		
		binCt.write_Coloration_Tag("strips", "uint16_t", "", offset, sizeof(uint16_t), "EMG_SubPart_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("submesh_count", "uint16_t", "", offset, sizeof(uint16_t), "EMG_SubPart_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("submesh_list_offset", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMG_SubPart_Section + val32(emg_SubPart->submesh_list_offset), true) + " EMG_EMG_SubMeshHeader", offset, sizeof(uint32_t), "EMG_SubPart_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("aabb", "12 x float", "center, min and max in vect4", offset, 4 * 3 * sizeof(float), "EMG_SubPart_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * 3 * sizeof(float);

		end_Emg_offset = offset;
		
		if (emg_SubPart->textures_lists_offset)
		{
			incParam = 0;
			uint32_t* textures_lists_offsets = (uint32_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_EMG_SubPart_Section + emg_SubPart->textures_lists_offset - startOffset));
			for (size_t j = 0; j < emg_SubPart->textures_lists_count; j++)
			{
				incSection = incSection_textures_lists;
				size_t offset_textures_lists = offset = offset_EMG_SubPart_Section + textures_lists_offsets[j];

				binCt.write_Coloration_Tag("offset_textures_lists", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_textures_lists, true) + " Ref to list adress to TextureList", offset_EMG_SubPart_Section + emg_SubPart->textures_lists_offset + j * sizeof(uint32_t), sizeof(uint32_t), "EMG_SubPart_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);


				EMG_TextureListHeader* emg_TextureListHeader = (EMG_TextureListHeader*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset - startOffset));
				incParam = 0;

				binCt.write_Coloration_Tag("number_texture", "uint16_t", "", offset, sizeof(uint16_t), "EMG_SubPart_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
				binCt.write_Coloration_Tag("unknow_0", "uint16_t", "", offset, sizeof(uint16_t), "EMG_SubPart_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
				

				for (size_t k = 0; k < emg_TextureListHeader->number_texture; k++)
				{
					incSection = incSection_EMGTextureDef;
					size_t offset_texture = offset = offset_textures_lists + sizeof(EMG_TextureListHeader) + k * sizeof(EMGTextureDef_Section);

					EMGTextureDef_Section* emg_TextureDef_Section = (EMGTextureDef_Section*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset - startOffset));
					incParam = 0;

					binCt.write_Coloration_Tag("flag0", "uint8_t", "", offset, sizeof(uint8_t), "EMGTextureUnitState", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
					binCt.write_Coloration_Tag("emb_TextureIndex", "uint8_t", "", offset, sizeof(uint8_t), "EMGTextureUnitState", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
					binCt.write_Coloration_Tag("adressMode_uv", "uint8_t", "", offset, sizeof(uint8_t), "EMGTextureUnitState", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
					binCt.write_Coloration_Tag("filtering_minMag", "uint8_t", "", offset, sizeof(uint8_t), "EMGTextureUnitState", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
					binCt.write_Coloration_Tag("textScale_u", "float", "", offset, sizeof(float), "EMGTextureUnitState", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
					binCt.write_Coloration_Tag("textScale_v", "float", "", offset, sizeof(float), "EMGTextureUnitState", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
				}

				end_Emg_offset = offset;
			}
		}

		if (emg_SubPart->submesh_list_offset)
		{
			incParam = 0;
			uint32_t* submesh_list_offsets = (uint32_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_EMG_SubPart_Section + emg_SubPart->submesh_list_offset - startOffset));


			offset = offset_EMG_SubPart_Section + emg_SubPart->submesh_list_offset + emg_SubPart->submesh_count * sizeof(uint32_t);
			size_t size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
			if (size_padding)
				binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMG_EMG_SubMeshHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
			offset += size_padding;

			for (size_t j = 0; j < emg_SubPart->submesh_count; j++)
			{
				incSection = incSection_EMG_SubMesh;
				size_t offset_submesh = offset = offset_EMG_SubPart_Section + submesh_list_offsets[j];

				binCt.write_Coloration_Tag("offset_submesh", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_submesh, true) + " Ref to list adress to EMG_EMG_SubMeshHeader", offset_EMG_SubPart_Section + emg_SubPart->submesh_list_offset + j * sizeof(uint32_t), sizeof(uint32_t), "EMG_SubPart_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);


				EMG_EMG_SubMeshHeader* emg_SubMesh = (EMG_EMG_SubMeshHeader*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset - startOffset));
				incParam = 0;

				binCt.write_Coloration_Tag("barycenter", "4 x float", "", offset, 4 * sizeof(float), "EMG_EMG_SubMeshHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset +=  4 * sizeof(float);
				binCt.write_Coloration_Tag("textureList_index", "uint16_t", "", offset, sizeof(uint16_t), "EMG_EMG_SubMeshHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
				binCt.write_Coloration_Tag("face_count", "uint16_t", "", offset, sizeof(uint16_t), "EMG_EMG_SubMeshHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
				binCt.write_Coloration_Tag("linked_bones_count", "uint16_t", "", offset, sizeof(uint16_t), "EMG_EMG_SubMeshHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
				binCt.write_Coloration_Tag("emm_material_name", "0x20 x char", "", offset, 0x20 * sizeof(char), "EMG_EMG_SubMeshHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 0x20 * sizeof(char);

				incSection = incSection_EMG_Face;
				incParam = 0;
				if (emg_SubMesh->face_count)
				{
					/*
					for (size_t k = 0; k < emg_SubMesh->face_count; k++)
					{
						binCt.write_Coloration_Tag("face_index", "uint16_t", "", offset, sizeof(uint16_t), "EMG_EMG_SubMeshHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
					}
					*/
					//some of tags file still to 220 Mo , have to reduce, so here one block for all faces.
					binCt.write_Coloration_Tag("face_index x " + ToString(emg_SubMesh->face_count), "uint16_t", "", offset, sizeof(uint16_t) * emg_SubMesh->face_count, "EMG_EMG_SubMeshHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t) * emg_SubMesh->face_count;
				}


				incSection = incSection_EMG_Bone;
				incParam = 0;
				if (emg_SubMesh->linked_bones_count)
				{
					/*
					for (size_t k = 0; k < emg_SubMesh->linked_bones_count; k++)
					{
						binCt.write_Coloration_Tag("bone_index", "uint16_t", "", offset, sizeof(uint16_t), "EMG_EMG_SubMeshHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
					}
					*/
					//some of tags file still to 220 Mo , have to reduce, so here one block for all bones.
					binCt.write_Coloration_Tag("bone_index x " + ToString(emg_SubMesh->linked_bones_count), "uint16_t", "", offset, sizeof(uint16_t) * emg_SubMesh->linked_bones_count, "EMG_EMG_SubMeshHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t) * emg_SubMesh->linked_bones_count;
				}

				if ((j + 1 != emg_SubPart->submesh_count) || (i + 1 != hdr->subparts_count))						//not on the last.
				{
					size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
					if (size_padding)
						binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMG_EMG_SubMeshHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
					offset += size_padding;
				}


				end_Emg_offset = offset;
			}
		}
		
		


		if (emg_SubPart->vertex_offset)
		{
			incSection = incSection_EMG_Vertex;
			incParam = 0;
			size_t offset_vertices = offset = offset_EMG_SubPart_Section + emg_SubPart->vertex_offset;

			string infosVertex = "Vert" + ToString(emg_SubPart->vertex_size) +
								((emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_POS) ? "p" : "") +
								((emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_NORM) ? "n" : "") +
								((emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_TEX) ? "u" : "") +
								((emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_TEX2) ? "v" : "") +
								((emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_TANGENT) ? "t" : "") +
								((emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_COLOR) ? "c" : "") +
								((emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_BLEND_WEIGHT) ? "B" : "") +
								((emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_COMPRESSED_FORMAT) ? "L" : "");

			/*
			for (size_t j = 0; j < emg_SubPart->vertex_count; j++)
			{
				size_t offset_vertices_tmp = offset = offset_vertices + j * emg_SubPart->vertex_size;

				if (j != 0)
				{
					binCt.write_Coloration_Tag(ToString(j) +"/"+ ToString(emg_SubPart->vertex_count) + "_" + infosVertex, "float", "", offset, emg_SubPart->vertex_size, "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += emg_SubPart->vertex_size;

				} else {


					if (emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_POS)
					{
						binCt.write_Coloration_Tag("posX", "float", "", offset, sizeof(float), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
						binCt.write_Coloration_Tag("posY", "float", "", offset, sizeof(float), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
						binCt.write_Coloration_Tag("posZ", "float", "", offset, sizeof(float), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
					}

					if (!(emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_COMPRESSED_FORMAT))
					{
						if (emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_NORM)
						{
							binCt.write_Coloration_Tag("normX", "float", "", offset, sizeof(float), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
							binCt.write_Coloration_Tag("normY", "float", "", offset, sizeof(float), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
							binCt.write_Coloration_Tag("normZ", "float", "", offset, sizeof(float), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
						}
						if (emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_TEX)
						{
							binCt.write_Coloration_Tag("uvU", "float", "", offset, sizeof(float), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
							binCt.write_Coloration_Tag("uvV", "float", "", offset, sizeof(float), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
						}
						if (emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_TEX2)
						{
							binCt.write_Coloration_Tag("uv2U", "float", "", offset, sizeof(float), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
							binCt.write_Coloration_Tag("uv2V", "float", "", offset, sizeof(float), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
						}
						if (emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_TANGENT)
						{
							binCt.write_Coloration_Tag("tangX", "float", "", offset, sizeof(float), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
							binCt.write_Coloration_Tag("tangY", "float", "", offset, sizeof(float), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
							binCt.write_Coloration_Tag("tangZ", "float", "", offset, sizeof(float), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
						}

					}
					else {

						if (emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_NORM)
						{
							binCt.write_Coloration_Tag("normX", "float16", "", offset, sizeof(uint16_t), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
							binCt.write_Coloration_Tag("normY", "float16", "", offset, sizeof(uint16_t), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
							binCt.write_Coloration_Tag("normZ", "float16", "", offset, sizeof(uint16_t), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
							binCt.write_Coloration_Tag("padding", "padding", "", offset, sizeof(uint16_t), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
						}
						if (emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_TEX)
						{
							binCt.write_Coloration_Tag("uvU", "float16", "", offset, sizeof(uint16_t), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
							binCt.write_Coloration_Tag("uvV", "float16", "", offset, sizeof(uint16_t), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
						}
						if (emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_TEX2)
						{
							binCt.write_Coloration_Tag("uv2U", "float16", "", offset, sizeof(uint16_t), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
							binCt.write_Coloration_Tag("uv2V", "float16", "", offset, sizeof(uint16_t), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
						}
						if (emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_TANGENT)
						{
							binCt.write_Coloration_Tag("tangX", "float16", "", offset, sizeof(uint16_t), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
							binCt.write_Coloration_Tag("tangY", "float16", "", offset, sizeof(uint16_t), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
							binCt.write_Coloration_Tag("tangZ", "float16", "", offset, sizeof(uint16_t), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
							binCt.write_Coloration_Tag("padding", "padding", "", offset, sizeof(uint16_t), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
						}
					}

					if (emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_COLOR)
					{
						binCt.write_Coloration_Tag("color", "color32", "", offset, sizeof(uint32_t), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
					}

					if (emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_BLEND_WEIGHT)
					{
						binCt.write_Coloration_Tag("4 x blend_bone_index", "4 * uint8_t", "", offset, sizeof(uint32_t), "EMG_EMG_SubMeshHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

						if (!(emg_SubPart->vertex_type_flag & EMG_VTX_FLAG_COMPRESSED_FORMAT))
						{
							//binCt.write_Coloration_Tag("4 x blend_bone_weight", "4 x float", "", offset, 4 * sizeof(float), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);		//work on dbxv2 files, but not on saintseya's files.
							binCt.write_Coloration_Tag("3 x blend_bone_weight (4nd guessed)", "3 x float", "", offset, 3 * sizeof(float), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 3 * sizeof(float);
						}
						else {
							binCt.write_Coloration_Tag("4 x blend_bone_weight", "4 x float16", "", offset, 4 * sizeof(uint16_t), "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(uint16_t);
						}
					}
				}
			}
			*/
			
			
			//some of tags file still to 220 Mo , have to reduce, so hereone block for all vertex.
			binCt.write_Coloration_Tag(infosVertex + " x "+ ToString(emg_SubPart->vertex_count) + "___" + EMO_BaseFile::UnsignedToString(offset_vertices - offset_EMG_SubPart_Section, true), "float", "", offset, emg_SubPart->vertex_size * emg_SubPart->vertex_count, "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += emg_SubPart->vertex_size * emg_SubPart->vertex_count;
			
			if (vertexInside)
			{
				size_padding = (size_t)(ceil((double)(offset - offset_vertices) / 16.0) * 16.0) - (offset - offset_vertices);
				if (size_padding)
					binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
				offset += size_padding;
			}


			if ((i + 1 != hdr->subparts_count) || (vertexInside))				//not on the last or vertex inside EMG (case SSSSS BBdr_BossBas.eo.emo)
			{
				size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
				if (size_padding)
					binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMG_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
				offset += size_padding;

			}

			end_Emg_vertex_offset = offset;
		}
	}

	offset = end_Emg_offset;												//vertex are after skeleton, and it's a merge of all emg vertex. it's weird , but it's like that
	double padding = (vertexInside) ? 4.0 : 16.0;							//case SSSSS BBdr_BossBas.eo.emo
	size_padding = (size_t)(ceil((double)offset / padding) * padding) - offset;
	if (size_padding)
		binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMG", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
	offset += size_padding;

	return end_Emg_vertex_offset;
}



/*-------------------------------------------------------------------------------\
|                             write_Coloration_Skeleton		                     |
\-------------------------------------------------------------------------------*/
void EMO_Skeleton::write_Coloration_Skeleton(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size, size_t startOffset_Skeleton, bool addLastPadding, std::vector<bool> &listBytesAllreadyTagged)
{
	size_t idTag = 0;
	size_t offset = 0;
	size_t incSection = 0;
	size_t incParam = 0;


	bool isSfiv = false;

	SkeletonHeader *hdr = (SkeletonHeader *)buf;
	offset = startOffset_Skeleton;
	binCt.write_Coloration_Tag("node_count", "uint16_t", "", offset, sizeof(uint16_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("ik2_count", "uint16_t", "", offset, sizeof(uint16_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("ik_count", "uint32_t", "", offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("bones_offset", "uint32_t", "", offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("names_offset", "uint32_t", "", offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("ik2_offset", "uint32_t", "", offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("ik2_names_offset", "uint32_t", "", offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("offset_boneExtraInfo", "uint32_t", "", offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("offset_absMatrices", "uint32_t", "", offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("ik_data_offset", "uint32_t", "", offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow_2", "uint32_t", "", offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow_3", "uint32_t", "", offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow_4", "uint32_t", "", offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow_5", "uint32_t", "", offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow_6", "uint32_t", "", offset, sizeof(uint32_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("skeletonUniqueId", "uint64_t", "like for esk", offset, sizeof(uint64_t), "SkeletonHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint64_t);



	incSection++;
	incParam = 0;
	size_t incSection_IKs = incSection;
	size_t incSection_IkGroup = incSection + 1;
	size_t incSection_Ik = incSection + 2;
	size_t incSection_IkRelation = incSection + 3;
	size_t incSection_IkRelationB = incSection + 4;

	size_t incSection_Ik2 = incSection + 5;
	size_t incSection_Ik2Relation = incSection + 6;
	size_t incSection_Ik2nameOffset = incSection + 7;
	size_t incSection_Ik2names = incSection + 8;


	uint16_t node_count = val32(hdr->node_count);

	if (hdr->ik_data_offset)
	{
		/*
		size_t ik_size = 0;
		for (uint16_t i = 0; i < val32(hdr->ik_count); i++)
			ik_size += val16(*(uint16_t *)(buf + startOffset_data + ik_size + 2));

		offset = startOffset_Skeleton + startOffset_data;			//Todo change blob by using like esk.
		binCt.write_Coloration_Tag("Data InverseKinematic", "blob", "", offset, ik_size, "DataIK", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += ik_size;
		*/

		
		//no ESK_IK_Section here because we allready have the hdr->ik_count
		offset = startOffset_Skeleton + hdr->ik_data_offset;
		

		for (size_t i = 0; i < hdr->ik_count; i++)
		{
			incSection = incSection_IkGroup;
			incParam = 0;

			size_t offset_ESK_IK_Group = offset;

			ESK_IK_Group* esk_IK_Group = (ESK_IK_Group*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset - startOffset_Skeleton));
			binCt.write_Coloration_Tag("typeGroup", "uint16_t", "always 1 (for 'weighted'), except for SSSS HSY.ema and some of streetFighters game, with 0 for equals", offset, sizeof(uint16_t), "ESK_IK_Group", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("sizeGroup", "uint16_t", "number of bytes for the groups", offset, sizeof(uint16_t), "ESK_IK_Group", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			
			if (esk_IK_Group->typeGroup == 1)				//index + weight for X relationIKs
			{
				incSection = incSection_Ik;
				incParam = 0;

				ESK_IK* esk_IK = (ESK_IK*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset - startOffset_Skeleton));
				binCt.write_Coloration_Tag("unknow_0", "uint8_t", "", offset, sizeof(uint8_t), "ESK_IK", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
				binCt.write_Coloration_Tag("number_relations", "uint8_t", "", offset, sizeof(uint8_t), "ESK_IK", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
				uint8_t number_relations = esk_IK->number_relations;

				incSection = incSection_IkRelation;
				incParam = 0;

				
				for (size_t k = 0; k <= number_relations; k++)
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


			}else if (esk_IK_Group->typeGroup == 0) {					//case SSSS HSY.ema: 5 index to follow, no weight

				incSection = incSection_Ik;
				incParam = 0;

				ESK_IK* esk_IK = (ESK_IK*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset - startOffset_Skeleton));
				binCt.write_Coloration_Tag("unknow_0", "uint8_t", "", offset, sizeof(uint8_t), "ESK_IK_b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
				binCt.write_Coloration_Tag("unknow_1", "uint8_t", "", offset, sizeof(uint8_t), "ESK_IK_b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
				uint8_t number_relations = esk_IK->number_relations;

				incSection = incSection_IkRelation;
				incParam = 0;

				for (size_t k = 0; k <= 4; k++)
				{
					binCt.write_Coloration_Tag("boneIndex (special case)", "uint16_t", "", offset, sizeof(uint16_t), "ESK_IK_Relation_b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
				}
			}
			offset = offset_ESK_IK_Group + esk_IK_Group->sizeGroup;
		}
		incSection = incSection_IkRelationB;

	}
	



	if (hdr->ik2_offset)
	{
		size_t offset_ESK_IK_2 = offset = startOffset_Skeleton + hdr->ik2_offset;

		
		ESK_IK_2* esk_IK_2 = (ESK_IK_2*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset - startOffset_Skeleton));
		uint16_t* esk_IK_2_listBones = (uint16_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, (offset + hdr->ik2_count * sizeof(ESK_IK_2)) - startOffset_Skeleton));

		size_t inc_bones = 0;
		for (size_t i = 0; i < hdr->ik2_count; i++)
		{
			incSection = incSection_Ik2;
			incParam = 0;

			offset = offset_ESK_IK_2 + i * sizeof(ESK_IK_2);
			
			binCt.write_Coloration_Tag("number_bones", "uint32_t", "", offset, sizeof(uint32_t), "ESK_IK_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			binCt.write_Coloration_Tag("unknow_0", "uint32_t", "", offset, sizeof(uint32_t), "ESK_IK_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);


			incSection = incSection_Ik2Relation;
			incParam = 0;

			offset = offset_ESK_IK_2 + hdr->ik2_count * sizeof(ESK_IK_2) + inc_bones * sizeof(uint16_t);
			binCt.write_Coloration_Tag("bonesIndexes (uint16)", ToString(esk_IK_2[i].number_bones) + " * uint16_t", "", offset, esk_IK_2[i].number_bones * sizeof(uint16_t), "ESK_IK_2_Bones", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += esk_IK_2[i].number_bones * sizeof(uint16_t);
			inc_bones += esk_IK_2[i].number_bones;
		}

		size_t size_padding = (size_t)(ceil((double)offset / 4.0) * 4.0) - offset;
		if (size_padding)
			binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "ESK_IK_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
		
		incParam = 0;
		offset = startOffset_Skeleton + val32(hdr->ik2_names_offset);
		uint32_t* esk_IK_2_namesOffsets = (uint32_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset - startOffset_Skeleton));

		string name;
		for (size_t i = 0; i < hdr->ik2_count; i++)
		{
			incSection = incSection_Ik2nameOffset;
			binCt.write_Coloration_Tag("nameoffsets", "uint32_t", " => "+ UnsignedToString(startOffset_Skeleton + esk_IK_2_namesOffsets[i], true), offset, sizeof(uint32_t), "ESK_IK_2_nameOffsets", parent, idTag++, incSection, incParam, listBytesAllreadyTagged); offset += sizeof(uint32_t);

			incSection = incSection_Ik2names;
			name = std::string((char *)GetOffsetPtr(buf, esk_IK_2_namesOffsets[i]));
			binCt.write_Coloration_Tag("ik2_name", "string", "", startOffset_Skeleton + esk_IK_2_namesOffsets[i], name.length() + 1, "ESK_IK_2_names", parent, idTag++, incSection, incParam, listBytesAllreadyTagged, i);

			incParam++;
		}
		if (hdr->ik2_count != 0)
			offset = startOffset_Skeleton + esk_IK_2_namesOffsets[hdr->ik2_count - 1] + name.length() + 1;
		incSection = incSection_Ik2names;

		size_padding = (size_t)(ceil((double)offset / 4.0) * 4.0) - offset;
		if (size_padding)
			binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "ESK_IK_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
		offset += size_padding;
	}


	uint32_t *names_table = (uint32_t *)GetOffsetPtr(buf, hdr->names_offset);
	SkeletonNode *nodes = (SkeletonNode*)GetOffsetPtr(buf, hdr->bones_offset);
	EMO_Ske_Bone_extraInfo *unks = nullptr;
	EMO_Ske_Bone_Absolute_Matrix *matrixes = nullptr;


	


	incParam = 0;
	incSection++;
	if (hdr->offset_boneExtraInfo)
	{
		unks = (EMO_Ske_Bone_extraInfo*)GetOffsetPtr(buf, hdr->offset_boneExtraInfo);

		offset = startOffset_Skeleton + hdr->offset_boneExtraInfo;
		for (size_t i = 0; i < node_count; i++)
		{
			incParam = 0;
			binCt.write_Coloration_Tag("unknow_0", "uint16_t", "", offset, sizeof(uint16_t), "EMO_Ske_Bone_extraInfo", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("unknow_1", "uint16_t", "", offset, sizeof(uint16_t), "EMO_Ske_Bone_extraInfo", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("unknow_2", "uint16_t", "", offset, sizeof(uint16_t), "EMO_Ske_Bone_extraInfo", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("unknow_3", "uint16_t", "", offset, sizeof(uint16_t), "EMO_Ske_Bone_extraInfo", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		}
	}


	incParam = 0;
	incSection++;

	if (hdr->names_offset)
	{
		offset = startOffset_Skeleton + hdr->names_offset;
		for (size_t i = 0; i < node_count; i++)
		{
			binCt.write_Coloration_Tag("nameOffset", "uint32_t", "", offset, sizeof(uint32_t), "NameOffset", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		}
	}



	size_t startNames = 0;
	size_t offset_lastBoneName = 0;
	for (uint16_t i = 0; i < node_count; i++)
	{
		EMO_Bone bone;

		size_t startSkeletonNode = hdr->bones_offset + i * sizeof(SkeletonNode);
		

		incParam = 0;
		incSection = 5;
		offset = startOffset_Skeleton + startSkeletonNode;
		binCt.write_Coloration_Tag("parent_id", "uint16_t", "", offset, sizeof(uint16_t), "SkeletonNode", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("child_id", "uint16_t", "", offset, sizeof(uint16_t), "SkeletonNode", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("sibling_id", "uint16_t", "", offset, sizeof(uint16_t), "SkeletonNode", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("emgIndex", "uint16_t", "", offset, sizeof(uint16_t), "SkeletonNode", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unknow_0", "uint16_t", "", offset, sizeof(uint16_t), "SkeletonNode", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("ik_flag", "uint16_t", "", offset, sizeof(uint16_t), "SkeletonNode", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unknow_1", "float", " may be length or witdh TODO ", offset, sizeof(float), "SkeletonNode", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("relativeMatrix", "matrix4x4", "", offset, 4 * 4 * sizeof(float), "SkeletonNode", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset +=  4 * 4  * sizeof(float);
		

		incParam = 0;
		incSection++;

		bone.meta_original_offset = EMO_BaseFile::DifPointer(&nodes[i], buf);
		bone.name = std::string((char *)GetOffsetPtr(buf, names_table, i));
		offset = startOffset_Skeleton + names_table[i];
		if(i==0)
			startNames = startOffset_Skeleton + names_table[0];
		binCt.write_Coloration_Tag("BoneName", "string", "", startOffset_Skeleton + names_table[i], bone.name.length() + 1, "SkeletonName", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += bone.name.length() + 1;
		offset_lastBoneName = offset;
	}



	if ((offset_lastBoneName) && ((addLastPadding) || (hdr->offset_absMatrices)) )
	{
		float padding = (isSfiv) ? 4.0f : 16.0f;
		size_t size_padding = (size_t)(ceil((double)offset_lastBoneName / padding) * padding) - offset_lastBoneName;
		if (size_padding)
			binCt.write_Coloration_Tag("padding", "padding", "", offset_lastBoneName, size_padding, "SkeletonName", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
	}


	/*
	//todo find the way to do this padding.
	if (addLastPadding)
	{
		startNames += (size_t)(ceil((double)(startNames) / 8.0) * 8.0) - startNames;
		//startNames = (node_count) ? (startOffset_Skeleton + hdr->names_offset + 8) : offset;			// begin to nameoffsets, with a decal of 8 ? Todo remove

		//size_t size_padding = (size_t)(ceil((double) (offset - startNames) / 16.0) * 16.0) - (offset - startNames);
		size_t size_padding = (size_t)(ceil((double)(offset - startNames) / 8.0) * 8.0) - (offset - startNames);				//todo better , it's just about find real error , so some of will have empty because this padding is not deal well until now..
		if (size_padding)
			binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "SkeletonName", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
		offset += size_padding;
	}
	*/

	/*
	if (addLastPadding)
	{
		size_t size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;				//todo better , it's just about find real error , so some of will have empty because this padding is not deal well until now..
		if (size_padding)
			binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "SkeletonName", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
		offset += size_padding;
	}
	*/



	incParam = 0;
	incSection++;
	if (hdr->offset_absMatrices)
	{
		matrixes = (EMO_Ske_Bone_Absolute_Matrix *)GetOffsetPtr(buf, hdr->offset_absMatrices);

		offset = startOffset_Skeleton + hdr->offset_absMatrices;

		for (uint16_t i = 0; i < node_count; i++)
		{
			binCt.write_Coloration_Tag("absoluteMatrix", "matrix4x4", "", offset, 4 * 4 * sizeof(float), "EMO_Ske_Bone_Absolute_Matrix", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * 4 * sizeof(float);
		}
	}
}










/*-------------------------------------------------------------------------------\
|                             write_Coloration				                     |
\-------------------------------------------------------------------------------*/
void EMA::write_Coloration(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size)
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
	size_t size_padding = 0;

	binCt.write_Coloration_Tag("signature", "string", "", offset, 4 * sizeof(char), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);	offset += 4 * sizeof(char);
	binCt.write_Coloration_Tag("endianess_check", "uint16_t", "", offset, sizeof(uint16_t), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("header_size", "uint16_t", "", offset, sizeof(uint16_t), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("version", "4 * uint8_t", "", offset, 4 * sizeof(uint8_t), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(uint8_t);
	binCt.write_Coloration_Tag("skeleton_offset", "uint32_t", "", offset, sizeof(uint32_t), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("anim_count", "uint16_t", "", offset, sizeof(uint16_t), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("type", "uint16_t", "", offset, sizeof(uint16_t), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("unknow_0", "uint32_t", "", offset, sizeof(uint32_t), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow_1", "uint32_t", "", offset, sizeof(uint32_t), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow_2", "uint32_t", "", offset, sizeof(uint32_t), "EMAHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);


	incParam = 0;
	incSection++;
	
	

	size_t startOffset_animAdress = hdr->header_size;

	uint32_t* anim_offsets = (uint32_t *)GetOffsetPtr(buf, val16(startOffset_animAdress), true);
	animations.resize(val16(hdr->anim_count));

	for (size_t i = 0; i < animations.size(); i++)
	{
		incParam = 0;
		incSection = 2;
		binCt.write_Coloration_Tag("offset_animation", "uint32_t", "", startOffset_animAdress + i * sizeof(uint32_t), sizeof(uint32_t), "Animation", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i);

		size_t startAnimOffset = anim_offsets[i];
		offset = startAnimOffset;

		EmaAnimation &animation = animations[i];
		EMAAnimationHeader *ahdr = (EMAAnimationHeader*)GetOffsetPtr(buf, offset);

		incParam = 0;
		incSection++;
		binCt.write_Coloration_Tag("duration", "uint16_t", "", offset, sizeof(uint16_t), "EMAAnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("cmd_count", "uint16_t", "", offset, sizeof(uint16_t), "EMAAnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("value_count", "uint32_t", "", offset, sizeof(uint32_t), "EMAAnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("type", "uint8_t", "", offset, sizeof(uint8_t), "EMAAnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint8_t);
		binCt.write_Coloration_Tag("light_unknow", "uint8_t", "", offset, sizeof(uint8_t), "EMAAnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint8_t);
		binCt.write_Coloration_Tag("frame_float_size", "uint16_t", "", offset, sizeof(uint16_t), "EMAAnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("name_offset", "uint32_t", "", offset, sizeof(uint32_t), "EMAAnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("values_offset", "uint32_t", "", offset, sizeof(uint32_t), "EMAAnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);

		incParam = 0;
		incSection++;
		/*
		for (size_t j = 0; j < ahdr->cmd_count; j++)						//pb tinyXml ram memory
		{
			binCt.write_Coloration_Tag("animNode_offsets_"+ std::to_string(i), "uint32_t", "", offset, sizeof(uint32_t), "EMAAnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
		}
		*/
		binCt.write_Coloration_Tag("animNode_offsets_" + std::to_string(i), ToString(ahdr->cmd_count) + " * uint32_t", "", offset, ahdr->cmd_count * sizeof(uint32_t), "EMAAnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += ahdr->cmd_count * sizeof(uint32_t);


		animation.duration = val16(ahdr->duration);
		animation.type = ahdr->type;
		animation.light_unknow = ahdr->light_unknow;
		animation.frame_float_size = val16(ahdr->frame_float_size);
		animation.nodes.resize(val16(ahdr->cmd_count));


		offset = startAnimOffset + ahdr->name_offset;
		EMAAnimationName* animName = (EMAAnimationName*)GetOffsetPtr(buf, offset);
		
		binCt.write_Coloration_Tag("unknow_0", "uint32_t", "", offset, sizeof(uint32_t), "EMAAnimationName", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("unknow_1", "uint32_t", "", offset, sizeof(uint32_t), "EMAAnimationName", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("unknow_2", "uint16_t", "", offset, sizeof(uint16_t), "EMAAnimationName", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("nbChar", "uint8_t", "", offset, sizeof(uint8_t), "EMAAnimationName", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);

		animation.name = (char *)GetOffsetPtr(buf, offset);
		binCt.write_Coloration_Tag("name", "string", "", offset, animation.name.length() + 1, "EMAAnimationName", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += animation.name.length() + 1;
		if (i + 1 != animations.size())
		{
			size_padding = (size_t)(ceil((double)offset / 4.0) * 4.0) - offset;
			if (size_padding)
				binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMAAnimationName", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
			offset += size_padding;
		}


		for (size_t j = 0; j < animation.nodes.size(); j++)
		{
			size_t startAnimNodeOffset = startAnimOffset + ahdr->animNode_offsets[j];
			offset = startAnimNodeOffset;

			EMAAnimationNode &node = animation.nodes[j];
			EMAAnimationAnimNodeHeader *chdr = (EMAAnimationAnimNodeHeader *)GetOffsetPtr(buf, startAnimNodeOffset);

			incParam = 0;
			incSection = 5;
			binCt.write_Coloration_Tag("bone_idx", "uint16_t", "", offset, sizeof(uint16_t), "EMAAnimationAnimNodeHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("transform", "uint8_t", "", offset, sizeof(uint8_t), "EMAAnimationAnimNodeHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint8_t);
			binCt.write_Coloration_Tag("transformComponent", "uint8_t", "", offset, sizeof(uint8_t), "EMAAnimationAnimNodeHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint8_t);
			binCt.write_Coloration_Tag("keyframe_count", "uint16_t", "", offset, sizeof(uint16_t), "EMAAnimationAnimNodeHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("indices_offset", "uint16_t", "", offset, sizeof(uint16_t), "EMAAnimationAnimNodeHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);



			if (HasSkeleton())
			{
				if (val16(chdr->bone_idx) >= GetNumBones())
				{
					LOG_DEBUG("Bone idx 0x%x out of bounds, in animation \"%s\", in AnimNode 0x%x\n", chdr->bone_idx, animation.name.c_str(), j);
					//return;
					node.bone = nullptr;
				}else {
					node.bone = &bones[val16(chdr->bone_idx)];
				}
			}else{
				node.bone = nullptr;
			}

			node.transformComponent = (chdr->transformComponent & 0xF);

			node.transform = chdr->transform;
			node.keyframes.resize(val16(chdr->keyframe_count));


			size_t startOffsetKeyframeTimeIndexes = startAnimNodeOffset + sizeof(EMAAnimationAnimNodeHeader);
			offset = startOffsetKeyframeTimeIndexes;

			incParam = 0;
			incSection++;

			uint8_t* timing_u8 = (uint8_t*)GetOffsetPtr(buf, startOffsetKeyframeTimeIndexes);
			uint16_t* timing_16 = (uint16_t*)GetOffsetPtr(buf, startOffsetKeyframeTimeIndexes);

			/*
			for (size_t k = 0; k < node.keyframes.size(); k++)						//pb tinyXml ram memory
			{
				EMAKeyframe &keyframe = node.keyframes[k];

				if (!(chdr->transformComponent & 0x20))
				{
					keyframe.time = timing_u8[k];
					binCt.write_Coloration_Tag("indexForTiming", "uint8_t", "", offset, sizeof(uint8_t), "Keyframe", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint8_t);
				}else {
					keyframe.time = val16(timing_16[k]);
					binCt.write_Coloration_Tag("indexForTiming", "uint16_t", "", offset, sizeof(uint16_t), "Keyframe", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint16_t);
				}
			}
			*/
			if (!(chdr->transformComponent & 0x20))
			{
				binCt.write_Coloration_Tag("indexForTiming (8bits)", ToString(node.keyframes.size()) + " * uint8_t", "", offset, node.keyframes.size() * sizeof(uint8_t), "Keyframe", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += node.keyframes.size() * sizeof(uint8_t);
			}else{
				binCt.write_Coloration_Tag("indexForTiming (16bits)", ToString(node.keyframes.size()) + " * uint16_t", "", offset, node.keyframes.size() * sizeof(uint16_t), "Keyframe", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += node.keyframes.size() * sizeof(uint16_t);
			}

			size_padding = (size_t)(ceil((double)offset / 4.0) * 4.0) - offset;
			if (size_padding)
				binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "Keyframe indexForTiming", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
			offset += size_padding;



			size_t startOffsetKeyframeIndexes = startAnimNodeOffset + val16(chdr->indices_offset);
			offset = startOffsetKeyframeIndexes;

			incParam = 0;
			incSection++;

			uint16_t* indices_u16 = (uint16_t*)GetOffsetPtr(buf, startOffsetKeyframeIndexes);
			uint32_t* indices_u32 = (uint32_t*)GetOffsetPtr(buf, startOffsetKeyframeIndexes);

			/*
			for (size_t k = 0; k < node.keyframes.size(); k++)						//pb tinyXml ram memory
			{
				EMAKeyframe &keyframe = node.keyframes[k];

				if (!(chdr->transformComponent & 0x40))
				{
					keyframe.index = val16(indices_u16[k]);
					binCt.write_Coloration_Tag("indexForValue", "uint16_t", "", offset, sizeof(uint16_t), "Keyframe", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint16_t);
				}else {
					keyframe.index = val32(indices_u32[k]);
					binCt.write_Coloration_Tag("indexForValue", "uint32_t", "", offset, sizeof(uint32_t), "Keyframe", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint32_t);
				}
			}
			*/
			if (!(chdr->transformComponent & 0x40))
			{
				binCt.write_Coloration_Tag("indexForValue (16bits)", ToString(node.keyframes.size()) + " * uint16_t", "", offset, node.keyframes.size() * sizeof(uint16_t), "Keyframe", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += node.keyframes.size() * sizeof(uint16_t);
			}else{
				binCt.write_Coloration_Tag("indexForValue (32bits)", ToString(node.keyframes.size()) + " * uint32_t", "", offset, node.keyframes.size() * sizeof(uint32_t), "Keyframe", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += node.keyframes.size() * sizeof(uint32_t);
			}


			size_padding = (size_t)(ceil((double)offset / 4.0) * 4.0) - offset;
			if (size_padding)
				binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "Keyframe indexForValue", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
			offset += size_padding;
		}




		animation.values.resize(val32(ahdr->value_count));

		size_t startoffsetFloatValues = startAnimOffset + ahdr->values_offset;
		offset = startoffsetFloatValues;

		float* values_32 = (float *)GetOffsetPtr(buf, startoffsetFloatValues);
		uint16_t* values_u16 = (uint16_t*)values_32;

		incParam = 0;
		incSection++;
		/*
		for (size_t j = 0; j < animation.values.size(); j++)						//pb tinyXml ram memory
		{
			if (animation.frame_float_size == 0)
			{
				animation.values[j] = val_float(values_32[j]);
				binCt.write_Coloration_Tag("Float", "uint32_t", "", offset, sizeof(uint32_t), "ListFloatValues", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
			}else {
				animation.values[j] = float16ToFloat(values_u16[j]);
				binCt.write_Coloration_Tag("Float", "uint16_t", "", offset, sizeof(uint16_t), "ListFloatValues", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
			}
		}*/
		if (animation.frame_float_size == 0)
		{
			binCt.write_Coloration_Tag("Float (32bits)", ToString(animation.values.size()) + " * float", "", offset, animation.values.size() * sizeof(uint32_t), "ListFloatValues", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += animation.values.size() * sizeof(uint32_t);
		}else {
			binCt.write_Coloration_Tag("Float (16bits)", ToString(animation.values.size()) + " * float16", "", offset, animation.values.size() * sizeof(uint16_t), "ListFloatValues", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += animation.values.size() * sizeof(uint16_t);
		}


		size_padding = (size_t)(ceil((double)offset / 4.0) * 4.0) - offset;
		if (size_padding)
			binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "ListFloatValues", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
		offset += size_padding;

	}


	size_padding = (size_t)(ceil((double)offset / 64.0) * 64.0) - offset;
	if ((size_padding) && (hdr->type != EMA_TYPE_ANIM_Light))
		binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "before Skeleton or animationsNames", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
	offset += size_padding;


	
	incParam = 0;
	incSection++;
	if (hdr->skeleton_offset)
	{
		EMO_Skeleton emo_Ske;
		emo_Ske.write_Coloration_Skeleton(binCt, parent, GetOffsetPtr(buf, hdr->skeleton_offset), size - val32(hdr->skeleton_offset), hdr->skeleton_offset, true, listBytesAllreadyTagged);
	}




	binCt.write_Coloration_Gaps(0x0, size, sizeof(uint8_t), idTag, "Empty", parent, listBytesAllreadyTagged);		//test Empty/gaps/holes.
}




}