#include "LibXenoverse.h"

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: iggytexPack file\n");
		getchar();
		return 1;
	}

	LibXenoverse::initializeDebuggingLog();

	string pack_name = ToString(argv[1]);

	if((pack_name.find(".iggytex") != string::npos)|| (pack_name.find(".iggyted") != string::npos))
	{
		LibXenoverse::IGGYTEX *iggytext_pack = new LibXenoverse::IGGYTEX();
		iggytext_pack->load(pack_name);
		string name = LibXenoverse::nameFromFilenameNoExtension(pack_name, true);
		string folder = LibXenoverse::folderFromFilename(pack_name);

		string new_folder = folder + name;
		CreateDirectory(new_folder.c_str(), NULL);
		iggytext_pack->extract(new_folder + "/");
		delete iggytext_pack;

	}else {

		LibXenoverse::IGGYTEX *iggytext_pack = new LibXenoverse::IGGYTEX();
		iggytext_pack->addFolder(pack_name + "/");
		iggytext_pack->save(pack_name + ".iggytex", iggytext_pack->detectFilenameMode());
		delete iggytext_pack;
	}

	return 0;
}