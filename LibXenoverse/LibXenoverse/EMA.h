#ifndef __EMAFILE_H__
#define __EMAFILE_H__

// From emoTools of Eternity


#include <vector>
#include "EMO_common.h"
#include "EMO_Skeleton.h"
#include "EMO.h"

// "#EMA"
#define EMA_SIGNATURE	0x414D4523

#define EMA_TYPE_ANIM_Object_or_Camera	0x3
#define EMA_TYPE_ANIM_Light				0x4
#define EMA_TYPE_ANIM_Material			0x8

#define EMA_ANIM_TARGET_Object			0x0
#define EMA_ANIM_TARGET_Camera			0x1
#define EMA_ANIM_TARGET_Light			0x2
#define EMA_ANIM_TARGET_Material		0x3


#ifdef _MSC_VER
#pragma pack(push,1)
#endif

namespace LibXenoverse
{

struct EMAHeader
{
	uint32_t signature;			// 0
	uint16_t endianess_check;	// 4
	uint16_t header_size;		// 6
	uint8_t version[4];			// 8			//could be something else but difficult to see.
	uint32_t skeleton_offset;	// 0xC
	uint16_t anim_count;		// 0x10
	uint16_t type;				// 0x12			//apparently 8: materialAnim, 3: object, 4: light, , ... Todo check and complet 
	uint32_t unknow_0;			// 0x14
	uint32_t unknow_1;			// 0x18
	uint32_t unknow_2;			// 0x1C
} PACKED;
static_assert(sizeof(EMAHeader) == 0x20, "Incorrect structure size.");

struct EMAAnimationHeader
{
	uint16_t lastframeTime;		// 0				//it's like duration - 1
	uint16_t cmd_count;			// 2
	uint32_t value_count;		// 4
	uint8_t type;				// 8
	uint8_t light_unknow;		// 9
	uint16_t frame_float_size;	// A
	uint32_t name_offset;		// C
	uint32_t values_offset;		// 10
	uint32_t animNode_offsets[1];	// 14
} PACKED;
static_assert(sizeof(EMAAnimationHeader) == 0x18, "Incorrect structure size.");

struct EMAAnimationAnimNodeHeader
{
	uint16_t bone_idx;			// 0
	uint8_t transform;			// 2 // 0 -> translation, 1 -> rotation, 2 -> scale
	uint8_t transformComponent; // 3 //4bits : ? / 2bits: transform component ( a record has 0, the next 1, then 2 ) // 0 -> x, 1 -> y, 2 -> z. // so rotations is Euler angles (no 4 quaternions componentes)
	uint16_t keyframe_count;		// 4
	uint16_t indices_offset;	// 6
} PACKED;
static_assert(sizeof(EMAAnimationAnimNodeHeader) == 8, "Incorrect structure size.");


struct EMAAnimationName
{
	uint32_t unknow_0;			// 0
	uint32_t unknow_1;			// 4
	uint16_t unknow_2;			// 8
	uint8_t nbChar;				// A
} PACKED;
static_assert(sizeof(EMAAnimationName) == 0xB, "Incorrect structure size.");


#ifdef _MSC_VER
#pragma pack(pop)
#endif














class EMA;
class EmaAnimation;

struct EMAKeyframe
{
	uint16_t time;
	uint32_t index;
	uint8_t interpolation;			//0x4: isQuadraticBezier (a middle diff point for spline), 0x8: isCubicBezier (tangent start and end spline's segment), 0x: linear
	
	EMAKeyframe() { interpolation = 0; time = 0; index = 0; }
	EMAKeyframe(uint16_t time, uint32_t index) { this->time = time; this->index = index; interpolation = 0; }

	inline bool operator==(const EMAKeyframe &rhs) const { return (this->time == rhs.time && this->index == rhs.index && (this->interpolation == rhs.interpolation)); }
	inline bool operator!=(const EMAKeyframe &rhs) const { return !(*this == rhs); }

