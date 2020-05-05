
#include "LibXenoverse.h"




int main(int argc, char** argv)
{
	printf("*******************************************************************\n\
 This tool is for simplify Havok Xml version\n\
 Usage: 'HavokXmlSimplifier.exe [options] file.ext '\n\
 Files formats supported : hkx.\n\
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

	LibXenoverse::Havok havok;

	size_t nbArg = arguments.size();
	for (size_t i = 0; i < nbArg; i++)
	{
		string filename = arguments.at(i);
		string extension = LibXenoverse::extensionFromFilename(filename, true);
		string basefilename = filename.substr(0, filename.length() - (extension.size() + 1)) ;
		string extension2 = LibXenoverse::extensionFromFilename(basefilename, true);
		string basefilename2 = basefilename.substr(0, basefilename.length() - (extension2.size() + 1));
		string extension3 = LibXenoverse::extensionFromFilename(basefilename2, true);
		string basefilename3 = basefilename2.substr(0, basefilename2.length() - (extension3.size() + 1));

		printf("Process on %s. Please Wait...\n", filename.c_str());


		if ((extension == "xml") && (extension2 == "hkx")) {

			havok.simplifyHavokXml(filename, basefilename2 +"_simp.hkx.xml");

		}else {
			printf("Error on arguments or extension not recognized.\n");
			LibXenoverse::notifyError();
		}
	}

	printf("finished.\n");
	LibXenoverse::waitOnEnd();
	return 0;
}