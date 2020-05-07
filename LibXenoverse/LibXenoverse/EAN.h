#include "ESK.h"

#ifndef LIBXENOVERSE_EAN_H_INCLUDED
#define LIBXENOVERSE_EAN_H_INCLUDED

#define LIBXENOVERSE_EAN_SIGNATURE    "#EAN"

#define LIBXENOVERSE_EAN_ANIMATION_TYPE_NORMAL  0x0
#define LIBXENOVERSE_EAN_ANIMATION_TYPE_CAMERA  0x1
//hyp from ema, can't success to check. may be for full conversion with ema, I should add it
//define LIBXENOVERSE_EAN_ANIMATION_TYPE_LIGHT		0x2
//define LIBXENOVERSE_EAN_ANIMATION_TYPE_MATERIAL	0x3

#define LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION						0x0
#define LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION_or_TargetPosition	0x1
#define LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE_or_CAMERA				0x2


//Notice: Camera is x=Roll (in radian) y=Focal (Fov in radian) but Xml est in degreee to simplify.
//		  Rotation is case of animation of camera, is in reality Camera Target Position.

#include "EMO_BaseFile.h"
#include "BinColorTag.h"


namespace LibXenoverse
{


struct EAN_Header
{
	char signature[4];			// 0
	uint16_t endian;			// 4
	uint16_t header_size;		// 6
	uint8_t version[4];			// 8			//could be something else but difficult to see.
	uint32_t unknow_0;			// C
	uint8_t type;				// 10
	uint8_t unknow_1;			// 11
	uint16_t number_animations;	// 12
	uint32_t offset_skeleton;	// 14
	uint32_t offset_animations;	// 18
	uint32_t offset_animNames;	// 1C
} PACKED;
static_assert(sizeof(EAN_Header) == 0x20, "Incorrect structure size.");

//some part are come from ESK.

struct EAN_Anim_Section
{
	uint16_t unknow_0;			// 0
	uint8_t indexSize;			// 2	//to have the number of bytes for index
	uint8_t floatSize;			// 3	//to have the number of bytes for float
	uint32_t number_frames;		// 4
	uint32_t number_animNodes;	// 8	//from each bone/node
	uint32_t offset_animNodes;	// C
} PACKED;
static_assert(sizeof(EAN_Anim_Section) == 0x10, "Incorrect structure size.");

struct EAN_AnimNode_Section
{
	uint16_t bone_index;		// 0
	uint16_t number_animKfs;	// 2	//keyframed animation
	uint32_t offset_animKfs;	// 4
} PACKED;
static_assert(sizeof(EAN_AnimNode_Section) == 0x8, "Incorrect structure size.");

struct EAN_AnimKf_Section
{
	uint8_t type_flag;				// 0	//position rotation scale, cam_fov, etc ...
	uint8_t unknow_0;				// 1	//0x7 for normal, 0x3 for camera special parameters (Todo find what is it. could be interpolation, but it's don't look that) 
	uint16_t unknow_1;				// 2	//padding
	uint32_t number_Kfs;			// 4	//keyframes
	uint32_t offset_Kf_frametime;	// 8	//time in number of frame
	uint32_t offset_Kf_values;		// C	/values depend of type flag (vector4)
} PACKED;
static_assert(sizeof(EAN_AnimKf_Section) == 0x10, "Incorrect structure size.");





class EANAnimation;

/***************************** class EANKeyframe ***************************************/
class EANKeyframe
{
public:
	size_t frame;
	float x;
	float y;
	float z;
	float w;

	EANKeyframe(unsigned int frame = 0, float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
	EANKeyframe(EANKeyframe *source);
	EANKeyframe(const EANKeyframe &source);
	
	void read(File *file, unsigned char keyframe_size);
	void write(File *file, unsigned char keyframe_size);
	void readFrame(File *file, unsigned char index_size);
	void writeFrame(File *file, unsigned char index_size);

	unsigned int &getFrame() { return frame; }
	void setFrame(unsigned int value) { frame = value; }
	void setXYZW(float x, float y, float z, float w);

	void operator=(EANKeyframe *source);

	bool	importXml(TiXmlElement* xmlCurrentNode, size_t flag, size_t type);
	TiXmlElement*	exportXml(size_t flag, size_t typeAnim);
};






/***************************** class EANKeyframedAnimation ***************************************/
class EANKeyframedAnimation
{
	friend class EMAAnimationNode;
	friend class EmaAnimation;

protected:
	uint8_t flag;
	uint8_t unknow_0;						//0x7 (most), 0x3 for camera parameters, and 0x1 and 0x2 on sdbh files.
	uint16_t unknow_1;
	vector<EANKeyframe> keyframes;

public:
	EANKeyframedAnimation(unsigned int flag = 0) { this->flag = flag; unknow_0 = 0x7; unknow_1= 0; }
	EANKeyframedAnimation(EANKeyframedAnimation *source);

