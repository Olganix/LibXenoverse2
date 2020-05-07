#ifndef __EMO_SKELETON_H__
#define __EMO_SKELETON_H__

#include <stdint.h>

#include <string>
#include <vector>
#include <stdexcept>

#include "EMO_BaseFile.h"
#include "BinColorTag.h"

namespace LibXenoverse
{


#ifdef _MSC_VER
#ifndef FBX_SUPPORT
 #define FBX_SUPPORT
#endif
#endif

#ifdef FBX_SUPPORT
#include <fbxsdk.h>
#endif

#ifdef _MSC_VER
#pragma pack(push,1)
#endif

#define LIBXENOVERSE_EMO_BONE_HAVE_IK    4

#define LIBXENOVERSE_EMO_IK_TYPE_EQUAL     0
#define LIBXENOVERSE_EMO_IK_TYPE_WEIGHT    1


struct SkeletonHeader
{
	uint16_t node_count;		// 0
	uint16_t ik2_count;			// 2
	uint32_t ik_count;          // 4
	uint32_t bones_offset;		// 8
	uint32_t names_offset;		// 0x0C
	uint32_t ik2_offset;		// 0x10
	uint32_t ik2_names_offset;	// 0x14
	uint32_t offset_boneExtraInfo; // 0x18   EMO_Ske_Bone_extraInfo
	uint32_t offset_absMatrices;// 0x1C   EMO_Ske_Bone_Absolute_Matrix
	uint32_t ik_data_offset;	// 0x20
	uint32_t unknow_0;			// 0x24
	uint32_t unknow_1;			// 0x28
	uint32_t unknow_2;			// 0x2C
	uint32_t unknow_3;			// 0x30
	uint32_t unknow_4;			// 0x34
	uint64_t skeletonUniqueId;  // 0x38		//like esk
} PACKED;
static_assert(sizeof(SkeletonHeader) == 0x40, "Incorrect structure size.");


struct SkeletonNode
{
	uint16_t parent_id;			// 0
	uint16_t child_id;			// 2
	uint16_t sibling_id;		// 4
	uint16_t emgIndex;			// 6		//only for Emo file (0xFFFF for other)
	uint16_t unknow_0;			// 8
	uint16_t ik_flag;			// A
	float unknow_1;				// C
	float relativeMatrix[16];	// 0x10
} PACKED;
static_assert(sizeof(SkeletonNode) == 0x50, "Incorrect structure size.");


struct EMO_Ske_Bone_extraInfo
{
	uint16_t unknow_0;		// 0
	uint16_t unknow_1;		// 2
	uint16_t unknow_2;		// 4
	uint16_t unknow_3;		// 6
} PACKED;
static_assert(sizeof(EMO_Ske_Bone_extraInfo) == 8, "Incorrect structure size.");


struct EMO_Ske_Bone_Absolute_Matrix
{
	float absoluteMatrix[16]; // 0
} PACKED;
static_assert(sizeof(EMO_Ske_Bone_Absolute_Matrix) == 0x40, "Incorrect structure size.");


#ifdef _MSC_VER
#pragma pack(pop)
#endif


class EMO_Bone;
class EMO_Skeleton;
class EMO;
class BoneSorter;



/********************************************************************************************
*									EMO_Bone												*
********************************************************************************************/
class Emo_IK_Relation											//Inverse Kinematic.
{
public:
	struct IKR_Bone
	{
		EMO_Bone* bone;
		float value;

		IKR_Bone(EMO_Bone* bone = 0, float value = 0.0f) { this->bone = bone; this->value = value; }
	};

	uint8_t unknow_0;
	uint8_t unknow_1;							//only in case of group type LIBXENOVERSE_EMO_IK_TYPE_EQUAL (case SSSS HSY.ema)
	std::vector<IKR_Bone> mListBones;

	Emo_IK_Relation() { unknow_0 = 0; };
	~Emo_IK_Relation() {};

