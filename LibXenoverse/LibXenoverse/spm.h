#ifndef SPMFILE_H
#define SPMFILE_H

#include "EMO_BaseFile.h"
#include "BinColorTag.h"



namespace LibXenoverse
{

#ifdef _MSC_VER
#pragma pack(push,1)
#endif

#define SPM_SIGNATURE_M "#SPM"
#define SPM_SIGNATURE_E "#SPE"
#define SPM_SIGNATURE_X "#SPX"
#define DSC_SIGNATURE "#DSC"


struct SPMHeader_M				//first version, signature #SPE
{
	char signature[4];			// 0
	char name[4];				// 4
	
	uint32_t number_Section1;	// 8
	uint32_t offset_Section1;	// c
} PACKED;
static_assert(sizeof(SPMHeader_M) == 0x10, "Incorrect structure size.");
//notice , there is a version before SPM, with no tag and name, just 00 at the same place.


struct SPMHeader_E				//second version, signature #SPE
{
	char signature[4];			// 0
	uint32_t version;			// 4

	uint32_t number_Section1;	// 8
	uint32_t size_Section1;		// C
	uint32_t offset_Section1;	// 10
} PACKED;
static_assert(sizeof(SPMHeader_E) == 0x14, "Incorrect structure size.");


struct SPMHeader_X				// version E with DSC file extension, , signature #SPX
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
	uint16_t godRayAndSunHalo;// 1c
	uint16_t unk_7;			// 1e
	

	float unk_8_a_0[4];		// 20

	float lightDirection_x;	// 30
	float lightDirection_y;	// 34
	float lightDirection_z;	// 38
	float lightDirection_w;	// 3c

	float unk_8_a_1[19];	// 40

	uint32_t unk_8_a_1_a;	// EC

	float unk_8_a_1_b[23];	// 40

	uint32_t unk_8_a_1_c;	// EC

	float unk_8_a_1_d[4];	// F0

	float fogColor_r;		// 100
	float fogColor_g;		// 104
	float fogColor_b;		// 108
	float fogColor_a;		// 10c

	float fog_startDist;	// 110
	float fog_endDist;		// 114

	float unk_8_a_2;		// 118
	float colorSaturation;	// 11c

	float mutiplyColor_r;	// 120
	float mutiplyColor_g;	// 124
	float mutiplyColor_b;	// 128

	float filterColor_r;	// 12c
	float filterColor_g;	// 130
	float filterColor_b;	// 134
	
	float additiveColor_r;	// 138
	float additiveColor_g;	// 13c
	float additiveColor_b;	// 140

	float filterHotCold;	// 144
	
	float unk_8_a_3[5];		// 148

	float mutiplyInverseFactor;// 15c

	float blur_startDist;	// 160
	
	float unk_8_a_4;		// 164

	uint16_t halo_alphaRejection;// 168
	uint16_t unk_8_c;		// 16a



	float unk_8_d_0;		// 16c
	float shadow_endAngle;	// 170
	float unk_8_d_1;		// 174
	float shadow_startAngle;// 178

	float sunFactor;		// 17c
	float sunFactor_hiden;	// 180
	float sunSize;			// 184
	
	float unk_8_d_2;		// 188
	float unk_8_d_3;		// 18c

	float highIlluminated_glareInverseFactor;//190

	float background_glare_additiveColor_r;	//194
	float background_glare_additiveColor_g;	//198
	float background_glare_additiveColor_b;	//19c
	float background_glare_additiveColor_a;	//1a0

	float unk_8_d_4[6];		// 1a4

	float character_glare_additiveColor_r;	// 1bc
	float character_glare_additiveColor_g;	// 1c0
	float character_glare_additiveColor_b;	// 1c4

	uint32_t unk_8_e;			// 1C8

	float edgeStrokes_factor;	// 1CC
	float edgeStrokes_width;	// 1D0

	float edgeStrokes_color_r;	// 1D4
	float edgeStrokes_color_g;	// 1D8
	float edgeStrokes_color_b;	// 1Dc
	
