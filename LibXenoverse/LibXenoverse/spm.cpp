#include "Spm.h"



#include <sstream>
#include <iomanip>

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out << std::setprecision(n) << a_value;
	return out.str();
}

namespace LibXenoverse
{

/*-------------------------------------------------------------------------------\
|                             Spm												 |
\-------------------------------------------------------------------------------*/
Spm::Spm()
{
	//section 0 - SPMHeader : black/grey
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#232323");			//background color
	listTagColors.back().back().push_back("#FFFFFF");			//font color (just to possibility read from background color)
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#464646");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 1 - SPMUnk0AEntry : red
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC0000");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC7777");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 2 - SPMUnk0BEntry : green
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#00CC00");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#77CC77");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 3 - SPMUnk1Entry : bleu
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#0000CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7777CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f




	//section 5 - SPMUnk2Entry : yellow
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCC00");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCC77");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 6 - SPMUnk3Entry : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#00CCCC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#77CCCC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 7 - SPMUnk3Entry indexes : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#009999");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#339999");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 8 - SPMUnk4Entry : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC00CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC77CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 9 - SPMUnk4Entry_Next : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#990099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#993399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f



	//section 10 - SPMUnk4Entry_Next_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 11 - SPMUnk4Entry_Next_c : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC0099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC3399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 12 - SPMUnk4Entry_Next_d : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900AA");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933AA");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 13 - SPMUnk4Entry_Next_e : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA0099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA3399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f



	//section 14 - SPMUnk4Entry_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#770077");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#773377");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 15 - SPMUnk4Entry_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#550055");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#553355");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f





	//section 16 - SPMUnk4Entry_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA00FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA33FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 17 - SPMUnk4Entry_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 18 - SPMUnk4Entry_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7700FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7733FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 19 - SPMUnk4Entry_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#5500FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#5533FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f




	//section 20 - SPMUnk5Entry : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCCCC");			//bg
	listTagColors.back().back().push_back("#000000");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AAAAAA");			//bg
	listTagColors.back().back().push_back("#000000");			//f


	//section 21 - SPMUnk5Entry_Box : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#999999");			//bg
	listTagColors.back().back().push_back("#000000");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#777777");			//bg
	listTagColors.back().back().push_back("#000000");			//f
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void Spm::Reset()
{
	/*
	mListPart.clear();

	size_t nbText = mListTextureUnit.size();
	for (size_t i = 0; i < nbText; i++)
		delete mListTextureUnit.at(i);
	mListTextureUnit.clear();
	*/
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void Spm_Part::Reset()
{
	// Todo
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*
bool Spm::Load(const uint8_t *buf, size_t size)
{
	Reset();

	SPMHeader *hdr = (SPMHeader *)buf;

	if (size < sizeof(SPMHeader) || memcmp(hdr->signature, SPM_SIGNATURE, 4) != 0)
		return false;

	
	//Section1 make reference to Section2. so we begin by section2.
	size_t offset = 0;
	for (size_t i = 0; i < hdr->nbSection2; i++)
	{
		offset = hdr->offsetSection2 + i * sizeof(SPM_Section2);

		Spm_TextureUnit* textUnit = new Spm_TextureUnit();
		if( textUnit->Load( GetOffsetPtr(buf, offset), size - val32(offset) ) )
			mListTextureUnit.push_back(textUnit);
	}


	for (size_t i = 0; i < hdr->nbSection1; i++)
	{
		offset = hdr->offsetSection1 + i * sizeof(SPM_Section1);

		Spm_Part part;
		if (part.Load(GetOffsetPtr(buf, offset), size - val32(offset), hdr->offsetSection2 - offset, mListTextureUnit))
			mListPart.push_back(part);
	}

	return true;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////











/*-------------------------------------------------------------------------------\
|                             calculeFilesize		                             |
\-------------------------------------------------------------------------------*
size_t Spm::calculeFilesize(bool withTextureUnits)
{
	size_t filesize = 0x20;							//header, + filling to have proper alignement 16 bytes, taht help to have stable subDatas


	size_t nbParts = mListPart.size();
	for (size_t i = 0; i < nbParts; i++)
		filesize += mListPart.at(i).calculeFilesize();

	size_t nbText = (withTextureUnits) ? mListTextureUnit.size() : 0;
	for (size_t i = 0; i < nbText; i++)
		filesize += mListTextureUnit.at(i)->calculeFilesize();

	return filesize;
}









/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*-------------------------------------------------------------------------------\
|                             CreateFile			                             |
\-------------------------------------------------------------------------------*
uint8_t* Spm::CreateFile(unsigned int *psize)
{
	size_t filesize = calculeFilesize();

	uint8_t *buf = new uint8_t[filesize];
	if (!buf)
	{
		LOG_DEBUG("%s: Memory allocation error (0x%x)\n", FUNCNAME, filesize);
		return nullptr;
	}
	memset(buf, 0, filesize);				//fill by 0 to secure, and not having random memory.


	write(buf, filesize);


	*psize = filesize;
	return buf;
}
/*-------------------------------------------------------------------------------\
|                             write												 |
\-------------------------------------------------------------------------------*
size_t Spm::write(uint8_t *buf, size_t size)
{
	SPMHeader* hdr = (SPMHeader*)buf;
	size_t offset = 0;

	strcpy(&(hdr->signature[0]), SPM_SIGNATURE);
	hdr->endian = val16(0xFFFE);
	hdr->version = 0x92c00018;
	hdr->unk_00 = 0;

	hdr->nbSection1 = mListPart.size();
	hdr->offsetSection1 = 0x20;										//we fix the start at 0x20 for better view
	hdr->nbSection2 = mListTextureUnit.size();
	hdr->offsetSection2 = calculeFilesize(false);


	offset = hdr->offsetSection1;
	for (size_t i = 0; i < hdr->nbSection1; i++)
	{
		offset = hdr->offsetSection1 + i * sizeof(SPM_Section1);
		mListPart.at(i).write_Main(buf + offset, size - offset);
	}

	size_t startSection1Offset = 0;
	offset = hdr->offsetSection1 + hdr->nbSection1 * sizeof(SPM_Section1);
	for (size_t i = 0; i < hdr->nbSection1; i++)
	{
		startSection1Offset = hdr->offsetSection1 + i * sizeof(SPM_Section1);

		*((uint32_t*)GetOffsetPtr(buf, startSection1Offset + 0xAC, true)) = (mListPart.at(i).mListAnimationA.size()) ? (offset - startSection1Offset - 0xA8) : 0;		//setup section1->offsetSection5;
		offset += mListPart.at(i).write_AnimationsA(buf + offset, size - offset);


		*((uint32_t*)GetOffsetPtr(buf, startSection1Offset + 0xA4, true)) = (mListPart.at(i).mListAnimationB.size()) ? (offset - startSection1Offset - 0x78) : 0;		//setup section1->offsetSection4;
		offset += mListPart.at(i).write_AnimationsB(buf + offset, size - offset);
	}

	
	for (size_t i = 0; i < hdr->nbSection1; i++)
	{
		startSection1Offset = hdr->offsetSection1 + i * sizeof(SPM_Section1);

		*((uint32_t*)GetOffsetPtr(buf, startSection1Offset + 0x70, true)) = (mListPart.at(i).mListTextureUnit.size()) ? (offset - startSection1Offset) : 0;		//setup section1->offsetSection2;
		offset += mListPart.at(i).write_TextureUnitPointers(buf + offset, size - offset, hdr->offsetSection2 - (hdr->offsetSection1 + i * sizeof(SPM_Section1)) , mListTextureUnit);
	}


	for (size_t i = 0; i < hdr->nbSection1; i++)
	{
		startSection1Offset = hdr->offsetSection1 + i * sizeof(SPM_Section1);

		*((uint32_t*)GetOffsetPtr(buf, startSection1Offset + 0x74, true)) = offset - startSection1Offset;		//setup section1->offsetSection3;
		offset += mListPart.at(i).write_Extrude(buf + offset, size - offset);
	}


	//TextureUnits
	offset = hdr->offsetSection2;
	for (size_t i = 0; i < hdr->nbSection2; i++)
	{
		offset = hdr->offsetSection2 + i * sizeof(SPM_Section2);
		mListTextureUnit.at(i)->write_Main(buf + offset, size - offset);
	}

	size_t startSection2Offset = 0;
	offset = hdr->offsetSection2 + hdr->nbSection2 * sizeof(SPM_Section2);
	for (size_t i = 0; i < hdr->nbSection2; i++)
	{
		if ((mListTextureUnit.at(i)->mUseSpeed) || (mListTextureUnit.at(i)->mListKeyframe.size() <=1 ))
			continue;

		startSection2Offset = hdr->offsetSection2 + i * sizeof(SPM_Section2);
		
		*((uint32_t*)GetOffsetPtr(buf, startSection2Offset + 0x18, true)) = (mListTextureUnit.at(i)->mListKeyframe.size()) ? (offset - startSection2Offset - 0xC) : 0;		//Section2_b_02->offsetSection2_02_c;
		offset += mListTextureUnit.at(i)->write_Animation(buf + offset, size - offset);
	}

	return offset;
}




















/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/*-------------------------------------------------------------------------------\
|                             saveXml											 |
\-------------------------------------------------------------------------------*
void Spm::saveXml(string filename)
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
\-------------------------------------------------------------------------------*
TiXmlElement* Spm::export_Xml()
{
	TiXmlElement* mainNode = new TiXmlElement("Spm");
	
	TiXmlElement* node = new TiXmlElement("Parts");
	size_t nbPart = mListPart.size();
	for (size_t i = 0; i < nbPart; i++)
		node->LinkEndChild(mListPart.at(i).export_Xml(mListTextureUnit));
	mainNode->LinkEndChild(node);


	node = new TiXmlElement("TextureUnits");
	size_t nbText = mListTextureUnit.size();
	for (size_t i = 0; i < nbText; i++)
		node->LinkEndChild(mListTextureUnit.at(i)->export_Xml());
	mainNode->LinkEndChild(node);


	return mainNode;
}









//////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//notice, as we discovered nothing releveant, we have to recreate the file directly with the Xml. So it's also for debug for now
/*-------------------------------------------------------------------------------\
|                             Compile				                             |
\-------------------------------------------------------------------------------*/
bool Spm::Compile(TiXmlDocument *doc, bool big_endian)
{
	this->big_endian = big_endian;

	std::vector<size_t> listBytes;
	size_t offset = 0;

	TiXmlHandle handle(doc);
	TiXmlElement* rootNode = EMO_BaseFile::FindRoot(&handle, "Spm");
	if (!rootNode)
	{
		LOG_DEBUG("Cannot find\"Spm\" in xml.\n");
		return false;
	}

	TiXmlElement* node;
	TiXmlElement* node_SPMHeader = rootNode->FirstChildElement("Header");
	if (!node_SPMHeader)
	{
		LOG_DEBUG("Cannot find\"Header\" in xml.\n");
		return false;
	}

	bool isVersion_E = (rootNode->FirstChildElement("ListDSC") == 0);
	string str = "";
	size_t tmp = 0;
	float f_tmp = 0.0f;


	


	listBytes.push_back( '#' + ('S' << 8) + ('P' << 16) + ( (isVersion_E ? 'E' : 'X') << 24) );	//signature
	listBytes.push_back(0);			//version
	listBytes.push_back(0);			//number_Section1
	listBytes.push_back(0);			//size_Section1
	listBytes.push_back( isVersion_E ? sizeof(SPMHeader_E) : (sizeof(SPMHeader_X) + sizeof(uint32_t)) );	// offset_Section1
	if (!isVersion_E)
	{
		listBytes.push_back(0);		//number_Section2
		listBytes.push_back(0);		//offset_Section2
		listBytes.push_back(0);		//0 filing
	}



	//SpmSection1
	TiXmlElement* node_ListS1 = rootNode->FirstChildElement("ListSection1");
	char* buf_names = new char[16];

	std::vector<std::vector<size_t>> listByteSection1;
	for (TiXmlElement* node_tmp = node_ListS1 ? node_ListS1->FirstChildElement("Section1") : 0; node_tmp; node_tmp = node_tmp->NextSiblingElement("Section1"))
	{
		listByteSection1.push_back(std::vector<size_t>());
		
		/*
		node = node_tmp->FirstChildElement("name"); if (node) node->QueryStringAttribute("string", &str);
		size_t nbChar = str.length();
		for (size_t i = 0; i < 4; i++)
			listByteSection1.back().push_back(val32(  (((i * 4 < nbChar) ? str.at(i * 4) : '\0') << 0) + (((i * 4 + 1 < nbChar) ? str.at(i * 4 + 1) : '\0') << 8) + (((i * 4 + 2 < nbChar) ? str.at(i * 4 + 2) : '\0') << 16) + (((i * 4 + 3 < nbChar) ? str.at(i * 4 + 3) : '\0')  << 24) ));
		*/

		
		
		memset(buf_names, 0, 16);
		node = node_tmp->FirstChildElement("name"); 
		if (node)
		{
			str = "";
			node->QueryStringAttribute("main", &str);
			if (str.length() >= 16)
				str = str.substr(0, 16);
			size_t nbChar = str.length();
			for (size_t i = 0; i < nbChar; i++)
				buf_names[i] = str.at(i);
			
			if (nbChar < 16)
				buf_names[nbChar] = '\0';


			size_t startOffset = nbChar + 1;
			for (size_t j = 1; j < 9; ++j)
			{
				str = "";
				if ((startOffset >= 16)||(node->QueryStringAttribute(("sub_" + std::to_string(j)).c_str(), &str)== TIXML_NO_ATTRIBUTE))
					break;

				if (startOffset + str.length() >= 16)
					str = str.substr(0, 16 - startOffset);
				size_t nbChar = str.length();
				for (size_t i = 0; i < nbChar; i++)
					buf_names[startOffset + i] = str.at(i);

				if (startOffset + nbChar < 16)
					buf_names[startOffset + nbChar] = '\0';

				startOffset += nbChar + 1;
			}
		}
		
		for (size_t i = 0; i < 4; i++)									//inversion because of endian somewhere.
			listByteSection1.back().push_back( val32( buf_names[i * 4] + (buf_names[i * 4 + 1] << 8) + (buf_names[i * 4 + 2] << 16) + (buf_names[i * 4 + 3] << 24) ));


		

		node = node_tmp->FirstChildElement("lenFlareEnabled"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF));
		node = node_tmp->FirstChildElement("unk_1"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16;				//because endian

		node = node_tmp->FirstChildElement("unk_2"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF));
		node = node_tmp->FirstChildElement("unk_3"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16;

		node = node_tmp->FirstChildElement("unk_4"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF));
		node = node_tmp->FirstChildElement("unk_5"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16;

		node = node_tmp->FirstChildElement("unk_6"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF));
		node = node_tmp->FirstChildElement("unk_7"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16;

		for (size_t j = 0; j < 82; j++)
		{
			f_tmp = 0.0f;
			node = node_tmp->FirstChildElement("unk_8_a__" + std::to_string(j)); if (node) node->QueryFloatAttribute("float", &f_tmp);
			listByteSection1.back().push_back( val32( *((size_t*)(&f_tmp)) ));
		}

		node = node_tmp->FirstChildElement("unk_8_b"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF));
		node = node_tmp->FirstChildElement("unk_8_c"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16;

		for (size_t j = 0; j < 45; j++)
		{
			f_tmp = 0.0f;
			node = node_tmp->FirstChildElement("unk_8_d__" + std::to_string(j)); if (node) node->QueryFloatAttribute("float", &f_tmp);
			listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)) ) );
		}



		node = node_tmp->FirstChildElement("unk_9"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF));
		node = node_tmp->FirstChildElement("unk_10"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16;				//because endian will be applyed after.

		node = node_tmp->FirstChildElement("unk_11"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF));
		node = node_tmp->FirstChildElement("unk_12"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16;

		node = node_tmp->FirstChildElement("unk_13"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF));
		node = node_tmp->FirstChildElement("unk_14"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16;

		node = node_tmp->FirstChildElement("unk_15"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF));
		node = node_tmp->FirstChildElement("unk_16"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16;


		for (size_t j = 0; j < 56; j++)
		{
			f_tmp = 0.0f;
			node = node_tmp->FirstChildElement("unk_17__" + std::to_string(j)); if (node) node->QueryFloatAttribute("float", &f_tmp);
			listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
		}


		node = node_tmp->FirstChildElement("unk_18"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF));
		node = node_tmp->FirstChildElement("unk_19"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16;				//because endian will be applyed after.

		node = node_tmp->FirstChildElement("unk_20"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF));
		node = node_tmp->FirstChildElement("unk_21"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16;

		node = node_tmp->FirstChildElement("unk_22"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF));
		node = node_tmp->FirstChildElement("unk_23"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16;

		node = node_tmp->FirstChildElement("unk_24"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF));
		node = node_tmp->FirstChildElement("unk_25"); if (node) node->QueryStringAttribute("u16", &str);
		listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16;


		TiXmlElement* node_SPMSection1_UnknowEnd = node_tmp->FirstChildElement("UnknowEnd");
		if (node_SPMSection1_UnknowEnd)
		{
			for (size_t j = 0; j < 500; j++)
			{
				node = node_SPMSection1_UnknowEnd->FirstChildElement("unk_" + std::to_string(j));
				if (!node)
					break;

				node->QueryStringAttribute("u32", &str);
				listByteSection1.back().push_back(val32(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFFFFFF));
			}
		}
	}




	size_t nbSection1 = listByteSection1.size();
	size_t sizeSection1 = 0;
	for (size_t i=0;i<nbSection1;i++)								//search for maximum
	{
		if (listByteSection1.at(i).size()> sizeSection1)
			sizeSection1 = listByteSection1.at(i).size();
	}

	//update header
	listBytes.at(2) = val32(nbSection1);
	listBytes.at(3) = val32(sizeSection1 * 4);
	if (!isVersion_E)
		listBytes.at(6) = val32(listBytes.at(4) + (nbSection1 * sizeSection1) * 4);		//offset_Section2 = offset_Section1 + nbSection1 * sizeSection1

	//and finally copy Section1 bytes, + filling '\0' for smaller Section1
	size_t nbBytes = 0;
	for (size_t i = 0; i<nbSection1; i++)
	{
		nbBytes = listByteSection1.at(i).size();
		for (size_t j = 0; j < sizeSection1; j++)
			listBytes.push_back((j < nbBytes) ? listByteSection1.at(i).at(j) : 0);
	}





	//section2
	if (!isVersion_E)
	{
		size_t startSection2List = listBytes.size();

		TiXmlElement* node_ListDsc = rootNode->FirstChildElement("ListDSC");

		size_t nbDsc = 0;
		for (TiXmlElement* node_tmp = node_ListDsc ? node_ListDsc->FirstChildElement("DSC") : 0; node_tmp; node_tmp = node_tmp->NextSiblingElement("DSC"))
			nbDsc++;

		listBytes.at(5) = nbDsc;				//number_Section2
		if (nbDsc == 0)
			listBytes.at(6) = 0;				//offset_Section2



		for (size_t i = 0; i < nbDsc; i++)
			listBytes.push_back(0);		//offset_DscFile
		listBytes.push_back(0);		//empty


		size_t inc_Section2 = 0;
		for (TiXmlElement* node_tmp = node_ListDsc ? node_ListDsc->FirstChildElement("DSC") : 0; node_tmp; node_tmp = node_tmp->NextSiblingElement("DSC"))
		{
			size_t startDsc = listBytes.size();
			listBytes.at(startSection2List + inc_Section2++) = val32((startDsc - startSection2List) * 4);		//offset_DscFile


			listBytes.push_back(val32(('#' << 0) + ('D' << 8) + ('S' << 16) + ('C' << 24)));
			listBytes.push_back(0);			//number_Section1
			node = node_tmp->FirstChildElement("unk_0"); if (node) node->QueryStringAttribute("u32", &str); listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFFFFFF));
			node = node_tmp->FirstChildElement("unk_1"); if (node) node->QueryStringAttribute("u32", &str); listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFFFFFF));
			listBytes.push_back(val32(sizeof(DSC_Header)));	//offset_Section1
			node = node_tmp->FirstChildElement("unk_3"); if (node) node->QueryStringAttribute("u32", &str); listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFFFFFF));




			size_t startDscListSect_1 = listBytes.size();


			TiXmlElement* node_DscListSection1 = node_tmp->FirstChildElement("ListSect_1");
			size_t nbSection1 = 0;
			for (TiXmlElement* node_DscSection1 = node_DscListSection1 ? node_DscListSection1->FirstChildElement("Sect_1") : 0; node_DscSection1; node_DscSection1 = node_DscSection1->NextSiblingElement("Sect_1"))
				nbSection1++;

			listBytes.at(startDsc + 1) = val32(nbSection1);
			//if (nbSection1 == 0)
			//	listBytes.at(startDsc + 4) = 0;	//offset_Section1		(FLBY03IN.spm said , same if there isn't Section1, offset could be 18)
			

			for (TiXmlElement* node_DscSection1 = node_DscListSection1 ? node_DscListSection1->FirstChildElement("Sect_1") : 0; node_DscSection1; node_DscSection1 = node_DscSection1->NextSiblingElement("Sect_1"))
			{
				listBytes.push_back(0);			//offset_Section1_b
				node = node_DscSection1->FirstChildElement("unk_0"); if (node) node->QueryStringAttribute("u32", &str); listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFFFFFF));
			}


			size_t inc_sect1 = 0;
			for (TiXmlElement* node_DscSection1 = node_DscListSection1 ? node_DscListSection1->FirstChildElement("Sect_1") : 0; node_DscSection1; node_DscSection1 = node_DscSection1->NextSiblingElement("Sect_1"))
			{
				size_t startSect1 = listBytes.size();

				TiXmlElement* node_DscSection1_b = node_DscSection1->FirstChildElement("Sect_1_b");
				if (!node_DscSection1_b)
					continue;

				listBytes.at(startDscListSect_1 + inc_sect1 * 2) = val32((startSect1 - startDscListSect_1) * 4);			//offset_Section1_b
				inc_sect1++;

				bool haveExtra = (node_DscSection1_b->FirstChildElement("unk_3") != 0);

				
				memset(buf_names, 0, 16);
				node = node_DscSection1_b->FirstChildElement("name");
				if (node)
				{
					str = "";
					node->QueryStringAttribute("main", &str);
					if (str.length() >= 16)
						str = str.substr(0, 16);
					size_t nbChar = str.length();
					for (size_t i = 0; i < nbChar; i++)
						buf_names[i] = str.at(i);

					if (nbChar < 16)
						buf_names[nbChar] = '\0';


					size_t startOffset = nbChar + 1;
					for (size_t j = 1; j < 9; ++j)
					{
						str = "";
						if ((startOffset >= 16) || (node->QueryStringAttribute(("sub_" + std::to_string(j)).c_str(), &str) == TIXML_NO_ATTRIBUTE))
							break;

						if (startOffset + str.length() >= 16)
							str = str.substr(0, 16 - startOffset);
						size_t nbChar = str.length();
						for (size_t i = 0; i < nbChar; i++)
							buf_names[startOffset + i] = str.at(i);

						if (startOffset + nbChar < 16)
							buf_names[startOffset + nbChar] = '\0';

						startOffset += nbChar + 1;
					}
				}

				for (size_t i = 0; i < 4; i++)									//inversion because of endian somewhere.
					listBytes.push_back(val32(buf_names[i * 4] + (buf_names[i * 4 + 1] << 8) + (buf_names[i * 4 + 2] << 16) + (buf_names[i * 4 + 3] << 24)));



				listBytes.push_back(0);			//number_Section1_c
				node = node_DscSection1_b->FirstChildElement("unk_0"); if (node) node->QueryStringAttribute("u32", &str); listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFFFFFF));
				listBytes.push_back(val32(sizeof(DSC_Section1_c)));			//size_Section1_c
				node = node_DscSection1_b->FirstChildElement("unk_1"); if (node) node->QueryStringAttribute("u32", &str); listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFFFFFF));
				listBytes.push_back(val32(sizeof(DSC_Section1_b) - ((!haveExtra) ? 8 : 0)));			//offset_Section1_c
				node = node_DscSection1_b->FirstChildElement("unk_2"); if (node) node->QueryStringAttribute("u32", &str); listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFFFFFF));

				if (haveExtra)
				{
					node = node_DscSection1_b->FirstChildElement("index"); if (node) node->QueryStringAttribute("u32", &str); listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFFFFFF));
					node = node_DscSection1_b->FirstChildElement("unk_3"); if (node) node->QueryStringAttribute("u32", &str); listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFFFFFF));
				}



				size_t startDscListSect_1_c = listBytes.size();

				TiXmlElement* node_DscListSection1_c = node_DscSection1_b->FirstChildElement("ListSect_1_c");
				size_t nbSection1_c = 0;
				for (TiXmlElement* node_DscSection1_c = node_DscListSection1_c ? node_DscListSection1_c->FirstChildElement("Sect_1_c") : 0; node_DscSection1_c; node_DscSection1_c = node_DscSection1_c->NextSiblingElement("Sect_1_c"))
					nbSection1_c++;


				listBytes.at(startSect1 + 4) = val32(nbSection1_c);
				if (nbSection1_c == 0)
					listBytes.at(startSect1 + 8) = 0;	//offset_Section1_c		


				for (TiXmlElement* node_DscSection1_c = node_DscListSection1_c ? node_DscListSection1_c->FirstChildElement("Sect_1_c") : 0; node_DscSection1_c; node_DscSection1_c = node_DscSection1_c->NextSiblingElement("Sect_1_c"))
				{
					node = node_DscSection1_c->FirstChildElement("unk_0"); if (node) node->QueryStringAttribute("u32", &str); listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFFFFFF)); str = "0";
					node = node_DscSection1_c->FirstChildElement("unk_1"); if (node) node->QueryStringAttribute("u32", &str); listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFFFFFF)); str = "0";
					node = node_DscSection1_c->FirstChildElement("unk_2"); if (node) node->QueryStringAttribute("u32", &str); listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFFFFFF)); str = "0";
					node = node_DscSection1_c->FirstChildElement("unk_3"); if (node) node->QueryStringAttribute("u32", &str); listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFFFFFF)); str = "0";
					node = node_DscSection1_c->FirstChildElement("unk_4"); if (node) node->QueryStringAttribute("u32", &str); listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFFFFFF)); str = "0";

					node = node_DscSection1_c->FirstChildElement("Color");

					f_tmp = 0.0f;
					if (node) node->QueryFloatAttribute("R", &f_tmp); listBytes.push_back(*((size_t*)&f_tmp));	f_tmp = 0.0f;
					if (node) node->QueryFloatAttribute("G", &f_tmp); listBytes.push_back(*((size_t*)&f_tmp));	f_tmp = 0.0f;
					if (node) node->QueryFloatAttribute("B", &f_tmp); listBytes.push_back(*((size_t*)&f_tmp));	f_tmp = 0.0f;
				}

				//if ((!haveExtra)&&(inc_sect1 != nbSection1))			//not the last, 0 filling
				if (!haveExtra)
				{
					listBytes.push_back(0);
					listBytes.push_back(0);
				}
			}
		}


	}

	delete buf_names;




	//////////////////////////////
	size_t nbSizeT = listBytes.size();
	size_t filesize = nbSizeT * 4;

	uint8_t *buf = new uint8_t[filesize];
	if (!buf)
	{
		LOG_DEBUG("%s: Memory allocation error (0x%x)\n", FUNCNAME, filesize);
		return nullptr;
	}
	memset(buf, 0, filesize);				//fill by 0 to secure, and not having random memory.


	uint32_t* buf_uint32 = (uint32_t*)buf;
	for (size_t i = 0; i < nbSizeT; i++)
		buf_uint32[i] = listBytes.at(i);
	


	///////////////////////////////

	string filename = originefilename;


	FILE *f = fopen(filename.c_str(), "wb");
	if (!f)
	{
		printf("Cannot open for write/create file \"%s\"\n", filename.c_str());
		return false;
	}

	size_t wd = fwrite(buf, 1, filesize, f);
	fclose(f);

	
	delete buf;

	return true;
}

