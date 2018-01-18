#include "LibXenoverse.h"

int main(int argc, char** argv)
{
	if (argc < 2) {
		printf("Usage: 'empEmd file.emp' will extract #EGM part to make a Emd + esk (tools are for this format).\n or if you use 'empEmd file.emd file.emp' it will replace #EMG part into emp by name matching EmdMesh into emd\n");
		getchar();
		return 1;
	}

	LibXenoverse::initializeDebuggingLog();

	

	string filename = ToString(argv[1]);
	string ext = LibXenoverse::extensionFromFilename(filename, true);

	string filename_1 = (ext == "emp") ? filename  : "";
	string filename_2 = (ext == "emd") ? filename : "";

	if (argc > 2)
	{
		filename = ToString(argv[2]);
		ext = LibXenoverse::extensionFromFilename(filename, true);
		
		if(ext == "emp")
			filename_1 = filename;
		if (ext == "emd")
			filename_2 = filename;
	}


	string name_1 = (filename_1.length()) ? LibXenoverse::nameFromFilenameNoExtension(filename_1, true) : "";
	string folder_1 = (filename_1.length()) ? LibXenoverse::folderFromFilename(filename_1) : "";
	string name_2 = (filename_2.length()) ? LibXenoverse::nameFromFilenameNoExtension(filename_2, true) : "";
	string folder_2 = (filename_2.length()) ? LibXenoverse::folderFromFilename(filename_2) : "";

	if (filename_1.length() == 0)
	{
		printf("emp file not found.");
		getchar();
		return 0;
	}
	
	if ((argc > 2)&&(filename_2.length() == 0))
	{
		printf("There is 2 file, but emd file not found.");
		getchar();
		return 0;
	}

	if (filename_2.length() == 0)			// don't have emd => so it's the EMG's extract operation
	{
		/*
		LibXenoverse::EMP* emp = new LibXenoverse::EMP();				//not finish yet, TODO this.
		emp->load(pack_name);
		*/

		LibXenoverse::EMP* emp = new LibXenoverse::EMP();
		LibXenoverse::EMD* emd = new LibXenoverse::EMD();
		LibXenoverse::EMM* emm = new LibXenoverse::EMM();			//Todo remove when Xenoviewer don't need anymore
		LibXenoverse::EMB* emb = new LibXenoverse::EMB();			//Todo remove when Xenoviewer don't need anymore

		emp->extractEmd(filename_1, emd, emm);

		emd->save(folder_1 + "\\" + name_1 + ".emd");
		emm->save(folder_1 + "\\" + name_1 + ".emm");
		emb->save(folder_1 + "\\" + name_1 + ".emb");


		delete emp;
		delete emd;
		delete emm;
		delete emb;

	}else {									//there is a Emd => So it's the replace of EMG operation
		

		LibXenoverse::EMP* emp = new LibXenoverse::EMP();
		LibXenoverse::EMD *emd = new LibXenoverse::EMD();
		
		if (!emd->load(filename_2))
		{
			printf("can't load %s", filename_2.c_str());
			getchar();
			return 0;
		}

		emp->replaceEmgFromEmd(filename_1, emd);
		
		delete emp;
		delete emd;
	}

	return 0;
}