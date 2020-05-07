#include "Obl.h"

#include <sstream>
#include <iomanip>


namespace LibXenoverse
{

/*-------------------------------------------------------------------------------\
|                             Obl												 |
\-------------------------------------------------------------------------------*/
Obl::Obl()
{
	
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void Obl::Reset()
{
	mListPart.clear();
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void Obl_Part::Reset()
{
	unk_0 = unk_1 = unk_2 = unk_3 = 0xFFFFFFFF;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool Obl::Load(const uint8_t *buf, size_t size)
{
	Reset();

	OBLHeader *hdr = (OBLHeader*)buf;

	if (size < sizeof(OBLHeader) || memcmp(hdr->signature, OBL_SIGNATURE, 4) != 0)
		return false;

	this->big_endian = (buf[4] != 0xFE);

	size_t startOffsetForSection = hdr->headerSize;
	size_t maxNbSection = (startOffsetForSection - sizeof(OBLHeader)) / sizeof(uint32_t);



	size_t offset = startOffsetForSection;
	OBL_Section* section;
	size_t nbParts = 0;
	for (size_t i = 0; i < maxNbSection; i++)
	{
		nbParts = *(uint32_t*)(buf + sizeof(OBLHeader) + sizeof(uint32_t) * i);
		if (nbParts == 0)
			continue;

		mListPart.push_back(std::vector<Obl_Part>());


		section = (OBL_Section*)(buf + offset);
		offset += sizeof(OBL_Section) * nbParts;

		for (size_t j = 0; j < nbParts; j++)
			mListPart.back().push_back(Obl_Part(section[j].unk_0, section[j].unk_1, section[j].unk_2, section[j].unk_3));
	}

	return true;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*-------------------------------------------------------------------------------\
|                             calculeFilesize		                             |
\-------------------------------------------------------------------------------*/
size_t Obl::calculeFilesize(bool withTextureUnits)
{
	size_t filesize = sizeof(OBLHeader) + mListPart.size() * sizeof(uint32_t);							//header
	if (filesize & 0xF)
		filesize += (0x10 - (filesize & 0xF));

	size_t nbParts = 0;
	size_t nbSection = mListPart.size();
	for (size_t i = 0; i < nbSection; i++)
		filesize += mListPart.at(i).size() * sizeof(OBL_Section);

	return filesize;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*-------------------------------------------------------------------------------\
|                             CreateFile			                             |
\-------------------------------------------------------------------------------*/
uint8_t* Obl::CreateFile(unsigned int *psize)
{
	size_t filesize = calculeFilesize();

	uint8_t *buf = new uint8_t[filesize];
	if (!buf)
	{
		printf("%s: Memory allocation error (0x%x)\n", FUNCNAME, filesize);
		LibXenoverse::notifyError();
		return nullptr;
	}
	memset(buf, 0, filesize);				//fill by 0 to secure, and not having random memory.


	write(buf, filesize);


	*psize = filesize;
	return buf;
}
/*-------------------------------------------------------------------------------\
|                             write												 |
\-------------------------------------------------------------------------------*/
size_t Obl::write(uint8_t *buf, size_t size)
{
	OBLHeader* hdr = (OBLHeader*)buf;
	strcpy(&(hdr->signature[0]), OBL_SIGNATURE);
	hdr->endian = val16(0xFFFE);
	
	size_t offset = sizeof(OBLHeader) + mListPart.size() * sizeof(uint32_t);
	if (offset & 0xF)
		offset += (0x10 - (offset & 0xF));
	hdr->headerSize = offset;
	

	OBL_Section* section = (OBL_Section*)(buf + offset);
	size_t inc = 0;

	size_t nbParts = 0;
	size_t nbSection = mListPart.size();
	for (size_t i = 0; i < nbSection; i++)
	{
		nbParts = mListPart.at(i).size();
		*(uint32_t*)(buf + sizeof(OBLHeader) + sizeof(uint32_t) * i) = nbParts;

		for (size_t j = 0; j < nbParts; j++)
		{
			Obl_Part &part = mListPart.at(i).at(j);
			section[inc].unk_0 = part.unk_0;
			section[inc].unk_1 = part.unk_1;
			section[inc].unk_2 = part.unk_2;
			section[inc].unk_3 = part.unk_3;
			inc++;
		}
	}

	return offset;
}




















/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/*-------------------------------------------------------------------------------\
|                             saveXml											 |
\-------------------------------------------------------------------------------*/
void Obl::saveXml(string filename)
{
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement* rootNode = export_Xml();
	doc.LinkEndChild(rootNode);

	doc.SaveFile(filename);
}
/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* Obl::export_Xml()
{
	TiXmlElement* mainNode = new TiXmlElement("Obl");
	
	
	TiXmlElement* node_tmp;
	size_t nbSection = mListPart.size();
	for (size_t i = 0; i < nbSection; i++)
	{
		TiXmlElement* node = new TiXmlElement("Part");
		mainNode->LinkEndChild(node);

		size_t nbParts = mListPart.at(i).size();
		for (size_t j = 0; j < nbParts; j++)
		{
			Obl_Part &part = mListPart.at(i).at(j);

			node_tmp = new TiXmlElement("Element");
			node->LinkEndChild(node_tmp);

			node_tmp->SetAttribute("unk_0", UnsignedToString(part.unk_0, true));
			node_tmp->SetAttribute("unk_1", UnsignedToString(part.unk_1, true));
			node_tmp->SetAttribute("unk_2", UnsignedToString(part.unk_2, true));
			node_tmp->SetAttribute("unk_3", UnsignedToString(part.unk_3, true));
		}
	}

	return mainNode;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------\
|                             Compile				                             |
\-------------------------------------------------------------------------------*/
bool Obl::Compile(TiXmlDocument *doc, bool big_endian)
{
	this->big_endian = big_endian;

	TiXmlHandle handle(doc);
	TiXmlElement* rootNode = EMO_BaseFile::FindRoot(&handle, "Obl");

	if (!rootNode)
	{
		printf("Cannot find\"Obl\" in xml.\n");
		notifyError();
		return false;
	}

	return import_Xml(rootNode);
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool Obl::import_Xml(TiXmlElement* rootNode)
{
	Reset();

	//Parts make some reference to the TextureUnits, So we have to deal with TextureUnits first.
	size_t unk_0, unk_1, unk_2, unk_3;
	string str;
	for (TiXmlElement* node = rootNode->FirstChildElement("Part"); node; node = node->NextSiblingElement("Part"))
	{
		mListPart.push_back(std::vector<Obl_Part>());
		
		for (TiXmlElement* node_tmp = node->FirstChildElement("Element"); node_tmp; node_tmp = node_tmp->NextSiblingElement("Element"))
		{
			node_tmp->QueryStringAttribute("unk_0", &str); unk_0 = EMO_BaseFile::GetUnsigned(str, 0xFFFFFFFF);
			node_tmp->QueryStringAttribute("unk_1", &str); unk_1 = EMO_BaseFile::GetUnsigned(str, 0xFFFFFFFF);
			node_tmp->QueryStringAttribute("unk_2", &str); unk_2 = EMO_BaseFile::GetUnsigned(str, 0xFFFFFFFF);
			node_tmp->QueryStringAttribute("unk_3", &str); unk_3 = EMO_BaseFile::GetUnsigned(str, 0xFFFFFFFF);

			mListPart.back().push_back(Obl_Part(unk_0, unk_1, unk_2, unk_3));
		}
	}
	return true;
}

}