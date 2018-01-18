#ifndef ETRFILE_H
#define ETRFILE_H

#include "EMO_BaseFile.h"


namespace LibXenoverse
{

#ifdef _MSC_VER
#pragma pack(push,1)
#endif

#define ETR_SIGNATURE "#ETR"







struct ETRHeader
{
	char signature[4];			// 0
	uint16_t endian;			// 4
	uint32_t version;			// 6
	uint16_t unk_00;			// A	 alway 0

	uint16_t nbSection1;		// C
	uint16_t nbSection2;		// E
	uint32_t offsetSection1;	// 10
	uint32_t offsetSection2;	// 14
} PACKED;
static_assert(sizeof(ETRHeader) == 0x18, "Incorrect structure size.");




///////////////////////////////////////////

struct ETR_Section1
{
	char boneName[32];		// 0	boneName to attach to. hyp : the name are on 2 line of 16 bytes as it's clear in emp (but not here because of lot of 0 after)

	uint32_t unk_0_0;		// 20	alway 0
	uint32_t unk_0_1;		// 24	alway 0
	uint32_t unk_0_2;		// 28	alway 0
	uint32_t unk_0_3;		// 2C	alway 0
	uint32_t unk_0_4;		// 30	alway 0
	uint32_t unk_0_5;		// 34	alway 0
	uint32_t unk_0_6;		// 38	alway 0
	uint32_t unk_0_7;		// 3C	alway 0
	float unk_0_8;			// 40	alway 0
	float unk_0_9;			// 44	alway 0
	float unk_0_10;			// 48	alway 0
	float unk_0_11;			// 4C	alway 0
	float unk_0_12;			// 50	alway 0
	float unk_0_13;			// 54	alway 0

	uint8_t unk_1;			// 58	
	uint8_t numberPointsExtrudePathSection3;	// 59	
	uint16_t unk_1b;		// 5A	
	
	uint8_t unk_2;			// 5C	
	uint8_t unk_2b;			// 5D	
	uint16_t unk_3;			// 5E

	uint8_t unk_4;			// 60
	uint8_t unk_4b;			// 61
	uint16_t unk_5;			// 62

	uint32_t unk_6;			// 64
	float unk_7;			// 68

	uint16_t emmMaterialId;	// 6C
	
	uint16_t numberSection2;// 6E	seam always 1, so may be a count of a subSection
	uint32_t offsetSection2;// 70	offset (add with the start of the current Section1), to another offset witch target one of section2.
	uint32_t offsetSection3;// 74	offset (add start Section1) to target a Section3.

	float colorStart[4];	// 78	it's a RGBA color which modify the final color (source: Cero). I extrapolate that the color start, and the second line is a color end.
	float colorEnd[4];		// 88


	float unk_9;			// 98
	uint32_t numberSegmentsExtrudeShapeSection3;		// 9C
	uint16_t unk_11;		// A0
	uint16_t numberSection4; // A2
	uint32_t offsetSection4; // A4	

	uint16_t unk_13;		// A8	seam always 1, so may be a count of a subSection
	uint16_t numberSection5; // AA
	uint32_t offsetSection5;// AC	Start at unk_13

} PACKED;
static_assert(sizeof(ETR_Section1) == 0xB0, "Incorrect structure size.");





///////////////////////////////////////////


struct ETR_Section2
{
	uint8_t unk_0;			// 0	alway 0
	uint8_t embTextureId;	// 1	
	
	uint8_t unk_1;			// 2	seam alway 0x22
	uint8_t unk_1b;			// 3	
	uint32_t unk_2;			// 4	always 0

	uint16_t unk_3;			// 8	always 0
	uint16_t typeSection2_b; // A	2 = the sub-data entries i described earlier, 0 = some floats, 1 also seems to contain floats
	
