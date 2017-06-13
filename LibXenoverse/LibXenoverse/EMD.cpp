namespace LibXenoverse
{




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
		save(&file);
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
	
	unsigned short model_total = 0;

	file->goToAddress(startAdress + 0x10);
	file->readInt16E(&unknown_total);
	file->readInt16E(&model_total);

	LOG_DEBUG("Total Unknown: %d\n", unknown_total);
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
	
	unsigned short unknown_1 = 0x001C;
	unsigned int unknown_2 = 0x00010001;
	unsigned short model_total = models.size();
	file->goToAddress(startAdress + 6);
	file->writeInt16E(&unknown_1);
	file->writeInt32E(&unknown_2);
	file->writeNull(4);
	file->writeInt16E(&unknown_total);
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


}