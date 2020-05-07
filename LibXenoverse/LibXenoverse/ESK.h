#ifndef LIBXENOVERSE_ESK_H_INCLUDED
#define LIBXENOVERSE_ESK_H_INCLUDED

#define LIBXENOVERSE_ESK_SIGNATURE    "#ESK"
#define LIBXENOVERSE_ESK_BONE_HAVE_IK    4

#define LIBXENOVERSE_ESK_BONE_HIERARCHY_FIRST    1
#define LIBXENOVERSE_ESK_BONE_HIERARCHY_SECOND   2


#include "EMO_BaseFile.h"
#include "BinColorTag.h"


namespace LibXenoverse
{

struct ESK_Header
{
	char signature[4];			// 0
	uint16_t endian;			// 4
	uint16_t header_size;		// 6
	uint8_t version[4];			// 8			//could be something else but difficult to see.
	uint32_t unknow_0;			// C
	uint32_t offset_skeleton;	// 10
	uint32_t offset_nextPart;	// 14			// in case of nsk , it's the offset adress for next part, a emd.
	uint32_t unknow_1;			// 18			//padding ? Todo check
	uint32_t unknow_2;			// 1C			//padding ? Todo check
} PACKED;
static_assert(sizeof(ESK_Header) == 0x20, "Incorrect structure size.");


struct ESK_Skeleton_Section
{
	uint16_t number_bones;			// 0
	uint16_t unknow_flag;			// 2
	uint32_t offset_bonesHierarchy;	// 4	//parentIndex + childIndex + siblingINdex + unk_0
	uint32_t offset_boneNames;		// 8
	uint32_t offset_relTransforms;	// C	//relative position, orientation (quaternion), scale (relative to parent)
	uint32_t offset_absMatrices;	// 10	//abslute matrix.
	uint32_t offset_IKs;			// 14	//inverse cinematics
	uint32_t offset_boneExtraInfo;	// 18
	uint32_t skeletonUniqueId_p1;	// 1C
	uint32_t skeletonUniqueId_p2;	// 20
} PACKED;
static_assert(sizeof(ESK_Skeleton_Section) == 0x24, "Incorrect structure size.");


struct ESK_Bone_Hierarchy
{
	uint16_t parent_index;		// 0
	uint16_t child_index;		// 2
	uint16_t sibling_index;		// 4
	uint16_t ik_flag;			// 6
	
} PACKED;
static_assert(sizeof(ESK_Bone_Hierarchy) == 0x8, "Incorrect structure size.");


struct ESK_Bone_Relative_Transform
{
	float position[4];		// 0
	float orientation[4];	// 10
	float scale[4];			// 20
} PACKED;
static_assert(sizeof(ESK_Bone_Relative_Transform) == 0x30, "Incorrect structure size.");

struct ESK_Bone_Absolute_Matrix
{
	float m0[4];			// 0
	float m1[4];			// 10
	float m2[4];			// 20
	float m3[4];			// 30
} PACKED;
static_assert(sizeof(ESK_Bone_Absolute_Matrix) == 0x40, "Incorrect structure size.");


struct ESK_IK_Section
{
	uint32_t number_IkGroups;	// 0
} PACKED;
static_assert(sizeof(ESK_IK_Section) == 0x4, "Incorrect structure size.");


struct ESK_IK_Group
{
	uint16_t typeGroup;		// 0
	uint16_t sizeGroup;		// 2			//number of bytes for the groups
} PACKED;
static_assert(sizeof(ESK_IK_Group) == 0x4, "Incorrect structure size.");


struct ESK_IK
{
	uint8_t unknow_0;			// 0
	uint8_t number_relations;	// 1
} PACKED;
static_assert(sizeof(ESK_IK) == 0x2, "Incorrect structure size.");

struct ESK_IK_b
{
	uint8_t unknow_0;			// 0
	uint8_t unknow_1;			// 1
} PACKED;
static_assert(sizeof(ESK_IK_b) == 0x2, "Incorrect structure size.");



struct ESK_Bone_extraInfo			//notice : not linked to IK (could not have IK)
{
	uint16_t unknow_0;		// 0
	uint16_t unknow_1;		// 2
	uint16_t unknow_2;		// 4
	uint16_t unknow_3;		// 6
} PACKED;
static_assert(sizeof(ESK_Bone_extraInfo) == 0x8, "Incorrect structure size.");




//case StreetFighters emg
struct ESK_IK_2
{
	uint32_t number_bones;	// 0
	uint32_t unknow_0;		// 1
} PACKED;
static_assert(sizeof(ESK_IK_2) == 0x8, "Incorrect structure size.");




class ESK;
class ESKBone;



class Esk_IK_Relation											//Inverse Kinematic.
{
public:
	struct IKR_Bone
	{
		ESKBone* bone;
		float value;

		IKR_Bone(ESKBone* bone = 0, float value = 0.0f) {this->bone = bone; this->value = value;}
	};

