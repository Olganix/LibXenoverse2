
/*
	From EmoTool3_0.042 
	Authors : Eternity
	http://animegamemods.freeforums.net/thread/31/tools-saint-seiya-soldier-soul

	Emo file is for stage and skill.
	could have skeleton definition and also have inside some emg file whitch is meshes.
*/

#ifndef __EMOFILE_H__
#define __EMOFILE_H__

#include <stdint.h>
#include "EMO_Skeleton.h"
#include "EMG.h"
#include "EMM.h"


namespace LibXenoverse
{

// "#EMO"
#define EMO_SIGNATURE	0x4F4D4523

#ifdef _MSC_VER
#pragma pack(push,1)
#endif

struct EMO_Header
{
	uint32_t signature; 		// 0
	uint16_t endianess_check;	// 4
	uint16_t header_size;		// 6
	uint16_t unk_08[2]; 		// 8
	uint32_t parts_offset; 		// 0x0C
	uint32_t skeleton_offset;	// 0x10
	uint32_t vertex_offset;		// 0x14
	uint32_t unk_18[2];			// 0x18
	// 0x20
} PACKED;

static_assert(sizeof(EMO_Header) == 0x20, "Incorrect structure size.");

struct EMO_PartsGroupHeader
{
	uint16_t groups_count;	// 0
	uint16_t material_count;// 2
	uint32_t names_offset;	// 4
	uint32_t offsets[1];	// 8
	// 0xC Remaining offsets
} PACKED;
static_assert(sizeof(EMO_PartsGroupHeader) == 0xC, "Incorrect structure size.");

struct EMO_PartHeader
{
	uint16_t emg_count; // 0
	uint16_t unk_02; // Let's assume padding
	uint32_t offsets[1]; // 4
	// 0x8 Remaining offsets
} PACKED;
static_assert(sizeof(EMO_PartHeader) == 8, "Incorrect structure size.");

#ifdef _MSC_VER
#pragma pack(pop)
#endif




/********************************************************************************************
*								EMO_PartsGroup												*
********************************************************************************************/
class EMO_PartsGroup
{
	friend class EMO;

private:
	std::string name;
	std::vector<EMG> parts;

	// METADATA - DON'T USE IN COMPARISON
	// Only makes sense after loading, before any modification. It only exists to display original file info
	uint32_t meta_original_offset;




public:
	inline bool operator==(const EMO_PartsGroup &rhs) const { return (this->name == rhs.name && this->parts == rhs.parts); }
	inline bool operator!=(const EMO_PartsGroup &rhs) const { return !(*this == rhs); }
	inline EMG &operator[](size_t n) { return parts[n]; }
	inline const EMG &operator[](size_t n) const { return parts[n]; }
	inline std::vector<EMG>::iterator begin() { return parts.begin(); }
	inline std::vector<EMG>::iterator end() { return parts.end(); }

	inline void SetName(const std::string &name) { this->name = name; }
	inline std::string GetName() const { return name; }
	inline uint32_t GetOriginalOffset() const { return meta_original_offset; }
	
	inline uint16_t GetNumParts() const { return parts.size(); }
	inline bool PartExists(uint16_t idx) { return (GetPart(idx) != nullptr); }
	inline EMG *GetPart(uint16_t idx) { return ((idx >= parts.size()) ? nullptr : &parts[idx]); }
	bool IsEdge() const;
	size_t GetNumVertex();


	size_t GetLinkedBones(std::vector<EMO_Bone*> &list, bool clear_vector, bool unique=true, const EMO_Skeleton *sorter=nullptr) const;

	size_t GetEmmMaterials(std::vector<std::string> &list, bool clear_vector, bool unique = true) const;
	size_t ReplaceEmmMaterial(const std::string &old_mat, const std::string &new_mat);

	size_t GetEmbIndexes(std::vector<uint8_t> &list, bool clear_vector, bool unique = true, bool sort = false) const;
	size_t ReplaceEmbIndex(uint8_t old_index, uint8_t new_index);



