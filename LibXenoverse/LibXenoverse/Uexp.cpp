

#include "Uexp.h"

#include <sstream>
#include <iomanip>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>




namespace LibXenoverse
{


/*-------------------------------------------------------------------------------\
|                             Uexp								                 |
\-------------------------------------------------------------------------------*/
Uexp::~Uexp(void)
{
	if (bndfll)
		delete bndfll;
	bndfll = 0;
}
/*-------------------------------------------------------------------------------\
|                             load								                 |
\-------------------------------------------------------------------------------*/
bool Uexp::load(string filename)
{
	name = nameFromFilenameNoExtension(filename, true);

	File file(filename, LIBXENOVERSE_FILE_READ_BINARY);
	if (!file.valid())
		return false;
	
	read(&file);
	file.close();
	return true;
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void Uexp::read(File *file)
{
	// UEXP_Header
	file->readInt32E(&uexpType);
	file->readInt32E(&unknow0);
	file->readInt32E(&uexpTypeData);

	bool overPass = false;
	if (file->getFileSize() > 0x19)					//test To search others case of BNDFLL (hyp flag is always on the same place.)
	{
		size_t mem = file->getCurrentAddress();

		file->goToAddress(0x14);
		unsigned char c;
		if ((file->readUChar(&c), c == 0x42) &&
			(file->readUChar(&c), c == 0x4E) && 
			(file->readUChar(&c), c == 0x44) && 
			(file->readUChar(&c), c == 0x46) && 
			(file->readUChar(&c), c == 0x4C) && 
			(file->readUChar(&c), c == 0x4C) )
		{
			overPass = true;
		}

		file->goToAddress(mem);
	}


	if (( ( (uexpType != 0x7) &&
		    (uexpType != 0x6))
		|| (uexpTypeData != 0)) 
		&& (!overPass))
	{
		return;
	}

	//UEXP_Header_0x7
	size_t sizeUexp = 0;
	file->readInt32E(&sizeUexp);
	file->readInt32E(&unknow2);

	if (sizeUexp >= file->getFileSize())
		return;

	size_t startOffsetSub = file->getCurrentAddress();

	tagName = "";
	file->readString(&tagName);
	file->fixPaddingRead(4);

	if (tagName != LIBXENOVERSE_BNDFLL_SIGNATURE)
		return;

	if ((overPass) && ((uexpType != 0x7) && (uexpType != 0x6)) || (uexpTypeData != 0))	//test todo remove
		debugAdded = true;


	bndfll = new Uexp_Bndfll();
	bndfll->read(file, startOffsetSub);




	file->goToAddress(startOffsetSub + sizeUexp);
	file->readInt32E(&uexp_finaleId);
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void Uexp_Bndfll::read(File *file, size_t startOffsetSub)
{
	files.clear();
	
	///BNDFF_Header
	size_t nbFiles = 0;
	size_t offsetFilesHeaders = 0;
	size_t sizeFilesHeaders = 0;
	size_t offsetFilesDatas = 0;

	file->readInt32E(&unknow0);
	file->readInt32E(&nbFiles);
	file->readInt32E(&offsetFilesHeaders);
	file->readInt32E(&sizeFilesHeaders);
	file->readInt32E(&unknow1);
	file->readInt32E(&unknow2);
	file->readInt32E(&offsetFilesDatas);
	file->readInt32E(&unknow3);
	file->readInt32E(&unknow4);
	file->readInt32E(&unknow5);

	for (size_t i = 0; i < nbFiles; i++)
	{
		file->goToAddress(startOffsetSub + offsetFilesHeaders + i * sizeFilesHeaders);
		
		files.push_back(new Uexp_Bndfll_File());
		files.back()->read(file, startOffsetSub, startOffsetSub + offsetFilesDatas);
	}
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void Uexp_Bndfll_File::read(File *file, size_t startOffsetSub, size_t startOffsetFilesDatas)
{
	size_t startOffset = file->getCurrentAddress();
	
	//BNDFF_FileHeader
	size_t offsetSubHeader = 0;							// += startOffset, but don't care is next to the first section.

	file->readInt32E(&fileId);
	file->readInt32E(&offsetSubHeader);
	file->readInt32E(&sizeFile);
	file->readInt32E(&unknow0_a);

	//BNDFF_FileSubHeader
	size_t offsetName = 0;
	size_t offsetNextHeader = 0;
	size_t offsetFileData = 0;

	file->readInt32E(&sizeFile);
	file->readInt32E(&unknow0);
	file->readInt32E(&unknow1);
	file->readInt32E(&useId);
	file->readInt32E(&offsetName);
	file->readInt32E(&unknow2);
	file->readInt32E(&offsetNextHeader);				//don't acer we have the size of block BNDFF_FileHeader + BNDFF_FileSubHeader 
	file->readInt32E(&unknow3);
	file->readInt32E(&offsetFileData);
	file->readInt32E(&unknow4);


	file->goToAddress(startOffsetSub + offsetName);
	file->readString(&name);

	fileDatas = (byte*)malloc(sizeFile);
	file->goToAddress(startOffsetFilesDatas + offsetFileData);
	file->read(fileDatas, sizeFile);
}










/*-------------------------------------------------------------------------------\
|                             save					                             |
\-------------------------------------------------------------------------------*/
void Uexp::save(string filename, bool big_endian)
{
	File file(filename, LIBXENOVERSE_FILE_WRITE_BINARY);
	if (file.valid())
	{
		write(&file);
		file.close();
	}
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void Uexp::write(File *file)
{
	size_t totalSize = getFileSize();
	file->writeNull(totalSize);
	file->goToAddress(0);


	// UEXP_Header
	file->writeInt32E(&uexpType);
	file->writeInt32E(&unknow0);
	file->writeInt32E(&uexpTypeData);

	if ( ( (uexpType != 0x7) && 
		   (uexpType != 0x6) ) || 
		(uexpTypeData != 0) || (!bndfll))
	{
		file->writeInt32E(&uexp_finaleId);
		return;
	}

	//UEXP_Header_0x7
	size_t sizeUexp = totalSize - (sizeof(UEXP_Header) + sizeof(UEXP_Header_0x7) + sizeof(uint32_t));
	file->writeInt32E(&sizeUexp);
	file->writeInt32E(&unknow2);


	size_t startOffsetSub = file->getCurrentAddress();

	file->writeString(&tagName);
	file->writeNull(1);
	file->fixPadding(4);



	bndfll->write(file, startOffsetSub);


	file->goToAddress(totalSize - sizeof(uint32_t));
	file->writeInt32E(&uexp_finaleId);

	file->goToEnd();
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void Uexp_Bndfll::write(File *file, size_t startOffsetSub)
{
	size_t offsetFilesDatas = sizeof(UEXP_Header_0x7) + sizeof(BNDFF_Header);

	size_t nbFiles = files.size();
	for (size_t i = 0; i < nbFiles; i++)
		offsetFilesDatas += files.at(i)->getFileSize() - files.at(i)->sizeFile;
	
	//BNDFF_Header
	
	size_t offsetFilesHeaders = sizeof(UEXP_Header_0x7) + sizeof(BNDFF_Header);
	size_t sizeFilesHeaders = 0x38;

	file->writeInt32E(&unknow0);
	file->writeInt32E(&nbFiles);
	file->writeInt32E(&offsetFilesHeaders);
	file->writeInt32E(&sizeFilesHeaders);
	file->writeInt32E(&unknow1);
	file->writeInt32E(&unknow2);
	file->writeInt32E(&offsetFilesDatas);
	file->writeInt32E(&unknow3);
	file->writeInt32E(&unknow4);
	file->writeInt32E(&unknow5);

	size_t offsetFileDatas = 0;
	size_t offsetFileNames = offsetFilesHeaders + nbFiles * (sizeof(BNDFF_FileHeader) + sizeof(BNDFF_FileSubHeader));
	for (size_t i = 0; i < nbFiles; i++)
	{
		file->goToAddress(startOffsetSub + offsetFilesHeaders + i * sizeFilesHeaders);
		files.at(i)->write(file, startOffsetSub, startOffsetSub + offsetFilesDatas, offsetFileDatas, offsetFileNames, (i+1==nbFiles) );
		offsetFileDatas += files.at(i)->sizeFile;
		offsetFileNames += (files.at(i)->name.length() + 1) * sizeof(char);
	}
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void Uexp_Bndfll_File::write(File *file, size_t startOffsetSub, size_t startOffsetFilesDatas, size_t offsetFileDatas, size_t offsetFileNames, bool islast)
{
	size_t startOffset = file->getCurrentAddress();

	//BNDFF_FileHeader
	size_t offsetSubHeader = 0x10;

	file->writeInt32E(&fileId);
	file->writeInt32E(&offsetSubHeader);
	file->writeInt32E(&sizeFile);
	file->writeInt32E(&unknow0_a);

	//BNDFF_FileSubHeader
	size_t offsetName = offsetFileNames;
	size_t offsetNextHeader = (islast) ? 0 : (startOffset - startOffsetSub + sizeof(BNDFF_FileHeader) + sizeof(BNDFF_FileSubHeader));
	size_t offsetFileData = offsetFileDatas;

	file->writeInt32E(&sizeFile);
	file->writeInt32E(&unknow0);
	file->writeInt32E(&unknow1);
	file->writeInt32E(&useId);
	file->writeInt32E(&offsetName);
	file->writeInt32E(&unknow2);
	file->writeInt32E(&offsetNextHeader);
	file->writeInt32E(&unknow3);
	file->writeInt32E(&offsetFileData);
	file->writeInt32E(&unknow4);


	file->goToAddress(startOffsetSub + offsetName);
	file->writeString(&name);

	file->goToAddress(startOffsetFilesDatas + offsetFileData);
	file->write(fileDatas, sizeFile);
}








/*-------------------------------------------------------------------------------\
|                             getFileSize			                             |
\-------------------------------------------------------------------------------*/
size_t Uexp::getFileSize()
{
	if (((uexpType != 0x7) && (uexpType != 0x6)) || (uexpTypeData != 0) || (!bndfll))
		return sizeof(UEXP_Header) + sizeof(uint32_t);

	return sizeof(UEXP_Header) + sizeof(UEXP_Header_0x7) + ((size_t)(ceil((tagName.length() + 1) / 4.0) * 4)) * sizeof(char) + bndfll->getFileSize() + sizeof(uint32_t);
}
/*-------------------------------------------------------------------------------\
|                             getFileSize			                             |
\-------------------------------------------------------------------------------*/
size_t Uexp_Bndfll::getFileSize()
{
	size_t fileSize = sizeof(BNDFF_Header);

	for (size_t i = 0, nb = files.size(); i < nb; i++)
		fileSize += files.at(i)->getFileSize();

	return fileSize;
}
/*-------------------------------------------------------------------------------\
|                             getFileSize			                             |
\-------------------------------------------------------------------------------*/
size_t Uexp_Bndfll_File::getFileSize()
{
	return sizeof(BNDFF_FileHeader) + sizeof(BNDFF_FileSubHeader) + (name.length() + 1) * sizeof(char) + sizeFile;
}




















/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Extract/Repack ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////






/*-------------------------------------------------------------------------------\
|                             extract				                             |
\-------------------------------------------------------------------------------*/
void Uexp::extract(string folder)
{

	if (!bndfll)
	{
		printf("Error no BNDFLL tags inside detected.\n");
		LibXenoverse::notifyError();
		LibXenoverse::waitOnEnd();
		return;
	}

	//add a Xml to keep files orders
	TiXmlDocument *doc = new TiXmlDocument();
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc->LinkEndChild(decl);

	TiXmlElement* rootNode = new TiXmlElement("Uexp");
	rootNode->SetAttribute("name", name);
	rootNode->SetAttribute("type", EMO_BaseFile::UnsignedToString(uexpType, true));
	rootNode->SetAttribute("typeData", EMO_BaseFile::UnsignedToString(uexpTypeData, true));
	rootNode->SetAttribute("uexp_finaleId", EMO_BaseFile::UnsignedToString(uexp_finaleId, true));
	rootNode->SetAttribute("unknow0", EMO_BaseFile::UnsignedToString(unknow0, true));
	rootNode->SetAttribute("unknow2", EMO_BaseFile::UnsignedToString(unknow2, true));
	rootNode->SetAttribute("tagName", tagName);

	TiXmlElement* xmlCurrentNode = new TiXmlElement("Bndfll");
	rootNode->LinkEndChild(xmlCurrentNode);

	xmlCurrentNode->SetAttribute("unknow0", EMO_BaseFile::UnsignedToString(bndfll->unknow0, true));
	xmlCurrentNode->SetAttribute("unknow1", EMO_BaseFile::UnsignedToString(bndfll->unknow1, true));
	xmlCurrentNode->SetAttribute("unknow2", EMO_BaseFile::UnsignedToString(bndfll->unknow2, true));
	xmlCurrentNode->SetAttribute("unknow3", EMO_BaseFile::UnsignedToString(bndfll->unknow3, true));
	xmlCurrentNode->SetAttribute("unknow4", EMO_BaseFile::UnsignedToString(bndfll->unknow4, true));
	xmlCurrentNode->SetAttribute("unknow5", EMO_BaseFile::UnsignedToString(bndfll->unknow5, true));


	std::vector<string> listFilenameAllreadyCreated;
	std::vector<size_t> listIndexToRenameDuplicate;

	TiXmlElement* xmlNode;
	Uexp_Bndfll_File* file;
	string filename;
	for (size_t i = 0, nbFiles = bndfll->files.size(); i < nbFiles; i++)
	{
		file = bndfll->files.at(i);
		filename = file->name;
		
		if (!file->save(folder + file->name))
		{
			printf("Error the file %s can't be saved.\n", file->name.c_str());
			LibXenoverse::notifyError();
			LibXenoverse::waitOnEnd();
		}


		xmlNode = new TiXmlElement("File");
		xmlNode->SetAttribute("name", file->name);
		xmlNode->SetAttribute("fileId", EMO_BaseFile::UnsignedToString(file->fileId, false));
		xmlNode->SetAttribute("useId", EMO_BaseFile::UnsignedToString(file->useId, true));
		xmlNode->SetAttribute("unknow0_a", EMO_BaseFile::UnsignedToString(file->unknow0_a, true));
		xmlNode->SetAttribute("unknow0", EMO_BaseFile::UnsignedToString(file->unknow0, true));
		xmlNode->SetAttribute("unknow1", EMO_BaseFile::UnsignedToString(file->unknow1, true));
		xmlNode->SetAttribute("unknow2", EMO_BaseFile::UnsignedToString(file->unknow2, true));
		xmlNode->SetAttribute("unknow3", EMO_BaseFile::UnsignedToString(file->unknow3, true));
		xmlNode->SetAttribute("unknow4", EMO_BaseFile::UnsignedToString(file->unknow4, true));

		//comments 
		TiXmlComment* xmlComment = new TiXmlComment(("index:" + std::to_string(i)).c_str());
		xmlCurrentNode->LinkEndChild(xmlComment);
		xmlCurrentNode->LinkEndChild(xmlNode);
	}

	doc->LinkEndChild(rootNode);

	doc->SaveFile(folder + "bndfllFiles.xml");
	delete doc;
}



/*-------------------------------------------------------------------------------\
|                             addFolder				                             |
\-------------------------------------------------------------------------------*/
void Uexp::addFolder(string folder)
{
	string filename_orderedFilesXml = "";

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile((folder + "*.*").c_str(), &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("Error can't scan the folder %s.\n", folder.c_str());
		LibXenoverse::notifyError();
		LibXenoverse::waitOnEnd();
		return;
	}

	{
		if (bndfll)
			delete bndfll;
		bndfll = new Uexp_Bndfll();

		do
		{
			const char *name = FindFileData.cFileName;
			if (name[0] == '.')
				continue;

			string str = ToString(name);
			string new_filename = folder + str;
			if (str != "bndfllFiles.xml")
				addFile(new_filename);
			else
				filename_orderedFilesXml = new_filename;				//there is a special file witch could give the order of files (when emb have filename, so Windows order is'nt good enougth)
		} while (FindNextFile(hFind, &FindFileData) != 0);
		FindClose(hFind);
	}

	if (filename_orderedFilesXml == "")
	{
		printf("Error bndfllFiles.xml found. it's needed.\n");
		LibXenoverse::notifyError();
		LibXenoverse::waitOnEnd();
		return;
	}
	
	TiXmlDocument doc(filename_orderedFilesXml);
	if (!doc.LoadFile())
	{
		printf("Error can't load bndfllFiles.xml.\n");
		LibXenoverse::notifyError();
		LibXenoverse::waitOnEnd();
		return;
	}
		
	
	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);

	TiXmlElement* rootNode = hDoc.FirstChildElement("Uexp").Element();
	if (!rootNode)
	{
		printf("Error Uexp not found in bndfllFiles.xml.\n");
		LibXenoverse::notifyError();
		LibXenoverse::waitOnEnd();
		return;
	}

	TiXmlElement* xmlCurrentNode = rootNode->FirstChildElement("Bndfll");
	if (!xmlCurrentNode)
	{
		printf("Error Bndfll not found in bndfllFiles.xml.\n");
		LibXenoverse::notifyError();
		LibXenoverse::waitOnEnd();
		return;
	}
	
	string str = "";
	rootNode->QueryStringAttribute("type", &str); uexpType = EMO_BaseFile::GetUnsigned(str);
	rootNode->QueryStringAttribute("typeData", &str); uexpTypeData = EMO_BaseFile::GetUnsigned(str);
	rootNode->QueryStringAttribute("uexp_finaleId", &str); uexp_finaleId = EMO_BaseFile::GetUnsigned(str);
	rootNode->QueryStringAttribute("unknow0", &str); unknow0 = EMO_BaseFile::GetUnsigned(str);
	rootNode->QueryStringAttribute("unknow2", &str); unknow2 = EMO_BaseFile::GetUnsigned(str);
	rootNode->QueryStringAttribute("tagName", &tagName);

	xmlCurrentNode->QueryStringAttribute("unknow0", &str); bndfll->unknow0 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow1", &str); bndfll->unknow1 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow2", &str); bndfll->unknow2 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow3", &str); bndfll->unknow3 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow4", &str); bndfll->unknow4 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow5", &str); bndfll->unknow5 = EMO_BaseFile::GetUnsigned(str);

	vector<Uexp_Bndfll_File*> newListFiles;
	Uexp_Bndfll_File* file;

	size_t nbFiles = bndfll->files.size();
	for (TiXmlElement* xmlNode = xmlCurrentNode->FirstChildElement("File"); xmlNode; xmlNode = xmlNode->NextSiblingElement("File"))
	{
		if (xmlNode->QueryStringAttribute("name", &str) != TIXML_SUCCESS)
			continue;

		file = 0;
		for (size_t i = 0; i < nbFiles; i++)
		{
			if (bndfll->files.at(i)->name == str)
			{
				file = bndfll->files.at(i);

				bndfll->files.erase(bndfll->files.begin() + i);
				nbFiles--;
				break;
			}
		}

		if (!file)
		{
			printf("Warning : % specified in sbndfllFiles.xml is not found in folder.\n", str.c_str());
			LibXenoverse::notifyWarning();
			LibXenoverse::waitOnEnd();
			continue;
		}

		xmlNode->QueryStringAttribute("fileId", &str); file->fileId = EMO_BaseFile::GetUnsigned(str);
		xmlNode->QueryStringAttribute("useId", &str); file->useId = EMO_BaseFile::GetUnsigned(str);
		xmlNode->QueryStringAttribute("unknow0_a", &str); file->unknow0_a = EMO_BaseFile::GetUnsigned(str);
		xmlNode->QueryStringAttribute("unknow0", &str); file->unknow0 = EMO_BaseFile::GetUnsigned(str);
		xmlNode->QueryStringAttribute("unknow1", &str); file->unknow1 = EMO_BaseFile::GetUnsigned(str);
		xmlNode->QueryStringAttribute("unknow2", &str); file->unknow2 = EMO_BaseFile::GetUnsigned(str);
		xmlNode->QueryStringAttribute("unknow3", &str); file->unknow3 = EMO_BaseFile::GetUnsigned(str);
		xmlNode->QueryStringAttribute("unknow4", &str); file->unknow4 = EMO_BaseFile::GetUnsigned(str);
		
		newListFiles.push_back(file);
	}
	bndfll->files = newListFiles;
}
/*-------------------------------------------------------------------------------\
|                             addFile				                             |
\-------------------------------------------------------------------------------*/
void Uexp::addFile(string filename)
{
	if (!fileCheck(filename))
		return;

	Uexp_Bndfll_File* bndfll_File = new Uexp_Bndfll_File();
	if (bndfll_File->load(filename))
		bndfll->files.push_back(bndfll_File);
	else
		delete bndfll_File;
}



/*-------------------------------------------------------------------------------\
|                             addFolder				                             |
\-------------------------------------------------------------------------------*/
void Uexp::updateUassetFile(string filename)
{
	size_t sizeOfUexp = getFileSize() - 4;

	File file_r(filename, LIBXENOVERSE_FILE_READ_BINARY);
	if (!file_r.valid())
	{
		printf("Warning %s not found for modification of size. You should do it manually : %s at 0x1F9.\n", (filename + ".uasset").c_str(), EMO_BaseFile::UnsignedToString(BE32(sizeOfUexp), true).c_str());
		LibXenoverse::notifyWarning();
		LibXenoverse::waitOnEnd();
		return;
	}

	
	size_t sizeFile = file_r.getFileSize();
	if (sizeFile <= 0x1FD)
	{
		printf("Warning %s don't have the right size. You should do it manually : %s at 0x1F9.\n", (filename + ".uasset").c_str(), EMO_BaseFile::UnsignedToString(BE32(sizeOfUexp), true).c_str());
		LibXenoverse::notifyWarning();
		LibXenoverse::waitOnEnd();

		file_r.close();
		return;
	}


	byte* fileDatas = (byte*)malloc(sizeFile);
	file_r.read(fileDatas, sizeFile);
	file_r.close();
	
	
	File file(filename, LIBXENOVERSE_FILE_WRITE_BINARY);
	if (!file.valid())
	{
		printf("Warning %s not allow to write inside for modification of size. You should do it manually : %s at 0x1F9.\n", (filename + ".uasset").c_str(), EMO_BaseFile::UnsignedToString(BE32(sizeOfUexp), true).c_str());
		LibXenoverse::notifyWarning();
		LibXenoverse::waitOnEnd();

		free(fileDatas);
		return;
	}

	file.write(fileDatas, sizeFile);

	file.goToAddress(0x1F9);
	file.writeInt32E(&sizeOfUexp);
	file.close();

	free(fileDatas);
}














/*-------------------------------------------------------------------------------\
|                             load					                             |
\-------------------------------------------------------------------------------*/
bool Uexp_Bndfll_File::load(string filename)
{
	File file(filename, LIBXENOVERSE_FILE_READ_BINARY);
	if (!file.valid())
		return false;

	name = nameFromFilename(filename);

	// Read entire file into data buffer
	sizeFile = file.getFileSize();
	fileDatas = (byte*)malloc(sizeFile);
	file.read(fileDatas, sizeFile);

	file.close();
	return true;
}
/*-------------------------------------------------------------------------------\
|                             save					                             |
\-------------------------------------------------------------------------------*/
bool Uexp_Bndfll_File::save(string filename)
{
	if ((sizeFile == 0) || (!fileDatas))
		return false;

	File file(filename, LIBXENOVERSE_FILE_WRITE_BINARY);
	if (!file.valid())
		return false;

	file.write(fileDatas, sizeFile);
	file.close();
	return true;
}











/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Xml ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////













/*-------------------------------------------------------------------------------\
|                             loadXml											 |
\-------------------------------------------------------------------------------*/
bool Uexp::loadXml(string filename)
{
	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
	{
		printf("Loading xml %s fail. skip.'\n", filename.c_str());
		getchar();
		return false;
	}

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);
	TiXmlElement* rootNode = hDoc.FirstChildElement("Uexp").Element();
	if (!rootNode)
	{
		printf("%s don't have 'Uexp' tags. skip.'\n", filename.c_str());
		getchar();
		return false;
	}

	return importXml(rootNode);
}
/*-------------------------------------------------------------------------------\
|                             saveXml											 |
\-------------------------------------------------------------------------------*/
void Uexp::saveXml(string filename)
{
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement* rootNode = exportXml();
	doc.LinkEndChild(rootNode);

	doc.SaveFile(filename);
}








/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool Uexp::importXml(TiXmlElement* xmlCurrentNode)
{
	string str = "";
	xmlCurrentNode->QueryStringAttribute("name", &name);
	xmlCurrentNode->QueryStringAttribute("type", &str); uexpType = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("typeData", &str); uexpTypeData = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("uexp_finaleId", &str); uexp_finaleId = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow0", &str); unknow0 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow2", &str); unknow2 = EMO_BaseFile::GetUnsigned(str);

	xmlCurrentNode->QueryStringAttribute("tagName", &tagName);			//test Todo remove.


	if (bndfll)
	{
		delete bndfll;
		bndfll = 0;
	}

	TiXmlElement* bndfllNode = xmlCurrentNode->FirstChildElement("Bndfll");
	if (!bndfllNode)
		return true;

	bndfll = new Uexp_Bndfll();
	bndfll->importXml(bndfllNode);

	return true;
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool Uexp_Bndfll::importXml(TiXmlElement* xmlCurrentNode)
{
	string str = "";
	xmlCurrentNode->QueryStringAttribute("unknow0", &str); unknow0 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow1", &str); unknow1 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow2", &str); unknow2 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow3", &str); unknow3 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow4", &str); unknow4 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow5", &str); unknow5 = EMO_BaseFile::GetUnsigned(str);
	
	for (TiXmlElement* xmlNode = xmlCurrentNode->FirstChildElement("File"); xmlNode; xmlNode = xmlNode->NextSiblingElement("File"))
	{
		files.push_back(new Uexp_Bndfll_File());
		if (!files.back()->importXml(xmlNode))
		{
			delete files.back();
			files.pop_back();
		}
	}

	return (files.size() != 0);
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool Uexp_Bndfll_File::importXml(TiXmlElement* xmlCurrentNode)
{
	string str = "";
	xmlCurrentNode->QueryStringAttribute("name", &name);

	xmlCurrentNode->QueryStringAttribute("fileId", &str); fileId = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("useId", &str); useId = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("sizeFile", &str); sizeFile = EMO_BaseFile::GetUnsigned(str);

	xmlCurrentNode->QueryStringAttribute("unknow0_a", &str); unknow0_a = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow0", &str); unknow0 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow1", &str); unknow1 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow2", &str); unknow2 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow3", &str); unknow3 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow4", &str); unknow4 = EMO_BaseFile::GetUnsigned(str);

	
	return (sizeFile != 0);
}












