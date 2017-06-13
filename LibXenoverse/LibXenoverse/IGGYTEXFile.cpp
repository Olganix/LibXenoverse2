namespace LibXenoverse
{

IGGYTEXFile::IGGYTEXFile(string filename)
{
	File file(filename, LIBXENOVERSE_FILE_READ_BINARY);
	if (file.valid())
	{
		name = nameFromFilename(filename);

		//try to search the Id of image in name.
		//data_id

		size_t data_found = name.find("DATA");
		size_t dds_found = name.find(".dds");
		if ((data_found != string::npos) && (dds_found != string::npos) && (data_found + 4 < dds_found))
		{
			try
			{
				data_id = atoi(name.substr(data_found + 4, dds_found - (data_found + 4)).c_str());
			}
			catch (...)
			{

			}
		}


		// Read entire file into data buffer
		data_size = file.getFileSize();
		data = (char *)malloc(data_size);
		file.read(data, data_size);

		file.close();
	}
}

void IGGYTEXFile::write(File *file)
{
	file->write(data, data_size);
}

void IGGYTEXFile::save(string filename)
{
	if (!data)
		return;

	File file(filename, LIBXENOVERSE_FILE_WRITE_BINARY);
	if (file.valid())
	{
		file.write(data, data_size);
		file.close();
	}
}

}