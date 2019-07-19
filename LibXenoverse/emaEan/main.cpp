#include "LibXenoverse.h"



int main(int argc, char** argv)
{
	
	printf("*******************************************************************\n\
 This tool is for convert animations files of Dbxv2 and SaintSeya : Ema <-> Ean.\n\
 Usage: 'emaEan.exe [options] file.ext'\n\
 Files could be .ema, or .ean .\n\
 Options : '-NoWait', '-AlwaysWait', '-WaitOnError' (default), or '-WaitOnWarning'.\n\
 Notice: by doing a shortcut, you could use another option and keep drag and drop of files.\n\
 Notice: \"path With Spaces\" allowed now. \n\
*******************************************************************\n");

	std::vector<string> arguments = LibXenoverse::initApplication(argc, argv);

	
	//options
	bool forceOrientationInterpolation = false;

	size_t nbArg = arguments.size();
	for (size_t i = 0; i < nbArg; i++)
	{
		if (arguments.at(i) == "-forceOrientationInterpolation")
		{
			arguments.erase(arguments.begin() + i);
			forceOrientationInterpolation = true;
			break;
		}
	}

	if (arguments.size() == 0)
	{
		printf("Error not enougth arguments.\n");
		LibXenoverse::notifyError();
		LibXenoverse::waitOnEnd();
		return 1;
	}

	

	printf("Converter Ema to Ean Started. please wait ...\n");


	string filename = arguments.at(0);
	string extension = LibXenoverse::extensionFromFilename(filename, true);

	if(extension == "ema")
	{
		LibXenoverse::EMA* ema = new LibXenoverse::EMA();
		if(ema->LoadFromFile(filename))
		{
			LibXenoverse::EAN* ean = new LibXenoverse::EAN();
			ema->writeEAN(ean);

			ean->save(filename + ".ean");
			//ema->DecompileToFile(filename + "_resave.ema.xml");		//test
			delete ean;
		}
		delete ema;

	}else if (extension == "ean"){

		LibXenoverse::EAN* ean = new LibXenoverse::EAN();
		if (ean->load(filename))
		{
			LibXenoverse::EMA* ema = new LibXenoverse::EMA();
			ema->readEAN(ean, forceOrientationInterpolation);

			ema->SaveToFile(filename + ".ema");
			//ema->DecompileToFile(filename + "_resave.ema.xml");		//test
			delete ema;
		}
		delete ean;

	}else {
		printf("Error on arguments.\n");
		LibXenoverse::notifyError();
	}

	printf("finished.\n");
	LibXenoverse::waitOnEnd();
	return 0;
}