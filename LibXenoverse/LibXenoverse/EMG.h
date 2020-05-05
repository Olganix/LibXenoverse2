#ifndef __EMGFILE_H__
#define __EMGFILE_H__

#include <utility>


//EMG is mesh into emo files.

#include "EMO_Skeleton.h"


#ifdef _MSC_VER
#ifndef FBX_SUPPORT
//#define FBX_SUPPORT
#endif
#endif

#ifdef FBX_SUPPORT
#include <fbxsdk.h>
#endif


namespace LibXenoverse
{

#define EMG_SIGNATURE	0x474D4523

#define EMG_VTX_FLAG_POS				0x1
#define EMG_VTX_FLAG_NORM				0x2
#define EMG_VTX_FLAG_TEX				0x4
#define EMG_VTX_FLAG_TEX2				0x8
#define EMG_VTX_FLAG_COLOR				0x40
#define EMG_VTX_FLAG_TANGENT			0x80
#define EMG_VTX_FLAG_BLEND_WEIGHT		0x200
#define EMG_VTX_FLAG_COMPRESSED_FORMAT	0x8000


#define EMG_TUS_ADRESSMODE_WRAP		0
#define EMG_TUS_ADRESSMODE_MIRROR	1
#define EMG_TUS_ADRESSMODE_CLAMP	2

#define EMG_TUS_FILTERING_NONE		0
#define EMG_TUS_FILTERING_POINT		1
#define EMG_TUS_FILTERING_LINEAR	2

// Notice:
//	if there isn't EMG_VTX_FLAG_COMPRESSED_FORMAT (like Saint Seya's Emo Files),
//		there is only float32 for all float values, 
//		Uint32 for color
//		blend = 4 * Uint8 = 4 octets
//		
//	else (like DragonBall Xenoverse) :
//		-Position is 3 x float32 = 12 octets
//		-Normal and tangent are 3 x float16 + 2 empty octet = 8 octets
//		-textureCoordinate is 2 * float16 = 4 octets.
//		-color is 1 * Uint32 = 4 octets.
//		-blendIndice is 4 * unit8 = 4 octets
//		-BlendWeight is 4 * float16 = 8 octets


#ifdef _MSC_VER
#pragma pack(push,1)
#endif

struct EMG_ChunkHeader
{
	uint32_t signature;			// 0    #EMG
	uint16_t unknow_inc;		// 4
	uint16_t subparts_count;	// 6
	uint32_t offsets[1];		// 8
	// remaining offsets
} PACKED;
static_assert(sizeof(EMG_ChunkHeader) == 0xC, "Incorrect structure size.");

struct EMG_SubPart_Section
{
	uint32_t vertex_type_flag;		// 0
	uint32_t textures_lists_count;	// 4
	uint32_t unknow_0;				// 8
	uint32_t textures_lists_offset; // 0x0C

	uint16_t vertex_count;			// 0x10
	uint16_t vertex_size;			// 0x12
	uint32_t vertex_offset;			// 0x14
	uint16_t strips;				// 0x18
	uint16_t submesh_count;			// 0x1A
	uint32_t submesh_list_offset;	// 0x1C
	float	aabb_center_x;			// 20
	float	aabb_center_y;			// 24
	float	aabb_center_z;			// 28
	float	aabb_center_w;			// 2c
	float	aabb_min_x;				// 30
	float	aabb_min_y;				// 34
	float	aabb_min_z;				// 38
	float	aabb_min_w;				// 3c
	float	aabb_max_x;				// 40
	float	aabb_max_y;				// 44
	float	aabb_max_z;				// 48
	float	aabb_max_w;				// 4c
} PACKED;
static_assert(sizeof(EMG_SubPart_Section) == 0x50, "Incorrect structure size.");

struct EMG_TextureListHeader
{
	uint32_t number_texture;	// 0
} PACKED;
static_assert(sizeof(EMG_TextureListHeader) == 0x4, "Incorrect structure size.");


struct EMGTextureDef_Section
{
	uint8_t flag0;				// 0
	uint8_t emb_TextureIndex;	// 1
	uint8_t adressMode_uv;		// 2
	uint8_t filtering_minMag;	// 3
	float textScale_u;			// 4
	float textScale_v;			// 8
} PACKED;
static_assert(sizeof(EMGTextureDef_Section) == 0xC, "Incorrect structure size.");

struct EMG_EMG_SubMeshHeader
{
	float barycenter[4]; // 0				// seam to be the barycenter for each submesh
	uint16_t textureList_index; // 0x10
	uint16_t face_count; // 0x12
	uint16_t linked_bones_count; // 0x14 bone mapping
	char emm_material_name[0x20]; // 0x16
	// size 0x36
} PACKED;
static_assert(sizeof(EMG_EMG_SubMeshHeader) == 0x36, "Incorrect structure size.");



//All case of vertex structure (by the flags) will complete this structure.
//is about get all datas in the same float size (not the case in certain files)
//Notice : so, if you rewrite file with this structure, the file could be bigger.
struct EMG_VertexCommon
{
	uint16_t flags;				//we keep the flags to know, what to use.
	
