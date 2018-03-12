#ifndef EMPFILE_H
#define EMPFILE_H

#include "EMO_BaseFile.h"


namespace LibXenoverse
{

#ifdef _MSC_VER
#pragma pack(push,1)
#endif

#define EMP_SIGNATURE "#EMP"







struct EMPHeader
{
	char signature[4];			// 0
	uint16_t endian;			// 4
	uint32_t version;			// 6
	uint16_t unk_00;			// A	 alway 0

	uint16_t numberSection1;	// C
	uint16_t numberSection2;	// E
	uint32_t offsetSection1;	// 10
	uint32_t offsetSection2;	// 14

	/*
	uint16_t unknow3;			// 18
	uint16_t unknow4;			// 1A
	uint16_t unknow5;			// 1C
	uint16_t unknow6;			// 1E
	*/

} PACKED;
static_assert(sizeof(EMPHeader) == 0x18, "Incorrect structure size.");



struct EMP_Section1
{
	char name[32];			// 0	name hyp : the name are on 2 line of 16 bytes

	uint8_t unk_0;			// 20
	uint8_t unk_0b;			// 21
	uint16_t unk_1;			// 22
	uint8_t section3_type;	// 24
	uint8_t section3_type_b; // 25
	uint16_t unk_3;			// 26
	uint8_t unk_4;			// 28
	uint8_t unk_4b;			// 29
	uint16_t unk_5;			// 2A
	uint8_t unk_6;			// 2C
	uint8_t unk_6b;			// 2D
	uint8_t unk_7;			// 2E
	uint8_t unk_7b;			// 2F

	uint32_t unk_8;			// 30		//seam always 1
	uint16_t unk_9;			// 34		//seam always 1
	uint16_t unk_9b;		// 36		//
	uint32_t unk_10;		// 38		//seam always 0
	uint32_t unk_11;		// 3C		//seam always 0

	float unk_12;			// 40		//seam always 0
	float unk_13;			// 44		//seam always 0
	float unk_14;			// 48		//
	uint32_t unk_15;		// 4C		//seam always 0

	float unk_16;			// 50		//seam always 0
	float unk_17;			// 54		//seam always 0
	float unk_18;			// 58		//seam always 0
	uint32_t unk_19;		// 5C		//seam always 0

	float unk_20;			// 60
	float unk_21;			// 64		//seam always 0
	float unk_22;			// 68		//seam always 0
	float unk_23;			// 6C		//seam always 0
	float unk_24;			// 70		//seam always 0
	float unk_24b;			// 74		//seam always 0
	float unk_25;			// 78		//
	uint32_t unk_25b;		// 7C		//seam always 0

	uint32_t unk_26;		// 80		//seam always 0
	float unk_27;			// 84		//seam always 0
	uint16_t unk_28;		// 88
	uint16_t numberSection4; // 8A		//lazybone Type0
	uint32_t offsetSection4; // 8C		//to add from adress of 'numberSection4'

	uint16_t numberSection5; // 90		//seam to be a number of Section5 (Type1), must check value different of 1
	uint16_t numberChildSection1;	 // 94		//
	uint32_t offsetSection5; // 95		//lazybone Type1, add start Section1
	uint32_t offsetSiblingSection1; // 98		//(add start Section1) could be sibling
	uint32_t offsetChildSection1; // 9C	//(add start Section1) could be child
} PACKED;
static_assert(sizeof(EMP_Section1) == 0xA0, "Incorrect structure size.");






struct EMP_Section2
{
	uint8_t unk_0;			// 0	
	uint8_t EmbTextureId;	// 1
	uint16_t unk_0b;			// 2	

	uint8_t unk_1;			// 4	
	uint8_t unk_2;			// 5	
	uint8_t unk_3;			// 6	
	uint8_t unk_3b;			// 7	

	uint8_t unk_4;			// 8	
	uint8_t unk_5;			// 9	
	uint16_t typeSection2_b; // A	2 = the sub-data entries i described earlier, 0 = some floats, 1 also seems to contain floats

	uint32_t noUse0;		//	depend of type
	uint32_t noUse1;		//	depend of type
	uint32_t noUse2;		//	depend of type
	uint32_t noUse3;		//	depend of type

} PACKED;
static_assert(sizeof(EMP_Section2) == 0x1C, "Incorrect structure size.");


struct EMP_Section2_b_00
{
	float color[4];		// 0	it's a RGBA color.

} PACKED;
static_assert(sizeof(EMP_Section2_b_00) == 0x10, "Incorrect structure size.");


struct EMP_Section2_b_01
{
	float color[4];		// 0	it's a RGBA color.

} PACKED;
static_assert(sizeof(EMP_Section2_b_01) == 0x10, "Incorrect structure size.");



struct EMP_Section2_b_02
{
	uint32_t unk_0;			// 0
	uint32_t unk_1;			// 4	
	uint16_t unk_2;			// 8	
	uint16_t numberSection2_02_c;	// A
	uint32_t offsetSection2_02_c;	// C

} PACKED;
static_assert(sizeof(EMP_Section2_b_02) == 0x10, "Incorrect structure size.");


struct EMP_Section2_b_02_c
{
	uint16_t unk_0;		// 0
	uint16_t unk_1;		// 2
	float color[4];		// 4	it's a RGBA color.

} PACKED;
static_assert(sizeof(EMP_Section2_b_02_c) == 0x14, "Incorrect structure size.");





struct EMP_Section3_TexturePart			//come when section3_type_b == 2
{
	uint16_t unk_0;			// 0
	uint16_t unk_1;			// 2
	uint32_t unk_2a;		// 4		
	uint32_t unk_2;			// 8
	uint32_t unk_3;			// C

