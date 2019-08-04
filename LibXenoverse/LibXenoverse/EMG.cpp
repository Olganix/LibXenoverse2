#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "EMG.h"
//#include "debug.h"
#include "EMO_BaseFile.h"


namespace LibXenoverse
{


bool EMG_VertexData::operator==(const EMG_VertexData &rhs) const
{
	if (this->size != rhs.size)
		return false;
	
	return (memcmp(&this->VertexUnion, &rhs.VertexUnion, sizeof(this->VertexUnion)) == 0);
}







/*-------------------------------------------------------------------------------\
|                             readEmdVertex			                             |
\-------------------------------------------------------------------------------*/
void EMG_VertexData::readEmdVertex(EMDVertex* emd)
{
	VertexUnion.flags = emd->flags;
	
	VertexUnion.pos_x = emd->pos_x;
	VertexUnion.pos_y = emd->pos_y;
	VertexUnion.pos_z = emd->pos_z;
	VertexUnion.norm_x = emd->norm_x;
	VertexUnion.norm_y = emd->norm_y;
	VertexUnion.norm_z = emd->norm_z;
	VertexUnion.tang_x = emd->tang_x;
	VertexUnion.tang_y = emd->tang_y;
	VertexUnion.tang_z = emd->tang_z;
	VertexUnion.text_u = emd->text_u;
	VertexUnion.text_v = emd->text_v;
	VertexUnion.text2_u = emd->text2_u;
	VertexUnion.text2_v = emd->text2_v;
	VertexUnion.color = emd->color;

	for (size_t i = 0; i < 4; i++)
	{
		VertexUnion.blend[i] = emd->blend[i];
		VertexUnion.blend_weight[i] = emd->blend_weight[i];
	}

	size = VertexUnion.getSizeFromFlags(true);
}
/*-------------------------------------------------------------------------------\
|                             writeEmdVertex		                             |
\-------------------------------------------------------------------------------*/
void EMG_VertexData::writeEmdVertex(EMDVertex* emd)
{
	emd->flags = VertexUnion.flags;

	emd->pos_x = VertexUnion.pos_x;
	emd->pos_y = VertexUnion.pos_y;
	emd->pos_z = VertexUnion.pos_z;
	emd->norm_x = VertexUnion.norm_x;
	emd->norm_y = VertexUnion.norm_y;
	emd->norm_z = VertexUnion.norm_z;
	emd->tang_x = VertexUnion.tang_x;
	emd->tang_y = VertexUnion.tang_y;
	emd->tang_z = VertexUnion.tang_z;
	emd->text_u = VertexUnion.text_u;
	emd->text_v = VertexUnion.text_v;
	emd->text2_u = VertexUnion.text2_u;
	emd->text2_v = VertexUnion.text2_v;
	emd->color = VertexUnion.color;

	for (size_t i = 0; i < 4; i++)
	{
		emd->blend[i] = VertexUnion.blend[i];
		emd->blend_weight[i] = VertexUnion.blend_weight[i];
	}
}






//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////



bool EMG_SubPart::InjectVertex(const std::vector<EMG_VertexCommon> &new_vertex, bool do_pos, bool do_normal, bool do_tang, bool do_uvmap, bool do_uvmap2, bool do_color, bool do_blend)
{
	if (vertex.size() != new_vertex.size())
	{
		LOG_DEBUG("ERROR: When injecting, number of vertex must be the same than existing ones! (tried: %Iu, existing: %u), on subpart \"%s\"\n", new_vertex.size(), vertex.size(), meta_name.c_str());
		LibXenoverse::notifyError();
		return false;
	}

	assert(vertex.size() > 0);
	assert(vertex[0].size == vertex[0].VertexUnion.getSizeFromFlags());

	for (size_t i = 0; i < vertex.size(); i++)
	{
		EMG_VertexCommon *vtx = &vertex[i].VertexUnion;
		const EMG_VertexCommon &vtx_inj = new_vertex[i];

		if (do_pos)
		{
			vtx->pos_x = vtx_inj.pos_x;
			vtx->pos_y = vtx_inj.pos_y;
			vtx->pos_z = vtx_inj.pos_z;
		}



		if (do_normal)
		{
			vtx->norm_x = vtx_inj.norm_x;
			vtx->norm_y = vtx_inj.norm_y;
			vtx->norm_z = vtx_inj.norm_z;
		}
		if (do_uvmap)
		{
			vtx->text_u = vtx_inj.text_u;
			vtx->text_v = vtx_inj.text_v;
		}
		if (do_uvmap2)
		{
			vtx->text2_u = vtx_inj.text2_u;
			vtx->text2_v = vtx_inj.text2_v;
		}
		if (do_tang)
		{
			vtx->tang_x = vtx_inj.tang_x;
			vtx->tang_y = vtx_inj.tang_y;
			vtx->tang_z = vtx_inj.tang_z;
		}

		
		if (do_color)
			vtx->color = vtx_inj.color;

		if (do_blend)
		{
			for (int i = 0; i < 4; i++)
			{
				vtx->blend[i] = vtx_inj.blend[i];
				vtx->blend_weight[i] = vtx_inj.blend_weight[i];
			}
		}
	}

	return true;
}

void EMG_SubPart::GetSubMeshVertexAndTriangles(uint16_t submesh_idx, std::vector<EMG_VertexData> &sm_vertex, std::vector<uint16_t> &sm_triangles) const
{
	std::vector<uint16_t> map;

	sm_triangles = GetTriangles(submesh_idx);
	sm_vertex.clear();

	for (uint16_t &id : sm_triangles)
	{
		std::vector<uint16_t>::iterator it = std::find(map.begin(), map.end(), id);
		if (it == map.end())
		{
			sm_vertex.push_back(vertex[id]);
			map.push_back(id);
			id = sm_vertex.size() - 1;
		}
		else
		{
			id = it - map.begin();
		}
	}
}

size_t EMG_SubPart::GetTrianglesInternal(size_t submesh_idx, std::vector<uint16_t> *tri_faces) const
{
	if (submesh_idx >= submeshes.size())
	{
		throw std::out_of_range("submesh_idx out of range.");
	}

	const EMG_SubMesh &submesh = submeshes[submesh_idx];

	if (!strips)
	{
		assert((submesh.faces.size() % 3) == 0);

		if (tri_faces)
			*tri_faces = submesh.faces;

		return submesh.faces.size();
	}

	size_t count = 0;

	if (tri_faces)
		tri_faces->clear();

	bool backface = false;
	int strip_size = 1;
	uint16_t idx1 = 0xFFFF, idx2 = 0xFFFF;
	size_t f_index = 0;

	for (size_t i = 0; i < submesh.faces.size(); i++, strip_size++)
	{
		assert(f_index < submesh.faces.size());

		uint16_t idx = submesh.faces[f_index];
		f_index++;

		if (idx == idx1)
		{
			backface = !backface;
			idx1 = idx2;
			idx2 = idx;
			strip_size = 2;
			continue;
		}

		if (idx == idx2)
		{
			backface = !backface;
			strip_size = 1;
			continue;
		}

		if (strip_size >= 3)
		{
			if (tri_faces)
			{
				if (backface)
				{
					tri_faces->push_back(idx);
					tri_faces->push_back(idx2);
					tri_faces->push_back(idx1);
				}
				else
				{
					tri_faces->push_back(idx1);
					tri_faces->push_back(idx2);
					tri_faces->push_back(idx);
				}
			}

			count += 3;
		}

		backface = !backface;
		idx1 = idx2;
		idx2 = idx;
	}

	return count;
}


void EMG_SubPart::SetVertex(const std::vector<EMG_VertexData> &vertex)
{
	this->vertex = vertex;
}

std::vector<uint16_t> EMG_SubPart::GetTriangles(size_t submesh_idx) const
{
	std::vector<uint16_t> tri_faces;

	GetTrianglesInternal(submesh_idx, &tri_faces);
	return tri_faces;
}

size_t EMG_SubPart::GetNumberOfPolygons(size_t submesh_idx) const
{
	size_t faces_num = GetTrianglesInternal(submesh_idx, nullptr);

	assert((faces_num % 3) == 0);
	return (faces_num / 3);
}

size_t EMG_SubPart::GetLinkedBones(std::vector<EMO_Bone *> &list, bool clear_vector, bool unique, const EMO_Skeleton *sorter) const
{
	size_t count = 0;

	if (clear_vector)
		list.clear();

	for (const EMG_SubMesh &sm : submeshes)
	{
		count += sm.GetLinkedBones(list, false, unique);
	}

	if (sorter)
	{
		std::sort(list.begin(), list.end(), BoneSorter(sorter));
	}

	return count;
}

size_t EMG_SubPart::GetEmmMaterials(std::vector<std::string> &list, bool clear_vector, bool unique) const
{
	size_t count = 0;

	if (clear_vector)
	{
		list.clear();
	}

	for (const EMG_SubMesh &sm : submeshes)
	{
		if (unique)
		{
			if (std::find_if(list.begin(), list.end(), EMO_BaseFile::StringFinder(sm.emm_material)) != list.end())
				continue;
		}

		list.push_back(sm.emm_material);
		count++;
	}

	return count;
}

size_t EMG_SubPart::GetEmbIndexes(std::vector<uint8_t> &list, bool clear_vector, bool unique, bool sort) const
{
	size_t count = 0;

	if (clear_vector)
		list.clear();

	for (const EMG_TexturesList &tl : textures_lists)
	{
		count += tl.GetEmbIndexes(list, false, unique);
	}

	if (sort)
	{
		std::sort(list.begin(), list.end());
	}

	return count;
}

size_t EMG_SubPart::ReplaceEmmMaterial(const std::string &old_mat, const std::string &new_mat)
{
	if (new_mat.length() > EMG_SubMesh::MAX_EMM_MAT_LENGTH)
		return (size_t)-1;

	size_t count = 0;

	for (EMG_SubMesh &sm : submeshes)
	{
		if (strcasecmp(sm.emm_material.c_str(), old_mat.c_str()) == 0)
		{
			sm.emm_material = new_mat;
			count++;
		}
	}

	return count;
}

size_t EMG_SubPart::ReplaceEmbIndex(uint8_t old_index, uint8_t new_index)
{
	size_t count = 0;

	for (EMG_TexturesList &tl : textures_lists)
	{
		count += tl.ReplaceEmbIndex(old_index, new_index);
	}

	return count;
}

bool EMG_SubPart::IsEdge() const
{
	size_t count = 0;

	for (const EMG_SubMesh &sm : submeshes)
	{
		if (sm.IsEdge())
			count++;
	}

	assert(count == 0 || count == submeshes.size());
	return (count != 0);
}


size_t EMG_SubPart::getSizeFromFlags(uint16_t flags_tmp, bool fromEmoFile)
{
	size_t nbOctets = 0;

	bool isCompressed = ((flags_tmp & EMG_VTX_FLAG_COMPRESSED_FORMAT) && (fromEmoFile));


	if (flags_tmp & EMG_VTX_FLAG_POS) 
		nbOctets += 3 * 4;

	if (flags_tmp & EMG_VTX_FLAG_NORM)
		nbOctets += ((isCompressed) ? (4 * 2) : (3 * 4));

	if (flags_tmp & EMG_VTX_FLAG_TANGENT)
		nbOctets += ((isCompressed) ? (4 * 2) : (3 * 4));

	if (flags_tmp & EMG_VTX_FLAG_TEX)
		nbOctets += 2 * ((isCompressed) ? 2 : 4);

	if (flags_tmp & EMG_VTX_FLAG_TEX2)
		nbOctets += 2 * ((isCompressed) ? 2 : 4);

	if (flags_tmp & EMG_VTX_FLAG_COLOR)
		nbOctets += 4;

	if (flags_tmp & EMG_VTX_FLAG_BLEND_WEIGHT)
		nbOctets += 4 + ((isCompressed) ? (4 * 2) : (3 * 4));

	return nbOctets;
}

bool EMG_SubPart::RemoveSubMesh(size_t submesh_idx)
{
	if (submesh_idx >= submeshes.size())
		return false;

	submeshes.erase(submeshes.begin() + submesh_idx);
	return true;
}

size_t EMG_SubPart::ExportObj(std::string *vertex_out, std::string *uvmap_out, std::string *normal_out, std::string *topology_out, size_t v_start_idx, bool write_group) const
{
	if (!vertex_out && !uvmap_out && !normal_out && !topology_out)
		return 0;

	std::ostringstream vss, uvss, nss, tss;

	vss.setf(std::ios::scientific, std::ios::floatfield);
	uvss.setf(std::ios::scientific, std::ios::floatfield);
	nss.setf(std::ios::scientific, std::ios::floatfield);

	if (vertex_out || uvmap_out || normal_out)
	{
		assert(vertex.size() != 0);
		assert(vertex[0].size == vertex[0].VertexUnion.getSizeFromFlags());

		for (const EMG_VertexData &vd : vertex)
		{
			const EMG_VertexCommon *vtx = &vd.VertexUnion;

			if (vertex_out)
			{
				vss << "v " << EMO_BaseFile::FloatToString(vtx->pos_x) << " " << EMO_BaseFile::FloatToString(vtx->pos_y) << " " << EMO_BaseFile::FloatToString(vtx->pos_z) << " \n";
			}

			if (uvmap_out)
			{
				uvss << "vt " << EMO_BaseFile::FloatToString(vtx->text_u) << " " << EMO_BaseFile::FloatToString(-vtx->text_v) << " \n";
			}

			if (normal_out)
			{
				nss << "vn " << EMO_BaseFile::FloatToString(vtx->norm_x) << " " << EMO_BaseFile::FloatToString(vtx->norm_y) << " " << EMO_BaseFile::FloatToString(vtx->norm_z) << " \n";
			}
		}
	}

	if (topology_out)
	{
		for (const EMG_SubMesh &sm : submeshes)
		{
			if (write_group)
			{
				tss << "g " << sm.emm_material << " \n";
			}

			bool backface = false;
			int strip_size = 1;
			uint16_t idx1 = 0xFFFF, idx2 = 0xFFFF;
			unsigned int n_vert = 0;
			uint16_t idx[3];

			size_t f_index = 0;

			for (size_t i = 0; i < sm.faces.size(); i++, strip_size++)
			{
				if (strips != 0)
				{
					assert(f_index < sm.faces.size());

					uint16_t idx = 1 + sm.faces[f_index] + v_start_idx;
					f_index++;

					if (idx == idx1)
					{
						backface = !backface;
						idx1 = idx2;
						idx2 = idx;
						strip_size = 2;
						continue;
					}

					if (idx == idx2)
					{
						backface = !backface;
						strip_size = 1;
						continue;
					}

					if (strip_size >= 3)
					{
						if (backface)
						{
							tss << "f " << idx << "/" << idx << "/" << idx << " " << idx2 << "/" << idx2 << "/" << idx2 << " " << idx1 << "/" << idx1 << "/" << idx1 << " \n";
						}
						else
						{
							tss << "f " << idx1 << "/" << idx1 << "/" << idx1 << " " << idx2 << "/" << idx2 << "/" << idx2 << " " << idx << "/" << idx << "/" << idx << " \n";
						}
					}

					backface = !backface;
					idx1 = idx2;
					idx2 = idx;
				}
				else
				{
					assert(f_index < sm.faces.size());

					idx[n_vert] = 1 + sm.faces[f_index] + v_start_idx;
					f_index++;

					if (n_vert >= 2)
					{
						tss << "f " << idx[0] << "/" << idx[0] << "/" << idx[0] << " " << idx[1] << "/" << idx[1] << "/" << idx[1] << " " << idx[2] << "/" << idx[2] << "/" << idx[2] << " \n";
						n_vert = 0;
					}
					else
					{
						n_vert++;
					}
				}
			}
		}
	}

	if (vertex_out)
	{
		*vertex_out = vss.str();
	}

	if (uvmap_out)
	{
		*uvmap_out = uvss.str();
	}

	if (normal_out)
	{
		*normal_out = nss.str();
	}

	if (topology_out)
	{
		*topology_out = tss.str();
	}

	return vertex.size();
}

bool EMG_SubPart::InjectObj(const std::string &obj, bool do_uv, bool do_normal, int v_start_idx, int vt_start_idx, int vn_start_idx, bool show_error)
{
	std::vector<EMG_VertexCommon> vertex;
	size_t vtx_count = 0, uv_count = 0, n_count = 0;

	if (!EMG::ReadObj(obj, vertex, do_uv, do_normal, v_start_idx, vt_start_idx, vn_start_idx, &vtx_count, &uv_count, &n_count, show_error))
		return false;

	return InjectVertex(vertex, true, (uv_count != 0), (n_count != 0));
}











/*-------------------------------------------------------------------------------\
|                             readEmdSubMesh		                             |
\-------------------------------------------------------------------------------*/
void EMG_SubPart::readEmdSubMesh(EMDSubmesh* emd)
{
	strips = 0;						//strips it's about a little compression on faces index list, with some backface. but we don't care when rebuild from emd (and now it's take care on the reverse operation)
	
	flags = emd->vertex_type_flag;

	vectors[0] = emd->aabb_center_x;
	vectors[1] = emd->aabb_center_y;
	vectors[2] = emd->aabb_center_z;
	vectors[3] = emd->aabb_center_w;
	vectors[4] = emd->aabb_min_x;
	vectors[5] = emd->aabb_min_y;
	vectors[6] = emd->aabb_min_z;
	vectors[7] = emd->aabb_min_w;
	vectors[8] = emd->aabb_max_x;
	vectors[9] = emd->aabb_max_y;
	vectors[10] = emd->aabb_max_z;
	vectors[11] = emd->aabb_max_w;


	EMG_TexturesList textList;
	size_t nbtext = emd->definitions.size();
	for (size_t i = 0; i < nbtext; i++)
	{
		textList.textures.push_back(EMG_Texture());
		textList.textures.back().readEmdDef(&emd->definitions.at(i));
	}
	textures_lists.push_back(textList);


	size_t nbVertex = emd->vertices.size();
	for (size_t i = 0; i < nbVertex; i++)				//Todo: try to reduce number of values.
	{
		vertex.push_back(EMG_VertexData());
		vertex.back().readEmdVertex(&emd->vertices.at(i));
	}
}
/*-------------------------------------------------------------------------------\
|                             writeEmdSubMesh		                             |
\-------------------------------------------------------------------------------*/
void EMG_SubPart::writeEmdSubMesh(EMDSubmesh* emd, size_t textListIndex)
{
	emd->vertex_type_flag = flags;
	emd->vertex_size = EMDVertex::getSizeFromFlags(emd->vertex_type_flag, true);

	emd->aabb_center_x = vectors[0];
	emd->aabb_center_y = vectors[1];
	emd->aabb_center_z = vectors[2];
	emd->aabb_center_w = vectors[3];
	emd->aabb_min_x = vectors[4];
	emd->aabb_min_y = vectors[5];
	emd->aabb_min_z = vectors[6];
	emd->aabb_min_w = vectors[7];
	emd->aabb_max_x = vectors[8];
	emd->aabb_max_y = vectors[9];
	emd->aabb_max_z = vectors[10];
	emd->aabb_max_w = vectors[11];



	if (textListIndex < textures_lists.size())
	{
		EMG_TexturesList &textList = textures_lists.at(textListIndex);

		size_t nbtext = textList.textures.size();
		for (size_t k = 0; k < nbtext; k++)
		{
			emd->definitions.push_back(EMDSubmeshDefinition());
			textList.textures.at(k).writeEmdDef(&(emd->definitions.back()));
		}
	}

	size_t nbVertex = vertex.size();
	for (size_t k = 0; k < nbVertex; k++)
	{
		emd->vertices.push_back(EMDVertex());
		vertex.at(k).writeEmdVertex(&(emd->vertices.back()));
	}
}






#ifdef FBX_SUPPORT


bool EMG_SubPart::ExportSubMeshFbx(const EMO_Skeleton &skl, uint16_t submesh_idx, const std::vector<FbxNode *> &fbx_bones, FbxScene *scene) const
{
	assert(vertex.size() > 0);
	assert(vertex[0].size == (unsigned int)vertex[0].VertexUnion.getSizeFromFlags(true));

	if (submesh_idx >= submeshes.size())
		return false;

	const EMG_SubMesh &submesh = submeshes[submesh_idx];

	std::vector<EMG_VertexData> sm_vertex;
	std::vector<uint16_t> sm_triangles;

	GetSubMeshVertexAndTriangles(submesh_idx, sm_vertex, sm_triangles);

	//LOG_DEBUG("%s: Vertex = %x, triangles = %x\n", submeshes[submesh_idx].meta_name.c_str(), sm_vertex.size(), sm_triangles.size());

	FbxMesh *fbx_mesh = FbxMesh::Create(scene, submesh.meta_name.c_str());
	if (!fbx_mesh)
		return false;

	unsigned int control_point_base = 0;
	unsigned int total_control_points = sm_vertex.size();
	fbx_mesh->InitControlPoints(total_control_points);

	FbxGeometryElementNormal *fbx_normal = nullptr;
	if (vertex[0].VertexUnion.flags & EMG_VTX_FLAG_NORM)
	{
		fbx_normal = fbx_mesh->CreateElementNormal();
		fbx_normal->SetMappingMode(FbxGeometryElement::eByControlPoint);
		//fbx_normal->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
		fbx_normal->SetReferenceMode(FbxGeometryElement::eDirect);
	}

	FbxGeometryElementTangent *fbx_tangent = nullptr;
	if (vertex[0].VertexUnion.flags & EMG_VTX_FLAG_TANGENT)
	{
		fbx_tangent = fbx_mesh->CreateElementTangent();
		fbx_tangent->SetMappingMode(FbxGeometryElement::eByControlPoint);
		//fbx_tangent->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
		fbx_tangent->SetReferenceMode(FbxGeometryElement::eDirect);
	}

	FbxGeometryElementUV *fbx_uv = nullptr;
	if (vertex[0].VertexUnion.flags & EMG_VTX_FLAG_TEX)
	{
		fbx_uv = fbx_mesh->CreateElementUV("DiffuseUV");
		fbx_uv->SetMappingMode(FbxGeometryElement::eByControlPoint);
		//fbx_uv->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
		fbx_uv->SetReferenceMode(FbxGeometryElement::eDirect);
	}

	FbxGeometryElementUV *fbx_uv2 = nullptr;
	if (vertex[0].VertexUnion.flags & EMG_VTX_FLAG_TEX2)
	{
		fbx_uv2 = fbx_mesh->CreateElementUV("DiffuseUV2");
		fbx_uv2->SetMappingMode(FbxGeometryElement::eByControlPoint);
		//fbx_uv2->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
		fbx_uv2->SetReferenceMode(FbxGeometryElement::eDirect);
	}

	FbxGeometryElementVertexColor *fbx_vcolor = nullptr;
	if (vertex[0].VertexUnion.flags & EMG_VTX_FLAG_COLOR)
	{
		FbxGeometryElementVertexColor *fbx_vcolor = fbx_mesh->CreateElementVertexColor();
		fbx_vcolor->SetMappingMode(FbxGeometryElement::eByControlPoint);
		//fbx_uv->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
		fbx_vcolor->SetReferenceMode(FbxGeometryElement::eDirect);
	}


	
	FbxNode *mesh_node = FbxNode::Create(scene, submesh.meta_name.c_str());
	mesh_node->LclTranslation.Set(FbxVector4(0, 0, 0));
	mesh_node->LclScaling.Set(FbxVector4(1, 1, 1));
	mesh_node->LclRotation.Set(FbxVector4(0, 0, 0));


	//Material (to have the name of material inFbx, could change him in 3dsmax/blender and read Again from fbx to give him in emo/emg part)
	int fbx_material_index = 0;
	{
		FbxGeometryElementMaterial* lMaterialElement = fbx_mesh->CreateElementMaterial();
		lMaterialElement->SetMappingMode(FbxGeometryElement::eByPolygon);
		lMaterialElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);

		std::vector<FbxSurfaceMaterial *> materials;
		materials.resize(1);
		materials[0] = exportFBXMaterial(scene, submeshes[submesh_idx].emm_material);
		fbx_material_index = mesh_node->GetMaterialCount();
		mesh_node->AddMaterial(materials[0]);
	}
	


