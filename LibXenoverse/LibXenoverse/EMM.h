#ifndef LIBXENOVERSE_EMM_H_INCLUDED
#define LIBXENOVERSE_EMM_H_INCLUDED

#define LIBXENOVERSE_EMM_SIGNATURE  "#EMM"

#include "EMO_BaseFile.h"
#include "BinColorTag.h"

//notice: there is some holes on certain cases from current colorTags, but it's about a bug on game's dev's tools witch create memory garabage, on allow to not have a parameter for a material.



#define EMM_ANIMCHANNEL_MATCOL_0		0x1
#define EMM_ANIMCHANNEL_MATCOL_1		0x2
#define EMM_ANIMCHANNEL_MATCOL_2		0x4
#define EMM_ANIMCHANNEL_MATCOL_3		0x8
#define EMM_ANIMCHANNEL_TEXTSCROLL_0	0x10000
#define EMM_ANIMCHANNEL_TEXTSCROLL_1	0x20000


namespace LibXenoverse
{

struct EMM_Header
{
	char signature[4];			// 0
	uint16_t endian;			// 4
	uint16_t header_size;		// 6
	uint8_t version[4];			// 8			//could be something else but difficult to see.
	uint32_t offset_materials;	// C
} PACKED;
static_assert(sizeof(EMM_Header) == 0x10, "Incorrect structure size.");


struct EMM_Header_Sub				//depend of the size of header_size.
{
	uint32_t offset_unknowValues; // 0
	uint32_t unknow_0;			// 4			//padding
	uint32_t unknow_1;			// 8			//padding
	uint32_t unknow_2;			// C			//padding
} PACKED;
static_assert(sizeof(EMM_Header_Sub) == 0x10, "Incorrect structure size.");

struct EMM_Material_Section
{
	char name[32];				// 0
	char shaderProgName[32];	// 20
	uint16_t number_parameters;	// 40
	uint16_t unknow_0;			// 42
} PACKED;
static_assert(sizeof(EMM_Material_Section) == 0x44, "Incorrect structure size.");


struct EMM_Parameter_Section
{
	char name[32];				// 0
	uint16_t type;				// 20	// 0: Float, 1: uint.
	uint16_t level;				// 22   //0: directShaderEntries, 1: gameSystem (Material , TextureUNitState, pass composition)
	uint32_t value;				// 24	//could also be a Float32, depend of type.
} PACKED;
static_assert(sizeof(EMM_Parameter_Section) == 0x28, "Incorrect structure size.");






/*-------------------------------------------------------------------------------\
|                             EMMParameter					                     |
\-------------------------------------------------------------------------------*/
class EMMParameter
{
	public:
		string name;
		uint16_t type;
		uint16_t level;						//0: directShaderEntries, 1: gameSystem (Material , TextureUNitState, pass composition)

		size_t uint_value;
		float float_value;

		EMMParameter(void);
		EMMParameter(string name, uint16_t type = 0x1, uint16_t level = 0x0, size_t uint_value = 0, float float_value = 0.0f) { this->name = name; this->type = type; this->level = level; this->uint_value = uint_value; this->float_value = float_value; }
		~EMMParameter(void) {}

		void	read(File *file);
		void	write(File *file);
		void	readXML(TiXmlElement* xmlCurrentNode);
		void	writeXML(TiXmlElement* xmlCurrentNode);
};



/*-------------------------------------------------------------------------------\
|                             EMMMaterial					                     |
\-------------------------------------------------------------------------------*/
class EMMMaterial
{
friend class EMM;

public:
	struct Sampler2D_shaderDefinition
	{
		string name;
		size_t samplerIndex;
		size_t size;

		Sampler2D_shaderDefinition(string name, size_t samplerIndex, size_t size)
		{
			this->name = name;
			this->samplerIndex = samplerIndex;
			this->size = size;
		}
	};

protected:
	string name;
	string shaderProgramName;
	uint16_t unknow_0;
	vector<EMMParameter*> parameters;
	std::vector<Sampler2D_shaderDefinition> listSampler2D;				//list of defintion of sampler in shader. it will be used when 
			

public:
	EMMMaterial(string name = "") { this->name = name; shaderProgramName = ""; unknow_0 = 0; }
	~EMMMaterial(void);

	string	getName(void) { return string(name); }
	void	setName(string name) { this->name = name; }
	string	getShaderName(void) { return string(shaderProgramName); }
	void	setShaderName(string name) { this->shaderProgramName = name; }
	void	setUnknow_0(uint16_t value) { this->unknow_0 = value; }
	vector<EMMParameter*>	&getParameters(void) { return parameters; }
	EMMParameter*	getParameter(const string &name);
	vector<Sampler2D_shaderDefinition>	&getListSampler2D(void) { return listSampler2D; }

	void	read(File *file);
	void	write(File *file);
	void	readXML(TiXmlElement* xmlCurrentNode);
	void	writeXML(TiXmlElement* xmlCurrentNode);
};



/*-------------------------------------------------------------------------------\
|                             EMM					                             |
\-------------------------------------------------------------------------------*/
class EMM
{
	friend class EMP;

protected:
	string name;
	string version;
	uint32_t unknow_0;					//padding.
	uint32_t unknow_1;					//padding.
	uint32_t unknow_2;					//padding.	

	vector<EMMMaterial*> materials;
	std::vector<size_t> listUnknowValues;

public:
	EMM(void) { name = ""; version = "0.147.0.0"; unknow_0 = unknow_1 = unknow_2 = 0; }
	~EMM(void);

	string	getName(void) { return name; }
	void	setName(string name) { this->name = name; }
	vector<EMMMaterial *>	&getMaterials(void) { return materials; }
	EMMMaterial*	getMaterial(string name);
	std::vector<string>	getListUniqueParameters(void);

	bool	load(string filename);
	void	save(string filename, bool big_endian=false);
	bool	loadXml(string filename);
	void	saveXML(string filename);
	void	read(File *file);
	void	write(File *file);	
	void	readXML(TiXmlElement* xmlCurrentNode);
	TiXmlElement*	writeXML();

	//Debug/work: create a file for wxHexEditor, for add tag and color on section of the file. also detect if a file still have empty part (but you better have to ouput in a log file)
	void save_Coloration(string filename, bool show_error = false);
	void write_Coloration(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size);
};


}

#endif