#include "LibXenoverse.h"

int main(int argc, char** argv)
{
	printf("*******************************************************************\n\
 This tool is for convert nsk <-> emd + esk.\n\
 Usage: 'nskpack.exe [options] file.nsk' or 'nskpack.exe [options] file.esk file.emd file2.emd ...'\n\
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


	size_t nbArg = arguments.size();
	for (size_t i = 0; i <nbArg; i++)										//we need to have esk first.
	{
		if (LibXenoverse::extensionFromFilename(arguments.at(i), true) == "esk")
		{
			arguments.insert(arguments.begin(), arguments.at(i));
			arguments.erase(arguments.begin() + i + 1);
			break;
		}
	}


	string filename = arguments.at(0);
	string extension = LibXenoverse::extensionFromFilename(filename, true);
	string basefilename = filename.substr(0, filename.length() - (extension.size() + 1));
	string folder = LibXenoverse::folderFromFilename(filename);

	if (extension == "nsk")
	{
		LibXenoverse::NSK *nsk_pack = new LibXenoverse::NSK();
		nsk_pack->direct_extract(filename);
		delete nsk_pack;

	}else if(nbArg==2) {
		
		string esk_filename = "";
		string emd_filename = "";

		for (size_t i = 0; i < nbArg; i++)
		{
			extension = LibXenoverse::extensionFromFilename(arguments.at(i));
			if (extension == "esk")
				esk_filename = arguments.at(i);
			else if (extension == "emd")
				emd_filename = arguments.at(i);
		}

		if ((esk_filename.length() != 0) && (emd_filename.length() != 0))
		{
			LibXenoverse::NSK *nsk_pack = new LibXenoverse::NSK();

			nsk_pack->direct_import(esk_filename, emd_filename);

			delete nsk_pack;
		}
	}


	printf("finished.\n");
	LibXenoverse::waitOnEnd();
	return 0;
}