	FbxVector4 *control_points = fbx_mesh->GetControlPoints();

	for (size_t i = 0; i < sm_vertex.size(); i++)
	{
		const EMG_VertexData &v = sm_vertex[i];
		float r, g, b, a;
		r = g = b = a = 0.0f;

		control_points[control_point_base + i] = FbxVector4(v.VertexUnion.pos_x, v.VertexUnion.pos_y, v.VertexUnion.pos_z, 1.0);	//vector4 for uniform
		
		if (fbx_normal)
			fbx_normal->GetDirectArray().Add(FbxVector4(v.VertexUnion.norm_x, v.VertexUnion.norm_y, v.VertexUnion.norm_z, 1.0));	//vector4 for uniform
		if (fbx_tangent)
			fbx_tangent->GetDirectArray().Add(FbxVector4(v.VertexUnion.tang_x, v.VertexUnion.tang_y, v.VertexUnion.tang_z, 0.0));	//vector4 for uniform //TODO why 0.0 ? must be 1.0, non ?
		if (fbx_uv)
			fbx_uv->GetDirectArray().Add(FbxVector2(v.VertexUnion.text_u, -v.VertexUnion.text_v));
		if (fbx_uv2)
			fbx_uv2->GetDirectArray().Add(FbxVector2(v.VertexUnion.text2_u, -v.VertexUnion.text2_v));

		if (fbx_vcolor)
		{
			EMG::ColorToFloat(v.VertexUnion.color, &r, &g, &b, &a);
			fbx_vcolor->GetDirectArray().Add(FbxColor(r, g, b, a));
		}



		// No need to add tangent for export
		
	}

	assert((sm_triangles.size() % 3) == 0);

	for (size_t i = 0; i < sm_triangles.size(); i += 3)
	{
		//fbx_mesh->BeginPolygon(fbx_material_index);
		fbx_mesh->BeginPolygon(fbx_material_index);
		fbx_mesh->AddPolygon(control_point_base + sm_triangles[i]);
		fbx_mesh->AddPolygon(control_point_base + sm_triangles[i + 1]);
		fbx_mesh->AddPolygon(control_point_base + sm_triangles[i + 2]);
		fbx_mesh->EndPolygon();
	}

	mesh_node->SetNodeAttribute(fbx_mesh);
	ExportSubMeshFbxSkin(skl, submesh, sm_vertex, fbx_bones, scene, fbx_mesh, mesh_node->EvaluateGlobalTransform());

	scene->GetRootNode()->AddChild(mesh_node);
	return true;
}



FbxSurfaceMaterial *EMG_SubPart::exportFBXMaterial(FbxScene *scene, string material_name) const
{
	FbxString lMaterialName = material_name.c_str();
	FbxString lShadingName = "Phong";
	FbxDouble3 lBlack(0.0, 0.0, 0.0);
	FbxDouble3 lDiffuseColor(1.0, 1.0, 1.0);
	FbxSurfacePhong* lMaterial = FbxSurfacePhong::Create(scene, lMaterialName.Buffer());

	// Generate primary and secondary colors.
	
	lMaterial->Emissive.Set(lBlack);
	lMaterial->Ambient.Set(lBlack);
	lMaterial->AmbientFactor.Set(1.0);
	// Add texture for diffuse channel
	lMaterial->Diffuse.Set(lDiffuseColor);
	lMaterial->DiffuseFactor.Set(1.0);
	lMaterial->ShadingModel.Set(lShadingName);
	lMaterial->Shininess.Set(0.0);
	lMaterial->Specular.Set(lBlack);
	lMaterial->SpecularFactor.Set(0.0);
	

	return (FbxSurfaceMaterial*)lMaterial;
}

void EMG_SubPart::ExportSubMeshFbxSkin(const EMO_Skeleton &skl, const EMG_SubMesh &submesh, const std::vector<EMG_VertexData> &sm_vertex, const std::vector<FbxNode *> &fbx_bones, FbxScene *scene, FbxMesh *fbx_mesh, FbxAMatrix skin_matrix) const
{
	FbxSkin* skin = FbxSkin::Create(scene, "");
	std::vector<FbxCluster *> skin_clusters;

	UNUSED(skl);


	/*skin_clusters.resize(fbx_bones.size());

	for (size_t i = 0; i < fbx_bones.size(); i++)
	{
	FbxCluster *cluster = nullptr; */

	/*const int cluster_count = scene->GetSrcObjectCount<FbxCluster>();
	for (int j = 0; j < cluster_count; j++)
	{
	FbxCluster *this_cluster = scene->GetSrcObject<FbxCluster>(j);
	FbxString name = this_cluster->GetName();

	if (name == fbx_bones[i]->GetName())
	{
	cluster = this_cluster;
	break;
	}
	}*/ /*

	if (!cluster)
	{
	cluster = FbxCluster::Create(scene, fbx_bones[i]->GetName());

	cluster->SetLink(fbx_bones[i]);
	cluster->SetLinkMode(FbxCluster::eTotalOne);
	//cluster->SetTransformMatrix(skin_matrix);

	FbxAMatrix XMatrix = fbx_bones[i]->EvaluateGlobalTransform();
	cluster->SetTransformLinkMatrix(XMatrix);
	}

	skin->AddCluster(cluster);
	skin_clusters[i] = cluster;
	}*/

	skin_clusters.resize(submesh.linked_bones.size());

	for (size_t i = 0; i < skin_clusters.size(); i++)
	{
		EMO_Bone *const &bone = submesh.linked_bones[i];
		FbxNode *fbx_bone = nullptr;

		for (size_t j = 0; j < fbx_bones.size(); j++)
		{
			if (bone->GetName() == fbx_bones[j]->GetName())
			{
				fbx_bone = fbx_bones[j];
				break;
			}
		}

		assert(fbx_bone);

		FbxCluster *cluster = FbxCluster::Create(scene, fbx_bone->GetName());

		cluster->SetLink(fbx_bone);
		cluster->SetLinkMode(FbxCluster::eTotalOne);
		//cluster->SetTransformMatrix(skin_matrix);

		FbxAMatrix XMatrix = fbx_bone->EvaluateGlobalTransform();
		cluster->SetTransformLinkMatrix(XMatrix);

		skin->AddCluster(cluster);
		skin_clusters[i] = cluster;
	}

	for (size_t i = 0; i < sm_vertex.size(); i++)
	{
		const EMG_VertexData &v = sm_vertex[i];

		for (size_t j = 0; j < 4; j++)
		{
			const float *blend_weight = nullptr;
			const uint8_t *blend = nullptr;

			float this_weight = 0.0f;

			if ((v.VertexUnion.flags & EMG_VTX_FLAG_BLEND_WEIGHT) && (blend_weight))
			{
				blend_weight = v.VertexUnion.blend_weight;
				blend = v.VertexUnion.blend;


				this_weight = ((j != 3) ? blend_weight[j] : (1.0f - (blend_weight[0] + blend_weight[1] + blend_weight[2])));	//hack for having a sum(blend_weight) = 1.0 with the four, so rewrite the fourth
			}

			//if (this_weight > 0.0f)
			if(blend)
			{
				uint8_t linked_bone_index = blend[j];

				assert(linked_bone_index < submesh.linked_bones.size());
				EMO_Bone *bone = submesh.linked_bones[linked_bone_index];

				/*bone_index = skl.EMO_BoneToIndex(bone);

				assert(bone->GetName() == skin_clusters[bone_index]->GetName().Buffer());
				skin_clusters[bone_index]->AddControlPointIndex(i, this_weight);*/
				//assert(bone->GetName() == skin_clusters[linked_bone_index]->GetName().Buffer());		//TODO
				skin_clusters[linked_bone_index]->AddControlPointIndex(i, this_weight);

				UNUSED(bone); // Stupid compiler doesn't see bone is used in assert.
			}
		}
	}

	fbx_mesh->AddDeformer(skin);
}



bool EMG_SubPart::ExportFbx(const EMO_Skeleton &skl, const std::vector<FbxNode *> &fbx_bones, FbxScene *scene) const
{
	for (size_t i = 0; i < submeshes.size(); i++)
		if (!ExportSubMeshFbx(skl, i, fbx_bones, scene))
			return false;

	return true;
}