	float unk_8_f[4];		// 1E0

	uint32_t unk_8_f_1;		// 1F0
	float unk_8_f_2[3];		// 1F4

	uint32_t unk_8_f_3;		// 200
	float unk_8_f_4[3];		// 204

	uint32_t unk_8_f_5;		// 210
	float unk_8_f_6[3];		// 214

	uint16_t unk_9;			// 220
	uint16_t unk_10;		// 222
	uint16_t unk_11;		// 224			// it's a id for map (switch between underwater and air in BFlnd) TODO
	uint16_t unk_12;		// 226
	uint16_t unk_13;		// 228
	uint16_t unk_14;		// 22a
	uint16_t unk_15;		// 22c
	uint16_t unk_16;		// 22e

	float unk_17_0[4];		// 230

	float backgroundSpecifics_multiplycolor_r;	// 240
	float backgroundSpecifics_multiplycolor_g;	// 244
	float backgroundSpecifics_multiplycolor_b;	// 248
	float backgroundSpecifics_multiplycolor_a;	// 24C

	float unk_17_1[4];			// 250

	float shadowDirection_x;	// 260
	float shadowDirection_y;	// 264
	float shadowDirection_z;	// 268

	float unk_17_2[13];			// 26C

	float shadows_filterColor_r;// 2A0
	float shadows_filterColor_g;// 2A4
	float shadows_filterColor_b;// 2A8

	float unk_17_3;				// 2Ac

	float sunGodRay_color_r;	// 2B0
	float sunGodRay_color_g;	// 2B4
	float sunGodRay_color_b;	// 2B8
	float sunGodRay_intensity;	// 2Bc

	float limitShadow_high_start;// 2c0
	float limitShadow_high_end;	 // 2c4
	float limitShadow_radius_start;// 2c8
	float limitShadow_radius_end;// 2cc

	
	float unk_17_4[8];		// 2d0

	float sunGodRay_polardistance;			// 2F0
	uint32_t sunGodRay_unk;					// 2F4
	float sunGodRay_fading_distance;		// 2F8
	float sunGodRay_fading_attenuationFactor;//2Fc
	float sunGodRay_fading_subdivision;		// 300
	float sunGodRay_repetition_angle;		// 304
	float sunGodRay_repetition_subdivision;	// 308

	float unk_17_5;							// 30C

} PACKED;
static_assert(sizeof(SPM_Section1) == 0x310, "Incorrect structure size.");



struct SPM_Section1_E				//extension of section1, differences from #SPM
{
	uint16_t unk_18;		// 0
	uint16_t unk_19;		// 2
	uint16_t unk_20;		// 4
	uint16_t unk_21;		// 6
	uint16_t unk_22;		// 8
	uint16_t unk_23;		// a
	uint16_t unk_24;		// c
	uint16_t unk_25;		// e

	uint32_t characterBorderStroke_enable;	// 10
	float characterBorderStroke_farWidth;	// 14
	float characterBorderStroke_nearWidth;	// 18
	float characterBorderStroke_startDist;	// 1c
	float characterBorderStroke_endDist;	// 20
	
	uint8_t characterBorderStroke_color_r;	// 24
	uint8_t characterBorderStroke_color_g;	// 25
	uint8_t characterBorderStroke_color_b;	// 26
	uint8_t characterBorderStroke_color_a;	// 27

	uint32_t unk_26_0;		// 28
	float unk_26_1[5];		// 2C
	uint32_t unk_26_2;		// 40
	float unk_26_3[2];		// 44
	uint32_t unk_26_4;		// 4c
} PACKED;
static_assert(sizeof(SPM_Section1_E) == 0x50, "Incorrect structure size.");





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

public:
	Spm() { }
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

	//Debug/work: create a file for wxHexEditor, for add tag and color on section of the file. also detect if a file still have empty part (but you better have to ouput in a log file)
	void save_Coloration(string filename, bool show_error = false);
	void write_Coloration(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size);
};





}

#endif // SPMFILE_H
