#ifndef LIBXENOVERSE_EMD_H_INCLUDED
#define LIBXENOVERSE_EMD_H_INCLUDED

#define LIBXENOVERSE_EMD_SIGNATURE           "#EMD"
#define LIBXENOVERSE_EMD_SUBMESH_BONE_LIMIT  24


#define EMD_VTX_FLAG_POS				0x1
#define EMD_VTX_FLAG_NORM				0x2
#define EMD_VTX_FLAG_TEX				0x4
#define EMD_VTX_FLAG_TEX2				0x8
#define EMD_VTX_FLAG_COLOR				0x40
#define EMD_VTX_FLAG_TANGENT			0x80
#define EMD_VTX_FLAG_BLEND_WEIGHT		0x200
#define EMD_VTX_FLAG_COMPRESSED_FORMAT	0x8000
//Notice tangent is after Norm.

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

#include "EMM.h"

namespace LibXenoverse
{

/*-------------------------------------------------------------------------------\
|                             EMDVertex				                             |
\-------------------------------------------------------------------------------*/
class EMDVertex
{
	friend class EMD;

public:
	uint16_t flags;					//we keep the flags to know, what to use.

	float pos_x;
	float pos_y;
	float pos_z;

	float norm_x;					//same is in DBX (may the flags 0x8000),
	float norm_y;					// you have float16 values
	float norm_z;					// we will stock in float (is float32)

	float tang_x;					//same as norm_x, could be in float16 in file
	float tang_y;					// but stock in float32.
	float tang_z;

	float text_u;					//same : text_u could be in float16 in certain files, 
	float text_v;					// but stock in float32.

	float text2_u;					//same as text_u
	float text2_v;

	uint32_t color;

	uint8_t blend[4];
	float blend_weight[4];			//same blend_weight[1] could be a float16 in certain files


	EMDVertex(EMDVertex* emdVertex = 0);

	void	zero(void);
	bool	operator == (const EMDVertex& vertex);
	
	void	read(File *file, uint16_t parentflags);
	void	write(File *file, uint16_t flags);

	void	getColorRGBAFloat(float &r, float &g, float &b, float &a);			//get values into [0,1] ranges
	void	setColorFromRGBAFloat(float r, float g, float b, float a);			//set values from [0,1] ranges

	static size_t getSizeFromFlags(uint16_t flags, bool fromFile = true);
	static std::string getFlagsVertexDefinitionInformations(uint16_t flags);

	bool	importXml(TiXmlElement* xmlCurrentNode, uint16_t flags = 0);
	TiXmlElement*	exportXml(void);
};

/*-------------------------------------------------------------------------------\
|                             EMDTriangles			                             |
\-------------------------------------------------------------------------------*/
class EMDTriangles
{
	friend class EMD;

public:
	vector<unsigned short> faces;
	vector<string> bone_names;


	EMDTriangles(EMDTriangles* emdTriangles = 0);

	void	read(File *file);
	void	write(File *file);

	void	getBonesNames(vector<string> &bones_names);
	void	replaceBonesNames(const string &oldName, const string &newName);

	bool	importXml(TiXmlElement* xmlCurrentNode, vector<EMDVertex> &vertices);
	TiXmlElement*	exportXml(void);
};

/*-------------------------------------------------------------------------------\
|                             EMDSubmeshDefinition	                             |
\-------------------------------------------------------------------------------*/
class EMDSubmeshDefinition
{
	friend class EMD;
	friend class EMG_Texture;

public:
	unsigned char flag0;
	unsigned char texIndex;
	unsigned char flag1;
	unsigned char flag2;
	float textScale_u;					//if 5 => 5 repetitions.
	float textScale_v;


	EMDSubmeshDefinition(EMDSubmeshDefinition* emdSubmeshDefinition = 0);
	EMDSubmeshDefinition(unsigned char texIndex, float textScale_u = 1.0f, float textScale_v = 1.0f) { zero(); this->texIndex = texIndex; this->textScale_u = textScale_u; this->textScale_v = textScale_v; }