bool EMG_SubPart::LoadFbxUV(FbxMesh *fbx_mesh, int poly_index, int pos_in_poly, int control_point_index, EMG_VertexData *v)
{
	
	if (fbx_mesh->GetElementUVCount() <= 0)
	{
		LOG_DEBUG("%s: No UV!\n", FUNCNAME);
		return false;
	}

	FbxGeometryElementUV* uv = fbx_mesh->GetElementUV(0);

	switch (uv->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:

		switch (uv->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			v->VertexUnion.text_u = (float)uv->GetDirectArray().GetAt(control_point_index)[0];
			v->VertexUnion.text_v = (float)uv->GetDirectArray().GetAt(control_point_index)[1];
			break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int id = uv->GetIndexArray().GetAt(control_point_index);
			v->VertexUnion.text_u = (float)uv->GetDirectArray().GetAt(id)[0];
			v->VertexUnion.text_v = (float)uv->GetDirectArray().GetAt(id)[1];
		}

		break;

		default:
			LOG_DEBUG("%s: Unsupported reference mode.\n", FUNCNAME);
			return false;
			break;
		}

		break;

	case FbxGeometryElement::eByPolygonVertex:
	{
		int texture_uv_index = fbx_mesh->GetTextureUVIndex(poly_index, pos_in_poly);
		switch (uv->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
			v->VertexUnion.text_u = (float)uv->GetDirectArray().GetAt(texture_uv_index)[0];
			v->VertexUnion.text_v = (float)uv->GetDirectArray().GetAt(texture_uv_index)[1];

			break;

		default:
			LOG_DEBUG("%s: Unsupported reference mode (2)\n", FUNCNAME);
			return false;
			break;
		}
	}
	break;

	default:
		LOG_DEBUG("%s: Unsupported mapping mode.\n", FUNCNAME);
		return false;
		break;
	}

	v->VertexUnion.text_u = v->VertexUnion.text_u;
	v->VertexUnion.text_v = -v->VertexUnion.text_v;
	return true;
}


bool EMG_SubPart::LoadFbxUV2(FbxMesh *fbx_mesh, int poly_index, int pos_in_poly, int control_point_index, EMG_VertexData *v)
{

	if (fbx_mesh->GetElementUVCount() <= 1)
	{
		LOG_DEBUG("%s: No UV!\n", FUNCNAME);
		return false;
	}

	FbxGeometryElementUV* uv = fbx_mesh->GetElementUV(1);

	switch (uv->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:

		switch (uv->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			v->VertexUnion.text2_u = (float)uv->GetDirectArray().GetAt(control_point_index)[0];
			v->VertexUnion.text2_v = (float)uv->GetDirectArray().GetAt(control_point_index)[1];
			break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int id = uv->GetIndexArray().GetAt(control_point_index);
			v->VertexUnion.text2_u = (float)uv->GetDirectArray().GetAt(id)[0];
			v->VertexUnion.text2_v = (float)uv->GetDirectArray().GetAt(id)[1];
		}

		break;

		default:
			LOG_DEBUG("%s: Unsupported reference mode.\n", FUNCNAME);
			return false;
			break;
		}

		break;

	case FbxGeometryElement::eByPolygonVertex:
	{
		int texture_uv_index = fbx_mesh->GetTextureUVIndex(poly_index, pos_in_poly);
		switch (uv->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
			v->VertexUnion.text2_u = (float)uv->GetDirectArray().GetAt(texture_uv_index)[0];
			v->VertexUnion.text2_v = (float)uv->GetDirectArray().GetAt(texture_uv_index)[1];

			break;

		default:
			LOG_DEBUG("%s: Unsupported reference mode (2)\n", FUNCNAME);
			return false;
			break;
		}
	}
	break;

	default:
		LOG_DEBUG("%s: Unsupported mapping mode.\n", FUNCNAME);
		return false;
		break;
	}

	v->VertexUnion.text2_u = v->VertexUnion.text2_u;
	v->VertexUnion.text2_v = -v->VertexUnion.text2_v;
	return true;
}

bool EMG_SubPart::LoadFbxNormal(FbxNode *fbx_node, int vertex_index, int control_point_index, EMG_VertexData *v)
{
	FbxVector4 vec4;
	FbxMesh *fbx_mesh = fbx_node->GetMesh();

	if (fbx_mesh->GetElementNormalCount() <= 0)
	{
		LOG_DEBUG("%s: no normals!\n", FUNCNAME);
		return false;
	}

	FbxGeometryElementNormal* normal = fbx_mesh->GetElementNormal(0);
	FbxGeometryElement::EMappingMode mapping_mode = normal->GetMappingMode();
	if (mapping_mode == FbxGeometryElement::eByControlPoint)
	{
		switch (normal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			vec4 = normal->GetDirectArray().GetAt(control_point_index);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int id = normal->GetIndexArray().GetAt(control_point_index);
			vec4 = normal->GetDirectArray().GetAt(id);
		}
		break;

		default:
			LOG_DEBUG("%s: Unsuuported reference mode.\n", FUNCNAME);
			return false;
			break;
		}
	}
	else if (mapping_mode == FbxGeometryElement::eByPolygonVertex)
	{
		switch (normal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			vec4 = normal->GetDirectArray().GetAt(vertex_index);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int id = normal->GetIndexArray().GetAt(vertex_index);
			vec4 = normal->GetDirectArray().GetAt(id);
		}
		break;

		default:
			LOG_DEBUG("%s: Unsupported reference mode (2)\n", FUNCNAME);
			return false;
			break;
		}
	}
	else
	{
		LOG_DEBUG("%s: Unsupported mapping mode.\n", FUNCNAME);
		return false;
	}

	/*FbxAMatrix rotation_matrix = fbx_node->EvaluateGlobalTransform();
	rotation_matrix.SetT(FbxVector4(0.0, 0.0, 0.0, 0.0));
	rotation_matrix.SetS(FbxVector4(1.0, 1.0, 1.0, 1.0));

	vec4 = rotation_matrix.MultT(vec4);*/

	// +++ --- -++ --+ -+- ++- +-+ +--
	v->VertexUnion.norm_x = (float)vec4[0];
	v->VertexUnion.norm_y = (float)vec4[1];
	v->VertexUnion.norm_z = (float)vec4[2];

	//normal->GetDirectArray().SetAt(control_point_index, vec4);

	return true;
}

bool EMG_SubPart::LoadFbxVertexColor(FbxMesh *fbx_mesh, int vertex_index, int control_point_index, uint32_t *color)
{
	if (fbx_mesh->GetElementVertexColorCount() <= 0)
	{
		LOG_DEBUG("%s: no vertex colors!\n", FUNCNAME);
		return false;
	}

	FbxGeometryElementVertexColor* vertex_color = fbx_mesh->GetElementVertexColor(0);
	FbxGeometryElement::EMappingMode mapping_mode = vertex_color->GetMappingMode();
	if (mapping_mode == FbxGeometryElement::eByControlPoint)
	{
		switch (vertex_color->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			FbxColor v_color = vertex_color->GetDirectArray().GetAt(control_point_index);
			*color = EMG::DoubleToColor(v_color.mRed, v_color.mGreen, v_color.mBlue, v_color.mAlpha);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int id = vertex_color->GetIndexArray().GetAt(control_point_index);
			FbxColor v_color = vertex_color->GetDirectArray().GetAt(id);
			*color = EMG::DoubleToColor(v_color.mRed, v_color.mGreen, v_color.mBlue, v_color.mAlpha);
		}
		break;

		default:
			LOG_DEBUG("%s: Unsuuported reference mode.\n", FUNCNAME);
			return false;
			break;
		}
	}
	else if (mapping_mode == FbxGeometryElement::eByPolygonVertex)
	{
		switch (vertex_color->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			FbxColor v_color = vertex_color->GetDirectArray().GetAt(vertex_index);
			*color = EMG::DoubleToColor(v_color.mRed, v_color.mGreen, v_color.mBlue, v_color.mAlpha);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int id = vertex_color->GetIndexArray().GetAt(vertex_index);
			FbxColor v_color = vertex_color->GetDirectArray().GetAt(id);
			*color = EMG::DoubleToColor(v_color.mRed, v_color.mGreen, v_color.mBlue, v_color.mAlpha);
		}
		break;

		default:
			LOG_DEBUG("%s: Unsupported reference mode (2)\n", FUNCNAME);
			return false;
			break;
		}
	}
	else
	{
		LOG_DEBUG("%s: Unsupported mapping mode.\n", FUNCNAME);
		return false;
	}

	return true;
}

bool EMG_SubPart::LoadFbxTangent(FbxMesh *fbx_mesh, int vertex_index, int control_point_index, EMG_VertexData *v)
{
	FbxVector4 vec4;

	if (fbx_mesh->GetElementTangentCount() <= 0)
	{
		LOG_DEBUG("%s: no tangents!\n", FUNCNAME);
		return false;
	}

	FbxGeometryElementTangent* tangent = fbx_mesh->GetElementTangent(0);
	FbxGeometryElement::EMappingMode mapping_mode = tangent->GetMappingMode();
	if (mapping_mode == FbxGeometryElement::eByControlPoint)
	{
		switch (tangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			vec4 = tangent->GetDirectArray().GetAt(control_point_index);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int id = tangent->GetIndexArray().GetAt(control_point_index);
			vec4 = tangent->GetDirectArray().GetAt(id);
		}
		break;

		default:
			LOG_DEBUG("%s: Unsuuported reference mode.\n", FUNCNAME);
			return false;
			break;
		}
	}
	else if (mapping_mode == FbxGeometryElement::eByPolygonVertex)
	{
		switch (tangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			vec4 = tangent->GetDirectArray().GetAt(vertex_index);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int id = tangent->GetIndexArray().GetAt(vertex_index);
			vec4 = tangent->GetDirectArray().GetAt(id);
		}
		break;

		default:
			LOG_DEBUG("%s: Unsupported reference mode (2)\n", FUNCNAME);
			return false;
			break;
		}
	}
	else
	{
		LOG_DEBUG("%s: Unsupported mapping mode.\n", FUNCNAME);
		return false;
	}

	v->VertexUnion.tang_x = (float)vec4[0];
	v->VertexUnion.tang_y = (float)vec4[1];
	v->VertexUnion.tang_z = (float)vec4[2];

	if (vec4[3] > 0) // direction			// oglanix : ??
	{
		v->VertexUnion.tang_x = -v->VertexUnion.tang_x;
		v->VertexUnion.tang_y = -v->VertexUnion.tang_y;
		v->VertexUnion.tang_z = -v->VertexUnion.tang_z;
	}

	return true;
}

// No binormals in emo, tex2 are tangent...
/*bool EMG_SubPart::LoadFbxBinormal(FbxMesh *fbx_mesh, int vertex_index, int control_point_index, EMG_VertexData *v)
{
FbxVector4 vec4;

if (fbx_mesh->GetElementBinormalCount() <= 0)
{
LOG_DEBUG("%s: no binormals!\n", FUNCNAME);
return false;
}

FbxGeometryElementBinormal* binormal = fbx_mesh->GetElementBinormal(0);
FbxGeometryElement::EMappingMode mapping_mode = binormal->GetMappingMode();
if (mapping_mode == FbxGeometryElement::eByControlPoint)
{
switch (binormal->GetReferenceMode())
{
case FbxGeometryElement::eDirect:
{
vec4 = binormal->GetDirectArray().GetAt(control_point_index);
}
break;

case FbxGeometryElement::eIndexToDirect:
{
int id = binormal->GetIndexArray().GetAt(control_point_index);
vec4 = binormal->GetDirectArray().GetAt(id);
}
break;

default:
LOG_DEBUG("%s: Unsuuported reference mode.\n", FUNCNAME);
return false;
break;
}
}
else if (mapping_mode == FbxGeometryElement::eByPolygonVertex)
{
switch (binormal->GetReferenceMode())
{
case FbxGeometryElement::eDirect:
{
vec4 = binormal->GetDirectArray().GetAt(vertex_index);
}
break;

case FbxGeometryElement::eIndexToDirect:
{
int id = binormal->GetIndexArray().GetAt(vertex_index);
vec4 = binormal->GetDirectArray().GetAt(id);
}
break;

default:
LOG_DEBUG("%s: Unsupported reference mode (2)\n", FUNCNAME);
return false;
break;
}
}
else
{
LOG_DEBUG("%s: Unsupported mapping mode.\n", FUNCNAME);
return false;
}

v->VertexUnion.vertex64.tex2[0] = -(float)vec4[0];
v->VertexUnion.vertex64.tex2[1] = -(float)vec4[1];
v->VertexUnion.vertex64.tex2[2] = -(float)vec4[2];

if (vec4[3] < 0) // direction
{
v->VertexUnion.vertex64.tex2[0] = -v->VertexUnion.vertex64.tex2[0];
v->VertexUnion.vertex64.tex2[1] = -v->VertexUnion.vertex64.tex2[1];
v->VertexUnion.vertex64.tex2[2] = -v->VertexUnion.vertex64.tex2[2];
}

return true;
}*/

bool EMG_SubPart::LoadFbxBlendData(EMO_Skeleton &skl, const std::vector<EMO_Bone *> &linked_bones, const std::vector<std::pair<double, FbxNode *> > &vertex_weights, uint8_t *blend, float *blend_weight)
{
	size_t size = vertex_weights.size();

	if (size > 4)
	{
		LOG_DEBUG("blend_weight has more than 4 elements (%d)! Not allowed.\n", size);
		return false;
	}

	blend[0] = blend[1] = blend[2] = blend[3] = 0;
	blend_weight[0] = blend_weight[1] = blend_weight[2] = 0.0f;

	for (size_t i = 0; i < size; i++)
	{

		FbxNode *fbx_bone = vertex_weights[i].second;
		assert(fbx_bone != nullptr);

		const char *bone_name = fbx_bone->GetName();
		EMO_Bone *b = skl.GetBone(bone_name);
		if (!b)
		{
			LOG_DEBUG("%s: Cannot find bone \"%s\"\n", bone_name);
			return false;
		}

		const std::vector<EMO_Bone *>::const_iterator it = std::find(linked_bones.begin(), linked_bones.end(), b);
		if (it == linked_bones.end())
		{
			LOG_DEBUG("%s: EMO_Bone is not in linked bones. Weird.\n", FUNCNAME);
			return false;
		}

		blend[i] = it - linked_bones.begin();

		if (i < 3)
			blend_weight[i] = (float)vertex_weights[i].first;
		else if (i == 3)
			blend_weight[3] = 1.0f - (blend_weight[0] + blend_weight[1] + blend_weight[2]);
		else
			blend_weight[i] = 0.0f;
	}

	return true;
}


bool EMG_SubPart::LoadFbxBlendWeights(FbxMesh *fbx_mesh, std::vector<std::vector<std::pair<double, FbxNode *>>> &weights)
{
	const int vertex_count = fbx_mesh->GetControlPointsCount();

	if (fbx_mesh->GetDeformerCount() == 0)
	{
		LOG_DEBUG("ERROR: deformer count is zero! (in submesh %s)\n", fbx_mesh->GetName());
		LibXenoverse::notifyError();
		return false;
	}

	if (fbx_mesh->GetDeformerCount() != 1)
	{
		LOG_DEBUG("WARNING: deformer count is different than 1 (in submesh %s)\n", fbx_mesh->GetName());
		LibXenoverse::notifyWarning();
	}

	FbxDeformer* deformer = fbx_mesh->GetDeformer(0);
	if (deformer->GetDeformerType() != FbxDeformer::eSkin)
	{
		LOG_DEBUG("Invalid deformer type.\n");
		LibXenoverse::notifyError();
		return false;
	}

	FbxSkin *skin = (FbxSkin *)deformer;
	weights.resize(vertex_count);

	const int cluster_count = skin->GetClusterCount();
	for (int i = 0; i < cluster_count; i++)
	{
		FbxCluster* cluster = skin->GetCluster(i);
		assert(cluster);
		FbxNode* linked_node = cluster->GetLink();
		assert(linked_node);

		const int vertex_index_count = cluster->GetControlPointIndicesCount();
		for (int j = 0; j < vertex_index_count; j++)
		{
			int index = cluster->GetControlPointIndices()[j];
			if (index >= vertex_count)
			{
				continue;
			}

			std::pair<double, FbxNode *> skin_link;

			skin_link.first = cluster->GetControlPointWeights()[j];
			skin_link.second = linked_node;

			bool added = false;
			std::vector<std::pair<double, FbxNode *>> &skin_bindings = weights[index];

			for (size_t k = 0; k < skin_bindings.size(); k++)
			{
				if (skin_bindings[k].first < skin_link.first)
				{
					skin_bindings.insert(skin_bindings.begin() + k, skin_link);
					added = true;
					break;
				}
			}

			if (!added)
				weights[index].push_back(skin_link);
		}
	}

	return true;
}




