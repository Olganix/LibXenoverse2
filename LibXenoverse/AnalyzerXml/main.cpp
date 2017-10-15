#include "LibXenoverse.h"
#include "qdir.h"





/*-------------------------------------------------------------------------------\
|                             XmlNode				                             |
\-------------------------------------------------------------------------------*/
// The goal of this class and this tool is to make a list of values for same tags.
// and make a analisys of that to find possibilities and try to understand what is what for.
//	for each value, we will only keep unique of them and display reference to come back to the original file.
//	So aa="1", will become aa="1[0]|2[1]|3[2;3;4]" where [0] is a reference (see mOrigineFromFile), and it could have many ref for the same value.
//	Same <aa>1</aa>, will become <aa>1[0]|2[1]|3[2;3;4]</aa>. (Notice all values are considered as String).

class XmlNode
{
public:

	struct valueWithRef										//we will merge all same values from differents file and list of child into each xml. (we will consider XPath without childIndex)
	{
		string mValue_str;									//value in string.
		string mOrigineFromFile;							//"f,i,j,k,..." f : index for file origine, i: index of first child list, j: index of second child list, ...		
	};
	
	string mName;
	string mType;											//doc, node, text, argument
	std::vector<valueWithRef> mListValuesPossibilities;		//all possibilities for value

	std::vector<XmlNode> mListChild;




public:
	XmlNode(string name, string type)
	{
		mName = name;
		mType = type;
	}
	virtual ~XmlNode() { }


	/*-------------------------------------------------------------------------------\
	|                             merge					                             |
	\-------------------------------------------------------------------------------*/
	void merge(TiXmlElement *node, string origineFromFile)
	{

		if (mType == "text")
		{
			valueWithRef value;
			value.mValue_str = ((TiXmlText*)node)->ValueStr();
			value.mOrigineFromFile = origineFromFile;

			mListValuesPossibilities.push_back(value);

		}else if (mType == "argument") {
			
			valueWithRef value;
			value.mValue_str = *(node->Attribute(mName));
			value.mOrigineFromFile = origineFromFile;
			mListValuesPossibilities.push_back(value);
			return;
		}



		size_t nbChilds;
		size_t isfound;
		string name, type;
		size_t inc = 0;
		bool pass;

		for (TiXmlElement* elem = node->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			name = elem->ValueStr();

			pass = false;
			switch (elem->Type())
			{
			case TiXmlElement::TINYXML_ELEMENT: type = "node"; break;
			case TiXmlElement::TINYXML_TEXT: type = "text"; break;
			default:  pass = true;
			}

			if (pass)
				continue;

			isfound = (size_t)-1;
			nbChilds = mListChild.size();
			for (size_t i = 0; i < nbChilds; i++)
			{
				if ((mListChild.at(i).mName == name) && (mListChild.at(i).mType == type))
				{
					isfound = i;
					break;
				}
			}
			if (isfound == (size_t)-1)
			{
				isfound = nbChilds;
				mListChild.push_back(XmlNode(name, type));
			}

			mListChild.at(isfound).merge(elem, origineFromFile +","+ std::to_string(inc++));
		}


		if (mType == "doc")						//no argument in document.
			return;


		//same for argument.
		for (TiXmlAttribute* elem = node->FirstAttribute(); elem != NULL; elem = elem->Next())
		{
			name = elem->Name();

			isfound = (size_t)-1;
			nbChilds = mListChild.size();
			for (size_t i = 0; i < nbChilds; i++)
			{
				if ((mListChild.at(i).mName == name) && (mListChild.at(i).mType == "argument"))
				{
					isfound = i;
					break;
				}
			}
			if (isfound == (size_t)-1)
			{
				isfound = nbChilds;
				mListChild.push_back(XmlNode(name, "argument"));
			}

			mListChild.at(isfound).merge(node, origineFromFile);
		}
	}



	/*-------------------------------------------------------------------------------\
	|                             exportAnalyze			                             |
	\-------------------------------------------------------------------------------*/
	TiXmlElement* exportAnalyze(TiXmlElement *parent = 0)
	{
		TiXmlElement* node = 0;
	

		if (mType == "doc")
		{
			node = (TiXmlElement*)(new TiXmlDocument(mName));

		}else if (mType == "node") {
			node = new TiXmlElement(mName);

		}else if (mType == "text") {
			node = (TiXmlElement*)(new TiXmlText(mName));
			node->SetValue(getValues());

		}else if(mType == "argument"){
			parent->SetAttribute(mName, getValues());
			return 0;
		}

		size_t nbChild = mListChild.size();
		for (size_t i = 0; i < nbChild; ++i)
			mListChild.at(i).exportAnalyze(node);

		if (mType != "doc")
			parent->LinkEndChild(node);

		return node;
	}


