#include "LibXenoverse.h"

int main(int argc, char** argv)
{
	printf("*******************************************************************\n\
 This tool is for extract #EGM part of particle files (.emp) to make a mesh .emd, and repack by matching names.\n\
 Usage: 'empEmd.exe [options] file.emp' or 'empEmd.exe [options] file.emd file.emp' \n\
 Notice : only certain .emp files have a mesh inside. See tutorial on emp for more informations.\n\
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
	for (size_t i = 0; i <nbArg; i++)										//we need to have emd first.
	{
		if (LibXenoverse::extensionFromFilename(arguments.at(i), true) == "emd")
		{
			arguments.insert(arguments.begin(), arguments.at(i));
			arguments.erase(arguments.begin() + i + 1);
			break;
		}
	}

	string filename = arguments.at(0);
	string extension = LibXenoverse::extensionFromFilename(filename, true);
	string basefilename = filename.substr(0, filename.length() - (extension.size() + 1));

	string filename2 = "";
	string extension2 = "";
	string basefilename2 = "";
	if (arguments.size() > 1)
	{
		filename2 = arguments.at(1);
		extension2 = LibXenoverse::extensionFromFilename(filename2, true);
		basefilename2 = filename2.substr(0, filename2.length() - (extension2.size() + 1));
	}



	if (extension == "emp")			// don't have emd => so it's the EMG's extract operation
	{
		/*
		LibXenoverse::EMP* emp = new LibXenoverse::EMP();				//not finish yet, TODO this.
		emp->load(pack_name);
		*/
		LibXenoverse::EMP* emp = new LibXenoverse::EMP();
		LibXenoverse::EMD* emd = new LibXenoverse::EMD();

		emp->extractEmd(filename, emd);
		if (emd->getNumberModel() != 0)
		{
			emd->save(basefilename + ".emd");
		}else {
			printf("No #EGM found in this emp.\n");
			LibXenoverse::notifyError();
		}
		delete emp;
		delete emd;




	}else if (extension == "emd") {					//there is a Emd => So it's the replace of EMG operation

		LibXenoverse::EMP* emp = new LibXenoverse::EMP();
		LibXenoverse::EMD *emd = new LibXenoverse::EMD();
		
		if (!emd->load(filename))
		{
			printf("can't load %s", filename.c_str());
			LibXenoverse::notifyError();
			LibXenoverse::waitOnEnd();
			return 0;
		}

		emp->replaceEmgFromEmd(filename2, emd);
		
		delete emp;
		delete emd;

	}else {
		printf("Error on arguments.\n");
		LibXenoverse::notifyError();
	}


	printf("finished.\n");
	LibXenoverse::waitOnEnd();
	return 0;
}