	vector<EANKeyframe> &getKeyframes(){ return keyframes; };

	void read(File *file, unsigned char index_size, unsigned char keyframe_size);
	size_t write(File *file, unsigned char index_size, unsigned char keyframe_size);

	void getInterpolatedFrame(unsigned int frame, float &x, float &y, float &z, float &w);
	void getFrame(EANKeyframe *keyframe, float &x, float &y, float &z, float &w);

	unsigned int getFlag() { return flag; }
	void setFlag(unsigned int flag) { this->flag = flag; }

	void operator=(EANKeyframedAnimation &source);
	void append(EANKeyframedAnimation &source, size_t delayStartFrame = 0);
	void delayTimeFrame(size_t delayStartFrame);
	void cut(size_t indexKfStart, size_t indexKfEnd, bool pushTo0 = true);
	void sort();
	void addKeyFrameAtTime(size_t frame);


	bool	importXml(TiXmlElement* xmlCurrentNode, size_t type);
	TiXmlElement*	exportXml(size_t typeAnim = LIBXENOVERSE_EAN_ANIMATION_TYPE_NORMAL);


	static bool timeOrder(EANKeyframe &a, EANKeyframe &b) { return (a.frame < b.frame); }

#ifdef LIBXENOVERSE_FBX_SUPPORT
	bool importFBXPositionAnimCurve(FbxNode *fbx_node, size_t nbframes, FbxAnimCurve* fbx_animCurve_translation_x, FbxAnimCurve* fbx_animCurve_translation_y, FbxAnimCurve* fbx_animCurve_translation_z);
	bool importFBXRotationAnimCurve(FbxNode *fbx_node, size_t nbframes, FbxAnimCurve* fbx_animCurve_rotation_x, FbxAnimCurve* fbx_animCurve_rotation_y, FbxAnimCurve* fbx_animCurve_rotation_z);
	bool importFBXScalingAnimCurve(FbxNode *fbx_node, size_t nbframes, FbxAnimCurve* fbx_animCurve_scale_x, FbxAnimCurve* fbx_animCurve_scale_y, FbxAnimCurve* fbx_animCurve_scale_z);
	bool importFBXTargetCameraPositionAnimCurve(FbxNode *fbx_node, size_t nbframes, FbxAnimCurve* fbx_animCurve_translation_x, FbxAnimCurve* fbx_animCurve_translation_y, FbxAnimCurve* fbx_animCurve_translation_z);
	bool importFBXCameraAnimCurve(FbxNode *fbx_node, size_t nbframes, FbxAnimCurve* fbx_animCurve_roll, FbxAnimCurve* fbx_animCurve_focale);
	

protected:
	void mergeListTime(FbxAnimCurve* fbx_animCurve, std::vector<FbxTime> &list_fbxtime);
	float getValueForTime(FbxAnimCurve* fbx_animCurve, FbxTime fbxtime, size_t &currentIndex, size_t nbFrames, float defaultValue);
#endif
};





/***************************** class EANAnimationNode ***************************************/
class EANAnimationNode
{
	friend class EmaAnimation;
	friend class EAN;

protected:
	unsigned short bone_index;
	vector<EANKeyframedAnimation> keyframed_animations;
	

public:
	EANAnimationNode() { this->bone_index = 0; }
	EANAnimationNode(EANAnimationNode *source);

	vector<EANKeyframedAnimation> &getKeyframed_animations(){ return keyframed_animations; }

	void read(File *file, unsigned char index_size, unsigned char keyframe_size);
	size_t write(File *file, unsigned char index_size, unsigned char keyframe_size);

	unsigned int getBoneIndex() { return bone_index; }
	void	setBoneIndex(unsigned int index) { bone_index = index; }

	bool haveKeyFrameAnimation(unsigned int flag);
	bool getInterpolatedFrame(unsigned int frame, unsigned int flag, float &x, float &y, float &z, float &w);
	void addTPoseAnimation(bool addCameraComponent = false);

	void operator=(EANAnimationNode &source);
	void append(EANAnimationNode &source, size_t delayStartFrame = 0);
	void delayTimeFrame(size_t delayStartFrame);
	void cut(size_t indexKfStart, size_t indexKfEnd, bool pushTo0 = true);
	void cleanAnimationForDuration(size_t duration);

	bool	importXml(TiXmlElement* xmlCurrentNode, ESK* esk, size_t type);
	TiXmlElement*	exportXml(ESK* esk, size_t typeAnim = LIBXENOVERSE_EAN_ANIMATION_TYPE_NORMAL);

#ifdef LIBXENOVERSE_FBX_SUPPORT
	size_t importFBXAnimationCurves(FbxNode *lNode, FbxAnimLayer* lAnimLayer, size_t boneIndex, FbxScene *scene);
#endif
};

class EAN;




/***************************** class EANAnimation ***************************************/
class EANAnimation
{
	friend class EmaAnimation;

protected:
	string name;
	uint32_t duration;
	uint8_t frame_float_size;
	EAN *parent;
	vector<EANAnimationNode> nodes;