	uint16_t emmValue;		// 10			//change the visual for shader
	uint16_t unk_4;			// 12			//seam always 1, may be the number of offsetSEction2.
	uint32_t offsetToOffsetToSection2; // 14	//(add start Section1)
	float unk_5;			// 18		//seam float
	float unk_6;			// 1C		//seam float

	float unk_7;			// 20		//seam float
	float unk_8;			// 24		//seam float
	float unk_9;			// 28		//seam float
	float unk_10;			// 2C		//seam float

	float color0[4];		// 30	it's a RGBA color.
	float color1[4];		// 40	it's a RGBA color.
	float color2[4];		// 50	it's a RGBA color.

	uint32_t unk_11;		// 60		//seam float
	uint32_t unk_12;		// 64		//seam float
	uint32_t unk_13;		// 68		//seam float
	uint32_t unk_14;		// 6C		//seam float

} PACKED;
static_assert(sizeof(EMP_Section3_TexturePart) == 0x70, "Incorrect structure size.");







//////////////////////////////////////////////

struct EMP_Section3_00_01
{
	float unk_0;			// 0		//could be some floats all
	float unk_1;			// 4
	float unk_2;			// 8
	float unk_3;			// C

	float unk_4;			// 10
	float unk_5;			// 14
	uint32_t unk_6;			// 18
	uint32_t unk_7;			// 1C
} PACKED;
static_assert(sizeof(EMP_Section3_00_01) == 0x20, "Incorrect structure size.");


struct EMP_Section3_01_01
{
	float unk_0;			// 0		//could be some floats all
	float unk_1;			// 4
	float unk_2;			// 8
	float unk_3;			// C
} PACKED;
static_assert(sizeof(EMP_Section3_01_01) == 0x10, "Incorrect structure size.");



struct EMP_Section3_02_01
{
	float unk_0;			// 0		//could be some floats all
	float unk_1;			// 4
	float unk_2;			// 8
	float unk_3;			// C

	float unk_4;			// 10
	float unk_5;			// 14
	float unk_6;			// 18
	float unk_7;			// 1C

	float unk_8;			// 20
	float unk_9;			// 24
	uint32_t unk_10;		// 28
	float unk_11;		// 2C
} PACKED;
static_assert(sizeof(EMP_Section3_02_01) == 0x30, "Incorrect structure size.");



struct EMP_Section3_03_01
{
	float unk_0;			// 0		//could be some floats all
	float unk_1;			// 4
	float unk_2;			// 8
	float unk_3;			// C

	float unk_4;			// 10
	float unk_5;			// 14
	float unk_6;			// 18
	float unk_7;			// 1C

	float unk_8;			// 20
	float unk_9;			// 24
	uint32_t unk_10;		// 28
	uint32_t unk_11;		// 2C
} PACKED;
static_assert(sizeof(EMP_Section3_03_01) == 0x30, "Incorrect structure size.");




//////////////////////////////////////////////
struct EMP_Section3_00_02
{
	EMP_Section3_TexturePart texturePart;	// 0

	float unk_0;		// 70		//seam float
	float unk_1;		// 74		//seam float
	float unk_2;		// 78		//seam float
	float unk_3;		// 7C		//seam float

} PACKED;
static_assert(sizeof(EMP_Section3_00_02) == 0x80, "Incorrect structure size.");





struct EMP_Section3_02_02
{
	EMP_Section3_TexturePart texturePart;	// 0

	float unk_0;		// 70		//seam float
	float unk_1;		// 74		//seam float
	float unk_2;		// 78		//seam float
	float unk_3;		// 7C		//seam float

	float unk_4;		// 80		//seam float
	float unk_5;		// 84		//seam float
	float unk_6;		// 88		//seam float
	uint32_t unk_7;		// 8C		//seam float

} PACKED;
static_assert(sizeof(EMP_Section3_02_02) == 0x90, "Incorrect structure size.");




struct EMP_Section3_03_02
{
	EMP_Section3_TexturePart texturePart;	// 0

	uint16_t unk_0;		// 70		//
	uint16_t unk_1;		// 72		//
	uint16_t unk_2;		// 74		//
	uint16_t lastIndexColors; // 76		//
	uint32_t unk_4;		// 78		//
	uint32_t offsetSection3_03_02_b; // 7C		//

} PACKED;
static_assert(sizeof(EMP_Section3_03_02) == 0x80, "Incorrect structure size.");

//EMP_Section3_03_02_b is floats series






struct EMP_Section3_04_02
{
	EMP_Section3_TexturePart texturePart;	// 0