bool EMG_SubPart::InjectSubMeshFbx(EMO_Skeleton &skl, EMG_SubMesh &submesh, FbxNode *fbx_node, std::vector<EMG_VertexData> &sm_vertex, int vertex_size, size_t v_start, bool use_fbx_tangent)
{
	std::vector<std::vector<std::pair<double, FbxNode *>>> blend_weights;
	FbxMesh *fbx_mesh = fbx_node->GetMesh();

	assert(fbx_mesh);

	
	//rebuid the flags.
	uint16_t flags = EMG_VTX_FLAG_POS;
	if (fbx_mesh->GetElementNormalCount()!=0)
		flags += EMG_VTX_FLAG_NORM;
	if (fbx_mesh->GetElementTangentCount() != 0)
		flags += EMG_VTX_FLAG_TANGENT;
	if (fbx_mesh->GetElementUVCount() != 0)
		flags += EMG_VTX_FLAG_TEX;
	if (fbx_mesh->GetElementUVCount() > 1)
		flags += EMG_VTX_FLAG_TEX2;
	if (fbx_mesh->GetElementVertexColorCount() != 0)
		flags += EMG_VTX_FLAG_COLOR;
	if (fbx_mesh->GetDeformerCount() != 0)
		flags += EMG_VTX_FLAG_BLEND_WEIGHT;
	
	if (flags & EMG_VTX_FLAG_BLEND_WEIGHT)
	{
		if (!LoadFbxBlendWeights(fbx_mesh, blend_weights))			//Todo regelr le probeleme sur le Bten01
		{
			flags -= EMG_VTX_FLAG_BLEND_WEIGHT;
		}
	}

	EMG_VertexCommon v_test_tmp;
	v_test_tmp.flags = flags;				//todo better
	if ((size_t)vertex_size<v_test_tmp.getSizeFromFlags(true))
		flags += EMG_VTX_FLAG_COMPRESSED_FORMAT;			//todo see if DBX accept non compressed file , if yes, we doesn't care about add it , if not, we have to add a argument.
	
	if (fbx_mesh->GetElementMaterialCount() != 0)
	{
		//submesh.SetEmmMaterial(fbx_mesh->GetElementMaterial(0)->GetName());
	}

	/*
	if (IsEdge())
	{
		//if (vertex_size == sizeof(EMG_Vertex64))		//TODO 
		//{
		//	LibXenoverse::notifyError();
		//	throw std::runtime_error("We weren't expecting an edge subpart with EMG_Vertex64 data! Report the emo file to eternity.\n");
		//}
		
		// both 3ds max and fbx sdk mess the normals of edge parts, so we have to generate them in inverse form.
		fbx_mesh->GenerateNormals(true, true, true);
	}

	//if (vertex_size == sizeof(EMG_Vertex64) && (!use_fbx_tangent))
	if (!use_fbx_tangent)				//TODO 
	{
		fbx_mesh->GenerateTangentsData(0, true);
	}
	*/

	
	
	if (flags & EMG_VTX_FLAG_BLEND_WEIGHT)
	{
		submesh.linked_bones.clear();

		for (auto &i : blend_weights)
		{
			for (std::pair<double, FbxNode *> &link : i)
			{
				const char *bone_name = link.second->GetName();
				EMO_Bone *b = skl.GetBone(bone_name);
				if (!b)
				{
					LOG_DEBUG("Cannot find bone \"%s\" in skeleton.\n", bone_name);
					return false;
				}

				if (std::find(submesh.linked_bones.begin(), submesh.linked_bones.end(), b) == submesh.linked_bones.end())
				{
					submesh.linked_bones.push_back(b);
				}
			}
		}
	}

	

	FbxVector4* control_points = fbx_mesh->GetControlPoints();
	const int polygon_count = fbx_mesh->GetPolygonCount();
	size_t vertex_index = 0;

	submesh.faces.resize(polygon_count * 3);
	sm_vertex.clear();

	for (int poly_index = 0; poly_index < polygon_count; poly_index++)
	{
		const int polygon_size = fbx_mesh->GetPolygonSize(poly_index);

		if (polygon_size != 3)
		{
			LOG_DEBUG("Unsupported polygin_size: %d\n", polygon_size);
			return false;
		}

		uint32_t *color;
		uint8_t *indices;
		float *weights;

		for (int pos_in_poly = 0; pos_in_poly < polygon_size; pos_in_poly++)
		{
			EMG_VertexData v;
			
			v.size = vertex_size;
			v.VertexUnion.flags = flags;


			int control_point_index = fbx_mesh->GetPolygonVertex(poly_index, pos_in_poly);
			FbxVector4& position = control_points[control_point_index];
			v.VertexUnion.pos_x = (float)position[0];
			v.VertexUnion.pos_y = (float)position[1];
			v.VertexUnion.pos_z = (float)position[2];


			if (flags & EMG_VTX_FLAG_NORM)
				LoadFbxNormal(fbx_node, vertex_index, control_point_index, &v);

			if (flags & EMG_VTX_FLAG_TANGENT)
				LoadFbxTangent(fbx_mesh, vertex_index, control_point_index, &v);

			if (flags & EMG_VTX_FLAG_TEX)
				LoadFbxUV(fbx_mesh, poly_index, pos_in_poly, control_point_index, &v);

			if (flags & EMG_VTX_FLAG_TEX2)
				LoadFbxUV2(fbx_mesh, poly_index, pos_in_poly, control_point_index, &v);

			if (flags & EMG_VTX_FLAG_COLOR)
			{
				color = &v.VertexUnion.color;
				LoadFbxVertexColor(fbx_mesh, vertex_index, control_point_index, color);
			}

			if (flags & EMG_VTX_FLAG_BLEND_WEIGHT)
			{
				indices = v.VertexUnion.blend;
				weights = v.VertexUnion.blend_weight;
				LoadFbxBlendData(skl, submesh.linked_bones, blend_weights[control_point_index], indices, weights);
			}


			auto it = std::find(sm_vertex.begin(), sm_vertex.end(), v);
			if (it == sm_vertex.end())
			{
				submesh.faces[(poly_index * 3) + pos_in_poly] = sm_vertex.size() + v_start;
				sm_vertex.push_back(v);
			} else {
				submesh.faces[(poly_index * 3) + pos_in_poly] = v_start + (it - sm_vertex.begin());
			}

			vertex_index++;
		}
	}

	return true;
}

bool EMG_SubPart::InjectFbx(EMO_Skeleton &skl, const std::vector<FbxNode *> fbx_nodes, bool use_fbx_tangent)
{
	LOG_DEBUG("Injecting subpart %s\n", meta_name.c_str());

	if (fbx_nodes.size() != submeshes.size())
	{
		LOG_DEBUG("%s: function usage error, number of fbx_meshes must be same as nuber of submeshes.\n", FUNCNAME);
		LibXenoverse::notifyError();
		return false;
	}

	if (submeshes.size() == 0)
		return true; // yes, true

	assert(vertex.size() > 0);
	const int vertex_size = vertex[0].size;

	size_t v_start = 0;
	//LOG_DEBUG("Vertex before: %d\n", vertex.size());
	vertex.clear();

	for (size_t i = 0; i < submeshes.size(); i++)
	{
		std::vector<EMG_VertexData> sm_vertex;

		if (!InjectSubMeshFbx(skl, submeshes[i], fbx_nodes[i], sm_vertex, vertex_size, v_start, use_fbx_tangent))
			return false;

		vertex.resize(v_start + sm_vertex.size());

		for (size_t j = v_start; j < vertex.size(); j++)
		{
			vertex[j] = sm_vertex[j - v_start];
		}

		v_start += sm_vertex.size();
	}

	SetStrips(false);
	//LOG_DEBUG("Vertex after: %d\n", vertex.size());
	return true;
}

bool EMG_SubPart::InjectFbx(EMO_Skeleton &skl, FbxScene *scene, bool use_fbx_tangent)
{
	std::vector<FbxNode *> fbx_nodes;

	for (size_t i = 0; i < submeshes.size(); i++)
	{
		bool found = false;

		const int node_count = scene->GetSrcObjectCount<FbxNode>();
		for (int j = 0; j < node_count; j++)
		{
			FbxNode *node = scene->GetSrcObject<FbxNode>(j);
			if (!node)
				continue;

			FbxMesh *mesh = node->GetMesh();
			if (!mesh)
				continue;

			LOG_DEBUG("Name: %s\n", mesh->GetName());

			if (submeshes[i].meta_name == node->GetName())
			{
				fbx_nodes.push_back(node);
				found = true;
			}
		}

		if (!found)
		{
			LOG_DEBUG("EMG_SubMesh \"%s\" not found in fbx, we will delete it from .emo.\n", submeshes[i].meta_name.c_str());
			submeshes.erase(submeshes.begin() + i);
			i--;
		}
	}

	if (submeshes.size() == 0)
		return true; // All submeshes were deleted

	return InjectFbx(skl, fbx_nodes, use_fbx_tangent);
}

#endif

bool EMG_SubPart::operator==(const EMG_SubPart &rhs) const
{
	if (this->textures_lists != rhs.textures_lists)
		return false;

	if (this->vertex != rhs.vertex)
		return false;

	if (this->submeshes != rhs.submeshes)
		return false;

	if (this->strips != rhs.strips)
		return false;

	if (memcmp(this->vectors, rhs.vectors, sizeof(this->vectors)) != 0)
		return false;

	if (this->flags != rhs.flags)
		return false;

	if (this->unk_02 != rhs.unk_02)
		return false;

	if (this->unk_06 != rhs.unk_06)
		return false;

	if (this->unk_08 != rhs.unk_08)
		return false;

	return true;
}





//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////




size_t EMG_TexturesList::GetEmbIndexes(std::vector<uint8_t> &list, bool clear_vector, bool unique, bool sort) const
{
	size_t count = 0;

	if (clear_vector)
		list.clear();

	for (const EMG_Texture &t : textures)
	{
		if (unique)
		{
			if (std::find(list.begin(), list.end(), t.emb_TextureIndex) != list.end())
				continue;
		}

		list.push_back(t.emb_TextureIndex);
		count++;
	}

	if (sort)
	{
		std::sort(list.begin(), list.end());
	}

	return count;
}

size_t EMG_TexturesList::ReplaceEmbIndex(uint8_t old_index, uint8_t new_index)
{
	size_t count = 0;

	for (EMG_Texture &t : textures)
	{
		if (t.emb_TextureIndex == old_index)
		{
			t.emb_TextureIndex = new_index;
			count++;
		}
	}

	return count;
}











//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////






size_t EMG_SubMesh::GetLinkedBones(std::vector<EMO_Bone *> &list, bool clear_vector, bool unique, const EMO_Skeleton *sorter) const
{
	size_t count = 0;

	if (clear_vector)
	{
		list.clear();
	}

	for (EMO_Bone *b : linked_bones)
	{
		if (unique)
		{
			if (std::find(list.begin(), list.end(), b) != list.end())
				continue;
		}

		list.push_back(b);
		count++;
	}

	if (sorter)
	{
		std::sort(list.begin(), list.end(), BoneSorter(sorter));
	}

	return count;
}


bool EMG_SubMesh::SetEmmMaterial(const std::string &emm_material)
{
	if (emm_material.length() > MAX_EMM_MAT_LENGTH)
		return false;

	this->emm_material = emm_material;
	return true;
}

bool EMG_SubMesh::IsEdge() const
{
	return EMO_BaseFile::BeginsWith(emm_material, "edge", false);
}




std::vector<uint16_t> EMG_SubMesh::getRealFaceIndex(bool strips)
{
	std::vector<uint16_t> realFaces;

	if (!strips)
	{
		assert((faces.size() % 3) == 0);

		size_t nbfaces = faces.size();
		for (size_t i = 0; i < nbfaces; i++)
			realFaces.push_back(faces.at(i));
		
		return realFaces;
	}

	bool backface = false;
	int strip_size = 1;
	uint16_t idx1 = 0xFFFF, idx2 = 0xFFFF;
	size_t f_index = 0;

	for (size_t i = 0; i < faces.size(); i++, strip_size++)
	{
		assert(f_index < faces.size());

		uint16_t idx = faces[f_index];
		f_index++;

		if (idx == idx1)
		{
			backface = !backface;
			idx1 = idx2;
			idx2 = idx;
			strip_size = 2;
			continue;
		}

		if (idx == idx2)
		{
			backface = !backface;
			strip_size = 1;
			continue;
		}

		if (strip_size >= 3)
		{
			
			if (backface)
			{
				realFaces.push_back(idx);
				realFaces.push_back(idx2);
				realFaces.push_back(idx1);
			}
			else
			{
				realFaces.push_back(idx1);
				realFaces.push_back(idx2);
				realFaces.push_back(idx);
			}
		}

		backface = !backface;
		idx1 = idx2;
		idx2 = idx;
	}

	return realFaces;
}



/*-------------------------------------------------------------------------------\
|                             readEmdSubMesh		                             |
\-------------------------------------------------------------------------------*/
void EMG_SubMesh::readEmdSubMesh(EMDSubmesh* emd, size_t textlist_index, size_t indexTriangle, EMO* emo)
{
	tl_index = textlist_index;
	emm_material = emd->name;

	EMDTriangles &triangle = emd->triangles.at(indexTriangle);


	

	size_t nbFacesIndex = triangle.faces.size();
	for (size_t i = 0; i < nbFacesIndex; i++)
		faces.push_back(triangle.faces.at(i));

	size_t nbBones = triangle.bone_names.size();
	for (size_t i = 0; i < nbBones; i++)
	{
		EMO_Bone* bone = emo->GetBone(triangle.bone_names.at(i));
		linked_bones.push_back(bone);
	}

	//calculate barycenter
	vector<EMDVertex> &listVertex = emd->getVertices();
	size_t nbvertex = listVertex.size();
	for (size_t i = 0; i < nbFacesIndex; i++)
	{
		if (triangle.faces.at(i) >= nbvertex)
			continue;

		EMDVertex &vertex = listVertex.at(triangle.faces.at(i));

		if (i == 0)
		{
			barycenter[0] = vertex.pos_x;
			barycenter[1] = vertex.pos_y;
			barycenter[2] = vertex.pos_z;
		}else{

			barycenter[0] = barycenter[0] * (((float)i) / ((float)(i + 1))) + (vertex.pos_x / ((float)(i + 1)));
			barycenter[1] = barycenter[1] * (((float)i) / ((float)(i + 1))) + (vertex.pos_y / ((float)(i + 1)));
			barycenter[2] = barycenter[2] * (((float)i) / ((float)(i + 1))) + (vertex.pos_z / ((float)(i + 1)));			
		}
		
	}
	barycenter[3] = 1.0;
}
/*-------------------------------------------------------------------------------\
|                             writeEmdSubMesh		                             |
\-------------------------------------------------------------------------------*/
void EMG_SubMesh::writeEmdSubMesh(EMDSubmesh* emd, bool strips)
{
	emd->name = emm_material;

	EMDTriangles triangle;

	std::vector<uint16_t> realFaces = getRealFaceIndex(strips);

	size_t nbFacesIndex = realFaces.size();
	for (size_t i = 0; i < nbFacesIndex; i++)
		triangle.faces.push_back(realFaces.at(i));

	size_t nbBones = linked_bones.size();
	for (size_t i = 0; i < nbBones; i++)
		triangle.bone_names.push_back(linked_bones.at(i)->GetName());

	emd->triangles.push_back(triangle);
}








bool EMG_SubMesh::operator==(const EMG_SubMesh &rhs) const
{
	if (this->emm_material != rhs.emm_material)
		return false;

	if (this->tl_index != rhs.tl_index)
		return false;

	if (this->faces != rhs.faces)
		return false;

	if (memcmp(this->barycenter, rhs.barycenter, sizeof(this->barycenter)) != 0)
		return false;

	if (this->linked_bones.size() != rhs.linked_bones.size())
		return false;

	for (size_t i = 0; i < this->linked_bones.size(); i++)
	{
		assert(this->linked_bones[i] != nullptr && rhs.linked_bones[i] != nullptr);

		if (*this->linked_bones[i] != *rhs.linked_bones[i])
			return false;
	}

	return true;
}







//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////






bool EMG_Texture::operator==(const EMG_Texture &rhs) const
{
	return (this->emb_TextureIndex == rhs.emb_TextureIndex &&
		this->textScale_u == rhs.textScale_u &&
		this->textScale_v == rhs.textScale_v &&
		this->unk_00 == rhs.unk_00 &&
		this->unk_02[0] == rhs.unk_02[0] &&
		this->unk_02[1] == rhs.unk_02[1]);
}




