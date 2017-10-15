#include "LibXenoverse.h"
#include "qdir.h"



std::vector<std::string> split(const std::string &text, char sep)
{
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}


enum typeData
{
	TD_BOOL = 0,
	TD_FLOAT,
	TD_FLOAT2,
	TD_FLOAT3,
	TD_FLOAT4,
	TD_FLOAT4x4,
	TD_FLOAT4x3,
	TD_SAMPLER2D,
	TD_SAMPLERCUBE,
	TD_UNKNOW,
};
string typeData_names[TD_UNKNOW + 1];

enum registerData
{
	RD_CONSTANTE = 0,
	RD_BOOL,
	RD_SAMPLER,
	RD_UNKNOW,
};
string registerData_names[RD_UNKNOW + 1];




struct shaderParam
{
	string mName;
	typeData mTypeData;
	
	registerData mRegisterData;
	size_t mRegisterIndex;
	size_t mRegisterSize;


	//links informations
	string mLinkEmmMaterialParameter;
	string mLinkEmmMaterial;												//debug

	string mLinkSdsParameter;
	string mLinkSds;														//debug

	
	//debug
	string mSrcFilename;

	shaderParam(string name, typeData typeData, string srcFilename = "")
	{
		mName = name;
		mTypeData = typeData;
		mRegisterData = RD_UNKNOW;
		mRegisterIndex = (size_t)-1;
		mRegisterSize = 0;

		mSrcFilename = srcFilename;

		mLinkEmmMaterialParameter = "";
		mLinkEmmMaterial = "";
		mLinkSdsParameter = "";
		mLinkSds = "";
	}

	bool operator==(shaderParam &other)
	{ 
		return ((this->mName == other.mName) && (this->mTypeData == other.mTypeData) && (this->mRegisterData == other.mRegisterData) && (this->mRegisterIndex == other.mRegisterIndex) && (this->mRegisterSize == other.mRegisterSize));
	}
	bool operator!=(shaderParam &other)
	{
		return (!(*this == other));
	}

	string toString(void)
	{
		return (registerData_names[mRegisterData] + ((mRegisterIndex != (size_t)-1) ? std::to_string(mRegisterIndex) : "") + "\tx" + std::to_string(mRegisterSize) + "\t" + typeData_names[mTypeData] + "\t" + mName + "\t=>\t" + ((mLinkEmmMaterialParameter != "") ? (mLinkEmmMaterialParameter + "\t(Mat:" + mLinkEmmMaterial + ")") : ((mLinkSdsParameter != "") ? (mLinkSdsParameter + "\t(Sds:" + mLinkSds + ")") : "")));
	}
};


bool order(const shaderParam* a, const shaderParam* b)
{
	if (a->mRegisterData != b->mRegisterData)
		return (a->mRegisterData < b->mRegisterData);

	if (a->mRegisterIndex != b->mRegisterIndex)
		return (a->mRegisterIndex < b->mRegisterIndex);

	if (a->mRegisterSize != b->mRegisterSize)
		return (a->mRegisterSize < b->mRegisterSize);

	if (a->mTypeData != b->mTypeData)
		return (a->mTypeData < b->mTypeData);

	return (a->mName < b->mName);
}


size_t mIncWorkingDir = (size_t)-1;

std::vector<LibXenoverse::SDS*> *mListSdsFiles;
std::vector<LibXenoverse::EMM*> *mListEmmFiles;

std::vector<shaderParam*> *mListVertexShadersParams;
std::vector<shaderParam*> *mListPixelShadersParams;


