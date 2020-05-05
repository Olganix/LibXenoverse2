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
		
	string type = "";
	rootNode->QueryStringAttribute("type", &type);

	
	bool isVersion_M_first = (type == "");			//version before SPM with no Tags
	bool isVersion_M = (type == "SPM");				//first version, on old games
	bool isVersion_E = (type == "SPE");				//new version
	bool isVersion_X = (type == "SPX");				//version SPE + DSC.
	
	if (isVersion_M_first)
		isVersion_M = true;


	if ((!isVersion_M) && (!isVersion_E) && (!isVersion_X))
		return false;

	

	listBytes.push_back( (!isVersion_M_first) ? ('#' + ('S' << 8) + ('P' << 16) + (type.at(2) << 24)) : 0);	//signature //isVersion_M_first : version before SPM, with no tag and name, just 00 at the same place.
	if (!isVersion_M)
	{
		string str = "";
		rootNode->QueryStringAttribute("version", &str); listBytes.push_back(val32(EMO_BaseFile::GetUnsigned(str, 0)));	
	}else {
		string str = "";
		rootNode->QueryStringAttribute("name", &str);

		size_t nbChar = str.length();
		if (nbChar > 4)
			nbChar = 4;

		listBytes.push_back(val32((((0 < nbChar) ? str.at(0) : '\0') << 0) + (((1 < nbChar) ? str.at(1) : '\0') << 8) + (((2 < nbChar) ? str.at(2) : '\0') << 16) + (((3 < nbChar) ? str.at(3) : '\0') << 24)));
	}

	listBytes.push_back(0);			//number_Section1
	if(!isVersion_M)
		listBytes.push_back(0);			//size_Section1
	listBytes.push_back( (isVersion_M ? sizeof(SPMHeader_M) : (isVersion_E ? sizeof(SPMHeader_E) : (sizeof(SPMHeader_X) + sizeof(uint32_t)) ) ));	// offset_Section1
	if (isVersion_X)
	{
		listBytes.push_back(0);		//number_Section2
		listBytes.push_back(0);		//offset_Section2
		listBytes.push_back(0);		//0 filing
	}



	//SpmSection1
	TiXmlElement* node_ListS1 = rootNode->FirstChildElement("ListSection1");
	char* buf_names = new char[16];
	string str = "";
	float f_tmp;

	std::vector<std::vector<size_t>> listByteSection1;
	for (TiXmlElement* node_tmp = node_ListS1 ? node_ListS1->FirstChildElement("Section1") : 0; node_tmp; node_tmp = node_tmp->NextSiblingElement("Section1"))
	{
		listByteSection1.push_back(std::vector<size_t>());


		size_t startOffsetName = listByteSection1.back().size();
		node_tmp->QueryStringAttribute("name", &str);
		size_t nbChar = str.length();
		for (size_t i = 0; i < 4; i++)
			listByteSection1.back().push_back(val32((((i * 4 < nbChar) ? str.at(i * 4) : '\0') << 0) + (((i * 4 + 1 < nbChar) ? str.at(i * 4 + 1) : '\0') << 8) + (((i * 4 + 2 < nbChar) ? str.at(i * 4 + 2) : '\0') << 16) + (((i * 4 + 3 < nbChar) ? str.at(i * 4 + 3) : '\0') << 24)));


		str = "";
		node_tmp->QueryStringAttribute("name_withpadding", &str);						//for perfect rebuild
		if (str.length())
		{
			std::vector<string> sv = split(str, ',');
			if (sv.size() == 16)
			{
				for (size_t j = 0; j < 4; j++)
				{
					listByteSection1.back().at(startOffsetName + j) = val32((GetUnsigned(sv.at(j * 4 + 0)) << 0) + (GetUnsigned(sv.at(j * 4 + 1)) << 8) + (GetUnsigned(sv.at(j * 4 + 2)) << 16) + (GetUnsigned(sv.at(j * 4 + 3)) << 24));
				}
			}
		}



		//////////////////
		node = node_tmp->FirstChildElement("lenFlareEnabled"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF)); }
		node = node_tmp->FirstChildElement("unk_1"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16; }				//because endian

		node = node_tmp->FirstChildElement("unk_2"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF)); }
		node = node_tmp->FirstChildElement("unk_3"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16; }

		node = node_tmp->FirstChildElement("unk_4"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF)); }
		node = node_tmp->FirstChildElement("unk_5"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16; }

		node = node_tmp->FirstChildElement("godRayAndSunHalo"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF)); }
		node = node_tmp->FirstChildElement("unk_7"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16; }



		//////////////////


		for (size_t j = 0; j < 4; j++)
		{
			node = node_tmp->FirstChildElement("unk_8_a_0_" + std::to_string(j)); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
		}

		node = node_tmp->FirstChildElement("lightDirection");
		if (node)
		{
			node->QueryStringAttribute("x", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			node->QueryStringAttribute("y", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			node->QueryStringAttribute("z", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			node->QueryStringAttribute("w", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
		}

		for (size_t j = 0; j < 19; j++)
		{
			node = node_tmp->FirstChildElement("unk_8_a_1_" + std::to_string(j)); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
		}

		node = node_tmp->FirstChildElement("unk_8_a_1_a"); if (node) { node->QueryStringAttribute("u32", &str); listByteSection1.back().push_back(val32(EMO_BaseFile::GetUnsigned(str, 0))); }

		for (size_t j = 0; j < 23; j++)
		{
			node = node_tmp->FirstChildElement("unk_8_a_1_b_" + std::to_string(j)); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
		}


		node = node_tmp->FirstChildElement("unk_8_a_1_c"); if (node) { node->QueryStringAttribute("u32", &str); listByteSection1.back().push_back(val32(EMO_BaseFile::GetUnsigned(str, 0))); }


		for (size_t j = 0; j < 4; j++)
		{
			node = node_tmp->FirstChildElement("unk_8_a_1_d_" + std::to_string(j)); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
		}

		TiXmlElement* node_fog = node_tmp->FirstChildElement("fog");
		if (node_fog)
		{
			node = node_fog->FirstChildElement("color");
			if (node)
			{
				node->QueryStringAttribute("r", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("g", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("b", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("a", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			}

			node = node_fog->FirstChildElement("distance");
			if (node)
			{
				node->QueryStringAttribute("start", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("end", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			}
		}


		node = node_tmp->FirstChildElement("unk_8_a_2"); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
		node = node_tmp->FirstChildElement("colorSaturation"); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }


		node = node_tmp->FirstChildElement("mutiplyColor");
		if (node)
		{
			node->QueryStringAttribute("r", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			node->QueryStringAttribute("g", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			node->QueryStringAttribute("b", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
		}

		node = node_tmp->FirstChildElement("filterColor");
		if (node)
		{
			node->QueryStringAttribute("r", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			node->QueryStringAttribute("g", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			node->QueryStringAttribute("b", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
		}

		node = node_tmp->FirstChildElement("additiveColor");
		if (node)
		{
			node->QueryStringAttribute("r", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			node->QueryStringAttribute("g", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			node->QueryStringAttribute("b", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
		}

		node = node_tmp->FirstChildElement("filterHotCold"); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }


		for (size_t j = 0; j < 5; j++)
		{
			node = node_tmp->FirstChildElement("unk_8_a_3_" + std::to_string(j)); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
		}

		node = node_tmp->FirstChildElement("mutiplyInverseFactor"); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }

		node = node_tmp->FirstChildElement("blurStartDistance"); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }

		node = node_tmp->FirstChildElement("unk_8_a_4"); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));}



		//////////////////

		node = node_tmp->FirstChildElement("haloAlphaRejection"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF)); }
		node = node_tmp->FirstChildElement("unk_8_c"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16; }



		//////////////////


		node = node_tmp->FirstChildElement("unk_8_d_0"); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }

		node = node_tmp->FirstChildElement("shadow");
		if (node)
		{
			node->QueryStringAttribute("endAngle", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
		}

		node = node_tmp->FirstChildElement("unk_8_d_1"); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }

		node = node_tmp->FirstChildElement("shadow");
		if (node)
		{
			node->QueryStringAttribute("startAngle", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
		}


		TiXmlElement* node_sun = node_tmp->FirstChildElement("sun");
		if (node_sun)
		{
			node = node_sun->FirstChildElement("sunFactor");
			if (node)
			{
				node->QueryStringAttribute("normal", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("hiden", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			}
			node = node_sun->FirstChildElement("size");
			if (node)
			{
				node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			}
		}

		node = node_tmp->FirstChildElement("unk_8_d_2"); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
		node = node_tmp->FirstChildElement("unk_8_d_3"); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }


		node = node_tmp->FirstChildElement("highIlluminated_glareInverseFactor"); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }


		node = node_tmp->FirstChildElement("background_glare_additiveColor");
		if (node)
		{
			node->QueryStringAttribute("r", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			node->QueryStringAttribute("g", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			node->QueryStringAttribute("b", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			node->QueryStringAttribute("a", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
		}

		for (size_t j = 0; j < 6; j++)
		{
			node = node_tmp->FirstChildElement("unk_8_d_4_" + std::to_string(j)); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
		}


		node = node_tmp->FirstChildElement("character_glare_additiveColor");
		if (node)
		{
			node->QueryStringAttribute("r", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			node->QueryStringAttribute("g", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			node->QueryStringAttribute("b", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
		}



		//////////////////

		node = node_tmp->FirstChildElement("unk_8_e"); if (node) { node->QueryStringAttribute("u32", &str); listByteSection1.back().push_back(val32(EMO_BaseFile::GetUnsigned(str, 0))); }



		//////////////////

		TiXmlElement* node_edgeStrokes = node_tmp->FirstChildElement("edgeStrokes");
		if (node_edgeStrokes)
		{
			node = node_edgeStrokes->FirstChildElement("strokes");
			if (node)
			{
				node->QueryStringAttribute("factor", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("width", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			}

			node = node_edgeStrokes->FirstChildElement("color");
			if (node)
			{
				node->QueryStringAttribute("r", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("g", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("b", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			}
		}

		for (size_t j = 0; j < 4; j++)
		{
			node = node_tmp->FirstChildElement("unk_8_f__" + std::to_string(j)); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
		}


		node = node_tmp->FirstChildElement("unk_8_f_1"); if (node) { node->QueryStringAttribute("u32", &str); listByteSection1.back().push_back(val32(EMO_BaseFile::GetUnsigned(str, 0))); }
		for (size_t j = 0; j < 3; j++)
		{
			node = node_tmp->FirstChildElement("unk_8_f_2_" + std::to_string(j)); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
		}

		node = node_tmp->FirstChildElement("unk_8_f_3"); if (node) { node->QueryStringAttribute("u32", &str); listByteSection1.back().push_back(val32(EMO_BaseFile::GetUnsigned(str, 0))); }
		for (size_t j = 0; j < 3; j++)
		{
			node = node_tmp->FirstChildElement("unk_8_f_4_" + std::to_string(j)); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
		}

		node = node_tmp->FirstChildElement("unk_8_f_5"); if (node) { node->QueryStringAttribute("u32", &str); listByteSection1.back().push_back(val32(EMO_BaseFile::GetUnsigned(str, 0))); }
		for (size_t j = 0; j < 3; j++)
		{
			node = node_tmp->FirstChildElement("unk_8_f_6_" + std::to_string(j)); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
		}



		//////////////////

		node = node_tmp->FirstChildElement("unk_9"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF)); }
		node = node_tmp->FirstChildElement("unk_10"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16; }				//because endian will be applyed after.

		node = node_tmp->FirstChildElement("unk_11"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF)); }
		node = node_tmp->FirstChildElement("unk_12"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16; }

		node = node_tmp->FirstChildElement("unk_13"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF)); }
		node = node_tmp->FirstChildElement("unk_14"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16; }

		node = node_tmp->FirstChildElement("unk_15"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF)); }
		node = node_tmp->FirstChildElement("unk_16"); if (node) { node->QueryStringAttribute("u16", &str); listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16; }





		//////////////////


		for (size_t j = 0; j < 4; j++)
		{
			node = node_tmp->FirstChildElement("unk_17_0_" + std::to_string(j)); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
		}



		node = node_tmp->FirstChildElement("backgroundSpecifics_multiplycolor");
		if (node)
		{
			node->QueryStringAttribute("r", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			node->QueryStringAttribute("g", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			node->QueryStringAttribute("b", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			node->QueryStringAttribute("a", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
		}


		for (size_t j = 0; j < 4; j++)
		{
			node = node_tmp->FirstChildElement("unk_17_1_" + std::to_string(j)); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
		}


		TiXmlElement* node_shadows = node_tmp->FirstChildElement("shadows");
		if (node_shadows)
		{
			node = node_shadows->FirstChildElement("direction");
			if (node)
			{
				node->QueryStringAttribute("x", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("y", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("z", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			}

			for (size_t j = 0; j < 13; j++)
			{
				node = node_shadows->FirstChildElement("unk_17_2_" + std::to_string(j)); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
			}

			node = node_shadows->FirstChildElement("filterColor");
			if (node)
			{
				node->QueryStringAttribute("r", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("g", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("b", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			}
		}


		node = node_tmp->FirstChildElement("unk_17_3"); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }




		TiXmlElement* node_sunGodRay = node_tmp->FirstChildElement("sunGodRay");
		if (node_sunGodRay)
		{
			node = node_sunGodRay->FirstChildElement("color");
			if (node)
			{
				node->QueryStringAttribute("r", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("g", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("b", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			}

			node = node_sunGodRay->FirstChildElement("intensity"); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
		}



		



		TiXmlElement* node_limitShadows = node_tmp->FirstChildElement("limitShadow");
		if (node_limitShadows)
		{
			node = node_limitShadows->FirstChildElement("highLimit");
			if (node)
			{
				node->QueryStringAttribute("start", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("end", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			}

			node = node_limitShadows->FirstChildElement("radiusLimit");
			if (node)
			{
				node->QueryStringAttribute("start", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("end", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			}
		}


		for (size_t j = 0; j < 8; j++)
		{
			node = node_tmp->FirstChildElement("unk_17_4_" + std::to_string(j)); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
		}

		if (node_sunGodRay)
		{
			node = node_sunGodRay->FirstChildElement("polardistance"); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));}
			node = node_sunGodRay->FirstChildElement("unk"); if (node) { node->QueryStringAttribute("u32", &str); listByteSection1.back().push_back(val32(EMO_BaseFile::GetUnsigned(str, 0))); }


			node = node_sunGodRay->FirstChildElement("fading");
			if (node)
			{
				node->QueryStringAttribute("distance", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("attenuationFactor", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("subdivision", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			}

			node = node_sunGodRay->FirstChildElement("repetition");
			if (node)
			{
				node->QueryStringAttribute("angle", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				node->QueryStringAttribute("subdivision", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
			}	
		}

		node = node_tmp->FirstChildElement("unk_17_5"); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }


		/////////////////////////////////////

		TiXmlElement* node_S1e = node_tmp->FirstChildElement("Section1_Ext");

		if (node_S1e)
		{
			node = node_S1e->FirstChildElement("unk_18"); if (node) node->QueryStringAttribute("u16", &str);
			listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF));
			node = node_S1e->FirstChildElement("unk_19"); if (node) node->QueryStringAttribute("u16", &str);
			listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16;				//because endian will be applyed after.

			node = node_S1e->FirstChildElement("unk_20"); if (node) node->QueryStringAttribute("u16", &str);
			listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF));
			node = node_S1e->FirstChildElement("unk_21"); if (node) node->QueryStringAttribute("u16", &str);
			listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16;

			node = node_S1e->FirstChildElement("unk_22"); if (node) node->QueryStringAttribute("u16", &str);
			listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF));
			node = node_S1e->FirstChildElement("unk_23"); if (node) node->QueryStringAttribute("u16", &str);
			listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16;

			node = node_S1e->FirstChildElement("unk_24"); if (node) node->QueryStringAttribute("u16", &str);
			listByteSection1.back().push_back(val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF));
			node = node_S1e->FirstChildElement("unk_25"); if (node) node->QueryStringAttribute("u16", &str);
			listByteSection1.back().back() += val16(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFF) << 16;



			//////////////////

			TiXmlElement* node_characterBorderStrokes = node_S1e->FirstChildElement("characterBorderStroke");
			if (node_characterBorderStrokes)
			{
				node_characterBorderStrokes->QueryStringAttribute("enable", &str); listByteSection1.back().push_back(val32(EMO_BaseFile::GetUnsigned(str, 0) & 0xFFFFFFFF));

				node = node_characterBorderStrokes->FirstChildElement("width");
				if (node)
				{
					node->QueryStringAttribute("far", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
					node->QueryStringAttribute("near", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				}

				node = node_characterBorderStrokes->FirstChildElement("distance");
				if (node)
				{
					node->QueryStringAttribute("start", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
					node->QueryStringAttribute("end", &str); f_tmp = StringToFloat(str);  listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp))));
				}

				node = node_characterBorderStrokes->FirstChildElement("color");
				if (node)
				{
					float r = 0; float g = 0; float b = 0; float a = 1;
					node->QueryStringAttribute("r", &str); r = StringToFloat(str);
					node->QueryStringAttribute("g", &str); g = StringToFloat(str);
					node->QueryStringAttribute("b", &str); b = StringToFloat(str);
					node->QueryStringAttribute("a", &str); a = StringToFloat(str);
					uint32_t uint32_tmp = (((size_t)(a * 255.0)) << 24) + (((size_t)(b * 255.0)) << 16) + (((size_t)(g * 255.0)) << 8) + ((size_t)(r * 255.0));
					listByteSection1.back().push_back( uint32_tmp);
				}
			}


			//////////////////

			node = node_S1e->FirstChildElement("unk_26_0"); if (node) { node->QueryStringAttribute("u32", &str); listByteSection1.back().push_back(val32(EMO_BaseFile::GetUnsigned(str, 0))); }
			for (size_t j = 0; j < 5; j++)
			{
				node = node_S1e->FirstChildElement("unk_26_1_" + std::to_string(j)); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
			}

			node = node_S1e->FirstChildElement("unk_26_2"); if (node) { node->QueryStringAttribute("u32", &str); listByteSection1.back().push_back(val32(EMO_BaseFile::GetUnsigned(str, 0))); }
			for (size_t j = 0; j < 2; j++)
			{
				node = node_S1e->FirstChildElement("unk_26_3_" + std::to_string(j)); if (node) { node->QueryStringAttribute("float", &str); f_tmp = StringToFloat(str); listByteSection1.back().push_back(val32(*((size_t*)(&f_tmp)))); }
			}

			node = node_S1e->FirstChildElement("unk_26_4"); if (node) { node->QueryStringAttribute("u32", &str); listByteSection1.back().push_back(val32(EMO_BaseFile::GetUnsigned(str, 0))); }
			




			/////////////////////////////////////

			TiXmlElement* node_SPMSection1_UnknowEnd = node_S1e->FirstChildElement("UnknowEnd");
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
	if (!isVersion_M)
		listBytes.at(3) = val32(sizeSection1 * 4);
	if (isVersion_X)
		listBytes.at(6) = val32(listBytes.at(4) + (nbSection1 * sizeSection1) * 4);		//offset_Section2 = offset_Section1 + nbSection1 * sizeSection1

	//and finally copy Section1 bytes, + filling '\0' for smaller Section1
	size_t nbBytes = 0;
	for (size_t i = 0; i<nbSection1; i++)
	{
		nbBytes = listByteSection1.at(i).size();
		for (size_t j = 0; j < sizeSection1; j++)
		{
			//listBytes.push_back((j < nbBytes) ? listByteSection1.at(i).at(j) : 0);
			if (j < nbBytes)
			{
				listBytes.push_back( listByteSection1.at(i).at(j) );
			}else{
				listBytes.push_back( 0 );
			}
		}
	}





	//section2
	if (isVersion_X)
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
		return false;
	}

	size_t wd = fwrite(buf, 1, filesize, f);
	::fclose(f);

	
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

	originefilename = filename;

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
	SPMHeader_X* hdr = (SPMHeader_X*)buf;
	if (size < sizeof(SPMHeader_E))
		return;

	bool isVersion_M = (memcmp(hdr->signature, SPM_SIGNATURE_M, 4) == 0);				//first version, on old games
	bool isVersion_E = (memcmp(hdr->signature, SPM_SIGNATURE_E, 4) == 0);				//new version
	bool isVersion_X = (memcmp(hdr->signature, SPM_SIGNATURE_X, 4) == 0);				//version SPE + DSC.

	bool isVersion_M_first = false;
	if ((hdr->signature[0] == 0) && (hdr->signature[1] == 0) && (hdr->signature[2] == 0) && (hdr->signature[3] == 0))	//version before SPM, with no tag and name, just 00 at the same place.
	{
		isVersion_M = true;
		isVersion_M_first = true;
	}
	

	if ((!isVersion_M) && (!isVersion_E) && (!isVersion_X))
		return;

	string type = isVersion_M ? ( !isVersion_M_first ? "SPM" : "") : (isVersion_E ? "SPE" : "SPX");
	parent->SetAttribute("type", type);

	
	size_t size_Section1 = hdr->size_Section1;
	size_t offset_Section1 = hdr->offset_Section1;
	size_t number_Section2 = hdr->number_Section2;
	size_t offset_Section2 = hdr->offset_Section2;

	if (!isVersion_X)
	{
		number_Section2 = 0;
		offset_Section2 = 0;
	}
	if (isVersion_M)
	{
		offset_Section1 = size_Section1;
		size_Section1 = 0x310;					//Todo confirm with color tags;
		string name = nameFromFilename(originefilename);
		if ((name == "ARD.spm") || (name == "IOR.spm") || (name == "JMN.spm") ||
			(name == "MUU.spm") || (name == "SHK.spm") || (name == "SRU.spm"))
			size_Section1 = 0x2B0;				//688
		else if ((name == "DTH.spm") || (name == "MST.spm"))
			size_Section1 = 0x290;				//656

		uint32_t name_u32 = val32(hdr->version);
		string str = "";
		uint8_t uint8_tmp;

		uint8_tmp = (uint8_t)((name_u32 >> 24) & 0xFF); if (uint8_tmp != 0) str += (char)uint8_tmp;
		uint8_tmp = (uint8_t)((name_u32 >> 16) & 0xFF); if (uint8_tmp != 0) str += (char)uint8_tmp;
		uint8_tmp = (uint8_t)((name_u32 >>  8) & 0xFF); if (uint8_tmp != 0) str += (char)uint8_tmp;
		uint8_tmp = (uint8_t)((name_u32 >>  0) & 0xFF); if (uint8_tmp != 0) str += (char)uint8_tmp;
		parent->SetAttribute("name", str);
	}else {
		parent->SetAttribute("version", hdr->version);
	}

	

	parent->SetAttribute("test", ToString(hdr->number_Section1) +"_"+ UnsignedToString(size_Section1, true));			//todo remove

	TiXmlElement* node_SPMHeader = new TiXmlElement("Header");				//todo remove.
	node_SPMHeader->LinkEndChild(new TiXmlComment("Just for debug , ignore header"));
	TiXmlElement* node;
	node = new TiXmlElement("number_Section1"); node->SetAttribute("value", UnsignedToString(hdr->number_Section1, false)); node_SPMHeader->LinkEndChild(node);
	node = new TiXmlElement("size_Section1"); node->SetAttribute("size", UnsignedToString(size_Section1, true)); node_SPMHeader->LinkEndChild(node);
	node = new TiXmlElement("offset_Section1"); node->SetAttribute("startOffset", UnsignedToString(offset_Section1, true)); node_SPMHeader->LinkEndChild(node);
	if (!isVersion_E)
	{
		node = new TiXmlElement("number_Section2"); node->SetAttribute("value", UnsignedToString(number_Section2, false)); node_SPMHeader->LinkEndChild(node);
		node = new TiXmlElement("offset_Section2"); node->SetAttribute("startOffset", UnsignedToString(offset_Section2, true)); node_SPMHeader->LinkEndChild(node);
	}
	parent->LinkEndChild(node_SPMHeader);
	


	

	size_t offset = 0;
	size_t offset_tmp = 0;

	if (offset_Section1)
	{
		TiXmlElement* node_ListS1 = new TiXmlElement("ListSection1");

		for (size_t i = 0; i < hdr->number_Section1; i++)
		{
			size_t startOffsetSection1 = offset_Section1 + i * size_Section1;
			offset = startOffsetSection1;

			SPM_Section1* section1 = (SPM_Section1*)GetOffsetPtr_native(buf, startOffsetSection1);

			TiXmlElement* node_SPMSection1 = new TiXmlElement("Section1");
			node_ListS1->LinkEndChild(node_SPMSection1);

			node_SPMSection1->SetAttribute("size", EMO_BaseFile::UnsignedToString(size_Section1, true));
			
			string str = section1->name;
			if (str.length() >= 16)
				str = str.substr(0, 16);
			node_SPMSection1->SetAttribute("name", str);
			
			str = "";
			for (size_t j = 0; j < 16; j++)
				str += ((j!=0) ? "," : "") + UnsignedToString(section1->name[j], true);
			node_SPMSection1->SetAttribute("name_withpadding", str);						//for prefect rebuild



			{
				node = new TiXmlElement("lenFlareEnabled"); node->SetAttribute("u16", UnsignedToString(section1->lenFlareEnabled, true)); node_SPMSection1->LinkEndChild(new TiXmlComment("lenFlareEnabled : it's not totally that, or may be it's with a combinaison of parameters, here do nothing here")); node_SPMSection1->LinkEndChild(node);
				node = new TiXmlElement("unk_1"); node->SetAttribute("u16", UnsignedToString(section1->unk_1, true)); node_SPMSection1->LinkEndChild(node);
				node = new TiXmlElement("unk_2"); node->SetAttribute("u16", UnsignedToString(section1->unk_2, true)); node_SPMSection1->LinkEndChild(node);
				node = new TiXmlElement("unk_3"); node->SetAttribute("u16", UnsignedToString(section1->unk_3, true)); node_SPMSection1->LinkEndChild(node);
				node = new TiXmlElement("unk_4"); node->SetAttribute("u16", UnsignedToString(section1->unk_4, true)); node_SPMSection1->LinkEndChild(node);
				node = new TiXmlElement("unk_5"); node->SetAttribute("u16", UnsignedToString(section1->unk_5, true)); node_SPMSection1->LinkEndChild(node);
				node = new TiXmlElement("godRayAndSunHalo"); node->SetAttribute("u16", UnsignedToString(section1->godRayAndSunHalo, true)); node_SPMSection1->LinkEndChild(node);
				node = new TiXmlElement("unk_7"); node->SetAttribute("u16", UnsignedToString(section1->unk_7, true)); node_SPMSection1->LinkEndChild(node);
			}
			

			for (size_t j = 0; j < 4; j++)
			{
				node = new TiXmlElement("unk_8_a_0_" + std::to_string(j)); node->SetAttribute("float", FloatToString(section1->unk_8_a_0[j])); node_SPMSection1->LinkEndChild(node);
			}

			node = new TiXmlElement("lightDirection"); node_SPMSection1->LinkEndChild(new TiXmlComment("lightDirection : not concern the visual of the sun. lightDirection_w is may be just padding")); node_SPMSection1->LinkEndChild(node);
			node->SetAttribute("x", FloatToString(section1->lightDirection_x));
			node->SetAttribute("y", FloatToString(section1->lightDirection_y));
			node->SetAttribute("z", FloatToString(section1->lightDirection_z));
			node->SetAttribute("w", FloatToString(section1->lightDirection_w));
			

			for (size_t j = 0; j < 19; j++)
			{
				node = new TiXmlElement("unk_8_a_1_"+ std::to_string(j)); node->SetAttribute("float", FloatToString(section1->unk_8_a_1[j])); node_SPMSection1->LinkEndChild(node);
			}

			node = new TiXmlElement("unk_8_a_1_a"); node->SetAttribute("u32", UnsignedToString(section1->unk_8_a_1_a, true));  node_SPMSection1->LinkEndChild(node);


			for (size_t j = 0; j < 23; j++)
			{
				node = new TiXmlElement("unk_8_a_1_b_" + std::to_string(j)); node->SetAttribute("float", FloatToString(section1->unk_8_a_1_b[j])); node_SPMSection1->LinkEndChild(node);
			}

			node = new TiXmlElement("unk_8_a_1_c"); node->SetAttribute("u32", UnsignedToString(section1->unk_8_a_1_c, true)); node_SPMSection1->LinkEndChild(node);


			for (size_t j = 0; j < 4; j++)
			{
				node = new TiXmlElement("unk_8_a_1_d_" + std::to_string(j)); node->SetAttribute("float", FloatToString(section1->unk_8_a_1_d[j])); node_SPMSection1->LinkEndChild(node);
			}



			{
				TiXmlElement* node_fog = new TiXmlElement("fog"); node_SPMSection1->LinkEndChild(node_fog);

				node = new TiXmlElement("color"); node_fog->LinkEndChild(node);
				node->SetAttribute("r", FloatToString(section1->fogColor_r));
				node->SetAttribute("g", FloatToString(section1->fogColor_g));
				node->SetAttribute("b", FloatToString(section1->fogColor_b));
				node->SetAttribute("a", FloatToString(section1->fogColor_a));

				node = new TiXmlElement("distance"); node_fog->LinkEndChild(node);
				node->SetAttribute("start", FloatToString(section1->fog_startDist));
				node->SetAttribute("end", FloatToString(section1->fog_endDist));
			}


			node = new TiXmlElement("unk_8_a_2"); node->SetAttribute("float", FloatToString(section1->unk_8_a_2)); node_SPMSection1->LinkEndChild(node);
			
			node = new TiXmlElement("colorSaturation"); node->SetAttribute("float", FloatToString(section1->colorSaturation)); node_SPMSection1->LinkEndChild(node);


			node = new TiXmlElement("mutiplyColor"); node_SPMSection1->LinkEndChild(node);
			node->SetAttribute("r", FloatToString(section1->mutiplyColor_r));
			node->SetAttribute("g", FloatToString(section1->mutiplyColor_g));
			node->SetAttribute("b", FloatToString(section1->mutiplyColor_b));

			node = new TiXmlElement("filterColor"); node_SPMSection1->LinkEndChild(new TiXmlComment("FilterColor ? seam to mutilpy some inverse. strange. , if 0, only have all Grey. -1 is fun and afraying. Notice: if 67=1 0 0, if 64 to 66 are all 1, or 64 is 1, it's the same result"));  node_SPMSection1->LinkEndChild(node);
			node->SetAttribute("r", FloatToString(section1->filterColor_r));
			node->SetAttribute("g", FloatToString(section1->filterColor_g));
			node->SetAttribute("b", FloatToString(section1->filterColor_b));

			node = new TiXmlElement("additiveColor"); node_SPMSection1->LinkEndChild(new TiXmlComment("ColorAdditive. sort ambient ? "));  node_SPMSection1->LinkEndChild(node);
			node->SetAttribute("r", FloatToString(section1->additiveColor_r));
			node->SetAttribute("g", FloatToString(section1->additiveColor_g));
			node->SetAttribute("b", FloatToString(section1->additiveColor_b));

			node = new TiXmlElement("filterHotCold"); node->SetAttribute("float", FloatToString(section1->filterHotCold)); node_SPMSection1->LinkEndChild(new TiXmlComment("CameraDistance witch apply a 'Hot - Cold' filter ? 6200.0 : normal, less: hot coloration (red yellow orange), more: cold color (blue)"));   node_SPMSection1->LinkEndChild(node);


			for (size_t j = 0; j < 5; j++)
			{
				node = new TiXmlElement("unk_8_a_3_" + std::to_string(j)); node->SetAttribute("float", FloatToString(section1->unk_8_a_3[j])); node_SPMSection1->LinkEndChild(node);
			}


			node = new TiXmlElement("mutiplyInverseFactor"); node->SetAttribute("float", FloatToString(section1->mutiplyInverseFactor)); node_SPMSection1->LinkEndChild(new TiXmlComment("InverseFactorOnFinalColor. if 1.0 all black ")); node_SPMSection1->LinkEndChild(node);

			node = new TiXmlElement("blurStartDistance"); node->SetAttribute("float", FloatToString(section1->blur_startDist)); node_SPMSection1->LinkEndChild(new TiXmlComment("0 it's really blurred. 45  it's normal")); node_SPMSection1->LinkEndChild(node);


			node = new TiXmlElement("unk_8_a_4"); node->SetAttribute("float", FloatToString(section1->unk_8_a_4)); node_SPMSection1->LinkEndChild(node);












			node = new TiXmlElement("haloAlphaRejection"); node->SetAttribute("u16", UnsignedToString(section1->halo_alphaRejection, true)); node_SPMSection1->LinkEndChild(new TiXmlComment("sort of Alpha rejection for the halo : 0 the halo is back to the Zeplin, 0xffff : halo is always behind "));  node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_8_c"); node->SetAttribute("u16", UnsignedToString(section1->unk_8_c, true)); node_SPMSection1->LinkEndChild(node);




			node = new TiXmlElement("unk_8_d_0"); node->SetAttribute("float", FloatToString(section1->unk_8_d_0)); node_SPMSection1->LinkEndChild(node);

			node = new TiXmlElement("shadow"); node_SPMSection1->LinkEndChild(new TiXmlComment("for start : if 280, I see no shadow. For end : if I put near of 0 , Shadow disapeard, depending of the angle from camera. but it's not a angle ... strange  "));  node_SPMSection1->LinkEndChild(node);
			node->SetAttribute("startAngle", FloatToString(section1->shadow_startAngle));
			node->SetAttribute("endAngle", FloatToString(section1->shadow_endAngle));

			node = new TiXmlElement("unk_8_d_1"); node->SetAttribute("float", FloatToString(section1->unk_8_d_1)); node_SPMSection1->LinkEndChild(node);


			TiXmlElement* node_sun = new TiXmlElement("sun"); node_SPMSection1->LinkEndChild(node_sun);
			node = new TiXmlElement("sunFactor"); node_sun->LinkEndChild(new TiXmlComment("change halo, godrays, and glares. for 'hiden' : inverse factor For the Sun_bis, witch disapread (by scaling) when something is behind a little (not the static sun disc). for 'size' : size of the sun (not the static one), and also change the reflection intensity (like on certain cell of the tournament ground) "));  node_sun->LinkEndChild(node);
			node->SetAttribute("normal", FloatToString(section1->sunFactor));
			node->SetAttribute("hiden", FloatToString(section1->sunFactor_hiden));
			node = new TiXmlElement("size"); node->SetAttribute("float", FloatToString(section1->sunSize)); node_sun->LinkEndChild(node);

			node = new TiXmlElement("unk_8_d_2"); node->SetAttribute("float", FloatToString(section1->unk_8_d_2)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_8_d_3"); node->SetAttribute("float", FloatToString(section1->unk_8_d_3)); node_SPMSection1->LinkEndChild(node);


			node = new TiXmlElement("highIlluminated_glareInverseFactor"); node->SetAttribute("float", FloatToString(section1->highIlluminated_glareInverseFactor)); node_SPMSection1->LinkEndChild(new TiXmlComment("BlurFactorOnLightIntensity, 0 : glare (and DynSun) are more presents, 0.17300001: normal,  6: no glare on that")); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("background_glare_additiveColor"); node_SPMSection1->LinkEndChild(new TiXmlComment("sort of LightColor for glare for illuminated part as Specular/reflection (not on shadows)")); node_SPMSection1->LinkEndChild(node);
			node->SetAttribute("r", FloatToString(section1->background_glare_additiveColor_r));
			node->SetAttribute("g", FloatToString(section1->background_glare_additiveColor_g));
			node->SetAttribute("b", FloatToString(section1->background_glare_additiveColor_b));
			node->SetAttribute("a", FloatToString(section1->background_glare_additiveColor_a));

			for (size_t j = 0; j < 6; j++)
			{
				node = new TiXmlElement("unk_8_d_4_" + std::to_string(j)); node->SetAttribute("float", FloatToString(section1->unk_8_d_4[j])); node_SPMSection1->LinkEndChild(node);
			}

			node = new TiXmlElement("character_glare_additiveColor"); node_SPMSection1->LinkEndChild(new TiXmlComment("ColorIlluminatedGlareOfCharacters")); node_SPMSection1->LinkEndChild(node);
			node->SetAttribute("r", FloatToString(section1->character_glare_additiveColor_r));
			node->SetAttribute("g", FloatToString(section1->character_glare_additiveColor_g));
			node->SetAttribute("b", FloatToString(section1->character_glare_additiveColor_b));





			node = new TiXmlElement("unk_8_e"); node->SetAttribute("u32", UnsignedToString(section1->unk_8_e, true)); node_SPMSection1->LinkEndChild(new TiXmlComment("DONT Touch !!!! offset for something never used, so crash the game if different thatn 0 (may be leaked to map file , see case AFnmc with underwater)")); node_SPMSection1->LinkEndChild(node);


			{
				TiXmlElement* node_edgeStrokes = new TiXmlElement("edgeStrokes"); node_edgeStrokes->LinkEndChild(new TiXmlComment("EdgeStrokeFactor with 1, you have stroke on limits of shadows, or edges. EdgeStrokeWitdh, may be in pixels, 40 look pretty nice ")); node_SPMSection1->LinkEndChild(node_edgeStrokes);

				node = new TiXmlElement("strokes"); node_edgeStrokes->LinkEndChild(node);
				node->SetAttribute("factor", FloatToString(section1->edgeStrokes_factor));
				node->SetAttribute("width", FloatToString(section1->edgeStrokes_width));

				node = new TiXmlElement("color"); node_edgeStrokes->LinkEndChild(node);
				node->SetAttribute("r", FloatToString(section1->edgeStrokes_color_r));
				node->SetAttribute("g", FloatToString(section1->edgeStrokes_color_g));
				node->SetAttribute("b", FloatToString(section1->edgeStrokes_color_b));
			}


			for (size_t j = 0; j < 4; j++)
			{
				node = new TiXmlElement("unk_8_f__" + std::to_string(j)); node->SetAttribute("float", FloatToString(section1->unk_8_f[j])); node_SPMSection1->LinkEndChild(node);
			}

			node = new TiXmlElement("unk_8_f_1"); node->SetAttribute("u32", UnsignedToString(section1->unk_8_f_1, true)); node_SPMSection1->LinkEndChild(node);

			for (size_t j = 0; j < 3; j++)
			{
				node = new TiXmlElement("unk_8_f_2_" + std::to_string(j)); node->SetAttribute("float", FloatToString(section1->unk_8_f_2[j])); node_SPMSection1->LinkEndChild(node);
			}


			node = new TiXmlElement("unk_8_f_3"); node->SetAttribute("u32", UnsignedToString(section1->unk_8_f_3, true)); node_SPMSection1->LinkEndChild(node);

			for (size_t j = 0; j < 3; j++)
			{
				node = new TiXmlElement("unk_8_f_4_" + std::to_string(j)); node->SetAttribute("float", FloatToString(section1->unk_8_f_4[j])); node_SPMSection1->LinkEndChild(node);
			}

			node = new TiXmlElement("unk_8_f_5"); node->SetAttribute("u32", UnsignedToString(section1->unk_8_f_5, true)); node_SPMSection1->LinkEndChild(node);

			for (size_t j = 0; j < 3; j++)
			{
				node = new TiXmlElement("unk_8_f_6_" + std::to_string(j)); node->SetAttribute("float", FloatToString(section1->unk_8_f_6[j])); node_SPMSection1->LinkEndChild(node);
			}



			node = new TiXmlElement("unk_9"); node->SetAttribute("u16", UnsignedToString(section1->unk_9, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_10"); node->SetAttribute("u16", UnsignedToString(section1->unk_10, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_11"); node->SetAttribute("u16", UnsignedToString(section1->unk_11, true)); node_SPMSection1->LinkEndChild(new TiXmlComment("flags : (!= 0 : sort of Fog, blur when too near the camera) . may be connected to Dsc twice. also todo Test unknow parameters of this section with this on"));  node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_12"); node->SetAttribute("u16", UnsignedToString(section1->unk_12, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_13"); node->SetAttribute("u16", UnsignedToString(section1->unk_13, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_14"); node->SetAttribute("u16", UnsignedToString(section1->unk_14, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_15"); node->SetAttribute("u16", UnsignedToString(section1->unk_15, true)); node_SPMSection1->LinkEndChild(node);
			node = new TiXmlElement("unk_16"); node->SetAttribute("u16", UnsignedToString(section1->unk_16, true)); node_SPMSection1->LinkEndChild(node);



			for (size_t j = 0; j < 4; j++)
			{
				node = new TiXmlElement("unk_17_0_" + std::to_string(j)); node->SetAttribute("float", FloatToString(section1->unk_17_0[j])); node_SPMSection1->LinkEndChild(node);
				if ((isVersion_M) && (0x230 + (j+1) * sizeof(float) >= size_Section1))
					break;
			}

			node = new TiXmlElement("backgroundSpecifics_multiplycolor"); node_SPMSection1->LinkEndChild(new TiXmlComment("ColorMultiply BUT only certain object are concerned , as small part of the entry, or the zeplin")); node_SPMSection1->LinkEndChild(node);
			node->SetAttribute("r", FloatToString(section1->backgroundSpecifics_multiplycolor_r));
			node->SetAttribute("g", FloatToString(section1->backgroundSpecifics_multiplycolor_g));
			node->SetAttribute("b", FloatToString(section1->backgroundSpecifics_multiplycolor_b));
			node->SetAttribute("a", FloatToString(section1->backgroundSpecifics_multiplycolor_a));


			for (size_t j = 0; j < 4; j++)
			{
				node = new TiXmlElement("unk_17_1_" + std::to_string(j)); node->SetAttribute("float", FloatToString(section1->unk_17_1[j])); node_SPMSection1->LinkEndChild(node);
				if ((isVersion_M) && (0x250 + (j + 1) * sizeof(float) >= size_Section1))
					break;
			}

			TiXmlElement* node_shadow = new TiXmlElement("shadows"); node_shadow->LinkEndChild(new TiXmlComment("for dynamic shadows. for 'filterColor' : ShadowsColorFilter shadow are Colorfilter so with white, we see nothing")); node_SPMSection1->LinkEndChild(node_shadow);
			node = new TiXmlElement("direction"); node_shadow->LinkEndChild(node);
			node->SetAttribute("x", FloatToString(section1->shadowDirection_x));
			node->SetAttribute("y", FloatToString(section1->shadowDirection_y));
			node->SetAttribute("z", FloatToString(section1->shadowDirection_z));

			for (size_t j = 0; j < 13; j++)
			{
				node = new TiXmlElement("unk_17_2_" + std::to_string(j)); node->SetAttribute("float", FloatToString(section1->unk_17_2[j])); node_shadow->LinkEndChild(node);
				if ((isVersion_M) && (0x26C + (j + 1) * sizeof(float) >= size_Section1))
					break;
			}
			if ((isVersion_M) && (0x2A0 >= size_Section1))
				continue;

			node = new TiXmlElement("filterColor");  node_shadow->LinkEndChild(node);
			node->SetAttribute("r", FloatToString(section1->shadows_filterColor_r));
			node->SetAttribute("g", FloatToString(section1->shadows_filterColor_g));
			node->SetAttribute("b", FloatToString(section1->shadows_filterColor_b));


			node = new TiXmlElement("unk_17_3"); node->SetAttribute("float", FloatToString(section1->unk_17_3)); node_SPMSection1->LinkEndChild(node);

			if ((isVersion_M) && (0x2B0 >= size_Section1))
				continue;


			TiXmlElement* node_sunGodRay = new TiXmlElement("sunGodRay"); 
			node_sunGodRay->LinkEndChild(new TiXmlComment("sunHaloExternalRadius or intensity. polardistance: distance of center of god ray from scene center")); node_SPMSection1->LinkEndChild(node_sunGodRay);
			node = new TiXmlElement("color");  node_sunGodRay->LinkEndChild(node);
			node->SetAttribute("r", FloatToString(section1->sunGodRay_color_r));
			node->SetAttribute("g", FloatToString(section1->sunGodRay_color_g));
			node->SetAttribute("b", FloatToString(section1->sunGodRay_color_b));
			node = new TiXmlElement("intensity"); node->SetAttribute("float", FloatToString(section1->sunGodRay_intensity)); node_sunGodRay->LinkEndChild(node);

			


			TiXmlElement* node_limitShadow = new TiXmlElement("limitShadow"); node_limitShadow->LinkEndChild(new TiXmlComment("over limit it's all dark. highLimit: is from Y up. radiusLimit: is from player position")); node_SPMSection1->LinkEndChild(node_limitShadow);
			node = new TiXmlElement("highLimit");  node_limitShadow->LinkEndChild(node);
			node->SetAttribute("start", FloatToString(section1->limitShadow_high_start));
			node->SetAttribute("end", FloatToString(section1->limitShadow_high_end));
			node = new TiXmlElement("radiusLimit");  node_limitShadow->LinkEndChild(node);
			node->SetAttribute("start", FloatToString(section1->limitShadow_radius_start));
			node->SetAttribute("end", FloatToString(section1->limitShadow_radius_end));


			for (size_t j = 0; j < 8; j++)
			{
				node = new TiXmlElement("unk_17_4_" + std::to_string(j)); node->SetAttribute("float", FloatToString(section1->unk_17_4[j])); node_SPMSection1->LinkEndChild(node);
				if ((isVersion_M) && (0x2D0 + (j + 1) * sizeof(float) >= size_Section1))
					break;
			}



			node = new TiXmlElement("polardistance"); node->SetAttribute("float", FloatToString(section1->sunGodRay_polardistance)); node_sunGodRay->LinkEndChild(node);
			node = new TiXmlElement("unk"); node->SetAttribute("u32", UnsignedToString(section1->sunGodRay_unk, true)); node_sunGodRay->LinkEndChild(new TiXmlComment("limit of the sunlensflare raycast to be displayed. if positive, you could see the halo in the camera by looking at exact inverse direction(camera between character and the sun).Ok also negative ... just differet of 0. After recheck , it could be not a float, so it's could no be limit also.")); node_sunGodRay->LinkEndChild(node);
			node = new TiXmlElement("fading"); node_sunGodRay->LinkEndChild(new TiXmlComment("attenuationFactor : factorFading /attenuation for god ray , if 1, no godray visible. subdivision: subdivision of the godray witch each the previous factor applyed, more subdivision, more fast is atternuate"));  node_sunGodRay->LinkEndChild(node);
			node->SetAttribute("distance", FloatToString(section1->sunGodRay_fading_distance)); 
			node->SetAttribute("attenuationFactor", FloatToString(section1->sunGodRay_fading_attenuationFactor));
			node->SetAttribute("subdivision", FloatToString(section1->sunGodRay_fading_subdivision));


			node = new TiXmlElement("repetition"); node_sunGodRay->LinkEndChild(new TiXmlComment("angle: a ray partern repeted every X degrees ")); node_sunGodRay->LinkEndChild(node);
			node->SetAttribute("angle", FloatToString(section1->sunGodRay_repetition_angle)); 
			node->SetAttribute("subdivision", FloatToString(section1->sunGodRay_repetition_subdivision));

			node = new TiXmlElement("unk_17_5"); node->SetAttribute("float", FloatToString(section1->unk_17_5)); node_SPMSection1->LinkEndChild(node);

			



			if (!isVersion_M)
			{
				SPM_Section1_E* section1_E = (SPM_Section1_E*)GetOffsetPtr_native(buf, startOffsetSection1 +  sizeof(SPM_Section1));

				TiXmlElement* node_SPMSection1_E = new TiXmlElement("Section1_Ext");
				node_SPMSection1->LinkEndChild(node_SPMSection1_E);



				node = new TiXmlElement("unk_18"); node->SetAttribute("u16", UnsignedToString(section1_E->unk_18, true)); node_SPMSection1_E->LinkEndChild(node);
				node = new TiXmlElement("unk_19"); node->SetAttribute("u16", UnsignedToString(section1_E->unk_19, true)); node_SPMSection1_E->LinkEndChild(node);
				node = new TiXmlElement("unk_20"); node->SetAttribute("u16", UnsignedToString(section1_E->unk_20, true)); node_SPMSection1_E->LinkEndChild(node);
				node = new TiXmlElement("unk_21"); node->SetAttribute("u16", UnsignedToString(section1_E->unk_21, true)); node_SPMSection1_E->LinkEndChild(node);
				node = new TiXmlElement("unk_22"); node->SetAttribute("u16", UnsignedToString(section1_E->unk_22, true)); node_SPMSection1_E->LinkEndChild(node);
				node = new TiXmlElement("unk_23"); node->SetAttribute("u16", UnsignedToString(section1_E->unk_23, true)); node_SPMSection1_E->LinkEndChild(node);
				node = new TiXmlElement("unk_24"); node->SetAttribute("u16", UnsignedToString(section1_E->unk_24, true)); node_SPMSection1_E->LinkEndChild(node);
				node = new TiXmlElement("unk_25"); node->SetAttribute("u16", UnsignedToString(section1_E->unk_25, true)); node_SPMSection1_E->LinkEndChild(node);


				TiXmlElement* node_characterBorderStroke = new TiXmlElement("characterBorderStroke"); node_SPMSection1_E->LinkEndChild(node_characterBorderStroke);
				node_characterBorderStroke->LinkEndChild(new TiXmlComment("it's about a shape of character, back to him. and it's 4 of this, and with the width you translate them enought to make like if character have (hard) halo around it"));
				node_characterBorderStroke->SetAttribute("enable", UnsignedToString(section1_E->characterBorderStroke_enable, true));
				node = new TiXmlElement("width"); node_characterBorderStroke->LinkEndChild(node);
				node->SetAttribute("near", FloatToString(section1_E->characterBorderStroke_nearWidth));
				node->SetAttribute("far", FloatToString(section1_E->characterBorderStroke_farWidth));
				node = new TiXmlElement("distance"); node_characterBorderStroke->LinkEndChild(node);
				node->SetAttribute("start", FloatToString(section1_E->characterBorderStroke_startDist));
				node->SetAttribute("end", FloatToString(section1_E->characterBorderStroke_endDist));

				node = new TiXmlElement("color"); node_characterBorderStroke->LinkEndChild(node);
				node->SetAttribute("r", FloatToString(((float)section1_E->characterBorderStroke_color_r) / 255.0f));
				node->SetAttribute("g", FloatToString(((float)section1_E->characterBorderStroke_color_g) / 255.0f));
				node->SetAttribute("b", FloatToString(((float)section1_E->characterBorderStroke_color_b) / 255.0f));
				node->SetAttribute("a", FloatToString(((float)section1_E->characterBorderStroke_color_a) / 255.0f));

				
				//todo continue, <UnknowEnd> <unk_6 u32="0x0" />	 Enable Effect  , SPM_Section1_E = uint32_t unk_26[10];	// 28

				node = new TiXmlElement("unk_26_0"); node->SetAttribute("u32", UnsignedToString(section1_E->unk_26_0, true)); node_SPMSection1_E->LinkEndChild(node);
				
				for (size_t j = 0; j < 5; j++)
				{
					node = new TiXmlElement("unk_26_1_" + std::to_string(j)); node->SetAttribute("float", FloatToString(section1_E->unk_26_1[j])); node_SPMSection1_E->LinkEndChild(node);
				}

				node = new TiXmlElement("unk_26_2"); node->SetAttribute("u32", UnsignedToString(section1_E->unk_26_2, true)); node_SPMSection1_E->LinkEndChild(node);

				for (size_t j = 0; j < 2; j++)
				{
					node = new TiXmlElement("unk_26_3_" + std::to_string(j)); node->SetAttribute("float", FloatToString(section1_E->unk_26_3[j])); node_SPMSection1_E->LinkEndChild(node);
				}

				node = new TiXmlElement("unk_26_4"); node->SetAttribute("u32", UnsignedToString(section1_E->unk_26_4, true)); node_SPMSection1_E->LinkEndChild(node);

				

				size_t startUnkEnd = startOffsetSection1 + sizeof(SPM_Section1) + sizeof(SPM_Section1_E);
				size_t sizeUnkEnd = size_Section1 - (sizeof(SPM_Section1) + sizeof(SPM_Section1_E));
				uint32_t* listUnkEnd = (uint32_t*)GetOffsetPtr_native(buf, startUnkEnd);

				TiXmlElement* node_SPMSection1_UnknowEnd = new TiXmlElement("UnknowEnd");
				node_SPMSection1_E->LinkEndChild(node_SPMSection1_UnknowEnd);

				size_t inc = 0;
				for (size_t j = 0; j < sizeUnkEnd; j += 4, inc++)
				{
					node = new TiXmlElement("unk_" + std::to_string(inc)); node->SetAttribute("u32", UnsignedToString(listUnkEnd[inc], true)); node_SPMSection1_UnknowEnd->LinkEndChild(node);
					//todo check the possiblity to have a float.
				}	
			}
		}
		parent->LinkEndChild(node_ListS1);
	}
	
	



	if (offset_Section2)
	{
		SPM_Section2* section2 = (SPM_Section2*)GetOffsetPtr_native(buf, offset_Section2);

		TiXmlElement* node_ListDsc = new TiXmlElement("ListDSC");

		for (size_t i = 0; i < number_Section2; i++)
		{
			size_t startSection2 = offset_Section2 + i * sizeof(SPM_Section2); 

			TiXmlElement* node_SPMSection2 = new TiXmlElement("DSC");

			if (section2[i].offset_DscFile)
			{
				size_t startDSC = section2[i].offset_DscFile + offset_Section2;
				DSC_Header* dscHdr = (DSC_Header*)GetOffsetPtr_native(buf, startDSC);

				node = new TiXmlElement("number_Section1"); node->SetAttribute("u32", UnsignedToString(dscHdr->number_Section1, false)); node_SPMSection2->LinkEndChild(node);
				node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(dscHdr->unk_0, true)); node_SPMSection2->LinkEndChild(node);
				node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(dscHdr->unk_1, true)); node_SPMSection2->LinkEndChild(node);
				node = new TiXmlElement("offset_Section1"); node->SetAttribute("startOffset", UnsignedToString(dscHdr->offset_Section1, true)); node_SPMSection2->LinkEndChild(node);
				node = new TiXmlElement("unk_3"); node->SetAttribute("u32", UnsignedToString(dscHdr->unk_3, true)); node_SPMSection2->LinkEndChild(new TiXmlComment("DON'T TOUCH OR IT CRASH"));  node_SPMSection2->LinkEndChild(node);


				TiXmlElement* node_DscListSection1 = new TiXmlElement("ListSect_1");

				if (dscHdr->offset_Section1)
				{
					size_t startDscListSect_1 = dscHdr->offset_Section1 + startDSC;

					DSC_Section1* dsc_Section1 = (DSC_Section1*)GetOffsetPtr_native(buf, startDscListSect_1);

					for (size_t j = 0; j < dscHdr->number_Section1; j++)
					{
						offset = startDscListSect_1 + j * sizeof(DSC_Section1);

						TiXmlElement* node_DscSection1 = new TiXmlElement("Sect_1");

						node = new TiXmlElement("offset_Section1_b"); node->SetAttribute("startOffset", UnsignedToString(dsc_Section1[j].offset_Section1_b, true)); node_DscSection1->LinkEndChild(node);
						node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(dsc_Section1[j].unk_0, true)); node_DscSection1->LinkEndChild(new TiXmlComment("DON'T TOUCH OR IT CRASH")); node_DscSection1->LinkEndChild(node);
						
						if (dsc_Section1[j].offset_Section1_b)
						{
							size_t offsetSection1b = dsc_Section1[j].offset_Section1_b + startDscListSect_1;
							offset = offsetSection1b;

							DSC_Section1_b* dsc_Section1_b = (DSC_Section1_b*)GetOffsetPtr_native(buf, offsetSection1b);

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
							node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(dsc_Section1_b->unk_1, true)); node_DscSection1_b->LinkEndChild(new TiXmlComment("DON'T TOUCH OR IT CRASH")); node_DscSection1_b->LinkEndChild(node);
							node = new TiXmlElement("offset_Section1_c"); node->SetAttribute("startOffset", UnsignedToString(dsc_Section1_b->offset_Section1_c, true)); node_DscSection1_b->LinkEndChild(node);
							node = new TiXmlElement("unk_2"); node->SetAttribute("u32", UnsignedToString(dsc_Section1_b->unk_2, true)); node_DscSection1_b->LinkEndChild(new TiXmlComment("DON'T TOUCH OR IT CRASH")); node_DscSection1_b->LinkEndChild(node);

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
									DSC_Section1_c* dsc_Section1_c = (DSC_Section1_c*)GetOffsetPtr_native(buf, offset);

									node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(dsc_Section1_c->unk_0, true)); node_DscSection1_c->LinkEndChild(node);
									node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(dsc_Section1_c->unk_1, true)); node_DscSection1_c->LinkEndChild(node);
									node = new TiXmlElement("unk_2"); node->SetAttribute("u32", UnsignedToString(dsc_Section1_c->unk_2, true)); node_DscSection1_c->LinkEndChild(node);
									node = new TiXmlElement("unk_3"); node->SetAttribute("u32", UnsignedToString(dsc_Section1_c->unk_3, true)); node_DscSection1_c->LinkEndChild(node);
									node = new TiXmlElement("unk_4"); node->SetAttribute("u32", UnsignedToString(dsc_Section1_c->unk_4, true)); node_DscSection1_c->LinkEndChild(node);
									
									node = new TiXmlElement("Color"); 
									node->SetAttribute("R", FloatToString(dsc_Section1_c->color[0]));
									node->SetAttribute("G", FloatToString(dsc_Section1_c->color[1]));
									node->SetAttribute("B", FloatToString(dsc_Section1_c->color[2]));
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

	originefilename = filename;

	buf = EMO_BaseFile::ReadFile(filename, &size, show_error);
	if (!buf)
		return;


	BinColorTag binCt;

	write_Coloration(binCt, binCt.startColoration(filename), buf, size);

	binCt.endColoration(filename);

	return;
}

/*-------------------------------------------------------------------------------\
|                             write_Coloration				                     |
\-------------------------------------------------------------------------------*/
void Spm::write_Coloration(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	SPMHeader_X* hdr = (SPMHeader_X*)buf;
	if (size < sizeof(SPMHeader_E))
		return;

	bool isVersion_M = (memcmp(hdr->signature, SPM_SIGNATURE_M, 4) == 0);				//first version, on old games
	bool isVersion_E = (memcmp(hdr->signature, SPM_SIGNATURE_E, 4) == 0);				//new version
	bool isVersion_X = (memcmp(hdr->signature, SPM_SIGNATURE_X, 4) == 0);				//version SPE + DSC.

	bool isVersion_M_first = false;
	if ((hdr->signature[0] == 0) && (hdr->signature[1] == 0) && (hdr->signature[2] == 0) && (hdr->signature[3] == 0))	//version before SPM, with no tag and name, just 00 at the same place.
	{
		isVersion_M = true;
		isVersion_M_first = true;
	}

	if ((!isVersion_M) && (!isVersion_E) && (!isVersion_X))
		return;

	string type = isVersion_M ? "SPM" : (isVersion_E ? "SPE" : "SPX");
	parent->SetAttribute("type", type);

	size_t size_Section1 = hdr->size_Section1;
	size_t offset_Section1 = hdr->offset_Section1;
	size_t number_Section2 = hdr->number_Section2;
	size_t offset_Section2 = hdr->offset_Section2;

	if (!isVersion_X)
	{
		number_Section2 = 0;
		offset_Section2 = 0;
	}
	if (isVersion_M)
	{
		offset_Section1 = size_Section1;
		size_Section1 = 0x310;					//Todo confirm with color tags;
		string name = nameFromFilename(originefilename);
		if ((name == "ARD.spm") || (name == "IOR.spm") || (name == "JMN.spm") ||
			(name == "MUU.spm") || (name == "SHK.spm") || (name == "SRU.spm"))
			size_Section1 = 0x2B0;				//688
		else if ((name == "DTH.spm") || (name == "MST.spm"))
			size_Section1 = 0x290;				//656
	}

	

	std::vector<bool> listBytesAllreadyTagged;
	listBytesAllreadyTagged.resize(size, false);				//to check override of the same byte (overflow)


	size_t idTag = 0;
	size_t offset = 0;
	size_t incSection = 0;
	size_t incParam = 0;

	binCt.write_Coloration_Tag("signature", "string", "", offset, 4 * sizeof(char), "SPMHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);	offset += 4 * sizeof(char);
	if (!isVersion_M)
	{
		binCt.write_Coloration_Tag("version", "uint32_t", "", offset, sizeof(uint32_t), "SPMHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	}else {
		binCt.write_Coloration_Tag("name", "4 x char", "", offset, sizeof(uint32_t), "SPMHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	}
	
	binCt.write_Coloration_Tag("number_Section1", "uint32_t", "maximum 8 (NLBY)", offset, sizeof(uint32_t), "SPMHeader", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	if (!isVersion_M)
	{
		binCt.write_Coloration_Tag("size_Section1", "uint32_t", "", offset, sizeof(uint32_t), "SPMHeader", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	}
	binCt.write_Coloration_Tag("offset_Section1", "uint32_t", "", offset, sizeof(uint32_t), "SPMHeader", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

	if (isVersion_X)
	{
		binCt.write_Coloration_Tag("number_Section2", "uint32_t", "always 1 (if X version)", offset, sizeof(uint32_t), "SPMHeader", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged);	offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("offset_Section2", "uint32_t", "", offset, sizeof(uint32_t), "SPMHeader", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged);	offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("padding", "padding", "", offset, sizeof(uint32_t), "SPMHeader", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged);	offset += sizeof(uint32_t);
	}



	SPM_Section1 *section1 = (SPM_Section1*)GetOffsetPtr_native(buf, offset_Section1);
	offset = offset_Section1;
	size_t offset_tmp = 0;
	incSection++;
	for (size_t i = 0; i < hdr->number_Section1; i++)
	{
		size_t offsetSection1 = offset_Section1 + i * size_Section1;
		offset = offsetSection1;

		incParam = 0;
		binCt.write_Coloration_Tag("name", "string", "interresting Names : all stages names, Scouter_Blue, toFLBY01, BFnms_LightS_Off, BFice_water", offset, 16 * sizeof(char), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += 16 * sizeof(char);

		binCt.write_Coloration_Tag("lenFlareEnabled", "uint16_t", "always 1", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unk_1", "uint16_t", "0, 1 (most)", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unk_2", "uint16_t", "always 0", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unk_3", "uint16_t", "0, 1 (most)", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unk_4", "uint16_t", "0 (most), 1", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unk_5", "uint16_t", "0, 1 (most)", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unk_6", "uint16_t", "0 (most), 1", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unk_7", "uint16_t", "always 0", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);

		for (size_t j=0; j < 4; j++)
		{
			binCt.write_Coloration_Tag("unk_8_a_0_"+ std::to_string(j), "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		}

		binCt.write_Coloration_Tag("lightDirection_x", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("lightDirection_y", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("lightDirection_z", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("lightDirection_w", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		for (size_t j = 0; j < 19; j++)
		{
			binCt.write_Coloration_Tag("unk_8_a_1_" + std::to_string(j), "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		}

		binCt.write_Coloration_Tag("unk_8_a_1_a", "uint32_t", "", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);

		for (size_t j = 0; j < 23; j++)
		{
			binCt.write_Coloration_Tag("unk_8_a_1_b_" + std::to_string(j), "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		}

		binCt.write_Coloration_Tag("unk_8_a_1_c", "uint32_t", "", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);

		for (size_t j = 0; j < 4; j++)
		{
			binCt.write_Coloration_Tag("unk_8_a_1_d_" + std::to_string(j), "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		}


		binCt.write_Coloration_Tag("fogColor_r", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("fogColor_g", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("fogColor_b", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("fogColor_a", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);


		binCt.write_Coloration_Tag("fog_startDist", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("fog_endDist", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		binCt.write_Coloration_Tag("unk_8_a_2", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("colorSaturation", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		binCt.write_Coloration_Tag("mutiplyColor_r", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("mutiplyColor_g", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("mutiplyColor_b", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		binCt.write_Coloration_Tag("filterColor_r", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("filterColor_g", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("filterColor_b", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		binCt.write_Coloration_Tag("additiveColor_r", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("additiveColor_g", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("additiveColor_b", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		binCt.write_Coloration_Tag("filterHotCold", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);


		for (size_t j = 0; j < 5; j++)
		{
			binCt.write_Coloration_Tag("unk_8_a_3_" + std::to_string(j), "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		}

		binCt.write_Coloration_Tag("mutiplyInverseFactor", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("blur_startDist", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("unk_8_a_4", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);





		
		binCt.write_Coloration_Tag("halo_alphaRejection", "uint16_t", "", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unk_8_c", "uint16_t", "", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		





		binCt.write_Coloration_Tag("unk_8_d_0", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("shadow_endAngle", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("unk_8_d_1", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("shadow_startAngle", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		binCt.write_Coloration_Tag("sunFactor", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("sunFactor_hiden", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("sunSize", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		binCt.write_Coloration_Tag("unk_8_d_2", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("unk_8_d_3", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		binCt.write_Coloration_Tag("highIlluminated_glareInverseFactor", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		binCt.write_Coloration_Tag("background_glare_additiveColor_r", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("background_glare_additiveColor_g", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("background_glare_additiveColor_b", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("background_glare_additiveColor_a", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		for (size_t j = 0; j < 6; j++)
		{
			binCt.write_Coloration_Tag("unk_8_d_4_" + std::to_string(j), "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		}

		binCt.write_Coloration_Tag("character_glare_additiveColor_r", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("character_glare_additiveColor_g", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("character_glare_additiveColor_b", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);




		






		binCt.write_Coloration_Tag("unk_8_e", "uint32_t", "", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);





		binCt.write_Coloration_Tag("edgeStrokes_factor", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("edgeStrokes_width", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		binCt.write_Coloration_Tag("edgeStrokes_color_r", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("edgeStrokes_color_g", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("edgeStrokes_color_b", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		for (size_t j = 0; j < 4; j++)
		{
			binCt.write_Coloration_Tag("unk_8_f_" + std::to_string(j), "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		}



		binCt.write_Coloration_Tag("unk_8_f_1", "uint32_t", "", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		for (size_t j = 0; j < 3; j++)
		{
			binCt.write_Coloration_Tag("unk_8_f_2_" + std::to_string(j), "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		}

		binCt.write_Coloration_Tag("unk_8_f_3", "uint32_t", "", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		for (size_t j = 0; j < 3; j++)
		{
			binCt.write_Coloration_Tag("unk_8_f_4_" + std::to_string(j), "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		}

		binCt.write_Coloration_Tag("unk_8_f_5", "uint32_t", "", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		for (size_t j = 0; j < 3; j++)
		{
			binCt.write_Coloration_Tag("unk_8_f_6_" + std::to_string(j), "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		}






		binCt.write_Coloration_Tag("unk_9", "uint16_t", "always 1", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unk_10", "uint16_t", "always 0", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unk_11", "uint16_t", "0 (most),1, 4, 5, 6, 7, 8, 9, 0xa,   0x100, 0x101,0x102, 0x103, 0x104, 0x105, 0x106, seam to be 2 x uint8", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unk_12", "uint16_t", "always 0xffff", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unk_13", "uint16_t", "0, 1 (most)", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unk_14", "uint16_t", "always 0", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unk_15", "uint16_t", "0, 1 (most)", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		binCt.write_Coloration_Tag("unk_16", "uint16_t", "0 (most), 1", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);














		for (size_t j = 0; j < 4; j++)
		{
			binCt.write_Coloration_Tag("unk_17_0_" + std::to_string(j), "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		}

		binCt.write_Coloration_Tag("backgroundSpecifics_multiplycolor_r", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("backgroundSpecifics_multiplycolor_g", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("backgroundSpecifics_multiplycolor_b", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("backgroundSpecifics_multiplycolor_a", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		
		for (size_t j = 0; j < 4; j++)
		{
			binCt.write_Coloration_Tag("unk_17_1_" + std::to_string(j), "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		}

		binCt.write_Coloration_Tag("shadowDirection_x", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("shadowDirection_y", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("shadowDirection_z", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);


		for (size_t j = 0; j < 13; j++)
		{
			binCt.write_Coloration_Tag("unk_17_2_" + std::to_string(j), "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

			if ((isVersion_M) && (offset - offsetSection1 >= size_Section1))
				break;
		}
		if ((isVersion_M) && (offset - offsetSection1 >= size_Section1))
			continue;



		binCt.write_Coloration_Tag("shadows_filterColor_r", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("shadows_filterColor_g", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("shadows_filterColor_b", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		binCt.write_Coloration_Tag("unk_17_3", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		if ((isVersion_M) && (offset - offsetSection1 >= size_Section1))
			continue;

		binCt.write_Coloration_Tag("sunGodRay_color_r", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("sunGodRay_color_g", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("sunGodRay_color_b", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("sunGodRay_intensity", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		binCt.write_Coloration_Tag("limitShadow_high_start", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("limitShadow_high_end", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("limitShadow_radius_start", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("limitShadow_radius_end", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);


		for (size_t j = 0; j < 8; j++)
		{
			binCt.write_Coloration_Tag("unk_17_4_" + std::to_string(j), "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		}

		binCt.write_Coloration_Tag("sunGodRay_polardistance", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("sunGodRay_unk", "uint32_t", "", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("sunGodRay_fading_distance", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("sunGodRay_fading_attenuationFactor", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("sunGodRay_fading_subdivision", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("sunGodRay_repetition_angle", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		binCt.write_Coloration_Tag("sunGodRay_repetition_subdivision", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		binCt.write_Coloration_Tag("unk_17_5", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);




		if (!isVersion_M)
		{
			binCt.write_Coloration_Tag("unk_18", "uint16_t", "always 1", offset, sizeof(uint16_t), "Section1_E", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("unk_19", "uint16_t", "always 0", offset, sizeof(uint16_t), "Section1_E", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("unk_20", "uint16_t", "same as unk_11", offset, sizeof(uint16_t), "Section1_E", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("unk_21", "uint16_t", "same as unk_12", offset, sizeof(uint16_t), "Section1_E", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("unk_22", "uint16_t", "same as unk_13", offset, sizeof(uint16_t), "Section1_E", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("unk_23", "uint16_t", "same as unk_14", offset, sizeof(uint16_t), "Section1_E", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("unk_24", "uint16_t", "same as unk_15", offset, sizeof(uint16_t), "Section1_E", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("unk_25", "uint16_t", "same as unk_16", offset, sizeof(uint16_t), "Section1_E", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);



			binCt.write_Coloration_Tag("characterBorderStroke_enable", "uint32_t", "", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			binCt.write_Coloration_Tag("characterBorderStroke_farWidth", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			binCt.write_Coloration_Tag("characterBorderStroke_nearWidth", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			binCt.write_Coloration_Tag("characterBorderStroke_startDist", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
			binCt.write_Coloration_Tag("characterBorderStroke_endDist", "float", "", offset, sizeof(float), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

			binCt.write_Coloration_Tag("characterBorderStroke_color_rbga", "uint32_t", "", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			


			binCt.write_Coloration_Tag("unk_26_0", "uint32_t", "", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			for (size_t j = 0; j < 5; j++)
			{
				binCt.write_Coloration_Tag("unk_26_1_" + std::to_string(j), "uint32_t", "", offset, sizeof(uint32_t), "Section1_E", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			}

			binCt.write_Coloration_Tag("unk_26_2", "uint32_t", "", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			for (size_t j = 0; j < 2; j++)
			{
				binCt.write_Coloration_Tag("unk_26_3_" + std::to_string(j), "uint32_t", "", offset, sizeof(uint32_t), "Section1_E", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
			}

			binCt.write_Coloration_Tag("unk_26_4", "uint32_t", "", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		}
		

		
		// ...
		

		/*
		//offsets
		offset_tmp = section1[i].offsetSection2;
		if (offset_tmp)
		{
			offset_tmp += offsetSection1;
			offset = offset_tmp;
			
			uint32_t* tmp = (uint32_t*)GetOffsetPtr_native(buf, offset);

			for (size_t j = 0; j < section1[i].numberSection2; j++)
			{
				binCt.write_Coloration_Tag("offsetSection2b", "uint32_t", "Offset (add with the start of Section1[" + std::to_string(i) + "] ), target a section2 => " + UnsignedToString(tmp[j] + offsetSection1, true), offset, sizeof(uint32_t), "Section1b", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
				listOffsetfromSection1ToSection2.push_back(tmp[j] + offsetSection1);			// we will made the link on header of Section2
			}
		}
		*/
	}




	SPM_Section2 *section2 = (SPM_Section2*)GetOffsetPtr_native(buf, offset_Section2);
	offset = offset_Section2;
	incSection += 2;			//todo update depend of Section1
	incParam = 0;
	for (size_t i = 0; i < number_Section2; i++)
	{
		size_t offsetSection2 = offset_Section2 + i * sizeof(SPM_Section2);
		offset = offsetSection2;

		binCt.write_Coloration_Tag("offset_DscFile", "uint32_t", "Start of Section2["+ std::to_string(i) +"] => " + UnsignedToString(offset_Section2 + section2[i].offset_DscFile, true), offset, sizeof(uint32_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	}



	offset = offset_Section2;
	incSection++;
	for (size_t i = 0; i < number_Section2; i++)
	{
		size_t offsetSection2 = offset_Section2 + i * sizeof(SPM_Section2);
		size_t offsetDsc = section2[i].offset_DscFile + offset_Section2;
		offset = offsetDsc;


		DSC_Header* dsc_hdr = (DSC_Header*)GetOffsetPtr_native(buf, offsetDsc);

		incParam = 0;
		binCt.write_Coloration_Tag("start_DscFile, signature", "uint32_t", "Start of Dsc[" + std::to_string(i) + "]", offset, sizeof(uint32_t), "Dsc_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("number_Section1", "uint32_t", "1 is most, 7 is max, 0 is min", offset, sizeof(uint32_t), "Dsc_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("unk_0", "uint32_t", "always 0x30", offset, sizeof(uint32_t), "Dsc_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("unk_1", "uint32_t", "Todo redo analyze", offset, sizeof(uint32_t), "Dsc_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("offset_Section1", "uint32_t", "", offset, sizeof(uint32_t), "Dsc_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("unk_3", "uint32_t", "always 0", offset, sizeof(uint32_t), "Dsc_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);


		//offsets
		offset_tmp = dsc_hdr->offset_Section1;
		if (offset_tmp)
		{
			offset_tmp += offsetDsc;
			size_t offsetSection1 = offset_tmp;

			DSC_Section1* dsc_Section1 = (DSC_Section1*)GetOffsetPtr_native(buf, offsetSection1);

			for (size_t j = 0; j < dsc_hdr->number_Section1; j++)
			{
				offset = offsetSection1 + j * sizeof(DSC_Section1);

				incParam = 0;
				binCt.write_Coloration_Tag("offset_Section1_b", "uint32_t", "Offset (add with the start of Dsc), => " + UnsignedToString(dsc_Section1[j].offset_Section1_b + offsetSection1, true), offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
				binCt.write_Coloration_Tag("unk_0", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);

				offset_tmp = dsc_Section1[j].offset_Section1_b;
				if (offset_tmp)
				{
					offset_tmp += offsetSection1;
					size_t offsetSection1b = offset_tmp;
					offset = offsetSection1b;
					
					DSC_Section1_b* dsc_Section1_b = (DSC_Section1_b*)GetOffsetPtr_native(buf, offsetSection1b);

					incParam = 0;
					binCt.write_Coloration_Tag("name", "string", " same as SpmSection1", offset, 16 * sizeof(char), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += 16 * sizeof(char);
					binCt.write_Coloration_Tag("number_Section1_c", "uint32_t", "1 (most) or 2", offset, sizeof(uint32_t), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
					binCt.write_Coloration_Tag("unk_0", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
					binCt.write_Coloration_Tag("size_Section1_c", "uint32_t", " 0x28[11,0,3,0,4,0,2,5;11,0,3,0,4,1,2,5] |0x30 (most) [0,0,3,0,4,0,2,5; ... x 99] So look the difference ", offset, sizeof(uint32_t), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
					binCt.write_Coloration_Tag("unk_1", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
					binCt.write_Coloration_Tag("offset_Section1_c", "uint32_t", " add start Section1_b => "+ UnsignedToString(dsc_Section1_b->offset_Section1_c + offsetSection1b, true), offset, sizeof(uint32_t), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
					binCt.write_Coloration_Tag("unk_2", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);

					if (dsc_Section1_b->offset_Section1_c >= sizeof(DSC_Section1_b))			//some file don't have index.
					{
						binCt.write_Coloration_Tag("index", "uint32_t", " Same values as DscSection1_c and a parameter into SpmSection1", offset, sizeof(uint32_t), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
						binCt.write_Coloration_Tag("unk_3", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1_b", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
					}

					offset_tmp = dsc_Section1_b->offset_Section1_c;
					if (offset_tmp)
					{
						offset_tmp += offsetSection1b;
						size_t offsetSection1c = offset_tmp;

						DSC_Section1_c* dsc_Section1_c = (DSC_Section1_c*)GetOffsetPtr_native(buf, offsetSection1c);

						for (size_t k = 0; k < dsc_Section1_b->number_Section1_c; k++)
						{
							offset = offsetSection1c + k * dsc_Section1_b->size_Section1_c;

							incParam = 0;
							binCt.write_Coloration_Tag("unk_0", "uint32_t", "0,1,4,5,6,7,8,9,0xa,0x100,0x101,0x102,0x103,0x104,0x105, 0xffffffff(none) same as a part in Spm Section1", offset, sizeof(uint32_t), "Section1_c", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint32_t);
							binCt.write_Coloration_Tag("unk_1", "uint32_t", " 0, 0xffffffff (most, none)", offset, sizeof(uint32_t), "Section1_c", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint32_t);
							binCt.write_Coloration_Tag("unk_2", "uint32_t", "0 (most) or 1", offset, sizeof(uint32_t), "Section1_c", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint32_t);
							binCt.write_Coloration_Tag("unk_3", "uint32_t", "0 (most), 0xffffffff (none)", offset, sizeof(uint32_t), "Section1_c", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint32_t);
							binCt.write_Coloration_Tag("unk_4", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1_c", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint32_t);
							binCt.write_Coloration_Tag("Color", "float[3]", " RGB always at 0 0 0", offset, 3 * sizeof(float), "Section1_c", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, k); offset += 3 * sizeof(float);
						}
					}
				}
			}
		}
	}


	binCt.write_Coloration_Gaps(0x0, size, sizeof(uint8_t), idTag, "Empty", parent, listBytesAllreadyTagged);		//test Empty/gaps/holes.
}


}