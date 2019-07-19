#include <algorithm>

#include "EMO.h"
//#include "debug.h" TODO

namespace LibXenoverse 
{

/*
 * Structure of emo files
 *
 * EMO_Header
 * EMO_PartsGroupHeader
 * [REPEAT for num groups]
 *  align to 0x10
 *  EMO_PartHeader
 *  [REPEAT for num parts]
 *      Align to 0x10 bytes
 *      EMG_ChunkHeader
 *      [REPEAT for num subparts]
 *          Align to 0x10 bytes
 *          EMG_Header
 *          Materials list offsets
 *          [REPEAT for num materials lists]
 *              num_materials
 *              num_materials * EMGMaterialHeader
 *          [END REPEAT for num materials lists]
 *          submesh list offsets
 *          Align to 0x10 bytes
 *          [REPEAT for num submeshes]
 *              EMG_EMG_SubMeshHeader
 *              faces * num_faces
 *              bones idx * num_linked_bones
 *          [END REPEAT for num submeshes]
 *      [END REPEAT for num subparts]
 * align to 0x10 bytes
 * part names table
 * [END REPEAT for num parts]
 * [END REPEAT for num groups]
 * Align to 0x40 bytes
 * SkeltonFile
 * Vertex
*/



bool EMO_PartsGroup::IsEdge() const
{
	size_t count = 0;
	size_t non_empty = 0;

	for (const EMG &p : parts)
	{
		if (!p.IsEmpty())
		{
			non_empty++;

			if (p.IsEdge())
				count++;
		}
	}

	assert(count == 0 || count == non_empty);
	return (count != 0);
}

size_t EMO_PartsGroup::GetNumVertex()
{
	size_t count = 0;

	for (EMG &p : parts)
		count += p.GetNumVertex();

	return count;
}


size_t EMO_PartsGroup::GetLinkedBones(std::vector<EMO_Bone *> &list, bool clear_vector, bool unique, const EMO_Skeleton *sorter) const
{
	size_t count = 0;

	if (clear_vector)
		list.clear();

	for (const EMG &p : parts)
		count += p.GetLinkedBones(list, false, unique);

	if (sorter)
		std::sort(list.begin(), list.end(), BoneSorter(sorter));

	return count;
}




size_t EMO_PartsGroup::GetEmmMaterials(std::vector<std::string> &list, bool clear_vector, bool unique) const
{
	size_t count = 0;

	if (clear_vector)
		list.clear();

	for (const EMG &p : parts)
		count += p.GetEmmMaterials(list, false, unique);

	return count;
}
size_t EMO_PartsGroup::ReplaceEmmMaterial(const std::string &old_mat, const std::string &new_mat)
{
	if (new_mat.length() > EMG_SubMesh::MAX_EMM_MAT_LENGTH)
		return (size_t)-1;

	size_t count = 0;

	for (EMG &p : parts)
		count += p.ReplaceEmmMaterial(old_mat, new_mat);

	return count;
}




size_t EMO_PartsGroup::GetEmbIndexes(std::vector<uint8_t> &list, bool clear_vector, bool unique, bool sort) const
{
	size_t count = 0;

	if (clear_vector)
		list.clear();

	for (const EMG &p : parts)
	{
		count += p.GetEmbIndexes(list, false, unique);
	}

	if (sort)
		std::sort(list.begin(), list.end());

	return count;
}
size_t EMO_PartsGroup::ReplaceEmbIndex(uint8_t old_index, uint8_t new_index)
{
	size_t count = 0;

	for (EMG &p : parts)
	{
		count += p.ReplaceEmbIndex(old_index, new_index);
	}

	return count;
}




size_t EMO_PartsGroup::ExportObj(std::string *vertex_out, std::string *uvmap_out, std::string *normal_out, std::string *topology_out, size_t v_start_idx, bool write_group) const
{
	if (!vertex_out && !uvmap_out && !normal_out && !topology_out)
		return 0;

	size_t count = 0;
	std::string ret_v, ret_uv, ret_n, ret_t;

	for (const EMG &p : parts)
	{
		std::string v, uv, n, t;

		size_t ret = p.ExportObj(&v, &uv, &n, &t, v_start_idx, write_group);

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
|                             readEmdModel			                             |
\-------------------------------------------------------------------------------*/
void EMO_PartsGroup::readEmdModel(EMDModel* emd, EMO* emo, size_t &nbSection)
{
	name = emd->name;

	size_t nbElements = emd->meshes.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		parts.push_back(EMG());

		nbSection++;
		parts.back().unk_04 = nbSection;				//section increment.

		parts.back().readEmdMesh(emd->meshes.at(i), emo);
	}
}
/*-------------------------------------------------------------------------------\
|                             writeEmdModel			                             |
\-------------------------------------------------------------------------------*/
void EMO_PartsGroup::writeEmdModel(EMDModel* emd)
{
	emd->name = name;

	size_t nbElements = parts.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		emd->meshes.push_back(new EMDMesh());
		parts.at(i).writeEmdMesh(emd->meshes.back(), name + "_" + std::to_string(i));
	}
}




#ifdef FBX_SUPPORT

bool EMO_PartsGroup::ExportFbx(const EMO_Skeleton &skl, const std::vector<FbxNode *> &fbx_bones, FbxScene *scene) const
{
    for (const EMG &p : parts)
    {
        if (!p.IsEmpty() && !p.ExportFbx(skl, fbx_bones, scene))
            return false;
    }

    return true;
}

bool EMO_PartsGroup::InjectFbx(EMO_Skeleton &skl, FbxScene *scene, bool use_fbx_tangent)
{
    size_t first_empty = parts.size();

    for (size_t i = 0; i < parts.size(); i++)
    {
        if (parts[i].IsEmpty())
        {
            first_empty = i;
            break;
        }
    }

    for (EMG &p : parts)
    {
        if (!p.IsEmpty() && !p.InjectFbx(skl, scene, use_fbx_tangent))
            return false;
    }

    for (size_t i = 0; i < first_empty; i++)
    {
        if (parts[i].IsEmpty())
        {
            LOG_DEBUG("Part \"%s\" has become empty. We will delete it.\n", parts[i].meta_name.c_str());
            parts.erase(parts.begin() + i);
            i--;
            first_empty--;
        }
    }

    return true;
}

#endif








//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////






bool EMO_PartsSorter::operator()(const EMG &part1, const EMG &part2)
{
	if (part1.IsEmpty())
	{
		if (!part2.IsEmpty())
			return false;
	}
	else if (part2.IsEmpty()){
		return true;
	}

	uint16_t idx1 = GetIdx(part1);
	uint16_t idx2 = GetIdx(part2);

	assert(idx1 != 0xFFFF);
	assert(idx2 != 0xFFFF);

	return (idx1 < idx2);
}

uint16_t EMO_PartsSorter::GetIdx(const EMG &part)
{
	for (uint16_t i = 0; i < group.GetNumParts(); i++)
		if (group[i] == part)
			return i;

	return (uint16_t)-1;
}







//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////



EMO::EMO()
{
	name = "";
    Reset();
}
EMO::EMO(uint8_t *buf, unsigned int size)
{
	name = "";
	Load(buf, size);
}
EMO::~EMO()
{
    Reset();
}


EMO &EMO::operator = (const EMO &other)
{
	if (this == &other)
		return *this;

	Copy(other);
	return *this;
}
EMO_PartsGroup &EMO::operator[](const std::string &group_name)
{
	EMO_PartsGroup *pg = GetGroup(group_name);
	if (!pg)
		throw std::out_of_range("Parts group " + group_name + " doesn't exist.");

	return *pg;
}
const EMO EMO::operator+(const EMO_PartsGroup &group) const
{
	EMO new_emo = *this;

	new_emo.AppendGroup(group);
	return new_emo;
}
EMO &EMO::operator+=(const EMO_PartsGroup &group)
{
	this->AppendGroup(group);
	return *this;
}
void EMO::Copy(const EMO &other)
{
	EMO_Skeleton::Copy(other);

	this->groups = other.groups;
	this->material_count = other.material_count;
	memcpy(this->unk_08, other.unk_08, sizeof(this->unk_08));
	memcpy(this->unk_18, other.unk_18, sizeof(this->unk_18));

	for (EMO_PartsGroup &pg : groups)
	{
		for (EMG &p : pg.parts)
		{
			for (EMG_SubPart &sp : p.subparts)
			{
				for (EMG_SubMesh &sm : sp.submeshes)
				{
					for (EMO_Bone *&b : sm.linked_bones)
					{
						b = GetBone(b->GetName());
						assert(b != nullptr);
					}
				}
			}
		}
	}

	this->big_endian = other.big_endian;
}
void EMO::Reset()
{
	//EMO_Skeleton::Reset();			//TODO

	unk_08[0] = 37568;
	unk_08[1] = unk_18[0] = unk_18[1] = 0;
	groups.clear();
}




uint16_t EMO::GroupPtrToIndex(const EMO_PartsGroup *group) const
{
	for (size_t i = 0; i < groups.size(); i++)
		if (group == &groups[i])
			return i;

	return (uint16_t)-1;
}
EMO_PartsGroup *EMO::GetGroup(const std::string & name)
{
	for (EMO_PartsGroup &p : groups)
		if (p.GetName() == name)
			return &p;
	return nullptr;
}
bool EMO::RemoveGroup(uint16_t idx)
{
	if (idx >= groups.size())
		return false;

	groups.erase(groups.begin() + idx);
	return true;
}
bool EMO::RemoveGroup(const std::string &name)
{
	for (auto it = groups.begin(); it != groups.end(); ++it)
	{
		if ((*it).GetName() == name)
		{
			groups.erase(it);
			return true;
		}
	}
	return false;
}
uint16_t EMO::AppendGroup(const EMO_PartsGroup &group)
{
	groups.push_back(group);
	return (groups.size() - 1);
}
uint16_t EMO::AppendGroup(EMO &other, const std::string &name)
{
	EMO_PartsGroup *group = other.GetGroup(name);
	if (!group)
		return (uint16_t)-1;

	return AppendGroup(*group);
}
bool EMO::HasEdges() const
{
	for (const EMO_PartsGroup &pg : groups)
		if (pg.IsEdge())
			return true;

	return false;
}
size_t EMO::GetNumVertex()
{
	size_t count = 0;
	for (EMO_PartsGroup &pg : groups)
		count += pg.GetNumVertex();
	return count;
}





bool EMO::PartPtrToIndex(const EMG *part, uint16_t *group_idx, uint16_t *part_idx) const
{
	for (size_t i = 0; i < groups.size(); i++)
	{
		for (size_t j = 0; j < groups[i].GetNumParts(); j++)
		{
			if (part == &groups[i][j])
			{
				*group_idx = i;
				*part_idx = j;
				return true;
			}
		}
	}

	return false;
}
EMG *EMO::GetPart(const std::string &part_name)
{
	std::string group_name;
	unsigned int part_idx;

	size_t len = part_name.length();
	size_t pos = len - 5;

	if (len <= 5 || part_name[pos] != '_')
		return nullptr;

	group_name = part_name.substr(0, pos);

	EMO_PartsGroup *group = GetGroup(group_name);
	if (!group)
		return nullptr;

	if (sscanf(part_name.substr(pos + 1).c_str(), "%04x", &part_idx) != 1)
		return nullptr;

	return group->GetPart(part_idx);
}
bool EMO::RemovePart(const std::string &part_name)
{
	uint16_t group_idx, part_idx;

	EMG *part = GetPart(part_name);
	if (!part)
		return false;

	bool ret = PartPtrToIndex(part, &group_idx, &part_idx);
	assert(ret);

	groups[group_idx].parts.erase(groups[group_idx].begin() + part_idx);
	return ret;
}







bool EMO::SubPartPtrToIndex(const EMG_SubPart *subpart, uint16_t *group_idx, uint16_t *part_idx, uint16_t *subpart_idx) const
{
	for (size_t i = 0; i < groups.size(); i++)
	{
		for (size_t j = 0; j < groups[i].GetNumParts(); j++)
		{
			for (size_t k = 0; k < groups[i][j].GetNumSubParts(); k++)
			{
				if (subpart == &groups[i][j][k])
				{
					*group_idx = i;
					*part_idx = j;
					*subpart_idx = k;
					return true;
				}
			}
		}
	}

	return false;
}
EMG_SubPart* EMO::GetSubPart(const std::string &subpart_name)
{
	std::string part_name;
	unsigned int subpart_idx;

	size_t len = subpart_name.length();
	size_t pos = len - 5;

	if (len <= 5 || subpart_name[pos] != '_')
		return nullptr;

	part_name = subpart_name.substr(0, pos);

	EMG *part = GetPart(part_name);
	if (!part)
		return nullptr;

	if (sscanf(subpart_name.substr(pos + 1).c_str(), "%04x", &subpart_idx) != 1)
		return nullptr;

	return part->GetSubPart(subpart_idx);
}
size_t EMO::GetNextObjSubPart(const std::string &content, size_t *pos, std::string &ret_obj, std::string &ret_name, size_t *num_v, size_t *num_uv, size_t *num_n, bool show_error)
{
	std::string line;
	bool name_set = false;
	int parse_state = 0; // 0 -> initial, 1 -> v group found, 2-> not in v group, 3 -> group found (next part)

	size_t part_begin = 0;
	size_t part_end = 0;

	ret_obj.clear();
	ret_name.clear();

	*num_v = *num_uv = *num_n = 0;

	size_t prev_pos = *pos;

	while (EMG::GetNextObjLine(content, pos, line))
	{
		if (line.length() < 3)
		{
			prev_pos = *pos;
			continue;
		}

		if (line[0] == 'v' && line[1] > 0 && line[1] <= ' ')
		{
			if (parse_state == 0)
			{
				parse_state = 1;
				*num_v = 1;
				part_begin = prev_pos;
			}
			else if (parse_state == 1)
			{
				*num_v = *num_v + 1;
			}
			else if (parse_state == 2)
			{
				parse_state = 3;

				if (part_end == 0)
				{
					part_end = prev_pos;
				}

				if (!name_set)
				{
					if (show_error)
					{
						LOG_DEBUG("%s: Next group found but part name had not been set, "
							"I can't understand this .obj, faulting line: \"%s\"\n", FUNCNAME, line.c_str());
						LibXenoverse::notifyError();
					}

					*pos = std::string::npos;
					return (size_t)-1;
				}

				ret_obj = content.substr(part_begin, part_end - part_begin);
				*pos = part_end;
				return *num_v;
			}
		}
		else
		{
			if (parse_state == 1)
			{
				parse_state = 2;
			}
		}

		if (parse_state == 2)
		{
			if (line[0] == 'v' && line[1] == 't' && line[2] > 0 && line[2] <= ' ')
			{
				*num_uv = *num_uv + 1;
			}
			else if (line[0] == 'v' && line[1] == 'n' && line[2] > 0 && line[2] <= ' ')
			{
				*num_n = *num_n + 1;
			}
		}

		if (line[0] == 'g' && line[1] > 0 && line[1] <= ' ')
		{
			if (name_set)
			{
				if (parse_state != 2 || part_end != 0)
				{
					if (show_error)
					{
						LOG_DEBUG("Part name had previously been set, I can't understand this .obj, faulting line: \"%s\"\n", line.c_str());
						LibXenoverse::notifyError();
					}

					*pos = std::string::npos;
					return (size_t)-1;
				}

				part_end = prev_pos;
			}
			else
			{
				size_t name_start = 2;

				while (name_start < line.length())
				{
					if (line[name_start] > 0 && line[name_start] <= ' ')
					{
						name_start++;
					}
					else
					{
						break;
					}
				}

				if (name_start == line.length())
				{
					if (show_error)
					{
						LOG_DEBUG("Cannot get name and cannot understand this .obj, faulting line: \"%s\"\n", line.c_str());
						LibXenoverse::notifyError();
					}

					*pos = std::string::npos;
					return (size_t)-1;
				}

				ret_name = line.substr(name_start);
				name_set = true;
			}
		}

		prev_pos = *pos;
	}

	*pos = std::string::npos; // Redundant

	if (name_set && parse_state == 2)
	{
		// Last part
		ret_obj = content.substr(part_begin);
		return *num_v;
	}

	return 0;
}
bool EMO::RemoveSubPart(const std::string &subpart_name)
{
	uint16_t group_idx, part_idx, subpart_idx;

	EMG_SubPart *subpart = GetSubPart(subpart_name);
	if (!subpart)
		return false;

	bool ret = SubPartPtrToIndex(subpart, &group_idx, &part_idx, &subpart_idx);
	assert(ret);

	groups[group_idx][part_idx].subparts.erase(groups[group_idx][part_idx].begin() + subpart_idx);
	return ret;
}






size_t EMO::GetEmmMaterials(std::vector<std::string> &list, bool clear_vector, bool unique) const
{
	size_t count = 0;

	if (clear_vector)
		list.clear();

	for (const EMO_PartsGroup &pg : groups)
	{
		count += pg.GetEmmMaterials(list, false, unique);
	}

	return count;
}
size_t EMO::ReplaceEmmMaterial(const std::string &old_mat, const std::string &new_mat)
{
	if (new_mat.length() > EMG_SubMesh::MAX_EMM_MAT_LENGTH)
		return (size_t)-1;

	size_t count = 0;

	for (EMO_PartsGroup &pg : groups)
	{
		count += pg.ReplaceEmmMaterial(old_mat, new_mat);
	}

	return count;
}

size_t EMO::GetEmbIndexes(std::vector<uint8_t> &list, bool clear_vector, bool unique, bool sort) const
{
	size_t count = 0;

	if (clear_vector)
		list.clear();

	for (const EMO_PartsGroup &pg : groups)
	{
		count += pg.GetEmbIndexes(list, false, unique);
	}

	if (sort)
		std::sort(list.begin(), list.end());

	return count;
}
size_t EMO::ReplaceEmbIndex(uint8_t old_index, uint8_t new_index)
{
	size_t count = 0;

	for (EMO_PartsGroup &pg : groups)
	{
		count += pg.ReplaceEmbIndex(old_index, new_index);
	}

	return count;
}








bool EMO::Load(const uint8_t *buf, unsigned int size)
{
	Reset();

	EMO_Header *hdr = (EMO_Header *)buf;
	if (size < sizeof(EMO_Header) || hdr->signature != EMO_SIGNATURE)
		return false;

	this->big_endian = (buf[4] != 0xFE);

	if (!EMO_Skeleton::Load(GetOffsetPtr(buf, hdr->skeleton_offset), size - val32(hdr->skeleton_offset)))
		return false;

	for (EMO_Bone &b : this->bones)
		b.meta_original_offset += val32(hdr->skeleton_offset);

	this->unk_08[0] = val16(hdr->unk_08[0]);
	this->unk_08[1] = val16(hdr->unk_08[1]);
	this->unk_18[0] = val32(hdr->unk_18[0]);
	this->unk_18[1] = val32(hdr->unk_18[1]);

	EMO_PartsGroupHeader *pghdr = (EMO_PartsGroupHeader *)GetOffsetPtr(buf, hdr->parts_offset);
	this->material_count = val16(pghdr->material_count);

	uint32_t *names_table = (uint32_t *)GetOffsetPtr(pghdr, pghdr->names_offset);

	for (uint16_t i = 0; i < val16(pghdr->groups_count); i++)
	{
		EMO_PartsGroup group;

		group.name = std::string((char *)GetOffsetPtr(pghdr, names_table, i));

		EMO_PartHeader *phdr = (EMO_PartHeader *)GetOffsetPtr(pghdr, pghdr->offsets, i);
		group.meta_original_offset = EMO_BaseFile::DifPointer(phdr, buf);

		if (phdr->unk_02 != 0)
		{
			LOG_DEBUG("%s: unk_02 not 0 as expected, in group %s\n", FUNCNAME, group.name.c_str());
			return false;
		}

		for (uint16_t j = 0; j < val16(phdr->emg_count); j++)
		{
			EMG part(big_endian);
			char meta_name[2048];

			snprintf(meta_name, sizeof(meta_name), "%s_%04x", group.name.c_str(), j);
			part.meta_name = meta_name;

			if (phdr->offsets[j] == 0)
			{
				group.parts.push_back(part);
				continue;
			}

			uint8_t *emg_buf = GetOffsetPtr(phdr, phdr->offsets, j);

			if (!part.Load(emg_buf, EMO_BaseFile::DifPointer(buf + size, emg_buf), this))
			{
				LOG_DEBUG("%s: Bad file, #EMG sub file couldn't be loaded (in group %s, part %04x)\n", FUNCNAME, group.name.c_str(), j);
				return false;
			}

			group.parts.push_back(part);
		}

		this->groups.push_back(group);
	}

	return true;
}

uint8_t *EMO::CreateFile(unsigned int *psize)
{
	unsigned int file_size;
	uint32_t offset, vertex_start;
	bool first = true;

	// We need to reorder parts first: but only when there are empty parts before non-empty parts!
	for (EMO_PartsGroup &pg : groups)
	{
		// Ignore these...
		//if (pg.name == "face" || pg.name == "edge")
		//	continue;

		uint16_t first_empty = 0xFFFF;

		for (size_t i = 0; i < pg.parts.size(); i++)
		{
			if (first_empty == 0xFFFF && pg.parts[i].IsEmpty())
			{
				first_empty = i;
				break;
			}
		}

		if (first_empty != 0xFFFF)
		{
			bool do_sort = false;

			for (size_t i = 0; i < pg.parts.size(); i++)
			{
				if (!pg.parts[i].IsEmpty() && i > first_empty)
				{
					do_sort = true;
					break;
				}
			}

			//printf("do_sort = %d\n", do_sort);

			if (do_sort)
			{
				std::sort(pg.parts.begin(), pg.parts.end(), EMO_PartsSorter(pg));
			}
		}
	}

	file_size = CalculateFileSize(&vertex_start);

	uint8_t *buf = new uint8_t[file_size];
	if (!buf)
	{
		LOG_DEBUG("%s: Memory allocation error (0x%x)\n", FUNCNAME, file_size);
		LibXenoverse::notifyError();
		return nullptr;
	}

	memset(buf, 0, file_size);

	EMO_Header *hdr = (EMO_Header *)buf;

	hdr->signature = EMO_SIGNATURE;
	hdr->endianess_check = val16(0xFFFE);
	hdr->header_size = val16(sizeof(EMO_Header));
	hdr->unk_08[0] = val16(this->unk_08[0]);
	hdr->unk_08[1] = val16(this->unk_08[1]);
	hdr->unk_18[0] = val32(this->unk_18[0]);
	hdr->unk_18[1] = val32(this->unk_18[1]);

	offset = sizeof(EMO_Header);
	hdr->parts_offset = val32(offset);

	EMO_PartsGroupHeader *pghdr = (EMO_PartsGroupHeader *)GetOffsetPtr(buf, offset, true);

	assert(groups.size() < 65536);
	pghdr->groups_count = val16(groups.size());
	pghdr->material_count = val16(this->material_count);

	offset += sizeof(EMO_PartsGroupHeader);

	if (groups.size() != 0)
		offset += (groups.size() - 1) * sizeof(uint32_t);

	for (size_t i = 0; i < groups.size(); i++)
	{
		if (offset & 0xF)
			offset += (0x10 - (offset & 0xF));

		pghdr->offsets[i] = val32(offset - sizeof(EMO_Header));

		EMO_PartHeader *phdr = (EMO_PartHeader *)GetOffsetPtr(buf, offset, true);
		EMO_PartsGroup &pg = groups[i];

		assert(pg.parts.size() < 65536);

		phdr->emg_count = val16(pg.parts.size());

		offset += sizeof(EMO_PartHeader);

		if (pg.parts.size() != 0)
		{
			offset += (pg.parts.size() - 1) * sizeof(uint32_t);
		}

		for (size_t j = 0; j < pg.parts.size(); j++)
		{
			EMG &part = pg.parts[j];

			part.big_endian = big_endian;

			if (part.IsEmpty())
			{
				phdr->offsets[j] = 0;
				continue;
			}

			if (offset & 0xF)
				offset += (0x10 - (offset & 0xF));

			phdr->offsets[j] = val32(EMO_BaseFile::DifPointer(buf + offset, phdr));

			if (first)
			{
				first = false;
			}
			else
			{
				if (vertex_start & 0xf)
					vertex_start += (0x10 - (vertex_start & 0xF));
			}

			uint32_t vertex_start_local1, vertex_start_local2;
			vertex_start_local1 = vertex_start_local2 = vertex_start - offset;

			offset += part.CreatePart(buf + offset, this, &vertex_start_local2);

			vertex_start += (vertex_start_local2 - vertex_start_local1);
		}
	}

	if (offset & 0xF)
	{
		offset += (0x10 - (offset & 0xF));
	}

	pghdr->names_offset = val32(offset - sizeof(EMO_Header));

	uint32_t *names_table = (uint32_t *)GetOffsetPtr(buf, offset, true);
	offset += groups.size() * sizeof(uint32_t);

	std::vector<string> listEMG;
	for (size_t i = 0; i < groups.size(); i++)
	{
		EMO_PartsGroup &pg = groups[i];

		names_table[i] = val32(offset - sizeof(EMO_Header));

		strcpy((char *)buf + offset, pg.name.c_str());
		offset += pg.name.length() + 1;
		
		listEMG.push_back(pg.name);
	}

	if (offset & 0x3F)
		offset += (0x40 - (offset & 0x3F));

	unsigned int skl_size;
	uint8_t *skl = EMO_Skeleton::CreateFile(&skl_size, listEMG);
	if (!skl)
	{
		delete[] buf;
		return nullptr;
	}

	hdr->skeleton_offset = val32(offset);
	memcpy(buf + offset, skl, skl_size);
	delete[] skl;

	offset += skl_size;

	hdr->vertex_offset = val32(offset);
	first = true;

	for (EMO_PartsGroup &pg : groups)
	{
		for (EMG &p : pg.parts)
		{
			if (first)
			{
				first = false;
			}
			else
			{
				if (offset & 0xf)
					offset += (0x10 - (offset & 0xF));
			}

			offset += p.CreateVertex(buf + offset);
		}
	}

	assert(offset == file_size);

	*psize = file_size;
	return buf;
}









unsigned int EMO::CalculateFileSize(uint32_t *vertex_start)
{
	unsigned int file_size;

	file_size = sizeof(EMO_Header);
	file_size += sizeof(EMO_PartsGroupHeader);

	if (groups.size() != 0)
		file_size += (groups.size() - 1) * sizeof(uint32_t);

	for (EMO_PartsGroup &pg : groups)
	{
		if (file_size & 0xF)
			file_size += (0x10 - (file_size & 0xF));

		//LOG_DEBUG("file size before part %s: %x\n", pg.name.c_str(), file_size);

		file_size += sizeof(EMO_PartHeader);
		if (pg.parts.size() != 0)
		{
			file_size += (pg.parts.size() - 1) * sizeof(uint32_t);
		}

		for (EMG &p : pg.parts)
		{
			if (p.IsEmpty())
				continue;

			if (file_size & 0xF)
				file_size += (0x10 - (file_size & 0xF));

			file_size += p.CalculatePartSize();
		}
	}

	if (file_size & 0xF)
	{
		file_size += (0x10 - (file_size & 0xF));
	}

	//LOG_DEBUG("file_size before names table %x\n", file_size);

	file_size += groups.size() * sizeof(uint32_t);
	for (EMO_PartsGroup &pg : groups)
	{
		file_size += (pg.name.length() + 1);
	}

	if (file_size & 0x3F)
		file_size += (0x40 - (file_size & 0x3F));


	//LOG_DEBUG("file size before skeleton: %x\n", file_size);

	file_size += EMO_Skeleton::CalculateFileSize();

	*vertex_start = file_size;
	bool first = true;

	for (EMO_PartsGroup &pg : groups)
	{
		for (EMG &p : pg.parts)
		{
			if (first)
			{
				first = false;
			}
			else
			{
				if (file_size & 0xf)
					file_size += (0x10 - (file_size & 0xF));
			}

			file_size += p.CalculateVertexSize();
		}
	}

	//LOG_DEBUG("File size = %x\n", file_size);
	return file_size;
}







size_t EMO::ExportObj(std::string *vertex_out, std::string *uvmap_out, std::string *normal_out, std::string *topology_out, size_t v_start_idx, bool write_group) const
{
	if (!vertex_out && !uvmap_out && !normal_out && !topology_out)
		return 0;

	size_t count = 0;
	std::string ret_v, ret_uv, ret_n, ret_t;

	for (const EMO_PartsGroup &pg : groups)
	{
		std::string v, uv, n, t;

		size_t ret = pg.ExportObj(&v, &uv, &n, &t, v_start_idx, write_group);

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
size_t EMO::InjectObj(const std::string &obj, bool do_uv, bool do_normal, bool show_error)
{
	std::vector<EMG_VertexCommon> vertex;
	size_t vtx_count = 0, uv_count = 0, n_count = 0;

	size_t total_vertex = GetNumVertex();
	size_t count = 0;

	vertex.reserve(total_vertex);

	if (!EMG::ReadObj(obj, vertex, do_uv, do_normal, 0, 0, 0, &vtx_count, &uv_count, &n_count, show_error))
		return (size_t)-1;

	if (vertex.size() != total_vertex)
	{
		if (show_error)
		{
			LOG_DEBUG("Number of vertex in obj is different to the one used in the .emo, cannot proceed "
				"(obj=%u, emo=%u)\n", vertex.size(), total_vertex);
			LibXenoverse::notifyError();
		}

		return (size_t)-1;
	}

	std::vector<EMO_PartsGroup> temp_groups = groups;
	size_t current_v = 0;

	for (EMO_PartsGroup &pg : temp_groups)
	{
		for (EMG &p : pg.parts)
		{
			for (EMG_SubPart &sp : p.subparts)
			{
				std::vector<EMG_VertexCommon> this_sp_vertex(&vertex[current_v], &vertex[current_v + sp.vertex.size()]);

				if (!sp.InjectVertex(this_sp_vertex, true, (uv_count != 0), (n_count != 0)))
					return false;

				count++;
				current_v += sp.vertex.size();
			}
		}
	}

	groups = temp_groups;
	return count;
}


size_t EMO::InjectObjBySubParts(const std::string &obj, bool do_uv, bool do_normal, bool show_error)
{
	int ret;
	size_t pos = 0;
	size_t count = 0;

	std::string next_obj;
	std::string subpart_name;

	size_t num_v, num_uv, num_n;
	size_t v_start, uv_start, n_start;

	v_start = uv_start = n_start = 0;

	std::vector<EMO_PartsGroup> backup = groups;

	while ((ret = (int)GetNextObjSubPart(obj, &pos, next_obj, subpart_name, &num_v, &num_uv, &num_n)) > 0)
	{
		EMG_SubPart *sp = GetSubPart(subpart_name);
		if (!sp)
		{
			if (show_error)
			{
				LOG_DEBUG(".obj specifies subpart \"%s\", but it doesn't exist on this .emo.\n", subpart_name.c_str());
				LibXenoverse::notifyError();
			}

			if (count != 0)
				groups = backup;

			return (size_t)-1;
		}

		if (!sp->InjectObj(next_obj, do_uv, do_normal, v_start, uv_start, n_start, show_error))
		{
			if (count != 0)
				groups = backup;

			return (size_t)-1;
		}

		v_start += num_v;
		uv_start += num_uv;
		n_start += num_n;
		count++;
	}

	if (ret < 0)
	{
		if (show_error)
		{
			LOG_DEBUG("%s: unknown error.\n", FUNCNAME);
			LibXenoverse::notifyError();
		}

		if (count != 0)
			groups = backup;

		return (size_t)-1;
	}

	return count;
}




//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


/*-------------------------------------------------------------------------------\
|                             readEmd				                             |
\-------------------------------------------------------------------------------*/
void EMO::readEmd(EMD* emd)
{
	name = emd->name;

	this->material_count = 0;

	size_t nbSection = 1;

	size_t nbElements = emd->models.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		groups.push_back(EMO_PartsGroup());
		groups.back().readEmdModel( emd->models.at(i), this, nbSection);
	}
}
/*-------------------------------------------------------------------------------\
|                             writeEmd				                             |
\-------------------------------------------------------------------------------*/
void EMO::writeEmd(EMD* emd)
{
	emd->name = name;

	size_t nbElements = groups.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		emd->models.push_back(new EMDModel());
		groups.at(i).writeEmdModel(emd->models.back());
	}
}
/*-------------------------------------------------------------------------------\
|                             readEsk				                             |
\-------------------------------------------------------------------------------*/
void EMO::readEsk(ESK* esk)
{
	name = esk->name;
	unk_38[0] = *(float*)(&(esk->unknown_offset_2));
	unk_38[1] = *(float*)(&(esk->unknown_offset_3));
	

	EskTreeNode* rootNode = esk->getTreeOrganisation();

	vector<EskTreeNode*> newListBonesOrdered;
	rootNode->getTreeBonesList(newListBonesOrdered);					//we get back the list of bones from follow hierarchy, to not having trouble for missing parents.

	bones.clear();

	EskTreeNode* node;
	size_t nbBones = newListBonesOrdered.size();
	for (size_t i = 1; i < nbBones; i++)					//we begin to 1 because the first is not a bone.
	{
		node = newListBonesOrdered.at(i);

		bones.push_back(EMO_Bone());
		bones.back().readESKBone(node->mBone);
	}


	for (size_t i = 1; i < nbBones; i++)					//same
	{
		node = newListBonesOrdered.at(i);

		if ((node->mParent) && (node->mParent->mBone))
		{
			string parentName = node->mParent->mBone->getName();
			for (size_t j = i - 1; j < i; --j)
			{
				if (bones.at(j).GetName() == parentName)
				{
					bones.at(i-1).parent = &(bones.at(j));

					if (bones.at(j).child == nullptr)
					{
						bones.at(j).child = &bones.at(i - 1);
					}else {

						size_t inc = 0;
						EMO_Bone* childBone = bones.at(j).child;

						while (childBone->sibling != nullptr)
						{
							childBone = childBone->sibling;
							++inc;
						}
						++inc;

						childBone->sibling = &bones.at(i - 1);
					}

					break;
				}
			}
		}
	}





	//Ik datas
	listInverseKinematic.clear();
	size_t nbBones_all = bones.size();
	size_t nbGroup = esk->listInverseKinematic.size();
	for (size_t i = 0; i < nbGroup; i++)
	{
		Esk_IK_Group &group = esk->listInverseKinematic.at(i);
		listInverseKinematic.push_back(Emo_IK_Group());

		size_t nbIk = group.mListIK.size();
		for (size_t j = 0; j < nbIk; j++)
		{
			Esk_IK_Relation &ik = group.mListIK.at(j);
			listInverseKinematic.back().mListIK.push_back(Emo_IK_Relation());
			Emo_IK_Relation &ik_b = listInverseKinematic.back().mListIK.back();

			size_t nbBones = ik.mListBones.size();
			for (size_t k = 0; k < nbBones; k++)
			{
				ESKBone* bone = ik.mListBones.at(k).bone;
				EMO_Bone* bone_b = 0;

				for (size_t m = 0; m < nbBones_all; m++)
				{
					if (bones.at(m).name == bone->getName())
					{
						bone_b = &bones.at(m);
						break;
					}
				}
				if (!bone_b)
					continue;

				ik_b.mListBones.push_back(Emo_IK_Relation::IKR_Bone(bone_b, ik.mListBones.at(k).value));
			}
		}
	}
}





void EMO::RebuildSkeleton(const std::vector<EMO_Bone *> &old_bones_ptr)
{
    EMO_Skeleton::RebuildSkeleton(old_bones_ptr);
    //LOG_DEBUG("Rebuild skeleton.\n");

    for (EMO_PartsGroup &pg : groups)
    {
        for (EMG &p : pg)
        {
            for (EMG_SubPart &sp : p)
            {
                for (EMG_SubMesh &sm : sp)
                {
                    for (EMO_Bone *&b : sm.linked_bones)
                    {
                        assert(b != nullptr);
                        uint16_t id = FindBone(old_bones_ptr, b, false);

                        if (id != 0xFFFF)
                        {
                            b = &bones[id];
                        }
                        else
                        {
                            // In this case, this is a bone from other emo that hasn't still been resolved
                            //LOG_DEBUG("Not resolved yet: %s\n", b->GetName().c_str());
                        }
                    }
                }
            }
        }
    }
}








size_t EMO::CloneLinkedBones(const EMO &other, EMO_PartsGroup &group, EMO_Bone **not_found)
{
    size_t count = 0;

    for (EMG &p : group.parts)
    {
        for (EMG_SubPart &sp : p.subparts)
        {
            for (EMG_SubMesh &sm : sp.submeshes)
            {
                // First check that all bone exists
                for (EMO_Bone *b : sm.linked_bones)
                {
                    assert(b != nullptr);

                    if (!other.BoneExists(b->name))
                    {
						LibXenoverse::notifyError();

                        // This is a critical error
                        if (not_found)
                            *not_found = b;

                        return (size_t)-1;
                    }

                    if (!BoneExists(b->name))
                    {
                        if (not_found)
                            *not_found = b;

                        return (size_t)-1;
                    }
                }

                // Now point the data to the local bones
                for (EMO_Bone *&b : sm.linked_bones)
                {
                    b = GetBone(b->name);
                    count++;
                }
            }
        }
    }

    return count;
}





















#ifdef FBX_SUPPORT




void EMO::oldSaveFbx(string filename)	//test because of saintSeya work well with old emoTools fbx convertion
{
	string output = filename;

	FbxManager *sdk_manager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(sdk_manager, IOSROOT);
	sdk_manager->SetIOSettings(ios);

	FbxScene *scene = FbxScene::Create(sdk_manager, "EMOTOOL3");

	if (!ExportFbx(scene, true, false))
	{
		printf("ExportFbx failed.\n");
		return;
	}

	bool ascii = false;
	bool y_up = true;
	bool inches = false;

	//static bool export_fbx_scene(FbxManager *sdk_manager, FbxScene *scene, const std::string &output, const FbxString &version, bool ascii, bool y_up, bool inches)
	{
		int format = sdk_manager->GetIOPluginRegistry()->GetNativeWriterFormat();

		if (ascii)
		{
			int count = sdk_manager->GetIOPluginRegistry()->GetWriterFormatCount();

			for (int i = 0; i < count; i++)
			{
				if (sdk_manager->GetIOPluginRegistry()->WriterIsFBX(i))
				{
					FbxString desc = sdk_manager->GetIOPluginRegistry()->GetWriterFormatDescription(i);
					if (desc.Find("ascii") >= 0)
					{
						format = i;
						break;
					}
				}
			}
		}

		FbxExporter* exporter = FbxExporter::Create(sdk_manager, "");
		bool ret = exporter->Initialize(output.c_str(), format, sdk_manager->GetIOSettings());

		if (!ret)
		{
			printf("FbxExporter::Initialize error:: %s\n\n", exporter->GetStatus().GetErrorString());
			LibXenoverse::notifyError();
			return;
		}

		if (!y_up)
			scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::eMax);

		if (inches)
			scene->GetGlobalSettings().SetSystemUnit(FbxSystemUnit::Inch);
		else
			scene->GetGlobalSettings().SetSystemUnit(FbxSystemUnit::m);

		//exporter->SetFileExportVersion(version);
		exporter->Export(scene);
		exporter->Destroy();
	}