/*-------------------------------------------------------------------------------\
|                             readEmdDef			                             |
\-------------------------------------------------------------------------------*/
void EMG_Texture::readEmdDef(EMDSubmeshDefinition* emd)
{
	unk_00 = emd->flag0;
	emb_TextureIndex = emd->texIndex;
	unk_02[0] = emd->adressMode_u + (emd->adressMode_v << 4);
	unk_02[1] = emd->filtering_minification + (emd->filtering_magnification << 4);
	textScale_u = emd->textScale_u;
	textScale_v = emd->textScale_v;
}
/*-------------------------------------------------------------------------------\
|                             writeEmdDef			                             |
\-------------------------------------------------------------------------------*/
void EMG_Texture::writeEmdDef(EMDSubmeshDefinition* emd)
{
	emd->flag0 = unk_00;
	emd->texIndex = emb_TextureIndex;
	emd->adressMode_u = unk_02[0] & 0x0F;									//Todo correct Emo
	emd->adressMode_v = (unk_02[0] & 0xF0) >> 4;									//Todo correct Emo
	emd->filtering_minification = unk_02[1] & 0x0F;									//Todo correct Emo
	emd->filtering_magnification = (unk_02[1] & 0xF0) >> 4;									//Todo correct Emo
	emd->textScale_u = textScale_u;
	emd->textScale_v = textScale_v;
}







//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////






void EMG::ColorToFloat(uint32_t color, float *r, float *g, float *b, float *a)
{
    uint32_t u_r, u_g, u_b, u_a;

    u_a = (color & 0xFF000000) >> 24;
    u_r = (color & 0x00FF0000) >> 16;
    u_g = (color & 0x0000FF00) >> 8;
    u_b = color&0xFF;

    *a = ((float) u_a) / 255.0f;
    *r = ((float) u_r) / 255.0f;
    *g = ((float) u_g) / 255.0f;
    *b = ((float) u_b) / 255.0f;
}

uint32_t EMG::DoubleToColor(double r, double g, double b, double a)
{
    uint32_t u_r, u_g, u_b, u_a;

    u_r = (uint32_t) (r * 255.0);
    u_g = (uint32_t) (g * 255.0);
    u_b = (uint32_t) (b * 255.0);
    u_a = (uint32_t) (a * 255.0);

    uint32_t color = (u_a << 24);
    color |= (u_r << 16);
    color |= (u_g << 8);
    color |= u_b;

    return color;
}






EMG::EMG()
{
    this->big_endian = false;
}

EMG::~EMG()
{
    Reset();
}

void EMG::Reset()
{
    subparts.clear();    
}

size_t EMG::GetNumVertex()
{
    size_t count = 0;

    for (EMG_SubPart &sp : subparts)
    {
        count += sp.vertex.size();
    }

    return count;
}


size_t EMG::GetLinkedBones(std::vector<EMO_Bone *> &list, bool clear_vector, bool unique, const EMO_Skeleton *sorter) const
{
    size_t count = 0;

    if (clear_vector)
        list.clear();

    for (const EMG_SubPart &sp : subparts)
    {
        count += sp.GetLinkedBones(list, false, unique);
    }

    if (sorter)
    {
        std::sort(list.begin(), list.end(), BoneSorter(sorter));
    }

    return count;
}

size_t EMG::GetEmmMaterials(std::vector<std::string> &list, bool clear_vector, bool unique) const
{
    size_t count = 0;

    if (clear_vector)
        list.clear();

    for (const EMG_SubPart &sp : subparts)
    {
        count += sp.GetEmmMaterials(list, false, unique);
    }

    return count;
}

size_t EMG::GetEmbIndexes(std::vector<uint8_t> &list, bool clear_vector, bool unique, bool sort) const
{
    size_t count = 0;

    if (clear_vector)
        list.clear();

    for (const EMG_SubPart &sp : subparts)
    {
        count += sp.GetEmbIndexes(list, false, unique);
    }

    if (sort)
    {
        std::sort(list.begin(), list.end());
    }

    return count;
}

size_t EMG::ReplaceEmmMaterial(const std::string &old_mat, const std::string &new_mat)
{
    if (new_mat.length() > EMG_SubMesh::MAX_EMM_MAT_LENGTH)
        return (size_t)-1;

    size_t count = 0;

    for (EMG_SubPart &sp : subparts)
    {
        count += sp.ReplaceEmmMaterial(old_mat, new_mat);
    }

    return count;
}

size_t EMG::ReplaceEmbIndex(uint8_t old_index, uint8_t new_index)
{
    size_t count = 0;

    for (EMG_SubPart &sp : subparts)
    {
        count += sp.ReplaceEmbIndex(old_index, new_index);
    }

    return count;
}

bool EMG::IsEdge() const
{
    size_t count = 0;

    for (const EMG_SubPart &sp : subparts)
    {
        if (sp.IsEdge())
            count++;
    }

    assert(count == 0 || count == subparts.size());
    return (count != 0);
}


unsigned int EMG::CalculatePartSize()
{
	unsigned int file_size = sizeof(EMG_ChunkHeader);

	if (this->subparts.size() != 0)
	{
		file_size += (this->subparts.size() - 1) * sizeof(uint32_t);
	}

	for (EMG_SubPart &sp : this->subparts)
	{
		if (file_size & 0xF)
			file_size += (0x10 - (file_size & 0xF));

		//LOG_DEBUG("File size in this subpart: %x\n", file_size);

		file_size += sizeof(EMG_Header);
		file_size += sp.textures_lists.size() * sizeof(uint32_t);

		for (EMG_TexturesList &tl : sp.textures_lists)
		{
			file_size += sizeof(uint32_t);
			file_size += tl.textures.size() * sizeof(EMG_EMG_TextureHeader);
		}

		file_size += sp.submeshes.size() * sizeof(uint32_t);

		for (EMG_SubMesh &sm : sp.submeshes)
		{
			if (file_size & 0xF)
				file_size += (0x10 - (file_size & 0xF));

			//LOG_DEBUG("File_size in submesh: %x\n", file_size);

			file_size += sizeof(EMG_EMG_SubMeshHeader);
			file_size += sm.faces.size() * sizeof(uint16_t);
			file_size += sm.linked_bones.size() * sizeof(uint16_t);
		}
	}

	return file_size;
}

unsigned int EMG::CalculateVertexSize()
{
	unsigned int file_size = 0;

	for (EMG_SubPart &sp : this->subparts)
	{
		if (sp.vertex.size() != 0)
		{
			file_size += sp.vertex.size() * sp.vertex[0].VertexUnion.getSizeFromFlags(true);
		}
	}

	return file_size;
}

unsigned int EMG::CreatePart(uint8_t *buf, EMO_Skeleton *skl, uint32_t *vertex_start)
{
	EMG_ChunkHeader *emgc = (EMG_ChunkHeader *)buf;
	unsigned int file_size = CalculatePartSize();
	uint32_t offset = 0;

	memset(buf, 0, file_size);

	emgc->signature = EMG_SIGNATURE;
	emgc->unk_04 = val16(this->unk_04);
	emgc->subparts_count = val16(this->subparts.size());

	offset = sizeof(EMG_ChunkHeader);
	if (this->subparts.size() != 0)
	{
		offset += (this->subparts.size() - 1) * sizeof(uint32_t);
	}

	for (size_t i = 0; i < this->subparts.size(); i++)
	{
		if (offset & 0xF)
			offset += (0x10 - (offset & 0xF));

		emgc->offsets[i] = val32(offset);

		EMG_Header *ehdr = (EMG_Header *)GetOffsetPtr(buf, offset, true);
		EMG_SubPart &subpart = this->subparts[i];

		assert(subpart.textures_lists.size() < 65536);
		assert(subpart.submeshes.size() < 65536);
		
		if (subpart.vertex.size() == 0)
			continue;
		assert(subpart.vertex.size() > 0);
		

		ehdr->flags = val16(subpart.flags);
		ehdr->unk_02 = val16(subpart.unk_02);
		ehdr->textures_lists_count = val16(subpart.textures_lists.size());
		ehdr->unk_06 = val16(subpart.unk_06);
		ehdr->unk_08 = val32(subpart.unk_08);

		ehdr->vertex_count = val16(subpart.vertex.size());
		ehdr->vertex_size = val16(subpart.vertex[0].size);
		ehdr->strips = val16(subpart.strips);
		ehdr->submesh_count = val16(subpart.submeshes.size());
		ehdr->vertex_offset = val32(*vertex_start - offset);
		*vertex_start += (subpart.vertex.size() * subpart.vertex[0].size);

		for (int j = 0; j < 12; j++)
		{
			copy_float(&ehdr->vectors[j], subpart.vectors[j]);
		}

		offset += sizeof(EMG_Header);
		ehdr->textures_lists_offset = val32(EMO_BaseFile::DifPointer(buf + offset, ehdr));

		uint32_t *ttable = (uint32_t *)GetOffsetPtr(buf, offset, true);
		offset += subpart.textures_lists.size() * sizeof(uint32_t);

		for (size_t j = 0; j < subpart.textures_lists.size(); j++)
		{
			ttable[j] = val32(EMO_BaseFile::DifPointer(buf + offset, ehdr));

			const EMG_TexturesList &textures_list = subpart.textures_lists[j];

			*(uint16_t *)(buf + offset) = val16(textures_list.textures.size());
			offset += sizeof(uint32_t); // yes, uint32_t: uint16_t + an empty uint16_t

			for (size_t k = 0; k < textures_list.textures.size(); k++)
			{
				EMG_EMG_TextureHeader *thdr = (EMG_EMG_TextureHeader *)GetOffsetPtr(buf, offset, true);
				const EMG_Texture &t = textures_list.textures[k];

				thdr->unk_00 = t.unk_00;
				thdr->emb_TextureIndex = t.emb_TextureIndex;
				thdr->unk_02[0] = t.unk_02[0];
				thdr->unk_02[1] = t.unk_02[1];
				copy_float(&thdr->textScale_u, t.textScale_u);
				copy_float(&thdr->textScale_v, t.textScale_v);

				offset += sizeof(EMG_EMG_TextureHeader);
			}
		}

		ehdr->submesh_list_offset = val32(EMO_BaseFile::DifPointer(buf + offset, ehdr));

		uint32_t *smtable = (uint32_t *)GetOffsetPtr(buf, offset, true);
		offset += subpart.submeshes.size() * sizeof(uint32_t);

		for (size_t j = 0; j < subpart.submeshes.size(); j++)
		{
			if (offset & 0xF)
				offset += (0x10 - (offset & 0xF));

			smtable[j] = val32(EMO_BaseFile::DifPointer(buf + offset, ehdr));

			EMG_EMG_SubMeshHeader *shdr = (EMG_EMG_SubMeshHeader *)GetOffsetPtr(buf, offset, true);
			EMG_SubMesh &submesh = subpart.submeshes[j];

			assert(submesh.emm_material.length() < 32);
			assert(submesh.faces.size() < 65536);
			assert(submesh.linked_bones.size() < 65536);

			copy_float(&shdr->barycenter[0], submesh.barycenter[0]);
			copy_float(&shdr->barycenter[1], submesh.barycenter[1]);
			copy_float(&shdr->barycenter[2], submesh.barycenter[2]);
			copy_float(&shdr->barycenter[3], submesh.barycenter[3]);

			shdr->tl_index = val16(submesh.tl_index);
			shdr->face_count = val16(submesh.faces.size());
			shdr->linked_bones_count = val16(submesh.linked_bones.size());
			strcpy(shdr->emm_material, submesh.emm_material.c_str());

			offset += sizeof(EMG_EMG_SubMeshHeader);
			uint16_t *faces = (uint16_t *)GetOffsetPtr(buf, offset, true);

			for (size_t k = 0; k < submesh.faces.size(); k++)
			{
				faces[k] = val16(submesh.faces[k]);
				offset += sizeof(uint16_t);
			}

			uint16_t *bones_idx = (uint16_t *)GetOffsetPtr(buf, offset, true);

			for (size_t k = 0; k < submesh.linked_bones.size(); k++)
			{
				uint16_t idx = skl->BoneToIndex(submesh.linked_bones[k]);
				assert(idx != 0xFFFF);

				bones_idx[k] = val16(idx);
				offset += sizeof(uint16_t);
			}
		}
	}

	//LOG_DEBUG("offset = %x, file_size = %x\n", offset, file_size);

	assert(offset == file_size);
	return file_size;
}




////////////////////////////////////////////////////////////////////////////////////////////////// Emo file / vertexBuffer





