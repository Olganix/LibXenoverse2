

namespace LibXenoverse {


std::vector<string> SDSParameter::parameterTypeNames;


bool SDS::load(string filename)
{
	name = nameFromFilenameNoExtension(filename, true);

	if (filename.find(".xml") != string::npos)
	{
		TiXmlDocument doc(filename);
		if (!doc.LoadFile())
			return false;

		TiXmlHandle hDoc(&doc);
		TiXmlElement* pElem;
		TiXmlHandle hRoot(0);

		pElem = hDoc.FirstChildElement().Element();
		if (!pElem)
			return false;

		pElem = pElem->FirstChildElement();
		for (pElem; pElem; pElem = pElem->NextSiblingElement())
		{
			string element_name = pElem->ValueStr();

			if(element_name == "SDSShaderProgram")
			{
				SDSShaderProgram *shaderPrograms = new SDSShaderProgram();
				shaderPrograms->readXML(pElem);
				this->shaderPrograms.push_back(shaderPrograms);
			}
		}

	}else {
		File file(filename, LIBXENOVERSE_FILE_READ_BINARY);

		if (file.valid() && file.readHeader(LIBXENOVERSE_SDS_SIGNATURE))
		{
			read(&file);
			file.close();
		}else{
			return false;
		}
	}

	return true;
}


void SDS::save(string filename, bool big_endian)
{
	File file(filename, LIBXENOVERSE_FILE_WRITE_BINARY);
	if (file.valid())
	{
		file.writeHeader(LIBXENOVERSE_SDS_SIGNATURE, big_endian);
		write(&file);
		file.close();
	}
}




void SDS::read(File *file)
{
	file->goToAddress(0x6);

	unsigned int shaderPrograms_base_address = 0;
	file->readInt32E(&shaderPrograms_base_address);
	file->moveAddress(2);

	unsigned short shaderPrograms_count = 0;
	file->readInt16E(&shaderPrograms_count);

	printf("Found %d shaderPrograms.\n", shaderPrograms_count);
	
	file->goToAddress(shaderPrograms_base_address);
	shaderPrograms.resize(shaderPrograms_count);

	for (size_t i = 0; i < shaderPrograms_count; i++)
	{
		file->goToAddress(shaderPrograms_base_address + i * 4);

		unsigned int shaderPrograms_address = 0;
		file->readInt32E(&shaderPrograms_address);
		file->goToAddress(shaderPrograms_address);

		shaderPrograms[i] = new SDSShaderProgram();
		shaderPrograms[i]->read(file);
	}
}




void SDS::write(File *file)
{
	// Header
	file->goToAddress(0x6);

	unsigned int shaderPrograms_base_address = 0x14;
	file->writeInt32E(&shaderPrograms_base_address);
	file->writeNull(2);

	unsigned short shaderPrograms_count = shaderPrograms.size();
	file->writeInt16E(&shaderPrograms_count);
	file->writeNull(2);

	unsigned int unknown_1 = 0x0014;
	file->writeInt32E(&unknown_1);

	file->writeNull(shaderPrograms_count * 4);
	for (size_t i = 0; i < shaderPrograms_count; i++)
	{
		unsigned int shaderPrograms_address = file->getCurrentAddress();
		file->goToAddress(shaderPrograms_base_address + i * 4);
		file->writeInt32E(&shaderPrograms_address);
		file->goToAddress(shaderPrograms_address);
		shaderPrograms[i]->write(file);
	}
}



void SDS::saveXML(string filename)
{
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("SDS");
	for (size_t i = 0; i < shaderPrograms.size(); i++)
		shaderPrograms[i]->writeXML(root);

	doc.LinkEndChild(root);

	doc.SaveFile(filename);
}




////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////




void SDSShaderProgram::read(File *file)
{
	size_t offset = file->getCurrentAddress();
	

	char str_tmp[255];
	file->read(str_tmp, 32);
	
	std::string str = str_tmp;
	size_t firstSplit = str.find(' ');
	if (firstSplit != string::npos)
		str = str.substr(0, firstSplit);
	firstSplit = str.find('\t');
	if (firstSplit != string::npos)
		str = str.substr(0, firstSplit);
	offset += str.size() + 1;						//one char by letter
	file->goToAddress(offset);

	name = str;


	
	file->read(str_tmp, 32);
	str = str_tmp;
	firstSplit = str.find(' ');
	if (firstSplit != string::npos)
		str = str.substr(0, firstSplit);
	firstSplit = str.find('\t');
	if (firstSplit != string::npos)
		str = str.substr(0, firstSplit);
	offset += str.size() + 1;						//one char by letter
	file->goToAddress(offset);

	vertexShader_name = str;


	
	file->read(str_tmp, 32);
	str = str_tmp;
	firstSplit = str.find(' ');
	if (firstSplit != string::npos)
		str = str.substr(0, firstSplit);
	firstSplit = str.find('\t');
	if (firstSplit != string::npos)
		str = str.substr(0, firstSplit);
	offset += str.size() + 1;						//one char by letter
	file->goToAddress(offset);

	pixelShader_name = str;


	//try to have the string for all params , while '\0' character
	string allParams = "";
	file->read(str_tmp, 255);
	str = str_tmp;
	allParams += str;
	while (str.size()==255)
	{
		file->read(str_tmp, 255);
		str = str_tmp;
		allParams += str;
	}
	offset += str.size() + 1;						//one char by letter
	file->goToAddress(offset);

	

	std::vector<string> listType = File::split(allParams, "\t/");

	string type = "";
	std::vector<string> listArguments;
	size_t nbArg = 0;

	size_t nbType = listType.size();
	for (size_t i = 0; i < nbType; i++)
	{
		str = listType.at(i);
		if (str == "")
			continue;

		listArguments = File::split(str, " ");

		nbArg = listArguments.size();
		if (nbArg <= 1)
			continue;

		type = listArguments.at(0);
		for (size_t j = 1; j < nbArg; j++)
			parameters.push_back(new SDSParameter(listArguments.at(j), type));
	}
}


void SDSShaderProgram::write(File *file)
{
	file->write((void*)(name +" ").c_str(), name.size() + 1);
	file->write((void*)(vertexShader_name + " ").c_str(), vertexShader_name.size() + 1 );
	file->write((void*)(pixelShader_name).c_str(), pixelShader_name.size());

	
	string typeName = "";
	bool firstForType = true;

	size_t parameter_count = parameters.size();
	for (size_t j = 0; j < SDSParameter::parameterType::max; j++)
	{
		typeName = SDSParameter::parameterTypeNames.at(j);
		firstForType = true;

		for (size_t i = 0; i < parameter_count; i++)
		{
			if (parameters.at(i)->type != typeName)
				continue;

			if (firstForType)
				file->write((void*)("\t/" + typeName).c_str(), typeName.size() + 2);
			firstForType = false;

			file->write((void*)(" " + parameters.at(i)->name).c_str(), parameters.at(i)->name.size() + 1);
		}
	}
	file->writeNull(1);
}




void SDSShaderProgram::readXML(TiXmlElement *root)
{
	string temp_str = "";
	
	root->QueryStringAttribute("name", &name);
	root->QueryStringAttribute("vertexShader_name", &vertexShader_name);
	root->QueryStringAttribute("pixelShader_name", &pixelShader_name);

	TiXmlElement *pElem = root->FirstChildElement();
	for (pElem; pElem; pElem = pElem->NextSiblingElement())
	{
		string element_name = pElem->ValueStr();

		if (element_name == "SDSParameter")
		{
			SDSParameter *parameter = new SDSParameter();
			parameter->readXML(pElem);
			parameters.push_back(parameter);
		}
	}
}




void SDSShaderProgram::writeXML(TiXmlElement *root)
{
	TiXmlElement* shaderProgramsRoot = new TiXmlElement("SDSShaderProgram");
	shaderProgramsRoot->SetAttribute("name", name);
	shaderProgramsRoot->SetAttribute("vertexShader_name", vertexShader_name);
	shaderProgramsRoot->SetAttribute("pixelShader_name", pixelShader_name);

	
	for (size_t i = 0; i < parameters.size(); i++)
		parameters[i]->writeXML(shaderProgramsRoot);

	root->LinkEndChild(shaderProgramsRoot);
}



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////



void SDSParameter::readXML(TiXmlElement *root)
{
	root->QueryStringAttribute("name", &name);
	root->QueryStringAttribute("type", &type);

	bool isfound = false;
	for (size_t i = 0; i < parameterType::max; i++)
	{
		if (type == parameterTypeNames.at(i))
		{
			isfound = true;
			break;
		}
	}
	
	if (!isfound)
	{
		printf("Warning : Unknown Parameter type %x\n", type);
		LibXenoverse::notifyWarning();
	}
}

void SDSParameter::writeXML(TiXmlElement *root)
{
	TiXmlElement* parameterRoot = new TiXmlElement("SDSParameter");
	parameterRoot->SetAttribute("name", name);
	parameterRoot->SetAttribute("type", type);

	root->LinkEndChild(parameterRoot);
}







}