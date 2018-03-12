#include "LibXenoverse.h"

int main(int argc, char** argv) {
	
	printf("*******************************************************************\n\
 This tool is for extract files from .emz files.\n\
 Usage: 'emzpack.exe [options] file.emz' or 'emzpack.exe [options] folder'\n\
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
	string basefilename = filename.substr(0, filename.length() - (extension.size() + 1));
	string name = LibXenoverse::nameFromFilenameNoExtension(filename, true);

	if (extension == "emz")
	{
		LibXenoverse::EMZ *emz_pack = new LibXenoverse::EMZ();
		emz_pack->load(filename);
		emz_pack->saveUncompressed(basefilename + emz_pack->detectNewExtension());
		delete emz_pack;

	}else{

		size_t data_size = 0;
		char *data = LibXenoverse::getBytesFrom(filename, data_size);

		if (data)
		{
			LibXenoverse::EMZ *emz_pack = new LibXenoverse::EMZ();
			emz_pack->setDataPtr((unsigned char *)data, data_size);
			emz_pack->save(basefilename + ".emz");
			delete emz_pack;
		}else {
			printf("Error on loading %s", filename);
			LibXenoverse::notifyError();
		}
	}

	printf("finished.\n");
	LibXenoverse::waitOnEnd();
	return 0;
}