	uint32_t noUse0;		// C	depend of type
	uint32_t noUse1;		// 10	depend of type
	uint32_t noUse2;		// 14	depend of type
	uint32_t noUse3;		// 18	depend of type
} PACKED;
static_assert(sizeof(ETR_Section2) == 0x1C, "Incorrect structure size.");



struct ETR_Section2_b_00
{
	float color[4];		// 0	it's a RGBA color.

} PACKED;
static_assert(sizeof(ETR_Section2_b_00) == 0x10, "Incorrect structure size.");


struct ETR_Section2_b_01
{
	float color[4];		// 0	it's a RGBA color.

} PACKED;
static_assert(sizeof(ETR_Section2_b_01) == 0x10, "Incorrect structure size.");



struct ETR_Section2_b_02
{
	uint32_t unk_0;			// 0
	uint32_t unk_1;			// 4	
	uint16_t unk_2;			// 8	
	uint16_t numberSection2_02_c;	// A
	uint32_t offsetSection2_02_c;	// C

} PACKED;
static_assert(sizeof(ETR_Section2_b_02) == 0x10, "Incorrect structure size.");


struct ETR_Section2_b_02_c
{
	uint32_t duration;		// 0
	float color[4];		// 4	it's a RGBA color.

} PACKED;
static_assert(sizeof(ETR_Section2_b_02_c) == 0x14, "Incorrect structure size.");







///////////////////////////////////////////

//Section3: seam to be :
// - 4 floats for a direction/upVector
// - some points with 4 floats (XY pos + UV) for path of extrude.
// - some point with 2 floats (XYpos) for making a shape to extrude.





///////////////////////////////////////////

struct ETR_Section4
{
	uint8_t unk_0;				// 0
	uint8_t unk_0b;				// 1
	uint16_t nbSection4_b;		// 2	
	uint32_t offsetSection4_b;	// 4	add start section4
} PACKED;
static_assert(sizeof(ETR_Section4) == 0x8, "Incorrect structure size.");



struct ETR_Section4b
{
	uint8_t unk_0;				// 0
	uint8_t unk_0b;				// 1
	uint8_t component;			// 2
	uint8_t unk_1;				// 3
	uint16_t unk_2;				// 4
	uint16_t numberKeyFrames;	// 6 and Floats
	uint32_t offsetKeyFrames;	// 8 add start section4b
	uint32_t offsetFloats;		// C add start section4b
} PACKED;
static_assert(sizeof(ETR_Section4b) == 0x10, "Incorrect structure size.");



///////////////////////////////////////////

struct ETR_Section5
{
	uint8_t unk_0;				// 0
	uint8_t unk_0b;				// 1
	uint8_t component;			// 2
	uint8_t unk_1;				// 3
	uint16_t unk_2;				// 4
	uint16_t numberKeyFrames;	// 6  and Floats
	uint32_t offsetKeyFrames;	// 8  add start section5
	uint32_t offsetFloats;		// C  add start section5
} PACKED;
static_assert(sizeof(ETR_Section5) == 0x10, "Incorrect structure size.");




#ifdef _MSC_VER
#pragma pack(pop)
#endif












/*-------------------------------------------------------------------------------\
|                             Etr_AnimationA					                 |
\-------------------------------------------------------------------------------*/
class Etr_AnimationA					//Section5
{
friend class Etr_Part;
friend class Etr_AnimationB;

public:
	struct Keyframe
	{
		int mTime;			//in frames
		float mValue;
		Keyframe(int time, float value) { mTime = time; mValue = value; }
	};

	enum StartAnimation
	{
		SA_EndWithNoInterpolation = 0,
		SA_FirstMiddlePoint,
		SA_Start,
	};

private:
	StartAnimation mStartAnimation;
	size_t unk_0b;
	size_t unk_1;
	size_t unk_1b;
	size_t mComponent;
	size_t unk_2;


	std::vector<Keyframe> mListKeyframe;

public:
	Etr_AnimationA() { Reset(); }
	virtual ~Etr_AnimationA() {}

	void Reset();
	bool Load(const uint8_t *buf, size_t size);
	size_t write_Main(uint8_t *buf, size_t size);
	size_t write_KeyframeTimeindex(uint8_t *buf, size_t size);
	size_t write_KeyframeValues(uint8_t *buf, size_t size);
	size_t write_EmptyIndexes(uint8_t *buf, size_t size);
	size_t calculeFilesize();