/**************************************************************************************
									analyzeSds
***************************************************************************************/
void analyzeSds(string filename)
{
	//saddly, we have to wait having shaders analyse before try to do the link. So we just load the file
	LibXenoverse::SDS *sds_shaderPrograms = new LibXenoverse::SDS();
	if (sds_shaderPrograms->load(filename))
		mListSdsFiles->push_back(sds_shaderPrograms);
	else
		delete sds_shaderPrograms;
}
/**************************************************************************************
									analyzeEmm
***************************************************************************************/
void analyzeEmm(string filename)
{
	try
	{
		//saddly, we have to wait having shaders analyse before try to do the link. So we just load the file
		LibXenoverse::EMM *emm = new LibXenoverse::EMM();
		if (emm->load(filename))
			mListEmmFiles->push_back(emm);
		else
			delete emm;
	}catch (...) {
		printf("error, exception on %s\n", filename.c_str());
	}
}
/**************************************************************************************
									analyzeShaderSourceAsm
***************************************************************************************/
void analyzeShaderSourceAsm(string filename, string sources, std::vector<shaderParam*> *listAllParams)
{
	bool isParameters = false;
	bool isRegisters = false;
	string line = "";
	size_t nbParams = 0;
	std::vector<string> sv;
	std::vector<string> sv2;
	string arg = "";
	string paramName = "";
	string regName = "";
	string regData = "";
	size_t regSize = 0;
	size_t regIndex = 0;


	std::vector<shaderParam*> listParams;

	//read shader source to know what it need.
	std::vector<std::string> lines = split(sources, '\n');

	size_t nbLines = lines.size();
	for (size_t i = 0; i < nbLines; i++)
	{
		line = lines.at(i);
		if (line.substr(0, 2) != "//")				//only keep commentaires.
			continue;

		if (line == "// Parameters:")
		{
			isParameters = true;
			isRegisters = false;
			continue;
		}

		if (line == "// Registers:")
		{
			isParameters = false;
			isRegisters = true;
			continue;
		}

		if ((line == "") || (line.substr(0, 12) == "// Generated") || (line == "//   Name                 Reg   Size") || (line == "//   -------------------- ----- ----"))
			continue;


		sv = split(line, ' ');
		nbParams = sv.size();
		sv2.clear();

		for (size_t j = 0; j < nbParams; j++)				//clean arg.
		{
			arg = sv.at(j);
			if ((arg == "") || (arg == "//"))
				continue;

			if (arg.substr(arg.length() - 1) == ";")
				arg = arg.substr(0, arg.length() - 1);

			if (arg.find("["))
				arg = arg.substr(0, arg.find("["));

			sv2.push_back(arg);
		}


		if ((isParameters) && (sv2.size() == 2))
		{
			if (sv2.at(1).substr(0, 1) == "$")					//for uniform parameters		//to not have $ avec fxc.exe => you must use global variable (instaed of fonction declarations)
				sv2.at(1) = sv2.at(1).substr(1);

			typeData td = TD_UNKNOW;
			if (sv2.at(0) == "bool")
				td = TD_BOOL;
			else if (sv2.at(0) == "float")
				td = TD_FLOAT;
			else if (sv2.at(0) == "float2")
				td = TD_FLOAT2;
			else if (sv2.at(0) == "float3")
				td = TD_FLOAT3;
			else if (sv2.at(0) == "float4")
				td = TD_FLOAT4;
			else if (sv2.at(0) == "float4x4")
				td = TD_FLOAT4x4;
			else if (sv2.at(0) == "float4x3")
				td = TD_FLOAT4x3;
			else if (sv2.at(0) == "sampler2D")
				td = TD_SAMPLER2D;
			else if (sv2.at(0) == "samplerCUBE")
				td = TD_SAMPLERCUBE;
			else
				assert(false);	//if we pass here, we have to complete by reading shaders

			listParams.push_back(new shaderParam(sv2.at(1), td, filename));
		}

		if ((isRegisters) && (sv2.size() == 3))
		{
			if ((sv2.at(0) == "Name") || (sv2.at(0).substr(0, 5) == "-----"))
				continue;

			if (sv2.at(0).substr(0, 1) == "$")					//for uniform parameters
				sv2.at(0) = sv2.at(0).substr(1);

			paramName = sv2.at(0);
			regName = sv2.at(1);
			regData = regName.substr(0, 1);
			regIndex = std::stoi(regName.substr(1));
			regSize = std::stoi(sv2.at(2));


			nbParams = listParams.size();
			for (size_t j = 0; j < nbParams; j++)
			{
				if (listParams.at(j)->mName == paramName)
				{
					if (regData == "c")
						listParams.at(j)->mRegisterData = RD_CONSTANTE;
					else if (regData == "b")
						listParams.at(j)->mRegisterData = RD_BOOL;
					else if (regData == "s")
						listParams.at(j)->mRegisterData = RD_SAMPLER;
					else
						assert(false);	//if we pass here, we have to complete by reading shaders

					listParams.at(j)->mRegisterIndex = regIndex;
					listParams.at(j)->mRegisterSize = regSize;

					break;
				}
			}
		}
	}
	

	//add if unique.
	nbParams = listParams.size();
	size_t nbParams2 = listAllParams->size();
	bool isfound = false;
	shaderParam* param;
	for (size_t i = 0; i < nbParams; i++)
	{
		param = listParams.at(i);

		isfound = false;
		for (size_t j = 0; j < nbParams2; j++)
		{
			if (*param == *(listAllParams->at(j)))
			{
				isfound = true;
				break;
			}
		}

		if (!isfound)
			listAllParams->push_back(param);
		else
			delete param;
	}
}
/**************************************************************************************
									analyzeXvu
***************************************************************************************/
void analyzeXvu(string filename)
{
	size_t data_size = 0;
	char *data = LibXenoverse::HLSLASM::disassembleFromFile(filename, data_size);
	if (data)
	{
		string sources(data);						//asm string
		
		analyzeShaderSourceAsm(filename, sources, mListVertexShadersParams);
		
		delete data;
	}
}
/**************************************************************************************
									analyzeXpu
***************************************************************************************/
void analyzeXpu(string filename)
{
	size_t data_size = 0;
	char *data = LibXenoverse::HLSLASM::disassembleFromFile(filename, data_size);
	if (data)
	{
		string sources(data);						//asm string

		analyzeShaderSourceAsm(filename, sources, mListPixelShadersParams);

		delete data;
	}
}


