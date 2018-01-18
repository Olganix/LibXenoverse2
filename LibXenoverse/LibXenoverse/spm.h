#ifndef SPMFILE_H
#define SPMFILE_H

#include "EMO_BaseFile.h"


namespace LibXenoverse
{

#ifdef _MSC_VER
#pragma pack(push,1)
#endif

#define SPM_SIGNATURE "#SPX"
#define SPM_SIGNATURE_E "#SPE"
#define DSC_SIGNATURE "#DSC"



struct SPMHeader_X
{
	char signature[4];			// 0
	uint32_t version;			// 4

	uint32_t number_Section1;	// 8
	uint32_t size_Section1;		// C
	uint32_t offset_Section1;	// 10

	uint32_t number_Section2;	// 14
	uint32_t offset_Section2;	// 18
} PACKED;
static_assert(sizeof(SPMHeader_X) == 0x1C, "Incorrect structure size.");


struct SPMHeader_E				//without Section2, depend of the Signature.
{
	char signature[4];			// 0
	uint32_t version;			// 4

	uint32_t number_Section1;	// 8
	uint32_t size_Section1;		// C
	uint32_t offset_Section1;	// 10
} PACKED;
static_assert(sizeof(SPMHeader_E) == 0x14, "Incorrect structure size.");







///////////////////////////////////////////

struct SPM_Section1
{
	char name[16];			// 0

	uint16_t lenFlareEnabled; // 10
	uint16_t unk_1;			// 12
	uint16_t unk_2;			// 14
	uint16_t unk_3;			// 16
	uint16_t unk_4;			// 18
	uint16_t unk_5;			// 1a
	uint16_t unk_6;			// 1c
	uint16_t unk_7;			// 1e
	
	float unk_8_a[82];		// 20
	uint16_t unk_8_b;		// 
	uint16_t unk_8_c;		// 
	float unk_8_d[45];		// 
	
	uint16_t unk_9;			// 220
	uint16_t unk_10;		// 222
	uint16_t unk_11;		// 224
	uint16_t unk_12;		// 226
	uint16_t unk_13;		// 228
	uint16_t unk_14;		// 22a
	uint16_t unk_15;		// 22c
	uint16_t unk_16;		// 22e

	float unk_17[56];		// 230

	uint16_t unk_18;		// 310
	uint16_t unk_19;		// 312
	uint16_t unk_20;		// 314
	uint16_t unk_21;		// 316
	uint16_t unk_22;		// 318
	uint16_t unk_23;		// 31a
	uint16_t unk_24;		// 31c
	uint16_t unk_25;		// 31e


	// ...

} PACKED;
static_assert(sizeof(SPM_Section1) == 0x320, "Incorrect structure size.");



///////////////////////////////////////////


struct SPM_Section2
{
	uint32_t offset_DscFile; // 0
} PACKED;
static_assert(sizeof(SPM_Section2) == 0x4, "Incorrect structure size.");

// + snap 64 bits




///////////////////////////////////////////
struct DSC_Header
{
	char signature[4];			// 0
	uint32_t number_Section1;	// 4
	uint32_t unk_0;				// 8
	uint32_t unk_1;				// C
	uint32_t offset_Section1;	// 10
	uint32_t unk_3;				// 14		//always 0
} PACKED;
static_assert(sizeof(DSC_Header) == 0x18, "Incorrect structure size.");


///////////////////////////////////////////
struct DSC_Section1
{
	uint32_t offset_Section1_b;	// 0
	uint32_t unk_0;				// 4		//always 0
} PACKED;
static_assert(sizeof(DSC_Section1) == 0x8, "Incorrect structure size.");


///////////////////////////////////////////
struct DSC_Section1_b
{
	char name[16];					// 0
	uint32_t number_Section1_c;		// 10
	uint32_t unk_0;					// 14	// always 0
	uint32_t size_Section1_c;		// 18
	uint32_t unk_1;					// 1C	// always 0
	uint32_t offset_Section1_c;		// 20
	uint32_t unk_2;					// 24	// always 0
	uint32_t index;					// 28
	uint32_t unk_3;					// 2C	// always 0

} PACKED;
static_assert(sizeof(DSC_Section1_b) == 0x30, "Incorrect structure size.");


///////////////////////////////////////////
struct DSC_Section1_c
{
	uint32_t unk_0;					// 0
	uint32_t unk_1;					// 4	// 0xffffffff for none
	uint32_t unk_2;					// 8
	uint32_t unk_3;					// C
	uint32_t unk_4;					// 10	

	float color[3];					// 14			//only RGB
} PACKED;
static_assert(sizeof(DSC_Section1_c) == 0x20, "Incorrect structure size.");







#ifdef _MSC_VER
#pragma pack(pop)
#endif











/*-------------------------------------------------------------------------------\
|                             Spm_Part											 |
\-------------------------------------------------------------------------------*/
class Spm_Part
{
friend class Spm;

public:
	Spm_Part() { Reset();  }
	virtual ~Spm_Part() {}

	void Reset();

	/*
	bool Load(const uint8_t *buf, size_t size, size_t offsetStartTextureUnits, std::vector<Spm_TextureUnit*> &listTextureUnit);
	size_t write_Main(uint8_t *buf, size_t size);
	size_t write_AnimationsA(uint8_t *buf, size_t size);
	size_t write_AnimationsB(uint8_t *buf, size_t size);
	size_t write_TextureUnitPointers(uint8_t *buf, size_t size, size_t startOffsetSection2, std::vector<Spm_TextureUnit*> &listTextureUnit);
	size_t write_Extrude(uint8_t *buf, size_t size);
	size_t calculeFilesize();

	TiXmlElement* export_Xml(std::vector<Spm_TextureUnit*> &listTextureUnit);
	bool import_Xml(TiXmlElement* node, std::vector<Spm_TextureUnit*> &listTextureUnit);
	*/
};



/*-------------------------------------------------------------------------------\
|                             Spm												 |
\-------------------------------------------------------------------------------*/
class Spm : public LibXenoverse::EMO_BaseFile
{
	/*
	std::vector<Spm_Part> mListPart;
	std::vector<Spm_TextureUnit*> mListTextureUnit;
	*/
public:
	string originefilename;

private:
	std::vector<std::vector<std::vector<string>>> listTagColors;	// by Section, param, and font/background.
	

public:
	Spm();
	virtual ~Spm() { Reset(); }

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

#endif // SPMFILE_H
