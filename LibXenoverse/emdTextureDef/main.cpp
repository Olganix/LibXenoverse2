#include "LibXenoverse.h"


int main(int argc, char** argv)
{
	printf("*******************************************************************\n\
 This tool is for easier edit TextureDefinitions of a emd files of Dbxv2.\n\
 It's also because 3d softwares (blender and 3dsmax) don't have good fbx exporter for textures and custom informations\n\
 You could extract, in xml version, the TextureDefinitions as for emd xml version,\n\
 but you are not missing into all the vertices and faces. a new extension for that : .textdefemd.xml\n\
 it's a kind of template file.\n\
 The second operation is use a emd + .textdefemd.xml to replace textureDef in emd (on condition of same names inside)\n\
 Usage1: 'emdTextureDef.exe [options] file.emd'\n\
 Usage2: 'emdTextureDef.exe [options] file.emd file.textdefemd.xml'\n\
 Usage2: 'emdTextureDef.exe [options] file.emd.xml file.textdefemd.xml'\n\
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


	LibXenoverse::EMD* emd = 0;
	LibXenoverse::EMD* emdTextureDef = 0;
	string basefilename_mem = "";

	size_t nbArg = arguments.size();
	for (size_t i = 0; i < nbArg; i++)
	{
		string filename = arguments.at(i);
		string extension = LibXenoverse::extensionFromFilename(filename, true);
		string basefilename = filename.substr(0, filename.length() - (extension.size() + 1));
		string extension2 = LibXenoverse::extensionFromFilename(basefilename, true);
		string basefilename2 = basefilename.substr(0, basefilename.length() - (extension2.size() + 1));
		string extension3 = LibXenoverse::extensionFromFilename(basefilename2, true);
		string basefilename3 = basefilename2.substr(0, basefilename2.length() - (extension3.size() + 1));

		printf("Process on %s. Please Wait...\n", filename.c_str());
		
		if (extension == "emd") {

			emd = new LibXenoverse::EMD();
			if (!emd->load(filename))
			{
				delete emd;
				emd = 0;
			}
			
			basefilename_mem = basefilename;

		}else if ((extension == "xml") && (extension2 == "emd")) {

			emd = new LibXenoverse::EMD();
			if (!emd->loadXml(filename))
			{
				delete emd;
				emd = 0;
			}

			basefilename_mem = basefilename;


		}else if ((extension == "xml") && (extension2 == "textdefemd")) {

			emdTextureDef = new LibXenoverse::EMD();
			if (!emdTextureDef->loadTextureDefXml(filename))
			{
				delete emdTextureDef;
				emdTextureDef = 0;
			}

		}else {
			printf("Error on arguments or extension not recognized : %s.\n", filename.c_str());
			LibXenoverse::notifyError();
		}
	}

	if ((emd) && (emdTextureDef == 0))
	{
		emd->saveTextureDefXml(basefilename_mem + ".textdefemd.xml");

	}else if((emd) && (emdTextureDef)){

		emd->useTextureDefTemplate(emdTextureDef);
		emd->save(basefilename_mem + "_new.emd");
	}

	if (emd)
		delete emd;
	if (emdTextureDef)
		delete emdTextureDef;

	printf("finished.\n");
	LibXenoverse::waitOnEnd();
	return 0;
}