/*-------------------------------------------------------------------------------\
|                             Compile				                             |
\-------------------------------------------------------------------------------*
bool Spm::Compile(TiXmlDocument *doc, bool big_endian)
{
	this->big_endian = big_endian;

	TiXmlHandle handle(doc);
	TiXmlElement* rootNode = EMO_BaseFile::FindRoot(&handle, "Spm");

	if (!rootNode)
	{
		LOG_DEBUG("Cannot find\"Spm\" in xml.\n");
		return false;
	}

	return import_Xml(rootNode);
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*
bool Spm::import_Xml(TiXmlElement* rootNode)
{
	Reset();

	TiXmlElement* partsNode = rootNode->FirstChildElement("Parts");
	if (!partsNode)
	{
		LOG_DEBUG("Cannot find\"Parts\" in xml.\n");
		return false;
	}

	TiXmlElement* textureUnitsNode = rootNode->FirstChildElement("TextureUnits");
	if (!textureUnitsNode)
	{
		LOG_DEBUG("Cannot find\"TextureUnits\" in xml.\n");
		return false;
	}


	//Parts make some reference to the TextureUnits, So we have to deal with TextureUnits first.
	for (TiXmlElement* node = textureUnitsNode->FirstChildElement("TextureUnit"); node; node = node->NextSiblingElement("TextureUnit"))
	{
		mListTextureUnit.push_back(new Spm_TextureUnit());
		if (!mListTextureUnit.back()->import_Xml(node))
			mListTextureUnit.pop_back();
	}

	for (TiXmlElement* node = partsNode->FirstChildElement("Part"); node; node = node->NextSiblingElement("Part"))
	{
		mListPart.push_back(Spm_Part());
		if (!mListPart.back().import_Xml(node, mListTextureUnit))
			mListPart.pop_back();
	}

	return true;
}










/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Xml First Version for Debug ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*-------------------------------------------------------------------------------\
|                             save_Xml				                             |
\-------------------------------------------------------------------------------*/
void Spm::save_Xml(string filename, bool show_error)
{
	uint8_t *buf;
	size_t size;

	buf = ReadFile(filename, &size, show_error);
	if (!buf)
		return;

	TiXmlDocument *doc = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	doc->LinkEndChild(decl);

	TiXmlElement* root = new TiXmlElement("Spm");
	EMO_BaseFile::WriteParamString(root, "path", filename);
	
	write_Xml(root, buf, size);

	doc->LinkEndChild(root);



	delete[] buf;

	doc->SaveFile(filename + ".xml");

	return;
}