	TiXmlElement* export_Xml();
	bool import_Xml(TiXmlElement* node);
};


/*-------------------------------------------------------------------------------\
|                             Etr_AnimationB					                 |
\-------------------------------------------------------------------------------*/
class Etr_AnimationB					//Section4
{
friend class Etr_Part;

private:
	size_t unk_0;
	size_t unk_0b;
	std::vector<Etr_AnimationA> mAnimations;

public:
	Etr_AnimationB() { Reset();  }
	virtual ~Etr_AnimationB() { }

	void Reset();
	bool Load(const uint8_t *buf, size_t size);
	size_t write_Main(uint8_t *buf, size_t size);
	size_t write_Animations(uint8_t *buf, size_t size);
	size_t calculeFilesize();

	TiXmlElement* export_Xml();
	bool import_Xml(TiXmlElement* node);
};



/*-------------------------------------------------------------------------------\
|                             Etr_TextureUnit					                 |
\-------------------------------------------------------------------------------*/
class Etr_TextureUnit
{
friend class Etr;

public:
	struct Keyframe
	{
		size_t timeIndex;
		float scroll_u, scroll_v, scale_u, scale_v;

		Keyframe(size_t timeIndex = 0, float scroll_u = 0.0, float scroll_v = 0.0, float scale_u = 1.0, float scale_v = 1.0) { this->timeIndex = timeIndex; this->scroll_u = scroll_u; this->scroll_v = scroll_v; this->scale_u = scale_u; this->scale_v = scale_v; }
	};

	enum TypeRepetition
	{
		TR_Wrap2 = 0,
		TR_Wrap,
		TR_Mirror,
		TR_Border,
	};

	enum TypeFiltering
	{
		TF_None = 0,
		TR_None2,
		TR_Linear,
	};
	
private:
	size_t unk_0;
	TypeRepetition mTypeRepetition_U;
	TypeRepetition mTypeRepetition_V;
	TypeFiltering mMinFiltering;
	TypeFiltering mMaxFiltering;
	size_t unk_2;
	size_t unk_3;

	size_t mEmbTextureId;	

	//here, it's use Keyframe or speed, but not in the same time
	bool mUseSpeed;
	float mScrollSpeed_U;
	float mScrollSpeed_V;

	size_t unk_2_0;			// for type 2
	size_t unk_2_1;			// for type 2
	size_t unk_2_2;			// for type 2
	std::vector<Keyframe> mListKeyframe; //only one key frame for type 0 and 1 (don't use unk values, because there is only one keyframe)

public:
	Etr_TextureUnit() { Reset();  }
	virtual ~Etr_TextureUnit() {}

	void Reset();
	bool Load(const uint8_t *buf, size_t size);
	size_t write_Main(uint8_t *buf, size_t size);
	size_t write_Animation(uint8_t *buf, size_t size);
	size_t calculeFilesize();


	TiXmlElement* export_Xml();
	bool import_Xml(TiXmlElement* node);
};


/*-------------------------------------------------------------------------------\
|                             Etr_Part											 |
\-------------------------------------------------------------------------------*/
class Etr_Part
{
friend class Etr;

public:
	struct Point
	{
		float x, y, z, w;		//z and w are for strangeparameter for Path, like the EndPoint
		Point(float x, float y, float z = 0, float w = 0) { this->x = x; this->y = y; this->z = z; this->w = w; }
	};

private:
	size_t unk_0_0;
	size_t unk_0_1;
	size_t unk_0_2;
	size_t unk_0_3;
	size_t unk_0_4;
	size_t unk_0_5;
	size_t unk_0_6;
	size_t unk_0_7;
	
	float mPositionOffset_X;
	float mPositionOffset_Y;
	float mPositionOffset_Z;

	float mRotation_Pitch;	// in degrees after loading
	float mRotation_Yaw;
	float mRotation_Roll;