/**************************************************************************************
									analyzeFolder
***************************************************************************************/
void analyzeFolder(QString folderPath)													//recursive
{
	string debug_str = folderPath.toStdString();
	printf("Analyse :%s\n", debug_str.c_str());
	
	QDir src_folder = QDir(folderPath);
	string filename = "";
	string extension = "";

	QFileInfoList files = src_folder.entryInfoList();
	foreach(QFileInfo file, files)
	{
		string debug_str2 = file.fileName().toStdString();
		printf("-find :%s\n", debug_str2.c_str());
		
		if ((file.fileName().toStdString() == ".") || (file.fileName().toStdString() == ".."))
			continue;

		if (file.isDir())
		{
			analyzeFolder( file.absoluteFilePath() );
			continue;
		}

		string debug_str3 = file.absolutePath().toStdString();

		filename = file.absolutePath().toStdString() + file.fileName().toStdString();
		extension = file.suffix().toStdString();


		if (extension == "emz")
		{
			LibXenoverse::EMZ *emz_pack = new LibXenoverse::EMZ();
			if (emz_pack->load(file.filePath().toStdString()))
			{
				mIncWorkingDir++;
				
				QString workingDir = "Temps_" + QString(std::to_string(mIncWorkingDir).c_str());
				if (!QDir().exists(workingDir))
					QDir().mkdir(workingDir);
				else
					QDir().cleanPath(workingDir);
				

				string new_extension = emz_pack->detectNewExtension();
				emz_pack->saveUncompressed(workingDir.toStdString() +"\\"+ file.baseName().toStdString() + new_extension);
				delete emz_pack;

				analyzeFolder(workingDir);

				QDir().cleanPath(workingDir);
				QDir().rmdir(workingDir);
				QDir().remove(workingDir);
			}

		}else if (extension == "emb"){

			LibXenoverse::EMB *emb_pack = new LibXenoverse::EMB();
			if (emb_pack->load(file.filePath().toStdString()))
			{
				mIncWorkingDir++;

				QString workingDir = "Temps_" + QString(std::to_string(mIncWorkingDir).c_str());
				if (!QDir().exists(workingDir))
					QDir().mkdir(workingDir);
				else
					QDir().cleanPath(workingDir);


				emb_pack->extract(workingDir.toStdString() + "/");
				delete emb_pack;

				analyzeFolder(workingDir);

				QDir().cleanPath(workingDir);
				QDir().rmdir(workingDir);
				QDir().remove(workingDir);
			}

		}else if (extension == "sds"){
			analyzeSds(file.filePath().toStdString());
		}else if (extension == "emm"){
			analyzeEmm(file.filePath().toStdString());
		}else if (extension == "xvu"){
			analyzeXvu(file.filePath().toStdString());
		}else if (extension == "xpu"){
			analyzeXpu(file.filePath().toStdString());
		}
	}
}




