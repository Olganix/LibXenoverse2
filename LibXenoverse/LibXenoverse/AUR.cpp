
#include "XenoFiles.h"

namespace LibXenoverse
{
	



/*-------------------------------------------------------------------------------\
|                             load					                             |
\-------------------------------------------------------------------------------*/
bool AUR::load(string filename)
{
	if (filename.find(".xml") != string::npos)
		return loadXml(filename);
		
		
	File file(filename, LIBXENOVERSE_FILE_READ_BINARY);

	if (file.valid() && file.readHeader(LIBXENOVERSE_AUR_SIGNATURE))
	{
		read(&file);
		file.close();
	}else {
		return false;
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             save					                             |
\-------------------------------------------------------------------------------*/
void AUR::save(string filename, bool big_endian)
{
	File file(filename, LIBXENOVERSE_FILE_WRITE_BINARY);
	if (file.valid())
	{
		file.writeHeader(LIBXENOVERSE_AUR_SIGNATURE, big_endian);
		write(&file);
		file.close();
	}
}
/*-------------------------------------------------------------------------------\
|                             loadXml				                             |
\-------------------------------------------------------------------------------*/
bool AUR::loadXml(string filename)
{
	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
		return false;

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);

	TiXmlElement* rootNode = hDoc.FirstChildElement("AUR").Element();
	if (!rootNode)
	{
		printf("%s don't have 'AUR' tags. skip.'\n", filename);
		notifyError();
		return false;
	}


	TiXmlElement* auraTypeNode = rootNode->FirstChildElement("AuraTypes");
	if (!auraTypeNode)
	{
		printf("%s don't have 'AURAuraTypes' tags. skip.'\n", filename);
		notifyError();
		return false;
	}

	TiXmlElement* auraNode = rootNode->FirstChildElement("Auras");
	if (!auraNode)
	{
		printf("%s don't have 'AURAuras' tags. skip.'\n", filename);
		notifyError();
		return false;
	}

	TiXmlElement* auraCharacNode = rootNode->FirstChildElement("CharacLinks");
	if (!auraCharacNode)
	{
		printf("%s don't have 'AURCharacs' tags. skip.'\n", filename);
		notifyError();
		return false;
	}


	string str = "";
	for (TiXmlElement* xmlNode = auraTypeNode->FirstChildElement("AuraType"); xmlNode; xmlNode = xmlNode->NextSiblingElement("AuraType"))
	{
		
		if (xmlNode->QueryStringAttribute("name", &str) != TIXML_SUCCESS)
			continue;
		
		mListAuraType.push_back(str);
	}


	for (TiXmlElement* xmlNode = auraNode->FirstChildElement("Aura"); xmlNode; xmlNode = xmlNode->NextSiblingElement("Aura"))
	{
		AURAura aurAura;
		if(aurAura.readXML(xmlNode, mListAuraType))
			mListAura.push_back(aurAura);
	}

	

	for (TiXmlElement* xmlNode = auraCharacNode->FirstChildElement("CharacLink"); xmlNode; xmlNode = xmlNode->NextSiblingElement("CharacLink"))
	{
		AURCharac aurAuraCharac;
		if (aurAuraCharac.readXML(xmlNode))
			mListCharacLink.push_back(aurAuraCharac);
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             saveXML				                             |
\-------------------------------------------------------------------------------*/
void AUR::saveXml(string filename)
{
	TiXmlDocument doc;
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement* rootNode = new TiXmlElement("AUR");



	TiXmlElement* auraTypeNode = new TiXmlElement("AuraTypes");
	TiXmlElement* xmlNode;
	size_t nbElements = mListAuraType.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		xmlNode = new TiXmlElement("AuraType");

		xmlNode->SetAttribute("name", mListAuraType.at(i));

		auraTypeNode->LinkEndChild(xmlNode);
	}
	rootNode->LinkEndChild(auraTypeNode);


	TiXmlElement* auraNode = new TiXmlElement("Auras");
	nbElements = mListAura.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		xmlNode = mListAura.at(i).writeXml(mListAuraType);
		auraNode->LinkEndChild(xmlNode);
	}
	rootNode->LinkEndChild(auraNode);




	


	TiXmlElement* auraCharacNode = new TiXmlElement("CharacLinks");
	nbElements = mListCharacLink.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		xmlNode = mListCharacLink.at(i).writeXml();
		auraCharacNode->LinkEndChild(xmlNode);
	}
	rootNode->LinkEndChild(auraCharacNode);


	doc.LinkEndChild(rootNode);

	doc.SaveFile(filename);
}





/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void AUR::read(File *file)
{
	file->goToAddress(0x8);
	
	unsigned int aura_count = 0;
	file->readInt32E(&aura_count);
	unsigned int listAura_address = 0;
	file->readInt32E(&listAura_address);

	
	unsigned int type_count = 0;
	file->readInt32E(&type_count);
	unsigned int type_address = 0;
	file->readInt32E(&type_address);


	unsigned int characAuraLink_count = 0;
	file->readInt32E(&characAuraLink_count);
	unsigned int characAuraLink_address = 0;
	file->readInt32E(&characAuraLink_address);



	
	mListAuraType.clear();
	unsigned int addressTypeName = 0;
	string str;
	for (size_t i = 0; i < type_count; i++)
	{
		file->goToAddress(type_address + i * 4);
		file->readInt32E(&addressTypeName);

		file->goToAddress(addressTypeName);
		file->readString(&str);

		mListAuraType.push_back(str);
	}


	file->goToAddress(listAura_address);
	mListAura.resize(aura_count);
	for (size_t i = 0; i < aura_count; i++)
	{
		file->goToAddress(listAura_address + i * 4 * 4);
		mListAura.at(i).read(file);
	}


	file->goToAddress(characAuraLink_address);
	mListCharacLink.resize(characAuraLink_count);
	for (size_t i = 0; i < characAuraLink_count; i++)
	{
		file->goToAddress(characAuraLink_address + i * 4 * 4);
		mListCharacLink.at(i).read(file);
	}
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void AURAura::read(File *file)
{	
	file->readInt32E(&mId);
	file->readInt32E(&mUnknow0);

	size_t countColors = 0;
	file->readInt32E(&countColors);
	size_t startAdressColors = 0;
	file->readInt32E(&startAdressColors);

	mListColorByType.clear();
	if (countColors!=0)
	{
		file->goToAddress(startAdressColors);
		
		size_t index = 0;
		size_t idEffect = 0;
		for (size_t i = 0; i < countColors; i++)
		{
			file->readInt32E(&index);
			file->readInt32E(&idEffect);

			mListColorByType.push_back(AURAuraEffect(index, idEffect));
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void AURCharac::read(File *file)
{
	file->readInt32E(&mIdCharac);
	file->readInt32E(&mCostumeId);
	file->readInt32E(&mIdAura);
	size_t tmp = 0;
	file->readInt32E(&tmp);
	mGlare = tmp != 0;
}





/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void AUR::write(File *file)
{
	// Header
	file->goToAddress(0x6);

	unsigned short header = 0x20;
	file->writeInt16E(&header);

	size_t aura_count = mListAura.size();
	file->writeInt32E(&aura_count);
	unsigned int listAura_address = 0x20;
	file->writeInt32E(&listAura_address);

	size_t type_count = mListAuraType.size();
	file->writeInt32E(&type_count);
	file->writeNull(4);			//after Aura (unknow size).

	size_t characAuraLink_count = mListCharacLink.size();
	file->writeInt32E(&characAuraLink_count);
	file->writeNull(4);			//after names (unknow size).


	size_t startColorAddress = listAura_address + aura_count * 4 * 4;
	for (size_t i = 0; i < aura_count; i++)
	{
		file->goToAddress(listAura_address + i * 4 * 4);
		mListAura.at(i).write(file, startColorAddress);
	}
	




	unsigned int type_address = file->getCurrentAddress();
	file->goToAddress(0x8 + 4 * 3);
	file->writeInt32E(&type_address);
	file->goToAddress(type_address);

	unsigned int addressTypeName = type_address + type_count * 4;
	for (size_t i = 0; i < type_count; i++)
	{
		file->goToAddress(type_address + i * 4);
		file->writeInt32E(&addressTypeName);

		file->goToAddress(addressTypeName);
		file->writeString(&(mListAuraType.at(i)));
		addressTypeName = file->getCurrentAddress();
	}
	file->fixPadding(4 * 4);




	unsigned int characAuraLink_address = file->getCurrentAddress();
	file->goToAddress(0x8 + 4 * 5);
	file->writeInt32E(&characAuraLink_address);
	file->goToAddress(characAuraLink_address);

	for (size_t i = 0; i < characAuraLink_count; i++)
	{
		file->goToAddress(characAuraLink_address + 4 * 4 * i);
		mListCharacLink.at(i).write(file);
	}
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void AURAura::write(File *file, size_t &startColorAddress)
{
	file->writeInt32E(&mId);
	file->writeInt32E(&mUnknow0);

	size_t tmp = mListColorByType.size();
	file->writeInt32E(&tmp);
	file->writeInt32E(&startColorAddress);

	file->goToAddress(startColorAddress);

	size_t nbColors = mListColorByType.size();
	for (size_t i = 0; i < nbColors; i++)
	{
		file->writeInt32E(&mListColorByType.at(i).mIdAuraType);
		file->writeInt32E(&mListColorByType.at(i).mIdEffect);
	}
	startColorAddress = file->getCurrentAddress();
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void AURCharac::write(File *file)
{
	file->writeInt32E(&mIdCharac);
	file->writeInt32E(&mCostumeId);
	file->writeInt32E(&mIdAura);
	size_t tmp = mGlare ? 1 : 0;
	file->writeInt32E(&tmp);
}










/*-------------------------------------------------------------------------------\
|                             readXML				                             |
\-------------------------------------------------------------------------------*/
bool AURAura::readXML(TiXmlElement* xmlCurrentNode, std::vector<string> &listAuraType)
{
	string str_tmp = "";
	xmlCurrentNode->QueryStringAttribute("id", &str_tmp);
	mId = parseHexaUnsignedInt((str_tmp.substr(0, 2) == "0x") ? str_tmp : str_tmp.substr(2));
	
	xmlCurrentNode->QueryStringAttribute("unknow_0", &str_tmp);
	mUnknow0 = parseHexaUnsignedInt((str_tmp.substr(0, 2) == "0x") ? str_tmp : str_tmp.substr(2));
	
	TiXmlElement* auraColorNode = xmlCurrentNode->FirstChildElement("Effects");
	if (!auraColorNode)
	{
		printf("don't have 'Effects' tags. skip.'\n");
		notifyError();
		return false;
	}


	size_t indexAuraType = 0;
	size_t color = 0;
	string tmp = "";
	size_t nbTypes = listAuraType.size();

	mListColorByType.clear();
	for (TiXmlElement* xmlNode = auraColorNode->FirstChildElement("Effect"); xmlNode; xmlNode = xmlNode->NextSiblingElement("Effect"))
	{
		indexAuraType = (size_t)-1;
		if (xmlNode->QueryStringAttribute("auraType", &tmp) == TIXML_SUCCESS)
		{
			
			for (size_t i = 0; i < nbTypes; i++)
			{
				if (listAuraType.at(i) == tmp)
				{
					indexAuraType = i;
					break;
				}
			}
		}
		if(indexAuraType==(size_t)-1)
			xmlNode->QueryUnsignedAttribute("indexAuraType", &indexAuraType);


		xmlNode->QueryStringAttribute("idEffect", &str_tmp);
		color = parseHexaUnsignedInt((str_tmp.substr(0, 2) == "0x") ? str_tmp : str_tmp.substr(2));

		mListColorByType.push_back(AURAuraEffect(indexAuraType, color));
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             readXML				                             |
\-------------------------------------------------------------------------------*/
bool AURCharac::readXML(TiXmlElement *xmlCurrentNode)
{
	string str_tmp = "";
	xmlCurrentNode->QueryStringAttribute("idCharac", &str_tmp);
	mIdCharac = parseHexaUnsignedInt((str_tmp.substr(0, 2) == "0x") ? str_tmp : str_tmp.substr(2));

	xmlCurrentNode->QueryUnsignedAttribute("idCostume", &mCostumeId);

	xmlCurrentNode->QueryStringAttribute("idAura", &str_tmp);
	mIdAura = parseHexaUnsignedInt((str_tmp.substr(0, 2) == "0x") ? str_tmp : str_tmp.substr(2));

	xmlCurrentNode->QueryStringAttribute("glare", &str_tmp);
	mGlare = ((str_tmp == "true") || (str_tmp=="1"));

	return true;
}




/*-------------------------------------------------------------------------------\
|                             writeXML				                             |
\-------------------------------------------------------------------------------*/
TiXmlElement* AURAura::writeXml(std::vector<string> &listAuraType)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("Aura");

	string tmp = "0x" + toStringHexa(mId);
	xmlCurrentNode->SetAttribute("id", tmp);

	tmp = "0x" + toStringHexa(mUnknow0);
	xmlCurrentNode->SetAttribute("unknow_0", tmp);

	TiXmlElement* auraColorNode = new TiXmlElement("Effects");
	TiXmlElement* xmlNode;
	size_t nbElements = mListColorByType.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		xmlNode = new TiXmlElement("Effect");

		if (mListColorByType.at(i).mIdAuraType<listAuraType.size())
			xmlNode->SetAttribute("auraType", listAuraType.at(mListColorByType.at(i).mIdAuraType));
		else
			xmlNode->SetAttribute("indexAuraType", mListColorByType.at(i).mIdAuraType);

		tmp = "0x" + toStringHexa(mListColorByType.at(i).mIdEffect);
		xmlNode->SetAttribute("idEffect", tmp);
		
		auraColorNode->LinkEndChild(xmlNode);
	}
	xmlCurrentNode->LinkEndChild(auraColorNode);

	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             writeXML				                             |
\-------------------------------------------------------------------------------*/
TiXmlElement* AURCharac::writeXml()
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("CharacLink");

	string tmp = "0x" + toStringHexa(mIdCharac);
	xmlCurrentNode->SetAttribute("idCharac", tmp);

	xmlCurrentNode->SetAttribute("idCostume", mCostumeId);

	tmp = "0x" + toStringHexa(mIdAura);
	xmlCurrentNode->SetAttribute("idAura", tmp);

	tmp = mGlare ? "true" : "false";
	xmlCurrentNode->SetAttribute("glare", tmp);

	return xmlCurrentNode;
}











}