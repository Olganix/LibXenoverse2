#include "LibXenoverse.h"

int main(int argc, char** argv)
{
	printf("*******************************************************************\n\
 This tool is for Extract files from .uexp files (with BNDFLL tag) or repack.\n\
 Usage: 'UassetUexpPack.exe [options] file.uexp' or 'UassetUexpPack.exe [options] folder'\n\
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

	string filename = arguments.at(0);
	string extension = LibXenoverse::extensionFromFilename(filename, true);
	string baseFilename = LibXenoverse::filenameNoExtension(filename);

	if ( (extension == "uexp") || (extension == "uasset") )
	{

		LibXenoverse::Uexp* uexp_pack = new LibXenoverse::Uexp();
		if (uexp_pack->load(baseFilename + ".uexp"))
		{
			string name = LibXenoverse::nameFromFilenameNoExtension(filename, true);
			string folder = LibXenoverse::folderFromFilename(filename);
			string new_folder = folder + name;

			CreateDirectory(new_folder.c_str(), NULL);
			uexp_pack->extract(new_folder + "/");
		}
		delete uexp_pack;
		
	}else{
		LibXenoverse::Uexp* uexp_pack = new LibXenoverse::Uexp();
		uexp_pack->addFolder(filename + "/");
		uexp_pack->save(filename + ".uexp");
		uexp_pack->updateUassetFile(filename + ".uasset");			//modification about uexp size.

		delete uexp_pack;
	}

	printf("finished.\n");
	LibXenoverse::waitOnEnd();
	return 0;
}