	bool operator==(const Emo_IK_Relation &other) const { size_t nbIk = mListBones.size(); if (other.mListBones.size() != nbIk) { return false; }  for (size_t i = 0; i < nbIk; i++)  {  if ((mListBones.at(i).bone != other.mListBones.at(i).bone)|| (mListBones.at(i).value != other.mListBones.at(i).value)) { return false; }  }  return true; }
	bool operator!=(const Emo_IK_Relation &other) const { return !(*this == other); }
};


class Emo_IK_Group												//group of inverse Kinematic.
{
public:
	uint16_t type;							//type LIBXENOVERSE_EMO_IK_TYPE_WEIGHT or LIBXENOVERSE_EMO_IK_TYPE_EQUAL (case SSSS HSY.ema)

	std::vector<Emo_IK_Relation> mListIK;

	Emo_IK_Group() { type = LIBXENOVERSE_EMO_IK_TYPE_WEIGHT; };
	~Emo_IK_Group() {};

	bool operator==(const Emo_IK_Group &other) const { size_t nbIk = mListIK.size(); if (other.mListIK.size() != nbIk) { return false; } for (size_t i = 0; i < nbIk; i++) { if (mListIK.at(i) != other.mListIK.at(i)) { return false; } } return true; }
	bool operator!=(const Emo_IK_Group &other) const {return !(*this == other); }
};


class Emo_IK_Relation2						//Inverse Kinematic. in StreetFighters
{
public:
	string name;
	uint32_t unknow_0;
	std::vector<Emo_IK_Relation::IKR_Bone> mListBones;			//values are not used in this case

	Emo_IK_Relation2() { name = ""; unknow_0 = 0; };
	~Emo_IK_Relation2() {};

	bool operator==(const Emo_IK_Relation2 &other) const { size_t nbIk = mListBones.size(); if (other.mListBones.size() != nbIk) { return false; }  for (size_t i = 0; i < nbIk; i++) { if ((mListBones.at(i).bone != other.mListBones.at(i).bone) || (mListBones.at(i).value != other.mListBones.at(i).value)) { return false; } }  return true; }
	bool operator!=(const Emo_IK_Relation2 &other) const { return !(*this == other); }
};




/********************************************************************************************
*									EMO_Bone												*
********************************************************************************************/
class EMO_Bone
{
	friend class EMO_Skeleton;
	friend class EMO;
	friend class EMA;

private:
	std::string name;

	EMO_Bone *parent;
	EMO_Bone *child;
	EMO_Bone *sibling;
	uint16_t emgIndex;						//only for Emo file (0xFFFF for other)
	uint16_t unknow_0;
	uint16_t ik_flag;						//index in Children
	float unknow_1;							//may be length or witdh TODO
	size_t unk_extraInfo_0;							//with bone_extraInfo_offset
	size_t unk_extraInfo_1;
	size_t unk_extraInfo_2;
	size_t unk_extraInfo_3;
	bool haveAbsoluteMatrix;

	float relativeMatrix[16];								//relative to parent.
	float absoluteMatrix[16];

	size_t debugIndex;					//todo remove

	// METADATA - DON'T USE IN COMPARISON!
	// This is only to display information about original file.
	// This looses its meaning when the skeleton is modified
	uint32_t meta_original_offset;

	static void DecompileTransformationMatrix(TiXmlElement *root, const char *name, const float *matrix);
	static bool CompileTransformationMatrix(const TiXmlElement *root, float *matrix);
	static bool PartialCompare(const EMO_Bone *b1, const EMO_Bone *b2);


public:
	EMO_Bone(void);

	inline std::string GetName() const { return name; }
	inline uint32_t GetOriginalOffset() const { return meta_original_offset; }
	inline void GetRelativeMatrix(float *m) const { memcpy(m, relativeMatrix, sizeof(relativeMatrix));}
	inline bool GetAbsoluteMatrix(float *m) const {if (!haveAbsoluteMatrix){return false;} memcpy(m, absoluteMatrix, sizeof(absoluteMatrix)); return true; }
	void SetRelativeMatrix(float *m) { memcpy(relativeMatrix, m, sizeof(relativeMatrix)); }
	bool SetAbsoluteMatrix(float *m) { if (!haveAbsoluteMatrix) { return false; } memcpy(absoluteMatrix, m, sizeof(absoluteMatrix)); return true; }
	inline bool HasAbsoluteMatrix() const { return haveAbsoluteMatrix; }
	inline void SetHasAbsoluteMatrix(bool haveAbsoluteMatrix) { this->haveAbsoluteMatrix = haveAbsoluteMatrix; }