	/*-------------------------------------------------------------------------------\
	|                             getValues				                             |
	\-------------------------------------------------------------------------------*/
	//	So aa="1", will become aa="1[0]|2[1]|3[2;3;4]" where [0] is a reference (see mOrigineFromFile), and it could have many ref for the same value.
	//	Same <aa>1</aa>, will become <aa>1[0]|2[1]|3[2;3;4]</aa>. (Notice all values are considered as String).
	string getValues()
	{
		string str = "";
		
		std::vector<valueWithRef> mlistUniques;

		size_t nbElements = mListValuesPossibilities.size();
		for (size_t i = 0; i < nbElements; ++i)			//premiere chose a faire c'est de merger les elements, pour n'avoir que les uniques.
		{
			valueWithRef &valueRef = mListValuesPossibilities.at(i);

			size_t isfound = (size_t)-1;
			size_t nbUniques = mlistUniques.size();
			for (size_t j = 0; j < nbUniques; j++)
			{
				if (mlistUniques.at(j).mValue_str == valueRef.mValue_str)
				{
					isfound = j;
					break;
				}
			}

			if (isfound == (size_t)-1)
				mlistUniques.push_back(valueRef);
			else
				mlistUniques.at(isfound).mOrigineFromFile += ";" + valueRef.mOrigineFromFile;		//merge des references.
		}

		nbElements = mlistUniques.size();

		if (nbElements == 1)				//if we have only one value, we will simplify the view
			return mlistUniques.at(0).mValue_str;

		for (size_t i = 0; i < nbElements; ++i)			//puis creation de la string.
			str += ((str.length() != 0) ? "|" : "") + mlistUniques.at(i).mValue_str + "[" + mlistUniques.at(i).mOrigineFromFile +"] ";

		return str;
	}
};








/*-------------------------------------------------------------------------------\
|                             Main					                             |
\-------------------------------------------------------------------------------*/

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: 'Analyzer.exe folder ...'\n");
		getchar();
		return 1;
	}

	string folder = ToString(argv[1]);
	std::vector<string> listFilename;
	
	{
		QString folderPath = (QString(folder.c_str()));
		QDir src_folder = QDir(folderPath);
		QFileInfoList files = src_folder.entryInfoList();
		foreach(QFileInfo file, files)
		{
			string filename_tmp = file.fileName().toStdString();
			if ((filename_tmp == "") || (filename_tmp == ".") || (filename_tmp == "..") || (file.isDir()))
			{

			}else {

				string filename_abs_tmp = file.absolutePath().toStdString();
				listFilename.push_back(string(filename_abs_tmp + "/" + filename_tmp));
			}
		}
	}

	size_t nbFiles = listFilename.size();
	if (nbFiles == 0)
	{
		printf("no file found in folder.'\n");
		return 0;
	}

	XmlNode xmlNode = XmlNode(LibXenoverse::folderFromFilename(listFilename.at(0)) + "\\AnalyzerXml.xml", "doc");

	std::vector<string> listValidFilenames;
	for (size_t i = 0; i < nbFiles; i++)
	{
		string filename = listFilename.at(i);
		string extension = LibXenoverse::extensionFromFilename(filename, true);
		string basefilename = filename.substr(0, filename.length() - (extension.size() + 1));

		if (extension != "xml")
			continue;
		if (filename.substr(filename.length() - 11) == "AnalyzerXml.xml")
			continue;

		printf("Try to load \"%s\"\n", filename.c_str());

		TiXmlDocument doc;

		if (!doc.LoadFile(filename))
		{
			if (doc.ErrorId() == TiXmlBase::TIXML_ERROR_OPENING_FILE)
				printf("Cannot open file \"%s\"\n", filename.c_str());
			else
				printf("Error parsing file \"%s\". This is what tinyxml has to say: %s. Row=%d, col=%d.\n", filename.c_str(), doc.ErrorDesc(), doc.ErrorRow(), doc.ErrorCol());

			continue;
		}


		printf("processing...(please wait)\n");
		xmlNode.merge((TiXmlElement*)(&doc), std::to_string(listValidFilenames.size()));
		listValidFilenames.push_back(filename);
	}


	printf("Build AnalyzerXml.xml ...(please wait)\n");
	TiXmlDocument* analyzerDoc = (TiXmlDocument*)xmlNode.exportAnalyze();
	
	TiXmlElement* node_listFile = new TiXmlElement("ListFile");
	nbFiles = listValidFilenames.size();
	for (size_t i = 0; i < nbFiles; i++)											//add the list of Files to do the link with origine.
	{
		TiXmlElement* node = new TiXmlElement("File");
		node->SetAttribute("id", i);
		node->SetAttribute("name", listValidFilenames.at(i));
		node_listFile->LinkEndChild(node);
	}
	analyzerDoc->LinkEndChild(node_listFile);

	analyzerDoc->SaveFile(xmlNode.mName);
	delete analyzerDoc;
	
	printf("finished.\n");

	return 0;
}