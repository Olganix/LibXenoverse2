#include "LibXenoverse.h"

int main(int argc, char** argv)
{
	if (argc < 2) {
		printf("Usage: nskpack file.nsk or nskpack file.esk file0.emd file1.emd ...\n");
		getchar();
		return 1;
	}

	LibXenoverse::initializeDebuggingLog();

	string pack_name = ToString(argv[1]);

	if (pack_name.find(".nsk") != string::npos)
	{
		LibXenoverse::NSK *nsk_pack = new LibXenoverse::NSK();
		nsk_pack->load(pack_name);

		string name = LibXenoverse::nameFromFilenameNoExtension(pack_name, true);
		string folder = LibXenoverse::folderFromFilename(pack_name);
		
		nsk_pack->extract(folder);
		delete nsk_pack;

	}else {
		
		LibXenoverse::NSK *nsk_pack = new LibXenoverse::NSK();

		string parameter;
		string extension;
		for (int i = 1; i < argc; i++)
		{
			parameter = ToString(argv[i]);
			extension = LibXenoverse::extensionFromFilename(parameter);

			if (extension == "emd")
				nsk_pack->addEmdFile(parameter);
			if (extension == "esk")
				nsk_pack->addEskFile(parameter);
		}

		nsk_pack->save(pack_name +".nsk");
	}

	return 0;
}