	void Decompile(TiXmlNode *root, const std::vector<float> &values, const EMAKeyframe* next_keyframe = 0) const;
	bool Compile(const TiXmlElement *root);
};






class EMAAnimationNode
{
	friend class EMA;
	friend class EmaAnimation;

private:
	uint16_t bone_idx;
	EMO_Bone *bone;
	uint8_t transform;
	uint8_t transformComponent;
	bool noInterpolation;
	size_t timesByteSize;					//oblige to keep, because some of StIv files don't respect the rule with duration.. notice the indexesByteSize respect the rule on values list.
	uint8_t unknow_0;						// at 0x8 of transformComponent
	uint8_t unknow_1;						// at 0x10 of transformComponent From StreetFighters // todo find what is for


	std::vector<EMAKeyframe> keyframes;

	//debug Todo remove TODO
	

public:
	EMAAnimationNode() { bone_idx = 0;  bone = 0; transform = 0; transformComponent = 0; noInterpolation = false; unknow_0 = unknow_1 = 0; timesByteSize = 0; }
	inline EMO_Bone *GetBone() { return bone; }
	uint16_t getBoneIndex() { return bone_idx; }
	uint8_t getTransform() { return transform; }
	uint8_t getTransformComponent() { return transformComponent; }
	float getDefaultValue(size_t anim_type);
	inline uint16_t GetNumKeyframes() const { return keyframes.size(); }
	inline bool RemoveKeyframe(uint16_t id) { if (id >= keyframes.size()) { return false; } keyframes.erase(keyframes.begin() + id); return true; }
	inline bool operator==(const EMAAnimationNode &rhs) const { if (!this->bone) { if (rhs.bone) { return false; } } else if (!rhs.bone) { return false; } if ((this->bone) && (this->bone->GetName() != rhs.bone->GetName())) { return false; } return (this->transform == rhs.transform && this->transformComponent == rhs.transformComponent && this->keyframes == rhs.keyframes); }
	inline bool operator!=(const EMAAnimationNode &rhs) const { return !(*this == rhs); }
	inline EMAKeyframe &operator[](size_t n) { return keyframes[n]; }
	inline const EMAKeyframe &operator[](size_t n) const { return keyframes[n]; }
	inline std::vector<EMAKeyframe>::iterator begin() { return keyframes.begin(); }
	inline std::vector<EMAKeyframe>::iterator end() { return keyframes.end(); }
	inline std::vector<EMAKeyframe>::const_iterator begin() const { return keyframes.begin(); }
	inline std::vector<EMAKeyframe>::const_iterator end() const { return keyframes.end(); }

	string transformNameForObjectAnim();
	string transformNameForCameraAnim();
	string transformNameForLightAnim();
	string transformNameForMaterialAnim();

	float getInterpolatedKeyframeComponent(float time, std::vector<float> &values, float default_value);
	void sort();
	void addKeyFrameAtTime(size_t frame);
	void cut(size_t indexKfStart, size_t indexKfEnd, std::vector<float> &values, float default_value, bool pushTo0 = true);
	void cleanAnimationForDuration(size_t duration, std::vector<float> &values, float default_value);

	static bool timeOrder(EMAKeyframe &a, EMAKeyframe &b) { return (a.time < b.time); }

	void Decompile(TiXmlNode *root, const std::vector<float> &values, size_t anim_type, size_t duration) const;
	bool Compile(const TiXmlElement *root, EMO_Skeleton &skl);

	void readEANKeyframedAnimation(EANKeyframedAnimation* ean, std::vector<float> &values, size_t currComponent, size_t duration, bool forceOrientationInterpolation = false);
	void writeEANKeyframe(EANKeyframe* ean, std::vector<float> &values, size_t frame, std::vector<bool> &isValuesReallyUsed);
};



//------------- Class reorganization by Node and Tranform for better work (don't have the datas)
class EMAAnimation_ByNode_ByTransform
{
	friend class EMA;
	friend class EmaAnimation;
private:
	uint8_t transform;
	
	std::vector<EMAAnimationNode*> components;

public:
	EMAAnimation_ByNode_ByTransform(uint8_t transform) { this->transform = transform; }
	uint8_t getTransform() { return transform; }
	std::vector<EMAAnimationNode*> &GetComponents() { return components; }

	EANKeyframe getInterpolatedKeyframe(float time, std::vector<float> &values, float default_x = 0, float default_y = 0, float default_z = 0, float default_w = 1);
};

class EMAAnimation_ByNode
{
	friend class EMA;
	friend class EmaAnimation;

private:
	uint16_t bone_idx;
	EMO_Bone *bone;
	