/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* Uexp::exportXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("Uexp");

	xmlCurrentNode->SetAttribute("name", name);
	xmlCurrentNode->SetAttribute("type", EMO_BaseFile::UnsignedToString(uexpType, true));
	xmlCurrentNode->SetAttribute("typeData", EMO_BaseFile::UnsignedToString(uexpTypeData, true));
	xmlCurrentNode->SetAttribute("uexp_finaleId", EMO_BaseFile::UnsignedToString(uexp_finaleId, true));
	
	xmlCurrentNode->SetAttribute("unknow0", EMO_BaseFile::UnsignedToString(unknow0, true));
	xmlCurrentNode->SetAttribute("unknow2", EMO_BaseFile::UnsignedToString(unknow2, true));

	xmlCurrentNode->SetAttribute("tagName", tagName);		//test todo remove
	xmlCurrentNode->SetAttribute("testTypes", EMO_BaseFile::UnsignedToString(uexpType, true) +"_"+ EMO_BaseFile::UnsignedToString(uexpTypeData, true));		//test todo remove
	if(debugAdded)
		xmlCurrentNode->SetAttribute("debugAdded", "true");			//test Todo remove.

	if (bndfll)
		xmlCurrentNode->LinkEndChild( bndfll->exportXml() );

	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* Uexp_Bndfll::exportXml()
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("Bndfll");

	xmlCurrentNode->SetAttribute("unknow0", EMO_BaseFile::UnsignedToString(unknow0, true));
	xmlCurrentNode->SetAttribute("unknow1", EMO_BaseFile::UnsignedToString(unknow1, true));
	xmlCurrentNode->SetAttribute("unknow2", EMO_BaseFile::UnsignedToString(unknow2, true));
	xmlCurrentNode->SetAttribute("unknow3", EMO_BaseFile::UnsignedToString(unknow3, true));
	xmlCurrentNode->SetAttribute("unknow4", EMO_BaseFile::UnsignedToString(unknow4, true));
	xmlCurrentNode->SetAttribute("unknow5", EMO_BaseFile::UnsignedToString(unknow5, true));


	for (size_t i = 0, nbFiles = files.size(); i < nbFiles; i++)
		xmlCurrentNode->LinkEndChild(files.at(i)->exportXml());

	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* Uexp_Bndfll_File::exportXml()
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("File");

	xmlCurrentNode->SetAttribute("name", name);

	xmlCurrentNode->SetAttribute("fileId", EMO_BaseFile::UnsignedToString(fileId, false));
	xmlCurrentNode->SetAttribute("useId", EMO_BaseFile::UnsignedToString(useId, true));
	xmlCurrentNode->SetAttribute("sizeFile", EMO_BaseFile::UnsignedToString(sizeFile, true));

	xmlCurrentNode->SetAttribute("unknow0_a", EMO_BaseFile::UnsignedToString(unknow0_a, true));
	xmlCurrentNode->SetAttribute("unknow0", EMO_BaseFile::UnsignedToString(unknow0, true));
	xmlCurrentNode->SetAttribute("unknow1", EMO_BaseFile::UnsignedToString(unknow1, true));
	xmlCurrentNode->SetAttribute("unknow2", EMO_BaseFile::UnsignedToString(unknow2, true));
	xmlCurrentNode->SetAttribute("unknow3", EMO_BaseFile::UnsignedToString(unknow3, true));
	xmlCurrentNode->SetAttribute("unknow4", EMO_BaseFile::UnsignedToString(unknow4, true));

	xmlCurrentNode->SetAttribute("TestUseId", extensionFromFilename(name) +"_"+ EMO_BaseFile::UnsignedToString(useId, true));		//test Todo remove

	return xmlCurrentNode;
}












