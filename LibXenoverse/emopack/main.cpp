#include "LibXenoverse.h"

int main(int argc, char** argv)
{
	if (argc < 2) {
		printf("Usage: emopack file.emo or emopack file.esk file0.emd file1.emd ...\n");
		getchar();
		return 1;
	}

	LibXenoverse::initializeDebuggingLog();

	string pack_name = ToString(argv[1]);
	string name = LibXenoverse::nameFromFilenameNoExtension(pack_name, true);
	string folder = LibXenoverse::folderFromFilename(pack_name);

	if (pack_name.find(".emo") != string::npos)
	{
		LibXenoverse::EMO *emo = new LibXenoverse::EMO();
		emo->load(pack_name);

		
		
		LibXenoverse::EMD *emd = new LibXenoverse::EMD();
		LibXenoverse::ESK *esk = new LibXenoverse::ESK();

		emo->writeEmdEsk(emd, esk);

		emd->save(folder + "\\" + name + ".emd");
		esk->save(folder + "\\" + name + ".esk");

		delete emo;
		delete emd;
		delete esk;
	}else {
		
		LibXenoverse::EMO *emo = new LibXenoverse::EMO();

		string parameter;
		string extension;
		for (int i = 1; i < argc; i++)				//need read esk before emd, because of pointer
		{
			parameter = ToString(argv[i]);
			extension = LibXenoverse::extensionFromFilename(parameter);

			if (extension == "esk")
			{
				LibXenoverse::ESK *esk = new LibXenoverse::ESK();
				esk->load(parameter);

				emo->readEsk(esk);

				delete esk;
			}
		}


		for (int i = 1; i < argc; i++)				//so emd after.
		{
			parameter = ToString(argv[i]);
			extension = LibXenoverse::extensionFromFilename(parameter);

			if (extension == "emd")
			{
				LibXenoverse::EMD *emd = new LibXenoverse::EMD();
				emd->load(parameter);

				emo->readEmd(emd);

				delete emd;
			}
		}

		emo->SaveToFile(folder + "\\" + name + ".emo");
	}

	return 0;
}