bool EMG::Load(uint8_t *buf, unsigned int size, EMO_Skeleton *skl)
{
	UNUSED(size);

	Reset();

	EMG_ChunkHeader *emgc = (EMG_ChunkHeader *)buf;

	if (memcmp(&emgc->signature, "#EMG", 4) != 0)
	{
		//LOG_DEBUG("%s: Bad file, #EMG signature was not found where expected (in group %s)\n", __PRETTY_FUNCTION__, group.name.c_str());
		return false;
	}

	this->unk_04 = val16(emgc->unk_04);

	for (uint16_t i = 0; i < val16(emgc->subparts_count); i++)
	{
		EMG_SubPart subpart;
		EMG_Header *ehdr = (EMG_Header *)GetOffsetPtr(emgc, emgc->offsets, i);

		assert(emgc->offsets[i] != 0);

		if (meta_name != "")
		{
			char sp_meta_name[2048];

			snprintf(sp_meta_name, sizeof(sp_meta_name), "%s_%04x", meta_name.c_str(), i);
			subpart.meta_name = sp_meta_name;
		}

		subpart.flags = val16(ehdr->flags);
		subpart.unk_02 = val16(ehdr->unk_02);
		subpart.unk_06 = val16(ehdr->unk_06);
		subpart.unk_08 = val32(ehdr->unk_08);
		subpart.strips = val16(ehdr->strips);

		printf("flags: 0x%04x\n", subpart.flags);


		for (int j = 0; j < 12; j++)
		{
			subpart.vectors[j] = val_float(ehdr->vectors[j]);
		}

		uint32_t *ttable = (uint32_t *)GetOffsetPtr(ehdr, ehdr->textures_lists_offset);

		for (uint16_t j = 0; j < val16(ehdr->textures_lists_count); j++)
		{
			EMG_TexturesList textures_list;
			uint16_t *pnum_th = (uint16_t *)GetOffsetPtr(ehdr, ttable, j);
			EMG_EMG_TextureHeader *thdr = (EMG_EMG_TextureHeader *)GetOffsetPtr(pnum_th, sizeof(uint32_t), true);

			if (pnum_th[1] != 0)
			{
				LOG_DEBUG("%s: pnum_th[1] not 0 as expected.\n", FUNCNAME);
				return false;
			}

			for (uint32_t k = 0; k < val16(*pnum_th); k++)
			{
				EMG_Texture texture;

				texture.unk_00 = thdr[k].unk_00;
				texture.unk_02[0] = thdr[k].unk_02[0];
				texture.unk_02[1] = thdr[k].unk_02[1];
				texture.emb_TextureIndex = thdr[k].emb_TextureIndex;
				texture.textScale_u = val_float(thdr[k].textScale_u);
				texture.textScale_v = val_float(thdr[k].textScale_v);

				textures_list.textures.push_back(texture);
			}

			subpart.textures_lists.push_back(textures_list);
		}

		uint32_t *smtable = (uint32_t *)GetOffsetPtr(ehdr, ehdr->submesh_list_offset);

		for (uint16_t j = 0; j < val16(ehdr->submesh_count); j++)
		{
			EMG_SubMesh submesh;
			EMG_EMG_SubMeshHeader *shdr = (EMG_EMG_SubMeshHeader *)GetOffsetPtr(ehdr, smtable, j);

			assert(smtable[j] != 0);

			if (subpart.meta_name != "")
			{
				char sm_meta_name[2048];
				snprintf(sm_meta_name, sizeof(sm_meta_name), "%s_%04x", subpart.meta_name.c_str(), j);

				submesh.meta_name = sm_meta_name;
			}

			printf("Submesh %d => mat :%s\n", j, shdr->emm_material);

			submesh.emm_material = shdr->emm_material;
			submesh.tl_index = val16(shdr->tl_index);
			submesh.barycenter[0] = val_float(shdr->barycenter[0]);
			submesh.barycenter[1] = val_float(shdr->barycenter[1]);
			submesh.barycenter[2] = val_float(shdr->barycenter[2]);
			submesh.barycenter[3] = val_float(shdr->barycenter[3]);

			uint16_t *faces = (uint16_t *)GetOffsetPtr(shdr, sizeof(EMG_EMG_SubMeshHeader), true);
			for (uint16_t k = 0; k < val16(shdr->face_count); k++)
			{
				submesh.faces.push_back(val16(faces[k]));
			}

			uint16_t *bones_idx = (uint16_t *)GetOffsetPtr(faces, val16(shdr->face_count) * sizeof(uint16_t), true);

			for (uint16_t k = 0; k < val16(shdr->linked_bones_count); k++)
			{
				if (val16(bones_idx[k]) >= skl->GetNumBones())
				{
					LOG_DEBUG("%s: bone 0x%x out of bounds in subpart %04x\n", FUNCNAME, k, i);
					return false;
				}

				submesh.linked_bones.push_back(&skl->GetBones()[val16(bones_idx[k])]);
			}

			subpart.submeshes.push_back(submesh);
		}

		if (ehdr->vertex_count == 0)
		{
			LOG_DEBUG("Number of vertex can't be 0!\n");
			return false;
		}


		uint16_t vertex_size = val16(ehdr->vertex_size);
		printf("vertex size: %d\n", vertex_size);

		if ((vertex_size != EMG_SubPart::getSizeFromFlags(subpart.flags, true)))
		{
			printf("%s: vertex size: %d is not compatible with flag : 0x%04x\n", FUNCNAME, vertex_size, subpart.flags);

			LOG_DEBUG("%s: vertex size: %d is not compatible with flag : 0x%04x\n", FUNCNAME, vertex_size, subpart.flags);
			return false;
		}

		uint8_t *vertex = GetOffsetPtr(ehdr, ehdr->vertex_offset);
		uint8_t *vertex_subpart = 0;

		uint8_t *p_uint8_tmp = nullptr;
		uint16_t *p_uint16_tmp = nullptr;
		
		uint32_t *p_uint32_tmp = nullptr;
		uint64_t *p_uint64_tmp = nullptr;
		uint64_t uint64_tmp = 0;
		float float32_tmp = 0.0f;
		uint16_t uint16_tmp = 0;
		float *float_tmp = nullptr;

		for (uint16_t j = 0; j < val16(ehdr->vertex_count); j++)
		{
			EMG_VertexData vd;

			vd.size = vertex_size;
			vd.VertexUnion.flags = subpart.flags;

			vertex_subpart = vertex;

			if (subpart.flags & EMG_VTX_FLAG_POS)
			{
				float_tmp = (float*)vertex_subpart;
				vd.VertexUnion.pos_x = val_float(float_tmp[0]);
				vd.VertexUnion.pos_y = val_float(float_tmp[1]);
				vd.VertexUnion.pos_z = val_float(float_tmp[2]);

				vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_POS, true);
			}

			


			if (!(subpart.flags & EMG_VTX_FLAG_COMPRESSED_FORMAT))
			{
				if (subpart.flags & EMG_VTX_FLAG_NORM)
				{
					float_tmp = (float*)vertex_subpart;
					vd.VertexUnion.norm_x = val_float(float_tmp[0]);
					vd.VertexUnion.norm_y = val_float(float_tmp[1]);
					vd.VertexUnion.norm_z = val_float(float_tmp[2]);

					vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_NORM, true);
				}

				if (subpart.flags & EMG_VTX_FLAG_TEX)
				{
					float_tmp = (float*)vertex_subpart;
					vd.VertexUnion.text_u = val_float(float_tmp[0]);
					vd.VertexUnion.text_v = val_float(float_tmp[1]);

					vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_TEX, true);
				}

				if (subpart.flags & EMG_VTX_FLAG_TEX2)
				{
					float_tmp = (float*)vertex_subpart;
					vd.VertexUnion.text2_u = val_float(float_tmp[0]);
					vd.VertexUnion.text2_v = val_float(float_tmp[1]);

					vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_TEX2, true);
				}

				if (subpart.flags & EMG_VTX_FLAG_TANGENT)
				{
					float_tmp = (float*)vertex_subpart;
					vd.VertexUnion.tang_x = val_float(float_tmp[0]);
					vd.VertexUnion.tang_y = val_float(float_tmp[1]);
					vd.VertexUnion.tang_z = val_float(float_tmp[2]);

					vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_TANGENT, true);
				}

			}else{

				if (subpart.flags & EMG_VTX_FLAG_NORM)
				{
					// is about 3 x float16 + 1 x float16 empty.
					p_uint16_tmp = (uint16_t*)vertex_subpart;
					vd.VertexUnion.norm_x = float16ToFloat(p_uint16_tmp[0]);
					vd.VertexUnion.norm_y = float16ToFloat(p_uint16_tmp[1]);
					vd.VertexUnion.norm_z = float16ToFloat(p_uint16_tmp[2]);

					vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_NORM | EMG_VTX_FLAG_COMPRESSED_FORMAT, true);
				}


				if (subpart.flags & EMG_VTX_FLAG_TEX)
				{
					//is about 2 x float16 in compressed version
					p_uint16_tmp = (uint16_t*)vertex_subpart;
					vd.VertexUnion.text_u = float16ToFloat(p_uint16_tmp[0]);
					vd.VertexUnion.text_v = float16ToFloat(p_uint16_tmp[1]);

					vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_TEX | EMG_VTX_FLAG_COMPRESSED_FORMAT, true);
				}

				if (subpart.flags & EMG_VTX_FLAG_TEX2)
				{
					//is about 2 x float16 in compressed version
					p_uint16_tmp = (uint16_t*)vertex_subpart;
					vd.VertexUnion.text2_u = float16ToFloat(p_uint16_tmp[0]);
					vd.VertexUnion.text2_v = float16ToFloat(p_uint16_tmp[1]);

					vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_TEX2 | EMG_VTX_FLAG_COMPRESSED_FORMAT, true);
				}

				if (subpart.flags & EMG_VTX_FLAG_TANGENT)
				{
					// is about 3 x float16 + 1 x float16 empty.
					p_uint16_tmp = (uint16_t*)vertex_subpart;
					vd.VertexUnion.tang_x = float16ToFloat(p_uint16_tmp[0]);
					vd.VertexUnion.tang_y = float16ToFloat(p_uint16_tmp[1]);
					vd.VertexUnion.tang_z = float16ToFloat(p_uint16_tmp[2]);

					vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_TANGENT | EMG_VTX_FLAG_COMPRESSED_FORMAT, true);
				}
			}



			if (subpart.flags & EMG_VTX_FLAG_COLOR)
			{
				p_uint32_tmp = (uint32_t*)vertex_subpart;
				vd.VertexUnion.color = val32(p_uint32_tmp[0]);

				vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_COLOR, true);
			}


			if (subpart.flags & EMG_VTX_FLAG_BLEND_WEIGHT)
			{
				//blend part
				p_uint8_tmp = vertex_subpart;
				for (size_t k = 0; k < 4; k++)
					vd.VertexUnion.blend[k] = p_uint8_tmp[ ((!big_endian) ? k : (3 - k)) ];

				p_uint8_tmp += 4;


				//blendWeight part
				if(! (subpart.flags & EMG_VTX_FLAG_COMPRESSED_FORMAT))
				{
					float_tmp = (float*)p_uint8_tmp;
					for (size_t k = 0; k < 4; k++)
						vd.VertexUnion.blend_weight[k] = val_float(float_tmp[k]);

					vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_BLEND_WEIGHT, true);

				}else{

					//is about 4 x 1 x float16 in compressed version
					p_uint16_tmp = (uint16_t*)p_uint8_tmp;
					for (size_t k = 0; k < 4; k++)
						vd.VertexUnion.blend_weight[k] = float16ToFloat(p_uint16_tmp[k]);

					vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_BLEND_WEIGHT | EMG_VTX_FLAG_COMPRESSED_FORMAT, true);
				}
			}

			subpart.vertex.push_back(vd);
			vertex += vertex_size;
		}

		this->subparts.push_back(subpart);
	}

	return true;
}




unsigned int EMG::CreateVertex(uint8_t *buf)
{
	unsigned int file_size = CalculateVertexSize();
	uint32_t offset = 0;

	memset(buf, 0, file_size);

	for (EMG_SubPart &sp : this->subparts)
	{
		if (sp.vertex.size() != 0)
		{
			int vertex_size = sp.vertex[0].VertexUnion.getSizeFromFlags(true);

			uint8_t *vertex_subpart = nullptr;

			uint8_t *p_uint8_tmp = nullptr;
			uint16_t *p_uint16_tmp = nullptr;
			uint32_t *p_uint32_tmp = nullptr;
			float float32_tmp = 0.0f;
			uint16_t uint16_tmp = 0;
			float *float_tmp = nullptr;
			uint64_t *p_uint64_tmp = nullptr;
			uint64_t uint64_tmp = 0;

			for (EMG_VertexData &vd : sp.vertex)
			{
				vertex_subpart = buf + offset;

				if (sp.flags & EMG_VTX_FLAG_POS)
				{
					float_tmp = (float*)vertex_subpart;
					copy_float(float_tmp, vd.VertexUnion.pos_x);
					copy_float(float_tmp + 1, vd.VertexUnion.pos_y);
					copy_float(float_tmp + 2, vd.VertexUnion.pos_z);

					vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_POS, true);
				}

				if (sp.flags & EMG_VTX_FLAG_NORM)
				{
					if(! (sp.flags & EMG_VTX_FLAG_COMPRESSED_FORMAT))
					{
						float_tmp = (float*)vertex_subpart;
						copy_float(float_tmp, vd.VertexUnion.norm_x);
						copy_float(float_tmp + 1, vd.VertexUnion.norm_y);
						copy_float(float_tmp + 2, vd.VertexUnion.norm_z);

						vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_NORM, true);
					}
					else{
						
						// is about 3 x float16 + 1 x float16 empty.
						p_uint16_tmp = (uint16_t*)vertex_subpart;
						copy_float16(p_uint16_tmp    , vd.VertexUnion.norm_x);
						copy_float16(p_uint16_tmp + 1, vd.VertexUnion.norm_y);
						copy_float16(p_uint16_tmp + 2, vd.VertexUnion.norm_z);
						
						vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_NORM | EMG_VTX_FLAG_COMPRESSED_FORMAT, true);
					}
				}


				if (sp.flags & EMG_VTX_FLAG_TANGENT)
				{
					if(! (sp.flags & EMG_VTX_FLAG_COMPRESSED_FORMAT))
					{
						float_tmp = (float*)vertex_subpart;
						copy_float(float_tmp, vd.VertexUnion.tang_x);
						copy_float(float_tmp + 1, vd.VertexUnion.tang_y);
						copy_float(float_tmp + 2, vd.VertexUnion.tang_z);

						vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_TANGENT, true);
					}
					else{

						// is about 3 x float16 + 1 x float16 empty.
						p_uint16_tmp = (uint16_t*)vertex_subpart;
						copy_float16(p_uint16_tmp, vd.VertexUnion.norm_x);
						copy_float16(p_uint16_tmp + 1, vd.VertexUnion.norm_y);
						copy_float16(p_uint16_tmp + 2, vd.VertexUnion.norm_z);

						vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_TANGENT | EMG_VTX_FLAG_COMPRESSED_FORMAT, true);
					}
				}


				if (sp.flags & EMG_VTX_FLAG_TEX)
				{
					if(! (sp.flags & EMG_VTX_FLAG_COMPRESSED_FORMAT))
					{
						float_tmp = (float*)vertex_subpart;
						copy_float(float_tmp, vd.VertexUnion.text_u);
						copy_float(float_tmp + 1, vd.VertexUnion.text_v);

						vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_TEX, true);
					}
					else{

						//is about 2 x float16 in compressed version
						p_uint16_tmp = (uint16_t*)vertex_subpart;
						copy_float16(p_uint16_tmp, vd.VertexUnion.text_u);
						copy_float16(p_uint16_tmp + 1, vd.VertexUnion.text_v);

						vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_TEX | EMG_VTX_FLAG_COMPRESSED_FORMAT, true);
					}
				}


				if (sp.flags & EMG_VTX_FLAG_TEX2)
				{
					if(! (sp.flags & EMG_VTX_FLAG_COMPRESSED_FORMAT))
					{
						float_tmp = (float*)vertex_subpart;
						copy_float(float_tmp, vd.VertexUnion.text2_u);
						copy_float(float_tmp + 1, vd.VertexUnion.text2_v);

						vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_TEX2, true);
					}
					else{
						//is about 2 x float16 in compressed version
						p_uint16_tmp = (uint16_t*)vertex_subpart;
						copy_float16(p_uint16_tmp, vd.VertexUnion.text2_u);
						copy_float16(p_uint16_tmp + 1, vd.VertexUnion.text2_v);

						vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_TEX2 | EMG_VTX_FLAG_COMPRESSED_FORMAT, true);
					}
				}

				if (sp.flags & EMG_VTX_FLAG_COLOR)
				{
					p_uint32_tmp = (uint32_t*)vertex_subpart;
					p_uint32_tmp[0] = val32(vd.VertexUnion.color);

					vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_COLOR, true);
				}

				if (sp.flags & EMG_VTX_FLAG_BLEND_WEIGHT)
				{
					//blend part
					p_uint8_tmp = vertex_subpart;
					for (size_t k = 0; k < 4; k++)
						p_uint8_tmp[k] = vd.VertexUnion.blend[ ((!big_endian) ? k : (3 - k)) ];

					p_uint8_tmp += 4;


					//blendWeight part
					if(! (sp.flags & EMG_VTX_FLAG_COMPRESSED_FORMAT))
					{
						float_tmp = (float*)p_uint8_tmp;
						for (size_t k = 0; k < 4; k++)
							copy_float(float_tmp + k, vd.VertexUnion.blend_weight[k]);

						vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_BLEND_WEIGHT, true);

					}else{

						//is about 4 x 1 x float16 in compressed version
						p_uint16_tmp = (uint16_t*)p_uint8_tmp;
						for (size_t k = 0; k < 4; k++)
							copy_float16(p_uint16_tmp + k, vd.VertexUnion.blend_weight[k]);

						vertex_subpart += EMG_SubPart::getSizeFromFlags(EMG_VTX_FLAG_BLEND_WEIGHT | EMG_VTX_FLAG_COMPRESSED_FORMAT, true);
					}
				}

				offset += vertex_size;
			}
		}
	}

	assert(offset == file_size);
	return file_size;
}



////////////////////////////////////////////////////////////////////////////////////////////////// Obj file


