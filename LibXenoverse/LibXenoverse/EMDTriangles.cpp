namespace LibXenoverse
{




/*-------------------------------------------------------------------------------\
|                             EMDTriangles			                             |
\-------------------------------------------------------------------------------*/
EMDTriangles::EMDTriangles(EMDTriangles* emdTriangles)
{
	if (emdTriangles)
	{		
		for (size_t i = 0, nb = emdTriangles->faces.size(); i < nb; i++)
			faces.push_back(emdTriangles->faces.at(i));
		for (size_t i = 0, nb = emdTriangles->bone_names.size(); i < nb; i++)
			bone_names.push_back(emdTriangles->bone_names.at(i));
	}
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void EMDTriangles::read(File *file)
{
	unsigned int base_face_address = file->getCurrentAddress();
	unsigned int face_count = 0;
	unsigned int face_name_count = 0;
	unsigned int face_table_address = 0;
	unsigned int face_name_table_address = 0;

	file->readInt32E(&face_count);
	file->readInt32E(&face_name_count);
	file->readInt32E(&face_table_address);
	file->readInt32E(&face_name_table_address);

	// Fail-safe in case it's 0, which it is in some files
	if (!face_table_address)
		face_table_address = 16;

	LOG_DEBUG("Reading Triangle List at %d\n", base_face_address);
	LOG_DEBUG("Triangle List Face Count: %d\n", face_count);
	LOG_DEBUG("Triangle List Bone Count: %d\n", face_name_count);
	LOG_DEBUG("Reading faces at %d\n", base_face_address + face_table_address);

	// Read Face Indices
	faces.resize(face_count);

	
	bool is32bits = face_count > 0xFFFF;					//Not it's a bug from theirs tools, because normally , it's should be the number of vertices witch must be check.

	for (size_t n = 0; n < face_count; n++)
	{
		if (!is32bits)
		{
			file->goToAddress(base_face_address + face_table_address + n * sizeof(uint16_t));
			unsigned short face = 0;
			file->readInt16E(&face);
			faces[n] = (unsigned int)face;

		}else {
			file->goToAddress(base_face_address + face_table_address + n * sizeof(uint32_t));
			unsigned int face = 0;
			file->readInt32E(&face);
			faces[n] = face;
		}
	}

	// Recalculate the value of face name table address because some files literally lie about the value for some reason => maybe uint16 vs uint32 ? Todo check
	face_name_table_address = face_table_address + face_count * (is32bits ? sizeof(uint32_t) : sizeof(uint16_t)) ;
	if ((face_name_table_address % 4) == 2)
		face_name_table_address += 2;

	// Read Face Names
	for (size_t n = 0; n < face_name_count; n++)
	{
		unsigned int address = 0;
		LOG_DEBUG("Reading bone name address for triangle list at %d\n", base_face_address + face_name_table_address + n * 4);
		file->goToAddress(base_face_address + face_name_table_address + n * 4);
		file->readInt32E(&address);
		file->goToAddress(base_face_address + address);

		string face_name = "";
		file->readString(&face_name);
		bone_names.push_back(face_name);

		LOG_DEBUG("Bone name %d for Triangle List: %s\n", n, face_name.c_str());
	}
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void EMDTriangles::write(File *file, size_t numberVertex)
{
	unsigned int base_face_address = file->getCurrentAddress();
	file->writeNull(16);

	// Write Indices
	unsigned int face_table_address = file->getCurrentAddress() - base_face_address;
	
	bool is32bits = faces.size() > 0xFFFF;
	

	for (size_t i = 0; i < faces.size(); i++)
	{
		if (!is32bits)
		{
			unsigned short face = faces[i];
			file->writeInt16E(&face);
		}else {
			file->writeInt32E(&faces[i]);
		}
	}

	file->fixPadding(0x4);

	// Write Names
	unsigned int face_name_table_address = ((bone_names.size()!=0) ? (file->getCurrentAddress() - base_face_address) : 0);
	file->writeNull(bone_names.size() * 4);

	for (size_t i = 0; i < bone_names.size(); i++)
	{
		unsigned int name_address = file->getCurrentAddress() - base_face_address;
		file->goToAddress(base_face_address + face_name_table_address + i * 4);
		file->writeInt32E(&name_address);
		file->goToAddress(base_face_address + name_address);
		file->writeString(&bone_names[i]);
	}
	file->fixPadding(0x4);

	// Write Face Header
	file->goToAddress(base_face_address);
	unsigned int face_count = faces.size();
	unsigned int face_name_count = bone_names.size();
	file->writeInt32E(&face_count);
	file->writeInt32E(&face_name_count);
	file->writeInt32E(&face_table_address);
	file->writeInt32E(&face_name_table_address);

	file->goToEnd();
}
/*-------------------------------------------------------------------------------\
|                             getBonesNames			                             |
\-------------------------------------------------------------------------------*/
void EMDTriangles::getBonesNames(vector<string> &bones_names)
{
	for (size_t j = 0; j < bone_names.size(); j++)
	{
		string name = bone_names.at(j);
			
		bool found = false;
		for (size_t i = 0; i < bones_names.size(); i++)
		{
			if (bones_names.at(i) == name)
			{
				found = true;
				break;
			}
		}

		if (!found)
			bones_names.push_back(name);
	}
}
/*-------------------------------------------------------------------------------\
|                             replaceBonesNames		                             |
\-------------------------------------------------------------------------------*/
void EMDTriangles::replaceBonesNames(const string &oldName, const string &newName)
{
	for (size_t j = 0; j < bone_names.size(); j++)
		if (bone_names.at(j) == oldName)
			bone_names.at(j) = newName;
}


}