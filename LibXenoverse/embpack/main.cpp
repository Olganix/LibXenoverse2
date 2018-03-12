#include "LibXenoverse.h"

int main(int argc, char** argv)
{
	printf("*******************************************************************\n\
 This tool is for Extract files from .emb files or repack.\n\
 Usage: 'embpack.exe [options] file.emb' or 'embpack.exe [options] folder'\n\
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

	if (extension == "emb")
	{
		LibXenoverse::EMB *emb_pack = new LibXenoverse::EMB();
		if (emb_pack->load(filename))
		{
			string name = LibXenoverse::nameFromFilenameNoExtension(filename, true);
			string folder = LibXenoverse::folderFromFilename(filename);
			string new_folder = folder + name;

			CreateDirectory(new_folder.c_str(), NULL);
			emb_pack->extract(new_folder + "/");	
		}
		delete emb_pack;
		
	}else if (extension == ""){

		LibXenoverse::EMB *emb_pack = new LibXenoverse::EMB();
		emb_pack->addFolder(filename + "/");
		emb_pack->save(filename + ".emb", emb_pack->detectFilenameMode());
		delete emb_pack;

	}else {
		printf("Error on arguments.\n");
		LibXenoverse::notifyError();
	}

	printf("finished.\n");
	LibXenoverse::waitOnEnd();
	return 0;
}