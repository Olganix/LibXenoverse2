
/*
	
	After lot of work to try to find the binary structur of havok version v2015,
	Alpha find a projet witch was just arrived on Git witch take care of havok v2016 in a pythin script.
	So, thank to TagTools project of blueskythlikesclouds (https://github.com/blueskythlikesclouds/TagTools)
	I have done the adaptations for read and write the havok 2015 with no differences.

*/




#ifndef HavokFILE_H
#define HavokFILE_H

#include "EMO_BaseFile.h"


namespace LibXenoverse
{


#ifdef _MSC_VER
#pragma pack(push,1)
#endif


#define Havok_TAG0_SIGNATURE "TAG0"
#define Havok_SDKV_SIGNATURE "SDKV"
#define Havok_v2015_SIGNATURE "2015"
#define Havok_major01_SIGNATURE "01"
#define Havok_minor00_SIGNATURE "00"
#define Havok_DATA_SIGNATURE "DATA"
#define Havok_TYPE_SIGNATURE "TYPE"
#define Havok_TSTR_SIGNATURE "TSTR"
#define Havok_TNAM_SIGNATURE "TNAM"
#define Havok_FSTR_SIGNATURE "FSTR"
#define Havok_TBOD_SIGNATURE "TBOD"
#define Havok_TPAD_SIGNATURE "TPAD"
#define Havok_INDX_SIGNATURE "INDX"
#define Havok_ITEM_SIGNATURE "ITEM"


struct Havok_PartHeader
{
	uint32_t size;				// 0	// not considere the 2 left bits. Size considere the header included.
	char signature[4];			// 4
} PACKED;
static_assert(sizeof(Havok_PartHeader) == 0x8, "Incorrect structure size.");


struct Havok_Version
{
	char year[4];				// 0
	char major[2];				// 4
	char minor[2];				// 6
} PACKED;
static_assert(sizeof(Havok_Version) == 0x8, "Incorrect structure size.");




////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////



#ifdef _MSC_VER
#pragma pack(pop)
#endif



class Havok;
class Havok_TagType;
class Havok_TagMember;
class Havok_TagTemplate;
class Havok_TagInterface;
class Havok_TagItem;
class Havok_TagObject;



/*-------------------------------------------------------------------------------\
|                             Havok_TagMember					                 |
\-------------------------------------------------------------------------------*/
class Havok_TagMember
{
public:

	string name;
	size_t flags;
	size_t byteOffset;
	Havok_TagType* type;

	Havok_TagMember(string name = "v", size_t flags = 0, size_t byteOffset = 0, Havok_TagType* type = 0) { this->name = name; this->flags = flags; this->byteOffset = byteOffset;  this->type = type; }

	TiXmlElement* exportXml(std::vector<Havok_TagType*> &listType);
	size_t importXml(TiXmlElement* node);
};


/*-------------------------------------------------------------------------------\
|                             Havok_TagTemplate					                 |
\-------------------------------------------------------------------------------*/
class Havok_TagTemplate
{
public:

	string name;
	size_t value;
	Havok_TagType* value_ptr;					//if isType, target a Havok_TagType

	Havok_TagTemplate(string name = "v", size_t value = 0) { this->name = name; this->value = value; value_ptr = 0; }
	bool isInt() { return (name.substr(0, 1) == "v"); }
	bool isType() { return (name.substr(0, 1) == "t"); }

	TiXmlElement* exportXml();
	void importXml(TiXmlElement* node);
};

/*-------------------------------------------------------------------------------\
|                             Havok_TagInterface				                 |
\-------------------------------------------------------------------------------*/
class Havok_TagInterface
{
public:

	Havok_TagType* type;
	size_t value;

	Havok_TagInterface(Havok_TagType* type = 0, size_t value = 0) { this->type = type, this->value = value; }

	TiXmlElement* exportXml(std::vector<Havok_TagType*> &listType);
	size_t importXml(TiXmlElement* node);
};


/*-------------------------------------------------------------------------------\
|                             Havok_TagType						                 |
\-------------------------------------------------------------------------------*/
class Havok_TagType
{
public:

