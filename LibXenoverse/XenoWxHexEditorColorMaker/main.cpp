#include "LibXenoverse.h"

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: 'XenoWxHexEditorColorMaker.exe [-noEndWait] FILE.EXT FILE2.EXT ...' or 'XenoXmlConverter.exe FILE.EXT.xml FILE2.EXT.xml ...'\nFor Now, we only support map files");
		getchar();
		return 1;
	}

	LibXenoverse::initializeDebuggingLog();

	bool endWaitEnable = true;
	for (int i = 1; i < argc; i++)
	{
		string filename = ToString(argv[i]);
		
		if (filename == "-noEndWait")
		{
			endWaitEnable = false;
			continue;
		}
		
		string extension = LibXenoverse::extensionFromFilename(filename, true);
		string basefilename = filename.substr(0, filename.length() - (extension.size() + 1)) ;
		string extension2 = LibXenoverse::extensionFromFilename(basefilename, true);
		string basefilename2 = basefilename.substr(0, basefilename.length() - (extension2.size() + 1));
		string extension3 = LibXenoverse::extensionFromFilename(basefilename2, true);
		string basefilename3 = basefilename2.substr(0, basefilename2.length() - (extension3.size() + 1));

	

		/////////////////////////////////////////////////////////////
		if (extension == "map")
		{
			LibXenoverse::FmpFile* fmp = new LibXenoverse::FmpFile();
			fmp->save_Coloration(filename);
		}


	}

	printf("finished.\n");
	if(endWaitEnable)
		getchar();


	return 0;
}