	void	zero(void);

	void	read(File *file);
	void	write(File *file);

	bool	importXml(TiXmlElement* xmlCurrentNode);
	TiXmlElement*	exportXml(void);
};


/*-------------------------------------------------------------------------------\
|                             EMDSubmesh			                             |
\-------------------------------------------------------------------------------*/
class EMDSubmesh
{
	friend class EMD;
	friend class EMDMesh;
	friend class EMG;
	friend class EMG_SubPart;
	friend class EMG_SubMesh;
	friend class FMP_Hitbox;
	friend class Havok;

protected:
	string name;									//it's also the materialName
	unsigned int vertex_type_flag;
	unsigned int vertex_size;
	uint8_t unknow_0;

	vector<EMDVertex> vertices;
	vector<EMDTriangles> triangles;
	vector<EMDSubmeshDefinition> definitions;
	
	float aabb_center_x;
	float aabb_center_y;
	float aabb_center_z;
	float aabb_center_w;
	float aabb_min_x;
	float aabb_min_y;
	float aabb_min_z;
	float aabb_min_w;
	float aabb_max_x;
	float aabb_max_y;
	float aabb_max_z;
	float aabb_max_w;

public:
	string uniqName;									//for instance
	

public:
	EMDSubmesh(EMDSubmesh* emdSubmesh = 0);
	void	zero(void);


	void	read(File *file);
	void	write(File *file);

	size_t	getVertexTypeFlags(void) { return vertex_type_flag; }
	void	setVertexScale(float scale);
	void	getBonesNames(vector<string> &bones_names);
	void	replaceBonesNames(const string &oldName, const string &newName);
	string	getMaterialName(void) { return name; }
	vector<EMDVertex>	&getVertices(void) { return vertices; }
	vector<EMDTriangles>	&getTriangles(void) { return triangles; }
	vector<EMDSubmeshDefinition>	&getDefinitions(void) { return definitions; }
	size_t	mergeVertex(EMDVertex &v);
	void	mergeTriangles();

	EMDTriangles*	getTriangleForBoneNameCombinaison(vector<string> bone_names, bool createIfDontExist = true);


	bool	importXml(TiXmlElement* xmlCurrentNode);
	TiXmlElement*	exportXml(void);

#ifdef LIBXENOVERSE_FBX_SUPPORT
	void	importFBX(FbxNode* fbxNode, bool compressedFlag = true);
	bool	importFBX_OldMethode(FbxNode* fbxNode, FbxMesh* fbxMesh, int material_index, FbxAMatrix transform_matrix, string material_names, vector<vector<pair<double, FbxNode *>>> &control_points_skin_bindings, bool compressedFlag = true);
	void	exportFBX(FbxScene* scene, std::vector<ESK::FbxBonesInstance_DBxv> &global_fbx_bones, FbxNode* parentNode, std::vector<EMB*> listTexturePackEMB, EMM* emmMaterial = 0, bool wantNoTexture = false, FbxNode* forceBoneNode = 0);
	FbxSurfaceMaterial*	exportFBXMaterial(FbxScene *scene, string material_name, std::vector<EMB*> listTexturePackEMB, EMM* emmMaterial = 0, bool wantNoTexture = false);
	FbxTexture* getTexture(FbxSurfaceMaterial * mat, const char* typeMap);
#endif
};


/*-------------------------------------------------------------------------------\
|                             EMDMesh				                             |
\-------------------------------------------------------------------------------*/
class EMDMesh
{
	friend class EMD;
	friend class EMG;
	friend class EMP;
	friend class FMP_Hitbox;
	friend class Havok;

protected:
	string name;
	unsigned short unknown_total;
	vector<EMDSubmesh*> submeshes;
	
	float aabb_center_x;
	float aabb_center_y;
	float aabb_center_z;
	float aabb_center_w;
	float aabb_min_x;
	float aabb_min_y;
	float aabb_min_z;
	float aabb_min_w;
	float aabb_max_x;
	float aabb_max_y;
	float aabb_max_z;
	float aabb_max_w;
	


public:
	EMDMesh(EMDMesh* emdMesh = 0);
	~EMDMesh(void);
	void	zero(void);