	std::vector<EMAAnimation_ByNode_ByTransform> transforms;

public:
	EMAAnimation_ByNode(uint16_t bone_idx, EMO_Bone *bone) { this->bone_idx = bone_idx;  this->bone = bone; }
	inline EMO_Bone *GetBone() { return bone; }
	uint16_t getBoneIndex() { return bone_idx; }
	std::vector<EMAAnimation_ByNode_ByTransform>  &GetTransforms() { return transforms; }
};
//------------- 




class EmaAnimation
{
	friend class EMA;
	friend class EMA_Material_Animation;
private:
	std::string name;
	uint16_t duration;
	uint8_t type;						// 0: object animations (.ema, .obj.ema, .fce.ema, .menu.ema, .bba.ema (saint seya attack, may be for enemy))
										// 1: camera animation (.cam.ema)
										// 2: for light (.light.ema)
										// 3: material animation (.mat.ema, .matbas.ema)

	uint8_t light_unknow;				//always 1 when light
	uint16_t frame_float_size;			//choose if it's 16 or 32 bytes for floats.

	uint32_t name_unknow_0;
	uint32_t name_unknow_1;
	uint16_t name_unknow_2;

	std::vector<float> values;
	std::vector<EMAAnimationNode> nodes;

	std::vector<EMAAnimation_ByNode> organizedNodes;

public:
	inline std::string GetName() const{ return name; }
	inline void SetName(const std::string &name){ this->name = name; }
	inline uint16_t GetDuration() const { return duration; }
	std::vector<EMAAnimationNode> &getNodes() { return nodes; }
	std::vector<float> &getValues() { return values; }
	std::vector<EMAAnimation_ByNode> &getOrganizedNodes() { return organizedNodes; }
	inline void SetDuration(uint16_t duration) { this->duration = duration; }
	inline uint16_t GetNumNodes() const{ return nodes.size(); }
	inline bool RemoveNode(uint16_t id) { if (id >= nodes.size()) {return false;} nodes.erase(nodes.begin() + id); return true;}
	inline bool operator==(const EmaAnimation &rhs) const { return (this->name == rhs.name && this->duration == rhs.duration && this->values == rhs.values && this->nodes == rhs.nodes); }
	inline bool operator!=(const EmaAnimation &rhs) const { return !(*this == rhs); }
	inline EMAAnimationNode &operator[](size_t n) { return nodes[n]; }
	inline const EMAAnimationNode &operator[](size_t n) const { return nodes[n]; }
	inline std::vector<EMAAnimationNode>::iterator begin() { return nodes.begin(); }
	inline std::vector<EMAAnimationNode>::iterator end() { return nodes.end(); }
	inline std::vector<EMAAnimationNode>::const_iterator begin() const { return nodes.begin(); }
	inline std::vector<EMAAnimationNode>::const_iterator end() const { return nodes.end(); }

	void buildOrganizedNodes();
	void clearOrganizedNodes() { organizedNodes.clear(); }

	void Decompile(TiXmlNode *root) const;
	bool Compile(const TiXmlElement *root, EMO_Skeleton &skl);

	void	readEANAnimation(EANAnimation* ean, ESK* esk, EMO_Skeleton* emoSkeleton, bool forceOrientationInterpolation = false);
	void	writeEANAnimation(EANAnimation* ean, ESK* esk);
};




class EMA : public EMO_Skeleton
{
private:
	string version;
	uint16_t type;						// 3: Object (also for camera anim), 4: Light, 8 materials
	uint32_t unknow_0;
	uint32_t unknow_1;
	uint32_t unknow_2;


	std::vector<EmaAnimation> animations;

public:
	EMA();
	EMA(const EMO &other) : EMO_Skeleton() { Copy(other); }
	virtual ~EMA();

	void Copy(const EMA &other);
	void Reset();

