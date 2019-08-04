
#include "NSK.h"

namespace LibXenoverse 
{

/*-------------------------------------------------------------------------------\
|                             load					                              |
\-------------------------------------------------------------------------------*/
bool NSK::load(string filename)
{
	mName = nameFromFilenameNoExtension(filename, true);

	File file(filename, LIBXENOVERSE_FILE_READ_BINARY);
	if (file.valid() && file.readHeader(LIBXENOVERSE_NSK_SIGNATURE))
	{
		file.goToAddress(0);
		read(&file);
		file.close();
	}else {
		return false;
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             save					                              |
\-------------------------------------------------------------------------------*/
void NSK::save(string filename, bool big_endian)
{
	File file(filename, LIBXENOVERSE_FILE_WRITE_BINARY);
	if (file.valid())
	{
		write(&file);
		file.close();
	}
}
/*-------------------------------------------------------------------------------\
|                             extract				                              |
\-------------------------------------------------------------------------------*/
void NSK::extract(string folder, bool big_endian)
{
	if (mEsk)
		mEsk->save(folder +"/"+ mName +".esk", big_endian);

	if(mEmd)
		mEmd->save(folder + "/" + mName + ".emd", big_endian);
}


/*-------------------------------------------------------------------------------\
|                             loadXml											 |
\-------------------------------------------------------------------------------*/
bool NSK::loadXml(string filename)
{
	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
	{
		printf("Loading xml %s fail. skip.'\n", filename);
		getchar();
		return false;
	}

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);
	TiXmlElement* rootNode = hDoc.FirstChildElement("NSK").Element();
	if (!rootNode)
	{
		printf("%s don't have 'NSK' tags. skip.'\n", filename);
		getchar();
		return false;
	}

	TiXmlElement* eskNode = rootNode->FirstChildElement("ESK");
	if (eskNode)
	{
		mEsk = new ESK();
		if (!mEsk->importXml(eskNode))
		{
			delete mEsk;
			mEsk = 0;
		}
	}

	TiXmlElement* emdNode = rootNode->FirstChildElement("EMD");
	if (emdNode)
	{
		mEmd = new EMD();
		if (!mEmd->importXml(emdNode))
		{
			delete mEmd;
			mEmd = 0;
		}
	}
	
	return (mEmd!=0);
}
/*-------------------------------------------------------------------------------\
|                             saveXml											 |
\-------------------------------------------------------------------------------*/
void NSK::saveXml(string filename)
{
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement* rootNode = new TiXmlElement("NSK");

	if (mEsk)
		rootNode->LinkEndChild(mEsk->exportXml());
	if (mEmd)
		rootNode->LinkEndChild(mEmd->exportXml());

	doc.LinkEndChild(rootNode);

	doc.SaveFile(filename);
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void NSK::read(File *file)
{
	if (mEsk)
		delete mEsk;
	mEsk = 0;


	size_t startAdress = file->getCurrentAddress();
	
	try
	{
		mEsk = new ESK();
		if (!mEsk->load(file, mName))
		{
			delete mEsk;
			mEsk = 0;
		}
	}catch (exception &e) {
		LOG_DEBUG("load ESK part faild : %s\n", e.what());

		delete mEsk;
		mEsk = 0;
	}




	if (mEmd)
		delete mEmd;
	mEmd = 0;
	
	size_t startAddress_Emd = 0;
	file->goToAddress(startAdress + 0x14);					//the first adresse, witch is in 0x10, it's for Esk, but, it will be readed directly by the read from the begining of the file. by contrerary,  0x14 adress, it's for emd.
	file->readInt32E(&startAddress_Emd);
	file->goToAddress(startAdress + startAddress_Emd);

	try
	{
		mEmd = new EMD();
		if (!mEmd->load(file, mName))
		{
			delete mEmd;
			mEmd = 0;
		}
	}catch (exception &e) {
		LOG_DEBUG("load EMD part faild : %s\n", e.what());
		delete mEmd;
		mEmd = 0;
	}
}
/*-------------------------------------------------------------------------------\
|                             write					                              |
\-------------------------------------------------------------------------------*/
void NSK::write(File *file)
{
	size_t startAddress = file->getCurrentAddress();

	if (mEsk)
		mEsk->save(file);

	file->fixPadding(16);

	size_t startAddress_Emd = file->getCurrentAddress() - startAddress;
	if (mEmd)
		mEmd->save(file);

	//mark address for Emd
	file->goToAddress(startAddress + 0x14);
	file->writeInt32E(&startAddress_Emd);

	//use custom ids
	size_t id = 0x9283;
	file->goToAddress(startAddress + 0x8);
	file->writeInt32E(&id);
	file->goToAddress(startAddress + startAddress_Emd + 0x8);
	file->writeInt32E(&id);
}
/*-------------------------------------------------------------------------------\
|                             addEmdFile			                              |
\-------------------------------------------------------------------------------*/
void NSK::addEmdFile(const std::string filename)
{
	EMD* emd = new EMD();

	try
	{
		emd->load(filename);

		if (mEmd)
			delete mEmd;

		mEmd = emd;
	}
	catch (exception &e) {
		LOG_DEBUG("load ESK %s faild : %s\n", filename.c_str(), e.what());
		if (emd)
			delete emd;
	}
}
/*-------------------------------------------------------------------------------\
|                             addEskFile			                              |
\-------------------------------------------------------------------------------*/
void NSK::addEskFile(const std::string filename)
{
	ESK* esk = new ESK();

	try
	{
		esk->load(filename);
		
		if (mEsk)
			delete mEsk;

		mEsk = esk;
	}
	catch (exception &e) {
		LOG_DEBUG("load ESK %s faild : %s\n", filename.c_str(), e.what());
		if (esk)
			delete esk;
	}
}




/*-------------------------------------------------------------------------------\
|                             direct_extract				                     |
\-------------------------------------------------------------------------------*/
bool NSK::direct_extract(string filename)
{
	mName = nameFromFilenameNoExtension(filename, true);
	string folder = folderFromFilename(filename);

	File file(filename, LIBXENOVERSE_FILE_READ_BINARY);
	if ((!file.valid()) || (!file.readHeader(LIBXENOVERSE_NSK_SIGNATURE)))
		return false;

	size_t startAddress_Esk = 0;
	size_t startAddress_Emd = 0;

	file.goToAddress(startAddress_Esk + 0x14);					//the first adresse, witch is in 0x10, it's for Esk, but, it will be readed directly by the read from the begining of the file. by contrerary,  0x14 adress, it's for emd.
	file.readInt32E(&startAddress_Emd);



	File file_Esk(folder + "/" + mName + ".esk", LIBXENOVERSE_FILE_WRITE_BINARY);
	if (file_Esk.valid())
	{
		file.goToAddress(startAddress_Esk);

		size_t size_esk = startAddress_Emd - startAddress_Esk;
		char* buf = (char*)malloc(size_esk);
		file.read(buf, size_esk);

		buf[0x14] = buf[0x15] = buf[0x16] = buf[0x17] = 0;					//cancel offset_emd, now it's a single esk

		file_Esk.write(buf, size_esk);
		file_Esk.close();
		::free(buf);
	}
	File file_Emd(folder + "/" + mName + ".emd", LIBXENOVERSE_FILE_WRITE_BINARY);
	if (file_Emd.valid())
	{
		file.goToAddress(startAddress_Emd);

		size_t size_emd = file.getFileSize() - startAddress_Emd;
		char* buf = (char*)malloc(size_emd);
		file.read(buf, size_emd);

		file_Emd.write(buf, size_emd);
		file_Emd.close();
		::free(buf);
	}

	file.close();

	return true;
}
/*-------------------------------------------------------------------------------\
|                             direct_import					                     |
\-------------------------------------------------------------------------------*/
bool NSK::direct_import(string filename_esk, string filename_emd)
{
	mName = nameFromFilenameNoExtension(filename_emd, true);
	string folder = folderFromFilename(filename_emd);

	File file_Esk(filename_esk, LIBXENOVERSE_FILE_READ_BINARY);
	if ((!file_Esk.valid()) || (!file_Esk.readHeader(LIBXENOVERSE_ESK_SIGNATURE)))
		return false;

	File file_Emd(filename_emd, LIBXENOVERSE_FILE_READ_BINARY);
	if ((!file_Emd.valid()) || (!file_Emd.readHeader(LIBXENOVERSE_EMD_SIGNATURE)))
		return false;



	File file(folder + "/" + mName + ".nsk", LIBXENOVERSE_FILE_WRITE_BINARY);
	if (!file.valid())
		return false;



	size_t startAddress_Esk = 0;
	size_t size_esk = file_Esk.getFileSize();
	size_t startAddress_Emd = size_esk;
	size_t size_emd = file_Emd.getFileSize();
	

	
	file.goToAddress(startAddress_Esk);
	file_Esk.goToAddress(0);

	char* buf = (char*)malloc(size_esk);
	file_Esk.read(buf, size_esk);

	file.write(buf, size_esk);
	::free(buf);

	file.goToAddress(startAddress_Esk + 0x14);
	file.writeInt32E(&startAddress_Emd);							//add offset adress for begin of Emd.

	file_Esk.close();
	
	file_Emd.goToAddress(0);
	file.goToAddress(startAddress_Emd);

	buf = (char*)malloc(size_emd);
	file_Emd.read(buf, size_emd);

	file.write(buf, size_emd);
	::free(buf);
	file_Emd.close();


	file.close();

	return true;
}


}