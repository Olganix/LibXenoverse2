#ifndef SCDFILE_H
#define SCDFILE_H

#include "EMO_BaseFile.h"


namespace LibXenoverse
{

#ifdef _MSC_VER
#pragma pack(push,1)
#endif

#define SCD_SIGNATURE "#SCD"

#define SCD_CFG0_SIGNATURE "CFG0"
#define SCD_CFG1_SIGNATURE "CFG1"

#define SCD_BDY0_SIGNATURE "BDY0"
#define SCD_BDY3_SIGNATURE "BDY3"
#define SCD_BDY4_SIGNATURE "BDY4"
#define SCD_BDY5_SIGNATURE "BDY5"

#define SCD_CSR1_SIGNATURE "CSR1"
#define SCD_CSR2_SIGNATURE "CSR2"

#define SCD_ACT0_SIGNATURE "ACT0"

#define SCD_END0_SIGNATURE "END0"

struct SCD_Header
{
	char signature[4];			// 0
	uint32_t version;			// 4

	uint32_t unk_0;				// 8  always 0
	uint32_t unk_1;				// C  always 0
} PACKED;
static_assert(sizeof(SCD_Header) == 0x10, "Incorrect structure size.");



struct SCD_Part_Header
{
	char signature[4];			// 0
	uint32_t size;				// 4  size from start of SCD_Part_Header (signature included)
} PACKED;
static_assert(sizeof(SCD_Part_Header) == 0x8, "Incorrect structure size.");







///////////////////////////////////////////

struct SCD_CFG0
{
	float unk_0;			// 0
} PACKED;
static_assert(sizeof(SCD_CFG0) == 0x4, "Incorrect structure size.");


struct SCD_CFG1
{
	float unk_0;			// 0
	float unk_1;			// 0
} PACKED;
static_assert(sizeof(SCD_CFG1) == 0x8, "Incorrect structure size.");


///////////////////////////////////////////


struct SCD_BDY0
{
	uint32_t nameOffset;	// 0
	uint32_t unk_0;			// 4
	float unk_1;			// 8
	uint32_t unk_2;			// C
	uint32_t unk_3;			// 10
	
	float unk_4;			// 14
	float unk_5;			// 18
	float unk_6;			// 1C
	
	float unk_7;			// 20
	float unk_8;			// 24
	
	uint32_t  unk_9;		// 28
	float unk_10;			// 2C
	float unk_11;			// 30
	float unk_12;			// 34
	float unk_13;			// 38
	float unk_14;			// 3C
	float unk_15;			// 40
	float unk_16;			// 44
	
	float unk_17;			// 48
	float unk_18;			// 4C
	float unk_19;			// 50
	
	uint32_t unk_20;		// 54
	uint32_t unk_21;		// 58
	float unk_22;			// 5C
	uint32_t unk_23;		// 60
	uint32_t unk_24;		// 64
	uint32_t unk_25;		// 68
	uint32_t unk_26;		// 6C

	float unk_27;			// 70

	float unk_28;			// 74
	float unk_29;			// 78
	float unk_30;			// 7C

	float unk_31;			// 80
	float unk_32;			// 84
	float unk_33;			// 88

	float unk_34;			// 8C
	float unk_35;			// 90
	float unk_36;			// 94

	uint32_t unk_37;		// 98
	uint32_t unk_38;		// 9C
	uint32_t unk_39;		// A0

	float unk_40;			// A4
	float unk_41;			// A8
	float unk_42;			// AC
} PACKED;
static_assert(sizeof(SCD_BDY0) == 0xB0, "Incorrect structure size.");




struct SCD_BDY3
{
	uint32_t nameOffset;	// 0
	uint32_t unk_0;			// 4
	uint32_t unk_1;			// 8
	float unk_2;			// C
	uint32_t unk_3;			// 10
	float unk_4;			// 14
	float unk_5;			// 18
	uint32_t unk_6;			// 1C
	float unk_7;			// 20
	float unk_8;			// 24
	uint32_t  unk_9;		// 28
	uint32_t unk_10;		// 2C
	uint32_t unk_11;		// 30
	uint32_t unk_12;		// 34
	float unk_13;			// 38
	float unk_14;			// 3C
	float unk_15;			// 40
	uint32_t unk_16;		// 44
	uint32_t unk_17;		// 48
	uint32_t unk_18;		// 4C
	uint32_t unk_19;		// 50
	uint32_t unk_20;		// 54
	uint32_t unk_21;		// 58
	float unk_22;			// 5C
	uint32_t unk_23;		// 60
	float unk_24;			// 64
	float unk_25;			// 68
	float unk_26;			// 6C
	float unk_27;			// 70
	float unk_28;			// 74
	float unk_29;			// 78
	uint32_t unk_30;		// 7C
	uint32_t unk_31;		// 80
	uint32_t unk_32;		// 84
	float unk_33;			// 88
	float unk_34;			// 8C
	float unk_35;			// 90
} PACKED;
static_assert(sizeof(SCD_BDY3) == 0x94, "Incorrect structure size.");



struct SCD_BDY4
{
	uint32_t nameOffset;	// 0
	uint32_t unk_0;			// 4
	float unk_1;			// 8
	float unk_2;			// C
	float unk_3;			// 10