	/*
	string output_dir = output.substr(0, output.length() - 4) + "_exported";
	LibXenoverse::EMO_BaseFile::CreatePath(output_dir, true);

	for (const LibXenoverse::EMO_PartsGroup &pg : *emo)
	{
		for (const LibXenoverse::EMG &p : pg)
		{
			for (const LibXenoverse::EMG_SubPart &sp : p)
			{
				if (sp.IsEdge() && !true)
					continue;

				if (!sp.IsEdge() && !true)
					continue;

				printf("Exporting subpart \"%s\"\n", sp.GetMetaName().c_str());

				scene = FbxScene::Create(sdk_manager, "EMOTOOL3");
				output = output_dir + "/" + sp.GetMetaName();

				if (!emo->ExportFbx(sp.GetMetaName(), scene))
				{
					printf("ExportFbx failed on subpart \"%s\"\n", sp.GetMetaName().c_str());
					return;
				}

				if (!export_fbx_scene(sdk_manager, scene, output, version, ascii, y_up, inches))
					return;
			}
		}
	}
	*/

	sdk_manager->Destroy();
}



bool EMO::ExportFbx(FbxScene *scene, bool normal_parts, bool edges) const
{
    std::vector<FbxNode *> fbx_bones;

    if (!EMO_Skeleton::ExportFbx(scene, fbx_bones))
        return false;

    for (const EMO_PartsGroup &pg : groups)
    {
        if (!edges && pg.IsEdge())
            continue;

        if (!normal_parts && !pg.IsEdge())
            continue;

        if (!pg.ExportFbx(*this, fbx_bones, scene))
            return false;
    }

    return true;
}


