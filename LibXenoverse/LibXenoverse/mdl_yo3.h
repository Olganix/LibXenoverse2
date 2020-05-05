#ifndef MDL_FILE_H
#define MDL_FILE_H

#include "EMO_BaseFile.h"
#include "BinColorTag.h"

// # Yu-Gi-Oh! Online 3: Duel Accelerator
//Todo remove the this file.

namespace LibXenoverse
{

#ifdef _MSC_VER
#pragma pack(push,1)
#endif

#define MDL_SIGNATURE "‰YO3"

#define MDL_TYPE_Material	0x2
#define MDL_TYPE_Texture	0x3
#define MDL_TYPE_Model		0xA
#define MDL_TYPE_Material_USE 0xC
#define MDL_TYPE_Shape		0x14
#define MDL_TYPE_Submesh	0x19
#define MDL_TYPE_Animation	0x28
#define MDL_TYPE_Animation_Bone	0x29
#define MDL_TYPE_Bone		0x46
//todo check definitive type.



#define MDL_Animation_Tranform_Orientation	0x0
#define MDL_Animation_Tranform_Scale		0x1
#define MDL_Animation_Tranform_Position		0x2
#define MDL_Animation_Tranform_unknow		0x9
//from 0x28
#define MDL_Animation_Tranform_position2	0x0
#define MDL_Animation_Tranform_position3	0x1
#define MDL_Animation_Tranform_unknow2		0x2

#define MDL_TypeFace_Triangle		0x3
#define MDL_TypeFace_Quad			0x4
#define MDL_TypeFace_Pentaogone		0x5
#define MDL_TypeFace_Hexagone		0x6

struct Mdl_Header
{
	char signature[4];			// 0
	uint16_t unk0;				// 4
	uint16_t unk1;				// 6
	uint16_t unk2;				// 8
} PACKED;
static_assert(sizeof(Mdl_Header) == 0xA, "Incorrect structure size.");

struct Mdl_BlockHeader
{
	uint8_t type;			// 0
	uint8_t nbNameChar;		// 1
} PACKED;
static_assert(sizeof(Mdl_BlockHeader) == 0x2, "Incorrect structure size.");




struct Mdl_Material
{
	uint32_t ambientColor;	// 0
	uint32_t difuseColor;	// 4
	uint32_t specularColor;	// 8
} PACKED;
static_assert(sizeof(Mdl_Material) == 0xC, "Incorrect structure size.");




struct Mdl_AnimationHeader
{
	uint8_t unk0;			// 0
	uint8_t transform;		// 1
	uint16_t nbFrames;		// 2
} PACKED;
static_assert(sizeof(Mdl_AnimationHeader) == 0x4, "Incorrect structure size.");


struct Mdl_Animation_Kf_Orientation
{
	uint16_t frameIndex;	// 0
	float quatW;			// 2
	float quatX;			// 6
	float quatY;			// a
	float quatZ;			// e
} PACKED;
static_assert(sizeof(Mdl_Animation_Kf_Orientation) == 0x12, "Incorrect structure size.");

struct Mdl_Animation_Kf_Scale_or_Position
{
	uint16_t frameIndex;	// 0
	float PosX;				// 2
	float PosY;				// 6
	float PosZ;				// a
} PACKED;
static_assert(sizeof(Mdl_Animation_Kf_Scale_or_Position) == 0xe, "Incorrect structure size.");



struct Mdl_Animation_Kf_Unkow
{
	uint16_t frameIndex;	// 0
	uint8_t value;			// 2
} PACKED;
static_assert(sizeof(Mdl_Animation_Kf_Unkow) == 0x3, "Incorrect structure size.");


struct Mdl_Animation_Kf_Unkow2
{
	uint16_t frameIndex;	// 0
	float value;			// 2
} PACKED;
static_assert(sizeof(Mdl_Animation_Kf_Unkow2) == 0x6, "Incorrect structure size.");





#ifdef _MSC_VER
#pragma pack(pop)
#endif












/*-------------------------------------------------------------------------------\
|                             Mdl												 |
\-------------------------------------------------------------------------------*/
class Mdl : public LibXenoverse::EMO_BaseFile
{
public:
	string originefilename;	

public:
	Mdl() { }
	virtual ~Mdl() { Reset(); }

	void Reset() {}

	
	//Debug/work: create a file for wxHexEditor, for add tag and color on section of the file. also detect if a file still have empty part (but you better have to ouput in a log file)
	void save_Coloration(string filename, bool show_error = false);
	void write_Coloration(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size);
};





}

#endif // SPMFILE_H
