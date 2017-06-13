#include "LibXenoverse.h"



int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: emaEan file.ema or file.ean.\n");
		getchar();
		return 1;
	}

	LibXenoverse::initializeDebuggingLog();

	printf("Converter Ema to Ean Started. please wait ...\n");

	
	
	string filename = ToString(argv[1]);
	string extension = LibXenoverse::extensionFromFilename(filename, true);

	if(extension == "ema")
	{
		LibXenoverse::EMA* ema = new LibXenoverse::EMA();
		if(ema->LoadFromFile(filename))
		{
			LibXenoverse::EAN* ean = new LibXenoverse::EAN();
			ema->writeEAN(ean);
			
			//ema->DecompileToFile(filename + "_resave.ema.xml");		//test

			ean->save(filename + ".ean");
			delete ean;
		}
		delete ema;

	}else if (extension == "ean"){

		LibXenoverse::EAN* ean = new LibXenoverse::EAN();
		if (ean->load(filename))
		{
			LibXenoverse::EMA* ema = new LibXenoverse::EMA();
			ema->readEAN(ean);
			ema->SaveToFile(filename + ".ema");

			//ema->DecompileToFile(filename + "_resave.ema.xml");		//test

			delete ema;
		}
		delete ean;
	}

	return 0;
}