	uint16_t getType() { return type; }
	std::vector<EmaAnimation> &getAnimations() { return animations;  }
	inline uint16_t GetNumAnimations() const { return animations.size(); }
	inline bool HasSkeleton() const { return ((bones.size() > 0) && (type != EMA_TYPE_ANIM_Light) && ( (type != EMA_TYPE_ANIM_Object_or_Camera) || (animations.size()==0) || (animations.at(0).type != EMA_ANIM_TARGET_Camera)) ); }
	inline EmaAnimation *GetAnimation(uint16_t idx) { if (idx >= animations.size()) { return nullptr; } return &animations[idx]; }
	inline const EmaAnimation *GetAnimation(uint16_t idx) const { if (idx >= animations.size()) { return nullptr; } return &animations[idx]; }
	inline EmaAnimation *GetAnimation(const std::string &name) { for (EmaAnimation &a : animations) { if (a.name == name) { return &a; } } return nullptr; }
	inline const EmaAnimation *GetAnimation(const std::string &name) const { for (const EmaAnimation &a : animations) { if (a.name == name) { return &a; } } return nullptr; }
	inline bool AnimationExists(uint16_t idx) const { return (GetAnimation(idx) != nullptr); }
	inline bool AnimationExists(const std::string &name) const { return (GetAnimation(name) != nullptr); }
	inline uint16_t FindAnimation(const std::string &name) const { for (size_t i = 0; i < animations.size(); i++) { if (animations[i].name == name) { return i; } } return 0xFFFF; }
	inline bool RemoveAnimation(uint16_t idx) { if (idx >= animations.size()) { return false; } animations.erase(animations.begin() + idx); }
	inline bool RemoveAnimation(const std::string &name) { for (auto it = animations.begin(); it != animations.end(); ++it) { if ((*it).name == name) { animations.erase(it); return true; } } return false; }
	bool LinkAnimation(EmaAnimation &anim, EMO_Bone **not_found = nullptr);
	uint16_t AppendAnimation(const EmaAnimation &animation);
	uint16_t AppendAnimation(const EMA &other, const std::string &name);
	inline EMA &operator=(const EMA &other) { if (this == &other) { return *this; } Copy(other); return *this; }
	bool operator==(const EMA &rhs) const;
	inline bool operator!=(const EMA &rhs) const { return !(*this == rhs); }
	inline EmaAnimation &operator[](size_t n) { return animations[n]; }
	inline const EmaAnimation &operator[](size_t n) const { return animations[n]; }
	inline EmaAnimation &operator[](const std::string &name) { EmaAnimation *a = GetAnimation(name); if (!a) { throw std::out_of_range("Animation " + name + " doesn't exist."); } return *a; }
	inline const EmaAnimation &operator[](const std::string &name) const { const EmaAnimation *a = GetAnimation(name); if (!a) { throw std::out_of_range("Animation " + name + " doesn't exist."); } return *a; }
	inline const EMA operator+(const EmaAnimation &animation) const { EMA new_ema = *this; new_ema.AppendAnimation(animation); return new_ema; }
	inline EMA &operator+=(const EmaAnimation &animation) { this->AppendAnimation(animation); return *this; }
	inline std::vector<EmaAnimation>::iterator begin() { return animations.begin(); }
	inline std::vector<EmaAnimation>::iterator end() { return animations.end(); }
	inline std::vector<EmaAnimation>::const_iterator begin() const { return animations.begin(); }
	inline std::vector<EmaAnimation>::const_iterator end() const { return animations.end(); }


	virtual bool load(string filename) { return this->SmartLoad(filename); }
	virtual bool Load(const uint8_t *buf, unsigned int size) override;
	virtual uint8_t *CreateFile(unsigned int *psize) override;
	unsigned int CalculateFileSize() const;

	void buildOrganizedNodes();
	void clearOrganizedNodes();

	void cleanAnimations();

	virtual bool DecompileToFile(const std::string &path, bool show_error = true, bool build_path = false) override;
	virtual bool CompileFromFile(const std::string &path, bool show_error = true, bool big_endian = false) override;
	virtual TiXmlDocument *Decompile() const override;
	virtual bool Compile(TiXmlDocument *doc, bool big_endian = false) override;

	void	readEAN(EAN* ean, bool forceOrientationInterpolation = false);
	void	readEsk(ESK* esk);
	void	writeEAN(EAN* ean);
	
	void save_Coloration(string filename, bool show_error = false);		//create a file for wxHexEditor, for add tag and color on section of the file.
	void write_Coloration(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size);


protected:
	virtual void RebuildSkeleton(const std::vector<EMO_Bone *> &old_bones_ptr) override;
};


}

#endif
