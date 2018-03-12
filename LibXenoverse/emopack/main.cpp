#include "LibXenoverse.h"

int main(int argc, char** argv)
{
	printf("*******************************************************************\n\
 This tool is for convert the files .emo <-> .emd + .esk.\n\
 Usage: 'emopack.exe [options] file.emo' or 'emopack.exe [options] file.esk file.emd file2.emd ...'\n\
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
	for (size_t i = 0; i <nbArg; i++)										//on the second case, we need to have esk first.
	{
		if (LibXenoverse::extensionFromFilename(arguments.at(i), true)=="esk")
		{
			arguments.insert(arguments.begin(), arguments.at(i));
			arguments.erase(arguments.begin() + i + 1);
			break;
		}
	}



	string filename = arguments.at(0);
	string extension = LibXenoverse::extensionFromFilename(filename, true);
	string basefilename = filename.substr(0, filename.length() - (extension.size() + 1));


	if (extension == "emo")
	{
		LibXenoverse::EMO *emo = new LibXenoverse::EMO();
		if (emo->load(filename))
		{
			LibXenoverse::EMD *emd = new LibXenoverse::EMD();
			LibXenoverse::ESK *esk = new LibXenoverse::ESK();
			emo->writeEmdEsk(emd, esk);


			//emo->oldSaveFbx(folder + "\\" + name + "_test" + ".fbx");		//test because of saintSeya work well with old emoTools fbx conversion

			emd->save(basefilename +".emd");
			esk->save(basefilename +".esk");

			delete emd;
			delete esk;
		}
		delete emo;




	}else if (extension == "esk") {
		
		LibXenoverse::EMO *emo = new LibXenoverse::EMO();
		
		LibXenoverse::ESK *esk = new LibXenoverse::ESK();
		if (esk->load(filename))
			emo->readEsk(esk);

		delete esk;
		arguments.erase(arguments.begin());
		nbArg--;


		for (size_t i = 0; i < nbArg; i++)				//so emd after.
		{
			filename = arguments.at(i);
			extension = LibXenoverse::extensionFromFilename(filename, true);

			if (extension != "emd")
				continue;

			
			LibXenoverse::EMD *emd = new LibXenoverse::EMD();
			if (emd->load(filename))
				emo->readEmd(emd);

			delete emd;
		}

		emo->SaveToFile(basefilename +".emo");
		delete emo;

	}else if (extension == "emd") {

		LibXenoverse::EMO *emo = new LibXenoverse::EMO();

		for (size_t i = 0; i < nbArg; i++)				//so emd after.
		{
			filename = arguments.at(i);
			extension = LibXenoverse::extensionFromFilename(filename, true);

			if (extension != "emd")
				continue;

			LibXenoverse::EMD *emd = new LibXenoverse::EMD();
			if (emd->load(filename))
				emo->readEmd(emd);

			delete emd;
		}

		emo->SaveToFile(basefilename + ".emo");
		delete emo;

	}else{
		printf("Error on arguments.\n");
		LibXenoverse::notifyError();
	}



	printf("finished.\n");
	LibXenoverse::waitOnEnd();
	return 0;
}