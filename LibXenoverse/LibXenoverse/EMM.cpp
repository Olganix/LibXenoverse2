
#include "XenoFiles.h"

namespace LibXenoverse
{
	


/*-------------------------------------------------------------------------------\
|                             ~EMM					                             |
\-------------------------------------------------------------------------------*/
EMM::~EMM(void)
{
	size_t nbMat = materials.size();
	for (size_t i = 0; i < nbMat; i++)
		delete materials.at(i);
	materials.clear();
}
/*-------------------------------------------------------------------------------\
|                             EMMMaterial			                             |
\-------------------------------------------------------------------------------*/
EMMMaterial::~EMMMaterial(void)
{
	size_t nbParam = parameters.size();
	for (size_t i = 0; i < nbParam; i++)
		delete parameters.at(i);
	parameters.clear();

	listSampler2D.clear();
}
/*-------------------------------------------------------------------------------\
|                             EMMParameter			                             |
\-------------------------------------------------------------------------------*/
EMMParameter::EMMParameter(void)
{
	name = "";
	type = 1;
	level = 0;
	uint_value = 0;
	float_value = 0.0f;
}











/*-------------------------------------------------------------------------------\
|                             load					                             |
\-------------------------------------------------------------------------------*/
bool EMM::load(string filename)
{
	name = nameFromFilenameNoExtension(filename, true);
	if (filename.find(".xml") != string::npos)
		return loadXml(filename);

	File file(filename, LIBXENOVERSE_FILE_READ_BINARY);

	if( (!file.valid()) || (!file.readHeader(LIBXENOVERSE_EMM_SIGNATURE)) )
		return false;
	
	read(&file);
	file.close();

	return true;
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void EMM::read(File *file)
{
	file->goToAddress(0x6);

	uint16_t header_size;
	file->readInt16E(&header_size);

	version = "";
	uint8_t tmp = 0;
	file->readUChar(&tmp);
	version += ToString((uint32_t)tmp) + ".";
	file->readUChar(&tmp);
	version += ToString((uint32_t)tmp) + ".";
	file->readUChar(&tmp);
	version += ToString((uint32_t)tmp) + ".";
	file->readUChar(&tmp);
	version += ToString((uint32_t)tmp);


	unsigned int material_base_address = 0;
	file->readInt32E(&material_base_address);

	unsigned int material_count = 0;
	file->goToAddress(material_base_address);
	file->readInt32E(&material_count);

	printf("Found %d materials.\n", material_count);

	materials.resize(material_count);
	for (size_t i = 0; i < material_count; i++)
	{
		file->goToAddress(material_base_address + i * 4 + 4);

		unsigned int material_address = 0;
		file->readInt32E(&material_address);
		file->goToAddress(material_base_address + material_address);

		materials.at(i) = new EMMMaterial();
		materials.at(i)->read(file);
	}


	//reading about (may be) default values of each parameter ( (may be) for other material in emm). all I remark there is the same number of octets at 0  than unique parameters.
	if (header_size > 0x10)
	{
		listUnknowValues.clear();
		
		file->goToAddress(0x10);
		unsigned int defaultValues_address = 0;
		file->readInt32E(&defaultValues_address);

		if (header_size > 0x14)
		{
			file->readInt32E(&unknow_0);
			if (header_size > 0x18)
			{
				file->readInt32E(&unknow_1);
				if (header_size > 0x1C)
				{
					file->readInt32E(&unknow_2);
				}
			}
		}

		if ((defaultValues_address != 0)&&(defaultValues_address < file->getFileSize()))			//not all emm have this. I don't know why yet.
		{
			file->goToAddress(defaultValues_address);

			for (size_t i = 0; i < 68; i++)				//always 68 octets
			{
				file->readUChar(&tmp);
				listUnknowValues.push_back((size_t)tmp);
			}
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void EMMMaterial::read(File *file)
{
	char buffer[32];
	file->read(buffer, 32);
	name = string(buffer);
	file->read(buffer, 32);
	shaderProgramName = string(buffer);

	unsigned short parameter_count = 0;
	file->readInt16E(&parameter_count);
	file->readInt16E(&unknow_0);

	printf("Reading Material %s with shader %s and %d parameters.\n", name.c_str(), shaderProgramName.c_str(), parameter_count);

	parameters.resize(parameter_count);
	for (size_t i = 0; i < parameter_count; i++)
	{
		parameters.at(i) = new EMMParameter();
		parameters.at(i)->read(file);
	}
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void EMMParameter::read(File *file)
{
	char buffer[32];
	file->read(buffer, 32);
	name = string(buffer);

	file->readInt16E(&type);
	file->readInt16E(&level);
	
	if (type == 0x0)						//Float
	{
		file->readFloat32E(&float_value);

	}else if (type == 0x1) {				//Uint

		file->readInt32E(&uint_value);

	}else {
		printf("Unknown Parameter Flag %x\n", type);
		notifyError();
		file->readInt32E(&uint_value);
	}
}




/*-------------------------------------------------------------------------------\
|                             save					                             |
\-------------------------------------------------------------------------------*/
void EMM::save(string filename, bool big_endian)
{
	File file(filename, LIBXENOVERSE_FILE_WRITE_BINARY);
	if (file.valid())
	{
		file.writeHeader(LIBXENOVERSE_EMM_SIGNATURE, big_endian);

		file.goToAddress(file.getCurrentAddress() - 2);						//header goes to far, endian finish on "FEFF"

		write(&file);
		file.close();
	}
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void EMM::write(File *file)
{	
	if ((listUnknowValues.size() != 0) && (version == "0.0.0.0"))			//header size is 0x10 if version is 0.0.0.0. but if in xml people add unknowValues part, we force to have the 0x20, because the offset of unknowpart is in. so we force the version.
		version = "0.147.0.0";

	// Header
	uint16_t header_size = (version == "0.0.0.0") ? 0x10 : 0x20;
	file->writeInt16E(&header_size);

	if (version.length() == 0)
		version = "0";
	std::vector<string> sv = split(version, '.');
	for (size_t i = 0; i < 4; i++)
	{
		if (i < sv.size())
		{
			uint8_t tmp = std::stoi(sv.at(i));
			file->writeUChar(&tmp);
		}else {
			file->writeNull(1);
		}
	}
	
	unsigned int material_base_address = header_size;
	file->writeInt32E(&material_base_address);

	if (header_size == 0x20)
	{
		file->writeNull(4);
		file->writeInt32E(&unknow_0);
		file->writeInt32E(&unknow_1);
		file->writeInt32E(&unknow_2);
	}
		



	unsigned int material_count = materials.size();
	file->writeInt32E(&material_count);
	file->writeNull( ((material_count > 0) ? material_count : 1) * sizeof(uint32_t));								//case dbxv Ibm_Crg_light.emm there is a padding of 4 to be on % 8

	for (size_t i = 0; i < material_count; i++)
	{
		unsigned int material_address = file->getCurrentAddress() - material_base_address;
		file->goToAddress(material_base_address + i * 4 + 4);

		file->writeInt32E(&material_address);

		file->goToAddress(material_address + material_base_address);
		materials.at(i)->write(file);
	}


	//writing about (may be) default values of each parameter ( (may be) for other material in emm). all I remark there is the same number of octets at 0  than unique parameters.
	if (listUnknowValues.size() != 0)							// notice header is extended to 0x20 in case of presence of unkowPart.
	{
		file->goToEnd();
		size_t endAdress = file->getCurrentAddress();
		file->goToAddress(0x10);
		file->writeInt32E(&endAdress);
		file->goToEnd();

		unsigned char tmp = '\0';
		size_t nbParams = listUnknowValues.size();
		for (size_t i = 0; i < nbParams; i++)
		{
			tmp = (unsigned char)listUnknowValues.at(i);
			file->writeUChar(&tmp);
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void EMMMaterial::write(File *file)
{
	char buffer[32];
	size_t nbChar = name.length();
	for (size_t i = 0; i < 32; i++)
		buffer[i] = (i < nbChar) ? name.at(i) : 0;
	file->write(&buffer, 32);

	nbChar = shaderProgramName.length();
	for (size_t i = 0; i < 32; i++)
		buffer[i] = (i < nbChar) ? shaderProgramName.at(i) : 0;
	file->write(&buffer, 32);

	unsigned short parameter_count = parameters.size();
	file->writeInt16E(&parameter_count);
	file->writeInt16E(&unknow_0);

	for (size_t i = 0; i < parameter_count; i++)
		parameters.at(i)->write(file);
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void EMMParameter::write(File *file)
{
	char buffer[32];
	size_t nbChar = name.length();
	for (size_t i = 0; i < 32; i++)
		buffer[i] = (i < nbChar) ? name.at(i) : 0;
	file->write(&buffer, 32);
	

	file->writeInt16E(&type);
	file->writeInt16E(&level);

	if (type == 0x0)						//Float
	{
		file->writeFloat32E(&float_value);

	}else if (type == 0x1) {				//Uint
		file->writeInt32E(&uint_value);
	}else {
		file->writeInt32E(&uint_value);
	}
}






/*-------------------------------------------------------------------------------\
|                             loadXml				                             |
\-------------------------------------------------------------------------------*/
bool EMM::loadXml(string filename)
{
	if (filename.find(".xml") == string::npos)
		return false;

	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
		return false;

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);

	TiXmlElement* rootNode = hDoc.FirstChildElement("EMM").Element();
	if (!rootNode)
	{
		printf("%s don't have 'EMM' tags. skip.'\n", filename);
		notifyError();
		return false;
	}

	readXML(rootNode);
	return true;
}
/*-------------------------------------------------------------------------------\
|                             readXML				                             |
\-------------------------------------------------------------------------------*/
void EMM::readXML(TiXmlElement* xmlCurrentNode)
{
	string str = "";
	xmlCurrentNode->QueryStringAttribute("version", &version);
	xmlCurrentNode->QueryUnsignedAttribute("unk_0", &unknow_0);
	xmlCurrentNode->QueryUnsignedAttribute("unk_1", &unknow_1);
	xmlCurrentNode->QueryUnsignedAttribute("unk_2", &unknow_2);


	for (TiXmlElement* xmlNode = xmlCurrentNode->FirstChildElement("EMMMaterial"); xmlNode; xmlNode = xmlNode->NextSiblingElement("EMMMaterial"))
	{
		EMMMaterial* material = new EMMMaterial();
		material->readXML(xmlNode);
		materials.push_back(material);
	}



	TiXmlElement* defaultValuesNode = xmlCurrentNode->FirstChildElement("UnknowValues");
	listUnknowValues.clear();

	if (defaultValuesNode)
	{
		string name = "";
		size_t flags = 0;
		for (TiXmlElement* xmlNode = defaultValuesNode->FirstChildElement("Value"); xmlNode; xmlNode = xmlNode->NextSiblingElement("Value"))
		{
			xmlNode->QueryUnsignedAttribute("value", &flags);
			listUnknowValues.push_back(flags);
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             readXML				                             |
\-------------------------------------------------------------------------------*/
void EMMMaterial::readXML(TiXmlElement* xmlCurrentNode)
{
	xmlCurrentNode->QueryStringAttribute("name", &name);
	xmlCurrentNode->QueryStringAttribute("shaderProgram", &shaderProgramName);
	uint32_t tmp = 0;
	xmlCurrentNode->QueryUnsignedAttribute("unk_0", &tmp);
	unknow_0 = (uint16_t)tmp;


	for (TiXmlElement* xmlNode = xmlCurrentNode->FirstChildElement("EMMParameter"); xmlNode; xmlNode = xmlNode->NextSiblingElement("EMMParameter"))
	{
		EMMParameter* parameter = new EMMParameter();
		parameter->readXML(xmlNode);
		parameters.push_back(parameter);
	}
}
/*-------------------------------------------------------------------------------\
|                             readXML				                             |
\-------------------------------------------------------------------------------*/
void EMMParameter::readXML(TiXmlElement *root)
{
	root->QueryStringAttribute("name", &name);

	string str = "";
	root->QueryStringAttribute("level", &str);
	level = ((str == "shader") ? 0 : ((str == "pass") ? 1 : (uint16_t)std::stoi(str) ));


	string str_tmp = "";
	root->QueryStringAttribute("type", &str_tmp);

	if (str_tmp == "Float")
	{
		type = 0x0;
		root->QueryStringAttribute("value", &str_tmp);
		float_value = StringToFloat(str_tmp);

	}else if (str_tmp == "UInt") {
		type = 0x1;
		root->QueryUnsignedAttribute("value", &uint_value);
	
	}else if (str_tmp.substr(0, 9) == "unk_0x") {

		type = parseHexaUnsignedInt(str_tmp.substr(9));

		root->QueryStringAttribute("value", &str_tmp);
		uint_value = parseHexaUnsignedInt((str_tmp.substr(0, 2) == "0x") ? str_tmp : str_tmp.substr(2));

	}else {
		printf("Unknown Parameter Flag %s\n", str_tmp);
		notifyError();
	}
}




/*-------------------------------------------------------------------------------\
|                             saveXML				                             |
\-------------------------------------------------------------------------------*/
void EMM::saveXML(string filename)
{
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement* rootNode = writeXML();

	doc.LinkEndChild(rootNode);

	doc.SaveFile(filename);
}
/*-------------------------------------------------------------------------------\
|                             writeXML				                             |
\-------------------------------------------------------------------------------*/
TiXmlElement*  EMM::writeXML()
{
	TiXmlElement* rootNode = new TiXmlElement("EMM");
	rootNode->SetAttribute("version", version);
	rootNode->SetAttribute("unk_0", unknow_0);
	rootNode->SetAttribute("unk_1", unknow_1);
	rootNode->SetAttribute("unk_2", unknow_2);

	rootNode->LinkEndChild(new TiXmlComment("For all files of Dbxv2:\n\tEMM unk_0 to 2 are always 0.\n\tEMMMaterial unk_0 is 0 or 65535 ('FF FF').\n\tversion 0.0.0.0 always have 0 and no unknowValues (but not inverses).\n\tWe didn't find what is the 68 unknowValues are for.\n\t"));

	size_t nbMaterial = materials.size();
	for (size_t i = 0; i < nbMaterial; i++)
	{
		rootNode->LinkEndChild(new TiXmlComment(("index:" + std::to_string(i)).c_str()));
		materials.at(i)->writeXML(rootNode);
	}



	if (listUnknowValues.size())
	{
		size_t nbParams = listUnknowValues.size();

		TiXmlElement* defaultValuesNode = new TiXmlElement("UnknowValues");

		TiXmlElement* xmlNode;

		for (size_t i = 0; i < nbParams; i++)
		{
			xmlNode = new TiXmlElement("Value");
			xmlNode->SetAttribute("value", listUnknowValues.at(i));

			defaultValuesNode->LinkEndChild(xmlNode);
		}
		rootNode->LinkEndChild(defaultValuesNode);
	}

	return rootNode;
}
/*-------------------------------------------------------------------------------\
|                             writeXML				                             |
\-------------------------------------------------------------------------------*/
void EMMMaterial::writeXML(TiXmlElement* xmlCurrentNode)
{
	TiXmlElement* materialRoot = new TiXmlElement("EMMMaterial");
	materialRoot->SetAttribute("name", name);
	materialRoot->SetAttribute("shaderProgram", shaderProgramName);
	materialRoot->SetAttribute("unk_0", unknow_0);

	size_t nbParam = parameters.size();
	for (size_t i = 0; i < nbParam; i++)
		parameters.at(i)->writeXML(materialRoot);

	xmlCurrentNode->LinkEndChild(materialRoot);
}
/*-------------------------------------------------------------------------------\
|                             writeXML				                             |
\-------------------------------------------------------------------------------*/
void EMMParameter::writeXML(TiXmlElement* xmlCurrentNode)
{
	TiXmlElement* parameterRoot = new TiXmlElement("EMMParameter");


	parameterRoot->SetAttribute("name", name);
	parameterRoot->SetAttribute("level", ((level==0) ? "shader" : ((level == 1) ? "pass" : ToString(level) )) );

	if (type == 0x0)
	{
		parameterRoot->SetAttribute("type", "Float");
		parameterRoot->SetAttribute("value", FloatToString(float_value) );

	}else if (type == 0x1){
		parameterRoot->SetAttribute("type", "UInt");
		parameterRoot->SetAttribute("value", uint_value);

	}else {
		string tmp = "unk_0x" + toStringHexa(type);
		parameterRoot->SetAttribute("type", tmp);

		tmp = "0x" + toStringHexa(uint_value);
		parameterRoot->SetAttribute("value", tmp);
	}

	xmlCurrentNode->LinkEndChild(parameterRoot);
}










/*-------------------------------------------------------------------------------\
|                             getMaterial				                         |
\-------------------------------------------------------------------------------*/
EMMMaterial* EMM::getMaterial(string name)
{

	string name_tmp = name;
	std::transform(name_tmp.begin(), name_tmp.end(), name_tmp.begin(), tolower);

	string name_tmp_b = "";
	for (size_t i = 0, nbMaterial = materials.size(); i < nbMaterial; i++)
	{
		name_tmp_b = materials.at(i)->getName();
		std::transform(name_tmp_b.begin(), name_tmp_b.end(), name_tmp_b.begin(), tolower);

		if (name_tmp_b == name_tmp_b)
			return materials.at(i);
	}
	return nullptr;
}
/*-------------------------------------------------------------------------------\
|                             getListUniqueParameters				             |
\-------------------------------------------------------------------------------*/
std::vector<string>	EMM::getListUniqueParameters(void)
{
	std::vector<string> listUniqueParams;

	size_t nbMaterial = materials.size();
	for (size_t i = 0; i < nbMaterial; i++)
	{

		size_t nbParams = materials.at(i)->parameters.size();
		for (size_t j = 0; j < nbParams; j++)
		{
			string paramName = materials.at(i)->parameters.at(j)->name;

			bool isfound = false;
			size_t nbUniq = listUniqueParams.size();
			for (size_t k = 0; k < nbUniq; k++)
			{
				if (listUniqueParams.at(k) == paramName)
				{
					isfound = true;
					break;
				}
			}
			if (isfound)
				continue;

			listUniqueParams.push_back(paramName);
		}
	}

	return listUniqueParams;
}
/*-------------------------------------------------------------------------------\
|                             getParameter			                             |
\-------------------------------------------------------------------------------*/
EMMParameter* EMMMaterial::getParameter(const string &name)
{
	size_t nbParameters = parameters.size();
	for (size_t i = 0; i < nbParameters; i++)
		if (parameters.at(i)->name == name)
			return parameters.at(i);
	return NULL;
}








}