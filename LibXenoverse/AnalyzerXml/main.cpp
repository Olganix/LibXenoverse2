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

		bool isBool;
		bool isHexa;
		bool isNumber;
		bool isFloatNumber;
		
		valueWithRef()
		{
			isBool = isHexa = isNumber = isFloatNumber = false;
		}

		void testCompatibily()
		{
			isBool = isHexa = isNumber = isFloatNumber = false;
			
			if (mValue_str.length() == 0)
				return;

			string value_lowercase = mValue_str;
			std::transform(value_lowercase.begin(), value_lowercase.end(), value_lowercase.begin(), ::tolower);

			isBool = ((value_lowercase == "true") || (value_lowercase == "false"));
			if (isBool)
				return;

			//isHexa = ((value_lowercase.length()>2) && (value_lowercase.substr(0, 2) == "0x") && (LibXenoverse::isIntNumber(value_lowercase.substr(2))) );
			isHexa = ((value_lowercase.length()>2) && (value_lowercase.substr(0, 2) == "0x"));	//todo do better
			if (isHexa)
			{
				isNumber = true;

				uint32_t test = LibXenoverse::parseHexaUnsignedInt(value_lowercase);
				float test_f = *(float*)(&test);
				
				if (!(test_f != test_f))		// To test NaN  https://stackoverflow.com/questions/570669/checking-if-a-double-or-float-is-nan-in-c
				{
					if (test_f < 0.0)
						test_f = -test_f;

					isFloatNumber =  ((test_f==0.0) || ((test_f>1.0e-30f) && (test_f<1.0e30f)) );
				}
				
				

			}else {
				isNumber = LibXenoverse::isNumber(value_lowercase);
				isFloatNumber = ((isNumber) && (value_lowercase.find(".") != std::string::npos));
			}
		}
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
			value.testCompatibily();

			mListValuesPossibilities.push_back(value);

		}else if (mType == "argument") {
			
			valueWithRef value;
			value.mValue_str = *(node->Attribute(mName));
			value.mOrigineFromFile = origineFromFile;
			value.testCompatibily();
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


		size_t limitOrigines = 30;

		std::vector<valueWithRef> mlistUniques;
		std::vector<size_t> mlistUniques_count;

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
			{
				mlistUniques.push_back(valueRef);
				mlistUniques_count.push_back(1);
			}
			else {
				mlistUniques_count.at(isfound) += 1;
				if(mlistUniques_count.at(isfound)<limitOrigines)			//that will speed up a little the process time.
					mlistUniques.at(isfound).mOrigineFromFile += ";" + valueRef.mOrigineFromFile;		//merge des references.
			}
		}


		//avoid too long string on Origine witch break the Analyzer.
		nbElements = mlistUniques.size();
		for (size_t i = 0; i < nbElements; i++)
		{
			if (mlistUniques_count.at(i) < limitOrigines)
				continue;
			
			size_t firstPart = mlistUniques.at(i).mOrigineFromFile.find(';');
			if (firstPart != std::string::npos)
				mlistUniques.at(i).mOrigineFromFile = mlistUniques.at(i).mOrigineFromFile.substr(0, firstPart + 1) + " ... x " + std::to_string(mlistUniques_count.at(i));
		}



		//search for the "Most"
		size_t most = (size_t)-1;
		size_t mostIndex = (size_t)-1;
		for (size_t i = 0; i < nbElements; i++)
		{
			if ((i == 0) || (mlistUniques_count.at(i) > most))
			{
				most = mlistUniques_count.at(i);
				mostIndex = i;
			}
		}
		string most_value_str = "lksqjdlkqjlsdjlqkjd akjzmalzkemalkdqjsdkbsvbishofdialsdnqlksjdlqkjw****";
		if (mostIndex != (size_t)-1)
			most_value_str = mlistUniques.at(mostIndex).mValue_str;




		//here, we just order by asc if number.
		struct TestToOrder
		{
			bool operator() (const valueWithRef & a, const valueWithRef & b) const
			{
				if ((!a.isNumber) && (!b.isNumber))
				{
					return (a.mValue_str.compare(b.mValue_str) < 0);
				}
				if ((!a.isNumber) && (b.isNumber))
					return false;
				if ((a.isNumber) && (!b.isNumber))
					return true;

				//so now we have 2 numbers. But it's could be different Types.

				double value_a = 0;
				if (a.isHexa)
				{
					uint32_t test = LibXenoverse::parseHexaUnsignedInt(a.mValue_str);
					float test_f = *(float*)(&test);
					value_a = ((a.isFloatNumber) ? (double)test_f : (double)test);
				}
				else {
					value_a = ((a.isFloatNumber) ? (double)stof(a.mValue_str) : (double)stoul(a.mValue_str));
				}

				double value_b = 0;
				if (b.isHexa)
				{
					uint32_t test = LibXenoverse::parseHexaUnsignedInt(b.mValue_str);
					float test_f = *(float*)(&test);
					value_b = ((b.isFloatNumber) ? (double)test_f : (double)test);
				}
				else {
					value_b = ((b.isFloatNumber) ? (double)stof(b.mValue_str) : (double)stoul(b.mValue_str));
				}

				return (value_a < value_b);
			}
		};
		std::sort(mlistUniques.begin(), mlistUniques.end(), TestToOrder());





		nbElements = mlistUniques.size();

		if (nbElements == 1)				//if we have only one value, we will simplify the view
		{
			string value_str = mlistUniques.at(0).mValue_str;

			//little adaptation to work faster
			bool allreadyModified = false;
			if (mlistUniques.at(0).isNumber)
			{
				//hypothese : previously the hexa format is privilegied on uint. So don't need to convert it on each number.

				if ((mlistUniques.at(0).isHexa) && (mlistUniques.at(0).isFloatNumber))			//if a Hexa could be converted into a float, we display it.
				{
					uint32_t test = LibXenoverse::parseHexaUnsignedInt(value_str);
					float test_f = *(float*)(&test);
					value_str += " = " + std::to_string(test_f);
					allreadyModified = true;
				}
			}

			return value_str;
		}

		string value_str = "";
		bool allreadyModified = false;
		for (size_t i = 0; i < nbElements; ++i)			//puis creation de la string.
		{
			value_str = mlistUniques.at(i).mValue_str;

			//little adaptation to work faster
			allreadyModified = false;
			if (mlistUniques.at(i).isNumber)
			{
				//hypothese : previously the hexa format is privilegied on uint. So don't need to convert it on each number.

				if ((mlistUniques.at(i).isHexa) && (mlistUniques.at(i).isFloatNumber) )			//if a Hexa could be converted into a float, we display it.
				{
					uint32_t test = LibXenoverse::parseHexaUnsignedInt(value_str);
					float test_f = *(float*)(&test);
					value_str += " = " + std::to_string(test_f);
					allreadyModified = true;
				}

				if (value_str == most_value_str)
					value_str += " (most) ";
			}
			
			str += ((str.length() != 0) ? "|" : "") + value_str + "[" + mlistUniques.at(i).mOrigineFromFile + "] ";
		}

		return str;
	}
};