	float unk_0;		// 70		//
	float unk_1;		// 74
	float unk_2;		// 78
	float unk_3;		// 7C
	
	uint32_t unk_4;		// 80		//
	uint32_t unk_5;		// 84
	uint32_t unk_6;		// 88		//
	uint32_t unk_7;		// 8C		//

	uint32_t unk_8;		// 90		//
	uint32_t offsetEmg; // 94		//
	uint32_t unk_9;		// 98		//
	uint32_t unk_10;	// 9C		//


} PACKED;
static_assert(sizeof(EMP_Section3_04_02) == 0xA0, "Incorrect structure size.");




struct EMP_Section3_05_02
{
	EMP_Section3_TexturePart texturePart;	// 0

	float unk_0;		// 70		//seam float
	float unk_1;		// 74		//seam float
	float unk_2;		// 78		//seam float
	float unk_3;		// 7C		//seam float

} PACKED;
static_assert(sizeof(EMP_Section3_05_02) == 0x80, "Incorrect structure size.");


//todo find a way to have the existence of EMP_Section3_05_02_b
struct EMP_Section3_05_02_b
{
	uint16_t lastIndexForFloatGroups;		// 0
	uint16_t numberFloatByGroup;	// 2
	uint32_t offsetSection3_05_02_c; // 4		//(add start Section1) the floats

} PACKED;
static_assert(sizeof(EMP_Section3_05_02_b) == 0x8, "Incorrect structure size.");

// EMP_Section3_05_02_c is the series of floats.





///////////////////////////////////////////////////////


struct EMP_Section4				//lazybone Type0
{
	uint8_t unk_0;			// 0
	uint8_t unk_1;			// 1
	uint16_t unk_2;			// 2	//seam always 0

	uint16_t unk_3;			// 4	//seam always 0
	uint16_t unk_4;			// 6	//seam always 0

	uint16_t numberIndexIntoSection4_b;	// 8
	uint16_t numberFloatIntoSection4_b;	// A
	uint32_t offsetSection4_b;	// C	// add start Section4

} PACKED;
static_assert(sizeof(EMP_Section4) == 0x10, "Incorrect structure size.");

//EMP_Section4_b / lazybone Type0 Data :
//if there is more than 2 floats (X) = > 4x 4bytes for unknow values + X floats + (204 if Type0, 80 if Type1) indexes.
//if it's 2 floats => 2x 4 bytes for unknow values  + 2 floats +  (204 if Type0 , 80 if Type1) indexes. 
//if ti's 1 floats = > 2x 4bytes for unknow values + one floats  (hyp : don't need index because all have the same value)
//if there is more than 10 floats (X) = > 16x 4bytes for unknow values + X floats + (204 if Type0, 80 if Type1) indexes.






struct EMP_Section5				//lazybone Type1 Header
{
	uint16_t unk_0;				// 0
	uint16_t numberSection5_b;	// 2	// section with float (lazybone Type1 Entry)
	uint32_t offsetSection5_b;	// 4	// add start Section5

} PACKED;
static_assert(sizeof(EMP_Section5) == 0x8, "Incorrect structure size.");


struct EMP_Section5_b				//lazybone Type1 Entry, simillar to Type0 Entry
{
	uint8_t unk_0;				// 0
	uint8_t unk_1;				// 1
	uint16_t unk_2;				// 2
	uint16_t unk_3;				// 4
	uint16_t unk_4;				// 6

	uint16_t numberIndexIntoSection5_c; 	// 8		
	uint16_t numberFloatIntoSection5_c;	  // A
	uint32_t offsetSection5_c;		  // C		//(add start Section5_b) section with floats (lazybone Type1 Data)
} PACKED;
static_assert(sizeof(EMP_Section5_b) == 0x10, "Incorrect structure size.");


//todo EMP_Section5_c quand on aura compris bien les histoire de 6400 (cf Section4_b)





#ifdef _MSC_VER
#pragma pack(pop)
#endif


class EMP : public LibXenoverse::EMO_BaseFile
{

	std::vector<std::vector<std::vector<string>>> listTagColors;	// by Section, param, and font/background.

public:
	EMP();
	virtual ~EMP() { Reset(); }

	bool Load(const uint8_t *buf, size_t size);
	void Reset();

	void save_Xml(string filename, bool show_error = false);		// Xml version for debug.
	void write_Xml(TiXmlElement *parent, const uint8_t *buf, size_t size);
	

	void save_Coloration(string filename, bool show_error = false);		//create a file for wxHexEditor, for add tag and color on section of the file.
	void write_Coloration(TiXmlElement *parent, const uint8_t *buf, size_t size);
	void write_Coloration_Tag(string paramName, string paramType, string paramComment, size_t startOffset, size_t size, string sectionName, TiXmlElement* parent, size_t idTag, size_t sectionIndex, size_t paramIndex, std::vector<bool> &listBytesAllreadyTagged, size_t sectionIndexInList = (size_t)-1, bool checkAllreadyTaggued = true);

	void extractEmd(string filename, EMD* emd);
	void replaceEmgFromEmd(string filename, EMD* emd);
};

}

#endif // EMPFILE_H