	float pos_x;
	float pos_y;
	float pos_z;
	
	float norm_x;				//same is in DBX (may the flags 0x8000),
	float norm_y;				// you have float16 values
	float norm_z;				// we will stock in float (is float32)
	
	float tang_x;				//same as norm_x, could be in float16 in file
	float tang_y;				// but stock in float32.
	float tang_z;

	float text_u;				//same : text_u could be in float16 in certain files, 
	float text_v;				// but stock in float32.

	float text2_u;				//same as text_u
	float text2_v;
	
	uint32_t color;

	uint8_t blend[4];
	float blend_weight[4];		//same blend_weight[1] could be a float16 in certain files

	EMG_VertexCommon();

	void	getColorRGBAFloat(float &r, float &g, float &b, float &a) const;			//get values into [0,1] ranges
	void	setColorFromRGBAFloat(float r, float g, float b, float a);			//set values from [0,1] ranges
	size_t getSizeFromFlags(bool fromEmoFile = false) const;

} PACKED;


#ifdef _MSC_VER
#pragma pack(pop)
#endif





class EMO;
class EMO_PartsGroup;
class EMG;

class EMG_SubPart;
class EMG_TexturesList;
class EMG_SubMesh;
class EMGTextureUnitState;


/********************************************************************************************
*								EMG_VertexData												*
********************************************************************************************/
struct EMG_VertexData
{
	EMG_VertexCommon VertexUnion;
	unsigned int size;

	bool operator==(const EMG_VertexData &rhs) const { if (this->size != rhs.size) { return false; } return (memcmp(&this->VertexUnion, &rhs.VertexUnion, sizeof(this->VertexUnion)) == 0); }
	inline bool operator!=(const EMG_VertexData &rhs) const { return !(*this == rhs); }

	void Decompile(TiXmlNode *root) const;								//to export in readable file
	bool Compile(const TiXmlElement *root);								//to import in readable file

	void readEmdVertex(EMDVertex* emd);
	void writeEmdVertex(EMDVertex* emd);
};


/********************************************************************************************
*									EMG_SubMesh												*
********************************************************************************************/
class EMG_SubMesh
{
	friend class EMG_SubPart;
	friend class EMG;
	friend class EMO_PartsGroup;
	friend class EMO;

private:
	std::string emm_material;
	uint16_t textureList_index;

	std::vector<uint16_t> faces;
	std::vector<EMO_Bone *> linked_bones;
	float barycenter[4];						//seam to be the barycenter of each submesh

	// METADATA - DON'T USE IN COMPARISON
	std::string meta_name;


public:
	static const size_t MAX_EMM_MAT_LENGTH = 31;



	bool operator==(const EMG_SubMesh &rhs) const;
	inline bool operator!=(const EMG_SubMesh &rhs) const { return !(*this == rhs); }


	inline const std::string &GetMetaName() const { return meta_name; }

	inline const std::vector<uint16_t> &GetFaces() const { return faces; }
	inline void SetFaces(const std::vector<uint16_t> &faces) { this->faces = faces; }


	inline std::vector<EMO_Bone *> &GetBones() { return linked_bones; }
	size_t GetLinkedBones(std::vector<EMO_Bone*> &list, bool clear_vector, bool unique = true, const EMO_Skeleton *sorter = nullptr) const;

	bool SetEmmMaterial(const std::string &emm_material);
	const std::string &getEmmMaterial(void){ return emm_material; }