	size_t id;
	string name;

	Havok_TagType* parent;
	Havok_TagType* pointer;

	size_t version;
	size_t flags;
	size_t subTypeFlags;

	size_t byteSize;
	size_t alignment;
	size_t abstractValue;

	std::vector<Havok_TagTemplate*> listTemplate;
	std::vector<Havok_TagMember*> members;
	std::vector<Havok_TagInterface> interfaces;


	Havok_TagType(size_t id, string name = "", Havok_TagType* parent = 0, size_t flags = 0, size_t subTypeFlags = 0, Havok_TagType* pointer = 0, size_t version = 0, size_t byteSize = 0, size_t alignment = 0, size_t abstractValue = 0) { this->id = id;  this->name = name; this->parent = parent, this->flags = flags; this->subTypeFlags = subTypeFlags; this->pointer = pointer; this->version = version; this->byteSize = byteSize; this->alignment = alignment; this->abstractValue = abstractValue; }
	~Havok_TagType() { Reset(); }
	void Reset(void);
	
	Havok_TagType* superType();
	size_t subType();
	std::vector<Havok_TagMember*> allMembers();
	size_t tupleSize() { return (subTypeFlags >> 8); }

	TiXmlElement* exportXml(std::vector<Havok_TagType*> &listType);
	void importXml(TiXmlElement* node);
	void importXml_secondStep(TiXmlElement* node, std::vector<Havok_TagType*> &listType);

};

/*-------------------------------------------------------------------------------\
|                             Havok_TagItem						                 |
\-------------------------------------------------------------------------------*/
class Havok_TagItem
{
public:

	Havok_TagType* type;
	size_t offset;
	size_t count;
	bool isPtr;
	std::vector<Havok_TagObject*>  value;

	size_t xmlTaglevel;

	Havok_TagItem(size_t xmlTaglevel = 0, Havok_TagType* type = 0, size_t offset = 0, size_t count = 0, bool isPtr = false) { this->xmlTaglevel = xmlTaglevel;  this->type = type; this->offset = offset; this->count = count; this->isPtr = isPtr; }
	static bool offsetOrder(Havok_TagItem* a, Havok_TagItem* b) { return (a->offset < b->offset); }
	static bool levelOrder(Havok_TagItem* a, Havok_TagItem* b) { return (a->xmlTaglevel < b->xmlTaglevel); }
	void remove(Havok_TagObject* obj);

	TiXmlElement* exportXml();
	bool importXml(TiXmlElement* node);
};



/*-------------------------------------------------------------------------------\
|                             Havok_TagObject					                 |
\-------------------------------------------------------------------------------*/
class Havok_TagObject
{
public:

	bool b_value;
	string s_value;
	long long i_value;
	float f_value;

	Havok_TagObject* objectPointer;
	std::vector<Havok_TagObject*> listObjectString;
	std::vector<Havok_TagObject*> listObjectClass;
	std::vector<Havok_TagObject*> listObjectArray;
	std::vector<Havok_TagObject*> listObjectTuple;

	Havok_TagType* type;
	Havok_TagItem* attachement;							//for rebuild

	Havok_TagObject(Havok_TagType* type = 0) { this->type = type; b_value = false; s_value = ""; i_value = 0; f_value = 0.0f; attachement = 0; objectPointer = 0; }
	~Havok_TagObject() { Reset();  }
	void Reset();
	Havok_TagObject* clone();

