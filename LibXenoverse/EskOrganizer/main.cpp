#include "LibXenoverse.h"
#include <iostream>

#define BUFLEN 1024


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


bool isNumber(const string& val)
{
	std::basic_stringstream<char, std::char_traits<char>, std::allocator<char> > str(val);
	float tst;
	str >> tst;
	return !str.fail() && str.eof();
}

std::string readLine()
{
	std::string line;
	int c = 0;

	fflush(stdin);

	while(c != '\n')
	{
		c = getchar();
		if (c != '\n')
		{
			putchar(c);
			line += string((char*)&c);
		}
		else{
			line += string("\0");
		}
	}

	printf("\n");
	return line;
}


int main(int argc, char** argv)
{

	LibXenoverse::initializeDebuggingLog();

	printf("Welcome to Esk organizer v0.1\n\n");


	string help = "Commands: \n\
		'Help' to see this list\n\
		'Load <path\\filenameEskFile> <file2.ext> ...' load a esk file (or ean file, because there is a esk part into)\n\
		'Save <indexEsk> <path\\filename>' save your modified esk (or ean) file\n\
		'GetEskFileList'\n\
		'GetBoneList <indexEsk> <useTree>' if useTree is 'true', you will parent-child relation, if 'false' just the list byIndex\n\
		'Rename <indexEsk> <indexBone> <newName> <renameAlsoIntoEmdFile>' notice you could use the name of the bone for indexBone. if you use renameAlsoIntoEmdFile at true, it will also rename bone in all Emd File loaded.\n\
		'Remove <indexEsk> <indexBone> <recursive>' Delete the specified bone. delete also its children if recursive is true (by defaut)\n\
		'Copy <indexEsk> <indexBone> <recursive>' Copy a bone. and its children if recursive is at true (by default)\n\
		'Paste <indexEsk> <indexBone> <alsoCopyAnimation> <matchDuration>' Paste bones Copyed on Child of bone specified by indexBone. in case of source and destination are Ean file, alsoCopyAnimation at 'true' will be also copy animation, and in this case matchDuration at 'true' will resampling source animation to match with destination animation (from other bones of destination file), helpfull for cycling animations.\n\
		'CalculTransformMatrixFromSkinningMatrix <indexEsk> <indexBone> ' (Experimentale)Read the Skinning Matrix (with Relative information about Transfrom) to make the transformMatrix (absolute informations), beacause Ean have esk part without transformMatrix. So you will could make a esk from ean, by using this methode and rename root bone. if indexEsk==-1 it will be on all bones\n\
		'ClearTransformMatrix <indexEsk> <indexBone> ' remove TranformMatrix. is to clean for a Ean  file. if indexEsk==-1 it will be on all bones\n\
		'CalculSkinningMatrixFromTransformMatrix <indexEsk> <indexBone> ' (Experimentale)Inverse opeartion.\n\
		'ClearSkinningMatrix <indexEsk> <indexBone>' remove SkinningMatrix.\n\
		'getJson <indexEsk>' for tests comparaisons.\n\
		'GetBonePosition <indexEsk> <indexBone> <absolute>' get the bone position, absolute (from transformMatrix) if true (else relative, so from SkinningMatrix).\n\
		'GetBoneOrientation <indexEsk> <indexBone> <absolute>' get the bone quaternion information of the orientation, absolute if true (else relative).\n\
		'GetBoneScale <indexEsk> <indexBone> <absolute>' get the bone position, absolute if true (else relative).\n\
		'SetBonePosition <indexEsk> <indexBone> <posX> <posY> <posZ> <absolute>'.\n\
		'SetBoneOrientation <indexEsk> <indexBone> <quatX> <quatY> <quatZ> <quatW> <absolute>' set by using quaternion definition.\n\
		'SetBoneScale <indexEsk> <indexBone> <scaleX> <scaleY> <scaleZ> <absolute>'.\n\
		'Merge <indexEsk_src> <indexEsk_dest>' same merge fonction used by emdFbx. Merge Ean file, and if it jsut about esk, merge esk.\n\
		'EMD_Load <path\\filenameEmdFile> [<path\\filenameEmdFile2> <path\\filenameEmdFile3> ...]' Load Emd (goal is to rename bones for mesh skinning). you could load many of files ones.\n\
		'EMD_Save [<indexEmd> <path\\filename>]' save your modified emd. if you don't specify a emd in particular, all file will be saved with '_modified'.\n\
		'EMD_GetEmdFileList'\n\
		'EMD_GetBoneList <indexEmd>'\n\
		'EMD_Rename' <indexEmd> <oldBoneName> <newBoneName>' rename bone into Emds, use -1 on indexEmd for apply on all Emd\n\
		'Save_Fpf_xml <indexEsk> <indexEsk_b> <path\\filename>' create and save a extract of fpf.xml (for pose on HeroCorp) from your modified esk (or ean) file. the second esk is the original of a character for get bones to export (because blender add bones). you can put the same if you want.\n\
		'Quit'\n";

	//'GetBoneRotation <indexEsk> <indexBone> <absolute>' get the bone taitBryan angles (know also as EulerAngle, with the diference to have this order : RotY (Yaw) -> RotZ (pitch) -> RotX (Roll) like a plane) information of the orientation, absolute if true (else relative).\n\
	//'SetBoneRotation <indexEsk> <indexBone> <RotY> <RotZ> <RotX> <absolute>' see GetBoneRotation.\n\
				
		

	printf((string("You could load many Esk or ean File, get bone list.\nFor Paths, please avoid space.\nThere is One rule about bones : don't rename c_Base and root bone. and keep unique Names at the end\n") + help +"\n").c_str());


	

	struct EanOrEskFile
	{
		LibXenoverse::EAN* mEan;
		LibXenoverse::ESK* mEsk;
		string mFilename;

		EanOrEskFile(string filename)
		{
			mEan = NULL;
			mEsk = NULL;
			mFilename = filename;
		}
	};

	struct EskOrganizer_Copy
	{
		LibXenoverse::EskTreeNode* mTreeNode;
		LibXenoverse::EskTreeNode* mRootToClean;
		EanOrEskFile* mSourceFile;

		EskOrganizer_Copy(LibXenoverse::EskTreeNode* treeNode, EanOrEskFile* sourceFile, LibXenoverse::EskTreeNode* rootToClean)
		{
			mTreeNode = treeNode;
			mRootToClean = rootToClean;
			mSourceFile = sourceFile;
		}
	};

	EskOrganizer_Copy eskOrganizer_Copy(nullptr, nullptr, nullptr);
	vector<EanOrEskFile*> listEskFile;
	vector<LibXenoverse::EMD*> listEmdFile;


	string line = "";
	std::vector<string> arguments;
	std::vector<string> arg_tmp;



	while ((arguments.size() == 0) || (arguments.at(0) != "Quit"))
	{
		printf("So, what do you want ?\n");
		line = readLine();
		



		//get full argument with take care of '"' to have filename with spaces.
		arg_tmp = split(line, ' ');
		size_t nbArg = arg_tmp.size();

		arguments.clear();
		string str_tmp = "";
		string str2_tmp = "";
		for (size_t i = 0; i < nbArg; i++)									//detection of options, remove then from arguments list, plus take care of folder with spaces inside (use " ")
		{
			str_tmp = arg_tmp.at(i);
			if ((str2_tmp.length() == 0) && (str_tmp.length() == 0))
				continue;

			if ((str2_tmp.length() == 0) && (str_tmp[0] == '"'))
			{
				if (str_tmp[str_tmp.length() - 1] != '"')
				{
					str2_tmp = str_tmp.substr(1);
				}
				else {
					str2_tmp = str_tmp.substr(1, str_tmp.length() - 2);
					arguments.push_back(str2_tmp);
					str2_tmp = "";
				}
				continue;
			}else if (str2_tmp.length() != 0) {

				str2_tmp += " " + str_tmp;

				if (str_tmp[str_tmp.length() - 1] == '"')
				{
					arguments.push_back(str2_tmp.substr(0, str2_tmp.length() - 1));
					str2_tmp = "";
				}
				continue;
			}
			arguments.push_back(str_tmp);
		}
		if (str2_tmp.length() != 0)					//put the last argument if they miss a " at the end
			arguments.push_back(str2_tmp);


		if (arguments.size() == 0)
			continue;








		//begin to work on Each case.

		string command = arguments.at(0);
		arguments.erase(arguments.begin());
		nbArg = arguments.size();


		///////////////////////////////////////////////////////////////////////////////////////////////////
		if (command == "Quit")
		{
			break;

		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "Help"){
			printf("%s",help.c_str());



		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "Load"){


			size_t inc = 0;
			for (size_t i = 0; i < nbArg; i++)
			{
				string filename = arguments.at(i);
				string extension = LibXenoverse::extensionFromFilename(filename, true);


				if (extension == "ean")
				{
					EanOrEskFile *eanOrEskFile = new EanOrEskFile(filename);
					eanOrEskFile->mEan = new LibXenoverse::EAN();
					if (eanOrEskFile->mEan->load(filename))
					{
						listEskFile.push_back(eanOrEskFile);
						eanOrEskFile->mEsk = eanOrEskFile->mEan->getSkeleton();
						printf("Ean Loaded\n");
					}else {
						printf("Error: Fail on loading %s", filename.c_str());
						delete eanOrEskFile;
					}

				}else if (extension == "esk") {

					EanOrEskFile *eanOrEskFile = new EanOrEskFile(filename);
					eanOrEskFile->mEsk = new LibXenoverse::ESK();
					if (eanOrEskFile->mEsk->load(filename))
					{
						listEskFile.push_back(eanOrEskFile);
						printf("Esk Loaded\n");
					}else {
						printf("Error: Fail on loading %s", filename.c_str());
						delete eanOrEskFile;
					}

				}else {
					printf("%s it's not a Ean or Esk", filename.c_str());
				}
			}
				



		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "Save"){

			if (nbArg < 2)
			{
				printf("You miss arguments. try 'Help' command\n");
				continue;
			}
			size_t index = std::stoi(arguments.at(0));
			string filename = arguments.at(1);
			string extension = LibXenoverse::extensionFromFilename(filename, true);

			if (index < listEskFile.size())
			{
				if ((listEskFile.at(index)->mEan) && (extension == "ean"))
				{
					listEskFile.at(index)->mEan->save(filename);
					printf("Ean file saved at %s.\n", filename.c_str());

				}else if (extension == "esk") {
					listEskFile.at(index)->mEsk->save(filename);
					printf("Esk file saved at %s.\n", filename.c_str());

				}else{
					printf("extension is not correct (.ean or .esk): %s.\n", filename.c_str());
				}
			}else{
				printf("index %i is not in file list.\n", index);
			}




		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "GetEskFileList"){

			size_t nbFile = listEskFile.size();
			for (size_t i = 0; i < nbFile; i++)
				printf("%i : %s\n", i, listEskFile.at(i)->mFilename.c_str());





		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "GetBoneList"){

			if (nbArg < 1)
			{
				printf("You miss arguments. try 'Help' command\n");
				continue;
			}
			size_t index = std::stoi(arguments.at(0));
			bool useTree = (nbArg > 1) ? (arguments.at(1) == "true") : false;
				


			if (index < listEskFile.size())
			{
				LibXenoverse::ESK *esk = listEskFile.at(index)->mEsk;

				if (esk)
				{
					std::vector<LibXenoverse::ESKBone*> &bones = esk->getBones();
					size_t nbBone = bones.size();

					if (useTree == false)												//just a list of index
					{
						for (size_t i = 0; i < nbBone; i++)
							printf("%i : %s\n", i, bones.at(i)->getName().c_str());
					}else{
						LibXenoverse::EskTreeNode *treeNode = esk->getTreeOrganisation();
						printf("%s\n", treeNode->getDisplayStr().c_str());
					}
				}else{
					printf("No skeleton definition in this file\n");
				}
			}else{
				printf("index %i is not in ean file list.\n", index);
			}




		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "Rename"){

			if (nbArg < 3)
			{
				printf("You miss arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			string boneRef = arguments.at(1);
			string newName = arguments.at(2);
			bool renameEmd = (nbArg > 3) ? (arguments.at(3) == "true") : false;
				

			if(indexFile < listEskFile.size())
			{				
				size_t boneIndex = (isNumber(boneRef) ? std::stoi(boneRef) : listEskFile.at(indexFile)->mEsk->getBoneIndex(boneRef));
				
				if (boneIndex < listEskFile.at(indexFile)->mEsk->getBones().size())
				{
					string oldName = listEskFile.at(indexFile)->mEsk->getBones().at(boneIndex)->getName();
					
					listEskFile.at(indexFile)->mEsk->getBones().at(boneIndex)->setName(newName);
					printf("Name changed.\n");

					if (renameEmd)
					{
						size_t nbEmdFile = listEmdFile.size();
						for (size_t i = 0; i < nbEmdFile; i++)
							listEmdFile.at(i)->replaceBonesNames(oldName, newName);

						printf("Emd bone also renamed.\n");
					}
				}else{
					printf("boneIndex %i is not in list of bones.\n", boneIndex);
				}
			}else{
				printf("index %i is not in file list.\n", indexFile);
			}
		

		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "Remove"){

			if (nbArg < 2)
			{
				printf("You miss arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			string boneRef = arguments.at(1);
			bool recursive = (nbArg>2) ? (arguments.at(2) == "true") : true;
				


			if (indexFile < listEskFile.size())
			{
				size_t boneIndex = (isNumber(boneRef) ? std::stoi(boneRef) : listEskFile.at(indexFile)->mEsk->getBoneIndex(boneRef));

				if (boneIndex < listEskFile.at(indexFile)->mEsk->getBones().size())
				{
					std::vector<std::vector<size_t>> listMovingindex = listEskFile.at(indexFile)->mEsk->remove(boneIndex, recursive);

					//if we have a ean, we have to deal with offset on bone's index.
					if (listEskFile.at(indexFile)->mEan)
						listEskFile.at(indexFile)->mEan->changeBoneIndexes(listMovingindex);

					printf("Done.\nATTENTION: index of bone could have changed.\n");
				}else{
					printf("boneIndex %i is not in list of bones.\n", boneIndex);
				}
			}else{
				printf("index %i is not in file list.\n", indexFile);
			}




		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "Copy"){

			if (nbArg < 2)
			{
				printf("You miss arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			string boneRef = arguments.at(1);
			bool recursive = (nbArg>2) ? (arguments.at(2) == "true") : true;
				


			if (indexFile < listEskFile.size())
			{
				size_t boneIndex = (isNumber(boneRef) ? std::stoi(boneRef) : listEskFile.at(indexFile)->mEsk->getBoneIndex(boneRef));

				if (boneIndex < listEskFile.at(indexFile)->mEsk->getBones().size())
				{
					if (eskOrganizer_Copy.mRootToClean != nullptr)
					{
						delete eskOrganizer_Copy.mRootToClean;

						eskOrganizer_Copy.mRootToClean = nullptr;
						eskOrganizer_Copy.mTreeNode = nullptr;
						eskOrganizer_Copy.mSourceFile = nullptr;
					}

					LibXenoverse::EskTreeNode* rootNode = listEskFile.at(indexFile)->mEsk->getTreeOrganisation();					
					eskOrganizer_Copy.mSourceFile = listEskFile.at(indexFile);
					eskOrganizer_Copy.mTreeNode = rootNode->getBoneWithIndex(boneIndex);
					eskOrganizer_Copy.mRootToClean = rootNode;

					printf("Copyed.\n");
				}else{
					printf("boneIndex %i is not in list of bones.\n", boneIndex);
				}
			}else{
				printf("index %i is not in file list.\n", indexFile);
			}






		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "Paste"){

			if (eskOrganizer_Copy.mRootToClean == nullptr)
			{
				printf("Nothing Copyed. please use 'Copy' command before. try 'Help' command\n");
				continue;
			}

			if (nbArg < 2)
			{
				printf("You miss arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			string boneRef = arguments.at(1);
			bool copyAnimation = (nbArg>2) ? (arguments.at(2) == "true") : true;
			bool matchDuration = (nbArg>3) ? (arguments.at(3) == "true") : true;


			

			if (indexFile < listEskFile.size())
			{
				size_t boneIndex = (isNumber(boneRef) ? std::stoi(boneRef) : listEskFile.at(indexFile)->mEsk->getBoneIndex(boneRef));

				if (boneIndex < listEskFile.at(indexFile)->mEsk->getBones().size())
				{
					std::vector<std::vector<std::vector<size_t>>> listMovingindex = listEskFile.at(indexFile)->mEsk->addTree(eskOrganizer_Copy.mTreeNode, boneIndex);

					std::vector<std::vector<size_t>> &listNewBonesAdded = listMovingindex.at(0);
					std::vector<std::vector<size_t>> &listMovingindex_forPrevious = listMovingindex.at(1);
					
					
					if (listEskFile.at(indexFile)->mEan)
					{
						//if the destination and source file are ean, and if copyAnimation, we have to do it with help of listNewBonesAdded
						if ((eskOrganizer_Copy.mSourceFile->mEan) && (copyAnimation))
							listEskFile.at(indexFile)->mEan->addBoneAnimationFromAnotherEan(eskOrganizer_Copy.mSourceFile->mEan, listNewBonesAdded, matchDuration);


						//if we have a Ean, we have to deal with offset on bone's index.
						listEskFile.at(indexFile)->mEan->changeBoneIndexes(listMovingindex_forPrevious);
					}

					printf("Pasted.\nATTENTION: index of bone could have changed.\n");
				}else{
					printf("boneIndex %i is not in list of bones.\n", boneIndex);
				}
			}else{
				printf("index %i is not in file list.\n", indexFile);
			}







		///////////////////////////////////////////////////////////////////////////////////////////////////	
		}else if ((command == "ClearTransformMatrix") || (command == "ClearSkinningMatrix")){

			if (nbArg < 2)
			{
				printf("You miss arguments. try 'Help' command\n");
				continue;
			}
			bool isSkinningMatrix = (command == "ClearSkinningMatrix");
			size_t indexFile = std::stoi(arguments.at(0));
			string boneRef = arguments.at(1);



			if (indexFile < listEskFile.size())
			{
				size_t boneIndex = (isNumber(boneRef) ? std::stoi(boneRef) : listEskFile.at(indexFile)->mEsk->getBoneIndex(boneRef));

				std::vector<LibXenoverse::ESKBone*> &bones = listEskFile.at(indexFile)->mEsk->getBones();
				LibXenoverse::ESKBone* bone = nullptr;

				size_t nbBones = bones.size();
				for (size_t i = 0; i < nbBones; i++)
				{
					if ((boneIndex != (size_t)-1) && (boneIndex != i))
						continue;

					bone = bones.at(i);

					if (!isSkinningMatrix)
					{
						//matrix4x4 iddentity
						bone->transform_matrix[0] = 1; bone->transform_matrix[1] = 0; bone->transform_matrix[2] = 0; bone->transform_matrix[3] = 0;
						bone->transform_matrix[4] = 0; bone->transform_matrix[5] = 1; bone->transform_matrix[6] = 0; bone->transform_matrix[7] = 0;
						bone->transform_matrix[8] = 0; bone->transform_matrix[9] = 0; bone->transform_matrix[10] = 1; bone->transform_matrix[11] = 0;
						bone->transform_matrix[12] = 0; bone->transform_matrix[13] = 0; bone->transform_matrix[14] = 0; bone->transform_matrix[15] = 1;
						bone->haveTransformMatrix = false;
					}else{
						//matrix3x4 iddentity
						bone->skinning_matrix[0] = 0; bone->skinning_matrix[1] = 0; bone->skinning_matrix[2] = 0; bone->skinning_matrix[3] = 1;
						bone->skinning_matrix[4] = 1; bone->skinning_matrix[5] = 0; bone->skinning_matrix[6] = 0; bone->skinning_matrix[7] = 0;
						bone->skinning_matrix[8] = 1; bone->skinning_matrix[9] = 1; bone->skinning_matrix[10] = 1; bone->skinning_matrix[11] = 1;
					}
					if (boneIndex != (size_t)-1)
						break;
				}

				printf("Done.\n");
			}else{
				printf("index %i is not in file list.\n", indexFile);
			}





		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if ((command == "CalculTransformMatrixFromSkinningMatrix") || (command == "CalculSkinningMatrixFromTransformMatrix")){

			
			if (nbArg < 2)
			{
				printf("You miss arguments. try 'Help' command\n");
				continue;
			}
			bool isSkinningMatrix = (command == "CalculSkinningMatrixFromTransformMatrix");
			size_t indexFile = std::stoi(arguments.at(0));
			string boneRef = arguments.at(1);

			

			if (indexFile < listEskFile.size())
			{
				size_t boneIndex = (isNumber(boneRef) ? std::stoi(boneRef) : listEskFile.at(indexFile)->mEsk->getBoneIndex(boneRef));

				std::vector<LibXenoverse::ESKBone*> &bones = listEskFile.at(indexFile)->mEsk->getBones();
				LibXenoverse::ESKBone* bone = nullptr;

				size_t nbBones = bones.size();
				for (size_t i = 0; i < nbBones; i++)
				{
					if ((boneIndex != (size_t)-1) && (boneIndex != i))
						continue;

					bone = bones.at(i);

					if (!isSkinningMatrix)
						bone->calculTransformMatrixFromSkinningMatrix(bones, true);
					else
						bone->calculSkinningMatrixFromTransformMatrix(bones, true);

					if (boneIndex != (size_t)-1)
						break;
				}

				printf("Done.\n");
			}
			else{
				printf("index %i is not in file list.\n", indexFile);
			}



		
		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if ((command == "GetBonePosition") || (command == "SetBonePosition")
			|| (command == "GetBoneOrientation") || (command == "SetBoneOrientation")
			|| (command == "GetBoneRotation") || (command == "SetBoneRotation")
			|| (command == "GetBoneScale") || (command == "SetBoneScale")
			){

			size_t nbMinimumArg = 1;
			if      (command == "SetBonePosition")		nbMinimumArg = 5;
			else if (command == "GetBoneOrientation")	nbMinimumArg = 2;
			else if (command == "SetBoneOrientation")	nbMinimumArg = 6;
			else if (command == "GetBoneRotation")		nbMinimumArg = 2;
			else if (command == "SetBoneRotation")		nbMinimumArg = 5;
			else if (command == "GetBoneScale")			nbMinimumArg = 2;
			else if (command == "SetBoneScale")			nbMinimumArg = 5;

			if (nbArg < nbMinimumArg)
			{
				printf("You miss arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));
			string boneRef = arguments.at(1);

			std::vector<float> vector;
			if (nbMinimumArg >= 5)
			{
				vector.push_back(std::stof(arguments.at(2)));
				vector.push_back(std::stof(arguments.at(3)));
				vector.push_back(std::stof(arguments.at(4)));
				if (nbMinimumArg >= 6)
					vector.push_back(std::stof(arguments.at(5)));
			}

			bool absolute = (nbArg > nbMinimumArg) ? (arguments.at(nbMinimumArg) == "true") : false;
			

			


			if (indexFile < listEskFile.size())
			{
				size_t boneIndex = (isNumber(boneRef) ? std::stoi(boneRef) : listEskFile.at(indexFile)->mEsk->getBoneIndex(boneRef));

				if (boneIndex < listEskFile.at(indexFile)->mEsk->getBones().size())
				{
					std::vector<LibXenoverse::ESKBone*> &bones = listEskFile.at(indexFile)->mEsk->getBones();
					LibXenoverse::ESKBone* bone = nullptr;

					size_t nbBones = bones.size();
					for (size_t i = 0; i < nbBones; i++)
					{
						if(boneIndex != i)
							continue;

						bone = bones.at(i);

						float skinning_matrix[12];
						if (!absolute)
						{
							for (size_t i = 0; i < 12; i++)
								skinning_matrix[i] = bone->skinning_matrix[i];
						}else{

							double resultTransformMatrix[16];

							for (size_t i = 0; i < 16; i++)
								resultTransformMatrix[i] = bone->transform_matrix[i];

							double tmpTransformMatrix[16];
							LibXenoverse::ESKBone::inverse4x4(&resultTransformMatrix[0], &tmpTransformMatrix[0]);

							LibXenoverse::ESKBone::transpose4x4(&tmpTransformMatrix[0]);		//come back to Ogre matrice way
							double skinning_matrix_b[12];						//special tranformation from observation between skinningMatrix and transformMatrix
							LibXenoverse::ESKBone::decomposition4x4(&tmpTransformMatrix[0], &skinning_matrix_b[0]);

							for (size_t i = 0; i < 12; i++)
								skinning_matrix[i] = (float)skinning_matrix_b[i];
						}

						bool isASet = false;
						if (command == "GetBonePosition")
						{
							printf("Position : %f, %f, %f\n", skinning_matrix[0], skinning_matrix[1], skinning_matrix[2]);
						}else if (command == "GetBoneOrientation"){
							printf("Orientation (xyzw): %f, %f, %f, %f\n", skinning_matrix[4], skinning_matrix[5], skinning_matrix[6], skinning_matrix[7]);
						}else if (command == "GetBoneRotation"){
							printf("TODO. sorry\n");
						}else if (command == "GetBoneScale"){
							printf("Scale : %f, %f, %f\n", skinning_matrix[8], skinning_matrix[9], skinning_matrix[10]);
						}else if (command == "SetBonePosition"){
							skinning_matrix[0] = vector.at(0);
							skinning_matrix[1] = vector.at(1);
							skinning_matrix[2] = vector.at(2);
							isASet = true;
						}else if (command == "SetBoneOrientation"){
							skinning_matrix[4] = vector.at(0);
							skinning_matrix[5] = vector.at(1);
							skinning_matrix[6] = vector.at(2);
							skinning_matrix[7] = vector.at(3);
							isASet = true;
						}else if (command == "SetBoneRotation"){
							printf("TODO. sorry\n");
						}else if (command == "SetBoneScale"){
							skinning_matrix[8] = vector.at(0);
							skinning_matrix[9] = vector.at(1);
							skinning_matrix[10] = vector.at(2);
							isASet = true;
						}

						if (isASet)
						{
							if (!absolute)
							{
								for (size_t i = 0; i < 12; i++)
									bone->skinning_matrix[i] = skinning_matrix[i];

								printf("Done. Please Considere to use 'CalculTransformMatrixFromSkinningMatrix %i -1' to update TransformMatrix.\n", indexFile);
							}else{

								double skinning_matrix_b[12];						//special tranformation from observation between skinningMatrix and transformMatrix
								for (size_t i = 0; i < 12; i++)
									skinning_matrix_b[i] = skinning_matrix[i];


								double resultTransformMatrix[16];
								LibXenoverse::ESKBone::makeTransform4x4(&skinning_matrix_b[0], &resultTransformMatrix[0]);

								LibXenoverse::ESKBone::transpose4x4(&resultTransformMatrix[0]);		//come back to Ogre matrice way

								double tmpTransformMatrix[16];
								LibXenoverse::ESKBone::inverse4x4(&resultTransformMatrix[0], &tmpTransformMatrix[0]);

								for (size_t i = 0; i < 16; i++)
									bone->transform_matrix[i] = (float)tmpTransformMatrix[i];

								printf("Done. Please Considere to use 'CalculSkinningMatrixFromTransformMatrix %i -1' to update SkinningMatrix.\n", indexFile);
							}
						}
						break;
					}
				}else{
					printf("boneIndex %i is not in list of bones.\n", boneIndex);
				}
			}else{
				printf("index %i is not in file list.\n", indexFile);
			}




		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "getJson"){

			if (nbArg < 1)
			{
				printf("You miss arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));

			if (indexFile < listEskFile.size())
				printf("\n\n%s\n\n", listEskFile.at(indexFile)->mEsk->getTreeOrganisation()->getJsonStr().c_str());
			else
				printf("index %i is not in file list.\n", indexFile);





		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "Test"){

			if (nbArg < 1)
			{
				printf("You miss arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile = std::stoi(arguments.at(0));


			if (indexFile < listEskFile.size())
			{
				std::vector<LibXenoverse::ESKBone*> &bones = listEskFile.at(indexFile)->mEsk->getBones();
				LibXenoverse::ESKBone* bone = nullptr;

				size_t nbBones = bones.size();
				for (size_t i = 0; i < nbBones; i++)
				{
					bone = bones.at(i);
					bone->test_calculTransformMatrixFromSkinningMatrix(bones, false);
				}

				printf("Done.\n");
			}else{
				printf("index %i is not in file list.\n", indexFile);
			}

		


		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "Merge"){

			if (nbArg < 2)
			{
				printf("You miss arguments. try 'Help' command\n");
				continue;
			}
			size_t indexFile_src = std::stoi(arguments.at(0));
			size_t indexFile_dest = std::stoi(arguments.at(1));


			if ((indexFile_src < listEskFile.size()) && (indexFile_dest < listEskFile.size()))
			{
				if ((listEskFile.at(indexFile_dest)->mEan) && (listEskFile.at(indexFile_src)->mEan))
				{
					listEskFile.at(indexFile_dest)->mEan->merge(listEskFile.at(indexFile_src)->mEan);
				}else{
					listEskFile.at(indexFile_dest)->mEsk->merge(listEskFile.at(indexFile_src)->mEsk);
				}

				printf("Done.\n");
			}else{
				if ((indexFile_src < listEskFile.size()) && (indexFile_dest < listEskFile.size()))
					printf("indexFile_src %i or  %i is not in file list.\n", indexFile_src, indexFile_dest);
			}
			



		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "EMD_Load"){


			for (size_t i = 0; i < nbArg; i++)
			{
				string filename = arguments.at(i);
				string extension = LibXenoverse::extensionFromFilename(filename, true);

				if (extension != "emd")
				{
					printf("%s not a Emd. skipped\n", filename.c_str());
					continue;
				}

				LibXenoverse::EMD* emd = new LibXenoverse::EMD();
				if (emd->load(filename))
				{
					listEmdFile.push_back(emd);
					printf("file %s loaded.\n", filename.c_str());
				}else{
					printf("Error: fail on loading %s\n", filename.c_str());
					delete emd;
				}
			}
			


		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "EMD_Save"){

			if (arguments.at(0) == "")
			{
				arguments.erase(arguments.begin());
				nbArg--;
			}
			size_t index = (nbArg>0) ? std::stoi(arguments.at(0)) : (size_t)-1;


			size_t nbEmdFile = listEmdFile.size();
			if ((index == (size_t)-1) || (index < nbEmdFile))
			{	
				for (size_t i = 0; i < nbEmdFile; i++)
				{
					if ((index != (size_t)-1) && (i != index))
						continue;

					string filename = listEmdFile.at(i)->getName();

					if (index == (size_t)-1)
					{
						filename = filename + "_modified.emd";
					}else{
						if (nbArg>1)
							filename = arguments.at(1);
					}

					listEmdFile.at(i)->save(filename);
					printf("file saved at %s\n", filename.c_str());

					if (index != (size_t)-1)
						break;
				}
			}else{
				printf("index %i is not in emd file list.\n", index);
			}



		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "EMD_GetEmdFileList"){

			size_t nbFile = listEmdFile.size();
			for (size_t i = 0; i < nbFile; i++)
				printf("%i : %s\n", i, listEmdFile.at(i)->getName().c_str());




		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "EMD_GetBoneList"){

			
			if (arguments.at(0) == "")
			{
				arguments.erase(arguments.begin());
				nbArg--;
			}
			size_t index = (nbArg>0) ? std::stoi(arguments.at(0)) : (size_t)-1;


			size_t nbEmdFile = listEmdFile.size();
			if ((index == (size_t)-1) || (index < nbEmdFile))
			{
				for (size_t i = 0; i < nbEmdFile; i++)
				{
					if ((index != (size_t)-1) && (i != index))
						continue;

					std::vector<string> listBones = listEmdFile.at(i)->getBonesNames();

					printf("*** file %s :\n", listEmdFile.at(i)->getName().c_str());

					size_t nbBones = listBones.size();
					for (size_t i = 0; i < nbBones; i++)
						printf("%s\n", listBones.at(i).c_str());

					if (index != (size_t)-1)
						break;
				}
			}else{
				printf("index %i is not in emd file list.\n", index);
			}




		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "EMD_Rename"){

			if (nbArg < 3)
			{
				printf("You miss arguments. try 'Help' command\n");
				continue;
			}
			size_t index = (arguments.size()>0) ? std::stoi(arguments.at(0)) : (size_t)-1;
			string oldName = arguments.at(1);
			string newName = arguments.at(2);

			size_t nbEmdFile = listEmdFile.size();
			if ((index == (size_t)-1) || (index < nbEmdFile))
			{
				for (size_t i = 0; i < nbEmdFile; i++)
				{
					if ((index != (size_t)-1) && (i != index))
						continue;

					listEmdFile.at(i)->replaceBonesNames(oldName, newName);

					if (index != (size_t)-1)
						break;
				}

				printf("Done.\n");
			}else{
				printf("index %i is not in emd file list.\n", index);
			}

			/*
			//TODO 'Rename <indexEsk> <indexBone> <newName> <renameAlsoIntoEmdFile>' notice you could use the name of the bone for indexBone. if you use renameAlsoIntoEmdFile at true, it will also rename bone in all Emd File loaded.\n\
			*/			





			///////////////////////////////////////////////////////////////////////////////////////////////////
		}else if (command == "Save_Fpf_xml") {

			if (nbArg < 3)
			{
				printf("You miss arguments. try 'Help' command\n");
				continue;
			}
			size_t index = std::stoi(arguments.at(0));
			size_t index_b = std::stoi(arguments.at(1));
			string filename = arguments.at(2);
			string extension = LibXenoverse::extensionFromFilename(filename, true);

			if ((index < listEskFile.size())&& (index_b < listEskFile.size()))
			{
				if (extension == "xml") 
				{
					LibXenoverse::ESK* esk = 0;
					LibXenoverse::ESK* esk_b = 0;

					if (listEskFile.at(index)->mEan)
						esk = listEskFile.at(index)->mEan->getSkeleton();
					else
						esk = listEskFile.at(index)->mEsk;

					if (listEskFile.at(index_b)->mEan)
						esk_b = listEskFile.at(index_b)->mEan->getSkeleton();
					else
						esk_b = listEskFile.at(index_b)->mEsk;

					esk->saveFpfXml_Extract(filename, esk_b);
					printf("Esk file saved as Fpf.xml extract %s.\n", filename.c_str());
				}else {
					printf("extension is not correct (.fpf.xml): %s.\n", filename.c_str());
				}
			}else {
				printf("index %i is not in file list.\n", index);
			}


		///////////////////////////////////////////////////////////////////////////////////////////////////
		}else{
			printf("Unknow command *%s*.\n Type 'Help' to see valid commands\n", line.c_str());
		}
	}
	

	printf("Ok. goodbye. See you next time.\n");
	return 0;
}