	bool IsEdge() const;

	void Decompile(TiXmlNode *root, size_t polygon_count) const;
	bool Compile(const TiXmlElement *root, EMO_Skeleton *skl);

	std::vector<uint16_t> getRealFaceIndex(bool strips = false);

	void readEmdSubMesh(EMDSubmesh* emd, size_t textlist_index, size_t indexTriangle, EMO* emo);
	void writeEmdSubMesh(EMDSubmesh* emd, bool strips = false);
};




/********************************************************************************************
*									EMGTextureUnitState												*
********************************************************************************************/
class EMGTextureUnitState
{
	friend class EMG_TexturesList;
	friend class EMG_SubPart;
	friend class EMG;
	friend class EMO_PartsGroup;
	friend class EMO;

private:
	uint8_t flag0;
	uint8_t emb_TextureIndex;
	unsigned char adressMode_u;
	unsigned char adressMode_v;					//wrap, mirror or clamp
	unsigned char filtering_minification;
	unsigned char filtering_magnification;		//none, point, linear.
	float textScale_u;
	float textScale_v;

public:
	EMGTextureUnitState(void) { emb_TextureIndex = flag0 = adressMode_u = adressMode_v = filtering_minification = filtering_magnification = 0; textScale_u = textScale_v = 1.0f; }
	bool operator==(const EMGTextureUnitState &rhs) const;
	inline bool operator!=(const EMGTextureUnitState &rhs) const { return !(*this == rhs); }


	inline uint8_t GetEmbTextureIndex() { return emb_TextureIndex; }
	inline void SetEmbTextureIndex(uint8_t index) { this->emb_TextureIndex = index; }

	inline float_t GetTextureScale_U() { return textScale_u; }
	inline float GetTextureScale_V() { return textScale_v; }

	void Decompile(TiXmlNode *root) const;
	bool Compile(const TiXmlElement *root);

	void readEmdDef(EMDTextureUnitState* emd);
	void writeEmdDef(EMDTextureUnitState* emd);
};

/********************************************************************************************
*									EMG_TexturesList										*
********************************************************************************************/
class EMG_TexturesList
{
	friend class EMG_SubPart;
	friend class EMG;
	friend class EMO_PartsGroup;
	friend class EMO;

private:
	std::vector<EMGTextureUnitState> textures;

public:
	inline bool operator==(const EMG_TexturesList &rhs) const { return (this->textures == rhs.textures); }
	inline bool operator!=(const EMG_TexturesList &rhs) const { return !(*this == rhs); }
	inline EMGTextureUnitState &operator[](size_t n) { return textures[n]; }
	inline const EMGTextureUnitState &operator[](size_t n) const { return textures[n]; }
	inline std::vector<EMGTextureUnitState>::iterator begin() { return textures.begin(); }
	inline std::vector<EMGTextureUnitState>::iterator end() { return textures.end(); }
	inline std::vector<EMGTextureUnitState>::const_iterator begin() const { return textures.begin(); }
	inline std::vector<EMGTextureUnitState>::const_iterator end() const { return textures.end(); }

	inline uint16_t GetNumTexture() const { return textures.size(); }
	inline EMGTextureUnitState *GetTexture(uint16_t idx) { return  ((idx >= textures.size()) ? nullptr : &textures[idx]); }
	inline const EMGTextureUnitState *GetTexture(uint16_t idx) const { return ((idx >= textures.size()) ? nullptr : &textures[idx]); }


	size_t GetEmbIndexes(std::vector<uint8_t> &list, bool clear_vector, bool unique = true, bool sort = false) const;
	size_t ReplaceEmbIndex(uint8_t old_index, uint8_t new_index);

	void Decompile(TiXmlNode *root) const;
	bool Compile(const TiXmlElement *root);
};








/********************************************************************************************
*									EMG_SubPart												*
********************************************************************************************/
class EMG_SubPart
{
	friend class EMG;
	friend class EMO_PartsGroup;
	friend class EMO;

	float	aabb_center_x;
	float	aabb_center_y;
	float	aabb_center_z;
	float	aabb_center_w;
	float	aabb_min_x;
	float	aabb_min_y;
	float	aabb_min_z;
	float	aabb_min_w;
	float	aabb_max_x;
	float	aabb_max_y;
	float	aabb_max_z;
	float	aabb_max_w;