	size_t ExportObj(std::string *vertex_out, std::string *uvmap_out, std::string *normal_out, std::string *topology_out, size_t v_start_idx = 0, bool write_group = false) const;
	void Decompile(TiXmlNode *root) const;
	bool Compile(const TiXmlElement *root, EMO_Skeleton *skl);


	void readEmdModel(EMDModel* emd, EMO* emo, size_t &nbSection);
	void writeEmdModel(EMDModel* emd);

#ifdef FBX_SUPPORT
	bool ExportFbx(const EMO_Skeleton &skl, const std::vector<FbxNode *> &fbx_bones, FbxScene *scene) const;
	bool InjectFbx(EMO_Skeleton &skl, FbxScene *scene, bool use_fbx_tangent);
#endif
};


/********************************************************************************************
*								EMO_PartsSorter												*
********************************************************************************************/
class EMO_PartsSorter
{
private:
	EMO_PartsGroup group;

public:
	EMO_PartsSorter(const EMO_PartsGroup &group)
	{
		this->group = group; // Note: we need a real copy, not reference, pointer, etc
	}

	bool operator()(const EMG &part1, const EMG &part2);

private:
	uint16_t GetIdx(const EMG &part);
};






/********************************************************************************************
*										EMO													*
********************************************************************************************/
class EMO : public EMO_Skeleton
{
	friend class EMG;

private:
	std::vector<EMO_PartsGroup> groups;
	uint16_t material_count;

	// From EMO_Header
	uint16_t unk_08[2];
	uint32_t unk_18[2];

public:
	EMO();
	EMO(uint8_t *buf, unsigned int size);
	EMO(const EMO &other) : EMO_Skeleton()
	{
		Copy(other);
	}
	virtual ~EMO();


	EMO &operator=(const EMO &other);
	bool operator==(const EMO &rhs) const;
	inline bool operator!=(const EMO &rhs) const { return !(*this == rhs); }
	inline EMO_PartsGroup &operator[](size_t n) { return groups[n]; }
	inline const EMO_PartsGroup &operator[](size_t n) const { return groups[n]; }
	EMO_PartsGroup &operator[](const std::string &group_name);
	const EMO operator+(const EMO_PartsGroup &group) const;
	EMO &operator+=(const EMO_PartsGroup &group);
	void Copy(const EMO &other);
	void Reset();


	inline std::vector<EMO_PartsGroup>::iterator begin() { return groups.begin(); }
	inline std::vector<EMO_PartsGroup>::iterator end() { return groups.end(); }
	inline std::vector<EMO_PartsGroup>::const_iterator begin() const { return groups.begin(); }
	inline std::vector<EMO_PartsGroup>::const_iterator end() const { return groups.end(); }


	//inline void SetMaterialCount(const EMM &emm) { material_count = emm.GetNumMaterials(); }	//TODO
	inline void SetMaterialCount(const EMM &emm) { material_count = 0; }			//test
	inline uint16_t GetMaterialCount() const { return material_count; }


	inline uint16_t GetNumGroups() const { return groups.size(); }
	uint16_t GroupPtrToIndex(const EMO_PartsGroup *group) const;
	inline bool GroupExists(uint16_t idx) { return (GetGroup(idx) != nullptr); }
	inline bool GroupExists(const std::string &name) { return (GetGroup(name) != nullptr); }
	EMO_PartsGroup *GetGroup(uint16_t idx) { return ((idx >= groups.size()) ? nullptr : &groups[idx]); }
	EMO_PartsGroup *GetGroup(const std::string & name);
	bool RemoveGroup(uint16_t idx);
	bool RemoveGroup(const std::string &name);
	uint16_t AppendGroup(const EMO_PartsGroup &group);
	uint16_t AppendGroup(EMO &other, const std::string &name);
	bool HasEdges() const;
	size_t GetNumVertex();


