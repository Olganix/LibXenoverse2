

#include "BinColorTag.h"


namespace LibXenoverse
{


/*-------------------------------------------------------------------------------\
|                             BinColorTag										 |
\-------------------------------------------------------------------------------*/
BinColorTag::BinColorTag()
{
	mDoc = 0;
	clean();
	
	listTagColors.clear();

	//section 0 - FMPHeader : black/grey
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#232323");			//background color
	listTagColors.back().back().push_back("#FFFFFF");			//font color (just to possibility read from background color)
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#464646");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 1 - Section0a : red
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC0000");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC7777");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 2 - Section0b : green
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#00CC00");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#77CC77");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 3 - Section0b_names : green
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#009900");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#339933");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 4 - Section1 : bleu
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#0000CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7777CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 5 - Section2 : yellow
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCC00");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCC77");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 6 - Section3 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#00CCCC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#77CCCC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 7 - Section3 indexes : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#009999");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#339999");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 8 - Section4 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC00CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC77CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 9 - Section4_Next : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#990099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#993399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f



	//section 10 - Section4_Next_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 11 - Section4_Next_c : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC0099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC3399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 12 - Section4_Next_d : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900AA");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933AA");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 13 - Section4_Next_e : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA0099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA3399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f



	//section 14 - Section4_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#770077");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#773377");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 15 - Section4_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#550055");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#553355");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f





	//section 16 - Section4_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA00FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA33FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 17 - Section4_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 18 - Section4_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7700FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7733FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 19 - Section4_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#5500FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#5533FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f




	//section 20 - Section5 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCCCC");			//bg
	listTagColors.back().back().push_back("#000000");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AAAAAA");			//bg
	listTagColors.back().back().push_back("#000000");			//f


	//section 21 - Section5_Box : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#999999");			//bg
	listTagColors.back().back().push_back("#000000");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#777777");			//bg
	listTagColors.back().back().push_back("#000000");			//f

}

/*-------------------------------------------------------------------------------\
|                             startColoration				                    |
\-------------------------------------------------------------------------------*/
TiXmlElement* BinColorTag::startColoration(string filename)
{
	clean();

	mDoc = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	mDoc->LinkEndChild(decl);

	mRoot = new TiXmlElement("wxHexEditor_XML_TAG");

	mFilename_node = new TiXmlElement("filename");
	EMO_BaseFile::WriteParamString(mFilename_node, "path", filename);

	return mFilename_node;
}
/*-------------------------------------------------------------------------------\
|                             endColoration					                     |
\-------------------------------------------------------------------------------*/
void BinColorTag::endColoration(string filename)
{
	mRoot->LinkEndChild(mFilename_node);
	mDoc->LinkEndChild(mRoot);

	mDoc->SaveFile(filename + ".tags");

	clean();
}
/*-------------------------------------------------------------------------------\
|                             clean							                     |
\-------------------------------------------------------------------------------*/
void BinColorTag::clean()
{
	if (mDoc)
		delete mDoc;
	
	mDoc = 0;
	mRoot = 0;
	mFilename_node = 0;
}
/*-------------------------------------------------------------------------------\
|                             write_Coloration_Tag			                     |
\-------------------------------------------------------------------------------*/
void BinColorTag::write_Coloration_Tag(string paramName, string paramType, string paramComment, size_t startOffset, size_t size, string sectionName, TiXmlElement* parent, size_t idTag, size_t sectionIndex, size_t paramIndex, std::vector<bool> &listBytesAllreadyTagged, size_t sectionIndexInList, bool checkAllreadyTaggued)
{
	TiXmlElement* tag_node = new TiXmlElement("TAG");

	tag_node->SetAttribute("id", EMO_BaseFile::UnsignedToString(idTag, false));


	TiXmlElement* start_offset_node = new TiXmlElement("start_offset");
	TiXmlText* text = new TiXmlText(std::to_string(startOffset));
	start_offset_node->LinkEndChild(text);
	tag_node->LinkEndChild(start_offset_node);

	TiXmlElement* end_offset_node = new TiXmlElement("end_offset");
	text = new TiXmlText(std::to_string(startOffset + size - 1));
	end_offset_node->LinkEndChild(text);
	tag_node->LinkEndChild(end_offset_node);

	TiXmlElement* text_node = new TiXmlElement("tag_text");
	text = new TiXmlText(sectionName + ((sectionIndexInList != (size_t)-1) ? "[" + std::to_string(sectionIndexInList) + "]" : "") + "." + paramName + " (" + paramType + ") : " + paramComment);
	text_node->LinkEndChild(text);
	tag_node->LinkEndChild(text_node);





	if (listTagColors.size() == 0)
	{
		listTagColors.push_back(std::vector<std::vector<string>>());
		listTagColors.back().push_back(std::vector<string>());
		listTagColors.back().back().push_back("#000000");			//background color
		listTagColors.back().back().push_back("#FFFFFF");			//font color (just to possibility read from background color)
	}

	size_t sectionIndex_tmp = sectionIndex % listTagColors.size();
	std::vector<std::vector<string>> &sectionColorlist = listTagColors.at(sectionIndex_tmp);
	size_t paramIndex_tmp = paramIndex % sectionColorlist.size();
	std::vector<string> &paramColors = sectionColorlist.at(paramIndex_tmp);

	TiXmlElement* font_colour_node = new TiXmlElement("font_colour");
	text = new TiXmlText(paramColors.at(1));
	font_colour_node->LinkEndChild(text);
	tag_node->LinkEndChild(font_colour_node);

	TiXmlElement* bg_colour_node = new TiXmlElement("note_colour");
	text = new TiXmlText(paramColors.at(0));
	bg_colour_node->LinkEndChild(text);
	tag_node->LinkEndChild(bg_colour_node);






	//un check d'overide , pour savoir si des blocks se chevauche.
	size_t index;
	string error_str = "";
	size_t limit = listBytesAllreadyTagged.size();
	for (size_t i = 0; i < size; i++)
	{
		index = startOffset + i;
		if (index >= limit)
		{
			printf("Error on tagID %i : overflow %s >= %s.\n", idTag, EMO_BaseFile::UnsignedToString(index, true).c_str(), EMO_BaseFile::UnsignedToString(limit, true).c_str());
			error_str += "Error on tagID " + ToString(idTag) + " : overflow " + EMO_BaseFile::UnsignedToString(index, true) + " up of " + EMO_BaseFile::UnsignedToString(limit, true) + " | ";
			LibXenoverse::notifyError();
			continue;
		}

		if ((checkAllreadyTaggued) && (listBytesAllreadyTagged.at(index)))
		{
			printf("warning on tagID %i : the byte %s allready taggued, may be a overflow between blocks. Infos : %s. \n", idTag, EMO_BaseFile::UnsignedToString(index, true).c_str(), (sectionName + ((sectionIndexInList != (size_t)-1) ? "[" + std::to_string(sectionIndexInList) + "]" : "") + "." + paramName + " (" + paramType + ") : " + paramComment).c_str());
			error_str += "warning on tagID " + ToString(idTag) + " : the byte "+ EMO_BaseFile::UnsignedToString(index, true) +" allready taggued, may be a overflow between blocks. | ";
			LibXenoverse::notifyError();
		}

		listBytesAllreadyTagged.at(index) = true;
	}

	if (error_str.length() != 0)
	{
		TiXmlElement* error_node = new TiXmlElement("Error");
		text = new TiXmlText(error_str);
		error_node->LinkEndChild(text);
		tag_node->LinkEndChild(error_node);
	}

	parent->LinkEndChild(tag_node);
}