/*-------------------------------------------------------------------------------\
|                             Main					                             |
\-------------------------------------------------------------------------------*/

int main(int argc, char** argv)
{
	printf("*******************************************************************\n\
 This tool is for Analyze a serie of Xml (or of tags in Xml), by create a xml with all merge of Tag/Attributes/values where it keep unique values, order them, and keep a reference to a file for sample the case.\n\
 Usage: 'Analyzer.exe [options] folder'\n\
 For each value the result will be : XXXX=\"value1[fileIndex1,tag1,tag2;fileIndex1,tag1,tag3;fileIndex2; ...x NumberReference]|value2[...]|value3[...]|...\" where fileIndex is the one for the list of filename at the end.\n\
 Options : '-NoWait', '-AlwaysWait', '-WaitOnError' (default), or '-WaitOnWarning'.\n\
 Notice: by doing a shortcut, you could use another option and keep drag and drop of files.\n\
 Notice: \"path With Spaces\" allowed now. \n\
*******************************************************************\n");

	std::vector<string> arguments = LibXenoverse::initApplication(argc, argv);

	if (arguments.size() == 0)
	{
		printf("Error not enougth arguments.\n");
		LibXenoverse::notifyError();
		LibXenoverse::waitOnEnd();
		return 1;
	}


	
	string folder = arguments.at(0);
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
		LibXenoverse::notifyError();
		LibXenoverse::waitOnEnd();
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

			LibXenoverse::notifyError();
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
	LibXenoverse::waitOnEnd();
	return 0;
}