	inline EMO_Bone *GetParent() { return parent; }
	inline EMO_Bone *GetChild() { return child; }
	inline EMO_Bone *GetSibling() { return sibling; }
	inline unsigned short GetEmgIndex() { return emgIndex; }
	inline const EMO_Bone *GetParent() const { return parent; }
	inline const EMO_Bone *GetChild() const { return child; }
	inline const EMO_Bone *GetSibling() const { return sibling; }
	inline const unsigned short GetEmgIndex() const { return emgIndex; }
	inline void SetName(const std::string & str) { name = str; }

	void Decompile(TiXmlNode *root, bool hasExtraData) const;
	bool Compile(const TiXmlElement *root, bool hasExtraData);
	

	void readESKBone(ESKBone* esk);
	void writeESKBone(ESKBone* esk);


#ifdef FBX_SUPPORT

	FbxAMatrix GetGlobalTransform() const;

	static void FbxMatrixToArray(float *mo, const FbxMatrix *mi);
	static void FbxMatrixToArray(double *mo, const FbxMatrix *mi);
	static FbxMatrix ArrayToFbxMatrix(const float *mi);
	static FbxAMatrix ArrayToFbxAMatrix(const float *mi);

	static FbxAMatrix GetGlobalDefaultPosition(FbxNode* node);
	static void SetGlobalDefaultPosition(FbxNode* node, FbxAMatrix global_position);

#endif

	bool operator==(const EMO_Bone & rhs) const;
	inline bool operator!=(const EMO_Bone & rhs) const { return !(*this == rhs); }
};






/********************************************************************************************
*									EMO_Skeleton											*
********************************************************************************************/
class EMO_Skeleton : public EMO_BaseFile
{
friend class EmgFile;
friend class SubPart;
friend class BoneSorter;
friend class EMO;
friend class EMA;

public:
	enum ContextUse
	{
		Object,								//not in Ema
		ObjectAnimation,					//in ema
		MaterialAnimation
	};										//camera and light animation is not here because there isn't any skeleton part. 
											// (notice: in cam.Ean  there is a Node)


private:
	bool mHaveExtraBytesOnEachBone;						//case of unknow_offset_1 != 0, only camera animation don't have this.
	uint64_t skeletonUniqueId;

	uint32_t unknow_0;
	uint32_t unknow_1;
	uint32_t unknow_2;
	uint32_t unknow_3;
	uint32_t unknow_4;
	
	
protected:
	std::vector<EMO_Bone> bones;
	std::vector<Emo_IK_Group> listInverseKinematic;
	std::vector<Emo_IK_Relation2> listInverseKinematic2;
	
	ContextUse context;							//some flag differ depend of context use.
	void* contextPtr;							//with animation there is someting special on ik_flags.

public:
	EMO_Skeleton(void);
	EMO_Skeleton(uint8_t *buf, unsigned int size);
	EMO_Skeleton(const EMO_Skeleton &other) { Reset();  Copy(other); }
	virtual ~EMO_Skeleton();

	ContextUse getContext() { return context; }
	void setContext(ContextUse value) { context = value; }
	inline uint16_t GetNumBones() const { return bones.size(); }
	uint16_t BoneToIndex(EMO_Bone *bone) const;
	std::vector<EMO_Bone> &GetBones() { return bones; }
	inline EMO_Bone *GetBone(uint16_t idx);
	inline EMO_Bone *GetBone(const std::string &name);
	inline const EMO_Bone *GetBone(const std::string &name) const;
	inline bool BoneExists(const std::string &name) const { return (GetBone(name) != nullptr); }

	uint16_t AppendBone(const EMO_Bone &bone);
	uint16_t AppendBone(const EMO_Skeleton &other, const std::string &name);

	bool CloneEMO_BoneParentChild(const EMO_Skeleton &other, const std::string &bone_name, EMO_Bone **not_found = nullptr);