	uint16_t strips;
	uint32_t vertex_type_flag;
	uint32_t unknow_0;

	std::vector<EMG_TexturesList> textures_lists;
	std::vector<EMG_SubMesh> submeshes;
	std::vector<EMG_VertexData> vertex;


	// METADATA - Don't use in comparison!
	std::string meta_name;

	uint16_t paddingForCompressedVertex;						//apparently, when compressed normal or tangent, there is  padding witch is not at 00, (it's not a float16) it's the same by mesh.

	size_t fuckingDebug_vertex_offset;				//todo remove.
	size_t fuckingDebug_vertex_size;

public:
	EMG_SubPart() { strips = vertex_type_flag = unknow_0 = 0; paddingForCompressedVertex = 0;}
	bool operator==(const EMG_SubPart &rhs) const;
	inline bool operator!=(const EMG_SubPart &rhs) const { return !(*this == rhs); }
	inline EMG_SubMesh &operator[](size_t n) { return submeshes[n]; }
	inline const EMG_SubMesh &operator[](size_t n) const { return submeshes[n]; }
	inline std::vector<EMG_SubMesh>::iterator begin() { return submeshes.begin(); }
	inline std::vector<EMG_SubMesh>::iterator end() { return submeshes.end(); }
	inline std::vector<EMG_SubMesh>::const_iterator begin() const { return submeshes.begin(); }
	inline std::vector<EMG_SubMesh>::const_iterator end() const { return submeshes.end(); }


	static size_t getSizeFromFlags(uint16_t flags_tmp, bool fromEmoFile = false);
	inline const std::string &GetMetaName() const { return meta_name; }

	inline uint16_t GetNumSubMeshes() const { return submeshes.size(); }
	inline bool RemoveSubMesh(size_t submesh_idx);
	inline EMG_SubMesh *GetSubMesh(uint16_t idx) { return  ((idx >= submeshes.size()) ? nullptr : &submeshes[idx]); }
	inline const EMG_SubMesh *GetSubMesh(uint16_t idx) const { return ((idx >= submeshes.size()) ? nullptr : &submeshes[idx]); }

	inline uint16_t GetNumTextureList() const { return textures_lists.size(); }
	inline EMG_TexturesList *GetTextureList(uint16_t idx) { return  ((idx >= textures_lists.size()) ? nullptr : &textures_lists[idx]); }
	inline const EMG_TexturesList *GetTextureList(uint16_t idx) const { return ((idx >= textures_lists.size()) ? nullptr : &textures_lists[idx]); }


	void SetVertex(const std::vector<EMG_VertexData> &vertex);
	bool InjectVertex(const std::vector<EMG_VertexCommon> &new_vertex, bool do_pos = true, bool do_normal = false, bool do_tang = false, bool do_uvmap = false, bool do_uvmap2 = false, bool do_color = false, bool do_blend = false);
	inline unsigned int GetVertexSize() const { return vertex[0].size; }
	inline std::vector<EMG_VertexData> &GetVertex() { return vertex; }
	inline const std::vector<EMG_VertexData> &GetVertex() const { return vertex; }
	bool IsEdge() const;

	size_t GetNumberOfPolygons(size_t submesh_idx) const;
	std::vector<uint16_t> GetTriangles(size_t submesh_idx) const;
	size_t GetTrianglesInternal(size_t submesh_idx, std::vector<uint16_t> *tri_faces) const;

	void GetSubMeshVertexAndTriangles(uint16_t submesh_idx, std::vector<EMG_VertexData> &sm_vertex, std::vector<uint16_t> &sm_triangles) const;

	inline bool GetStrips() const { return (strips == 0) ? false : true; }
	inline void SetStrips(bool strips) { this->strips = strips; }



	size_t GetLinkedBones(std::vector<EMO_Bone*> &list, bool clear_vector, bool unique = true, const EMO_Skeleton *sorter = nullptr) const;

	size_t ReplaceEmmMaterial(const std::string &old_mat, const std::string &new_mat);
	size_t GetEmmMaterials(std::vector<std::string> &list, bool clear_vector, bool unique = true) const;

	size_t ReplaceEmbIndex(uint8_t old_index, uint8_t new_index);
	size_t GetEmbIndexes(std::vector<uint8_t> &list, bool clear_vector, bool unique = true, bool sort = false) const;