	size_t unk_1;
	size_t mTimeStartExtrude;
	size_t unk_2;
	size_t mNumberFrameForNextSegment;
	size_t mDurationExtrude;
	bool unk_4_b[8];		//bits composition, the left bit is the first bool
	bool unk_4b_b[8];		//bits composition
	bool unk_5_b[8];		//bits composition
	size_t unk_6;


	bool mAutoOrientation;		//unk_4_b[0]
	bool mNoDegrade;			//unk_4_b[2], between Color1 and Color2.
	bool mDoublePointOfMiddleSection; //unk_4_b[3]
	bool mDisplayMiddleSection;	//unk_4b_b[0]
	bool mUVPauseOnExtrude;		//unk_4b_b[1]
	bool mUVPauseOnHold;		//unk_5_b[0]

	float mPositionExtrude_Z;
	float mScaleShape;
	int mDurationHold;
	size_t unk_13;



	string mAttachedBoneName;
	size_t mEmmMaterialId;

	float mColor0[4];
	float mColor1[4];


	std::vector<Etr_TextureUnit*> mListTextureUnit;		//section2

	float mEndExtrude_WorldScale_factor;					//section3
	float mEndExtrude_WorldScale_add;
	float mEndExtrude_WorldOffset_factor;
	float mEndExtrude_WorldOffset_factorb;
	std::vector<Point> mExtrudePath;			//like EndPoint
	std::vector<Point> mExtrudeLineShape;

	std::vector<Etr_AnimationB>	mListAnimationB;		//section4
	std::vector<Etr_AnimationA>	mListAnimationA;		//section5
	
public:
	Etr_Part() { Reset();  }
	virtual ~Etr_Part() {}

	void Reset();
	bool Load(const uint8_t *buf, size_t size, size_t offsetStartTextureUnits, std::vector<Etr_TextureUnit*> &listTextureUnit);
	size_t write_Main(uint8_t *buf, size_t size);
	size_t write_AnimationsA(uint8_t *buf, size_t size);
	size_t write_AnimationsB(uint8_t *buf, size_t size);
	size_t write_TextureUnitPointers(uint8_t *buf, size_t size, size_t startOffsetSection2, std::vector<Etr_TextureUnit*> &listTextureUnit);
	size_t write_Extrude(uint8_t *buf, size_t size);
	size_t calculeFilesize();

	TiXmlElement* export_Xml(std::vector<Etr_TextureUnit*> &listTextureUnit);
	bool import_Xml(TiXmlElement* node, std::vector<Etr_TextureUnit*> &listTextureUnit);
};



/*-------------------------------------------------------------------------------\
|                             Etr												 |
\-------------------------------------------------------------------------------*/
class Etr : public LibXenoverse::EMO_BaseFile
{
	std::vector<Etr_Part> mListPart;
	std::vector<Etr_TextureUnit*> mListTextureUnit;


	std::vector<std::vector<std::vector<string>>> listTagColors;	// by Section, param, and font/background.

public:
	Etr();
	virtual ~Etr() { Reset(); }

	void Reset();
	virtual bool load(string filename) { return this->SmartLoad(filename); }
	bool Load(const uint8_t *buf, size_t size);
	virtual uint8_t *CreateFile(unsigned int *psize);
	size_t write(uint8_t *buf, size_t size);
	size_t calculeFilesize(bool withTextureUnits = true);


	void saveXml(string filename);
	TiXmlElement* export_Xml();
	virtual bool Compile(TiXmlDocument *doc, bool big_endian = false);
	bool import_Xml(TiXmlElement* parent);



	void save_Coloration(string filename, bool show_error = false);		//create a file for wxHexEditor, for add tag and color on section of the file.
	void write_Coloration(TiXmlElement *parent, const uint8_t *buf, size_t size);
	void write_Coloration_Tag(string paramName, string paramType, string paramComment, size_t startOffset, size_t size, string sectionName, TiXmlElement* parent, size_t idTag, size_t sectionIndex, size_t paramIndex, std::vector<bool> &listBytesAllreadyTagged, size_t sectionIndexInList = (size_t)-1, bool checkAllreadyTaggued = true);
};





}

#endif // ETRFILE_H