	TiXmlElement* exportXml(string name = "");
	bool importXml(TiXmlElement* node, std::vector<Havok_TagType*> &listType, std::vector<Havok_TagItem*> &listItems, Havok_TagItem* parentAttachement = 0, size_t addToItemList = 0, size_t level = 0);

#ifdef LIBXENOVERSE_FBX_SUPPORT
	bool	importFBX(FbxNode *fbxNode, std::vector<string> &xmlPath, string typeName);
	void	exportFBX(string basename, FbxScene *scene, FbxNode* parentNode = 0);
	bool	exportFBX_CheckHave3dData();					//check if object or child have something knowed. is to avoid useless Node creation.
#endif

};




/*-------------------------------------------------------------------------------\
|                             Havok					                             |
\-------------------------------------------------------------------------------*/
class Havok : public LibXenoverse::EMO_BaseFile
{
public:
	enum TagSubType
	{
		TST_Void = 0x0,
		TST_Invalid = 0x1,
		TST_Bool = 0x2,
		TST_String = 0x3,
		TST_Int = 0x4,
		TST_Float = 0x5,
		TST_Pointer = 0x6,
		TST_Class = 0x7,
		TST_Array = 0x8,
		TST_Tuple = 0x28,
		TST_TypeMask = 0xff,
		TST_IsSigned = 0x200,
		TST_Float32 = 0x1746,
		TST_Int8 = 0x2000,
		TST_Int16 = 0x4000,
		TST_Int32 = 0x8000,
		TST_Int64 = 0x10000,
	};

	enum TagFlag
	{
		TF_SubType = 0x1,
		TF_Pointer = 0x2,
		TF_Version = 0x4,
		TF_ByteSize = 0x8,
		TF_AbstractValue = 0x10,
		TF_Members = 0x20,
		TF_Interfaces = 0x40,
		TF_Unknown = 0x80,
	};



private:
	string version;							//only "2015.01.00" accepted

	Havok_TagObject* rootObject;
	std::vector<Havok_TagType*> listType;
	
	std::vector<Havok_TagItem*> listItem;			//list of item is not really necessary (because we could find with analyze object hierarchy), but it's for making binary in the same order than original. Todo find order from object hierrachy and remove it




public:
	Havok();
	virtual ~Havok() { Reset(); }

	void Reset();
	virtual bool load(string filename) { return this->SmartLoad(filename); }
	bool Load(const uint8_t *buf, size_t size);
	virtual uint8_t *CreateFile(unsigned int *psize);
	

	Havok_TagObject* readObject(size_t index, const uint8_t *buf, size_t size, Havok_TagType* type, size_t &offset, std::vector<Havok_TagItem*> &listItem, std::vector<Havok_TagType*> &listType, Havok_TagItem* parentAttachement = 0);
	void writeObject(Havok_TagObject* obj, std::vector<byte> &listBytesData, std::vector<Havok_TagItem*> &listItem, std::vector<Havok_TagType*> &listType);
	std::vector<Havok_TagObject*> readItemPtr(const uint8_t *buf, size_t size, size_t &offset, std::vector<Havok_TagItem*> &listItem, std::vector<Havok_TagType*> &listType, bool indexInversed = false);
	uint32_t readPacked(const uint8_t *buf, size_t size, size_t &nbBytes);				//apparently the 3 first left bits is for definied the size (in bytes) of the value:
	void writePacked(std::vector<byte> &listBytesTNam, uint32_t value);					//apparently the 3 first left bits is for definied the size (in bytes) of the value:
	long long readFormat(const uint8_t *buf, size_t size, uint32_t flags, string &type_str, bool bigEndian = false, bool isSigned = false);
	void writeFormat(long long value, std::vector<byte> &listBytesData, uint32_t flags, bool isSigned = false);


	void saveXml(string filename);
	TiXmlElement* export_Xml(string filename);
	virtual bool Compile(TiXmlDocument *doc, bool big_endian = false);
	bool import_Xml(TiXmlElement* parent);							//version 2015
	//bool import_Xml_v2014_01_00_r1(TiXmlElement* parent);			//version 2014.01.00.r1
	//Havok_TagObject* objectImportXml_v2014_01_00_r1(TiXmlElement* node, std::vector<Havok_TagObject*> &linkObj, std::vector<size_t> &linkIndex, Havok_TagItem* parentAttachement = 0);
	Havok_TagType* searchForType(string name, Havok_TagType* pointer = 0);
	//void calculateByteSizeAndAlignmentOfClass(Havok_TagType* type);


	
#ifdef LIBXENOVERSE_FBX_SUPPORT
	bool	importFBX(FbxNode *fbxNode);
	void	exportFBX(FbxScene *scene);
#endif
};


}

#endif // HavokFILE_H