	uint8_t unknow_0;
	std::vector<IKR_Bone> mListBones;

	Esk_IK_Relation() {};
	~Esk_IK_Relation() {};
};


class Esk_IK_Group												//group of inverse Kinematic.
{
public:
	std::vector<Esk_IK_Relation> mListIK;

	Esk_IK_Group() {};
	~Esk_IK_Group() {};
};



class EskTreeNode												//to easely look and manipulate bones
{
public:
	EskTreeNode* mParent;
	std::vector<EskTreeNode*> mChildren;

	ESKBone* mBone;
	size_t mIndex;


	EskTreeNode(ESKBone* bone = nullptr, size_t index = (size_t)-1, EskTreeNode* parent = nullptr);
	~EskTreeNode();

	EskTreeNode* clone(bool alsoCloneBone, bool recursif = false);
	void	setupBones_list(vector<ESKBone*> &newListBones);						//the first thing to do is to make the new list bone and refresh index.
	void	setupBones(vector<ESKBone*> &newListBones);						//the second thing is to refresh parent , child and sibling index.
	string	getDisplayStr(size_t indent = 0);
	string	getJsonStr(size_t indent = 0, bool havePrevious = false);
	EskTreeNode*	getBoneWithIndex(size_t index);
	EskTreeNode*	getBoneWithName(string name);
	


	void	addTree(EskTreeNode* treeBoneSrc, bool duplicateBone, size_t startIndexForNewBones, std::vector<std::vector<size_t>> &listNewBonesAdded, vector<ESKBone*> &listBones, bool recursif = true);

	void	getTreeBonesList(vector<EskTreeNode*> &newListBones);
};




/*-------------------------------------------------------------------------------\
|                             ESKBone				                             |
\-------------------------------------------------------------------------------*/
class ESKBone
{

public:
	string name;
	unsigned short parent_index;
	unsigned short child_index;
	unsigned short sibling_index;
	unsigned short ik_flag;						//index in Children
	bool haveAbsoluteMatrix;
	bool mVisible;
	float absoluteMatrix[16];
	float relativeTransform[12];
	
	bool childIndex_is0xFFFF;					//some files have 0xFFFF as childrenIndex on the first Bone. that strange because the rest of hierarchy is good, but I didn't found why.
	size_t hierarchyIndex;						//some files have twice the first bone (a another bone have the same name). with the rest of bones, it's seam to be a second hierarchy : we read the first , and we begin agin from first bone (with the hyp that is the same bone).
												// so it's here a flag : LIBXENOVERSE_ESK_BONE_HIERARCHY_FIRST | LIBXENOVERSE_ESK_BONE_HIERARCHY_SECOND, and 0 if there is only one.

	std::vector<string> mListFilenameOrigin;
	size_t unk_extraInfo_0;							//with bone_extraInfo_offset
	size_t unk_extraInfo_1;
	size_t unk_extraInfo_2;
	size_t unk_extraInfo_3;
	

	ESKBone(string name = "");
	ESKBone(ESKBone *eskBone);
	~ESKBone(void) { }

	void	setName(const string &name) { this->name = name; }
	string	getName(void) { return name; }
	void	setVisible(bool enable) { mVisible = enable; }
	bool	getVisible(void) { return mVisible; }
	std::vector<string>	&getFilenameOriginList(void) { return mListFilenameOrigin; }

	void	readName(File *file);
	void	writeName(File *file);
	void	readIndices(File *file);
	void	writeIndices(File *file);
	void	readMatrix(File *file);
	void	writeMatrix(File *file);
	void	readRelativeTransform(File *file);
	void	writeRelativeTransform(File *file);
	
	void	mergeMatrix(ESKBone *eskBone);	
	void	mergeFilenameOrigin(ESKBone *eskBone);
	
	void	test_calculAbsoluteMatrixFromRelativeTransform(std::vector<ESKBone *> &listBones, bool recursive = false);
	void	calculAbsoluteMatrixFromRelativeTransform(std::vector<ESKBone *> &listBones, bool recursive = false);
	void	calculRelativeTransformFromAbsoluteMatrix(std::vector<ESKBone *> &listBones, bool recursive = false);
	void	calculRelativeTransformFromRelativeTransformMatrix(std::vector<double> relativeTransformMatrix);
	std::vector<double> calculRelativeMatrixFromAbsoluteMatrix(std::vector<ESKBone *> &listBones, bool recursive = false);

	string	getRelativeTransformDebug(void);
	string	getAbsoluteMatrixDebug(void);

	
	EskTreeNode*	importXml(TiXmlElement* xmlCurrentNode, std::vector<ESKBone*> &bones, EskTreeNode* treeParentNode, bool haveExtraBytesOnEachBone);
	TiXmlElement*	exportXml(EskTreeNode* EskTreeNode, bool haveExtraBytesOnEachBone);
			
