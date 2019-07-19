#ifndef LIBXENOVERSE_ESK_H_INCLUDED
#define LIBXENOVERSE_ESK_H_INCLUDED

#define LIBXENOVERSE_ESK_SIGNATURE    "#ESK"


namespace LibXenoverse
{

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
	unsigned short index_4;						//index in Children
	bool haveTransformMatrix;
	bool mVisible;
	float transform_matrix[16];
	float skinning_matrix[12];
	
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
	void	readSkinningMatrix(File *file);
	void	writeSkinningMatrix(File *file);
	
	void	mergeMatrix(ESKBone *eskBone);	
	void	mergeFilenameOrigin(ESKBone *eskBone);
	
	void	test_calculTransformMatrixFromSkinningMatrix(std::vector<ESKBone *> &listBones, bool recursive = false);
	void	calculTransformMatrixFromSkinningMatrix(std::vector<ESKBone *> &listBones, bool recursive = false);
	void	calculSkinningMatrixFromTransformMatrix(std::vector<ESKBone *> &listBones, bool recursive = false);

	string	getSkinningMatrixDebug(void);
	string	getTransformMatrixDebug(void);

	
	EskTreeNode*	importXml(TiXmlElement* xmlCurrentNode, std::vector<ESKBone*> &bones, EskTreeNode* treeParentNode, bool haveExtraBytesOnEachBone);
	TiXmlElement*	exportXml(EskTreeNode* EskTreeNode, bool haveExtraBytesOnEachBone);
			
	static	void	displayMatrix4x4(double* transformMatrix);
	static	void	transpose4x4(double* transformMatrix);
	static	void	makeTransform4x4(double* posOrientScaleMatrix, double* resultTransformMatrix);	//posOrientScaleMatrix is 3x4, orient is a quaternion informations, resultTransformMatrix is 4x4
	static	void	decomposition4x4(double* transformMatrix, double* resultPosOrientScaleMatrix); //posOrientScaleMatrix is 3x4, orient is a quaternion informations, TransformMatrix is 4x4
	static	void	inverse4x4(double* transformMatrix, double* inversedTransformMatrix);
	static	void	concatenate4x4(double* transformMatrix_left, double* transformMatrix_right, double* resultTransformMatrix);
};



/*-------------------------------------------------------------------------------\
|                             ESK					                             |
\-------------------------------------------------------------------------------*/
class ESK
{
	friend class EMO_Skeleton;
	friend class EMO;
	friend class EMA;

protected:
	string name;
	unsigned short flag;
	unsigned int unknown_offset_2;
	unsigned int unknown_offset_3;
	bool mHaveExtraBytesOnEachBone;						//case of unknown_offset_1 != 0, only camera animation don't have this.

	vector<ESKBone*> bones;
	std::vector<Esk_IK_Group> listInverseKinematic;

public:
			
	ESK(void);
	virtual ~ESK(void);

	bool	load(string filename);
	bool	load(File *file, string s_name);
	void	save(string filename, bool big_endian = false);
	void	save(File *file, bool big_endian = false);

	void	read(File *file);
	unsigned int	getWriteSize(bool withTransformMatrix = true);					//size of byte to record skeleton with write function.
	size_t	calculIksize();
	void	write(File *file, bool withTransformMatrix = true);

	void	merge(ESK *esk);
	void	merge_byAttach(ESK *esk);
	void	merge_byAttach_recursive(EskTreeNode* skeleton_tree_toAdd, EskTreeNode* skeleton_tree);
	void	clone(ESK *esk);
			
			
	string	getName(void) { return name; }
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

};

}

#endif