	size_t ExportObj(std::string *vertex_out, std::string *uvmap_out, std::string *normal_out, std::string *topology_out, size_t v_start_idx = 0, bool write_group = true) const;
	bool InjectObj(const std::string &obj, bool do_uv, bool do_normal, int v_start_idx = 0, int vt_start_idx = 0, int vn_start_idx = 0, bool show_error = true);

	void Decompile(TiXmlNode *root) const;
	bool Compile(const TiXmlElement *root, EMO_Skeleton *skl);



	void readEmdSubMesh(EMDSubmesh* emd);
	void writeEmdSubMesh(EMDSubmesh* emd, size_t textListIndex);



#ifdef FBX_SUPPORT
	bool ExportFbx(const EMO_Skeleton &skl, const std::vector<FbxNode *> &fbx_bones, FbxScene *scene) const;
	bool ExportSubMeshFbx(const EMO_Skeleton &skl, uint16_t submesh_idx, const std::vector<FbxNode *> &fbx_bones, FbxScene *scene) const;
	FbxSurfaceMaterial* exportFBXMaterial(FbxScene *scene, string material_name) const;
	void ExportSubMeshFbxSkin(const EMO_Skeleton &skl, const EMG_SubMesh &submesh, const std::vector<EMG_VertexData> &sm_vertex, const std::vector<FbxNode *> &fbx_bones, FbxScene *scene, FbxMesh *fbx_mesh, FbxAMatrix skin_matrix) const;


	static bool LoadFbxNormal(FbxNode *fbx_node, int vertex_index, int control_point_index, EMG_VertexData *v);
	static bool LoadFbxTangent(FbxMesh *fbx_mesh, int vertex_index, int control_point_index, EMG_VertexData *v);
	static bool LoadFbxUV(FbxMesh *fbx_mesh, int poly_index, int pos_in_poly, int control_point_index, EMG_VertexData *v);
	static bool LoadFbxUV2(FbxMesh *fbx_mesh, int poly_index, int pos_in_poly, int control_point_index, EMG_VertexData *v);
	static bool LoadFbxVertexColor(FbxMesh *fbx_mesh, int vertex_index, int control_point_index, uint32_t *color);
	static bool LoadFbxBinormal(FbxMesh *fbx_mesh, int vertex_index, int control_point_index, EMG_VertexData *v);
	static bool LoadFbxBlendData(EMO_Skeleton &skl, const std::vector<EMO_Bone *> &linked_bones, const std::vector<std::pair<double, FbxNode *>> &vertex_weights, uint8_t *blend, float *blend_weight);
	static bool LoadFbxBlendWeights(FbxMesh* fbx_mesh, std::vector<std::vector<std::pair<double, FbxNode *>>>& weights);

	bool InjectSubMeshFbx(EMO_Skeleton &skl, EMG_SubMesh &submesh, FbxNode *fbx_node, std::vector<EMG_VertexData> &sm_vertex, int vertex_size, size_t v_start, bool use_fbx_tangent);
	bool InjectFbx(EMO_Skeleton &skl, const std::vector<FbxNode *> fbx_nodes, bool use_fbx_tangent);
	bool InjectFbx(EMO_Skeleton &skl, FbxScene *scene, bool use_fbx_tangent);
#endif
};


/********************************************************************************************
*										EMG													*
********************************************************************************************/
class EMG : public EMO_BaseFile
{
	friend class EMO_PartsGroup;
	friend class EMO;
	friend class EMP;

private:
	uint16_t unknow_inc;			// it's seam to be a section increment, by take care of header (begin at 2, but sometime 1 , or 3 or 66, always increment but sometime keyframe is 2 or more), it's possible that is priority order (Cf Crack.emo and Crak2.emo in D09_04, Crack2 begin at 102 where Crack finish at 100)

	std::vector<EMG_SubPart> subparts;