bool EMG::ReadObj(const std::string &obj, std::vector<EMG_VertexCommon> &vertex, bool do_uv, bool do_normal, size_t v_start_idx, size_t vt_start_idx, size_t vn_start_idx, size_t *vtx_count, size_t *uv_count, size_t *n_count, bool show_error)
{
	std::string line;
	size_t pos = 0;

	std::vector<EMG_VertexCommon> uv;
	std::vector<EMG_VertexCommon> normal;

	while (GetNextObjLine(obj, &pos, line))
	{
		if (line.length() < 3)
			continue;

		if (line[0] == 'v')
		{
			if (line[1] > 0 && line[1] <= ' ')
			{
				float x, y, z;

				if (sscanf(line.c_str() + 2, "%f %f %f", &x, &y, &z) == 3)
				{
					size_t size = vertex.size();

					assert(*vtx_count <= size);

					if (*vtx_count == size)
					{
						EMG_VertexCommon vc;

						vc.pos_x = x;
						vc.pos_y = y;
						vc.pos_z = z;

						vertex.push_back(vc);
					}
					else // <
					{
						vertex[*vtx_count].pos_x = x;
						vertex[*vtx_count].pos_y = y;
						vertex[*vtx_count].pos_z = z;
					}

					*vtx_count = *vtx_count + 1;
				}
			}
			else if (do_uv && line[1] == 't' && line[2] > 0 && line[2] <= ' ')
			{
				float u, v;

				if (sscanf(line.c_str() + 3, "%f %f", &u, &v) == 2)
				{
					size_t size = uv.size();

					assert(*uv_count <= size);

					if (*uv_count == size)
					{
						EMG_VertexCommon vc;

						vc.text_u = u;
						vc.text_v = -v;

						uv.push_back(vc);
					}
					else // <
					{
						uv[*uv_count].text_u = u;
						uv[*uv_count].text_v = -v;
					}

					*uv_count = *uv_count + 1;
				}
			}
			else if (do_normal && line[1] == 'n' && line[2] > 0 && line[2] <= ' ')
			{
				float x, y, z;

				if (sscanf(line.c_str() + 3, "%f %f %f", &x, &y, &z) == 3)
				{
					size_t size = normal.size();

					assert(*n_count <= size);

					if (*n_count == size)
					{
						EMG_VertexCommon vc;

						vc.norm_x = x;
						vc.norm_y = y;
						vc.norm_z = z;

						normal.push_back(vc);
					}
					else // <
					{
						normal[*n_count].norm_x = x;
						normal[*n_count].norm_y = y;
						normal[*n_count].norm_z = z;
					}

					*n_count = *n_count + 1;
				}
			}
		}
	}

	if (*vtx_count == 0)
	{
		if (show_error)
		{
			LOG_DEBUG("No geometry vertex in input file!\n");
			LibXenoverse::notifyError();
		}

		return false;
	}

	if (*uv_count > *vtx_count)
	{
		if (show_error)
		{
			LOG_DEBUG("ERROR: Number of uv coordinates can't be greater than number of geometry vertex.\n");
			LibXenoverse::notifyError();
		}

		return false;
	}

	if (*n_count > *vtx_count)
	{
		if (show_error)
		{
			LOG_DEBUG("ERROR: Number of vertex normals can't be greater than number of geometry vertex.\n");
			LibXenoverse::notifyError();
		}

		return false;
	}

	if (*uv_count > 0 || *n_count > 0)
	{
		pos = 0;

		while (GetNextObjLine(obj, &pos, line))
		{
			if (line.length() < 3)
				continue;

			if (line[0] == 'f' && line[1] > 0 && line[1] <= ' ')
			{
				uint32_t v_idx, uv_idx, n_idx;
				std::string current = line.substr(2);
				size_t count = 0;

				while (current.length() > 0)
				{
					if (sscanf(current.c_str(), "%u/%u/%u", &v_idx, &uv_idx, &n_idx) == 3)
					{
						//printf("%u %u %u\n", v_idx, uv_idx, n_idx);

						v_idx = v_idx - 1 - v_start_idx;
						uv_idx = uv_idx - 1 - vt_start_idx;
						n_idx = n_idx - 1 - vn_start_idx;

						if (v_idx >= vertex.size())
						{
							LOG_DEBUG("ERROR: vertex index out of bounds (%u) at line \"%s\"\n", v_idx, line.c_str());
							LibXenoverse::notifyError();
							return false;
						}

						if (uv_idx >= *uv_count && *uv_count != 0)
						{
							LOG_DEBUG("ERROR: texture coordinate index out of bounds (%u) at line \"%s\"\n", uv_idx, line.c_str());
							LibXenoverse::notifyError();
							return false;
						}

						if (n_idx >= *n_count && *n_count != 0)
						{
							LOG_DEBUG("ERROR: vertex normal index out of bounds (%u) at line \"%s\"\n", n_idx, line.c_str());
							LibXenoverse::notifyError();
							return false;
						}

						if (*uv_count != 0)
						{
							vertex[v_idx].text_u = uv[uv_idx].text_u;
							vertex[v_idx].text_v = uv[uv_idx].text_v;
						}

						if (*n_count != 0)
						{
							vertex[v_idx].norm_x = normal[n_idx].norm_x;
							vertex[v_idx].norm_y = normal[n_idx].norm_y;
							vertex[v_idx].norm_z = normal[n_idx].norm_z;
						}

						count++;
					}
					else
					{
						if (count == 0)
						{
							LOG_DEBUG("WARNING: this line is being ignored: \"%s\"\n", line.c_str());
							LibXenoverse::notifyWarning();
							count = 3; // To skip warning
							break;
						}
						else
						{
							LOG_DEBUG("WARNING: this line is being partially ignored: \"%s\"\n", line.c_str());
							LibXenoverse::notifyWarning();
							count = 3; // To skip warning
							break;
						}
					}

					std::string next;
					bool in_space = false;

					for (size_t i = 0; i < current.length(); i++)
					{
						if (!in_space)
						{
							if (current[i] > 0 && current[i] <= ' ')
							{
								in_space = true;
							}
						}
						else
						{
							if (current[i] > ' ')
							{
								next = current.substr(i);
								break;
							}
						}
					}

					current = next;
				} // end while

				if (count < 3)
				{
					LOG_DEBUG("WARNING: it was expected 3 groups or more, at line \"%s\"\n", line.c_str());
					LibXenoverse::notifyWarning();
				}
				else if (count > 3)
				{

				}
			}
		}
	}

	return true;
}

bool EMG::GetNextObjLine(const std::string &content, size_t *pos, std::string &line)
{
	if (*pos >= content.length())
		return false;

	size_t new_pos = content.find('\n', *pos);

	if (new_pos == std::string::npos)
	{
		line = content.substr(*pos);
	}
	else if (new_pos == *pos)
	{
		line = "";
		*pos = *pos + 1;
		return true;
	}
	else
	{
		line = content.substr(*pos, new_pos - *pos);
	}

	EMO_BaseFile::TrimString(line, true, true);
	*pos = new_pos;

	return true;
}


size_t EMG::ExportObj(std::string *vertex_out, std::string *uvmap_out, std::string *normal_out, std::string *topology_out, size_t v_start_idx, bool write_group) const
{
    if (!vertex_out && !uvmap_out && !normal_out && !topology_out)
        return 0;

    size_t count = 0;
    std::string ret_v, ret_uv, ret_n, ret_t;

    for (const EMG_SubPart &sp : subparts)
    {
        std::string v, uv, n, t;

        size_t ret = sp.ExportObj(&v, &uv, &n, &t, v_start_idx, write_group);

        if (ret > 0)
        {
            count += ret;
            v_start_idx += ret;

            ret_v += v;
            ret_uv += uv;
            ret_n += n;
            ret_t += t;
        }
    }

    if (vertex_out)
    {
        *vertex_out = ret_v;
    }

    if (uvmap_out)
    {
        *uvmap_out = ret_uv;
    }

    if (normal_out)
    {
        *normal_out = ret_n;
    }

    if (topology_out)
    {
        *topology_out = ret_t;
    }

    return count;
}








/*-------------------------------------------------------------------------------\
|                             readEmdMesh			                             |
\-------------------------------------------------------------------------------*/
void EMG::readEmdMesh(EMDMesh* emd, EMO* emo)
{
	name = emd->name;

	//there is little difference with EMDSubmesh level,
	//-the material is defined on EMG_SubMesh instead of EMG_SubPart
	//-EMDSubmesh's triangles is for bones number limitation of 24 for shaders. but here, EMG_SubMesh have a directly a bone's list.
	//the flags for vertexDefinition is on EMDSubmesh and EMG_SubPart
	//So, we have to deal with some combinaisons in side of Emo.

	size_t nbTriangles;
	size_t nbElements = emd->submeshes.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		EMDSubmesh* submesh = emd->submeshes.at(i);


		EMG_SubPart subpart;
		subpart.readEmdSubMesh(emd->submeshes.at(i));


		nbTriangles = submesh->triangles.size();
		for(size_t j=0;j<nbTriangles;j++)
		{
			EMG_SubMesh submesh;
			submesh.readEmdSubMesh(emd->submeshes.at(i), subpart.textures_lists.size()-1, j, emo);

			subpart.submeshes.push_back(submesh);

			if(emo)
				emo->material_count++;
		}


		subparts.push_back(subpart);
	}
}
/*-------------------------------------------------------------------------------\
|                             writeEmdMesh			                             |
\-------------------------------------------------------------------------------*/
void EMG::writeEmdMesh(EMDMesh* emd, string name)
{
	emd->name = name;

	//saddly, EMG_SubPart don't have material information, it's the EMG_SubMesh witch have it.
	//but we are on EMDSubmesh level. So, we have to make a EMDSubmesh by EMG_SubPart + EMG_SubMesh couple.

	EMDSubmesh* submesh;
	size_t nbSubMesh;
	size_t nbElements = subparts.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		EMG_SubPart &subpart = subparts.at(i);

		nbSubMesh = subpart.submeshes.size();
		for (size_t j = 0; j < nbSubMesh; j++)
		{
			submesh = new EMDSubmesh();
			
			//info venant de EMG_SubPart
			subpart.writeEmdSubMesh(submesh, subpart.submeshes.at(j).tl_index);


			//info venant de EMG_SubMesh
			subpart.submeshes.at(j).writeEmdSubMesh(submesh, subpart.GetStrips());


			if (submesh->aabb_min_x < emd->aabb_min_x) emd->aabb_min_x = submesh->aabb_min_x;
			if (submesh->aabb_min_y < emd->aabb_min_y) emd->aabb_min_y = submesh->aabb_min_y;
			if (submesh->aabb_min_z < emd->aabb_min_z) emd->aabb_min_z = submesh->aabb_min_z;
			if (submesh->aabb_min_w < emd->aabb_min_w) emd->aabb_min_w = submesh->aabb_min_w;
			if (submesh->aabb_max_x > emd->aabb_max_x) emd->aabb_max_x = submesh->aabb_max_x;
			if (submesh->aabb_max_y > emd->aabb_max_y) emd->aabb_max_y = submesh->aabb_max_y;
			if (submesh->aabb_max_z > emd->aabb_max_z) emd->aabb_max_z = submesh->aabb_max_z;
			if (submesh->aabb_max_w > emd->aabb_max_w) emd->aabb_max_w = submesh->aabb_max_w;


			emd->submeshes.push_back(submesh);
		}
	}

	emd->aabb_center_x = (emd->aabb_max_x + emd->aabb_min_x) / 2.0f;
	emd->aabb_center_y = (emd->aabb_max_y + emd->aabb_min_y) / 2.0f;
	emd->aabb_center_z = (emd->aabb_max_z + emd->aabb_min_z) / 2.0f;
	emd->aabb_center_w = (emd->aabb_max_w + emd->aabb_min_w) / 2.0f;
}





////////////////////////////////////////////////////////////////////////////////////////////////// Fbx file




#ifdef FBX_SUPPORT

bool EMG::ExportFbx(const EMO_Skeleton &skl, const std::vector<FbxNode *> &fbx_bones, FbxScene *scene) const
{
	for (const EMG_SubPart &sp : subparts)
	{
		if (!sp.ExportFbx(skl, fbx_bones, scene))
			return false;
	}

	return true;
}

bool EMG::InjectFbx(EMO_Skeleton &skl, FbxScene *scene, bool use_fbx_tangent)
{
	for (EMG_SubPart &sp : subparts)
	{
		if (!sp.InjectFbx(skl, scene, use_fbx_tangent))
			return false;
	}

	for (size_t i = 0; i < subparts.size(); i++)
	{
		if (subparts[i].submeshes.size() == 0)
		{
			LOG_DEBUG("Subpart \"%s\" has become empty. We will delete it.\n", subparts[i].meta_name.c_str());
			subparts.erase(subparts.begin() + i);
			i--;
		}
	}

	return true;
}

#endif

















/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// load/save the Xml version	/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////



/*-------------------------------------------------------------------------------\
|                             Decompile				                             |
\-------------------------------------------------------------------------------*/
void EMG::Decompile(TiXmlNode *root, uint16_t id) const
{
	TiXmlElement *entry_root = new TiXmlElement("EMG");

	EMO_BaseFile::WriteParamUnsigned(entry_root, "U_04", unk_04, true);

	size_t nbSubpart = subparts.size();
	for (size_t i = 0; i < nbSubpart; i++)
		subparts.at(i).Decompile(entry_root);
	
	root->LinkEndChild(entry_root);
}
/*-------------------------------------------------------------------------------\
|                             Decompile				                             |
\-------------------------------------------------------------------------------*/
void EMG_SubPart::Decompile(TiXmlNode *root) const
{
	TiXmlElement *entry_root = new TiXmlElement("SubPart");

	EMO_BaseFile::WriteParamUnsigned(entry_root, "STRIPS", strips);
	EMO_BaseFile::WriteParamMultipleFloats(entry_root, "AABB", std::vector<float>(vectors, vectors + 12));

	EMO_BaseFile::WriteParamUnsigned(entry_root, "FLAGS", flags, true);
	EMO_BaseFile::WriteParamUnsigned(entry_root, "U_02", unk_02, true);
	EMO_BaseFile::WriteParamUnsigned(entry_root, "U_06", unk_06, true);
	EMO_BaseFile::WriteParamUnsigned(entry_root, "U_08", unk_08, true);

	size_t nbElement = textures_lists.size();
	for (size_t i = 0; i < nbElement; i++)
		textures_lists.at(i).Decompile(entry_root);

	nbElement = submeshes.size();
	for (size_t i = 0; i < nbElement; i++)
		submeshes.at(i).Decompile(entry_root, GetNumberOfPolygons(i));

	for (const EMG_VertexData &v : vertex)
		v.Decompile(entry_root);

	root->LinkEndChild(entry_root);
}
/*-------------------------------------------------------------------------------\
|                             Decompile				                             |
\-------------------------------------------------------------------------------*/
void EMG_TexturesList::Decompile(TiXmlNode *root) const
{
	TiXmlElement *entry_root = new TiXmlElement("TexturesList");

	for (const EMG_Texture &t : textures)
		t.Decompile(entry_root);

	root->LinkEndChild(entry_root);
}
/*-------------------------------------------------------------------------------\
|                             Decompile				                             |
\-------------------------------------------------------------------------------*/
void EMG_Texture::Decompile(TiXmlNode *root) const
{
	TiXmlElement *entry_root = new TiXmlElement("Texture");

	EMO_BaseFile::WriteParamUnsigned(entry_root, "EMB_INDEX", emb_TextureIndex);
	EMO_BaseFile::WriteParamFloat(entry_root, "textScale_u", textScale_u);
	EMO_BaseFile::WriteParamFloat(entry_root, "textScale_v", textScale_v);
	EMO_BaseFile::WriteParamUnsigned(entry_root, "U_00", unk_00, true);
	EMO_BaseFile::WriteParamMultipleUnsigned(entry_root, "U_02", std::vector<uint8_t>(unk_02, unk_02 + 2), true);

	root->LinkEndChild(entry_root);
}
/*-------------------------------------------------------------------------------\
|                             Decompile				                             |
\-------------------------------------------------------------------------------*/
void EMG_SubMesh::Decompile(TiXmlNode *root, size_t polygon_count) const
{
	TiXmlElement *entry_root = new TiXmlElement("SubMesh");

	EMO_BaseFile::WriteParamString(entry_root, "Material", emm_material);
	EMO_BaseFile::WriteParamUnsigned(entry_root, "TEXTURES_LISTS_INDEX", tl_index, true);
	EMO_BaseFile::WriteParamMultipleFloats(entry_root, "Barycenter", std::vector<float>(barycenter, barycenter + 4));

	EMO_BaseFile::WriteComment(entry_root, EMO_BaseFile::ToString(faces.size()) + " faces. " + EMO_BaseFile::ToString(polygon_count) + " polygons.");
	EMO_BaseFile::WriteParamMultipleUnsigned(entry_root, "FACES", faces, true);


	std::string bones_names;
	for (EMO_Bone *b : linked_bones)
		bones_names += ((bones_names.length() != 0) ? ", " : "") + b->GetName();
	EMO_BaseFile::WriteParamString(entry_root, "LINKED_BONES", bones_names);

	root->LinkEndChild(entry_root);
}
/*-------------------------------------------------------------------------------\
|                             Decompile				                             |
\-------------------------------------------------------------------------------*/
void EMG_VertexData::Decompile(TiXmlNode *root) const
{
	TiXmlElement *entry_root = new TiXmlElement("Vertex");

	const EMG_VertexCommon *vc = &VertexUnion;

	//to getBack to know what to use. Notice : for read, to not break files from previous Decompilation (before flags, with only Vertex64 and ertex52 of Saint Seya)
	// we will read flags, but if there isn't, we will make it from presents tags
	EMO_BaseFile::WriteParamUnsigned(entry_root, "FLAGS", vc->flags, true);

	if ((vc->flags & EMG_VTX_FLAG_POS) != 0)
		EMO_BaseFile::WriteParamMultipleFloats(entry_root, "POS", { vc->pos_x, vc->pos_y, vc->pos_z });

	if ((vc->flags & EMG_VTX_FLAG_NORM) != 0)
		EMO_BaseFile::WriteParamMultipleFloats(entry_root, "NORM", { vc->norm_x, vc->norm_y, vc->norm_z });

	if ((vc->flags & EMG_VTX_FLAG_TANGENT) != 0)
		EMO_BaseFile::WriteParamMultipleFloats(entry_root, "TANG", { vc->tang_x, vc->tang_y, vc->tang_z });

	if ((vc->flags & EMG_VTX_FLAG_TEX) != 0)
		EMO_BaseFile::WriteParamMultipleFloats(entry_root, "TEX", { vc->text_u, vc->text_v });

	if ((vc->flags & EMG_VTX_FLAG_TEX2) != 0)
		EMO_BaseFile::WriteParamMultipleFloats(entry_root, "TEX2", { vc->text2_u, vc->text2_v });

	if ((vc->flags & EMG_VTX_FLAG_COLOR) != 0)
		EMO_BaseFile::WriteParamUnsigned(entry_root, "COLOR", vc->color, true);

	if ((vc->flags & EMG_VTX_FLAG_BLEND_WEIGHT) != 0)
	{
		EMO_BaseFile::WriteParamMultipleUnsigned(entry_root, "BLEND", std::vector<uint8_t>(vc->blend, vc->blend + 4), true);
		EMO_BaseFile::WriteParamMultipleFloats(entry_root, "BLEND_WEIGHT", std::vector<float>(vc->blend_weight, vc->blend_weight + 4));
	}

	root->LinkEndChild(entry_root);
}



















