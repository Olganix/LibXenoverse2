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
		if(nsk_pack->load(filename))
			nsk_pack->extract(folder);
		delete nsk_pack;

	}else {
		
		LibXenoverse::NSK *nsk_pack = new LibXenoverse::NSK();

		string parameter;
		string extension;
		for (size_t i = 0; i < nbArg; i++)
		{
			extension = LibXenoverse::extensionFromFilename(arguments.at(i));

			if (extension == "emd")
				nsk_pack->addEmdFile(arguments.at(i));
			else if (extension == "esk")
				nsk_pack->addEskFile(arguments.at(i));
		}

		nsk_pack->save(filename +".nsk");
	}


	printf("finished.\n");
	LibXenoverse::waitOnEnd();
	return 0;
}