/**************************************************************************************
										makeLinks
***************************************************************************************/
void makeLinks( )																		//try to do links between shaders parameters and material and sds 's parameters.
{
	shaderParam* param;
	LibXenoverse::EMM* emmFile;
	LibXenoverse::SDS* sdsFile;
	LibXenoverse::EMMMaterial* emmMat;
	LibXenoverse::EMMParameter* emmParam;
	LibXenoverse::SDSShaderProgram* sdsShaderProg;
	LibXenoverse::SDSParameter* sdsParam;
	string name;
	string name_lowercase;
	string emmParamName_lowercase;
	size_t nbEmmMat;
	size_t nbSdsShaderProg;
	size_t nbEmmParams;
	bool isfoundLinkForParam = false;
	size_t nbParam = mListVertexShadersParams->size();
	size_t nbMaterial = mListEmmFiles->size();
	size_t nbSdsFiles = mListSdsFiles->size();
	for (size_t i = 0; i < nbParam; i++)
	{
		param = mListVertexShadersParams->at(i);
		name = LibXenoverse::nameFromFilenameNoExtension(param->mSrcFilename, true);
		
		name_lowercase = param->mName;
		std::transform(name_lowercase.begin(), name_lowercase.end(), name_lowercase.begin(), ::tolower);

		isfoundLinkForParam = false;
		
		for (size_t j = 0; ((j < nbMaterial) && (isfoundLinkForParam==false)); j++)
		{
			emmFile = mListEmmFiles->at(j);

			std::vector<LibXenoverse::EMMMaterial*> &listEmmMat = emmFile->getMaterials();
			nbEmmMat = listEmmMat.size();
			for (size_t k = 0; ((k < nbEmmMat) && (isfoundLinkForParam == false)); k++)
			{
				emmMat = listEmmMat.at(k);
				std::vector<LibXenoverse::EMMParameter*> &emmParamList = emmMat->getParameters();
				nbEmmParams = emmParamList.size();

				for (size_t m = 0; m < nbEmmParams; m++)
				{
					emmParam = emmParamList.at(m);			
					emmParamName_lowercase = emmParam->name;
					std::transform(emmParamName_lowercase.begin(), emmParamName_lowercase.end(), emmParamName_lowercase.begin(), ::tolower);
					
					isfoundLinkForParam = ((emmParamName_lowercase.find(name_lowercase) != (size_t)-1) || (name_lowercase.find(emmParamName_lowercase) != (size_t)-1));

					if(!isfoundLinkForParam)
					{
						string lastChar = emmParamName_lowercase.substr(emmParamName_lowercase.size()-1);
						if ((lastChar == "r") || (lastChar == "g") || (lastChar == "b") || (lastChar == "a") || (lastChar == "x") || (lastChar == "y") || (lastChar == "z") || (lastChar == "w") || (lastChar == "u") || (lastChar == "v"))
						{
							emmParamName_lowercase = emmParamName_lowercase.substr(0, emmParamName_lowercase.size() - 1);

							isfoundLinkForParam = ((emmParamName_lowercase.find(name_lowercase) != (size_t)-1) || (name_lowercase.find(emmParamName_lowercase) != (size_t)-1));
						}
					}

					if ((!isfoundLinkForParam) && (emmParamName_lowercase.find("mat") != (size_t)-1))			//special case material -> mat
					{
						size_t index = emmParamName_lowercase.find("mat");
						emmParamName_lowercase = emmParamName_lowercase.substr(0, index) + "material" + emmParamName_lowercase.substr(index + 3);

						isfoundLinkForParam = ((emmParamName_lowercase.find(name_lowercase) != (size_t)-1) || (name_lowercase.find(emmParamName_lowercase) != (size_t)-1));
					}

					if ((!isfoundLinkForParam) && (emmParamName_lowercase.find("texscrl") != (size_t)-1))			//special case texscroll -> texscrl
					{
						size_t index = emmParamName_lowercase.find("texscrl");
						emmParamName_lowercase = emmParamName_lowercase.substr(0, index) + "texscroll" + emmParamName_lowercase.substr(index + 7);

						isfoundLinkForParam = ((emmParamName_lowercase.find(name_lowercase) != (size_t)-1) || (name_lowercase.find(emmParamName_lowercase) != (size_t)-1));
					}

					if(isfoundLinkForParam)
					{
						param->mLinkEmmMaterialParameter = emmParam->name;
						param->mLinkEmmMaterial = emmMat->getName();				//debug

						isfoundLinkForParam = true;
						break;
					}
				}				
			}
		}



		
			
		//de meme avec les sds
		for (size_t j = 0; ((j < nbSdsFiles) && (isfoundLinkForParam == false)); j++)
		{
			sdsFile = mListSdsFiles->at(j);

			std::vector<LibXenoverse::SDSShaderProgram*> &listShaderProgram = sdsFile->GetshaderPrograms();
			nbSdsShaderProg = listShaderProgram.size();
			for (size_t k = 0; ((k < nbSdsShaderProg) && (isfoundLinkForParam == false)); k++)
			{
				sdsShaderProg = listShaderProgram.at(k);
				std::vector<LibXenoverse::SDSParameter*> &sdsParamList = sdsShaderProg->getParameters();
				nbEmmParams = sdsParamList.size();

				for (size_t m = 0; m < nbEmmParams; m++)
				{
					sdsParam = sdsParamList.at(m);
					emmParamName_lowercase = sdsParam->name;
					std::transform(emmParamName_lowercase.begin(), emmParamName_lowercase.end(), emmParamName_lowercase.begin(), ::tolower);

					isfoundLinkForParam = ((emmParamName_lowercase.find(name_lowercase) != (size_t)-1) || (name_lowercase.find(emmParamName_lowercase) != (size_t)-1));

					if (!isfoundLinkForParam)
					{
						string lastChar = emmParamName_lowercase.substr(emmParamName_lowercase.size() - 1);
						if ((lastChar == "r") || (lastChar == "g") || (lastChar == "b") || (lastChar == "a") || (lastChar == "x") || (lastChar == "y") || (lastChar == "z") || (lastChar == "w") || (lastChar == "u") || (lastChar == "v"))
						{
							emmParamName_lowercase = emmParamName_lowercase.substr(0, emmParamName_lowercase.size() - 1);

							isfoundLinkForParam = ((emmParamName_lowercase.find(name_lowercase) != (size_t)-1) || (name_lowercase.find(emmParamName_lowercase) != (size_t)-1));
						}
					}

					if ((!isfoundLinkForParam) && (emmParamName_lowercase.find("mat") != (size_t)-1))			//special case material -> mat
					{
						size_t index = emmParamName_lowercase.find("mat");
						emmParamName_lowercase = emmParamName_lowercase.substr(0, index) + "material" + emmParamName_lowercase.substr(index + 3);

						isfoundLinkForParam = ((emmParamName_lowercase.find(name_lowercase) != (size_t)-1) || (name_lowercase.find(emmParamName_lowercase) != (size_t)-1));
					}

					if ((!isfoundLinkForParam) && (emmParamName_lowercase.find("texscrl") != (size_t)-1))			//special case texscroll -> texscrl
					{
						size_t index = emmParamName_lowercase.find("texscrl");
						emmParamName_lowercase = emmParamName_lowercase.substr(0, index) + "texscroll" + emmParamName_lowercase.substr(index + 7);

						isfoundLinkForParam = ((emmParamName_lowercase.find(name_lowercase) != (size_t)-1) || (name_lowercase.find(emmParamName_lowercase) != (size_t)-1));
					}

					if (isfoundLinkForParam)
					{
						param->mLinkSdsParameter = sdsShaderProg->getName();
						param->mLinkSds = sdsFile->getName();				//debug

						isfoundLinkForParam = true;
						break;
					}
				}
			}
		}



	}


	//de meme pour les pixel shader
	for (size_t i = 0; i < nbParam; i++)
	{
		param = mListPixelShadersParams->at(i);
		name = LibXenoverse::nameFromFilenameNoExtension(param->mSrcFilename, true);

		name_lowercase = param->mName;
		std::transform(name_lowercase.begin(), name_lowercase.end(), name_lowercase.begin(), ::tolower);

		isfoundLinkForParam = false;

		for (size_t j = 0; ((j < nbMaterial) && (isfoundLinkForParam == false)); j++)
		{
			emmFile = mListEmmFiles->at(j);

			std::vector<LibXenoverse::EMMMaterial*> &listEmmMat = emmFile->getMaterials();
			nbEmmMat = listEmmMat.size();
			for (size_t k = 0; ((k < nbEmmMat) && (isfoundLinkForParam == false)); k++)
			{
				emmMat = listEmmMat.at(k);
				std::vector<LibXenoverse::EMMParameter*> &emmParamList = emmMat->getParameters();
				nbEmmParams = emmParamList.size();

				for (size_t m = 0; m < nbEmmParams; m++)
				{
					emmParam = emmParamList.at(m);
					emmParamName_lowercase = emmParam->name;
					std::transform(emmParamName_lowercase.begin(), emmParamName_lowercase.end(), emmParamName_lowercase.begin(), ::tolower);

					isfoundLinkForParam = ((emmParamName_lowercase.find(name_lowercase) != (size_t)-1) || (name_lowercase.find(emmParamName_lowercase) != (size_t)-1));

					if (!isfoundLinkForParam)
					{
						string lastChar = emmParamName_lowercase.substr(emmParamName_lowercase.size() - 1);
						if ((lastChar == "r") || (lastChar == "g") || (lastChar == "b") || (lastChar == "a") || (lastChar == "x") || (lastChar == "y") || (lastChar == "z") || (lastChar == "w") || (lastChar == "u") || (lastChar == "v") )
						{
							emmParamName_lowercase = emmParamName_lowercase.substr(0, emmParamName_lowercase.size() - 1);

							isfoundLinkForParam = ((emmParamName_lowercase.find(name_lowercase) != (size_t)-1) || (name_lowercase.find(emmParamName_lowercase) != (size_t)-1));
						}
					}

					if ((!isfoundLinkForParam) && (emmParamName_lowercase.find("mat") != (size_t)-1))			//special case material -> mat
					{
						size_t index = emmParamName_lowercase.find("mat");
						emmParamName_lowercase = emmParamName_lowercase.substr(0, index) + "material" + emmParamName_lowercase.substr(index + 3);

						isfoundLinkForParam = ((emmParamName_lowercase.find(name_lowercase) != (size_t)-1) || (name_lowercase.find(emmParamName_lowercase) != (size_t)-1));
					}

					if ((!isfoundLinkForParam) && (emmParamName_lowercase.find("texscrl") != (size_t)-1))			//special case texscroll -> texscrl
					{
						size_t index = emmParamName_lowercase.find("texscrl");
						emmParamName_lowercase = emmParamName_lowercase.substr(0, index) + "texscroll" + emmParamName_lowercase.substr(index + 7);

						isfoundLinkForParam = ((emmParamName_lowercase.find(name_lowercase) != (size_t)-1) || (name_lowercase.find(emmParamName_lowercase) != (size_t)-1));
					}
					

					if (isfoundLinkForParam)
					{
						param->mLinkEmmMaterialParameter = emmParam->name;
						param->mLinkEmmMaterial = emmMat->getName();				//debug

						isfoundLinkForParam = true;
						break;
					}
				}
			}
		}






		//de meme avec les sds
		for (size_t j = 0; ((j < nbSdsFiles) && (isfoundLinkForParam == false)); j++)
		{
			sdsFile = mListSdsFiles->at(j);

			std::vector<LibXenoverse::SDSShaderProgram*> &listShaderProgram = sdsFile->GetshaderPrograms();
			nbSdsShaderProg = listShaderProgram.size();
			for (size_t k = 0; ((k < nbSdsShaderProg) && (isfoundLinkForParam == false)); k++)
			{
				sdsShaderProg = listShaderProgram.at(k);
				std::vector<LibXenoverse::SDSParameter*> &sdsParamList = sdsShaderProg->getParameters();
				nbEmmParams = sdsParamList.size();

				for (size_t m = 0; m < nbEmmParams; m++)
				{
					sdsParam = sdsParamList.at(m);
					emmParamName_lowercase = sdsParam->name;
					std::transform(emmParamName_lowercase.begin(), emmParamName_lowercase.end(), emmParamName_lowercase.begin(), ::tolower);

					isfoundLinkForParam = ((emmParamName_lowercase.find(name_lowercase) != (size_t)-1) || (name_lowercase.find(emmParamName_lowercase) != (size_t)-1));

					if (!isfoundLinkForParam)
					{
						string lastChar = emmParamName_lowercase.substr(emmParamName_lowercase.size() - 1);
						if ((lastChar == "r") || (lastChar == "g") || (lastChar == "b") || (lastChar == "a") || (lastChar == "x") || (lastChar == "y") || (lastChar == "z") || (lastChar == "w") || (lastChar == "u") || (lastChar == "v"))
						{
							emmParamName_lowercase = emmParamName_lowercase.substr(0, emmParamName_lowercase.size() - 1);

							isfoundLinkForParam = ((emmParamName_lowercase.find(name_lowercase) != (size_t)-1) || (name_lowercase.find(emmParamName_lowercase) != (size_t)-1));
						}
					}

					if ((!isfoundLinkForParam) && (emmParamName_lowercase.find("mat") != (size_t)-1))			//special case material -> mat
					{
						size_t index = emmParamName_lowercase.find("mat");
						emmParamName_lowercase = emmParamName_lowercase.substr(0, index) + "material" + emmParamName_lowercase.substr(index + 3);

						isfoundLinkForParam = ((emmParamName_lowercase.find(name_lowercase) != (size_t)-1) || (name_lowercase.find(emmParamName_lowercase) != (size_t)-1));
					}

					if ((!isfoundLinkForParam) && (emmParamName_lowercase.find("texscrl") != (size_t)-1))			//special case texscroll -> texscrl
					{
						size_t index = emmParamName_lowercase.find("texscrl");
						emmParamName_lowercase = emmParamName_lowercase.substr(0, index) + "texscroll" + emmParamName_lowercase.substr(index + 7);

						isfoundLinkForParam = ((emmParamName_lowercase.find(name_lowercase) != (size_t)-1) || (name_lowercase.find(emmParamName_lowercase) != (size_t)-1));
					}

					if (isfoundLinkForParam)
					{
						param->mLinkSdsParameter = sdsShaderProg->getName();
						param->mLinkSds = sdsFile->getName();				//debug

						isfoundLinkForParam = true;
						break;
					}
				}
			}
		}

	}


}
/**************************************************************************************
							makeUnLinksEmmMaterialParameters_str
***************************************************************************************/
string makeUnLinksEmmMaterialParameters_str()
{
	string str = "";


	//essaye de trouver les parameters des Emm qui ne sont pas utilisé.
	shaderParam* param;
	LibXenoverse::EMM* emmFile;
	LibXenoverse::EMMMaterial* emmMat;
	LibXenoverse::EMMParameter* emmParam;
	string name;
	string name_lowercase;
	string emmParamName_lowercase;
	size_t nbEmmMat;
	size_t nbEmmParams;
	bool isfoundLinkForParam = false;
	size_t nbParam = mListVertexShadersParams->size();
	size_t nbMaterial = mListEmmFiles->size();

	std::vector<string> listMatParamAllReadyChecked;
	std::vector<string> listMatParamUnlinked;
	std::vector<string> listMatParamUnlinked_materialOrigin;						//debug

	for (size_t j = 0; j < nbMaterial; j++)
	{
		emmFile = mListEmmFiles->at(j);

		std::vector<LibXenoverse::EMMMaterial*> &listEmmMat = emmFile->getMaterials();
		nbEmmMat = listEmmMat.size();
		for (size_t k = 0; k < nbEmmMat; k++)
		{
			emmMat = listEmmMat.at(k);
			std::vector<LibXenoverse::EMMParameter*> &emmParamList = emmMat->getParameters();
			nbEmmParams = emmParamList.size();

			for (size_t m = 0; m < nbEmmParams; m++)
			{
				emmParam = emmParamList.at(m);
				emmParamName_lowercase = emmParam->name;


				//check dans les listMatParamAllReadyChecked
				size_t nbAllReadyChecked = listMatParamAllReadyChecked.size();
				bool isAllReadyChecked = false;
				for (size_t i = 0; i < nbAllReadyChecked; i++)
				{
					if (listMatParamAllReadyChecked.at(i) == emmParamName_lowercase)
					{
						isAllReadyChecked = true;
						break;
					}
				}
				if (isAllReadyChecked)
					continue;

				isfoundLinkForParam = false;


				for (size_t i = 0; i < nbParam; i++)
				{
					param = mListVertexShadersParams->at(i);
					string paramName = param->mLinkEmmMaterialParameter;

					if (paramName.size() != emmParamName_lowercase.size())
						continue;

					isfoundLinkForParam = paramName == emmParamName_lowercase;


					if (!isfoundLinkForParam)
					{
						string lastChar = emmParamName_lowercase.substr(emmParamName_lowercase.size() - 1);
						string lastChar2 = paramName.substr(paramName.size() - 1);

						std::transform(lastChar.begin(), lastChar.end(), lastChar.begin(), ::tolower);
						std::transform(lastChar2.begin(), lastChar2.end(), lastChar2.begin(), ::tolower);

						if (((lastChar == "r") || (lastChar == "g") || (lastChar == "b") || (lastChar == "a") || (lastChar == "x") || (lastChar == "y") || (lastChar == "z") || (lastChar == "w") || (lastChar == "u") || (lastChar == "v")) && ((lastChar2 == "r") || (lastChar2 == "g") || (lastChar2 == "b") || (lastChar2 == "a") || (lastChar2 == "x") || (lastChar2 == "y") || (lastChar2 == "z") || (lastChar2 == "w") || (lastChar2 == "u") || (lastChar2 == "v")))
							isfoundLinkForParam = paramName.substr(0, paramName.size() - 1) == emmParamName_lowercase.substr(0, paramName.size() - 1);
					}
					
					if (isfoundLinkForParam)
						break;
				}

				if (!isfoundLinkForParam)
				{
					listMatParamUnlinked.push_back(emmParamName_lowercase);
					listMatParamUnlinked_materialOrigin.push_back(emmMat->getName() + "," + emmFile->getName());
				}

				listMatParamAllReadyChecked.push_back(emmParamName_lowercase);
			}
		}
	}



	size_t nbParamUnlinked = listMatParamUnlinked.size();
	for (size_t i = 0; i < nbParamUnlinked; i++)
		str += "=> " + listMatParamUnlinked.at(i) + " (" + listMatParamUnlinked_materialOrigin.at(i) + ")\n";

	return str;
}