/*-------------------------------------------------------------------------------\
|                             Compile				                             |
\-------------------------------------------------------------------------------*/
bool EMG::Compile(const TiXmlElement *root, EMO_Skeleton *skl)
{
	subparts.clear();

	unsigned int unk_04 = 0;
	EMO_BaseFile::ReadParamUnsigned(root, "U_04", &unk_04);
	if (unk_04 > 0xFFFF)
	{
		LOG_DEBUG("%s: U_04 must be a 16 bits value.\n", FUNCNAME);
		unk_04 = unk_04 & 0xFFFF;
	}
	this->unk_04 = unk_04;

	
	for (const TiXmlElement *elem = root->FirstChildElement("SubPart"); elem; elem = elem->NextSiblingElement("SubPart"))
	{
		EMG_SubPart subpart;
		
		if (!subpart.Compile(elem, skl))
		{
			LOG_DEBUG("%s: compilation of SubPart failed.\n", FUNCNAME);
			continue;
		}

		char sp_meta_name[2048];
		snprintf(sp_meta_name, sizeof(sp_meta_name), "%s_%04x", meta_name.c_str(), subparts.size());
		subpart.meta_name = sp_meta_name;

		subparts.push_back(subpart);
	}

	return true;
}

/*-------------------------------------------------------------------------------\
|                             Compile				                             |
\-------------------------------------------------------------------------------*/
bool EMG_SubPart::Compile(const TiXmlElement *root, EMO_Skeleton *skl)
{
	submeshes.clear();
	textures_lists.clear();
	vertex.clear();

	unsigned int strips = 0;
	EMO_BaseFile::GetParamUnsigned(root, "STRIPS", &strips);
	if (strips > 0xFFFF)
	{
		LOG_DEBUG("%s: strips must be a 16 bits value.\n", FUNCNAME);
		strips = strips & 0xFFFF;
	}
	this->strips = strips;


	std::vector<float> vectorsAABB;
	EMO_BaseFile::GetParamMultipleFloats(root, "AABB", vectorsAABB);

	if (vectorsAABB.size() != 12)
	{
		LOG_DEBUG("%s: Invalid number of elements for \"AABB\". must be aabbCenter, aabbMin, aabbMax with vector4 each.\n", FUNCNAME);
		for (int i = vectorsAABB.size(); i < 12; i++)
			vectorsAABB.push_back(0.0f);
	}
	for (int i = 0; i < 12; i++)
		this->vectors[i] = vectorsAABB.at(i);



	unsigned int flags = 0;
	EMO_BaseFile::GetParamUnsigned(root, "FLAGS", &flags);
	if (flags > 0xFFFF)
	{
		LOG_DEBUG("%s: FLAGS must be a 16 bits value.\n", FUNCNAME);
		flags = flags & 0xFFFF;
	}
	this->flags = flags;

	unsigned int unk_02 = 0;
	EMO_BaseFile::GetParamUnsigned(root, "U_02", &unk_02);
	if (unk_02 > 0xFFFF)
	{
		LOG_DEBUG("%s: U_02 must be a 16 bits value.\n", FUNCNAME);
		unk_02 = unk_02 & 0xFFFF;
	}
	this->unk_02 = unk_02;


	unsigned int unk_06 = 0;
	EMO_BaseFile::GetParamUnsigned(root, "U_06", &unk_06);
	if (unk_06 > 0xFFFF)
	{
		LOG_DEBUG("%s: U_06 must be a 16 bits value.\n", FUNCNAME);
		unk_06 = unk_06 & 0xFFFF;
	}
	this->unk_06 = unk_06;


	this->unk_08 = 0;
	EMO_BaseFile::GetParamUnsigned(root, "U_08", &this->unk_08);


	
	for (const TiXmlElement *elem = root->FirstChildElement("SubMesh"); elem; elem = elem->NextSiblingElement("SubMesh"))
	{
		EMG_SubMesh submesh;

		if (!submesh.Compile(elem, skl))
		{
			LOG_DEBUG("%s: Compilation of SubMesh failed.\n", FUNCNAME);
			continue;
		}

		char sm_meta_name[2048];
		snprintf(sm_meta_name, sizeof(sm_meta_name), "%s_%04x", meta_name.c_str(), submeshes.size());
		submesh.meta_name = sm_meta_name;

		submeshes.push_back(submesh);
	}




	for (const TiXmlElement *elem = root->FirstChildElement("TexturesList"); elem; elem = elem->NextSiblingElement("TexturesList"))
	{
		EMG_TexturesList textList;

		if (!textList.Compile(elem))
		{
			LOG_DEBUG("%s: Compilation of EMG_TexturesList failed.\n", FUNCNAME);
			continue;
		}
		textures_lists.push_back(textList);
	}




	int vertex_size = -1;

	for (const TiXmlElement *elem = root->FirstChildElement("Vertex"); elem; elem = elem->NextSiblingElement("Vertex"))
	{
		
		//check all vertex have the same flags for vertex definition.
		if (vertex_size == -1)
		{
			vertex_size = getSizeFromFlags(this->flags);
		}else if (vertex_size != getSizeFromFlags(this->flags)) {
			LOG_DEBUG("%s: All vertex of the subpart must be of the same kind!\n", FUNCNAME);
			return false;
		}


		EMG_VertexData vd;

		if (!vd.Compile(elem, vertex_size))
		{
			LOG_DEBUG("%s: Compilation of Vertex failed.\n", FUNCNAME);
			continue;
		}
		vertex.push_back(vd);
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             Compile				                             |
\-------------------------------------------------------------------------------*/
bool EMG_TexturesList::Compile(const TiXmlElement *root)
{
	
	for (const TiXmlElement *elem = root->FirstChildElement("Texture"); elem != NULL; elem = elem->NextSiblingElement("Texture"))
	{
		EMG_Texture t;

		if (!t.Compile(elem))
		{
			LOG_DEBUG("%s: Compilation of texture failed.\n", FUNCNAME);
			continue;
		}
		textures.push_back(t);
	}

	return true;
}

/*-------------------------------------------------------------------------------\
|                             Compile				                             |
\-------------------------------------------------------------------------------*/
bool EMG_Texture::Compile(const TiXmlElement *root)
{

	unsigned int emb_index = 0;
	EMO_BaseFile::GetParamUnsigned(root, "EMB_INDEX", &emb_index);
	if (emb_index > 0xFF)
	{
		LOG_DEBUG("%s: emb_index must be a 8 bits value.\n", FUNCNAME);
		emb_index = emb_index & 0xFF;
	}
	this->emb_TextureIndex = emb_index;


	this->textScale_u = 1.0f;
	EMO_BaseFile::GetParamFloat(root, "textScale_u", &this->textScale_u);
	this->textScale_v = 1.0f;
	EMO_BaseFile::GetParamFloat(root, "textScale_v", &this->textScale_v);


	unsigned int unk_00 = 0;
	EMO_BaseFile::GetParamUnsigned(root, "U_00", &unk_00);
	if (unk_00 > 0xFF)
	{
		LOG_DEBUG("%s: U_00 must be a 8 bits value.\n", FUNCNAME);
		unk_00 = unk_00 & 0xFF;
	}
	this->unk_00 = unk_00;

	std::vector<uint8_t> unk_02;
	EMO_BaseFile::GetParamMultipleUnsigned(root, "U_02", unk_02);

	if (unk_02.size() != 2)
	{
		LOG_DEBUG("%s: invalid number of elements for \"U_02\".must be 2.\n", FUNCNAME);
		for (size_t i = unk_02.size(); i<2; i++)
			unk_02.push_back(0);
	}
	this->unk_02[0] = unk_02.at(0);
	this->unk_02[1] = unk_02.at(1);


	return true;
}
/*-------------------------------------------------------------------------------\
|                             Compile				                             |
\-------------------------------------------------------------------------------*/
bool EMG_SubMesh::Compile(const TiXmlElement *root, EMO_Skeleton *skl)
{
	emm_material = "";
	EMO_BaseFile::GetParamString(root, "Material", emm_material);

	if (emm_material.length() > 31)
	{
		LOG_DEBUG("%s: EMMMaterial can't have more than 31 characters. (faulting value = %s)\n", FUNCNAME, emm_material.c_str());
		emm_material = emm_material.substr(0, 31);
	}

	unsigned int tl_index = 0;
	EMO_BaseFile::GetParamUnsigned(root, "TEXTURES_LISTS_INDEX", &tl_index);
	if (tl_index > 0xFFFF)
	{
		LOG_DEBUG("%s: TEXTURES_LISTS_INDEX must be a 16 bits value.\n", FUNCNAME);
		tl_index = tl_index & 0xFFFF;
	}
	this->tl_index = tl_index;


	std::vector<float> vector;
	EMO_BaseFile::GetParamMultipleFloats(root, "VECTOR", vector);
	if (vector.size() != 4)
	{
		LOG_DEBUG("%s: Invalid size for \"VECTOR\"\n", FUNCNAME);
		for (size_t i = vector.size(); i < 4; i++)
			vector.push_back(0.0f);
	}
	memcpy(this->barycenter, vector.data(), sizeof(this->barycenter));



	if (!EMO_BaseFile::GetParamMultipleUnsigned(root, "FACES", faces))
	{
		LOG_DEBUG("%s: No faces indices detected. is it normal ? \"FACES\"\n", FUNCNAME);
	}



	std::vector<std::string> bone_names;
	EMO_BaseFile::GetParamMultipleStrings(root, "LINKED_BONES", bone_names);
	for (const std::string &s : bone_names)
	{
		EMO_Bone *bone = skl->GetBone(s);
		if (!bone)
		{
			LOG_DEBUG("%s: bone \"%s\" doesn't exist in skeleton.\n", FUNCNAME, s.c_str());
			continue;
		}
		linked_bones.push_back(bone);
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             Compile				                             |
\-------------------------------------------------------------------------------*/
bool EMG_VertexData::Compile(const TiXmlElement *root, unsigned int vertex_size)
{

	EMG_VertexCommon *vc = &VertexUnion;

	std::vector<float> f_vector;
	size_t nbFloats = 0;
	uint32_t u_tmp = 0;
	std::vector<uint8_t> blend;
	std::vector<float> blend_weight;


	uint32_t flags = 0;
	if (!EMO_BaseFile::GetParamUnsigned(root, "FLAGS", &flags))
	{
		//we didn't have the original Flags (could be a previous file from Vertex64 or Vertex52)
		//so we will rebuild it from tags presence.

		if (EMO_BaseFile::GetParamMultipleFloats(root, "POS", f_vector))
			flags += EMG_VTX_FLAG_POS;

		if (EMO_BaseFile::GetParamMultipleFloats(root, "NORM", f_vector))
			flags += EMG_VTX_FLAG_NORM;

		if (EMO_BaseFile::GetParamMultipleFloats(root, "TANG", f_vector))
			flags += EMG_VTX_FLAG_TANGENT;

		if (EMO_BaseFile::GetParamMultipleFloats(root, "TEX", f_vector))
			flags += EMG_VTX_FLAG_TEX;

		/*
		if (EMO_BaseFile::GetParamMultipleFloats(root, "TEX2", f_vector))
		flags += EMG_VTX_FLAG_TEX2;
		*/
		//hack from previous confusion between TEX2 and TANG. so old file could have TEX2 with 3 floats, but it's for TANG
		if (EMO_BaseFile::GetParamMultipleFloats(root, "TEX2", f_vector))
		{
			if (f_vector.size() == 3)
				flags += EMG_VTX_FLAG_TANGENT;
			else
				flags += EMG_VTX_FLAG_TEX2;
		}


		if (EMO_BaseFile::GetParamUnsigned(root, "COLOR", &u_tmp))
			flags += EMG_VTX_FLAG_COLOR;

		if ((EMO_BaseFile::GetParamMultipleUnsigned(root, "BLEND", blend)) && (EMO_BaseFile::GetParamMultipleFloats(root, "BLEND_WEIGHT", blend_weight)))
			flags += EMG_VTX_FLAG_BLEND_WEIGHT;

		f_vector.clear();
	}
	vc->flags = flags;

	if ((vertex_size == 0) || (vertex_size != vc->getSizeFromFlags()))
		return false;

	this->size = vc->getSizeFromFlags(true);


	if (((vc->flags & EMG_VTX_FLAG_POS) != 0) && (EMO_BaseFile::GetParamMultipleFloats(root, "POS", f_vector)))
	{
		nbFloats = f_vector.size();
		if (nbFloats>=1)											//Notice there is default value, so we just need to overide what we have.
			vc->pos_x = f_vector[0];
		if (nbFloats>=2)
			vc->pos_y = f_vector[1];
		if (nbFloats>=3)
			vc->pos_z = f_vector[2];
	}
	f_vector.clear();


	if (((vc->flags & EMG_VTX_FLAG_NORM) != 0) && (EMO_BaseFile::GetParamMultipleFloats(root, "NORM", f_vector)))
	{
		nbFloats = f_vector.size();
		if (nbFloats>=1)
			vc->norm_x = f_vector[0];
		if (nbFloats>=2)
			vc->norm_y = f_vector[1];
		if (nbFloats>=3)
			vc->norm_z = f_vector[2];
	}
	f_vector.clear();



	/*
	if (((vc->flags & EMG_VTX_FLAG_TANGENT) != 0) && (EMO_BaseFile::GetParamMultipleFloats(root, "TANG", f_vector)))
	{
	nbFloats = f_vector.size();
	if (nbFloats>=1)
	vc->tang_x = f_vector[0];
	if (nbFloats>=2)
	vc->tang_y = f_vector[1];
	if (nbFloats>=3)
	vc->tang_z = f_vector[2];
	}
	f_vector.clear();
	*/
	//hack from previous confusion between TEX2 and TANG. so old file could have TEX2 with 3 floats, but it's for TANG
	if ((vc->flags & EMG_VTX_FLAG_TANGENT) != 0)
	{
		if (!EMO_BaseFile::GetParamMultipleFloats(root, "TANG", f_vector))		//if we have the flags for TANG, but don't have it,
			EMO_BaseFile::GetParamMultipleFloats(root, "TEX2", f_vector);		//it's on old files, with TEX2 instead.

		nbFloats = f_vector.size();
		if (nbFloats>=1)
			vc->tang_x = f_vector[0];
		if (nbFloats>=2)
			vc->tang_y = f_vector[1];
		if (nbFloats>=3)
			vc->tang_z = f_vector[2];
	}
	f_vector.clear();


	if (((vc->flags & EMG_VTX_FLAG_TEX) != 0) && (EMO_BaseFile::GetParamMultipleFloats(root, "TEX", f_vector)))
	{
		nbFloats = f_vector.size();
		if (nbFloats>=1)
			vc->text_u = f_vector[0];
		if (nbFloats>=2)
			vc->text_v = f_vector[1];
	}
	f_vector.clear();



	if (((vc->flags & EMG_VTX_FLAG_TEX2) != 0) && (EMO_BaseFile::GetParamMultipleFloats(root, "TEX2", f_vector)))
	{
		nbFloats = f_vector.size();
		if (nbFloats>=1)
			vc->text2_u = f_vector[0];
		if (nbFloats>=2)
			vc->text2_v = f_vector[1];
	}
	f_vector.clear();

	if (((vc->flags & EMG_VTX_FLAG_COLOR) != 0) && (EMO_BaseFile::GetParamUnsigned(root, "COLOR", &(vc->color))))
	{
		
	}

	if (((vc->flags & EMG_VTX_FLAG_BLEND_WEIGHT) != 0) && (EMO_BaseFile::GetParamMultipleUnsigned(root, "BLEND", blend)) && (EMO_BaseFile::GetParamMultipleFloats(root, "BLEND_WEIGHT", blend_weight)))
	{
		size_t nbBlend = blend.size();
		nbFloats = blend_weight.size();

		for (size_t i = 0; i < 4; i++)
		{
			if (i<nbBlend)
				vc->blend[i] = blend[i];
			if (i<nbFloats)
				vc->blend_weight[i] = blend_weight[i];
		}
	}


	return true;
}










}
