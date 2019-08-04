#ifndef LIBXENOVERSE_BIN_COLOR_TAG_H_INCLUDED
#define LIBXENOVERSE_BIN_COLOR_TAG_H_INCLUDED

#include "EMO_BaseFile.h"




//vertex struct depend of flags.


namespace LibXenoverse
{

/*-------------------------------------------------------------------------------\
|                             BinColorTag			                             |
\-------------------------------------------------------------------------------*/
class BinColorTag
{
protected:
	TiXmlDocument* mDoc;
	TiXmlElement* mRoot;
	TiXmlElement* mFilename_node;

	std::vector<std::vector<std::vector<string>>> listTagColors;	// by Section, param, and font/background.

public:
	BinColorTag();
	~BinColorTag(void) { clean(); }
	
	//Debug/work: create a file for wxHexEditor, for add tag and color on section of the file. also detect if a file still have empty part (but you better have to ouput in a log file)
	TiXmlElement* startColoration(string filename);
	void endColoration(string filename);
	void clean();

	void write_Coloration_Tag(string paramName, string paramType, string paramComment, size_t startOffset, size_t size, string sectionName, TiXmlElement* parent, size_t idTag, size_t sectionIndex, size_t paramIndex, std::vector<bool> &listBytesAllreadyTagged, size_t sectionIndexInList = (size_t)-1, bool checkAllreadyTaggued = true);
	void write_Coloration_Gaps(size_t startOffsetToConsidere, size_t endOffsetToConsidere, size_t sizeFilter, size_t start_idTag, string sectionName, TiXmlElement* parent, std::vector<bool> &listBytesAllreadyTagged);
};

}

#endif