	virtual bool Load(const uint8_t *buf, unsigned int size) override;
	virtual uint8_t *CreateFile(unsigned int *psize, uint16_t typeFile);
	virtual uint8_t *CreateFile(unsigned int *psize, std::vector<string> listEMG, uint16_t typeFile, bool addLastPadding = true);
	virtual void setupBoneIkLinks();
	unsigned int CalculateFileSize(bool addLastPadding = true) const;
	size_t	calculIksize(bool addLastPadding = true) const;
	size_t	calculIksize2(bool addLastPadding = true) const;


	void Decompile(TiXmlNode *root) const;
	virtual TiXmlDocument *Decompile() const override;
	bool Compile(const TiXmlElement *root);
	virtual bool Compile(TiXmlDocument *doc, bool big_endian = false) override;

	virtual bool SaveSkeletonToFile(const std::string &path, bool show_error = true, bool build_path = false);
	virtual bool DecompileSkeletonToFile(const std::string &path, bool show_error = true, bool build_path = false);


	void Copy(const EMO_Skeleton &other);
	void Reset();

	static uint16_t FindBone(const std::vector<EMO_Bone *> &bones, EMO_Bone *bone, bool assert_if_not_found);
	virtual void RebuildSkeleton(const std::vector<EMO_Bone *> &old_bones_ptr);

	

	void writeEsk(ESK* esk);										//need read esk before emd, because of pointer
	void writeEsk__recursive(EMO_Bone* emoBone, std::vector<EMO_Bone> &bones, EskTreeNode* treeNode, std::vector<EskTreeNode*> &listTreeNode);


#ifdef FBX_SUPPORT

	bool ExportFbxBone(const EMO_Bone *parent, FbxNode *root_node, FbxScene *scene, std::vector<FbxNode *> &fbx_bones) const;
	bool ExportFbx(FbxScene *scene, std::vector<FbxNode *> &fbx_bones) const;
#endif

	//Debug/work: create a file for wxHexEditor, for add tag and color on section of the file. also detect if a file still have empty part (but you better have to ouput in a log file)
	void write_Coloration_Skeleton(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size, size_t startOffset_Skeleton, bool addLastPadding, std::vector<bool> &listBytesAllreadyTagged);


	inline EMO_Skeleton &operator=(const EMO_Skeleton &other){ if (this == &other) { return *this; } Copy(other); return *this; }
	bool operator==(const EMO_Skeleton &rhs) const;
	inline bool operator!=(const EMO_Skeleton &rhs) const { return !(*this == rhs); }
	inline EMO_Bone &operator[](size_t n) { return bones[n]; }
	inline const EMO_Bone &operator[](size_t n) const { return bones[n]; }
	inline EMO_Bone &operator[](const std::string &bone_name) { EMO_Bone *b = GetBone(bone_name); if (!b) { throw std::out_of_range("EMO_Bone " + bone_name + " doesn't exist."); } return *b; }
	inline const EMO_Bone &operator[](const std::string &bone_name) const { const EMO_Bone *b = GetBone(bone_name); if (!b) { throw std::out_of_range("EMO_Bone " + bone_name + " doesn't exist."); } return *b; }
	inline const EMO_Skeleton operator+(const EMO_Bone &bone) const { EMO_Skeleton new_skl = *this; new_skl.AppendBone(bone); return new_skl; }
	inline EMO_Skeleton &operator+=(const EMO_Bone &bone) { this->AppendBone(bone); return *this; }
	inline std::vector<EMO_Bone>::iterator begin() { return bones.begin(); }
	inline std::vector<EMO_Bone>::iterator end() { return bones.end(); }
	inline std::vector<EMO_Bone>::const_iterator begin() const { return bones.begin(); }
	inline std::vector<EMO_Bone>::const_iterator end() const { return bones.end(); }
};



/********************************************************************************************
*									BoneSorter												*
********************************************************************************************/
class BoneSorter
{
private:
	const EMO_Skeleton *skl;

public:
	BoneSorter(const EMO_Skeleton *skl) : skl(skl) {}
	bool operator()(EMO_Bone * const &b1, EMO_Bone * const &b2) { return (skl->BoneToIndex(b1) < skl->BoneToIndex(b2)); }
};



}

#endif // __EMO_SKELETON_H__
