
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


}