	// METADATA - DON'T USE IN COMPARISON
	std::string meta_name;


public:
	EMG();
	EMG(bool big_endian) : EMG() { this->big_endian = big_endian; }
	virtual ~EMG();
	inline bool operator==(const EMG &rhs) const { return ((this->IsEmpty() && rhs.IsEmpty()) ? true : (this->subparts == rhs.subparts && this->unknow_inc == rhs.unknow_inc)); }
	inline bool operator!=(const EMG &rhs) const { return !(*this == rhs); }
	inline EMG_SubPart &operator[](size_t n) { return subparts[n]; }
	inline const EMG_SubPart &operator[](size_t n) const { return subparts[n]; }
	inline std::vector<EMG_SubPart>::iterator begin() { return subparts.begin(); }
	inline std::vector<EMG_SubPart>::iterator end() { return subparts.end(); }
	inline std::vector<EMG_SubPart>::const_iterator begin() const { return subparts.begin(); }
	inline std::vector<EMG_SubPart>::const_iterator end() const { return subparts.end(); }

	uint16_t getUnknow_inc() { return unknow_inc; }
	void setUnknow_inc(uint16_t value) { unknow_inc = value; }

	bool Load(uint8_t *buf, unsigned int size, EMO_Skeleton *skl);
	void Reset();
	inline bool IsEmpty() const { return (subparts.size() == 0); }


	inline uint16_t GetNumSubParts() const { return subparts.size(); }
	inline bool SubPartExists(uint16_t idx) { return (GetSubPart(idx) != nullptr); }
	inline EMG_SubPart *GetSubPart(uint16_t idx) { return  ((idx >= subparts.size()) ? nullptr : &subparts[idx]); }
	inline const EMG_SubPart *GetSubPart(uint16_t idx) const { return ((idx >= subparts.size()) ? nullptr : &subparts[idx]); }
	

	size_t GetLinkedBones(std::vector<EMO_Bone*> &list, bool clear_vector, bool unique = true, const EMO_Skeleton *sorter = nullptr) const;
	
	size_t ReplaceEmmMaterial(const std::string &old_mat, const std::string &new_mat);
	size_t GetEmmMaterials(std::vector<std::string> &list, bool clear_vector, bool unique = true) const;
	
	size_t ReplaceEmbIndex(uint8_t old_index, uint8_t new_index);
	size_t GetEmbIndexes(std::vector<uint8_t> &list, bool clear_vector, bool unique = true, bool sort = false) const;

	unsigned int CreatePart(uint8_t *buf, EMO_Skeleton *skl, uint32_t startOffsetVertices, uint32_t startOffsetEmg, uint32_t *vertex_start, bool vertexInside = false);
	unsigned int CalculatePartSize(bool vertexInside = false);

	unsigned int CreateVertex(uint8_t *buf, size_t startOffset);
	size_t GetNumVertex();
	unsigned int CalculateVertexSize(size_t startOffset);
	bool IsEdge() const;

	static bool ReadObj(const std::string &obj, std::vector<EMG_VertexCommon> &vertex, bool do_uv, bool do_normal, size_t v_start_idx, size_t vt_start_idx, size_t vn_start_idx, size_t *vtx_count, size_t *uv_count, size_t *n_count, bool show_error = true);
	static bool GetNextObjLine(const std::string &content, size_t *pos, std::string &line);
	size_t ExportObj(std::string *vertex_out, std::string *uvmap_out, std::string *normal_out, std::string *topology_out, size_t v_start_idx = 0, bool write_group = true) const;

	void Decompile(TiXmlNode *root, uint16_t id, size_t &emg_inc) const;
	bool Compile(const TiXmlElement *root, EMO_Skeleton *skl);


	static void ColorToFloat(uint32_t color, float *r, float *g, float *b, float *a);
	static uint32_t DoubleToColor(double r, double g, double b, double a);


	void readEmdMesh(EMDMesh* emd, EMO* emo);
	void writeEmdMesh(EMDMesh* emd, string name);

#ifdef FBX_SUPPORT
	bool ExportFbx(const EMO_Skeleton &skl, const std::vector<FbxNode *> &fbx_bones, FbxScene *scene) const;
	bool InjectFbx(EMO_Skeleton &skl, FbxScene *scene, bool use_fbx_tangent);
#endif

	//Debug/work: create a file for wxHexEditor, for add tag and color on section of the file. also detect if a file still have empty part (but you better have to ouput in a log file)
	size_t write_Coloration_EMG(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size, size_t startOffset, std::vector<bool> &listBytesAllreadyTagged, bool vertexInside = false);
};

}

#endif