	std::vector<string> mListFilenameOrigin;
	uint16_t unknow_0;

public:
	EANAnimation(EAN *v = NULL);
	EANAnimation(EANAnimation *source, EAN *v = NULL);
	EANAnimation(EANAnimation *source, vector<string> &listBoneFilterNames, EAN *v = NULL);			//version with a bone filter

	unsigned char	getFrame_float_size(void){ return frame_float_size; }

	void	setParent(EAN *v) {parent = v; }
	EAN*	getParent(void) { return parent; }
		

	void	read(File *file);
	void	readName(File *file);
	size_t	write(File *file);
	void	writeName(File *file);

	unsigned int	getDuration(void) { return duration; }
	void	setDuration(unsigned int value) { duration = value; }
	void	cut(size_t indexKfStart, size_t indexKfEnd = (size_t)-1);

	string	getName(void) { return name; }
	void	setName(string newName){ name = newName; }
	vector<EANAnimationNode>	&getNodes(void) { return nodes; }
	string	getFilenameOriginList_str(void);
	std::vector<string>	&getFilenameOriginList(void) { return mListFilenameOrigin; }

	void copy(EANAnimation &source, bool keepName = true);
	void append(EANAnimation &source, bool keepName = true);
	void copy(EANAnimation &source, vector<string> &listBoneFilterNames, bool keepName = true);
		
	void	changeBoneIndexes(const std::vector<std::vector<size_t>> &listMovingindex);
	void	addBoneAnimationFromAnotherEan(EANAnimation &ean_Anim_src, std::vector<std::vector<size_t>> &listRelationIndex, bool matchDuration);
	void	addTPoseAnimation(ESK* skeleton, bool addCameraComponent = false);

	void operator=(EANAnimation &source);


	bool	importXml(TiXmlElement* xmlCurrentNode, EAN* eanParent);
	TiXmlElement*	exportXml(size_t typeAnim = LIBXENOVERSE_EAN_ANIMATION_TYPE_NORMAL, string version = "", bool isLast = false);

#ifdef LIBXENOVERSE_FBX_SUPPORT
	void	importFBXAnimation(FbxScene *scene, FbxAnimStack *lAnimStack, ESK* skeleton, bool allowCamera = false);
#endif
};






/***************************** class EAN ***************************************/
class EAN
{
	friend class EMA;

protected:
	string name;
	string version;
	uint32_t unknow_0;					//padding ?
	uint8_t type;						//0 for object, 1 for camera.
	uint8_t unknow_1;

	ESK* skeleton;
	vector<EANAnimation> animations;

public:
	EAN(void) { name = ""; version = "0.0.0.0"; skeleton = NULL; unknow_0 = 0; type = 0; unknow_1 = 4; }
	virtual ~EAN(void);

	bool	load(string filename);
	void	save(string filename, bool big_endian = false);

	void	read(File *file);
	void	write(File *file);

	void	merge(EAN *ean);

	string	getName(void) { return name; }
	void	setName(string name) { this->name = name; }
	size_t	getType(void) { return type; }
	void	setType(size_t type) { this->type = (uint8_t)type; }
	void	setSkeleton(ESK* skel) { skeleton = skel; }
	ESK*	getSkeleton(void){ return skeleton; }
	vector<EANAnimation>	&getAnimations(void) { return animations; }

	void	changeBoneIndexes(const std::vector<std::vector<size_t>> &listMovingindex);
	void	addBoneAnimationFromAnotherEan(EAN* ean_src, std::vector<std::vector<size_t>> &listRelationIndex, bool matchDuration);
	void	addTPoseAnimation(bool addCameraComponent = false);
	void	removeAnimation(string name);

	bool	loadXml(string filename);
	void	saveXml(string filename);
	bool	importXml(TiXmlElement* xmlCurrentNode);
	TiXmlElement*	exportXml(void);

#ifdef LIBXENOVERSE_FBX_SUPPORT
	void	importFBXAnimations(FbxScene *scene, std::vector<FbxAnimStack *> list_AnimStack);
	vector<FbxAnimCurveNode *>  exportFBXAnimations(FbxScene *scene, std::vector<FbxAnimStack *> list_AnimStack, ESK::FbxBonesInstance_DBxv* fbxBoneInst);

	FbxAnimCurveNode *createFBXAnimationCurveNode(FbxNode *fbx_node, EANAnimation *animation, EANAnimationNode* anim_node, FbxAnimStack *lAnimStack, FbxAnimLayer* lAnimLayer, FbxScene *scene, vector<FbxAnimCurveNode *> &fbx_anims);
#endif

	//Debug/work: create a file for wxHexEditor, for add tag and color on section of the file. also detect if a file still have empty part (but you better have to ouput in a log file)
	void save_Coloration(string filename, bool show_error = false);
	void write_Coloration(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size);
};

}

#endif