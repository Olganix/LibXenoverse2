#include "Scd.h"



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
|                             Scd												 |
\-------------------------------------------------------------------------------*/
Scd::Scd()
{
	//section 0 - SCDHeader : black/grey
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#232323");			//background color
	listTagColors.back().back().push_back("#FFFFFF");			//font color (just to possibility read from background color)
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#464646");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 1 - SCDUnk0AEntry : red
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC0000");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC7777");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 2 - SCDUnk0BEntry : green
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#00CC00");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#77CC77");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 3 - SCDUnk1Entry : bleu
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#0000CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7777CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f




	//section 5 - SCDUnk2Entry : yellow
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCC00");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCC77");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 6 - SCDUnk3Entry : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#00CCCC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#77CCCC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 7 - SCDUnk3Entry indexes : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#009999");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#339999");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 8 - SCDUnk4Entry : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC00CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC77CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 9 - SCDUnk4Entry_Next : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#990099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#993399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f



	//section 10 - SCDUnk4Entry_Next_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 11 - SCDUnk4Entry_Next_c : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC0099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC3399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 12 - SCDUnk4Entry_Next_d : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900AA");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933AA");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 13 - SCDUnk4Entry_Next_e : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA0099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA3399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f



	//section 14 - SCDUnk4Entry_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#770077");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#773377");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 15 - SCDUnk4Entry_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#550055");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#553355");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f





	//section 16 - SCDUnk4Entry_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA00FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA33FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 17 - SCDUnk4Entry_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 18 - SCDUnk4Entry_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7700FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7733FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 19 - SCDUnk4Entry_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#5500FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#5533FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f




	//section 20 - SCDUnk5Entry : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCCCC");			//bg
	listTagColors.back().back().push_back("#000000");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AAAAAA");			//bg
	listTagColors.back().back().push_back("#000000");			//f


	//section 21 - SCDUnk5Entry_Box : 
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
void Scd::Reset()
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
void Scd_Part::Reset()
{
	// Todo
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*
bool Scd::Load(const uint8_t *buf, size_t size)
{
	Reset();

	SCDHeader *hdr = (SCDHeader *)buf;

	if (size < sizeof(SCDHeader) || memcmp(hdr->signature, SCD_SIGNATURE, 4) != 0)
		return false;

	
	//Section1 make reference to Section2. so we begin by section2.
	size_t offset = 0;
	for (size_t i = 0; i < hdr->nbSection2; i++)
	{
		offset = hdr->offsetSection2 + i * sizeof(SCD_Section2);

		Scd_TextureUnit* textUnit = new Scd_TextureUnit();
		if( textUnit->Load( GetOffsetPtr(buf, offset), size - val32(offset) ) )
			mListTextureUnit.push_back(textUnit);
	}


	for (size_t i = 0; i < hdr->nbSection1; i++)
	{
		offset = hdr->offsetSection1 + i * sizeof(SCD_Section1);

		Scd_Part part;
		if (part.Load(GetOffsetPtr(buf, offset), size - val32(offset), hdr->offsetSection2 - offset, mListTextureUnit))
			mListPart.push_back(part);
	}

	return true;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////











/*-------------------------------------------------------------------------------\
|                             calculeFilesize		                             |
\-------------------------------------------------------------------------------*
size_t Scd::calculeFilesize(bool withTextureUnits)
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
uint8_t* Scd::CreateFile(unsigned int *psize)
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
\-------------------------------------------------------------------------------*
size_t Scd::write(uint8_t *buf, size_t size)
{
	SCDHeader* hdr = (SCDHeader*)buf;
	size_t offset = 0;

	strcpy(&(hdr->signature[0]), SCD_SIGNATURE);
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
		offset = hdr->offsetSection1 + i * sizeof(SCD_Section1);
		mListPart.at(i).write_Main(buf + offset, size - offset);
	}

	size_t startSection1Offset = 0;
	offset = hdr->offsetSection1 + hdr->nbSection1 * sizeof(SCD_Section1);
	for (size_t i = 0; i < hdr->nbSection1; i++)
	{
		startSection1Offset = hdr->offsetSection1 + i * sizeof(SCD_Section1);

		*((uint32_t*)GetOffsetPtr(buf, startSection1Offset + 0xAC, true)) = (mListPart.at(i).mListAnimationA.size()) ? (offset - startSection1Offset - 0xA8) : 0;		//setup section1->offsetSection5;
		offset += mListPart.at(i).write_AnimationsA(buf + offset, size - offset);


		*((uint32_t*)GetOffsetPtr(buf, startSection1Offset + 0xA4, true)) = (mListPart.at(i).mListAnimationB.size()) ? (offset - startSection1Offset - 0x78) : 0;		//setup section1->offsetSection4;
		offset += mListPart.at(i).write_AnimationsB(buf + offset, size - offset);
	}

	
	for (size_t i = 0; i < hdr->nbSection1; i++)
	{
		startSection1Offset = hdr->offsetSection1 + i * sizeof(SCD_Section1);

		*((uint32_t*)GetOffsetPtr(buf, startSection1Offset + 0x70, true)) = (mListPart.at(i).mListTextureUnit.size()) ? (offset - startSection1Offset) : 0;		//setup section1->offsetSection2;
		offset += mListPart.at(i).write_TextureUnitPointers(buf + offset, size - offset, hdr->offsetSection2 - (hdr->offsetSection1 + i * sizeof(SCD_Section1)) , mListTextureUnit);
	}


	for (size_t i = 0; i < hdr->nbSection1; i++)
	{
		startSection1Offset = hdr->offsetSection1 + i * sizeof(SCD_Section1);

		*((uint32_t*)GetOffsetPtr(buf, startSection1Offset + 0x74, true)) = offset - startSection1Offset;		//setup section1->offsetSection3;
		offset += mListPart.at(i).write_Extrude(buf + offset, size - offset);
	}


	//TextureUnits
	offset = hdr->offsetSection2;
	for (size_t i = 0; i < hdr->nbSection2; i++)
	{
		offset = hdr->offsetSection2 + i * sizeof(SCD_Section2);
		mListTextureUnit.at(i)->write_Main(buf + offset, size - offset);
	}

	size_t startSection2Offset = 0;
	offset = hdr->offsetSection2 + hdr->nbSection2 * sizeof(SCD_Section2);
	for (size_t i = 0; i < hdr->nbSection2; i++)
	{
		if ((mListTextureUnit.at(i)->mUseSpeed) || (mListTextureUnit.at(i)->mListKeyframe.size() <=1 ))
			continue;

		startSection2Offset = hdr->offsetSection2 + i * sizeof(SCD_Section2);
		
		*((uint32_t*)GetOffsetPtr(buf, startSection2Offset + 0x18, true)) = (mListTextureUnit.at(i)->mListKeyframe.size()) ? (offset - startSection2Offset - 0xC) : 0;		//Section2_b_02->offsetSection2_02_c;
		offset += mListTextureUnit.at(i)->write_Animation(buf + offset, size - offset);
	}

	return offset;
}




















/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/*-------------------------------------------------------------------------------\
|                             saveXml											 |
\-------------------------------------------------------------------------------*
void Scd::saveXml(string filename)
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
TiXmlElement* Scd::export_Xml()
{
	TiXmlElement* mainNode = new TiXmlElement("Scd");
	
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
bool Scd::Compile(TiXmlDocument *doc, bool big_endian)
{
	this->big_endian = big_endian;

	std::vector<size_t> listBytes;

	TiXmlHandle handle(doc);
	TiXmlElement* rootNode = EMO_BaseFile::FindRoot(&handle, "Scd");
	if (!rootNode)
	{
		printf("Cannot find\"Scd\" in xml.\n");
		notifyError();
		return false;
	}

	TiXmlElement* node_SCDHeader = rootNode->FirstChildElement("Header");
	if (!node_SCDHeader)
	{
		printf("Cannot find\"Header\" in xml.\n");
		notifyError();
		return false;
	}

	string str = "";
	size_t tmp = 0;
	float f_tmp = 0.0f;
	char* buf_names = new char[200];

	


	listBytes.push_back( '#' + ('S' << 8) + ('C' << 16) + ('D' << 24) );	//signature
	node_SCDHeader->QueryStringAttribute("version", &str);
	listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0)));
	node_SCDHeader->QueryStringAttribute("unk_0", &str);
	listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0)));
	node_SCDHeader->QueryStringAttribute("unk_1", &str);
	listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0)));

	size_t offset = listBytes.size();

	for (TiXmlElement* node_tmp = node_SCDHeader->NextSiblingElement(); node_tmp; node_tmp = node_tmp->NextSiblingElement())
	{
		string tagName = node_tmp->ValueStr();
		if ((tagName != SCD_CFG0_SIGNATURE) && (tagName != SCD_CFG1_SIGNATURE) && (tagName != SCD_BDY0_SIGNATURE) && (tagName != SCD_BDY3_SIGNATURE) && (tagName != SCD_BDY4_SIGNATURE) && (tagName != SCD_BDY5_SIGNATURE) && (tagName != SCD_CSR1_SIGNATURE) && (tagName != SCD_CSR2_SIGNATURE) && (tagName != SCD_ACT0_SIGNATURE))
			continue;

		offset = listBytes.size();

		//header
		listBytes.push_back(tagName.at(0) + (tagName.at(1) << 8) + (tagName.at(2) << 16) + (tagName.at(3) << 24));	//signature
		listBytes.push_back(0);						//fill later

		//part
		std::vector<string> listBoneNames;
		bool stillActiveForBoneNames = true;

		for (TiXmlElement* node_param = node_tmp->FirstChildElement(); node_param; node_param = node_param->NextSiblingElement())
		{
			string paramName = node_param->ValueStr();
			if ((stillActiveForBoneNames) && ((paramName == "Bone") || (paramName == "Bone2")))
			{
				node_param->QueryStringAttribute("name", &str);
				listBoneNames.push_back(str);
				listBytes.push_back(0);						//fill later
				continue;
			}
			stillActiveForBoneNames = false;

			//Normal parameter :
			TiXmlAttribute* attr = node_param->FirstAttribute();
			if (attr)
			{
				string attrName = string(attr->Name());
				if (attrName == "u32")
				{
					node_param->QueryStringAttribute("u32", &str);
					listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0)));

				}else if (attrName == "float") {

					node_param->QueryFloatAttribute("float", &f_tmp);
					listBytes.push_back(val32(*((size_t*)(&f_tmp))));
				}
			}
			

		}

		size_t nbBones = listBoneNames.size();
		size_t startNameOffset = listBytes.size();
		size_t sizeNames = 0;
		memset(buf_names, 0, 200);

		for (size_t i = 0; i < nbBones; i++)
		{
			listBytes.at(offset + 2 + i) = val32( (startNameOffset - offset) * 4 + sizeNames);			//update the nameOffset

			str = listBoneNames.at(i);
			if (sizeNames + str.length() >= 200)
				str = str.substr(0, 200 - sizeNames);
			
			size_t nbChar = str.length();
			for (size_t j = 0; j < nbChar; j++)
				buf_names[sizeNames + j] = str.at(j);

			if (sizeNames + nbChar < 200)
				buf_names[sizeNames + nbChar] = '\0';

			sizeNames += nbChar + 1;
		}

		if (sizeNames != 0)
		{
			size_t nbUint32 = sizeNames / 4;
			if ((nbUint32 * 4 < sizeNames) && (nbUint32 < 50))
				nbUint32++;

			for (size_t i = 0; i < nbUint32; i++)									//inversion because of endian somewhere.
				listBytes.push_back(val32(buf_names[i * 4] + (buf_names[i * 4 + 1] << 8) + (buf_names[i * 4 + 2] << 16) + (buf_names[i * 4 + 3] << 24)));
		}

		/*
		//padding 0 to match uint32_t   , no necessary because it's allready uint32 for record of names
		if (nbBones != 0)
		{
			size_t offset_tmp = listBytes.size();
			if (offset_tmp & 0x3)
			{
				offset_tmp += (0x4 - (offset_tmp & 0x3));
				for (size_t i = listBytes.size(); i < offset_tmp; i++)
					listBytes.push_back(0);
			}
		}
		*/

		// update the size of section.
		listBytes.at(offset + 1) = val32( (listBytes.size() - offset) * 4);
	}


	//END0 section
	listBytes.push_back('E' + ('N' << 8) + ('D' << 16) + ('0' << 24));	//signature
	listBytes.push_back(val32(8));



	delete buf_names;




	//////////////////////////////
	size_t nbSizeT = listBytes.size();
	size_t filesize = nbSizeT * 4;

	uint8_t *buf = new uint8_t[filesize];
	if (!buf)
	{
		printf("%s: Memory allocation error (0x%x)\n", FUNCNAME, filesize);
		LibXenoverse::notifyError();
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
		notifyError();
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
bool Scd::Compile(TiXmlDocument *doc, bool big_endian)
{
	this->big_endian = big_endian;

	TiXmlHandle handle(doc);
	TiXmlElement* rootNode = EMO_BaseFile::FindRoot(&handle, "Scd");

	if (!rootNode)
	{
		printf("Cannot find\"Scd\" in xml.\n");
		notifyError();
		return false;
	}

	return import_Xml(rootNode);
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*
bool Scd::import_Xml(TiXmlElement* rootNode)
{
	Reset();

	TiXmlElement* partsNode = rootNode->FirstChildElement("Parts");
	if (!partsNode)
	{
		printf("Cannot find\"Parts\" in xml.\n");
		notifyError();
		return false;
	}

	TiXmlElement* textureUnitsNode = rootNode->FirstChildElement("TextureUnits");
	if (!textureUnitsNode)
	{
		printf("Cannot find\"TextureUnits\" in xml.\n");
		notifyError();
		return false;
	}


	//Parts make some reference to the TextureUnits, So we have to deal with TextureUnits first.
	for (TiXmlElement* node = textureUnitsNode->FirstChildElement("TextureUnit"); node; node = node->NextSiblingElement("TextureUnit"))
	{
		mListTextureUnit.push_back(new Scd_TextureUnit());
		if (!mListTextureUnit.back()->import_Xml(node))
			mListTextureUnit.pop_back();
	}

	for (TiXmlElement* node = partsNode->FirstChildElement("Part"); node; node = node->NextSiblingElement("Part"))
	{
		mListPart.push_back(Scd_Part());
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
void Scd::save_Xml(string filename, bool show_error)
{
	uint8_t *buf;
	size_t size;

	buf = ReadFile(filename, &size, show_error);
	if (!buf)
		return;

	TiXmlDocument *doc = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	doc->LinkEndChild(decl);

	TiXmlElement* root = new TiXmlElement("Scd");
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
void Scd::write_Xml(TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	SCD_Header* hdr = (SCD_Header*)buf;
	if ((size < sizeof(SCD_Header)) || (memcmp(hdr->signature, SCD_SIGNATURE, 4) != 0))
		return;

	TiXmlElement* node;

	TiXmlElement* node_SCDHeader = new TiXmlElement("Header");
	node_SCDHeader->SetAttribute("version", UnsignedToString(hdr->version, true)); 
	node_SCDHeader->SetAttribute("unk_0", UnsignedToString(hdr->unk_0, true));
	node_SCDHeader->SetAttribute("unk_1", UnsignedToString(hdr->unk_1, true));
	parent->LinkEndChild(node_SCDHeader);
	

	size_t offset = sizeof(SCD_Header);
	size_t offset_tmp = 0;

	while (offset < size)
	{
		size_t startOffset_PartHeader = offset;
		SCD_Part_Header* partHdr = (SCD_Part_Header*)GetOffsetPtr(buf, startOffset_PartHeader);
		offset += sizeof(SCD_Part_Header);

		if (memcmp(partHdr->signature, SCD_END0_SIGNATURE, 4) == 0)				//End of SCD Files
			break;


		string str = string(partHdr->signature).substr(0, 4);
		TiXmlElement* node_part = new TiXmlElement(str);
		node_part->SetAttribute("size", UnsignedToString(partHdr->size, true));
		parent->LinkEndChild(node_part);

		
		if (memcmp(partHdr->signature, SCD_CFG0_SIGNATURE, 4) == 0)
		{
			SCD_CFG0* part = (SCD_CFG0*)GetOffsetPtr(buf, offset);
			node = new TiXmlElement("unk_0"); node->SetAttribute("float", FloatToString(part->unk_0)); node_part->LinkEndChild(node);

		}else if (memcmp(partHdr->signature, SCD_CFG1_SIGNATURE, 4) == 0) {
			SCD_CFG1* part = (SCD_CFG1*)GetOffsetPtr(buf, offset);
			node = new TiXmlElement("unk_0"); node->SetAttribute("float", FloatToString(part->unk_0)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("float", FloatToString(part->unk_1)); node_part->LinkEndChild(node);



		}else if (memcmp(partHdr->signature, SCD_BDY0_SIGNATURE, 4) == 0) {
			
			SCD_BDY0* part = (SCD_BDY0*)GetOffsetPtr(buf, offset);

			string str = string((char*)GetOffsetPtr(buf, part->nameOffset + startOffset_PartHeader));
			node = new TiXmlElement("Bone"); node->SetAttribute("name", str); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(part->unk_0, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("float", FloatToString(part->unk_1)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("u32", UnsignedToString(part->unk_2, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_3"); node->SetAttribute("u32", UnsignedToString(part->unk_3, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_4"); node->SetAttribute("float", FloatToString(part->unk_4)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_5"); node->SetAttribute("float", FloatToString(part->unk_5)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_6"); node->SetAttribute("float", FloatToString(part->unk_6)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_7"); node->SetAttribute("float", FloatToString(part->unk_7)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_8"); node->SetAttribute("float", FloatToString(part->unk_8)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_9"); node->SetAttribute("u32", UnsignedToString(part->unk_9, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_10"); node->SetAttribute("float", FloatToString(part->unk_10)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_11"); node->SetAttribute("float", FloatToString(part->unk_11)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_12"); node->SetAttribute("float", FloatToString(part->unk_12)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_13"); node->SetAttribute("float", FloatToString(part->unk_13)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_14"); node->SetAttribute("float", FloatToString(part->unk_14)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_15"); node->SetAttribute("float", FloatToString(part->unk_15)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_16"); node->SetAttribute("float", FloatToString(part->unk_16)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_17"); node->SetAttribute("float", FloatToString(part->unk_17)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_18"); node->SetAttribute("float", FloatToString(part->unk_18)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_19"); node->SetAttribute("float", FloatToString(part->unk_19)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_20"); node->SetAttribute("u32", UnsignedToString(part->unk_20, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_21"); node->SetAttribute("u32", UnsignedToString(part->unk_21, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_22"); node->SetAttribute("float", FloatToString(part->unk_22)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_23"); node->SetAttribute("u32", UnsignedToString(part->unk_23, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_24"); node->SetAttribute("u32", UnsignedToString(part->unk_24, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_25"); node->SetAttribute("u32", UnsignedToString(part->unk_25, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_26"); node->SetAttribute("u32", UnsignedToString(part->unk_26, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_27"); node->SetAttribute("float", FloatToString(part->unk_27)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_28"); node->SetAttribute("float", FloatToString(part->unk_28)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_29"); node->SetAttribute("float", FloatToString(part->unk_29)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_30"); node->SetAttribute("float", FloatToString(part->unk_30)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_31"); node->SetAttribute("float", FloatToString(part->unk_31)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_32"); node->SetAttribute("float", FloatToString(part->unk_32)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_33"); node->SetAttribute("float", FloatToString(part->unk_33)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_34"); node->SetAttribute("float", FloatToString(part->unk_34)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_35"); node->SetAttribute("float", FloatToString(part->unk_35)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_36"); node->SetAttribute("float", FloatToString(part->unk_36)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_37"); node->SetAttribute("u32", UnsignedToString(part->unk_37, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_38"); node->SetAttribute("u32", UnsignedToString(part->unk_38, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_39"); node->SetAttribute("u32", UnsignedToString(part->unk_39, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_40"); node->SetAttribute("float", FloatToString(part->unk_40)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_41"); node->SetAttribute("float", FloatToString(part->unk_41)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_42"); node->SetAttribute("float", FloatToString(part->unk_42)); node_part->LinkEndChild(node);

		}else if (memcmp(partHdr->signature, SCD_BDY3_SIGNATURE, 4) == 0) {
			
			SCD_BDY3* part = (SCD_BDY3*)GetOffsetPtr(buf, offset);

			string str = string((char*)GetOffsetPtr(buf, part->nameOffset + startOffset_PartHeader));
			node = new TiXmlElement("Bone"); node->SetAttribute("name", str); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(part->unk_0, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(part->unk_1, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("float", FloatToString(part->unk_2)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_3"); node->SetAttribute("u32", UnsignedToString(part->unk_3, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_4"); node->SetAttribute("float", FloatToString(part->unk_4)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_5"); node->SetAttribute("float", FloatToString(part->unk_5)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_6"); node->SetAttribute("u32", UnsignedToString(part->unk_6, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_7"); node->SetAttribute("float", FloatToString(part->unk_7)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_8"); node->SetAttribute("float", FloatToString(part->unk_8)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_9"); node->SetAttribute("u32", UnsignedToString(part->unk_9, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_10"); node->SetAttribute("u32", UnsignedToString(part->unk_10, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_11"); node->SetAttribute("u32", UnsignedToString(part->unk_11, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_12"); node->SetAttribute("u32", UnsignedToString(part->unk_12, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_13"); node->SetAttribute("float", FloatToString(part->unk_13)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_14"); node->SetAttribute("float", FloatToString(part->unk_14)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_15"); node->SetAttribute("float", FloatToString(part->unk_15)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_16"); node->SetAttribute("u32", UnsignedToString(part->unk_16, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_17"); node->SetAttribute("u32", UnsignedToString(part->unk_17, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_18"); node->SetAttribute("u32", UnsignedToString(part->unk_18, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_19"); node->SetAttribute("u32", UnsignedToString(part->unk_19, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_20"); node->SetAttribute("u32", UnsignedToString(part->unk_20, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_21"); node->SetAttribute("u32", UnsignedToString(part->unk_21, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_22"); node->SetAttribute("float", FloatToString(part->unk_22)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_23"); node->SetAttribute("u32", UnsignedToString(part->unk_23, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_24"); node->SetAttribute("float", FloatToString(part->unk_24)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_25"); node->SetAttribute("float", FloatToString(part->unk_25)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_26"); node->SetAttribute("float", FloatToString(part->unk_26)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_27"); node->SetAttribute("float", FloatToString(part->unk_27)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_28"); node->SetAttribute("float", FloatToString(part->unk_28)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_29"); node->SetAttribute("float", FloatToString(part->unk_29)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_30"); node->SetAttribute("u32", UnsignedToString(part->unk_30, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_31"); node->SetAttribute("u32", UnsignedToString(part->unk_31, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_32"); node->SetAttribute("u32", UnsignedToString(part->unk_32, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_33"); node->SetAttribute("float", FloatToString(part->unk_33)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_34"); node->SetAttribute("float", FloatToString(part->unk_34)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_35"); node->SetAttribute("float", FloatToString(part->unk_35)); node_part->LinkEndChild(node);


		}else if (memcmp(partHdr->signature, SCD_BDY4_SIGNATURE, 4) == 0) {


			SCD_BDY4* part = (SCD_BDY4*)GetOffsetPtr(buf, offset);

			string str = string((char*)GetOffsetPtr(buf, part->nameOffset + startOffset_PartHeader));
			node = new TiXmlElement("Bone"); node->SetAttribute("name", str); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(part->unk_0, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("float", FloatToString(part->unk_1)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("float", FloatToString(part->unk_2)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_3"); node->SetAttribute("float", FloatToString(part->unk_3)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_4"); node->SetAttribute("float", FloatToString(part->unk_4)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_5"); node->SetAttribute("float", FloatToString(part->unk_5)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_6"); node->SetAttribute("u32", UnsignedToString(part->unk_6, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_7"); node->SetAttribute("float", FloatToString(part->unk_7)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_8"); node->SetAttribute("float", FloatToString(part->unk_8)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_9"); node->SetAttribute("float", FloatToString(part->unk_9)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_10"); node->SetAttribute("float", FloatToString(part->unk_10)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_11"); node->SetAttribute("float", FloatToString(part->unk_11)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_12"); node->SetAttribute("float", FloatToString(part->unk_12)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_13"); node->SetAttribute("float", FloatToString(part->unk_13)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_14"); node->SetAttribute("float", FloatToString(part->unk_14)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_15"); node->SetAttribute("float", FloatToString(part->unk_15)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_16"); node->SetAttribute("float", FloatToString(part->unk_16)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_17"); node->SetAttribute("u32", UnsignedToString(part->unk_17, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_18"); node->SetAttribute("u32", UnsignedToString(part->unk_18, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_19"); node->SetAttribute("float", FloatToString(part->unk_19)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_20"); node->SetAttribute("u32", UnsignedToString(part->unk_20, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_21"); node->SetAttribute("u32", UnsignedToString(part->unk_21, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_22"); node->SetAttribute("u32", UnsignedToString(part->unk_22, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_23"); node->SetAttribute("float", FloatToString(part->unk_23)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_24"); node->SetAttribute("float", FloatToString(part->unk_24)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_25"); node->SetAttribute("float", FloatToString(part->unk_25)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_26"); node->SetAttribute("float", FloatToString(part->unk_26)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_27"); node->SetAttribute("float", FloatToString(part->unk_27)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_28"); node->SetAttribute("float", FloatToString(part->unk_28)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_29"); node->SetAttribute("float", FloatToString(part->unk_29)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_30"); node->SetAttribute("float", FloatToString(part->unk_30)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_31"); node->SetAttribute("float", FloatToString(part->unk_31)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_32"); node->SetAttribute("float", FloatToString(part->unk_32)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_33"); node->SetAttribute("float", FloatToString(part->unk_33)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_34"); node->SetAttribute("float", FloatToString(part->unk_34)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_35"); node->SetAttribute("float", FloatToString(part->unk_35)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_36"); node->SetAttribute("float", FloatToString(part->unk_36)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_37"); node->SetAttribute("float", FloatToString(part->unk_37)); node_part->LinkEndChild(node);

		}else if (memcmp(partHdr->signature, SCD_BDY5_SIGNATURE, 4) == 0) {
			
			
			SCD_BDY5* part = (SCD_BDY5*)GetOffsetPtr(buf, offset);

			string str = string((char*)GetOffsetPtr(buf, part->nameOffset + startOffset_PartHeader));
			node = new TiXmlElement("Bone"); node->SetAttribute("name", str); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(part->unk_0, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("float", FloatToString(part->unk_1)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("float", FloatToString(part->unk_2)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_3"); node->SetAttribute("float", FloatToString(part->unk_3)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_4"); node->SetAttribute("float", FloatToString(part->unk_4)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_5"); node->SetAttribute("float", FloatToString(part->unk_5)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_6"); node->SetAttribute("float", FloatToString(part->unk_6)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_7"); node->SetAttribute("float", FloatToString(part->unk_7)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_8"); node->SetAttribute("float", FloatToString(part->unk_8)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_9"); node->SetAttribute("u32", UnsignedToString(part->unk_9, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_10"); node->SetAttribute("float", FloatToString(part->unk_10)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_11"); node->SetAttribute("float", FloatToString(part->unk_11)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_12"); node->SetAttribute("float", FloatToString(part->unk_12)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_13"); node->SetAttribute("float", FloatToString(part->unk_13)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_14"); node->SetAttribute("float", FloatToString(part->unk_14)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_15"); node->SetAttribute("float", FloatToString(part->unk_15)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_16"); node->SetAttribute("float", FloatToString(part->unk_16)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_17"); node->SetAttribute("float", FloatToString(part->unk_17)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_18"); node->SetAttribute("float", FloatToString(part->unk_18)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_19"); node->SetAttribute("float", FloatToString(part->unk_19)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_20"); node->SetAttribute("u32", UnsignedToString(part->unk_20, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_21"); node->SetAttribute("u32", UnsignedToString(part->unk_21, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_22"); node->SetAttribute("float", FloatToString(part->unk_22)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_23"); node->SetAttribute("u32", UnsignedToString(part->unk_23, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_24"); node->SetAttribute("u32", UnsignedToString(part->unk_24, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_25"); node->SetAttribute("u32", UnsignedToString(part->unk_25, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_26"); node->SetAttribute("float", FloatToString(part->unk_26)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_27"); node->SetAttribute("float", FloatToString(part->unk_27)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_28"); node->SetAttribute("float", FloatToString(part->unk_28)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_29"); node->SetAttribute("float", FloatToString(part->unk_29)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_30"); node->SetAttribute("float", FloatToString(part->unk_30)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_31"); node->SetAttribute("float", FloatToString(part->unk_31)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_32"); node->SetAttribute("float", FloatToString(part->unk_32)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_33"); node->SetAttribute("float", FloatToString(part->unk_33)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_34"); node->SetAttribute("float", FloatToString(part->unk_34)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_35"); node->SetAttribute("float", FloatToString(part->unk_35)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_36"); node->SetAttribute("u32", UnsignedToString(part->unk_36, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_37"); node->SetAttribute("u32", UnsignedToString(part->unk_37, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_38"); node->SetAttribute("float", FloatToString(part->unk_38)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_39"); node->SetAttribute("float", FloatToString(part->unk_39)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_40"); node->SetAttribute("float", FloatToString(part->unk_40)); node_part->LinkEndChild(node);


		}else if (memcmp(partHdr->signature, SCD_CSR1_SIGNATURE, 4) == 0) {


			SCD_CSR1* part = (SCD_CSR1*)GetOffsetPtr(buf, offset);

			TiXmlComment* comment = new TiXmlComment("Notice: Variant Size (we didn't find why for now)."); node_part->LinkEndChild(comment);

			string str = string((char*)GetOffsetPtr(buf, part->nameOffset + startOffset_PartHeader));
			node = new TiXmlElement("Bone"); node->SetAttribute("name", str); node_part->LinkEndChild(node);
			str = string((char*)GetOffsetPtr(buf, part->nameOffset_2 + startOffset_PartHeader));
			node = new TiXmlElement("Bone2"); node->SetAttribute("name", str); node_part->LinkEndChild(node);

			
			//as there still some trouble about size definition, we try to display on Xml all values we can, by size definition, until the names
			size_t dataSize = part->nameOffset - sizeof(SCD_Part_Header);
			size_t numberU32 = dataSize / sizeof(uint32_t);

			uint32_t* values = (uint32_t*)GetOffsetPtr(buf, offset);
			float* values_f = (float*)GetOffsetPtr(buf, offset);
			for (size_t i = 2; i < numberU32; i++)
			{
				if ((i - 2 == 0) || (i - 2 >= 28))
				{
					node = new TiXmlElement("unk_" + std::to_string(i - 2)); node->SetAttribute("u32", UnsignedToString(values[i], true)); node_part->LinkEndChild(node);
				}else{
					node = new TiXmlElement("unk_" + std::to_string(i - 2)); node->SetAttribute("float", FloatToString(values_f[i])); node_part->LinkEndChild(node);
				}
			}


		}else if (memcmp(partHdr->signature, SCD_CSR2_SIGNATURE, 4) == 0) {


			SCD_CSR2* part = (SCD_CSR2*)GetOffsetPtr(buf, offset);

			TiXmlComment* comment = new TiXmlComment("Notice: Variant Size (we didn't find why for now)."); node_part->LinkEndChild(comment);

			string str = string((char*)GetOffsetPtr(buf, part->nameOffset + startOffset_PartHeader));
			node = new TiXmlElement("Bone"); node->SetAttribute("name", str); node_part->LinkEndChild(node);
			str = string((char*)GetOffsetPtr(buf, part->nameOffset_2 + startOffset_PartHeader));
			node = new TiXmlElement("Bone2"); node->SetAttribute("name", str); node_part->LinkEndChild(node);

			//as there still some trouble about size definition, we try to display on Xml all values we can, by size definition, until the names
			size_t dataSize = part->nameOffset - sizeof(SCD_Part_Header);
			size_t numberU32 = dataSize / sizeof(uint32_t);

			uint32_t* values = (uint32_t*)GetOffsetPtr(buf, offset);
			float* values_f = (float*)GetOffsetPtr(buf, offset);
			for (size_t i = 2; i < numberU32; i++)
			{
				if ((i - 2 == 0) || (i - 2 >= 28))
				{
					node = new TiXmlElement("unk_" + std::to_string(i - 2)); node->SetAttribute("u32", UnsignedToString(values[i], true)); node_part->LinkEndChild(node);
				}
				else {
					node = new TiXmlElement("unk_" + std::to_string(i - 2)); node->SetAttribute("float", FloatToString(values_f[i])); node_part->LinkEndChild(node);
				}
			}

		}else if (memcmp(partHdr->signature, SCD_ACT0_SIGNATURE, 4) == 0) {


			SCD_ACT0* part = (SCD_ACT0*)GetOffsetPtr(buf, offset);

			string str = string((char*)GetOffsetPtr(buf, part->nameOffset + startOffset_PartHeader));
			node = new TiXmlElement("Bone"); node->SetAttribute("name", str); node_part->LinkEndChild(node);
			str = string((char*)GetOffsetPtr(buf, part->nameOffset_2 + startOffset_PartHeader));
			node = new TiXmlElement("Bone2"); node->SetAttribute("name", str); node_part->LinkEndChild(node);

			node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(part->unk_0, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("float", FloatToString(part->unk_1)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("float", FloatToString(part->unk_2)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_3"); node->SetAttribute("float", FloatToString(part->unk_3)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_4"); node->SetAttribute("float", FloatToString(part->unk_4)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_5"); node->SetAttribute("float", FloatToString(part->unk_5)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_6"); node->SetAttribute("float", FloatToString(part->unk_6)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_7"); node->SetAttribute("float", FloatToString(part->unk_7)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_8"); node->SetAttribute("float", FloatToString(part->unk_8)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_9"); node->SetAttribute("float", FloatToString(part->unk_9)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_10"); node->SetAttribute("u32", UnsignedToString(part->unk_10, true)); node_part->LinkEndChild(node);
			node = new TiXmlElement("unk_11"); node->SetAttribute("u32", UnsignedToString(part->unk_11, true)); node_part->LinkEndChild(node);
			
		}else {
			printf("Warning : Unknow signature :%s\n", partHdr->signature);
			LibXenoverse::notifyWarning();
		}

		
		offset = startOffset_PartHeader + partHdr->size;
	}
}







/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// wxHexEditor coloration ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////




/*-------------------------------------------------------------------------------\
|                             save_Coloration		                             |
\-------------------------------------------------------------------------------*/
void Scd::save_Coloration(string filename, bool show_error)
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
void Scd::write_Coloration(TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	std::vector<bool> listBytesAllreadyTagged;
	listBytesAllreadyTagged.resize(size, false);				//to check override of the same byte (overflow)


	SCD_Header* hdr = (SCD_Header*)buf;
	if ((size < sizeof(SCD_Header)) || (memcmp(hdr->signature, SCD_SIGNATURE, 4) != 0))
		return;

	size_t idTag = 0;
	size_t offset = 0;
	size_t incSection = 0;
	size_t incParam = 0;

	write_Coloration_Tag("signature", "string", "", offset, 4 * sizeof(char), "SCDHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);	offset += 4 * sizeof(char);
	write_Coloration_Tag("version", "uint32_t", "", offset, sizeof(uint32_t), "SCDHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_0", "uint32_t", "", offset, sizeof(uint32_t), "SCDHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_1", "uint32_t", "", offset, sizeof(uint32_t), "SCDHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

	incSection+=2;

	while (offset < size)
	{
		size_t startOffset_PartHeader = offset;
		SCD_Part_Header* partHdr = (SCD_Part_Header*)GetOffsetPtr(buf, startOffset_PartHeader);
		
		incParam = 0;
		write_Coloration_Tag("signature", "string", "", offset, 4 * sizeof(char), "PartHeader", parent, idTag++, 1, incParam++, listBytesAllreadyTagged);	offset += 4 * sizeof(char);
		write_Coloration_Tag("size", "uint32_t", "", offset, sizeof(uint32_t), "PartHeader", parent, idTag++, 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);


		if (memcmp(partHdr->signature, SCD_END0_SIGNATURE, 4) == 0)				//End of SCD Files
			break;


		incParam = 0;
		incSection++;

		if (memcmp(partHdr->signature, SCD_CFG0_SIGNATURE, 4) == 0)
		{
			SCD_CFG0* part = (SCD_CFG0*)GetOffsetPtr(buf, offset);
			write_Coloration_Tag("unk_0", "float", "2.0, 10.0 (most) or 20.0", offset, sizeof(float), "CFG_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);

		}else if (memcmp(partHdr->signature, SCD_CFG1_SIGNATURE, 4) == 0) {
			SCD_CFG1* part = (SCD_CFG1*)GetOffsetPtr(buf, offset);
			write_Coloration_Tag("unk_0", "float", "1.0, 5.0 or 10.0 (most)", offset, sizeof(float), "CFG_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_1", "float", "0.0 (most), 0.3 or 0.5", offset, sizeof(float), "CFG_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);


		}else if (memcmp(partHdr->signature, SCD_BDY0_SIGNATURE, 4) == 0) {
			SCD_BDY0* part = (SCD_BDY0*)GetOffsetPtr(buf, offset);
			
			write_Coloration_Tag("nameOffset", "uint32_t", "", offset, sizeof(uint32_t), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_0", "uint32_t", "0, 4 (most), 5, or 6", offset, sizeof(uint32_t), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_1", "float", " [-0.8, 0.0 (most)] ", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_2", "uint32_t", "always 0", offset, sizeof(uint32_t), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_3", "uint32_t", "always 0", offset, sizeof(uint32_t), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_4", "float", " 0.0 (most), 0.577350 or 1.0", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_5", "float", " 0.0 (most), 0.577350 or 1.0", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_6", "float", " 0.0 (most), 0.577350 or 1.0", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_7", "float", "10.0, 20.0 or 180.0 (most)", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_8", "float", "-180.0 (most) or 0.0", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_9", "uint32_t", "1 (most), 2 or 3", offset, sizeof(uint32_t), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_10", "float", "[0.0, 0.1], 0.01 most", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_11", "float", " [-0.01, 0.35], 0.0 most", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_12", "float", " [-0.22, 0.28], 0.0 most", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_13", "float", " [-0.16, 0.1], 0.0 most", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_14", "float", "[-20.0, 90.0], 0.0 most. look like 5 degrees step.", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_15", "float", "-5.0 -2.0 or 0.0 (most)", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_16", "float", " [-90.0, 10.0], 0.0 most. look like 5 degrees step.", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_17", "float", " [0.01, 0.25], 0.05 most.", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_18", "float", " [0.0, 0.12], 0.05 most", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_19", "float", " [-0.03, 0.3], 0.05 most ", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_20", "uint32_t", "always 0", offset, sizeof(uint32_t), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_21", "uint32_t", "always 0", offset, sizeof(uint32_t), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_22", "float", "0.0 (most) or 0.03", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_23", "uint32_t", "always 0", offset, sizeof(uint32_t), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_24", "uint32_t", "1 or 2 (most)", offset, sizeof(uint32_t), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_25", "uint32_t", "0 (most), 3 or 7", offset, sizeof(uint32_t), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_26", "uint32_t", "10 (most), 13 or 19", offset, sizeof(uint32_t), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_27", "float", " [0.1, 15.0], 1.0 most", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_28", "float", " [0.0, 1.0], 0.1 most", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_29", "float", " [0.0, 1.0], 0.1 most", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_30", "float", " [0.0, 1.2], 0.1 most", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_31", "float", " [0.0, 10.0], 5.0 most", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_32", "float", " [0.0, 15.0], 5.0 most", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_33", "float", " [0.0, 50.0], 5.0 most", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_34", "float", " 0.0, 1.0 (most) or 5.0", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_35", "float", " [0.0 (most), 2.0]", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_36", "float", " [0.0(most, 2.0)]", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_37", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_38", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_39", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_40", "float", " [1.0, 500.0], 80.0 most", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_41", "float", "always same values as unk_40, may be a scaleXYZ", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_42", "float", "always same values as unk_40, may be a scaleXYZ", offset, sizeof(float), "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);


			//nameOffset
			string str = string((char*)GetOffsetPtr(buf, part->nameOffset + startOffset_PartHeader));
			write_Coloration_Tag("BoneName", "string", "", part->nameOffset + startOffset_PartHeader, str.length()+ 1, "BDY_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);



		}else if (memcmp(partHdr->signature, SCD_BDY3_SIGNATURE, 4) == 0) {

			SCD_BDY3* part = (SCD_BDY3*)GetOffsetPtr(buf, offset);

			write_Coloration_Tag("nameOffset", "uint32_t", "", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_0", "uint32_t", " always 4", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_1", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_2", "float", " always 1.0", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_3", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_4", "float", " always 180.0", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_5", "float", " always -180.0", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_6", "uint32_t", " always 1", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_7", "float", " 0.0, 0.08 or 0.1 (most)", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_8", "float", " 0.0 (most) or 0.02", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_9", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_10", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_11", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_12", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_13", "float", "0.1 (most), 0.18 or 0.2", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_14", "float", " always 0.02", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_15", "float", " 0.06, 0.08 (most), 0.12, 0.15", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_16", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_17", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_18", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_19", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_20", "uint32_t", " 1 (most) or 4", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_21", "uint32_t", " 10 (most), 19", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_22", "float", " 0.26, 0.32 or 0.4 (most)", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_23", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_24", "float", " always 0.2", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_25", "float", " 0.0, 0.5 or 1.0 (most)", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_26", "float", " always 0.9", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_27", "float", " always 0.9", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_28", "float", " always 1.0", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_29", "float", " 0.26, 0.32 or 4.0 (most)", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_30", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_31", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_32", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_33", "float", " always 5.0", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_34", "float", " always 5.0", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_35", "float", " always 5.0", offset, sizeof(float), "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);

			//nameOffset
			string str = string((char*)GetOffsetPtr(buf, part->nameOffset + startOffset_PartHeader));
			write_Coloration_Tag("BoneName", "string", "", part->nameOffset + startOffset_PartHeader, str.length() + 1, "BDY_3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);






		}else if (memcmp(partHdr->signature, SCD_BDY4_SIGNATURE, 4) == 0) {


			SCD_BDY4* part = (SCD_BDY4*)GetOffsetPtr(buf, offset);

			write_Coloration_Tag("nameOffset", "uint32_t", "", offset, sizeof(uint32_t), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_0", "uint32_t", "4 (most) or 5", offset, sizeof(uint32_t), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_1", "float", " 0.0 (most), 0.577350, or 1.0", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_2", "float", " 0.0, 0.577350, or 1.0 (most)", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_3", "float", " 0.0 (most), 0.577350, or 1.0", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_4", "float", " always 180.0", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_5", "float", " always -180.0", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_6", "uint32_t", " 1 (most), 2 or 3 ", offset, sizeof(uint32_t), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_7", "float", " [0.0, 0.05 (most)]", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_8", "float", " [-0.04, 0.22], 0.0 most ", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_9", "float", " [-0.06, 0.13], 0.0 most ", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_10", "float", " -0.16, -0.005 or 0.0 (most) ", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_11", "float", " always 0.0", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_12", "float", " -5.0, -2.0 or 0.0 (most)", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_13", "float", " [0.0 (most), 30.0]", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_14", "float", " [0.02, 0.16], 0.06 most", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_15", "float", " [0.0, 0.06], 0.01 most", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_16", "float", " [-0.03, 0.3], 0.01 most", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_17", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_18", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_19", "float", " 0.0 (most), 0.03", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_20", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_21", "uint32_t", " 1 (most) or 4", offset, sizeof(uint32_t), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_22", "uint32_t", " 10 (most), 13 or 19", offset, sizeof(uint32_t), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_23", "float", " [1.0 (most), 16.0]", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_24", "float", " [0.0 (most), 1.0]", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_25", "float", " [0.0, 1.0], 0.2 most", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_26", "float", " [0.0 (most), 1.0]", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_27", "float", " [0.0, 12.0], 0.9 most", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_28", "float", " [0.05, 12.0], 0.9 most", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_29", "float", " [1.0, 20.0 (most)]", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_30", "float", " [0.0, 1.5], 1.0 most", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_31", "float", " [0.0, 2.0], 1.0 most", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_32", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_33", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_34", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_35", "float", " [5.0, 100.0], 80.0 most, look like a step of 5.0", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_36", "float", " always the same values as unk_35", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_37", "float", " always the same values as unk_35", offset, sizeof(float), "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);

			//nameOffset
			string str = string((char*)GetOffsetPtr(buf, part->nameOffset + startOffset_PartHeader));
			write_Coloration_Tag("BoneName", "string", "", part->nameOffset + startOffset_PartHeader, str.length() + 1, "BDY_4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);



		}
		else if (memcmp(partHdr->signature, SCD_BDY5_SIGNATURE, 4) == 0) {
			SCD_BDY5* part = (SCD_BDY5*)GetOffsetPtr(buf, offset);

			write_Coloration_Tag("nameOffset", "uint32_t", "", offset, sizeof(uint32_t), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_0", "uint32_t", "0, 4 (most), or 5", offset, sizeof(uint32_t), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_1", "float", " [-0.18, 0.0 (most)]", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_2", "float", " 0 (most) or 0x80000000, could be 0.0 or -0.0", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_3", "float", " always 0", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_4", "float", " 0.0 (most), 1.0", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_5", "float", " 0.0 or 1.0 (most)", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_6", "float", " 0.0 (most) or 1.0", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_7", "float", " always 180.0", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_8", "float", " always -180.0", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_9", "uint32_t", " 1 (most), 2, 3", offset, sizeof(uint32_t), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_10", "float", " [0.0 (most), 0.125]", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_11", "float", " [-0.39, 2.78], 0.0 most", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_12", "float", " [-0.02, 0.37], 0.0 most", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_13", "float", " [-0.64, 0.37], 0.0 most", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_14", "float", " [-135.0, 0.0 (most)]", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_15", "float", " [-11.0, 6.0], 0.0 most", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_16", "float", " [-20.0, 10.0], 0.0 most", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_17", "float", " [0.01, 1.6], 0.04 most", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_18", "float", " [0.0, 0.3], 0.01 most", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_19", "float", " [-0.03, 1.25], 0.02 most", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_20", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_21", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_22", "float", " 0.0 (most), 0.03", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_23", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_24", "uint32_t", " 1 (most), 4", offset, sizeof(uint32_t), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_25", "uint32_t", " 10, 13 or 19 (most)", offset, sizeof(uint32_t), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_26", "float", " [0.2, 20.0], 1.0 most", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_27", "float", " [0.0 (most), 0.5]", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_28", "float", " [0.0 (most) , 1.5]", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_29", "float", " [0.0, 1.0], 0.01 most, 0.0 second", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_30", "float", " [0.0, 15.0], 5.0 most", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_31", "float", " [0.0, 15.0], 3.0 most", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_32", "float", " [0.2, 20.0], 2.0 most", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_33", "float", " [0.0, 1.4], 0.2 most", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_34", "float", " [0.0 (most), 3.0]", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_35", "float", " -0.1 or 0.0 (most)", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_36", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_37", "uint32_t", " always 0", offset, sizeof(uint32_t), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_38", "float", " [1.0, 150.0], 30.0 most. ", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_39", "float", " always same value as unk_38", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_40", "float", " always same value as unk_38", offset, sizeof(float), "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			
			//nameOffset
			string str = string((char*)GetOffsetPtr(buf, part->nameOffset + startOffset_PartHeader));
			write_Coloration_Tag("BoneName", "string", "", part->nameOffset + startOffset_PartHeader, str.length() + 1, "BDY_5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);

		}else if (memcmp(partHdr->signature, SCD_CSR1_SIGNATURE, 4) == 0) {


			SCD_CSR1* part = (SCD_CSR1*)GetOffsetPtr(buf, offset);

			write_Coloration_Tag("nameOffset", "uint32_t", "", offset, sizeof(uint32_t), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("nameOffset_2", "uint32_t", "", offset, sizeof(uint32_t), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_0", "uint32_t", " 3 (most) or 6 ", offset, sizeof(uint32_t), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_1", "float", " [-2.78, -0.02], -0.25", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_2", "float", " [-0.57, 0.0 (most)]", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_3", "float", " -0.011, 0.0 (most), 0.005", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_4", "float", " [-2.633086, 2.767339]", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_5", "float", " [-0.063686, 2.095143], same high level as unk_4", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_6", "float", " [-0.899229, 1.003976], 0.014090 most", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_7", "float", " 1.0 (most), 2.0 or 10.0", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_8", "float", " always 0.0 ", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_9", "float", " [0.0 (most), 1.0]", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_10", "float", " [-1.0, 1.0], 0.0 most", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_11", "float", " [0.0 (most), 1.0]", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_12", "float", " [-1.0, 1.0], 0.0 most, lots of values", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_13", "float", " [-0.999994, 1.0], 0.0 most, lots of values", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_14", "float", " [-1.0, 1.0 (most)], lost of values", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_15", "float", " [-0.124034, 0.819232], 0.0 most", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_16", "float", " [-1.0 (most), 1.0], 0.0 second", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_17", "float", " [-1.0, 1.0], 0.0 most", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_18", "float", " [-0.912480, 0.912481], 0.118516 most, lot of values", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_19", "float", " [-1.0, 1.0], 0.000284 most, lot of values", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_20", "float", " [-0.999626, 1.0], 0.0 most, lot of values", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_21", "float", " [-10.0, 60.0], -5.0 most, look like 5.0 for step, not every time", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_22", "float", " [-75.0, 20.0], 0.0 most, same remark", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_23", "float", " [0.0, 1.0], 0.01 most", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_24", "float", " [0.4, 1.0 (most)]", offset, sizeof(float), "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			//unk_25", "float", "[0.0, 60.0], look like a 5.0 for step"
			//unk_26", "float", "[-60.0, 0.0]"
			//unk_27", "float", "[0.0, 30.0 (most)]"
			//unk_28", "uint32_t", " 0 or 1 (most)"
			//unk_29", "uint32_t", " always 0 "

			//nameOffset
			string str = string((char*)GetOffsetPtr(buf, part->nameOffset + startOffset_PartHeader));
			write_Coloration_Tag("BoneName", "string", "", part->nameOffset + startOffset_PartHeader, str.length() + 1, "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);

			str = string((char*)GetOffsetPtr(buf, part->nameOffset_2 + startOffset_PartHeader));
			write_Coloration_Tag("BoneName_2", "string", "", part->nameOffset_2 + startOffset_PartHeader, str.length() + 1, "CSR_1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);





		}else if (memcmp(partHdr->signature, SCD_CSR2_SIGNATURE, 4) == 0) {



			SCD_CSR2* part = (SCD_CSR2*)GetOffsetPtr(buf, offset);

			write_Coloration_Tag("nameOffset", "uint32_t", "", offset, sizeof(uint32_t), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("nameOffset_2", "uint32_t", "", offset, sizeof(uint32_t), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_0", "uint32_t", "3 (most) or 6", offset, sizeof(uint32_t), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_1", "float", " [-0.31, 0.0], -0.05 most, ", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_2", "float", " [-0.07, 0.07], 0.0 most", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_3", "float", " [-0.02, 0.02], 0.0 most", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_4", "float", " [-0.51, 0.451057]", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_5", "float", " [-0.367560, 0.487466]", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_6", "float", " [-0.245079, 0.351605]", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_7", "float", " [0.0, 10000.0], 1.0 most, look lke for high order", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_8", "float", " always 0", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_9", "float", " [-0.380586, 1.0], 0.0 most", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_10", "float", " [-1.0, 1.0], 0.0 most", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_11", "float", " [-0.192010, 1.0 (most)]", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_12", "float", " [-1.0, 1.0], 0.0 most, lots of values", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_13", "float", " [-1.0, 1.0], 0.0 most, lots of values", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_14", "float", " [-1.0, 1.0], 1.0 most, lots of values", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_15", "float", " [-0.124034, 1.0], 0.0 most", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_16", "float", " [-1.0 (most), 1.0]", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_17", "float", " [-1.0, 1.0], 0.0 most", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_18", "float", " [-1.0, 1.000001], 0.0 most, lot of diff values", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_19", "float", " [-1.0 (most), 1.0], lot ofs diff values", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_20", "float", " [-1.0, 1.0 (second)], 0.0 most, lof of diff values", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_21", "float", " [-10.0, 90.0], 30.0 most, look like 5.0 step (not always)", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_22", "float", " [-60.0, 20.0], 0.0 most, look like 5.0 step (not always)", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_23", "float", " [0.0, 50.0], 0.01 most", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_24", "float", " [0.2, 1.0 (most)]", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_25", "float", " [0.0 (most), 90.0], look like 5.0 step ", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_26", "float", " [-60.0, 0.0 (most)], look like 5.0 step ", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_27", "float", " [0.0 (most), 75.0], look like 5.0 step", offset, sizeof(float), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_28", "uint32_t", " 0 (most) or 1", offset, sizeof(uint32_t), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_29", "uint32_t", " always 0", offset, sizeof(uint32_t), "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			//unk_30", "float", " [0.0 (most), 10000.0], look lke for high order"
			//unk_31", "", " there is 2 case : float : [0.0, 10000.0]" or uint3é : 0, 0xc 0xd or 0x10,  may be wrong type
			//unk_32", "", " [0.0, 100000.0]" may be wrong type
			//unk_33", "", ""  may be wrong type
			//unk_34", "", ""  may be wrong type
			//unk_35", "", ""  may be wrong type
			
			//nameOffset
			string str = string((char*)GetOffsetPtr(buf, part->nameOffset + startOffset_PartHeader));
			write_Coloration_Tag("BoneName", "string", "", part->nameOffset + startOffset_PartHeader, str.length() + 1, "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);

			str = string((char*)GetOffsetPtr(buf, part->nameOffset_2 + startOffset_PartHeader));
			write_Coloration_Tag("BoneName_2", "string", "", part->nameOffset_2 + startOffset_PartHeader, str.length() + 1, "CSR_2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);



		}else if (memcmp(partHdr->signature, SCD_ACT0_SIGNATURE, 4) == 0) {


			SCD_ACT0* part = (SCD_ACT0*)GetOffsetPtr(buf, offset);

			write_Coloration_Tag("nameOffset", "uint32_t", "", offset, sizeof(uint32_t), "ACT_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("nameOffset_2", "uint32_t", "", offset, sizeof(uint32_t), "ACT_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_0", "uint32_t", " always 0", offset, sizeof(uint32_t), "ACT_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_1", "float", " [-0.26, 0.06], 0.0 most", offset, sizeof(float), "ACT_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_2", "float", " [-0.07, 0.18], 0.0 most", offset, sizeof(float), "ACT_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_3", "float", " [-0.25, 0.25], 0.0 most", offset, sizeof(float), "ACT_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_4", "float", " [-0.294314, 0.179241], 0.0 most, lot of diff values", offset, sizeof(float), "ACT_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_5", "float", " [-0.221907, 0.359095], 0.0 most, lot of diff values", offset, sizeof(float), "ACT_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_6", "float", " [-0.593591, 0.593528], 0.0 most, lot of diff values", offset, sizeof(float), "ACT_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_7", "float", " [-1 (most), 1.0], 0.0 in second place", offset, sizeof(float), "ACT_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_8", "float", " [0.5, 1000.0], 1.0 most. values seam to be exponential progression. like a order of high ('ordre de grandeur' in french)", offset, sizeof(float), "ACT_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_9", "float", " [0.0, 3.0], 0.9 most", offset, sizeof(float), "ACT_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("unk_10", "uint32_t", "0 (most) or 3", offset, sizeof(uint32_t), "ACT_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_11", "uint32_t", " always 0", offset, sizeof(uint32_t), "ACT_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			
			//nameOffset
			string str = string((char*)GetOffsetPtr(buf, part->nameOffset + startOffset_PartHeader));
			write_Coloration_Tag("BoneName", "string", "", part->nameOffset + startOffset_PartHeader, str.length() + 1, "ACT_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);

			str = string((char*)GetOffsetPtr(buf, part->nameOffset_2 + startOffset_PartHeader));
			write_Coloration_Tag("BoneName_2", "string", "", part->nameOffset_2 + startOffset_PartHeader, str.length() + 1, "ACT_0", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);

		}else {
			printf("Warning : Unknow signature :%s\n", partHdr->signature);
			LibXenoverse::notifyWarning();
		}

		offset = startOffset_PartHeader + partHdr->size;
	}

}





/*-------------------------------------------------------------------------------\
|                             write_Coloration_Tag			                     |
\-------------------------------------------------------------------------------*/
void Scd::write_Coloration_Tag(string paramName, string paramType, string paramComment, size_t startOffset, size_t size, string sectionName, TiXmlElement* parent, size_t idTag, size_t sectionIndex, size_t paramIndex, std::vector<bool> &listBytesAllreadyTagged, size_t sectionIndexInList, bool checkAllreadyTaggued)
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
			LibXenoverse::notifyError();
			continue;
		}

		if (index == 0x11ac)			//for test
			int aa = 42;

		if ((checkAllreadyTaggued) && (listBytesAllreadyTagged.at(index)))
		{
			printf("warning on tagID %i : the byte %i allready taggued, may be a overflow between blocks. Infos : %s. \n", idTag, index, (sectionName + ((sectionIndexInList != (size_t)-1) ? "[" + std::to_string(sectionIndexInList) + "]" : "") + "." + paramName + " (" + paramType + ") : " + paramComment).c_str());
			LibXenoverse::notifyError();
		}

		listBytesAllreadyTagged.at(index) = true;
	}
}


}