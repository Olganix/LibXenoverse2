namespace LibXenoverse {

	bool EMB::load(string filename) {
		name = nameFromFilenameNoExtension(filename, true);

		File file(filename, LIBXENOVERSE_FILE_READ_BINARY);
		if (file.valid() && file.readHeader(LIBXENOVERSE_EMB_SIGNATURE)) {
			file.goToAddress(0xC);

			unsigned int file_count = 0;
			unsigned int data_table_address = 0;
			unsigned int filename_table_address = 0;

			file.readInt32E(&file_count);

			file.goToAddress(0x18);
			file.readInt32E(&data_table_address);
			file.readInt32E(&filename_table_address);

			printf("Found %d files\n", file_count);

			for (size_t i = 0; i < file_count; i++)
			{
				unsigned int data_address = 0;
				unsigned int data_size = 0;
				unsigned int data_base_offset_entry = data_table_address + i * 8;

				file.goToAddress(data_base_offset_entry);

				file.readInt32E(&data_address);
				file.readInt32E(&data_size);

				char *data = (char *) malloc(data_size);
				file.goToAddress(data_base_offset_entry + data_address);
				file.read(data, data_size);

				EMBFile *file_entry = new EMBFile(data, data_size);
				file_entry->setIndex(i);
				files.push_back(file_entry);

				printf("File Entry %d with size %d\n", i, data_size);
			}

			if (filename_table_address) {
				for (size_t i = 0; i < file_count; i++) {
					file.goToAddress(filename_table_address + i*4);
					unsigned int string_address = 0;
					file.readInt32E(&string_address);
					file.goToAddress(string_address);

					string filename_entry = "";
					file.readString(&filename_entry);

					printf("File Entry %d with name %s\n", i, filename_entry.c_str());

					if (files.size() > i) {
						files[i]->setName(filename_entry);
					}
				}
			}

			file.close();
		}
		else return false;

		return true;
	}

	void EMB::extract(string folder)
	{
		//add a Xml to keep files orders
		TiXmlDocument *doc = new TiXmlDocument();
		TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
		doc->LinkEndChild(decl);

		TiXmlElement* rootNode = new TiXmlElement("EMB");

		std::vector<string> listFilenameAllreadyCreated;
		std::vector<size_t> listIndexToRenameDuplicate;

		TiXmlElement* xmlNode;
		for (size_t i = 0; i < files.size(); i++)
		{
			string name = files[i]->getName();

			if (!name.size())							//if no name, it's dds texture.
			{
				if (i < 1000)
				{
					char suffix[] = "000";
					sprintf(suffix, "%03d", i);
					name = "DATA" + ToString(suffix) + ".dds";

				}else if(i < 10000){
					char suffix[] = "0000";
					sprintf(suffix, "%04d", i);
					name = "DATA" + ToString(suffix) + ".dds";

				}else{
					char suffix[] = "00000";
					sprintf(suffix, "%05d", i);
					name = "DATA" + ToString(suffix) + ".dds";
				}
			}

				

			string filename = name;



			bool isfound = false;
			size_t nbfilenames = listFilenameAllreadyCreated.size();
			for (size_t j = 0; j<nbfilenames; j++)					//solve duplicate name, in pbind.emb
			{
				if (listFilenameAllreadyCreated.at(j)== filename)
				{
					listIndexToRenameDuplicate.at(j) += 1;

					string basename = LibXenoverse::nameFromFilenameNoExtension(filename, true);
					string extension = filename.substr(basename.length());

					filename = basename + "_" + std::to_string(listIndexToRenameDuplicate.at(j)) + extension;

					break;
				}
			}
			if (!isfound)
			{
				listFilenameAllreadyCreated.push_back(filename);
				listIndexToRenameDuplicate.push_back(0);
			}

			files[i]->save(folder + filename);


			xmlNode = new TiXmlElement("File");
			xmlNode->SetAttribute("name", name);
			xmlNode->SetAttribute("filename", filename);

			//comments 
			TiXmlComment* xmlComment = new TiXmlComment(("index:"+ std::to_string(i)).c_str());
			rootNode->LinkEndChild(xmlComment);

			rootNode->LinkEndChild(xmlNode);
		}

		doc->LinkEndChild(rootNode);

		doc->SaveFile(folder +"embFiles.xml");
		delete doc;
	}


	void EMB::addFile(string filename)
	{
		if (fileCheck(filename)) {
			EMBFile *emb_file = new EMBFile(filename);
			emb_file->setIndex(files.size());

			if (emb_file) {
				files.push_back(emb_file);
			}
		}
	}

	void EMB::addFolder(string folder)
	{
		string filename_orderedFilesXml = "";
		
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		hFind = FindFirstFile((folder + "*.*").c_str(), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
		
		}else{
			
			do
			{
				const char *name = FindFileData.cFileName;
				if (name[0] == '.')
					continue;

				string str = ToString(name);
				string new_filename = folder + str;
				if (str != "embFiles.xml")
					addFile(new_filename);
				else
					filename_orderedFilesXml = new_filename;				//there is a special file witch could give the order of files (when emb have filename, so Windows order is'nt good enougth)
			} while (FindNextFile(hFind, &FindFileData) != 0);
			FindClose(hFind);
		}

		if (filename_orderedFilesXml != "")
		{
			TiXmlDocument doc(filename_orderedFilesXml);
			if (doc.LoadFile())
			{
				TiXmlHandle hDoc(&doc);
				TiXmlHandle hRoot(0);

				TiXmlElement* rootNode = hDoc.FirstChildElement("EMB").Element();
				if (rootNode)
				{
					std::vector<string> listFilenames;
					std::vector<string> listnames;

					string str = "";
					string str2 = "";
					for (TiXmlElement* xmlNode = rootNode->FirstChildElement("File"); xmlNode; xmlNode = xmlNode->NextSiblingElement("File"))
					{
						if (xmlNode->QueryStringAttribute("name", &str) != TIXML_SUCCESS)
							continue;

						if (xmlNode->QueryStringAttribute("filename", &str2) != TIXML_SUCCESS)
							str2 = str;

						listnames.push_back(str);
						listFilenames.push_back(str2);
					}

					if (listFilenames.size())
					{
						vector<EMBFile*> newListFiles;

						string currentName = "";
						size_t nbFiles = listFilenames.size();
						size_t nbFiles_old = files.size();
						for (size_t i = 0; i < nbFiles; i++)
						{
							currentName = listFilenames.at(i);

							for (size_t j = 0; j < nbFiles_old; j++)					//try to match names
							{
								if (files.at(j)->getName() == currentName)
								{
									if (currentName != listnames.at(i))					//if name != filename
										files.at(j)->setName(listnames.at(i));			//just rename correctly
									
									newListFiles.push_back( files.at(j) );				//to order in a new list

									files.erase(files.begin() + j);
									nbFiles_old--;
								}
							}
						}

						for(size_t i=0;i<nbFiles_old;i++)							//new files in folder, witch is not referenced in the file, are add at the end.
							newListFiles.push_back(files.at(i));

						files = newListFiles;
					}
				}
			}
		}
	}


	void EMB::save(string filename, bool enable_filenames, bool big_endian)
	{
		File file(filename, LIBXENOVERSE_FILE_WRITE_BINARY);
		if (file.valid())
		{
			file.writeHeader(LIBXENOVERSE_EMB_SIGNATURE, big_endian);

			file.goToAddress(0x6);

			// Header
			unsigned short unknown_1 = 0x0020;
			unsigned int unknown_2 = 0x00010001;
			unsigned int file_total = files.size();
			file.writeInt16E(&unknown_1);
			file.writeInt32E(&unknown_2);
			file.writeInt32E(&file_total);
			file.writeNull(8);

			// Main Data
			file.writeNull(8);

			unsigned int data_table_address = file.getCurrentAddress();
			file.writeNull(file_total * 8);

			unsigned int string_table_address = 0;
			if (enable_filenames) {
				string_table_address = file.getCurrentAddress();
				file.writeNull(file_total * 4);
			}

			for (size_t i = 0; i < file_total; i++) {
				file.fixPadding(0x40);
				unsigned int file_data_address = file.getCurrentAddress() - i*8 - 32;
				files[i]->write(&file);
				
				file.goToAddress(data_table_address + i*8);
				file.writeInt32E(&file_data_address);

				unsigned int file_data_size = files[i]->getSize();
				file.writeInt32E(&file_data_size);

				file.goToEnd();
			}

			if (enable_filenames) {
				for (size_t i = 0; i < file_total; i++) {
					unsigned int string_address = file.getCurrentAddress();
					file.writeString(&files[i]->getName());
					file.goToAddress(string_table_address + i * 4);
					file.writeInt32E(&string_address);
					file.goToEnd();
				}
			}

			file.goToAddress(0x18);
			file.writeInt32E(&data_table_address);
			file.writeInt32E(&string_table_address);

			file.close();
		}
	}

	bool EMB::detectFilenameMode() {
		bool only_data_dds = true;
		for (size_t i = 0; i < files.size(); i++) {
			string name = files[i]->getName();
			size_t data_found = name.find("DATA");
			size_t dds_found = name.find(".dds");

			if ((data_found == string::npos) || (dds_found == string::npos)) {
				only_data_dds = false;
				break;
			}
		}

		return !only_data_dds;
	}


	EMB* EMB::clone(void)
	{
		EMB* emb = new EMB();
		emb->name = name;

		size_t nbFile = files.size();
		for (size_t i = 0; i < nbFile; i++)
			emb->files.push_back(files.at(i)->clone());

		return emb;
	}
}