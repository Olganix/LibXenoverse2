#include "LibXenoverse.h"

int main(int argc, char** argv) {
	
	printf("*******************************************************************\n\
 This tool is for convert vertex or pixel binary (.xvu/.xpu) to assembly (.asm, with assembly instructions), and reverse.\n\
 Usage: 'hlslasm.exe [options] file.ext'\n\
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

	if ((extension == "xpu") || (extension == "xvu"))
	{
		size_t data_size = 0;
		char *data = LibXenoverse::HLSLASM::disassembleFromFile(filename, data_size);
		if (data)
		{
			LibXenoverse::writeTextTo(filename + ".asm", data, data_size);
			delete data;
		}else {
			printf("disassemble fail\n");
			LibXenoverse::notifyError();
			LibXenoverse::waitOnEnd();
		}

	}else if (extension == "asm") {
		size_t data_size = 0;
		char *data = LibXenoverse::HLSLASM::assembleFromFile(filename, data_size);
		if (data)
		{
			LibXenoverse::writeBytesTo(basefilename, data, data_size);
			delete data;
		} else {
			printf("assemble fail\n");
			LibXenoverse::notifyError();
			LibXenoverse::waitOnEnd();
		}

	}else {
		printf("Error on arguments.\n");
		LibXenoverse::notifyError();
	}


	printf("finished.\n");
	LibXenoverse::waitOnEnd();
	return 0;
}