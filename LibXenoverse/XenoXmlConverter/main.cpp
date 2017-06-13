#include "LibXenoverse.h"

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: 'XenoXmlConverter.exe FILE.EXT FILE2.EXT ...' or 'XenoXmlConverter.exe FILE.EXT.xml FILE2.EXT.xml ...'\nFor Now, we only support sds, emm , emd , esk, ean, nsk, aur files");
		getchar();
		return 1;
	}

	LibXenoverse::initializeDebuggingLog();

	for (int i = 1; i < argc; i++)
	{
		string filename = ToString(argv[i]);
		string extension = LibXenoverse::extensionFromFilename(filename, true);
		string basefilename = filename.substr(0, filename.length() - (extension.size() + 1)) ;
		string extension2 = LibXenoverse::extensionFromFilename(basefilename, true);
		string basefilename2 = basefilename.substr(0, basefilename.length() - (extension2.size() + 1));

	
		if (extension == "sds")
		{
			LibXenoverse::SDS* sds = new LibXenoverse::SDS();
			if(sds->load(filename))
				sds->saveXML(filename +".xml");
			delete sds;

		}else if ((extension == "xml")&&(extension2 == "sds")) {
			LibXenoverse::SDS* sds = new LibXenoverse::SDS();
			if(sds->load(filename))
				sds->save(basefilename2 + ".sds");
			delete sds;




		}else if (extension == "emm") {		// .emm  .ptcl.emm  .trc.emm  .tbind.emm  .pbind.emm
			
			if((extension2 == "tbind") || (extension2 == "pbind"))
			{
				printf("Error: .tbind.emm and .pbind.emm are not really like .emm file (there a EMB and a unknow EMP ETR flags. So skip. Sorry.\n");
				getchar();
				continue;
			}

			LibXenoverse::EMM* emm = new LibXenoverse::EMM();
			if (emm->load(filename))
				emm->saveXML(filename + ".xml");
			delete emm;

		}else if ((extension == "xml") && (extension2 == "emm") ) {

			LibXenoverse::EMM* emm = new LibXenoverse::EMM();
			if(emm->load(filename))
				emm->save(basefilename2 + ".emm");
			delete emm;



		}else if (extension == "emd") {
			LibXenoverse::EMD* emd = new LibXenoverse::EMD();
			if(emd->load(filename))
				emd->saveXml(filename + ".xml");
			delete emd;

		}else if ((extension == "xml") && (extension2 == "emd")) {

			LibXenoverse::EMD* emd = new LibXenoverse::EMD();
			if(emd->loadXml(filename))
				emd->save(basefilename2 + ".emd");
			delete emd;


		}else if (extension == "esk") {
			LibXenoverse::ESK* esk = new LibXenoverse::ESK();
			if (esk->load(filename))
				esk->saveXml(filename + ".xml");
			delete esk;

		}else if ((extension == "xml") && (extension2 == "esk"))  {

			LibXenoverse::ESK* esk = new LibXenoverse::ESK();
			if(esk->loadXml(filename))
				esk->save(basefilename2 + ".esk");
			delete esk;



		}else if (extension.substr(extension.length()-3) == "ean") {		//.ean, .fce.ean, .cam.ena
			LibXenoverse::EAN* ean = new LibXenoverse::EAN();
			if (ean->load(filename))
				ean->saveXml(filename + ".xml");
			delete ean;

		}else if ((extension == "xml") && (extension2 == "ean")) {

			LibXenoverse::EAN* ean = new LibXenoverse::EAN();
			if(ean->loadXml(filename))
				ean->save(basefilename2 + ".ean");
			delete ean;



		}else if (extension == "nsk") {
			LibXenoverse::NSK* nsk = new LibXenoverse::NSK();
			if (nsk->load(filename))
				nsk->saveXml(filename + ".xml");
			delete nsk;

		}else if ((extension == "xml") && (extension2 == "nsk")) {

			LibXenoverse::NSK* nsk = new LibXenoverse::NSK();
			if (nsk->loadXml(filename))
				nsk->save(basefilename2 + ".nsk");
			delete nsk;



		}else if (extension == "aur") {
			LibXenoverse::AUR* aur = new LibXenoverse::AUR();
			if (aur->load(filename))
				aur->saveXml(filename + ".xml");
			delete aur;

		}
		else if ((extension == "xml") && (extension2 == "aur")) {

			LibXenoverse::AUR* aur = new LibXenoverse::AUR();
			if (aur->loadXml(filename))
				aur->save(basefilename2 + ".aur");
			delete aur;
		}


	}




	return 0;
}