	float unk_4;			// 14
	float unk_5;			// 18

	uint32_t unk_6;			// 1C
	
	float unk_7;			// 20
	float unk_8;			// 24
	float unk_9;			// 28

	float unk_10;			// 2C
	float unk_11;			// 30
	float unk_12;			// 34

	float unk_13;			// 38
	float unk_14;			// 3C
	float unk_15;			// 40
	float unk_16;			// 44

	uint32_t unk_17;		// 48
	uint32_t unk_18;		// 4C
	float unk_19;			// 50
	uint32_t unk_20;		// 54

	uint32_t unk_21;		// 58
	uint32_t unk_22;		// 5C

	float unk_23;			// 60
	float unk_24;			// 64
	float unk_25;			// 68
	float unk_26;			// 6C

	float unk_27;			// 70
	float unk_28;			// 74

	float unk_29;			// 78

	float unk_30;			// 7C
	float unk_31;			// 80

	float unk_32;			// 84
	float unk_33;			// 88
	float unk_34;			// 8C
	float unk_35;			// 90
	float unk_36;			// 94
	float unk_37;			// 98

} PACKED;
static_assert(sizeof(SCD_BDY4) == 0x9C, "Incorrect structure size.");





struct SCD_BDY5
{
	uint32_t nameOffset;	// 0
	uint32_t unk_0;			// 4

	float unk_1;			// 8
	float unk_2;			// C
	float unk_3;			// 10
	
	float unk_4;			// 14
	float unk_5;			// 18
	float unk_6;			// 1C

	float unk_7;			// 20
	float unk_8;			// 24
	
	uint32_t  unk_9;		// 28

	float unk_10;			// 2C
	float unk_11;			// 30
	float unk_12;			// 34
	float unk_13;			// 38

	float unk_14;			// 3C
	float unk_15;			// 40
	float unk_16;			// 44

	float unk_17;			// 48
	float unk_18;			// 4C
	float unk_19;			// 50

	uint32_t unk_20;		// 54
	uint32_t unk_21;		// 58
	float unk_22;			// 5C
	uint32_t unk_23;		// 60

	uint32_t unk_24;		// 64
	uint32_t unk_25;		// 68

	float unk_26;			// 6C
	float unk_27;			// 70
	float unk_28;			// 74
	float unk_29;			// 78

	float unk_30;			// 7C
	float unk_31;			// 80
	float unk_32;			// 84

	float unk_33;			// 88
	float unk_34;			// 8C

	float unk_35;			// 90
	uint32_t unk_36;		// 94
	uint32_t unk_37;		// 98

	float unk_38;			// 9C
	float unk_39;			// A0
	float unk_40;			// A4
} PACKED;
static_assert(sizeof(SCD_BDY5) == 0xA8, "Incorrect structure size.");





///////////////////////////////////////////



struct SCD_CSR1						//NOTICE size could change TODO
{
	uint32_t nameOffset;	// 0
	uint32_t nameOffset_2;	// 4
	uint32_t unk_0;			// 8
	
	float unk_1;			// C
	float unk_2;			// 10
	float unk_3;			// 14

	float unk_4;			// 18
	float unk_5;			// 1C
	float unk_6;			// 20

	float unk_7;			// 24
	float unk_8;			// 28
	float unk_9;			// 2C

	float unk_10;			// 30
	float unk_11;			// 34

	float unk_12;			// 38
	float unk_13;			// 3c
	float unk_14;			// 40

	float unk_15;			// 44
	float unk_16;			// 48
	float unk_17;			// 4c

	float unk_18;			// 50
	float unk_19;			// 54
	float unk_20;			// 58

	float unk_21;			// 5C
	float unk_22;			// 60

	float unk_23;			// 64
	float unk_24;			// 68

	/*
	float unk_25;			// 6C
	float unk_26;			// 70
	float unk_27;			// 74
	
	uint32_t unk_28;		// 78
	uint32_t unk_29;		// 7C
	*/
} PACKED;
static_assert(sizeof(SCD_CSR1) == 0x6C, "Incorrect structure size.");


struct SCD_CSR2						//NOTICE size could change TODO
{
	uint32_t nameOffset;	// 0
	uint32_t nameOffset_2;	// 4
	uint32_t unk_0;			// 8
	float unk_1;			// C
	float unk_2;			// 10
	float unk_3;			// 14