	static	void	displayMatrix4x4(double* transformMatrix);
	static	void	transpose4x4(double* transformMatrix);
	static	void	makeTransform4x4(double* posOrientScaleMatrix, double* resultTransformMatrix);	//posOrientScaleMatrix is 3x4, orient is a quaternion informations, resultTransformMatrix is 4x4
	static	void	decomposition4x4(double* transformMatrix, double* resultPosOrientScaleMatrix); //posOrientScaleMatrix is 3x4, orient is a quaternion informations, TransformMatrix is 4x4
	static	void	inverse4x4(double* transformMatrix, double* inversedTransformMatrix);
	static	void	concatenate4x4(double* transformMatrix_left, double* transformMatrix_right, double* resultTransformMatrix);

	TiXmlElement*	exporFpftXml(size_t index, std::vector<ESKBone *> &listBones);
};



/*-------------------------------------------------------------------------------\
|                             ESK					                             |
\-------------------------------------------------------------------------------*/
class ESK
{
	friend class EMO_Skeleton;
	friend class EMO;
	friend class EMA;
	friend class EAN;

protected:
	string name;
	string version;
	uint16_t flag;

	uint64_t skeletonUniqueId;
	bool mHaveExtraBytesOnEachBone;						//case of unknow_offset_1 != 0, only camera animation don't have this.

	vector<ESKBone*> bones;
	std::vector<Esk_IK_Group> listInverseKinematic;

	uint32_t unknow_0;
	uint32_t unknow_1;
	uint32_t unknow_2;

public:
			
	ESK(ESK* esk = 0);
	virtual ~ESK(void);

	bool	load(string filename);
	bool	load(File *file, string s_name);
	void	save(string filename, bool big_endian = false);
	void	save(File *file, bool big_endian = false);

	void	read(File *file);
	unsigned int	getWriteSize(bool withAbsoluteMatrix = true);					//size of byte to record skeleton with write function.
	size_t	calculIksize();
	void	setupBoneIkLinks();
	void	write(File *file, bool withAbsoluteMatrix = true);

	void	merge(ESK *esk);
	void	merge_byAttach(ESK *esk);
	void	merge_byAttach_recursive(EskTreeNode* skeleton_tree_toAdd, EskTreeNode* skeleton_tree);
	void	clone(ESK *esk);
			
			
	string	getName(void) { return name; }
	void	setName(string name) { this->name = name; }
	vector<ESKBone*>	&getBones(void) { return bones; }
	unsigned short	getFlag(void) { return flag; }
			
	ESKBone*	getBone(size_t index) { return ((index < bones.size()) ? bones.at(index) : NULL); }
	size_t	getBoneIndex(const string &name);
	ESKBone*	getBone(const string &name) { return getBone(getBoneIndex(name)); }
	void	addBone(ESKBone* bone) { bones.push_back(bone); }

	std::vector<size_t>	getListIndexOfChildBones(size_t index);

	EskTreeNode*	getTreeOrganisation(size_t index = (size_t)-1, EskTreeNode* parent = nullptr, std::vector<bool>* listAllReadyUsed = 0);
	std::vector<std::vector<size_t>>	setTreeOrganisation(EskTreeNode* treeBone);

	std::vector<std::vector<size_t>>	remove(size_t index, bool recursive = true);
	void	clear(void);
	std::vector<std::vector<std::vector<size_t>>>	addTree(EskTreeNode* treeBone, size_t index);

	bool	loadXml(string filename);
	void	saveXml(string filename);
	bool	importXml(TiXmlElement* xmlCurrentNode);
	TiXmlElement*	exportXml(void);

#ifdef LIBXENOVERSE_FBX_SUPPORT

	struct FbxBonesInstance_DBxv
	{
		std::string mName;
		FbxNode* mNode;
		size_t mIndexInAnimation;												//notice , it's not the same index into EMDtriangle because we select only few bones.

		FbxBonesInstance_DBxv(std::string name, FbxNode* node, size_t indexInAnimation)
		{
			mName = name;
			mNode = node;
			mIndexInAnimation = indexInAnimation;
		}
	};

	void	importFBXSkeleton(FbxScene *scene, bool allowCamera = false);
	void	exportFBXBone(FbxScene *scene, vector<ESK::FbxBonesInstance_DBxv> &global_fbx_bones, FbxNode* parentFbxNode = 0, unsigned short parentIndex = 0xFFFF);			//recursive
#endif
	

	void saveFpfXml_Extract(string filename, ESK* esk_sourceBonesName);
	TiXmlElement*	exporFpftXml(ESK* esk_sourceBonesName);

	//Debug/work: create a file for wxHexEditor, for add tag and color on section of the file. also detect if a file still have empty part (but you better have to ouput in a log file)
	void save_Coloration(string filename, bool show_error = false);
	void write_Coloration(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size);
};

}

#endif