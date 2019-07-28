
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
/*-------------------------------------------------------------------------------\
|                             EMMParameter			                             |
\-------------------------------------------------------------------------------*/
EMMParameter::EMMParameter(void)
{
	name = "";
	type = 1;
	int_value = 0;
	uint_value = 0;
	bool_value = false; 
	float_value = 0.0f;
}


/*-------------------------------------------------------------------------------\
|                             load					                             |
\-------------------------------------------------------------------------------*/
bool EMM::load(string filename)
{
	name = nameFromFilenameNoExtension(filename, true);

	if (filename.find(".xml") != string::npos)
	{
		TiXmlDocument doc(filename);
		if (!doc.LoadFile())
			return false;

		TiXmlHandle hDoc(&doc);
		TiXmlHandle hRoot(0);

		TiXmlElement* rootNode = hDoc.FirstChildElement("EMM").Element();
		if (!rootNode)
		{
			printf("%s don't have 'EMM' tags. skip.'\n", filename);
			getchar();
			return false;
		}

		for (TiXmlElement* xmlNode = rootNode->FirstChildElement("EMMMaterial"); xmlNode; xmlNode = xmlNode->NextSiblingElement("EMMMaterial"))
		{
			EMMMaterial* material = new EMMMaterial();
			material->readXML(xmlNode);
			materials.push_back(material);
		}


		
		TiXmlElement* defaultValuesNode = rootNode->FirstChildElement("UnknowValues");
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

	}else {
		File file(filename, LIBXENOVERSE_FILE_READ_BINARY);

		if (file.valid() && file.readHeader(LIBXENOVERSE_EMM_SIGNATURE))
		{
			read(&file);
			file.close();
		}else {
			return false;
		}
	}

	return true;
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
		write(&file);
		file.close();
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

	TiXmlElement* rootNode = new TiXmlElement("EMM");

	size_t nbMaterial = materials.size();
	for (size_t i = 0; i < nbMaterial; i++)
	{
		rootNode->LinkEndChild(new TiXmlComment(("index:"+ std::to_string(i)).c_str() ));

		materials.at(i)->writeXML(rootNode);
	}



	if (listUnknowValues.size())
	{
		TiXmlElement* defaultValuesNode = new TiXmlElement("UnknowValues");

		TiXmlElement* xmlNode;
		size_t nbParams = listUnknowValues.size();
		for (size_t i = 0; i < nbParams; i++)
		{
			xmlNode = new TiXmlElement("Value");
			xmlNode->SetAttribute("value", listUnknowValues.at(i));

			defaultValuesNode->LinkEndChild(xmlNode);
		}
		rootNode->LinkEndChild(defaultValuesNode);
	}


	doc.LinkEndChild(rootNode);

	doc.SaveFile(filename);
}





