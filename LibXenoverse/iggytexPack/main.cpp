#include "LibXenoverse.h"

int main(int argc, char** argv)
{
	printf("*******************************************************************\n\
 This tool is for extract the files from iggy texture, and repack.\n\
 Usage: 'iggytexPack.exe [options] file.ext' or 'iggytexPack.exe [options] folder'\n\
 Files could be .iggytex or .iggyted (same format , they just rename extension).\n\
 Options : '-iggyted' '-NoWait', '-AlwaysWait', '-WaitOnError' (default), or '-WaitOnWarning'.\n\
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
	

	
	//options
	bool renameExt_iggyted = false;

	size_t nbArg = arguments.size();
	for (size_t i = 0; i < nbArg; i++)
	{
		if (arguments.at(i) == "-iggyted")
		{
			arguments.erase(arguments.begin() + i);
			nbArg--;
			renameExt_iggyted = true;
			break;
		}
	}


	string filename = arguments.at(0);
	string extension = LibXenoverse::extensionFromFilename(filename, true);
	string basefilename = filename.substr(0, filename.length() - (extension.size() + 1));

	if((extension == "iggytex")|| (extension == "iggyted"))
	{
		LibXenoverse::IGGYTEX *iggytext_pack = new LibXenoverse::IGGYTEX();
		if (iggytext_pack->load(filename))
		{
			CreateDirectory(basefilename.c_str(), NULL);
			iggytext_pack->extract(basefilename + "/");
		}else {
			printf("Fail to load the file.\n");
			LibXenoverse::notifyError();
		}
		delete iggytext_pack;

	}else if(extension == "") {
		LibXenoverse::IGGYTEX *iggytext_pack = new LibXenoverse::IGGYTEX();
		iggytext_pack->addFolder(filename + "/");
		iggytext_pack->save(filename + (renameExt_iggyted ? ".iggyted" : ".iggytex"), iggytext_pack->detectFilenameMode());
		delete iggytext_pack;

	}else {
		printf("Error on arguments.\n");
		LibXenoverse::notifyError();
	}


	printf("finished.\n");
	LibXenoverse::waitOnEnd();
	return 0;
}