/**************************************************************************************
										main
***************************************************************************************/
int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: Analyzer path\\data\nMake a txt file with all (unique) parameters of shaders, materials or shaderProgram.\nNote: could take some time and some Ram.\n");
		getchar();
		return 1;
	}


	typeData_names[TD_BOOL] = "bool";
	typeData_names[TD_FLOAT] = "float";
	typeData_names[TD_FLOAT2] = "float2";
	typeData_names[TD_FLOAT3] = "float3";
	typeData_names[TD_FLOAT4] = "float4";
	typeData_names[TD_FLOAT4x4] = "float4x4";
	typeData_names[TD_FLOAT4x3] = "float4x3";
	typeData_names[TD_SAMPLER2D] = "sampler2D";
	typeData_names[TD_SAMPLERCUBE] = "samplerCUBE";
	typeData_names[TD_UNKNOW] = "unknow";

	registerData_names[RD_CONSTANTE] = "c";
	registerData_names[RD_BOOL] = "b";
	registerData_names[RD_SAMPLER] = "s";
	registerData_names[RD_UNKNOW] = "u";



	mListSdsFiles = new std::vector<LibXenoverse::SDS*>();
	mListEmmFiles = new std::vector<LibXenoverse::EMM*>();

	mListVertexShadersParams = new std::vector<shaderParam*>();
	mListPixelShadersParams = new std::vector<shaderParam*>();


	LibXenoverse::initializeDebuggingLog();

	string rootPath = ToString(argv[1]);

	analyzeFolder(QString(rootPath.c_str()));

	string str = "";

	
	makeLinks();



	//make a list.
	std::sort(mListVertexShadersParams->begin(), mListVertexShadersParams->end(), &order);
	std::sort(mListPixelShadersParams->begin(), mListPixelShadersParams->end(), &order);


	str += "\n\nVertexShaderParams :------------------------------------------\n";
	size_t nbParam = mListVertexShadersParams->size();
	for (size_t i = 0; i < nbParam; i++)
		str += mListVertexShadersParams->at(i)->toString() + "\n";

	str += "\n\nPixelShaderParams :------------------------------------------\n";
	nbParam = mListPixelShadersParams->size();
	for (size_t i = 0; i < nbParam; i++)
		str += mListPixelShadersParams->at(i)->toString() + "\n";

	str += "\n\nUnLinked EmmMaterialParameters :------------------------------------------\n";
	str += makeUnLinksEmmMaterialParameters_str();

	LibXenoverse::writeTextTo("Analyze.txt", str.c_str(), str.size());
	

	/*
	////////////////////////////////////// second analyze: we do the list of shaders for characters ("light" problem)
	std::vector<string> listUniqueSdsShader;
	
	LibXenoverse::EMM* emm;
	LibXenoverse::EMMMaterial* mat;
	string shaderName = "";
	size_t nbMaterial = 0;
	size_t nbShaders = 0;
	bool isfound = false;

	size_t nbEmm = mListEmmFiles->size();
	for (size_t i = 0; i < nbEmm; i++)
	{
		emm = mListEmmFiles->at(i);
		std::vector<LibXenoverse::EMMMaterial*> matlist = emm->getMaterials();

		nbMaterial = matlist.size();
		for (size_t j = 0; j < nbMaterial; j++)
		{
			mat = matlist.at(j);

			shaderName = mat->getShaderName();

			nbShaders = listUniqueSdsShader.size();
			isfound = false;
			for (size_t k = 0; k < nbShaders; k++)
			{
				if (listUniqueSdsShader.at(k) == shaderName)
				{
					isfound = true;
					break;
				}
			}

			if (isfound)
				continue;
			
			listUniqueSdsShader.push_back(shaderName);
			str += emm->getName() + " : " + mat->getName() + " -> " + shaderName +"\n";
		}
	}
	LibXenoverse::writeTextTo("Analyze_SdsShader_forCharacters.txt", str.c_str(), str.size());
	*/

	delete mListVertexShadersParams;
	delete mListPixelShadersParams;
	delete mListSdsFiles;
	delete mListEmmFiles;

	printf("Finish.");
	getchar();

	return 0;
}