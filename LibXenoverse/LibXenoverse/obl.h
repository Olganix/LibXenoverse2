#ifndef OBLFILE_H
#define OBLFILE_H

#include "EMO_BaseFile.h"


namespace LibXenoverse
{

#ifdef _MSC_VER
#pragma pack(push,1)
#endif

#define OBL_SIGNATURE "#OBL"


struct OBLHeader
{
	char signature[4];			// 0
	uint16_t endian;			// 4
	uint16_t headerSize;		// 6
} PACKED;
static_assert(sizeof(OBLHeader) == 0x8, "Incorrect structure size.");


///////////////////////////////////////////

struct OBL_Section
{
	uint32_t unk_0;		// 0
	uint32_t unk_1;		// 4
	uint32_t unk_2;		// 8
	uint32_t unk_3;		// C
} PACKED;
static_assert(sizeof(OBL_Section) == 0x10, "Incorrect structure size.");




#ifdef _MSC_VER
#pragma pack(pop)
#endif











/*-------------------------------------------------------------------------------\
|                             Obl_Part											 |
\-------------------------------------------------------------------------------*/
class Obl_Part
{
friend class Obl;
	
	size_t unk_0;
	size_t unk_1;
	size_t unk_2;
	size_t unk_3;

public:
	Obl_Part(size_t unk_0 = 0xFFFFFFFF, size_t unk_1 = 0xFFFFFFFF, size_t unk_2 = 0xFFFFFFFF, size_t unk_3 = 0xFFFFFFFF) { this->unk_0 = unk_0; this->unk_1 = unk_1; this->unk_2 = unk_2; this->unk_3 = unk_3; }
	virtual ~Obl_Part() {}

	void Reset();
};



/*-------------------------------------------------------------------------------\
|                             Obl												 |
\-------------------------------------------------------------------------------*/
class Obl : public LibXenoverse::EMO_BaseFile
{
	std::vector<std::vector<Obl_Part>> mListPart;

public:
	Obl();
	virtual ~Obl() { Reset(); }

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
};

}

#endif // OBLFILE_H