bool EMO::ExportFbx(const std::string &subpart, FbxScene *scene)
{
	EMG_SubPart *sp = GetSubPart(subpart);
	if (!sp)
		return false;

	std::vector<FbxNode *> fbx_bones;

	if (!EMO_Skeleton::ExportFbx(scene, fbx_bones))
		return false;

	return sp->ExportFbx(*this, fbx_bones, scene);
}


bool EMO::InjectFbx(FbxScene *scene, bool normal_parts, bool edges, bool use_fbx_tangent)
{
    for (EMO_PartsGroup &pg : groups)
    {
        if (!edges && pg.IsEdge())
            continue;

        if (!normal_parts && !pg.IsEdge())
            continue;

        if (!pg.InjectFbx(*this, scene, use_fbx_tangent))
            return false;
    }

    for (size_t i = 0; i < groups.size(); i++)
    {
        if (groups[i].parts.size() == 0 || groups[i].parts[0].IsEmpty())
        {
            LOG_DEBUG("Parts group \"%s\" has become empty. We will delete it.\n", groups[i].name.c_str());
            groups.erase(groups.begin()+i);
            i--;
        }
    }

    return true;
}

bool EMO::InjectFbx(const std::string &subpart, FbxScene *scene, bool use_fbx_tangent)
{
    EMG_SubPart *sp = GetSubPart(subpart);
    if (!sp)
        return false;

    return sp->InjectFbx(*this, scene, use_fbx_tangent);
}