	void	read(File *file);
	void	write(File *file);

	void	setVertexScale(float scale);
	string	getName(void) { return name; }
	
	void	getBonesNames(vector<string> &bones_names);
	void	replaceBonesNames(const string &oldName, const string &newName);
			
	vector<EMDSubmesh *>	&getSubmeshes(void)	{ return submeshes; }
	void	mergeTriangles();

	bool	importXml(TiXmlElement* xmlCurrentNode);
	TiXmlElement*	exportXml(void);

#ifdef LIBXENOVERSE_FBX_SUPPORT
	void	importFBX(FbxNode* fbxNode, bool compressedFlag = true);
	void	importFBX_OldMethode(FbxNode* fbxNode, bool compressedFlag = true);
	void	exportFBX(FbxScene* scene, std::vector<ESK::FbxBonesInstance_DBxv> &global_fbx_bones, FbxNode* parentNode, std::vector<EMB*> listTexturePackEMB, EMM* emmMaterial = 0, bool wantNoTexture = false, FbxNode* forceBoneNode = 0);
#endif
};


/*-------------------------------------------------------------------------------\
|                             EMDModel				                             |
\-------------------------------------------------------------------------------*/
class EMDModel
{
	friend class EMD;
	friend class EMO_PartsGroup;
	friend class EMP;
	friend class FMP_Hitbox;
	friend class Havok;

protected:
	string name;
	unsigned short unknown_total;
	vector<EMDMesh*> meshes;
	

public:
	EMDModel(EMDModel* emdModel = 0);
	~EMDModel(void);

	void	read(File *file);
	void	write(File *file);

	string	getName(void) { return name; }
	void	setVertexScale(float scale);
	vector<string> getMeshNames(void);
	vector<EMDMesh *>	&getMeshes(void) { return meshes; }
	void	getBonesNames(vector<string> &bones_names);
	void	replaceBonesNames(const string &oldName, const string &newName);
	void	mergeTriangles();

	bool	importXml(TiXmlElement* xmlCurrentNode);
	TiXmlElement*	exportXml(void);

#ifdef LIBXENOVERSE_FBX_SUPPORT
	void	importFBX(FbxNode* fbxNode, bool compressedFlag = true);
	void	exportFBX(FbxScene* scene, std::vector<ESK::FbxBonesInstance_DBxv> &global_fbx_bones, FbxNode *parentNode, size_t indexModel, std::vector<EMB*> listTexturePackEMB, EMM* emmMaterial = 0, bool wantNoTexture = false);
#endif
};


/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
class EMD
{
	friend class EMO;
	friend class EMP;
	friend class FMP_Hitbox;
	friend class Havok;

protected:
	string name;
	unsigned short unknown_total;
	vector<EMDModel*> models;

public:
	EMD(EMD* emd = 0);
	~EMD(void);

	bool	load(string filename);
	bool	load(File *file, string s_name);
	void	save(string filename, bool big_endian = false);
	void	save(File *file, bool big_endian = false);

	void	read(File *file);
	void	write(File *file);

	string	getName(void) { return name; }
	void	setVertexScale(float scale);
	vector<string>	getBonesNames(void);
	void	replaceBonesNames(const string &oldName, const string &newName);
	size_t	getNumberModel() { return models.size(); }
	void	mergeTriangles();

	bool	loadXml(string filename);
	void	saveXml(string filename);
	bool	importXml(TiXmlElement* xmlCurrentNode);
	TiXmlElement*	exportXml(void);

#ifdef LIBXENOVERSE_FBX_SUPPORT
	void	importFBX(FbxNode *fbxNode, bool compressedFlag = true);
	void	exportFBX(FbxScene *scene, std::vector<ESK::FbxBonesInstance_DBxv> &global_fbx_bones, std::vector<EMB*> listTexturePackEMB, EMM* emmMaterial = 0, bool wantNoTexture = false);
#endif
};

}

#endif