	inline bool PartExists(const std::string &part_name) { return (GetPart(part_name) != nullptr); }
	bool PartPtrToIndex(const EMG *part, uint16_t *group_idx, uint16_t *part_idx) const;
	EMG *GetPart(const std::string &part_name);
	bool RemovePart(const std::string &part_name);

	
	inline bool SubPartExists(const std::string &subpart_name) { return (GetSubPart(subpart_name) != nullptr); }
	bool SubPartPtrToIndex(const EMG_SubPart *subpart, uint16_t *group_idx, uint16_t *part_idx, uint16_t *subpart_idx) const;
	EMG_SubPart *GetSubPart(const std::string &subpart_name);
	size_t GetNextObjSubPart(const std::string &content, size_t *pos, std::string &ret_obj, std::string &ret_name, size_t *num_v, size_t *num_uv, size_t *num_n, bool show_error = true);
	bool RemoveSubPart(const std::string &subpart_name);




	inline size_t GetLinkedBones(const EMO_PartsGroup &group, std::vector<EMO_Bone*> &list, bool clear_vector, bool unique=true, bool sort=false) const { return (group.GetLinkedBones(list, clear_vector, unique, (sort) ? this : nullptr)); }
	size_t CloneLinkedBones(const EMO &other, EMO_PartsGroup &group, EMO_Bone **not_found = nullptr);

	size_t GetEmmMaterials(std::vector<std::string> &list, bool clear_vector, bool unique = true) const;
	size_t ReplaceEmmMaterial(const std::string &old_mat, const std::string &new_mat);

	size_t GetEmbIndexes(std::vector<uint8_t> &list, bool clear_vector, bool unique = true, bool sort = false) const;
	size_t ReplaceEmbIndex(uint8_t old_index, uint8_t new_index);




	virtual bool load(string filename) { return this->SmartLoad(filename); }
	virtual bool Load(const uint8_t *buf, unsigned int size);
	virtual uint8_t *CreateFile(unsigned int *psize);
	virtual bool Compile(TiXmlDocument *doc, bool big_endian = false);
	virtual TiXmlDocument *Decompile() const;
	virtual bool DecompileToFile(const std::string &path, bool show_error = true, bool build_path = false);
	virtual bool CompileFromFile(const std::string &path, bool show_error = true, bool big_endian = false);
	unsigned int CalculateFileSize(uint32_t *vertex_start);
	size_t ExportObj(std::string *vertex_out, std::string *uvmap_out, std::string *normal_out, std::string *topology_out, size_t v_start_idx = 0, bool write_group = false) const;
	size_t InjectObj(const std::string &obj, bool do_uv, bool do_normal, bool show_error = true);
	size_t InjectObjBySubParts(const std::string &obj, bool do_uv, bool do_normal, bool show_error = true);


	void readEmd(EMD* emd);
	void readEsk(ESK* esk);
	void writeEsk(ESK* esk);										//need read esk before emd, because of pointer
	void writeEmd(EMD* emd);										//need read esk before emd, because of pointer
	void writeEmdEsk(EMD* emd, ESK* esk) { writeEsk(esk); writeEmd(emd);}

	void writeEsk__recursive(EMO_Bone* emoBone, std::vector<EMO_Bone> &bones, EskTreeNode* treeNode, std::vector<EskTreeNode*> &listTreeNode);


#ifdef FBX_SUPPORT
	bool ExportFbx(FbxScene *scene, bool normal_parts, bool edges) const;
	bool InjectFbx(FbxScene *scene, bool normal_parts, bool edges, bool use_fbx_tangent);

	bool ExportFbx(const std::string &subpart, FbxScene *scene);
	bool InjectFbx(const std::string &subpart, FbxScene *scene, bool use_fbx_tangent);
#endif


protected:
	virtual void RebuildSkeleton(const std::vector<EMO_Bone *> &old_bones_ptr);
};





}

#endif // __EMOFILE_H__