#endif

bool EMO::operator==(const EMO &rhs) const
{
    if (this->material_count != rhs.material_count)
        return false;

    if (memcmp(this->unk_08, rhs.unk_08, sizeof(this->unk_08)) != 0)
        return false;

    if (memcmp(this->unk_18, rhs.unk_18, sizeof(this->unk_18)) != 0)
        return false;

    if (this->groups != rhs.groups)
        return false;

    if (EMO_Skeleton::operator ==(rhs) != true)
        return false;

    return true;
}




/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// load/save the Xml version	/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////





/*-------------------------------------------------------------------------------\
|                             DecompileToFile		                             |
\-------------------------------------------------------------------------------*/
bool EMO::DecompileToFile(const std::string &path, bool show_error, bool build_path)	//save
{
	LOG_DEBUG("Emo is being saved to .xml file. This process may take several seconds.\n");
	bool ret = EMO_BaseFile::DecompileToFile(path, show_error, build_path);

	if (ret)
	{
		LOG_DEBUG("Emo has been saved to .xml.\n");
	}

	return ret;
}

/*-------------------------------------------------------------------------------\
|                             Decompile				                             |
\-------------------------------------------------------------------------------*/
TiXmlDocument *EMO::Decompile() const
{
	TiXmlDocument *doc = new TiXmlDocument();

	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "utf-8", "");
	doc->LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("EMO");

	EMO_BaseFile::WriteParamUnsigned(root, "MATERIAL_COUNT", material_count);
	EMO_BaseFile::WriteParamMultipleUnsigned(root, "U_08", std::vector<uint16_t>(unk_08, unk_08 + 2), true);
	EMO_BaseFile::WriteParamMultipleUnsigned(root, "U_18", std::vector<uint32_t>(unk_18, unk_18 + 2), true);

	for (const EMO_PartsGroup &pg : groups)
		pg.Decompile(root);

	doc->LinkEndChild(root);
	EMO_Skeleton::Decompile(doc);

	return doc;
}

