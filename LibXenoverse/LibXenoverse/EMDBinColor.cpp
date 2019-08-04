
#include "EMD.h"

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
void EMD::save_Coloration(string filename, bool show_error)
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
void EMD::write_Coloration(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	EMD_Header* hdr = (EMD_Header*)buf;
	if (size < sizeof(EMD_Header) || memcmp(hdr->signature, LIBXENOVERSE_EMD_SIGNATURE, 4) != 0)
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

	binCt.write_Coloration_Tag("signature", "string", "", offset, 4 * sizeof(char), "EMD_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(char);
	binCt.write_Coloration_Tag("endian", "uint32_t", "", offset, sizeof(uint32_t), "EMD_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("version", "uint32_t", "", offset, sizeof(uint32_t), "EMD_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknown_0", "uint32_t", "", offset, sizeof(uint32_t), "EMD_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

	
	size_t offset_EMD_Section = offset;
	EMD_Section* emd_Section = (EMD_Section*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_EMD_Section));
	incSection++;
	incParam = 0;

	binCt.write_Coloration_Tag("unknown_0", "uint16_t", "", offset, sizeof(uint16_t), "EMD_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("number_models", "uint16_t", "", offset, sizeof(uint16_t), "EMD_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("offset_models", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + emd_Section->offset_models, true) + " Ref to list adress to EMDModel_Section", offset, sizeof(uint32_t), "EMD_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("offset_models_name", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startAdress + emd_Section->offset_models_name, true) + " Ref to string", offset, sizeof(uint32_t), "EMD_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);


	incSection = 2;
	incParam = 0;
	offset = startAdress + emd_Section->offset_models + emd_Section->number_models * sizeof(uint32_t);
	size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
	if (size_padding)
		binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMDModel_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);

	isBigFile = (emd_Section->number_models >= 100);

	uint32_t* offset_models = (uint32_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, startAdress + emd_Section->offset_models));
	for (size_t i = 0; i < emd_Section->number_models; i++)
	{
		incSection = 2;
		size_t offset_EMDModel_Section = startAdress + offset_models[i];
		offset = offset_EMDModel_Section;

		binCt.write_Coloration_Tag("offset_model", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMDModel_Section, true) + " Ref to list adress to EMDModel_Section", startAdress + emd_Section->offset_models + i * sizeof(uint32_t), sizeof(uint32_t), "EMD_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);


		EMDModel_Section* emdModel_Section = (EMDModel_Section*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
		incParam = 0;

		binCt.write_Coloration_Tag("unknown_0", "uint16_t", "", offset, sizeof(uint16_t), "EMDModel_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("number_meshs", "uint16_t", "", offset, sizeof(uint16_t), "EMDModel_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("offset_meshs", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMD_Section + emdModel_Section->offset_meshs, true) + " Ref to list adress to EMDMesh_Section", offset, sizeof(uint32_t), "EMDModel_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		incSection = 3;
		incParam = 0;
		offset = offset_EMDModel_Section + emdModel_Section->offset_meshs + emdModel_Section->number_meshs * sizeof(uint32_t);
		size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
		if (size_padding)
			binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMDModel_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);


		isBigFile = isBigFile || (emdModel_Section->number_meshs >= 100);

		uint32_t* offset_meshs = (uint32_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_EMDModel_Section + emdModel_Section->offset_meshs));
		for (size_t j = 0; j < emdModel_Section->number_meshs; j++)
		{
			size_t offset_EMDMesh_Section = offset_EMDModel_Section + offset_meshs[j];
			offset = offset_EMDMesh_Section;

			incSection = 3;
			incParam = 0;
			binCt.write_Coloration_Tag("offset_mesh", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMDMesh_Section, true) + " Ref to list adress to EMDMesh_Section", offset_EMDModel_Section + emdModel_Section->offset_meshs + j * sizeof(uint32_t), sizeof(uint32_t), "EMDModel_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);

			EMDMesh_Section* emdMesh_Section = (EMDMesh_Section*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
			incSection = 3;
			incParam = 0;

			binCt.write_Coloration_Tag("aabb_center_x", "float", "", offset, sizeof(float), "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			binCt.write_Coloration_Tag("aabb_center_y", "float", "", offset, sizeof(float), "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			binCt.write_Coloration_Tag("aabb_center_z", "float", "", offset, sizeof(float), "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			binCt.write_Coloration_Tag("aabb_center_w", "float", "", offset, sizeof(float), "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			binCt.write_Coloration_Tag("aabb_min_x", "float", "", offset, sizeof(float), "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			binCt.write_Coloration_Tag("aabb_min_y", "float", "", offset, sizeof(float), "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			binCt.write_Coloration_Tag("aabb_min_z", "float", "", offset, sizeof(float), "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			binCt.write_Coloration_Tag("aabb_min_w", "float", "", offset, sizeof(float), "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			binCt.write_Coloration_Tag("aabb_max_x", "float", "", offset, sizeof(float), "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			binCt.write_Coloration_Tag("aabb_max_y", "float", "", offset, sizeof(float), "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			binCt.write_Coloration_Tag("aabb_max_z", "float", "", offset, sizeof(float), "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			binCt.write_Coloration_Tag("aabb_max_w", "float", "", offset, sizeof(float), "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			binCt.write_Coloration_Tag("offset_mesh_name", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMDMesh_Section + emdMesh_Section->offset_mesh_name, true) + " Ref to a name", offset, sizeof(uint32_t), "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			binCt.write_Coloration_Tag("unknown_0", "uint16_t", "", offset, sizeof(uint16_t), "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("number_submeshs", "uint16_t", "", offset, sizeof(uint16_t), "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("offset_submeshs", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMDMesh_Section + emdMesh_Section->offset_submeshs, true) + " Ref to list adress to EMDSubmesh_Section", offset, sizeof(uint32_t), "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

			offset = offset_EMDMesh_Section + emdMesh_Section->offset_mesh_name;
			name_tmp = string((char*)EMO_BaseFile::GetOffsetPtr_native(buf, offset));
			binCt.write_Coloration_Tag("mesh_name", "string", "", offset, (name_tmp.size() + 1) * sizeof(char), "EMDMesh_Section", parent, idTag++, incSection + 1, i, listBytesAllreadyTagged, (size_t)-1, false); offset += (name_tmp.size() + 1) * sizeof(char);

			size_padding = (size_t)(ceil((double)offset / 4.0) * 4.0) - offset;
			if (size_padding)
				binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);


			incSection = 4;
			incParam = 0;
			offset = offset_EMDMesh_Section + emdMesh_Section->offset_submeshs + emdMesh_Section->number_submeshs * sizeof(uint32_t);
			size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
			if(size_padding)
				binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);

			isBigFile = isBigFile || (emdMesh_Section->number_submeshs >= 100);

			uint32_t* offset_submeshs = (uint32_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_EMDMesh_Section + emdMesh_Section->offset_submeshs));
			for (size_t k = 0; k < emdMesh_Section->number_submeshs; k++)
			{
				size_t offset_EMDSubmesh_Section = offset_EMDMesh_Section + offset_submeshs[k];
				offset = offset_EMDSubmesh_Section;
				
				incSection = 4;
				incParam = 0;
				binCt.write_Coloration_Tag("offset_submesh", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMDSubmesh_Section, true) + " Ref to list adress to EMDSubmesh_Section", offset_EMDMesh_Section + emdMesh_Section->offset_submeshs + k * sizeof(uint32_t), sizeof(uint32_t), "EMDMesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);

				EMDSubmesh_Section* emdSubmesh_Section = (EMDSubmesh_Section*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
				incSection = 4;
				incParam = 0;

				binCt.write_Coloration_Tag("aabb_center_x", "float", "", offset, sizeof(float), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
				binCt.write_Coloration_Tag("aabb_center_y", "float", "", offset, sizeof(float), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
				binCt.write_Coloration_Tag("aabb_center_z", "float", "", offset, sizeof(float), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
				binCt.write_Coloration_Tag("aabb_center_w", "float", "", offset, sizeof(float), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
				binCt.write_Coloration_Tag("aabb_min_x", "float", "", offset, sizeof(float), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
				binCt.write_Coloration_Tag("aabb_min_y", "float", "", offset, sizeof(float), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
				binCt.write_Coloration_Tag("aabb_min_z", "float", "", offset, sizeof(float), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
				binCt.write_Coloration_Tag("aabb_min_w", "float", "", offset, sizeof(float), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
				binCt.write_Coloration_Tag("aabb_max_x", "float", "", offset, sizeof(float), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
				binCt.write_Coloration_Tag("aabb_max_y", "float", "", offset, sizeof(float), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
				binCt.write_Coloration_Tag("aabb_max_z", "float", "", offset, sizeof(float), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
				binCt.write_Coloration_Tag("aabb_max_w", "float", "", offset, sizeof(float), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);				
				binCt.write_Coloration_Tag("vertex_type_flag", "uint32_t", "", offset, sizeof(uint32_t), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
				binCt.write_Coloration_Tag("vertex_size", "uint32_t", "", offset, sizeof(uint32_t), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
				binCt.write_Coloration_Tag("number_vertex", "uint32_t", "", offset, sizeof(uint32_t), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
				binCt.write_Coloration_Tag("offset_vertex", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMDSubmesh_Section + emdSubmesh_Section->offset_vertex, true) + " Ref to list adress to vertex structures (depend of flag) ", offset, sizeof(uint32_t), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
				binCt.write_Coloration_Tag("offset_submesh_name", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMDSubmesh_Section + emdSubmesh_Section->offset_submesh_name, true) + " Ref to a name", offset, sizeof(uint32_t), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
				binCt.write_Coloration_Tag("unknown_0", "uint8_t", "", offset, sizeof(uint8_t), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
				binCt.write_Coloration_Tag("number_textureDef", "uint8_t", "", offset, sizeof(uint8_t), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
				binCt.write_Coloration_Tag("number_triangles", "uint16_t", "", offset, sizeof(uint16_t), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
				binCt.write_Coloration_Tag("offset_textureDef", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMDSubmesh_Section + emdSubmesh_Section->offset_textureDef, true) + " Ref to list adress to EMDTextureDef_Section", offset, sizeof(uint32_t), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
				binCt.write_Coloration_Tag("offset_triangles", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMDSubmesh_Section + emdSubmesh_Section->offset_triangles, true) + " Ref to list adress to EMDTriangles_Section", offset, sizeof(uint32_t), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

				offset = offset_EMDSubmesh_Section + emdSubmesh_Section->offset_submesh_name;
				name_tmp = string((char*)EMO_BaseFile::GetOffsetPtr_native(buf, offset));
				binCt.write_Coloration_Tag("submesh_name", "string", "", offset, (name_tmp.size() + 1) * sizeof(char), "EMDSubmesh_Section", parent, idTag++, incSection + 1, i, listBytesAllreadyTagged, (size_t)-1, false); offset += (name_tmp.size() + 1) * sizeof(char);

				size_padding = (size_t)(ceil((double)offset / 4.0) * 4.0) - offset;
				if (size_padding)
					binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
				offset += size_padding;

				


				for (size_t m = 0; m < emdSubmesh_Section->number_textureDef; m++)
				{
					size_t offset_EMDTextureDef_Section = offset_EMDSubmesh_Section + emdSubmesh_Section->offset_textureDef + m * sizeof(EMDTextureDef_Section);
					offset = offset_EMDTextureDef_Section;
					EMDTextureDef_Section* emdTextureDef_Section = (EMDTextureDef_Section*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
					incSection = 6;
					incParam = 0;

					binCt.write_Coloration_Tag("unknown_0", "uint8_t", "", offset, sizeof(uint8_t), "EMDTextureDef_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
					binCt.write_Coloration_Tag("textureIndex", "uint8_t", "", offset, sizeof(uint8_t), "EMDTextureDef_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
					binCt.write_Coloration_Tag("unknown_1", "uint8_t", "", offset, sizeof(uint8_t), "EMDTextureDef_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
					binCt.write_Coloration_Tag("unknown_2", "uint8_t", "", offset, sizeof(uint8_t), "EMDTextureDef_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
					binCt.write_Coloration_Tag("texture_scale_u", "float", "", offset, sizeof(float), "EMDTextureDef_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
					binCt.write_Coloration_Tag("texture_scale_v", "float", "", offset, sizeof(float), "EMDTextureDef_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
				}


				isBigFile = isBigFile || (emdSubmesh_Section->number_triangles >= 100);

				uint32_t* offset_triangles = (uint32_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_EMDSubmesh_Section + emdSubmesh_Section->offset_triangles));
				for (size_t m = 0; m < emdSubmesh_Section->number_triangles; m++)
				{
					size_t offset_EMDTriangles_Section = offset_EMDSubmesh_Section + offset_triangles[m];
					offset = offset_EMDTriangles_Section;

					binCt.write_Coloration_Tag("offset_triangle", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMDTriangles_Section, true) + " Ref to list adress to EMDTriangles_Section", offset_EMDSubmesh_Section + emdSubmesh_Section->offset_triangles + m * sizeof(uint32_t), sizeof(uint32_t), "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);

					EMDTriangles_Section* emdTriangles_Section = (EMDTriangles_Section*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
					incSection = 7;
					incParam = 0;

					binCt.write_Coloration_Tag("number_faces", "uint32_t", "", offset, sizeof(uint32_t), "EMDTriangles_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
					binCt.write_Coloration_Tag("number_bones", "uint32_t", "", offset, sizeof(uint32_t), "EMDTriangles_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
					binCt.write_Coloration_Tag("offset_faces", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMDTriangles_Section + emdTriangles_Section->offset_faces, true) + " Ref to list faces", offset, sizeof(uint32_t), "EMDTriangles_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
					binCt.write_Coloration_Tag("offset_bone_names", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(offset_EMDTriangles_Section + emdTriangles_Section->offset_bone_names, true) + " Ref to list adress to bone names", offset, sizeof(uint32_t), "EMDTriangles_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);


					incSection = 8;
					incParam = 0;
					offset = offset_EMDTriangles_Section + emdTriangles_Section->offset_faces;
					
					uint16_t* faceValues = (uint16_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
					bool is32bits = ((emdTriangles_Section->number_faces>5)&&(faceValues[1]==0)&&(faceValues[3]==0) && (faceValues[5] == 0));			//find how to choose 16 or 32 bits. => number of vertex don't work (check on files), and tehre no other value hide in padding or else.

					isBigFile = isBigFile || (emdTriangles_Section->number_faces >= 3000);

					if (!isBigFile)
					{
						for (size_t n = 0; n < emdTriangles_Section->number_faces; n++)
						{
							if (!is32bits)
							{
								binCt.write_Coloration_Tag("face(" + ToString(n) + ")", "uint16_t", "", offset, sizeof(uint16_t), "EMDTriangles_Section_Faces", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
							}else{
								binCt.write_Coloration_Tag("face(" + ToString(n) + ")", "uint32_t", "", offset, sizeof(uint32_t), "EMDTriangles_Section_Faces", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
							}
						}

					}else {												//version to reduce nbTag, and avoid too much memory for big file.

						if (!is32bits)
						{
							binCt.write_Coloration_Tag("faces x " + ToString(emdTriangles_Section->number_faces), "uint16_t", "", offset, emdTriangles_Section->number_faces * sizeof(uint16_t), "EMDTriangles_Section_Faces", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += emdTriangles_Section->number_faces * sizeof(uint16_t);
						}else{
							binCt.write_Coloration_Tag("faces x " + ToString(emdTriangles_Section->number_faces), "uint32_t", "", offset, emdTriangles_Section->number_faces * sizeof(uint32_t), "EMDTriangles_Section_Faces", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += emdTriangles_Section->number_faces * sizeof(uint32_t);
						}

					}
					if ((!is32bits) && ((size_t)(ceil((double)offset / 4.0) * 4.0) - offset != 0))
					{
						size_padding = (size_t)(ceil((double)offset / 4.0) * 4.0) - offset;
						if (size_padding)
							binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMDTriangles_Section_Faces", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
						offset += size_padding;
					}



					incSection = 9;
					incParam = 0;
					//uint32_t* offset_bone_names = (uint32_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset_EMDTriangles_Section + emdTriangles_Section->offset_bone_names));

					offset = offset_EMDTriangles_Section + emdTriangles_Section->offset_bone_names;
					for (size_t n = 0; n < emdTriangles_Section->number_bones; n++)
					{
						name_offset = (uint32_t*)EMO_BaseFile::GetOffsetPtr_native(buf, offset);

						binCt.write_Coloration_Tag("offset_bone_name", "uint32_t", "", offset, sizeof(uint32_t), "EMDTriangles_Section_Bones", parent, idTag++, incSection, i, listBytesAllreadyTagged); offset += sizeof(uint32_t);

						if (name_offset)
						{
							name_tmp = string((char*)EMO_BaseFile::GetOffsetPtr_native(buf, offset_EMDTriangles_Section + (*name_offset) ));
							binCt.write_Coloration_Tag("bone_name", "string", "", offset_EMDTriangles_Section + (*name_offset), (name_tmp.size() + 1) * sizeof(char), "EMDTriangles_Section_Bones", parent, idTag++, incSection + 1, i, listBytesAllreadyTagged, (size_t)-1, false);
						}
					}

					if (emdTriangles_Section->number_bones)
					{
						name_tmp = string((char*)EMO_BaseFile::GetOffsetPtr_native(buf, offset_EMDTriangles_Section + (*name_offset)));
						offset = offset_EMDTriangles_Section + (*name_offset) + (name_tmp.size() + 1) * sizeof(char);
					}


					if (m + 1 != emdSubmesh_Section->number_triangles)
					{
						size_padding = (size_t)(ceil((double)offset / 4.0) * 4.0) - offset;
						if (size_padding)
							binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMDTriangles_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);

						offset += size_padding;
					}

				}




				size_padding = (size_t)(ceil((double)offset / 4.0) * 4.0) - offset;
				if (size_padding)
					binCt.write_Coloration_Tag("padding 2 ", "padding", "", offset, size_padding, "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
				offset += size_padding;



				incSection = 5;
				incParam = 0;

				isBigFile = isBigFile || (emdSubmesh_Section->number_vertex >= 3000);

				if(!isBigFile)
				{
					for (size_t m = 0; m < emdSubmesh_Section->number_vertex; m++)
					{
						offset = offset_EMDSubmesh_Section + emdSubmesh_Section->offset_vertex + m * emdSubmesh_Section->vertex_size;

						binCt.write_Coloration_Tag("Vertex(" + ToString(m) + ")", "custom flag", "", offset, emdSubmesh_Section->vertex_size, "EMDSubmesh_Section_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
					}

				}else{												//version to reduce nbTag, and avoid too much memory for big file.
					
					binCt.write_Coloration_Tag("Vertex", "custom flag: "+ EMO_BaseFile::UnsignedToString(emdSubmesh_Section->vertex_size, true), "", offset_EMDSubmesh_Section + emdSubmesh_Section->offset_vertex, emdSubmesh_Section->number_vertex * emdSubmesh_Section->vertex_size, "EMDSubmesh_Section_Vertex", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
				}
				if (emdSubmesh_Section->number_vertex)
					offset = offset_EMDSubmesh_Section + emdSubmesh_Section->offset_vertex + emdSubmesh_Section->number_vertex * emdSubmesh_Section->vertex_size;



				if (k +1 != emdMesh_Section->number_submeshs)
				{
					size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
					if (size_padding)
						binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);

					offset += size_padding;
				}
			}


			if (j + 1 != emdModel_Section->number_meshs)
			{
				size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
				if (size_padding)
					binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
				offset += size_padding;
			}
		}

		if (i + 1 != emd_Section->number_models)
		{
			size_padding = (size_t)(ceil((double)offset / 16.0) * 16.0) - offset;
			if (size_padding)
				binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "EMDSubmesh_Section", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
			offset += size_padding;
		}
	}


	//(todo color better full name, not just 4 char)
	incSection = 2;
	incParam = 0;

	//uint32_t* offset_models_name = (uint32_t*)(EMO_BaseFile::GetOffsetPtr_native(buf, startAdress + emd_Section->offset_models_name));
	offset = startAdress + emd_Section->offset_models_name;
	for (size_t i = 0; i < emd_Section->number_models; i++)
	{
		name_offset = (uint32_t*)EMO_BaseFile::GetOffsetPtr_native(buf, offset);

		binCt.write_Coloration_Tag("offset_model_name", "uint32_t", "", offset, sizeof(uint32_t), "EMD_Section", parent, idTag++, incSection, i, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		if (name_offset)
		{
			name_tmp = string((char*)EMO_BaseFile::GetOffsetPtr_native(buf, startAdress + (*name_offset) ));
			binCt.write_Coloration_Tag("model_name", "string", "", startAdress + (*name_offset), (name_tmp.size() + 1) * sizeof(char), "EMD_Section", parent, idTag++, incSection + 1, i, listBytesAllreadyTagged, (size_t)-1, false);
		}
	}
	




	binCt.write_Coloration_Gaps(0x0, size, sizeof(uint8_t), idTag, "Empty", parent, listBytesAllreadyTagged);		//test Empty/gaps/holes.
}





}