/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void EMM::read(File *file)
{
	file->goToAddress(0xC);

	unsigned int material_base_address = 0;
	file->readInt32E(&material_base_address);

	unsigned short material_count = 0;
	file->goToAddress(material_base_address);
	file->readInt16E(&material_count);
	file->moveAddress(2);

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
	file->goToAddress(0x10);
	unsigned int defaultValues_address = 0;
	file->readInt32E(&defaultValues_address);
	listUnknowValues.clear();

	if (defaultValues_address != 0)							//not all emm have this. I don't know why.
	{
		file->goToEnd();
		size_t endAdress = file->getCurrentAddress();

		file->goToAddress(defaultValues_address);

		unsigned char tmp = '\0';
		size_t nbParams = endAdress - defaultValues_address;
		for (size_t i = 0; i < nbParams; i++)
		{
			file->readUChar(&tmp);
			listUnknowValues.push_back((size_t)tmp);
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
	file->moveAddress(2);

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

	file->readInt32E(&type);
	

	if (type == 0x0)						//Float
	{
		file->readFloat32E(&float_value);

	}else if (type == 0x1) {				//Bool

		size_t tmp = 0;
		file->readInt32E(&tmp);
		bool_value = (tmp != 0);

	}else if (type == 0x10000) {			//Unknow, is for MipMapLod0, MipMapLod1, MipMapLod2

		file->readInt32E(&uint_value);
	
	}else if (type == 0x10001) {			//UInt

		file->readInt32E(&uint_value);

	}else {
		printf("Unknown Parameter Flag %x\n", type);

		file->readInt32E(&uint_value);
	}
}





/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void EMM::write(File *file)
{
	// Header
	unsigned short unknown_1 = 0x0010;
	file->writeInt16E(&unknown_1);
	file->writeNull(2);

	unsigned int material_base_address = 0x20;
	file->writeInt32E(&material_base_address);
	file->writeNull(16);

	unsigned short material_count = materials.size();
	file->writeInt16E(&material_count);
	file->writeNull(2);

	file->writeNull(material_count * 4);
	for (size_t i = 0; i < material_count; i++)
	{
		unsigned int material_address = file->getCurrentAddress() - material_base_address;
		file->goToAddress(material_base_address + i * 4 + 4);

		file->writeInt32E(&material_address);

		file->goToAddress(material_address + material_base_address);
		materials.at(i)->write(file);
	}


	//writing about (may be) default values of each parameter ( (may be) for other material in emm). all I remark there is the same number of octets at 0  than unique parameters.
	if (listUnknowValues.size() != 0)
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
	unsigned short unknownMarker = 0xFFFF;
	file->writeInt16E(&unknownMarker);

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
	
	file->writeInt32E(&type);


	if (type == 0x0)						//Float
	{
		file->writeFloat32E(&float_value);

	}else if (type == 0x1) {				//Bool

		size_t tmp = (bool_value) ? 1 : 0;
		file->writeInt32E(&tmp);

	}else if (type == 0x10000) {			//Unknow, is for MipMapLod0, MipMapLod1, MipMapLod2

		file->writeInt32E(&uint_value);

	}else if (type == 0x10001) {			//UInt

		file->writeInt32E(&uint_value);

	}else {

		file->writeInt32E(&uint_value);
	}
}










/*-------------------------------------------------------------------------------\
|                             readXML				                             |
\-------------------------------------------------------------------------------*/
void EMMMaterial::readXML(TiXmlElement* xmlCurrentNode)
{
	xmlCurrentNode->QueryStringAttribute("name", &name);
	xmlCurrentNode->QueryStringAttribute("shaderProgram", &shaderProgramName);


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

	string str_tmp = "";
	root->QueryStringAttribute("type", &str_tmp);


	if (str_tmp == "Float")
	{
		type = 0x0;
		root->QueryFloatAttribute("value", &float_value);


	}else if (str_tmp == "Bool") {
		
		type = 0x1;
		root->QueryStringAttribute("value", &str_tmp);
		std::transform(str_tmp.begin(), str_tmp.end(), str_tmp.begin(), ::tolower);
		bool_value = ((str_tmp == "true") ? 1 : 0);


	}else if (str_tmp == "Unknow_0x10000"){

		type = 0x10000;

		root->QueryStringAttribute("value", &str_tmp);
		uint_value = parseHexaUnsignedInt((str_tmp.substr(0, 2) == "0x") ? str_tmp : str_tmp.substr(2));

	}else if (str_tmp == "UInt") {
		
		type = 0x10001;
		root->QueryUnsignedAttribute("value", &uint_value);
	

	}else if (str_tmp.substr(0,9) == "Unknow_0x") {

		type = parseHexaUnsignedInt(str_tmp.substr(9));

		root->QueryStringAttribute("value", &str_tmp);
		uint_value = parseHexaUnsignedInt( (str_tmp.substr(0,2)=="0x") ? str_tmp : str_tmp.substr(2));

	}else {
		printf("Unknown Parameter Flag %s\n", str_tmp);
		getchar();
	}
}





/*-------------------------------------------------------------------------------\
|                             writeXML				                             |
\-------------------------------------------------------------------------------*/
void EMMMaterial::writeXML(TiXmlElement* xmlCurrentNode)
{
	TiXmlElement* materialRoot = new TiXmlElement("EMMMaterial");
	materialRoot->SetAttribute("name", name);
	materialRoot->SetAttribute("shaderProgram", shaderProgramName);

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
	

	if (type == 0x0)
	{
		parameterRoot->SetAttribute("type", "Float");
		parameterRoot->SetDoubleAttribute("value", float_value);

	}else if (type == 0x1){
		parameterRoot->SetAttribute("type", "Bool");
		parameterRoot->SetAttribute("value", ((bool_value) ? "true" : "false"));

	}else if (type == 0x10000) {

		parameterRoot->SetAttribute("type", "Unknow_0x10000");

		string tmp = "0x" + toStringHexa(uint_value);
		parameterRoot->SetAttribute("value", tmp);
		
	}else if (type == 0x10001){
		parameterRoot->SetAttribute("type", "UInt");
		parameterRoot->SetAttribute("value", uint_value);

	}else {

		string tmp = "Unknow_0x" + toStringHexa(type);
		parameterRoot->SetAttribute("type", tmp);

		tmp = "0x" + toStringHexa(uint_value);
		parameterRoot->SetAttribute("value", tmp);
	}

	xmlCurrentNode->LinkEndChild(parameterRoot);
}















}