/*-------------------------------------------------------------------------------\
|                             write_Xml						                     |
\-------------------------------------------------------------------------------*/
void Spm::write_Xml(TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	SPMHeader_E* hdr_e = (SPMHeader_E*)buf;
	if (size < sizeof(SPMHeader_E))
		return;

	bool isVersion_X = (memcmp(hdr_e->signature, SPM_SIGNATURE, 4) == 0);
	bool isVersion_E = (memcmp(hdr_e->signature, SPM_SIGNATURE_E, 4) == 0);

	if ((!isVersion_X) && (!isVersion_E))
		return;

	SPMHeader_X* hdr = 0;
	if (isVersion_E)
	{
		hdr = new SPMHeader_X();
		hdr->version = hdr_e->version;
		hdr->number_Section1 = hdr_e->number_Section1;
		hdr->size_Section1 = hdr_e->size_Section1;
		hdr->offset_Section1 = hdr_e->offset_Section1;
		hdr->number_Section2 = 0;
		hdr->offset_Section2 = 0;
	}
	else {
		hdr = (SPMHeader_X*)buf;
	}


	TiXmlElement* node_SPMHeader = new TiXmlElement("Header");
	TiXmlElement* node;
	node = new TiXmlElement("number_Section1"); node->SetAttribute("value", UnsignedToString(hdr->number_Section1, false)); node_SPMHeader->LinkEndChild(node);
	node = new TiXmlElement("size_Section1"); node->SetAttribute("size", UnsignedToString(hdr->size_Section1, true)); node_SPMHeader->LinkEndChild(node);
	node = new TiXmlElement("offset_Section1"); node->SetAttribute("startOffset", UnsignedToString(hdr->offset_Section1, true)); node_SPMHeader->LinkEndChild(node);
	if (!isVersion_E)
	{
		node = new TiXmlElement("number_Section2"); node->SetAttribute("value", UnsignedToString(hdr->number_Section2, false)); node_SPMHeader->LinkEndChild(node);
		node = new TiXmlElement("offset_Section2"); node->SetAttribute("startOffset", UnsignedToString(hdr->offset_Section2, true)); node_SPMHeader->LinkEndChild(node);
	}
	parent->LinkEndChild(node_SPMHeader);
	


	

	size_t offset = 0;
	size_t offset_tmp = 0;

	if (hdr->offset_Section1)
	{
		

		
		TiXmlElement* node_ListS1 = new TiXmlElement("ListSection1");

		for (size_t i = 0; i < hdr->number_Section1; i++)
		{
			size_t startOffsetSection1 = hdr->offset_Section1 + i * hdr->size_Section1;
			offset = startOffsetSection1;

			SPM_Section1* section1 = (SPM_Section1*)GetOffsetPtr(buf, startOffsetSection1);

			TiXmlElement* node_SPMSection1 = new TiXmlElement("Section1");

			/*
			string str = section1->name;
			if (str.length() >= 16)
				str = str.substr(0, 16);
				*/
			std::vector<string> fullNames;
			string str = "";
			string char_str_tmp = "a";
			bool isOnString = false;
			for (size_t j = 0; j < 16; ++j)
			{
				if (section1->name[j] == '\0')			//if it's a empty char
				{
					//if ((j==0)&&(fullNames.size() == 0))			//if first char is '\0', main name is "".
					if (!isOnString)			//if char is '\0' (same for previous), name is "".
					{
						fullNames.push_back("");
					}else if (isOnString) {				//if previously , we ahve a begin if a world, so we are at the end. so finish add action

						fullNames.push_back(str);
						str = "";
						isOnString = false;
						continue;
					}

				}else{								//if it's not a empty char

					char_str_tmp[0] = section1->name[j];
					str += char_str_tmp;
					isOnString = true;
				}
			}
			if (isOnString)								//last world witch don't have '\0' to end because of limited number of char.
				fullNames.push_back(str);


			node = new TiXmlElement("name"); 
			node->SetAttribute("main", fullNames.at(0)); 
			size_t nbStr = fullNames.size();
			for(size_t j=1;j<nbStr;++j)
				node->SetAttribute("sub_"+ std::to_string(j), fullNames.at(j));
			node_SPMSection1->LinkEndChild(node);

			node = new TiXmlElement("lenFlareEnabled"); node->SetAttribute("u16", UnsignedToString(section1->lenFlareEnabled, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("u16", UnsignedToString(section1->unk_1, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("u16", UnsignedToString(section1->unk_2, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_3"); node->SetAttribute("u16", UnsignedToString(section1->unk_3, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_4"); node->SetAttribute("u16", UnsignedToString(section1->unk_4, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_5"); node->SetAttribute("u16", UnsignedToString(section1->unk_5, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_6"); node->SetAttribute("u16", UnsignedToString(section1->unk_6, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_7"); node->SetAttribute("u16", UnsignedToString(section1->unk_7, true)); node_SPMSection1->LinkEndChild(node);
			
			for (size_t j = 0; j < 82; j++)
			{
				node = new TiXmlElement("unk_8_a__"+ std::to_string(j)); node->SetAttribute("float", to_string_with_precision(section1->unk_8_a[j], 8)); node_SPMSection1->LinkEndChild(node);
			}

			node = new TiXmlElement("unk_8_b"); node->SetAttribute("u16", UnsignedToString(section1->unk_8_b, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_8_c"); node->SetAttribute("u16", UnsignedToString(section1->unk_8_c, true)); node_SPMSection1->LinkEndChild(node);

			for (size_t j = 0; j < 45; j++)
			{
				node = new TiXmlElement("unk_8_d__" + std::to_string(j)); node->SetAttribute("float", to_string_with_precision(section1->unk_8_d[j], 8)); node_SPMSection1->LinkEndChild(node);
			}

			node = new TiXmlElement("unk_9"); node->SetAttribute("u16", UnsignedToString(section1->unk_9, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_10"); node->SetAttribute("u16", UnsignedToString(section1->unk_10, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_11"); node->SetAttribute("u16", UnsignedToString(section1->unk_11, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_12"); node->SetAttribute("u16", UnsignedToString(section1->unk_12, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_13"); node->SetAttribute("u16", UnsignedToString(section1->unk_13, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_14"); node->SetAttribute("u16", UnsignedToString(section1->unk_14, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_15"); node->SetAttribute("u16", UnsignedToString(section1->unk_15, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_16"); node->SetAttribute("u16", UnsignedToString(section1->unk_16, true)); node_SPMSection1->LinkEndChild(node);

			for (size_t j = 0; j < 56; j++)
			{
				node = new TiXmlElement("unk_17__" + std::to_string(j)); node->SetAttribute("float", to_string_with_precision(section1->unk_17[j], 8)); node_SPMSection1->LinkEndChild(node);
			}

			node = new TiXmlElement("unk_18"); node->SetAttribute("u16", UnsignedToString(section1->unk_18, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_19"); node->SetAttribute("u16", UnsignedToString(section1->unk_19, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_20"); node->SetAttribute("u16", UnsignedToString(section1->unk_20, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_21"); node->SetAttribute("u16", UnsignedToString(section1->unk_21, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_22"); node->SetAttribute("u16", UnsignedToString(section1->unk_22, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_23"); node->SetAttribute("u16", UnsignedToString(section1->unk_23, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_24"); node->SetAttribute("u16", UnsignedToString(section1->unk_24, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_25"); node->SetAttribute("u16", UnsignedToString(section1->unk_25, true)); node_SPMSection1->LinkEndChild(node);


			TiXmlElement* node_SPMSection1_UnknowEnd = new TiXmlElement("UnknowEnd");
			size_t inc = 0;
			for (size_t j = sizeof(SPM_Section1); j < hdr->size_Section1; j += 4)
			{
				node = new TiXmlElement("unk_" + std::to_string(inc++)); node->SetAttribute("u32", UnsignedToString(*(uint32_t*)GetOffsetPtr(buf, startOffsetSection1 + j), true)); node_SPMSection1_UnknowEnd->LinkEndChild(node);
				//todo check the possiblity to have a float.
			}
			node_SPMSection1->LinkEndChild(node_SPMSection1_UnknowEnd);


			node_ListS1->LinkEndChild(node_SPMSection1);
		}
		parent->LinkEndChild(node_ListS1);
	}
	
	



	if (hdr->offset_Section2)
	{
		SPM_Section2* section2 = (SPM_Section2*)GetOffsetPtr(buf, hdr->offset_Section2);

		TiXmlElement* node_ListDsc = new TiXmlElement("ListDSC");

		for (size_t i = 0; i < hdr->number_Section2; i++)
		{
			size_t startSection2 = hdr->offset_Section2 + i * sizeof(SPM_Section2); 

			TiXmlElement* node_SPMSection2 = new TiXmlElement("DSC");

			if (section2[i].offset_DscFile)
			{
				size_t startDSC = section2[i].offset_DscFile + hdr->offset_Section2;
				DSC_Header* dscHdr = (DSC_Header*)GetOffsetPtr(buf, startDSC);

				node = new TiXmlElement("number_Section1"); node->SetAttribute("u32", UnsignedToString(dscHdr->number_Section1, false)); node_SPMSection2->LinkEndChild(node);
				node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(dscHdr->unk_0, true)); node_SPMSection2->LinkEndChild(node);
				node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(dscHdr->unk_1, true)); node_SPMSection2->LinkEndChild(node);
				node = new TiXmlElement("offset_Section1"); node->SetAttribute("startOffset", UnsignedToString(dscHdr->offset_Section1, true)); node_SPMSection2->LinkEndChild(node);
				node = new TiXmlElement("unk_3"); node->SetAttribute("u32", UnsignedToString(dscHdr->unk_3, true)); node_SPMSection2->LinkEndChild(node);


				TiXmlElement* node_DscListSection1 = new TiXmlElement("ListSect_1");

				if (dscHdr->offset_Section1)
				{
					size_t startDscListSect_1 = dscHdr->offset_Section1 + startDSC;

					DSC_Section1* dsc_Section1 = (DSC_Section1*)GetOffsetPtr(buf, startDscListSect_1);

					for (size_t j = 0; j < dscHdr->number_Section1; j++)
					{
						offset = startDscListSect_1 + j * sizeof(DSC_Section1);

						TiXmlElement* node_DscSection1 = new TiXmlElement("Sect_1");

						node = new TiXmlElement("offset_Section1_b"); node->SetAttribute("startOffset", UnsignedToString(dsc_Section1[j].offset_Section1_b, true)); node_DscSection1->LinkEndChild(node);
						node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(dsc_Section1[j].unk_0, true)); node_DscSection1->LinkEndChild(node);
						
						if (dsc_Section1[j].offset_Section1_b)
						{
							size_t offsetSection1b = dsc_Section1[j].offset_Section1_b + startDscListSect_1;
							offset = offsetSection1b;

							DSC_Section1_b* dsc_Section1_b = (DSC_Section1_b*)GetOffsetPtr(buf, offsetSection1b);

							TiXmlElement* node_DscSection1_b = new TiXmlElement("Sect_1_b");

							/*
							string str = dsc_Section1_b->name;
							if (str.length() >= 16)
								str = str.substr(0, 16);
							node = new TiXmlElement("name"); node->SetAttribute("string", str); node_DscSection1_b->LinkEndChild(node);
							*/

							std::vector<string> fullNames;
							string str = "";
							string char_str_tmp = "a";
							bool isOnString = false;
							for (size_t j = 0; j < 16; ++j)
							{
								if (dsc_Section1_b->name[j] == '\0')			//if it's a empty char
								{
									if (!isOnString)			//if char is '\0' (same for previous), name is "".
									{
										fullNames.push_back("");
									}else if (isOnString) {				//if previously , we ahve a begin if a world, so we are at the end. so finish add action

										fullNames.push_back(str);
										str = "";
										isOnString = false;
										continue;
									}

								}else {								//if it's not a empty char

									char_str_tmp[0] = dsc_Section1_b->name[j];
									str += char_str_tmp;
									isOnString = true;
								}
							}
							if (isOnString)								//last world witch don't have '\0' to end because of limited number of char.
								fullNames.push_back(str);


							node = new TiXmlElement("name");
							node->SetAttribute("main", fullNames.at(0));
							size_t nbStr = fullNames.size();
							for (size_t j = 1; j<nbStr; ++j)
								node->SetAttribute("sub_" + std::to_string(j), fullNames.at(j));
							node_DscSection1_b->LinkEndChild(node);


							node = new TiXmlElement("number_Section1_c"); node->SetAttribute("value", UnsignedToString(dsc_Section1_b->number_Section1_c, false)); node_DscSection1_b->LinkEndChild(node);
							node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(dsc_Section1_b->unk_0, true)); node_DscSection1_b->LinkEndChild(node);
							node = new TiXmlElement("size_Section1_c"); node->SetAttribute("size", UnsignedToString(dsc_Section1_b->size_Section1_c, true)); node_DscSection1_b->LinkEndChild(node);
							node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(dsc_Section1_b->unk_1, true)); node_DscSection1_b->LinkEndChild(node);
							node = new TiXmlElement("offset_Section1_c"); node->SetAttribute("startOffset", UnsignedToString(dsc_Section1_b->offset_Section1_c, true)); node_DscSection1_b->LinkEndChild(node);
							node = new TiXmlElement("unk_2"); node->SetAttribute("u32", UnsignedToString(dsc_Section1_b->unk_2, true)); node_DscSection1_b->LinkEndChild(node);

							if (dsc_Section1_b->offset_Section1_c >= sizeof(DSC_Section1_b))			//some file don't have index.
							{
								node = new TiXmlElement("index"); node->SetAttribute("u32", UnsignedToString(dsc_Section1_b->index, true)); node_DscSection1_b->LinkEndChild(node);
								node = new TiXmlElement("unk_3"); node->SetAttribute("u32", UnsignedToString(dsc_Section1_b->unk_3, true)); node_DscSection1_b->LinkEndChild(node);
							}

							offset_tmp = dsc_Section1_b->offset_Section1_c;
							if (offset_tmp)
							{
								offset_tmp += offsetSection1b;
								size_t offsetSection1c = offset_tmp;

								TiXmlElement* node_DscListSection1_c = new TiXmlElement("ListSect_1_c");

								for (size_t k = 0; k < dsc_Section1_b->number_Section1_c; k++)
								{
									TiXmlElement* node_DscSection1_c = new TiXmlElement("Sect_1_c");
									
									offset = offsetSection1c + k * dsc_Section1_b->size_Section1_c;
									DSC_Section1_c* dsc_Section1_c = (DSC_Section1_c*)GetOffsetPtr(buf, offset);

									node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(dsc_Section1_c->unk_0, true)); node_DscSection1_c->LinkEndChild(node);
									node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(dsc_Section1_c->unk_1, true)); node_DscSection1_c->LinkEndChild(node);
									node = new TiXmlElement("unk_2"); node->SetAttribute("u32", UnsignedToString(dsc_Section1_c->unk_2, true)); node_DscSection1_c->LinkEndChild(node);
									node = new TiXmlElement("unk_3"); node->SetAttribute("u32", UnsignedToString(dsc_Section1_c->unk_3, true)); node_DscSection1_c->LinkEndChild(node);
									node = new TiXmlElement("unk_4"); node->SetAttribute("u32", UnsignedToString(dsc_Section1_c->unk_4, true)); node_DscSection1_c->LinkEndChild(node);
									
									node = new TiXmlElement("Color"); 
									node->SetAttribute("R", to_string_with_precision(dsc_Section1_c->color[0],8));
									node->SetAttribute("G", to_string_with_precision(dsc_Section1_c->color[1],8));
									node->SetAttribute("B", to_string_with_precision(dsc_Section1_c->color[2],8));
									node_DscSection1_c->LinkEndChild(node);


									node_DscListSection1_c->LinkEndChild(node_DscSection1_c);
								}

								node_DscSection1_b->LinkEndChild(node_DscListSection1_c);
							}

							node_DscSection1->LinkEndChild(node_DscSection1_b);
						}
						node_DscListSection1->LinkEndChild(node_DscSection1);
					}
				}
				node_SPMSection2->LinkEndChild(node_DscListSection1);
			}
			node_ListDsc->LinkEndChild(node_SPMSection2);
		}
		parent->LinkEndChild(node_ListDsc);
	}

	if (isVersion_E)
		delete hdr;
}







/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// wxHexEditor coloration ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////




/*-------------------------------------------------------------------------------\
|                             save_Coloration		                             |
\-------------------------------------------------------------------------------*/
void Spm::save_Coloration(string filename, bool show_error)
{
	uint8_t *buf;
	size_t size;

	buf = ReadFile(filename, &size, show_error);
	if (!buf)
		return;

	TiXmlDocument *doc = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	doc->LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("wxHexEditor_XML_TAG");


	TiXmlElement* filename_node = new TiXmlElement("filename");
	EMO_BaseFile::WriteParamString(filename_node, "path", filename);



	write_Coloration(filename_node, buf, size);



	root->LinkEndChild(filename_node);
	doc->LinkEndChild(root);



	delete[] buf;

	doc->SaveFile(filename + ".tags");

	return;
}




/*-------------------------------------------------------------------------------\
|                             write_Coloration				                     |
\-------------------------------------------------------------------------------*/
void Spm::write_Coloration(TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	SPMHeader_E* hdr_e = (SPMHeader_E*)buf;
	if (size < sizeof(SPMHeader_E))
		return;

	bool isVersion_X = (memcmp(hdr_e->signature, SPM_SIGNATURE, 4) == 0);
	bool isVersion_E = (memcmp(hdr_e->signature, SPM_SIGNATURE_E, 4) == 0);
	
	if ((!isVersion_X) && (!isVersion_E))
		return;

	SPMHeader_X* hdr = 0;
	if (isVersion_E)
	{
		hdr = new SPMHeader_X();
		hdr->version = hdr_e->version;
		hdr->number_Section1 = hdr_e->number_Section1;
		hdr->size_Section1 = hdr_e->size_Section1;
		hdr->offset_Section1 = hdr_e->offset_Section1;
		hdr->number_Section2 = 0;
		hdr->offset_Section2 = 0;
	}else{
		hdr = (SPMHeader_X*)buf;
	}

	

	std::vector<bool> listBytesAllreadyTagged;
	listBytesAllreadyTagged.resize(size, false);				//to check override of the same byte (overflow)


	size_t idTag = 0;
	size_t offset = 0;
	size_t incSection = 0;
	size_t incParam = 0;

	write_Coloration_Tag("signature", "string", "", offset, 4 * sizeof(char), "SPMHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);	offset += 4 * sizeof(char);
	write_Coloration_Tag("version", "uint32_t", "", offset, sizeof(uint32_t), "SPMHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	
	write_Coloration_Tag("number_Section1", "uint32_t", "maximum 8 (NLBY)", offset, sizeof(uint32_t), "SPMHeader", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("size_Section1", "uint32_t", "", offset, sizeof(uint32_t), "SPMHeader", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("offset_Section1", "uint32_t", "", offset, sizeof(uint32_t), "SPMHeader", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

	if (!isVersion_E)
	{
		write_Coloration_Tag("number_Section2", "uint32_t", "always 1 (if X version)", offset, sizeof(uint32_t), "SPMHeader", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged);	offset += sizeof(uint32_t);
		write_Coloration_Tag("offset_Section2", "uint32_t", "", offset, sizeof(uint32_t), "SPMHeader", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged);	offset += sizeof(uint32_t);
	}



	SPM_Section1 *section1 = (SPM_Section1*)GetOffsetPtr(buf, hdr->offset_Section1);
	offset = hdr->offset_Section1;
	size_t offset_tmp = 0;
	incSection++;
	for (size_t i = 0; i < hdr->number_Section1; i++)
	{
		size_t offsetSection1 = hdr->offset_Section1 + i * hdr->size_Section1;
		offset = offsetSection1;

		incParam = 0;
		write_Coloration_Tag("name", "string", "interresting Names : all stages names, Scouter_Blue, toFLBY01, BFnms_LightS_Off, BFice_water", offset, 16 * sizeof(char), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += 16 * sizeof(char);

		write_Coloration_Tag("lenFlareEnabled", "uint16_t", "always 1", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_1", "uint16_t", "0, 1 (most)", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_2", "uint16_t", "always 0", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_3", "uint16_t", "0, 1 (most)", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_4", "uint16_t", "0 (most), 1", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_5", "uint16_t", "0, 1 (most)", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_6", "uint16_t", "0 (most), 1", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_7", "uint16_t", "always 0", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);

		for (size_t j=0; j < 128; j++)
		{
			write_Coloration_Tag("unk_8__"+ std::to_string(j), "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		}

		write_Coloration_Tag("unk_9", "uint16_t", "always 1", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_10", "uint16_t", "always 0", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_11", "uint16_t", "0 (most),1, 4, 5, 6, 7, 8, 9, 0xa,   0x100, 0x101,0x102, 0x103, 0x104, 0x105, 0x106, seam to be 2 x uint8", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_12", "uint16_t", "always 0xffff", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_13", "uint16_t", "0, 1 (most)", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_14", "uint16_t", "always 0", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_15", "uint16_t", "0, 1 (most)", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_16", "uint16_t", "0 (most), 1", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);

		for (size_t j = 0; j < 56; j++)
		{
			write_Coloration_Tag("unk_17__" + std::to_string(j), "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		}

		write_Coloration_Tag("unk_18", "uint16_t", "always 1", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_19", "uint16_t", "always 0", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_20", "uint16_t", "same as unk_11", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_21", "uint16_t", "same as unk_12", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_22", "uint16_t", "same as unk_13", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_23", "uint16_t", "same as unk_14", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_24", "uint16_t", "same as unk_15", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_25", "uint16_t", "same as unk_16", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);

		

		
		// ...
		

		/*
		//offsets
		offset_tmp = section1[i].offsetSection2;
		if (offset_tmp)
		{
			offset_tmp += offsetSection1;
			offset = offset_tmp;
			
			uint32_t* tmp = (uint32_t*)GetOffsetPtr(buf, offset);

			for (size_t j = 0; j < section1[i].numberSection2; j++)
			{
				write_Coloration_Tag("offsetSection2b", "uint32_t", "Offset (add with the start of Section1[" + std::to_string(i) + "] ), target a section2 => " + UnsignedToString(tmp[j] + offsetSection1, true), offset, sizeof(uint32_t), "Section1b", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
				listOffsetfromSection1ToSection2.push_back(tmp[j] + offsetSection1);			// we will made the link on header of Section2
			}
		}
		*/
	}




	SPM_Section2 *section2 = (SPM_Section2*)GetOffsetPtr(buf, hdr->offset_Section2);
	offset = hdr->offset_Section2;
	incSection += 2;			//todo update depend of Section1
	incParam = 0;
	for (size_t i = 0; i < hdr->number_Section2; i++)
	{
		size_t offsetSection2 = hdr->offset_Section2 + i * sizeof(SPM_Section2);
		offset = offsetSection2;

		write_Coloration_Tag("offset_DscFile", "uint32_t", "Start of Section2["+ std::to_string(i) +"] => " + UnsignedToString(hdr->offset_Section2 + section2[i].offset_DscFile, true), offset, sizeof(uint32_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	}



	offset = hdr->offset_Section2;
	incSection++;
	for (size_t i = 0; i < hdr->number_Section2; i++)
	{
		size_t offsetSection2 = hdr->offset_Section2 + i * sizeof(SPM_Section2);
		size_t offsetDsc = section2[i].offset_DscFile + hdr->offset_Section2;
		offset = offsetDsc;


		DSC_Header* dsc_hdr = (DSC_Header*)GetOffsetPtr(buf, offsetDsc);

		incParam = 0;
		write_Coloration_Tag("start_DscFile, signature", "uint32_t", "Start of Dsc[" + std::to_string(i) + "]", offset, sizeof(uint32_t), "Dsc_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		write_Coloration_Tag("number_Section1", "uint32_t", "1 is most, 7 is max, 0 is min", offset, sizeof(uint32_t), "Dsc_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_0", "uint32_t", "always 0x30", offset, sizeof(uint32_t), "Dsc_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_1", "uint32_t", "Todo redo analyze", offset, sizeof(uint32_t), "Dsc_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		write_Coloration_Tag("offset_Section1", "uint32_t", "", offset, sizeof(uint32_t), "Dsc_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_3", "uint32_t", "always 0", offset, sizeof(uint32_t), "Dsc_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);


		//offsets
		offset_tmp = dsc_hdr->offset_Section1;
		if (offset_tmp)
		{
			offset_tmp += offsetDsc;
			size_t offsetSection1 = offset_tmp;

			DSC_Section1* dsc_Section1 = (DSC_Section1*)GetOffsetPtr(buf, offsetSection1);

			for (size_t j = 0; j < dsc_hdr->number_Section1; j++)
			{
				offset = offsetSection1 + j * sizeof(DSC_Section1);

				incParam = 0;
				write_Coloration_Tag("offset_Section1_b", "uint32_t", "Offset (add with the start of Dsc), => " + UnsignedToString(dsc_Section1[j].offset_Section1_b + offsetSection1, true), offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
				write_Coloration_Tag("unk_0", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);

				offset_tmp = dsc_Section1[j].offset_Section1_b;
				if (offset_tmp)
				{
					offset_tmp += offsetSection1;
					size_t offsetSection1b = offset_tmp;
					offset = offsetSection1b;
					
					DSC_Section1_b* dsc_Section1_b = (DSC_Section1_b*)GetOffsetPtr(buf, offsetSection1b);

					incParam = 0;
					write_Coloration_Tag("name", "string", " same as SpmSection1", offset, 16 * sizeof(char), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += 16 * sizeof(char);
					write_Coloration_Tag("number_Section1_c", "uint32_t", "1 (most) or 2", offset, sizeof(uint32_t), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
					write_Coloration_Tag("unk_0", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
					write_Coloration_Tag("size_Section1_c", "uint32_t", " 0x28[11,0,3,0,4,0,2,5;11,0,3,0,4,1,2,5] |0x30 (most) [0,0,3,0,4,0,2,5; ... x 99] So look the difference ", offset, sizeof(uint32_t), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
					write_Coloration_Tag("unk_1", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
					write_Coloration_Tag("offset_Section1_c", "uint32_t", " add start Section1_b => "+ UnsignedToString(dsc_Section1_b->offset_Section1_c + offsetSection1b, true), offset, sizeof(uint32_t), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
					write_Coloration_Tag("unk_2", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);

					if (dsc_Section1_b->offset_Section1_c >= sizeof(DSC_Section1_b))			//some file don't have index.
					{
						write_Coloration_Tag("index", "uint32_t", " Same values as DscSection1_c and a parameter into SpmSection1", offset, sizeof(uint32_t), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
						write_Coloration_Tag("unk_3", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
					}

					offset_tmp = dsc_Section1_b->offset_Section1_c;
					if (offset_tmp)
					{
						offset_tmp += offsetSection1b;
						size_t offsetSection1c = offset_tmp;

						DSC_Section1_c* dsc_Section1_c = (DSC_Section1_c*)GetOffsetPtr(buf, offsetSection1c);

						for (size_t k = 0; k < dsc_Section1_b->number_Section1_c; k++)
						{
							offset = offsetSection1c + k * dsc_Section1_b->size_Section1_c;

							incParam = 0;
							write_Coloration_Tag("unk_0", "uint32_t", "0,1,4,5,6,7,8,9,0xa,0x100,0x101,0x102,0x103,0x104,0x105, 0xffffffff(none) same as a part in Spm Section1", offset, sizeof(uint32_t), "Section1_c", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint32_t);
							write_Coloration_Tag("unk_1", "uint32_t", " 0, 0xffffffff (most, none)", offset, sizeof(uint32_t), "Section1_c", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint32_t);
							write_Coloration_Tag("unk_2", "uint32_t", "0 (most) or 1", offset, sizeof(uint32_t), "Section1_c", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint32_t);
							write_Coloration_Tag("unk_3", "uint32_t", "0 (most), 0xffffffff (none)", offset, sizeof(uint32_t), "Section1_c", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint32_t);
							write_Coloration_Tag("unk_4", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1_c", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint32_t);
							write_Coloration_Tag("Color", "float[3]", " RGB always at 0 0 0", offset, 3 * sizeof(float), "Section1_c", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, k); offset += 3 * sizeof(float);
						}
					}
				}
			}
		}
	}





	if (isVersion_E)
		delete hdr;
}





/*-------------------------------------------------------------------------------\
|                             write_Coloration_Tag			                     |
\-------------------------------------------------------------------------------*/
void Spm::write_Coloration_Tag(string paramName, string paramType, string paramComment, size_t startOffset, size_t size, string sectionName, TiXmlElement* parent, size_t idTag, size_t sectionIndex, size_t paramIndex, std::vector<bool> &listBytesAllreadyTagged, size_t sectionIndexInList, bool checkAllreadyTaggued)
{
	TiXmlElement* tag_node = new TiXmlElement("TAG");

	tag_node->SetAttribute("id", UnsignedToString(idTag, false));


	TiXmlElement* start_offset_node = new TiXmlElement("start_offset");
	TiXmlText* text = new TiXmlText(std::to_string(startOffset));
	start_offset_node->LinkEndChild(text);
	tag_node->LinkEndChild(start_offset_node);

	TiXmlElement* end_offset_node = new TiXmlElement("end_offset");
	text = new TiXmlText(std::to_string(startOffset + size - 1));
	end_offset_node->LinkEndChild(text);
	tag_node->LinkEndChild(end_offset_node);

	TiXmlElement* text_node = new TiXmlElement("tag_text");
	text = new TiXmlText(sectionName + ((sectionIndexInList != (size_t)-1) ? "[" + std::to_string(sectionIndexInList) + "]" : "") + "." + paramName + " (" + paramType + ") : " + paramComment);
	text_node->LinkEndChild(text);
	tag_node->LinkEndChild(text_node);





	if (listTagColors.size() == 0)
	{
		listTagColors.push_back(std::vector<std::vector<string>>());
		listTagColors.back().push_back(std::vector<string>());
		listTagColors.back().back().push_back("#000000");			//background color
		listTagColors.back().back().push_back("#FFFFFF");			//font color (just to possibility read from background color)
	}

	size_t sectionIndex_tmp = sectionIndex % listTagColors.size();
	std::vector<std::vector<string>> &sectionColorlist = listTagColors.at(sectionIndex_tmp);
	size_t paramIndex_tmp = paramIndex % sectionColorlist.size();
	std::vector<string> &paramColors = sectionColorlist.at(paramIndex_tmp);

	TiXmlElement* font_colour_node = new TiXmlElement("font_colour");
	text = new TiXmlText(paramColors.at(1));
	font_colour_node->LinkEndChild(text);
	tag_node->LinkEndChild(font_colour_node);

	TiXmlElement* bg_colour_node = new TiXmlElement("note_colour");
	text = new TiXmlText(paramColors.at(0));
	bg_colour_node->LinkEndChild(text);
	tag_node->LinkEndChild(bg_colour_node);

	parent->LinkEndChild(tag_node);




	//un check d'overide , pour savoir si des blocks se chevauche.
	size_t index;
	size_t limit = listBytesAllreadyTagged.size();
	for (size_t i = 0; i < size; i++)
	{
		index = startOffset + i;
		if (index >= limit)
		{
			printf("Error on tagID %i : overflow %i >= %i.\n", idTag, index, limit);
			continue;
		}

		if (index == 0x11ac)			//for test
			int aa = 42;

		if ((checkAllreadyTaggued) && (listBytesAllreadyTagged.at(index)))
			printf("warning on tagID %i : the byte %i allready taggued, may be a overflow between blocks. Infos : %s. \n", idTag, index, (sectionName + ((sectionIndexInList != (size_t)-1) ? "[" + std::to_string(sectionIndexInList) + "]" : "") + "." + paramName + " (" + paramType + ") : " + paramComment).c_str() );

		listBytesAllreadyTagged.at(index) = true;
	}
}


}