	float unk_4;			// 18
	float unk_5;			// 1C
	float unk_6;			// 20

	float unk_7;			// 24
	
	float unk_8;			// 28
	float unk_9;			// 2C
	float unk_10;			// 30

	float unk_11;			// 34

	float unk_12;			// 38
	float unk_13;			// 3c
	float unk_14;			// 40

	float unk_15;			// 44
	float unk_16;			// 48
	float unk_17;			// 4c

	float unk_18;			// 50
	float unk_19;			// 54
	float unk_20;			// 58

	float unk_21;			// 5C
	float unk_22;			// 60

	float unk_23;			// 64
	float unk_24;			// 68

	float unk_25;			// 6C
	float unk_26;			// 70
	float unk_27;			// 74

	uint32_t unk_28;		// 78
	uint32_t unk_29;		// 7C

	//todo look at the end, problem of size and type.
} PACKED;
static_assert(sizeof(SCD_CSR2) == 0x80, "Incorrect structure size.");
	





///////////////////////////////////////////



struct SCD_ACT0
{
	uint32_t nameOffset;	// 0
	uint32_t nameOffset_2;	// 4
	uint32_t unk_0;			// 8
	float unk_1;			// C
	float unk_2;			// 10
	float unk_3;			// 14
	
	float unk_4;			// 18
	float unk_5;			// 1C
	float unk_6;			// 20
	
	float unk_7;			// 24
	float unk_8;			// 28
	float unk_9;			// 2C
	uint32_t unk_10;		// 30
	uint32_t unk_11;		// 34
} PACKED;
static_assert(sizeof(SCD_ACT0) == 0x38, "Incorrect structure size.");




///////////////////////////////////////////


#ifdef _MSC_VER
#pragma pack(pop)
#endif











/*-------------------------------------------------------------------------------\
|                             Scd_Part											 |
\-------------------------------------------------------------------------------*/
class Scd_Part
{
friend class Scd;

public:
	Scd_Part() { Reset();  }
	virtual ~Scd_Part() {}

	void Reset();

	/*
	bool Load(const uint8_t *buf, size_t size, size_t offsetStartTextureUnits, std::vector<Scd_TextureUnit*> &listTextureUnit);
	size_t write_Main(uint8_t *buf, size_t size);
	size_t write_AnimationsA(uint8_t *buf, size_t size);
	size_t write_AnimationsB(uint8_t *buf, size_t size);
	size_t write_TextureUnitPointers(uint8_t *buf, size_t size, size_t startOffsetSection2, std::vector<Scd_TextureUnit*> &listTextureUnit);
	size_t write_Extrude(uint8_t *buf, size_t size);
	size_t calculeFilesize();

	TiXmlElement* export_Xml(std::vector<Scd_TextureUnit*> &listTextureUnit);
	bool import_Xml(TiXmlElement* node, std::vector<Scd_TextureUnit*> &listTextureUnit);
	*/
};



/*-------------------------------------------------------------------------------\
|                             Scd												 |
\-------------------------------------------------------------------------------*/
class Scd : public LibXenoverse::EMO_BaseFile
{
	/*
	std::vector<Scd_Part> mListPart;
	std::vector<Scd_TextureUnit*> mListTextureUnit;
	*/
public:
	string originefilename;

private:
	std::vector<std::vector<std::vector<string>>> listTagColors;	// by Section, param, and font/background.
	

public:
	Scd();
	virtual ~Scd() { Reset(); }

	void Reset();
	virtual bool load(string filename) { return this->SmartLoad(filename); }
	
	/*
	bool Load(const uint8_t *buf, size_t size);
	virtual uint8_t *CreateFile(unsigned int *psize);
	size_t write(uint8_t *buf, size_t size);
	size_t calculeFilesize(bool withTextureUnits = true);


	void saveXml(string filename);
	TiXmlElement* export_Xml();
	virtual bool Compile(TiXmlDocument *doc, bool big_endian = false);
	bool import_Xml(TiXmlElement* parent);
	*/

	virtual bool Compile(TiXmlDocument *doc, bool big_endian = false);


	//debug version
	void save_Xml(string filename, bool show_error = true);
	void write_Xml(TiXmlElement *parent, const uint8_t *buf, size_t size);

	void save_Coloration(string filename, bool show_error = false);		//create a file for wxHexEditor, for add tag and color on section of the file.
	void write_Coloration(TiXmlElement *parent, const uint8_t *buf, size_t size);
	void write_Coloration_Tag(string paramName, string paramType, string paramComment, size_t startOffset, size_t size, string sectionName, TiXmlElement* parent, size_t idTag, size_t sectionIndex, size_t paramIndex, std::vector<bool> &listBytesAllreadyTagged, size_t sectionIndexInList = (size_t)-1, bool checkAllreadyTaggued = true);
};





}

#endif // SCDFILE_H
