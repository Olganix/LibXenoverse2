namespace LibXenoverse
{



/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
EMD::EMD(EMD* emd)
{
	name = "";
	version = "";
	unknow_0 = 0;
	unknow_1 = 0;

	if (emd)
	{
		name = emd->name;
		version = emd->version;
		unknow_0 = emd->unknow_0;
		unknow_1 = emd->unknow_1;

		for (size_t i = 0, nb = emd->models.size(); i < nb; i++)
			models.push_back( new EMDModel(emd->models.at(i)));
	}
}
/*-------------------------------------------------------------------------------\
|                             ~EMD					                             |
\-------------------------------------------------------------------------------*/
EMD::~EMD(void)
{
	size_t nbModels = models.size();
	for (size_t i = 0; i < nbModels; i++)
		delete models.at(i);
	models.clear();
}
/*-------------------------------------------------------------------------------\
|                             load					                             |
\-------------------------------------------------------------------------------*/
bool EMD::load(string filename)
{
	name = nameFromFilenameNoExtension(filename, true);

	File file(filename, LIBXENOVERSE_FILE_READ_BINARY);
	bool ret = file.valid() && load(&file, name);
	file.close();

	return ret;
}
/*-------------------------------------------------------------------------------\
|                             load					                             |
\-------------------------------------------------------------------------------*/
bool EMD::load(File *file, std::string s_name)
{
	if (!file->valid())
		return false;

	name = s_name;


	size_t startAdress = file->getCurrentAddress();
	if (!file->readHeader(LIBXENOVERSE_EMD_SIGNATURE))
		return false;

	file->goToAddress(startAdress);
	read(file);

	return true;
}
/*-------------------------------------------------------------------------------\
|                             save					                             |
\-------------------------------------------------------------------------------*/
void EMD::save(string filename, bool big_endian)
{
	File file(filename, LIBXENOVERSE_FILE_WRITE_BINARY);
	if (file.valid())
	{
		save(&file, big_endian);
		file.close();
	}
}
/*-------------------------------------------------------------------------------\
|                             save					                             |
\-------------------------------------------------------------------------------*/
void EMD::save(File *file, bool big_endian)
{
	if (!file->valid())
		return;

	//signature
	size_t startAdress = file->getCurrentAddress();
	file->writeHeader(LIBXENOVERSE_EMD_SIGNATURE, big_endian);

	file->goToAddress(startAdress);
	write(file);
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void EMD::read(File *file)
{
	size_t startAdress = file->getCurrentAddress();
	file->goToAddress(startAdress + 0x06);
	
	//uint16_t header_size;
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


	file->readInt32E(&unknow_0);


	unsigned short model_total = 0;
	
	file->readInt16E(&unknow_1);
	file->readInt16E(&model_total);

	LOG_DEBUG("Total Unknown: %d\n", unknow_1);
	LOG_DEBUG("Total Models: %d\n", model_total);

	unsigned int root_model_address = 0;
	unsigned int root_names_address = 0;

	file->readInt32E(&root_model_address);
	file->readInt32E(&root_names_address);

	// Root Model Section
	unsigned int address = 0;
	for (size_t i = 0; i < model_total; i++)
	{
		file->goToAddress(startAdress + root_model_address + i * 4);
		file->readInt32E(&address);
		file->goToAddress(startAdress + address);

		EMDModel *emd_model = new EMDModel();
		emd_model->read(file);
		models.push_back(emd_model);
	}

	// Root Names Section
	for (size_t i = 0; i < model_total; i++)
	{
		file->goToAddress(startAdress + root_names_address + i * 4);
		file->readInt32E(&address);
		file->goToAddress(startAdress + address);

		string name = "";
		file->readString(&name);
		models.at(i)->name = name;

		printf("Found something with index %d and name %s\n", i, name.c_str());
	}
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void EMD::write(File *file)
{
	size_t startAdress = file->getCurrentAddress();
	file->goToAddress(startAdress + 6);

	//uint16_t header_size = 0x001C;
	file->writeInt16E(&header_size);
	
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

	file->writeInt32E(&unknow_0);
	file->writeInt16E(&unknow_1);

	unsigned short model_total = models.size();
	file->writeInt16E(&model_total);
	file->writeNull(8);

	// Model Table
	unsigned int model_table_address = file->getCurrentAddress();
	file->writeNull(model_total * 4);

	unsigned int address = 0;
	for (size_t i = 0; i < model_total; i++)
	{
		file->fixPadding(16);
		unsigned int model_address = file->getCurrentAddress();
		address = model_address - startAdress;

		// Fix Offset On Table
		file->goToAddress(model_table_address + i*4);
		file->writeInt32E(&address);

		// Write Model
		file->goToAddress(model_address);
		models.at(i)->write(file);
	}


	// Name Table
	unsigned int name_table_address = file->getCurrentAddress();
	file->writeNull(model_total * 4);

	for (size_t i = 0; i < model_total; i++)
	{
		unsigned int name_address = file->getCurrentAddress();
		address = name_address - startAdress;

		// Fix Offset On Table
		file->goToAddress(name_table_address + i * 4);
		file->writeInt32E(&address);

		// Write Name
		file->goToAddress(name_address);
		file->writeString(&models.at(i)->name);
	}

	file->goToAddress(startAdress + 0x14);
	address = model_table_address - startAdress;
	file->writeInt32E(&address);

	address = name_table_address - startAdress;
	file->writeInt32E(&address);
	
	file->goToEnd();
}
/*-------------------------------------------------------------------------------\
|                             setVertexScale		                             |
\-------------------------------------------------------------------------------*/
void EMD::setVertexScale(float scale)
{
	for (size_t i = 0; i < models.size(); i++)
		models[i]->setVertexScale(scale);
}
/*-------------------------------------------------------------------------------\
|                             getBonesNames							             |
\-------------------------------------------------------------------------------*/
vector<string> EMD::getBonesNames()
{
	vector<string> bones_names;
	for (size_t i = 0; i < models.size(); i++)
		models[i]->getBonesNames(bones_names);
	return bones_names;
}
/*-------------------------------------------------------------------------------\
|                             replaceBonesNames									 |
\-------------------------------------------------------------------------------*/
void EMD::replaceBonesNames(const string &oldName, const string &newName)
{
	for (size_t i = 0; i < models.size(); i++)
		models[i]->replaceBonesNames(oldName, newName);
}
/*-------------------------------------------------------------------------------\
|                             mergeTriangles									 |
\-------------------------------------------------------------------------------*/
void EMD::mergeTriangles()
{
	for (size_t i = 0; i < models.size(); i++)
		models[i]->mergeTriangles();
}


}