/*-------------------------------------------------------------------------------\
|                             write_Coloration_Gaps			                     |
\-------------------------------------------------------------------------------*/
void BinColorTag::write_Coloration_Gaps(size_t startOffsetToConsidere, size_t endOffsetToConsidere, size_t sizeFilter, size_t start_idTag, string sectionName, TiXmlElement* parent, std::vector<bool> &listBytesAllreadyTagged)
{
	size_t nbBytes = listBytesAllreadyTagged.size();
	if ((nbBytes == 0) || (startOffsetToConsidere >= nbBytes) || (startOffsetToConsidere > endOffsetToConsidere))
		return;

	if (endOffsetToConsidere >= nbBytes)
		endOffsetToConsidere = nbBytes - 1;


	size_t sectionStart = (size_t)-1;
	size_t inc = 0;
	bool isByteTagged = false;
	for (size_t i = startOffsetToConsidere; i <= endOffsetToConsidere; i++)			//detection of the Hole
	{
		isByteTagged = listBytesAllreadyTagged.at(i);

		if ((!isByteTagged) && (sectionStart == (size_t)-1))
		{
			sectionStart = i;

		}
		else if (((isByteTagged) || (i == endOffsetToConsidere)) && (sectionStart != (size_t)-1)) {		//if end of Section, or end of range.

			size_t size = i - sectionStart;

			if (size > sizeFilter)
			{
				TiXmlElement* tag_node = new TiXmlElement("TAG");

				tag_node->SetAttribute("id", EMO_BaseFile::UnsignedToString(start_idTag++, false));


				TiXmlElement* start_offset_node = new TiXmlElement("start_offset");
				TiXmlText* text = new TiXmlText(std::to_string(sectionStart));
				start_offset_node->LinkEndChild(text);
				tag_node->LinkEndChild(start_offset_node);

				start_offset_node = new TiXmlElement("start_offset_Hexa");
				text = new TiXmlText(EMO_BaseFile::UnsignedToString(sectionStart, true));
				start_offset_node->LinkEndChild(text);
				tag_node->LinkEndChild(start_offset_node);

				TiXmlElement* end_offset_node = new TiXmlElement("end_offset");
				text = new TiXmlText(std::to_string(sectionStart + size - 1));
				end_offset_node->LinkEndChild(text);
				tag_node->LinkEndChild(end_offset_node);

				TiXmlElement* text_node = new TiXmlElement("tag_text");
				text = new TiXmlText(sectionName + "[" + std::to_string(inc++) + "]. Size=" + std::to_string(size));
				text_node->LinkEndChild(text);
				tag_node->LinkEndChild(text_node);

				TiXmlElement* font_colour_node = new TiXmlElement("font_colour");
				text = new TiXmlText("#000000");
				font_colour_node->LinkEndChild(text);
				tag_node->LinkEndChild(font_colour_node);

				TiXmlElement* bg_colour_node = new TiXmlElement("note_colour");
				text = new TiXmlText("#FF0000");
				bg_colour_node->LinkEndChild(text);
				tag_node->LinkEndChild(bg_colour_node);

				parent->LinkEndChild(tag_node);
			}

			sectionStart = (size_t)-1;
		}
	}
}





}