/*-------------------------------------------------------------------------------\
|                             Decompile				                             |
\-------------------------------------------------------------------------------*/
void EMO_PartsGroup::Decompile(TiXmlNode *root) const
{
	TiXmlElement *entry_root = new TiXmlElement("PartsGroup");

	entry_root->SetAttribute("name", name);

	for (size_t i = 0; i < parts.size(); i++)
		parts.at(i).Decompile(entry_root, i);

	root->LinkEndChild(entry_root);
}







/*-------------------------------------------------------------------------------\
|                             CompileFromFile		                             |
\-------------------------------------------------------------------------------*/
bool EMO::CompileFromFile(const std::string &path, bool show_error, bool big_endian)		//load
{
	LOG_DEBUG("Emo is being loaded from .xml file. This process may take several seconds.\n");
	bool ret = EMO_BaseFile::CompileFromFile(path, show_error, big_endian);

	if (ret)
	{
		LOG_DEBUG("Emo has been loaded from .xml.\n");
	}

	return ret;
}

/*-------------------------------------------------------------------------------\
|                             Compile				                             |
\-------------------------------------------------------------------------------*/
bool EMO::Compile(TiXmlDocument *doc, bool big_endian)
{
	Reset();
	this->big_endian = big_endian;

	if (!EMO_Skeleton::Compile(doc, big_endian))
		return false;

	TiXmlHandle handle(doc);
	const TiXmlElement *root = EMO_BaseFile::FindRoot(&handle, "EMO");

	if (!root)
	{
		LOG_DEBUG("Cannot find\"EMO\" in xml.\n");
		return false;
	}

	unsigned int value;
	if (!EMO_BaseFile::GetParamUnsigned(root, "MATERIAL_COUNT", &value))
		value = 0;

	if (value > 0xFFFF)
	{
		LOG_DEBUG("%s: material_count must be a 16 bits value.\n", FUNCNAME);
		return false;
	}
	material_count = value;


	std::vector<uint16_t> unk_08;
	EMO_BaseFile::GetParamMultipleUnsigned(root, "U_08", unk_08);

	if (unk_08.size() != 2)
	{
		LOG_DEBUG("%s: Invalid size of U_08. must be 2.\n", FUNCNAME);
		
		for (size_t i = unk_08.size(); i < 2; i++)			// get the minimum
			unk_08.push_back(0);
	}

	std::vector<uint32_t> unk_18;
	EMO_BaseFile::GetParamMultipleUnsigned(root, "U_18", unk_18);

	if (unk_18.size() != 2)
	{
		LOG_DEBUG("%s: Invalid size of U_18. must be 2.\n", FUNCNAME);
		
		for (size_t i = unk_18.size(); i < 2; i++)			// get the minimum
			unk_08.push_back(0);
	}


	for (int i = 0; i < 2; i++)
	{
		this->unk_08[i] = unk_08.at(i);
		this->unk_18[i] = unk_18.at(i);
	}


	for (const TiXmlElement *elem = root->FirstChildElement("PartsGroup"); elem; elem = elem->NextSiblingElement("PartsGroup"))
	{
		EMO_PartsGroup pg;

		if (!pg.Compile(elem, this))
		{
			LOG_DEBUG("%s: PartsGroup compilation failed.\n", FUNCNAME);
			continue;
		}

		groups.push_back(pg);
	}

	return true;
}


/*-------------------------------------------------------------------------------\
|                             Compile				                             |
\-------------------------------------------------------------------------------*/
bool EMO_PartsGroup::Compile(const TiXmlElement *root, EMO_Skeleton *skl)
{
	parts.clear();


	if (root->QueryStringAttribute("name", &name) != TIXML_SUCCESS)
	{
		LOG_DEBUG("%s: Cannot get name of PartsGroup\n", FUNCNAME);
		return false;
	}

	meta_original_offset = 0xFFFFFFFF;

	
	for (const TiXmlElement *elem = root->FirstChildElement("EMG"); elem; elem = elem->NextSiblingElement("EMG"))
	{
		EMG emg;

		if (!emg.Compile(elem, skl))
		{
			LOG_DEBUG("%s: Compilation of EMG failed.\n", FUNCNAME);
			continue;
		}

		char meta_name[2048];
		snprintf(meta_name, sizeof(meta_name), "%s_%04x", name.c_str(), parts.size());
		emg.meta_name = meta_name;				//may be for unique name.

		parts.push_back(emg);
	}

	return true;
}










}