/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// wxHexEditor coloration ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////





/*-------------------------------------------------------------------------------\
|                             save_Coloration		                             |
\-------------------------------------------------------------------------------*/
void Uexp::save_Coloration(string filename, bool show_error)
{
	name = filename;

	uint8_t *buf;
	size_t size;

	buf = EMO_BaseFile::ReadFile(filename, &size, show_error);
	if (!buf)
		return;

	BinColorTag binCt;

	write_Coloration(binCt, binCt.startColoration(filename), buf, size);

	binCt.endColoration(filename);

	return;
}




/*-------------------------------------------------------------------------------\
|                             write_Coloration				                     |
\-------------------------------------------------------------------------------*/
void Uexp::write_Coloration(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size)
{	
	
	

	std::vector<bool> listBytesAllreadyTagged;
	listBytesAllreadyTagged.resize(size, false);				//to check override of the same byte (overflow)


	size_t idTag = 0;
	size_t offset = 0;
	size_t size_padding = 0;
	size_t incSection = 0;
	size_t incParam = 0;

	size_t startAdress = 0;
	uint32_t *name_offset = 0;
	string name_tmp = "";

	UEXP_Header* hdr = (UEXP_Header*)buf;
	binCt.write_Coloration_Tag("uexpType", "uint32_t", "0x7", offset, sizeof(uint32_t), "UEXP_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow0", "uint32_t", "", offset, sizeof(uint32_t), "UEXP_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("uexpTypeData", "uint32_t", "0x0 is for bndFll, 0x4 for textures, 0xA for movies (todo check)", offset, sizeof(uint32_t), "UEXP_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	
	if( ( (hdr->uexpType != 0x7) &&
		  (hdr->uexpType != 0x6) ) ||
		(hdr->uexpTypeData != 0))				//Todo check if there is not a big restrictions (but for first tests, we check if we pass all files with only bndFll , witch apparently is for this ==0)
		return;

	incSection++;
	incParam = 0;

	UEXP_Header_0x7* bndfll_Header_0x7 = (UEXP_Header_0x7*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
	binCt.write_Coloration_Tag("sizeUexp", "uint32_t", "", offset, sizeof(uint32_t), "UEXP_Header_0x7", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow2", "uint32_t", "", offset, sizeof(uint32_t), "UEXP_Header_0x7", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

	if (bndfll_Header_0x7->sizeUexp >= size)
		return;
	
	incSection++;
	incParam = 0;

	size_t startoffset_BNDFF = offset;
	name_tmp = string((char*)EMO_BaseFile::GetOffsetPtr_native(buf, offset));
	binCt.write_Coloration_Tag("UexpType/Flag", "string", "", offset, (name_tmp.size() + 1) * sizeof(char), "UexpTypeName", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, (size_t)-1, false); offset += (name_tmp.size() + 1) * sizeof(char);


	if (name_tmp.substr(0, string(LIBXENOVERSE_BNDFLL_SIGNATURE).length()) != LIBXENOVERSE_BNDFLL_SIGNATURE)
		return;
	
	LOG_DEBUG("Have BNDFLL : \n");

	incSection++;
	incParam = 0;

	size_padding = (size_t)(ceil((double)offset / 4.0) * 4.0) - offset;
	if (size_padding)
		binCt.write_Coloration_Tag("padding", "padding", "", offset, size_padding, "UexpTypeName", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
	offset += size_padding;



	size_t startoffset_BNDFF_Header = offset;
	BNDFF_Header* bndfll_Header = (BNDFF_Header*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
	incSection++;
	incSection++;
	incParam = 0;

	binCt.write_Coloration_Tag("unknow0", "uint32_t", "", offset, sizeof(uint32_t), "BNDFF_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("nbFiles", "uint32_t", "", offset, sizeof(uint32_t), "BNDFF_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("offsetFilesHeaders", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startoffset_BNDFF + bndfll_Header->offsetFilesHeaders, true) + " BNDFF_FileHeaders", offset, sizeof(uint32_t), "BNDFF_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("sizeFilesHeaders", "uint32_t", "", offset, sizeof(uint32_t), "BNDFF_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow1", "uint32_t", "", offset, sizeof(uint32_t), "BNDFF_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow2", "uint32_t", "", offset, sizeof(uint32_t), "BNDFF_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("offsetFilesDatas", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startoffset_BNDFF + bndfll_Header->offsetFilesDatas, true) + " FilesDatas", offset, sizeof(uint32_t), "BNDFF_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow3", "uint32_t", "", offset, sizeof(uint32_t), "BNDFF_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow4", "uint32_t", "", offset, sizeof(uint32_t), "BNDFF_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	binCt.write_Coloration_Tag("unknow5", "uint32_t", "", offset, sizeof(uint32_t), "BNDFF_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);


	incSection++;
	incParam = 0;
	size_t incSection_FH = incSection;
	size_t incSection_FHS = incSection + 1;
	size_t incSection_FN = incSection + 2;
	size_t incSection_FD = incSection + 3;
	for (size_t i = 0; i < bndfll_Header->nbFiles; i++)
	{
		size_t startoffset_BNDFF_FileHeader = offset = startoffset_BNDFF + bndfll_Header->offsetFilesHeaders + i * bndfll_Header->sizeFilesHeaders;

		incSection = incSection_FH;
		incParam = 0;

		BNDFF_FileHeader* bndfll_FileHeader = (BNDFF_FileHeader*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
		binCt.write_Coloration_Tag("fileId", "uint32_t", "same as on filename of the packed file", offset, sizeof(uint32_t), "BNDFF_FileHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("offsetSubHeader", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startoffset_BNDFF_FileHeader + bndfll_FileHeader->offsetSubHeader, true) + " BNDFF_FileSubHeader", offset, sizeof(uint32_t), "BNDFF_FileHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("sizeFile", "uint32_t", "", offset, sizeof(uint32_t), "BNDFF_FileHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("unknow0", "uint32_t", "", offset, sizeof(uint32_t), "BNDFF_FileHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		
		incSection = incSection_FHS;
		incParam = 0;

		size_t startoffset_BNDFF_FileSubHeader = offset = startoffset_BNDFF_FileHeader + bndfll_FileHeader->offsetSubHeader;

		BNDFF_FileSubHeader* bndfll_FileSubHeader = (BNDFF_FileSubHeader*)(EMO_BaseFile::GetOffsetPtr_native(buf, offset));
		binCt.write_Coloration_Tag("sizeFile", "uint32_t", "", offset, sizeof(uint32_t), "BNDFF_FileSubHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("unknow0", "uint32_t", "", offset, sizeof(uint32_t), "BNDFF_FileSubHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("unknow1", "uint32_t", "", offset, sizeof(uint32_t), "BNDFF_FileSubHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("useId", "uint32_t", "Id linked to the file extension, so it's for doing something with the specified file", offset, sizeof(uint32_t), "BNDFF_FileSubHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("offsetName", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startoffset_BNDFF + bndfll_FileSubHeader->offsetName, true) + " BNDFF_FileName", offset, sizeof(uint32_t), "BNDFF_FileSubHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("unknow2", "uint32_t", "", offset, sizeof(uint32_t), "BNDFF_FileSubHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("offsetNextHeader", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startoffset_BNDFF + bndfll_FileSubHeader->offsetNextHeader, true) + " Next BNDFF_FileSubHeader", offset, sizeof(uint32_t), "BNDFF_FileSubHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("unknow3", "uint32_t", "", offset, sizeof(uint32_t), "BNDFF_FileSubHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("offsetFileData", "uint32_t", " => " + EMO_BaseFile::UnsignedToString(startoffset_BNDFF + bndfll_Header->offsetFilesDatas + bndfll_FileSubHeader->offsetFileData, true) + " Next BNDFF_FileData", offset, sizeof(uint32_t), "BNDFF_FileSubHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		binCt.write_Coloration_Tag("unknow4", "uint32_t", "", offset, sizeof(uint32_t), "BNDFF_FileSubHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);



		incSection = incSection_FN;
		incParam = 0;

		offset = startoffset_BNDFF + bndfll_FileSubHeader->offsetName;
		name_tmp = string((char*)EMO_BaseFile::GetOffsetPtr_native(buf, offset));
		binCt.write_Coloration_Tag("filename", "string", "", offset, (name_tmp.size() + 1) * sizeof(char), "BNDFF_FileName", parent, idTag++, incSection, i, listBytesAllreadyTagged, (size_t)-1, false); offset += (name_tmp.size() + 1) * sizeof(char);



		incSection = incSection_FD + i;
		incParam = 0;

		offset = startoffset_BNDFF + bndfll_Header->offsetFilesDatas + bndfll_FileSubHeader->offsetFileData;
		binCt.write_Coloration_Tag("FileData["+ ToString(i) +"]", ToString(bndfll_FileSubHeader->sizeFile) +" x octets", "", offset, bndfll_FileSubHeader->sizeFile * sizeof(char), "BNDFF_FileData", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += bndfll_FileSubHeader->sizeFile * sizeof(char);
	}
	incSection = incSection_FD + bndfll_Header->nbFiles;


	incSection++;
	incParam = 0;
	binCt.write_Coloration_Tag("UEXP_FinaleId", "uint32_t", "same as first Uasset uint32", offset, sizeof(uint32_t), "Uexp", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);






	binCt.write_Coloration_Gaps(0x0, size, sizeof(uint8_t), idTag, "Empty", parent, listBytesAllreadyTagged);		//test Empty/gaps/holes.
}


}