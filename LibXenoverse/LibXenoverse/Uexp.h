
#ifndef LIBXENOVERSE_UEXP_H_INCLUDED
#define LIBXENOVERSE_UEXP_H_INCLUDED

#define LIBXENOVERSE_BNDFLL_SIGNATURE    "BNDFLL"


//#define LIBXENOVERSE_EAN_ANIMATION_TYPE_NORMAL  0x0

#include "EMO_BaseFile.h"
#include "BinColorTag.h"


namespace LibXenoverse
{


struct UEXP_Header
{
	uint32_t uexpType;			// 0	//apparently id!=7 are not the same.. may be 0x7 is for package resources (cf infos de uexpTypeData pour le cas 0x7 ) Todo check.
	uint32_t unknow0;			// 4
	uint32_t uexpTypeData;		// 8	//apparently 0x0 is for bndFll, 0x4 for textures, 0xA for movies (todo check)
} PACKED;
static_assert(sizeof(UEXP_Header) == 0xC, "Incorrect structure size.");


struct UEXP_Header_0x7
{
	uint32_t sizeUexp;			// 0
	uint32_t unknow2;			// 4
} PACKED;
static_assert(sizeof(UEXP_Header_0x7) == 0x8, "Incorrect structure size.");


//between, there is a the name "BNDFLL\0" + padding to 4	=> Todo check if all Uexp have the BNDFF tags

struct BNDFF_Header
{
	uint32_t unknow0;			// 0	//may be a nb of something (group of files ? ) Todo search
	uint32_t nbFiles;			// 4
	uint32_t offsetFilesHeaders;// 8
	uint32_t sizeFilesHeaders;	// C
	uint32_t unknow1;			// 10
	uint32_t unknow2;			// 14
	uint32_t offsetFilesDatas;	// 18
	uint32_t unknow3;			// 1C
	uint32_t unknow4;			// 20	//also the offsetFilesHeaders ? strange => Todo confirm it's always the same .
	uint32_t unknow5;			// 24
} PACKED;
static_assert(sizeof(BNDFF_Header) == 0x28, "Incorrect structure size.");


struct BNDFF_FileHeader					//todo check if the size is always the same , else reduce at mininum + padding
{
	uint32_t fileId;			// 0
	uint32_t offsetSubHeader;	// 4	//todo check
	uint32_t sizeFile;			// 8
	uint32_t unknow0;			// C
} PACKED;
static_assert(sizeof(BNDFF_FileHeader) == 0x10, "Incorrect structure size.");

struct BNDFF_FileSubHeader
{
	uint32_t sizeFile;			// 0	//why having both time ? => Todo check it's always the same.
	uint32_t unknow0;			// 4
	uint32_t unknow1;			// 8	//seam a important value.
	uint32_t useId;				// C	//seam to be the same id by extension of the file, so it's for iddentify how to use it. Todo check.
	uint32_t offsetName;		// 10
	uint32_t unknow2;			// 14
	uint32_t offsetNextHeader;	// 18	//no need to use normally (=> may be if the file are truely not ordered as the package want ? Todo check)
	uint32_t unknow3;			// 1C
	uint32_t offsetFileData;	// 20	//relative to BNDFF_Header.offsetFilesDatas
	uint32_t unknow4;			// 24
} PACKED;
static_assert(sizeof(BNDFF_FileSubHeader) == 0x28, "Incorrect structure size.");

// File Names
// File Datas
// uint32_t UEXP_FinaleId , witch is the same as the start of Uasset.















/***************************** class Uexp_Bndfll_File ***************************************/
class Uexp_Bndfll_File
{
friend class Uexp_Bndfll;
friend class Uexp;

protected:
	string name;
	uint32_t fileId;
	uint32_t useId;

	uint32_t sizeFile;
	byte* fileDatas;
	
	uint32_t unknow0_a;
	uint32_t unknow0;
	uint32_t unknow1;
	uint32_t unknow2;
	uint32_t unknow3;
	uint32_t unknow4;

public:
	Uexp_Bndfll_File() { name = "";  sizeFile = 0; fileDatas = 0; unknow0_a = unknow0 = unknow1 = unknow2 = unknow3 = unknow4 = 0; }
	~Uexp_Bndfll_File() { if (fileDatas) { delete fileDatas; } fileDatas = 0; }

	void read(File *file, size_t startOffsetSub, size_t startOffsetFilesDatas);
	void write(File *file, size_t startOffsetSub, size_t startOffsetFilesDatas, size_t offsetFileDatas, size_t offsetFileNames, bool islast);
	size_t	getFileSize();

	bool load(string filename);
	bool save(string filename);

	bool	importXml(TiXmlElement* xmlCurrentNode);
	TiXmlElement*	exportXml();
};





/***************************** class Uexp_Bndfll ***************************************/
class Uexp_Bndfll
{
friend class Uexp;

protected:
	uint32_t unknow0;
	uint32_t unknow1;
	uint32_t unknow2;
	uint32_t unknow3;
	uint32_t unknow4;
	uint32_t unknow5;

	vector<Uexp_Bndfll_File*> files;

	
public:
	Uexp_Bndfll() {  };
	~Uexp_Bndfll() { for (size_t i = 0, nb = files.size(); i < nb; i++) delete files[i]; files.clear(); }
	
	void	read(File *file, size_t startOffsetSub);
	void	write(File *file, size_t startOffsetSub);
	size_t	getFileSize();

	vector<Uexp_Bndfll_File*>	&getFiles(void) { return files; }

	bool	importXml(TiXmlElement* xmlCurrentNode);
	TiXmlElement*	exportXml();
};






/***************************** class Uexp ***************************************/
class Uexp
{
	friend class UassetUexp;

protected:
	string name;

	uint32_t uexpType;
	uint32_t uexpTypeData;
	string typeDataName;

	uint32_t unknow0;
	uint32_t unknow2;

	Uexp_Bndfll* bndfll;

	uint32_t uexp_finaleId;					//should be the same as first id in .uasset

	//debug in Xml
	string tagName;

	bool debugAdded;					//Todo remove.

public:
	Uexp(void) { name = ""; uexpType = 0x7; uexpTypeData = 0x0; typeDataName = ""; unknow0 = 0; unknow2 = 0; bndfll = 0; uexp_finaleId = 0xC1832A9E; debugAdded= false;  }
	virtual ~Uexp(void);

	bool	load(string filename);
	void	save(string filename, bool big_endian = false);
	void	read(File *file);
	void	write(File *file);
	size_t	getFileSize();

	void	extract(string folder);
	void	addFile(string filename);
	void	addFolder(string folder);
	void	updateUassetFile(string filename);

	bool	loadXml(string filename);
	void	saveXml(string filename);
	bool	importXml(TiXmlElement* xmlCurrentNode);
	TiXmlElement*	exportXml(void);

	//Debug/work: create a file for wxHexEditor, for add tag and color on section of the file. also detect if a file still have empty part (but you better have to ouput in a log file)
	void save_Coloration(string filename, bool show_error = false);
	void write_Coloration(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size);
};

}

#endif