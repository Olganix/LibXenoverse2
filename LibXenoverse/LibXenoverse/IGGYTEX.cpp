namespace LibXenoverse
{

bool IGGYTEX::load(string filename)
{
	name = nameFromFilenameNoExtension(filename, true);

	File file(filename, LIBXENOVERSE_FILE_READ_BINARY);
	if (file.valid() && file.readHeader(LIBXENOVERSE_IGGYTEX_SIGNATURE))
	{
		file.goToAddress(0x08);

		unsigned int file_count = 0;
		unsigned int data_table_address = 0;

		file.readInt32E(&file_count);
		file.readInt32E(&data_table_address);

		printf("Found %d files\n", file_count);

		for (size_t i = 0; i < file_count; i++)
		{
			unsigned short data_id = 0;
			unsigned short unknow_id = 0;
			unsigned int data_address = 0;
			unsigned int data_size = 0;
			unsigned int data_base_offset_entry = data_table_address + i * 16;

			file.goToAddress(data_base_offset_entry);

			file.readInt16E(&data_id);
			file.readInt16E(&unknow_id);
			file.readInt32E(&data_size);
			file.readInt32E(&data_address);
			

			char *data = (char *) malloc(data_size);
			file.goToAddress(data_address);
			file.read(data, data_size);

			IGGYTEXFile *file_entry = new IGGYTEXFile(data_id, unknow_id, data, data_size);
			file_entry->setIndex(i);
			files.push_back(file_entry);

			printf("File Entry %d with id: %d size %d, unknow_id:%d\n", i, data_id, data_size, unknow_id);
		}
		file.close();

	}else{
		return false;
	}

	return true;
}



void IGGYTEX::save(string filename, bool enable_filenames, bool big_endian)
{
	File file(filename, LIBXENOVERSE_FILE_WRITE_BINARY);
	if (file.valid())
	{
		//file.writeHeader(LIBXENOVERSE_IGGYTEX_SIGNATURE, big_endian);		//note a classique Flags (no enddian defined)
		string signature = LIBXENOVERSE_IGGYTEX_SIGNATURE;
		
		char signature_a[8];
		memset(signature_a, 0, 8);
		strcpy(signature_a, signature.c_str());


		file.write(signature_a, signature.size());
		file.writeNull(1);

		// Header
		unsigned int file_total = files.size();
		unsigned int data_table_address = 0x0010;

		file.goToAddress(0x08);
		file.writeInt32E(&file_total);
		file.writeInt32E(&data_table_address);
		
		file.goToAddress(data_table_address);


		// Main Data
		file.writeNull(file_total * 16);
		

		for (size_t i = 0; i < file_total; i++)
		{
			file.fixPadding(0x80);		//try to begin always at a adresse multiple of x80
			unsigned int file_data_address = file.getCurrentAddress();
			files[i]->write(&file);

			unsigned short file_data_id = files[i]->getId();
			unsigned short unknow_id = files[i]->getUnknowId();
			unsigned int file_data_size = files[i]->getSize();
			if (file_data_size == 0)
				file_data_address = 0;
			
			file.goToAddress(data_table_address + i * 16);
			file.writeInt16E(&file_data_id);
			file.writeInt16E(&unknow_id);
			file.writeInt32E(&file_data_size);
			file.writeInt32E(&file_data_address);
			file.writeNull(4);

			file.goToEnd();
		}
		file.close();
	}
}


void IGGYTEX::extract(string folder)
{
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement* rootNode = new TiXmlElement("Iggytex");
	doc.LinkEndChild(rootNode);

	for (size_t i = 0; i < files.size(); i++)
	{
		TiXmlElement* node = new TiXmlElement("File");
		node->SetAttribute("id", files[i]->getId());
		node->SetAttribute("unk_id", files[i]->getUnknowId());
		rootNode->LinkEndChild(node);

		string filename = files[i]->getName();

		char suffix[] = "000";
		sprintf(suffix, "%03d", i);

		if (!filename.size())
			filename = "DATA" + ToString(suffix) + ".dds";

		if(files[i]->getSize()!=0)
			files[i]->save(folder + filename);
	}
	doc.SaveFile(folder + "iggytexFiles.xml");
}


void IGGYTEX::addFile(string filename)
{
	if (fileCheck(filename))
	{
		printf("add filename : %s\n", filename.c_str());
		IGGYTEXFile *iggytext_file = new IGGYTEXFile(filename);
		iggytext_file->setIndex(files.size());
		files.push_back(iggytext_file);
	}
}

void IGGYTEX::addFolder(string folder)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile((folder + "*.*").c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
		return;

	do
	{
		const char *name = FindFileData.cFileName;
		if (name[0] == '.')
			continue;

		if (string(name) == "iggytexFiles.xml")
			continue;

		string new_filename = folder + ToString(name);
		addFile(new_filename);
	}while (FindNextFile(hFind, &FindFileData) != 0);

	FindClose(hFind);


	TiXmlDocument doc(folder + "iggytexFiles.xml");
	if (!doc.LoadFile())
		return;

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);
	TiXmlElement* rootNode = hDoc.FirstChildElement("Iggytex").Element();
	if (!rootNode)
		return;

	size_t id = 0;
	size_t unk_id = 65535;
	size_t nbFiles = files.size();
	for (TiXmlElement* xmlNode = rootNode->FirstChildElement("File"); xmlNode; xmlNode = xmlNode->NextSiblingElement("File"))
	{
		xmlNode->QueryUnsignedAttribute("id", &id);
		xmlNode->QueryUnsignedAttribute("unk_id", &unk_id);
		
		bool isfound = false;
		for (size_t i = 0; i < nbFiles; i++)
		{
			if (files.at(i)->getId() == id)
			{
				isfound = true;
				files.at(i)->setUnknowId(unk_id);
				break;
			}
		}
		if (!isfound)
			files.push_back(new IGGYTEXFile(id, unk_id, 0, 0));
	}

	std::sort(files.begin(), files.end(), &IGGYTEXFile::idOrder);				//order to keep informations.
}



bool IGGYTEX::detectFilenameMode()
{
	bool only_data_dds = true;
	for (size_t i = 0; i < files.size(); i++)
	{
		string name = files[i]->getName();
		size_t data_found = name.find("DATA");
		size_t dds_found = name.find(".dds");

		if ((data_found == string::npos) || (dds_found == string::npos))
		{
			only_data_dds = false;
			break;
		}
	}
	return !only_data_dds;
}

}