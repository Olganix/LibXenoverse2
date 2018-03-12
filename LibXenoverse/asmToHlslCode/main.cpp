




#include "LibXenoverse.h"
#include "Shader_vs_5_0.h"



/************************************************************************************
*									readFile
************************************************************************************/
std::string readFile(std::string filename)
{
	FILE* f = 0;
	fopen_s(&f, filename.c_str(), "rb");
	if (!f)
	{
		LOG_DEBUG("Cannot open file \"%s\" for reading.\n", filename.c_str());
		return "";
	}

	size_t size, rd;
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	uint8_t *buf = new uint8_t[size];
	if (!buf)
	{
		LOG_DEBUG("%s: Memory allocation error.", FUNCNAME);
		fclose(f);
		return "";
	}

	rd = fread(buf, 1, size, f);
	fclose(f);

	if (rd != size)
	{
		LOG_DEBUG("Read failure on file \"%s\"\n", path.c_str());
		delete[] buf;
		return "";
	}



	std::string asm_str = std::string((char*)buf);
	delete[] buf;

	return asm_str;
}
/************************************************************************************
*									writeFile
************************************************************************************/
void writeFile(std::string filename, std::string str)
{
	FILE* f = 0;
	fopen_s(&f, filename.c_str(), "wb");

	if (!f)
	{
		LOG_DEBUG("Cannot open for write/create file \"%s\"\n", path.c_str());
		return;
	}

	size_t wd = fwrite(str.c_str(), 1, str.length(), f);
	fclose(f);

	return;
}
/************************************************************************************
*									shader_vs_5_0_decode
************************************************************************************/
std::string shader_vs_5_0_decode(std::string asm_str, string filename = "")
{
	LibXenoverse::Shader_vs_5_0 shader_vs_5_0(filename);

	shader_vs_5_0.parseAsm(asm_str);
	return shader_vs_5_0.writeHlsl();
}
/************************************************************************************
*									shader_ps_5_0_decode
************************************************************************************/
std::string shader_ps_5_0_decode(std::string asm_str, string filename = "")
{
	LibXenoverse::Shader_vs_5_0 shader_vs_5_0(filename);				//todo use Shader_ps_5_0
	shader_vs_5_0.parseAsm(asm_str);
	return shader_vs_5_0.writeHlsl();
}





/************************************************************************************
*									main
************************************************************************************/
int main(int argc, char** argv)
{

	printf("*******************************************************************\n\
 This tool is for converter shader from assembler to Hlsl.\n\
 Usage: 'asmToHlslCode.exe [options] file.ext file2.ext ... '\n\
 Files could be assembler vertexShader (.xvu.asm) or pixelShader (.xpu.asm).\n\
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
	for (size_t i = 0; i < nbArg; i++)
	{
		string filename = arguments.at(i);
		string baseFilename = LibXenoverse::nameFromFilenameNoExtension(filename);
		string extension = LibXenoverse::extensionFromFilename(filename);


		if ((extension != "xvu.asm") && (extension != "xpu.asm"))
			continue;



		std::string asm_str = readFile(filename);

		std::string hlsl_str = "";
		if (extension == "xvu.asm")
		{
			if (asm_str.find("vs_5_0") == std::string::npos)
			{
				printf("miss vs_5_0\n");
				continue;
			}
			hlsl_str = shader_vs_5_0_decode(asm_str, filename);

		}else {

			if (asm_str.find("ps_5_0") == std::string::npos)
			{
				printf("miss ps_5_0\n");
				continue;
			}
			hlsl_str = shader_ps_5_0_decode(asm_str, filename);
		}

		writeFile(filename.substr(0, filename.length() - 3) + "hlsl", hlsl_str);
	}

	printf("finished.\n");
	LibXenoverse::waitOnEnd();
	return 0;
}