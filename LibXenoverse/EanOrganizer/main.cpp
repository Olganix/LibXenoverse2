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

	printf("Welcome to Ean organizer v0.1\n\n");


	string help = "Commands: \n\
		'Help' to see this list\n\
		'Load <path\\filenameEanFile>' load a ean file\n\
		'Save <indexEan> <path\\filename>' save your modified ean file\n\
		'GetEanFileList'\n\
		'GetAnimList <Eanindex>'\n\
		'Copy <indexEan> <indexAnimation>' copy on a sort of clipboard the specified animation.\n\
		'Paste <indexEan> <indexAnimation>' it will erase the previous destination animation by the animation in clipboard. you could add animation by using an out of range index, but it's temporary, just for working on .\n\
		'Rename <indexEan> <indexAnimation> <newName>'\n\
		'Erase <EanIndex> <AnimIndex> <AnimIndex_rangeEnd>' erase only Animation in AnimIndex. but if you use AnimIndex_rangeEnd, it will erase the range between the two animaIndex.\n\
		'Insert <indexEan> <indexAnimation>' insert an animation before index of Animation specified.\n\n\
		'GetBoneList <indexEan>'\n\
		'AddBoneFilter <indexEan> <indexBone1> <indexBone2> <indexBone3> ...' by using the filter, when you paste, all the destination animation wiil be not erased, just only for bone specified in filter. Ex: if you just copy the tails animation but not the body animation, just add bone of tail in filter.\n\
		'AddAllBoneInFilterFor <indexEan> <indexBone1_notIn> <indexBone2_notIn> <indexBone3_notIn> ...' fast version to add all of but not the specified index.\n\
		'GetBoneFilter' list all bones in filter by eanfile\n\
		'ResetBoneFilter' clear the filter\n\
		'PasteWithBoneFilter <indexEan> <indexAnimation>'\n\
		'CopyPasteRange <indexEan_Src> <indexEan_Dest> <indexAnimation_Start> <indexAnimation_End>' fast version of Copy and Paste, on a range of animations\n\
		'CopyPasteRange_WithBoneFilter <indexEan_Src> <indexEan_Dest> <indexAnimation_Start> <indexAnimation_End>' fast version of Copy and Paste, on a range of animations\n\
		'GetDuration <indexEan> <AnimIndex>' Get the duration of a animation in seconds\n\
		'SetDuration <indexEan> <AnimIndex> X.xxx' Set the duration of a animation in second (float)\n\
		'GetDuration <indexEan> <AnimIndex> <indexBone>' Get the duration of a animation for a bone\n\
		'SetDuration <indexEan> <AnimIndex> <indexBone> X.xxx' Set the duration of a animation of a bone in second (float)\n\
		'LoopAnimation <indexEan> <AnimIndex> <indexBone> X' Make X loop for a animation. indexBone = -1 for all bones\n\
		'AddBoneOffsetScaleOnAnimationPosition <indexEan> <AnimIndex> <indexBone> <offsetToAdd_X> <offsetToAdd_Y> <offsetToAdd_Z> <scaleToMultiply_X> <scaleToMultiply_Y> <scaleToMultiply_Z>' (Experimental) Add a offset and multiply by a scale on a bone for a animation. Note : use -1 for AnimIndex to apply on all animations. Neutral Values are offsetToAdd: 0.0, scaleToMultiply: 1.0\n\
		'MatchAnimationDuration <indexEAN_src> <indexEAN_toMatch> <indexAnimation_Start> <indexAnimation_End> ' the second Ean will have the same duration of the same Named animation from the Ean src. For a range of animations. Notice the range is about animations of the second Ean..\
		'Quit'\n";

	printf((string("You could load many Ean File, get the animation list, copy an animation from a file to another with index. KEEP ORDER of animations if you want to use modified ean in game (need configurator).\nFor Paths, please avoid space.\n") + help +"\n").c_str());


	
	vector<LibXenoverse::EAN*> listEanFile;
	vector<string> listFileName;
	LibXenoverse::EANAnimation *mSavedAnimation = 0;
	
	vector<vector<size_t>> mListBoneFilters;
	vector<size_t> mListBoneFilters_eanIndex;

	string line = "";

	while (line.substr(0, 4) != "Quit")
	{
		printf("So, what do you want ?\n");
		line = readLine();


		if (line.substr(0, 4) == "Quit")
		{
			break;
		}else if (line.substr(0, 4) == "Help"){
			printf("%s",help.c_str());

		}else if (line.substr(0, 4) == "Load"){
	
			if (line.size() <= 5)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}

			LibXenoverse::EAN *eanFile = NULL;
			string filename = line.substr(5);

			try{
				eanFile = new LibXenoverse::EAN();
				eanFile->load(filename);
				listEanFile.push_back(eanFile);
				listFileName.push_back(filename);
				printf("Ean file loaded.\n");
			} catch(...){

				printf("Error on try to load %s\n", filename.c_str());
				if (eanFile != NULL)
					delete eanFile;
			}

		}else if (line.substr(0, 4) == "Save"){

			if (line.size() <= 5)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}
			
			std::vector<std::string> sv = split(line.substr(5), ' ');
			size_t index = std::stoi(sv.at(0));

			if (index < listEanFile.size())
			{
				string filename = listFileName.at(index);

				if (sv.size()>1)
					filename = sv.at(1);

				listEanFile.at(index)->save(filename);
				printf("Ean file saved at %s.\n", filename.c_str());
			}else{
				printf("index %i is not in ean file list.\n", index);
			}






		}else if (line.substr(0, 14) == "GetEanFileList"){

			size_t nbFile = listFileName.size();
			for (size_t i = 0; i < nbFile; i++)
				printf("%i : %s\n", i, listFileName.at(i).c_str());

		}else if (line.substr(0, 11) == "GetAnimList"){

			if (line.size() <= 12)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}
			
			std::vector<std::string> sv = split(line.substr(12), ' ');
			size_t index = std::stoi(sv.at(0));

			if (index < listEanFile.size())
			{
				LibXenoverse::EAN *eanFile = listEanFile.at(index);
				
				size_t nbAnim = eanFile->getAnimations().size();
				for (size_t i = 0; i < nbAnim; i++)
					printf("%i : %s\n", i, eanFile->getAnimations().at(i).getName().c_str());
			}else{
				printf("index %i is not in ean file list.\n", index);
			}
		
		}else if (line.substr(0, 11) == "GetBoneList"){

			if (line.size() <= 12)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}
			
			std::vector<std::string> sv = split(line.substr(12), ' ');
			size_t index = std::stoi(sv.at(0));

			if (index < listEanFile.size())
			{
				LibXenoverse::EAN *eanFile = listEanFile.at(index);

				if (eanFile->getSkeleton())
				{
					std::vector<LibXenoverse::ESKBone*> &bones = eanFile->getSkeleton()->getBones();
					size_t nbBone = bones.size();
					for (size_t i = 0; i < nbBone; i++)
						printf("%i : %s\n", i, bones.at(i)->getName().c_str());
				}
				else{
					printf("No skeleton definition in this ean file\n");
				}
			}
			else{
				printf("index %i is not in ean file list.\n", index);
			}

		}else if (line.substr(0, 13) == "GetBoneFilter"){

			size_t nbEanFileFilter = mListBoneFilters.size();

			for (size_t i = 0; i < nbEanFileFilter; i++)
			{
				size_t nbBone = mListBoneFilters.at(i).size();
				for (size_t j = 0; j < nbBone; j++)
					printf("eanfile : %i  boneIndex : %i boneName %s\n", mListBoneFilters_eanIndex.at(i), mListBoneFilters.at(i).at(j), listEanFile.at(mListBoneFilters_eanIndex.at(i))->getSkeleton()->getBones().at(mListBoneFilters.at(i).at(j))->getName().c_str());
			}

			if (nbEanFileFilter == 0)
				printf("filter list is empty.\n");



		}
		else if (line.substr(0, 29) == "CopyPasteRange_WithBoneFilter"){

			if (line.size() <= 30)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}
			
			std::vector<std::string> sv = split(line.substr(30), ' ');
			
			if (sv.size() <= 3)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}
			
			size_t indexFile_src = std::stoi(sv.at(0));
			size_t indexFile_dest = std::stoi(sv.at(1));
			size_t indexAnim_start = std::stoi(sv.at(2));
			size_t indexAnim_end = std::stoi(sv.at(3));

			if (indexAnim_start > indexAnim_end)
			{
				size_t tmp = indexAnim_end;
				indexAnim_end = indexAnim_start;
				indexAnim_start = tmp;
			}

			for (size_t m = indexAnim_start; m <= indexAnim_end; m++)
			{
				size_t indexAnim = m;
				size_t indexFile = indexFile_src;

				printf("****** copy paste for the indexAnim %i ******\n", indexAnim);

				if ((indexFile < listEanFile.size()) && (indexAnim < listEanFile.at(indexFile)->getAnimations().size()))
				{
					LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
					mSavedAnimation = &(eanFile->getAnimations().at(indexAnim));
					printf("animation %s is saved for a futur copy\n", mSavedAnimation->getName().c_str());
				}
				else{
					printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
					continue;
				}

				indexFile = indexFile_dest;
				if (indexFile < listEanFile.size())
				{
					if (mSavedAnimation)
					{
						LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);

						//make a list of name of bone for filter
						vector<string> listBoneFilterNames;
						size_t nbEanFileFilter = mListBoneFilters.size();
						for (size_t i = 0; i < nbEanFileFilter; i++)
						{
							size_t nbBone = mListBoneFilters.at(i).size();
							for (size_t j = 0; j < nbBone; j++)
								listBoneFilterNames.push_back(listEanFile.at(mListBoneFilters_eanIndex.at(i))->getSkeleton()->getBones().at(mListBoneFilters.at(i).at(j))->getName());
						}

						//if miss a animation, go to add one
						if (indexAnim >= eanFile->getAnimations().size())
						{
							printf("indexAnim don't existe, animation will push back of the list (take care of index of animation for file working in game).\n");

							indexAnim = eanFile->getAnimations().size();
							eanFile->getAnimations().push_back(LibXenoverse::EANAnimation(mSavedAnimation, listBoneFilterNames, eanFile));

						}
						else{

							LibXenoverse::EANAnimation &animationToChange = eanFile->getAnimations().at(indexAnim);
							animationToChange.copy(*mSavedAnimation, listBoneFilterNames);
							printf("animation is change by copy of another. we keep old name (Use rename if you want).\n");
						}
					}
					else{
						printf("error : you must use Copy command first, to select the source.\n");
					}
				}
				else{
					printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
				}

			}







		}
		else if (line.substr(0, 14) == "CopyPasteRange"){

			if (line.size() <= 15)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}

			
			std::vector<std::string> sv = split(line.substr(15), ' ');

			if (sv.size() <= 3)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}

			size_t indexFile_src = std::stoi(sv.at(0));
			size_t indexFile_dest = std::stoi(sv.at(1));
			size_t indexAnim_start = std::stoi(sv.at(2));
			size_t indexAnim_end = std::stoi(sv.at(3));

			if (indexAnim_start > indexAnim_end)
			{
				size_t tmp = indexAnim_end;
				indexAnim_end = indexAnim_start;
				indexAnim_start = tmp;
			}

			for (size_t m = indexAnim_start; m <= indexAnim_end; m++)
			{
				size_t indexAnim = m;
				size_t indexFile = indexFile_src;

				printf("****** copy paste for the indexAnim %i ******\n", indexAnim);

				if ((indexFile < listEanFile.size()) && (indexAnim < listEanFile.at(indexFile)->getAnimations().size()))
				{
					LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
					mSavedAnimation = &(eanFile->getAnimations().at(indexAnim));
					printf("animation %s is saved for a futur copy\n", mSavedAnimation->getName().c_str());
				}
				else{
					printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
					continue;
				}


				indexFile = indexFile_dest;

				if (indexFile < listEanFile.size())
				{
					if (mSavedAnimation)
					{
						LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);

						//if miss a animation, go to add one
						if (indexAnim >= eanFile->getAnimations().size())
						{
							printf("indexAnim don't existe, animation will push back of the list (take care of index of animation for file working in game).\n");

							indexAnim = eanFile->getAnimations().size();
							eanFile->getAnimations().push_back(LibXenoverse::EANAnimation(mSavedAnimation, eanFile));
						}
						else{
							LibXenoverse::EANAnimation &animationToChange = eanFile->getAnimations().at(indexAnim);
							animationToChange.copy(*mSavedAnimation);

							printf("animation is change by copy of another. we keep old name (Use rename if you want).\n");
						}
					}
					else{
						printf("error : you must use Copy command first, to select the source.\n");
					}
				}
				else{
					printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
				}

			}




		}else if (line.substr(0, 4) == "Copy"){

			if (line.size() <= 5)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}

			
			std::vector<std::string> sv = split(line.substr(5), ' ');
			
			if (sv.size() <= 1)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}
			
			size_t indexFile = std::stoi(sv.at(0));
			size_t indexAnim = std::stoi(sv.at(1));

			if ((indexFile < listEanFile.size()) && (indexAnim < listEanFile.at(indexFile)->getAnimations().size()))
			{
				LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
				mSavedAnimation = &(eanFile->getAnimations().at(indexAnim));
				printf("animation %s is saved for a futur copy\n", mSavedAnimation->getName().c_str());
			}
			else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}


		}else if (line.substr(0, 19) == "PasteWithBoneFilter"){

			if (line.size() <= 20)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}

			std::vector<std::string> sv = split(line.substr(20), ' ');

			if (sv.size() <= 1)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}

			size_t indexFile = std::stoi(sv.at(0));
			size_t indexAnim = std::stoi(sv.at(1));

			if (indexFile < listEanFile.size()) 
			{
				if (mSavedAnimation)
				{
					LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);

					//make a list of name of bone for filter
					vector<string> listBoneFilterNames;
					size_t nbEanFileFilter = mListBoneFilters.size();
					for (size_t i = 0; i < nbEanFileFilter; i++)
					{
						size_t nbBone = mListBoneFilters.at(i).size();
						for (size_t j = 0; j < nbBone; j++)
							listBoneFilterNames.push_back(listEanFile.at(mListBoneFilters_eanIndex.at(i))->getSkeleton()->getBones().at(mListBoneFilters.at(i).at(j) )->getName());
					}

					//if miss a animation, go to add one
					if (indexAnim >= eanFile->getAnimations().size())
					{
						printf("indexAnim don't existe, animation will push back of the list (take care of index of animation for file working in game).\n");

						indexAnim = eanFile->getAnimations().size();
						eanFile->getAnimations().push_back(LibXenoverse::EANAnimation(mSavedAnimation, listBoneFilterNames, eanFile));

					}else{

						LibXenoverse::EANAnimation &animationToChange = eanFile->getAnimations().at(indexAnim);
						animationToChange.copy(*mSavedAnimation, listBoneFilterNames);
						printf("animation is change by copy of another. we keep old name (Use rename if you want).\n");
					}
				}
				else{
					printf("error : you must use Copy command first, to select the source.\n");
				}
			}
			else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}


		}else if (line.substr(0, 5) == "Paste"){


			if (line.size() <= 6)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}

			std::vector<std::string> sv = split(line.substr(6), ' ');
			
			if (sv.size() <= 1)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}
			
			size_t indexFile = std::stoi(sv.at(0));
			size_t indexAnim = std::stoi(sv.at(1));

			if (indexFile < listEanFile.size())
			{
				if (mSavedAnimation)
				{
					LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);

					//if miss a animation, go to add one
					if (indexAnim >= eanFile->getAnimations().size())
					{
						printf("indexAnim don't existe, animation will push back of the list (take care of index of animation for file working in game).\n");
						
						indexAnim = eanFile->getAnimations().size();
						eanFile->getAnimations().push_back(LibXenoverse::EANAnimation(mSavedAnimation, eanFile));
					}
					else{
						LibXenoverse::EANAnimation &animationToChange = eanFile->getAnimations().at(indexAnim);
						animationToChange.copy(*mSavedAnimation);

						printf("animation is change by copy of another. we keep old name (Use rename if you want).\n");
					}	
				}
				else{
					printf("error : you must use Copy command first, to select the source.\n");
				}
			}
			else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}

		
		

		}else if (line.substr(0, 15) == "ResetBoneFilter"){

			mListBoneFilters.clear();
			mListBoneFilters_eanIndex.clear();
		
			
		}else if (line.substr(0, 13) == "AddBoneFilter"){

			
			if (line.size() <= 14)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}

			std::vector<std::string> sv = split(line.substr(14), ' ');
			
			if (sv.size() <= 0)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}
			
			size_t indexFile = std::stoi(sv.at(0));
			
			size_t nbEanFileFilter = mListBoneFilters.size();
			size_t isfound = (size_t)-1;
			for (size_t i = 0; i < nbEanFileFilter; i++)
			{
				if (mListBoneFilters_eanIndex.at(i) == indexFile)
				{
					isfound = i;
					break;
				}
			}

			if (isfound==(size_t)-1)
			{
				isfound = mListBoneFilters_eanIndex.size();
				mListBoneFilters_eanIndex.push_back(indexFile);
				mListBoneFilters.push_back(vector<size_t>());
			}

			size_t nbBoneArg = sv.size();
			for (size_t j = 1; j < nbBoneArg; j++)
			{
				size_t indexBone = std::stoi(sv.at(j));

				size_t nbBone = mListBoneFilters.at(isfound).size();
				bool isfound_2 = false;
				for (size_t i = 0; i < nbEanFileFilter; i++)
				{
					if (mListBoneFilters.at(isfound).at(i) == indexBone)
					{
						isfound_2 = true;
						break;
					}
				}
				if (!isfound_2)
					mListBoneFilters.at(isfound).push_back(indexBone);
			}

		}
		else if (line.substr(0, 21) == "AddAllBoneInFilterFor"){

			if (line.size() <= 22)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}
			
			std::vector<std::string> sv = split(line.substr(22), ' ');
			
			if (sv.size() <= 0)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}

			size_t indexFile = std::stoi(sv.at(0));

			size_t nbEanFileFilter = mListBoneFilters.size();
			size_t isfound = (size_t)-1;
			for (size_t i = 0; i < nbEanFileFilter; i++)
			{
				if (mListBoneFilters_eanIndex.at(i) == indexFile)
				{
					isfound = i;
					break;
				}
			}

			if (isfound == (size_t)-1)
			{
				isfound = mListBoneFilters_eanIndex.size();
				mListBoneFilters_eanIndex.push_back(indexFile);
				mListBoneFilters.push_back(vector<size_t>());
			}


			if (indexFile < listEanFile.size())
			{
				LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);

				size_t  nbBone = eanFile->getSkeleton()->getBones().size();
				size_t nbBoneArg = sv.size();
				for (size_t k = 0; k < nbBone; k++)
				{
					bool isfound_index = false;
					for (size_t j = 1; j < nbBoneArg; j++)
					{
						size_t indexBone = std::stoi(sv.at(j));
						if (indexBone == k)
						{
							isfound_index = true;
							break;
						}
					}

					if (!isfound_index)					//could add
					{
						size_t nbBone = mListBoneFilters.at(isfound).size();
						bool isfound_2 = false;
						for (size_t i = 0; i < nbEanFileFilter; i++)
						{
							if (mListBoneFilters.at(isfound).at(i) == k)
							{
								isfound_2 = true;
								break;
							}
						}
						if (!isfound_2)
							mListBoneFilters.at(isfound).push_back(k);
					}
				}
			}

		}else if (line.substr(0, 6) == "Rename"){

			if (line.size() <= 7)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}

			std::vector<std::string> sv = split(line.substr(7), ' ');
			
			if (sv.size() <= 2)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}


			size_t indexFile = std::stoi(sv.at(0));
			size_t indexAnim = std::stoi(sv.at(1));
			string newName = sv.at(2);

			if ((indexFile < listEanFile.size()) && (indexAnim < listEanFile.at(indexFile)->getAnimations().size()))
			{
					LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
					eanFile->getAnimations().at(indexAnim).setName(newName);
					printf("Name changed.\n");
			}
			else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}

		}else if (line.substr(0, 5) == "Erase"){

			
			if (line.size() <= 6)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}
			
			std::vector<std::string> sv = split(line.substr(6), ' ');
			
			if (sv.size() <= 1)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}

			
			size_t indexFile = std::stoi(sv.at(0));
			size_t indexAnim = std::stoi(sv.at(1));

			size_t indexAnim_range_end = (size_t)-1;
			if (sv.size()>2)
				indexAnim_range_end = std::stoi(sv.at(2));

			if ((indexFile < listEanFile.size()) && (indexAnim < listEanFile.at(indexFile)->getAnimations().size()))
			{
				LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
				if ((indexAnim_range_end != (size_t)-1) && (indexAnim_range_end>indexAnim))
				{
					for (size_t i = indexAnim_range_end; i >= indexAnim; i--)
					{
						eanFile->getAnimations().erase(eanFile->getAnimations().begin() + i);
						printf("Animation %i Erased.\n", i);
					}

				}else{
					eanFile->getAnimations().erase(eanFile->getAnimations().begin() + indexAnim);
					printf("Animation Erased.\n");
				}
			}
			else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}


		}
		else if (line.substr(0, 6) == "Insert"){

			if (line.size() <= 7)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}
			
			std::vector<std::string> sv = split(line.substr(7), ' ');
			
			if (sv.size() <= 1)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}
			
			size_t indexFile = std::stoi(sv.at(0));
			size_t indexAnim = std::stoi(sv.at(1));

			if ((indexFile < listEanFile.size()) && (indexAnim <= listEanFile.at(indexFile)->getAnimations().size()))
			{
				if (mSavedAnimation)
				{
					LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);

					if (indexAnim == eanFile->getAnimations().size())
					{
						eanFile->getAnimations().push_back(LibXenoverse::EANAnimation(mSavedAnimation, eanFile));
					}else{
						eanFile->getAnimations().insert(eanFile->getAnimations().begin() + indexAnim, (LibXenoverse::EANAnimation(mSavedAnimation, eanFile)));
					}

					printf("animation added.\n");
				}
				else{
					printf("error : you must use Copy command first, to select the source.\n");
				}
			}
			else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}

		





		}else if (line.substr(0, 37) == "AddBoneOffsetScaleOnAnimationPosition"){

			
			if (line.size() <= 38)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}
			
			std::vector<std::string> sv = split(line.substr(38), ' ');
			
			if (sv.size() <= 8)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}
			
			size_t indexFile = std::stoi(sv.at(0));
			size_t indexAnim = std::stoi(sv.at(1));
			size_t indexBone = std::stoi(sv.at(2));

			float offsetToAdd_X = std::stof(sv.at(3));
			float offsetToAdd_Y = std::stof(sv.at(4));
			float offsetToAdd_Z = std::stof(sv.at(5));
			float scaletoMultiply_X = std::stof(sv.at(6));
			float scaletoMultiply_Y = std::stof(sv.at(7));
			float scaletoMultiply_Z = std::stof(sv.at(8));

			if ((indexFile < listEanFile.size()) && ((indexAnim==-1)||(indexAnim < listEanFile.at(indexFile)->getAnimations().size())))
			{
				LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
				
				vector<LibXenoverse::EANAnimation> &listAnimation = eanFile->getAnimations();
				
				size_t nbAnimations = listAnimation.size();
				for (size_t m = 0; m < nbAnimations; m++)
				{
					if ((m != indexAnim) && (indexAnim != -1))
						continue;

					LibXenoverse::EANAnimation *animation = &(listAnimation.at(m));
					vector<LibXenoverse::EANAnimationNode> &listNodes = animation->getNodes();

					bool isfound = false;
					size_t nbBones = listNodes.size();
					for (size_t i = 0; i < nbBones; i++)
					{
						if (listNodes.at(i).getBoneIndex() != indexBone)
							continue;

						isfound = true;

						size_t nblastKeyframe = 0;
						vector<LibXenoverse::EANKeyframedAnimation> &listKeyFramesAnim = listNodes.at(i).getKeyframed_animations();
						size_t nbKeyFramesAnim = listKeyFramesAnim.size();
						for (size_t j = 0; j < nbKeyFramesAnim; j++)
						{
							LibXenoverse::EANKeyframedAnimation &keyframeAnim = listKeyFramesAnim.at(j);
							if (keyframeAnim.getFlag() != LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION)	//only work on position.
								continue;

							vector<LibXenoverse::EANKeyframe> &listKeyFrames = keyframeAnim.getKeyframes();
							size_t nbKeyFrames = listKeyFrames.size();
							for (size_t k = 0; k < nbKeyFrames; k++)
							{
								LibXenoverse::EANKeyframe &eanKeyframe = listKeyFrames.at(k);
								eanKeyframe.setXYZW(eanKeyframe.x * scaletoMultiply_X + offsetToAdd_X, eanKeyframe.y * scaletoMultiply_Y + offsetToAdd_Y, eanKeyframe.z * scaletoMultiply_Z + offsetToAdd_Z, eanKeyframe.w);
							}
						}

						if (!isfound)
							printf("animation %s don't have bone with index %i. Use GetBoneList command to look the bone present in this animation.\n", animation->getName().c_str(), indexBone);
					}

				}

				printf("Done.\n");

			}
			else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}




		

		}else if (line.substr(0, 11) == "GetDuration"){

			if (line.size() <= 12)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}

			
			std::vector<std::string> sv = split(line.substr(12), ' ');
			
			if (sv.size() <= 1)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}
			
			size_t indexFile = std::stoi(sv.at(0));
			size_t indexAnim = std::stoi(sv.at(1));

			size_t indexBone = (size_t)-1;
			if (sv.size()>2)
				indexBone = std::stoi(sv.at(2));


			if ((indexFile < listEanFile.size()) && (indexAnim < listEanFile.at(indexFile)->getAnimations().size()))
			{
				LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
				LibXenoverse::EANAnimation *animation = &(eanFile->getAnimations().at(indexAnim));
				
				if (indexBone == (size_t)-1)
				{
					printf("animation %s have %i frames (60 fps) so %f second\n", animation->getName().c_str(), animation->getFrameCount(), animation->getFrameCount() * (1.0 / 60.0));
				}else{

					vector<LibXenoverse::EANAnimationNode> &listNodes = animation->getNodes();
					size_t nbBones = listNodes.size();
					bool isfound = false;
					for (size_t i = 0; i < nbBones; i++)
					{
						if (listNodes.at(i).getBoneIndex() != indexBone)
							continue;

						isfound = true;
						
						size_t nblastKeyframe = 0;
						vector<LibXenoverse::EANKeyframedAnimation> &listKeyFramesAnim = listNodes.at(i).getKeyframed_animations();
						size_t nbKeyFramesAnim = listKeyFramesAnim.size();
						for (size_t j = 0; j < nbKeyFramesAnim; j++)
						{
							LibXenoverse::EANKeyframedAnimation &keyframeAnim = listKeyFramesAnim.at(j);
							
							vector<LibXenoverse::EANKeyframe> &listKeyFrames = keyframeAnim.getKeyframes();
							size_t nbKeyFrames = listKeyFrames.size();
							for (size_t k = 0; k < nbKeyFrames; k++)
							{
								size_t frameNumber = listKeyFrames.at(k).getFrame();
								if (frameNumber > nblastKeyframe)
									nblastKeyframe = frameNumber;
							}
						}
						
						printf("animation %s for bone index %i have %i frames (60 fps) so %f second\n", animation->getName().c_str(), indexBone, nblastKeyframe, nblastKeyframe * (1.0 / 60.0));
						break;
					}
					if (!isfound)
						printf("animation %s don't have bone with index %i. Use GetBoneList command to look the bone present in this animation.\n", animation->getName().c_str(), indexBone);
				}
			}
			else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}

	
		}
		else if (line.substr(0, 11) == "SetDuration"){

			if (line.size() <= 12)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}

			std::vector<std::string> sv = split(line.substr(12), ' ');
			
			if (sv.size() <= 1)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}
			
			size_t indexFile = std::stoi(sv.at(0));
			size_t indexAnim = std::stoi(sv.at(1));

			double duration = 2.0;
			size_t indexBone = (size_t)-1;
			if (sv.size() > 3)
			{
				indexBone = std::stoi(sv.at(2));
				duration = std::stod(sv.at(3));
			}
			else if (sv.size() > 2){
				duration = std::stod(sv.at(2));
			}

			struct InterpolatedKey
			{
				float x;
				float y;
				float z;
				float w;

				InterpolatedKey(float x, float y, float z, float w)
				{
					this->x = x;
					this->y = y;
					this->z = z;
					this->w = w;
				}
			};

			if ((indexFile < listEanFile.size()) && (indexAnim < listEanFile.at(indexFile)->getAnimations().size()))
			{
				LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
				LibXenoverse::EANAnimation *animation = &(eanFile->getAnimations().at(indexAnim));

				size_t original_Duration_inFrames = (size_t)-1;
				size_t target_Duration_inFrames = (size_t)round(duration / (1.0 / 60.0));
				if (target_Duration_inFrames == 0)
					target_Duration_inFrames = 1;


				if (indexBone == (size_t)-1)
				{
					original_Duration_inFrames = animation->getFrameCount();

				}
				else{

					vector<LibXenoverse::EANAnimationNode> &listNodes = animation->getNodes();
					size_t nbBones = listNodes.size();
					bool isfound = false;
					for (size_t i = 0; i < nbBones; i++)
					{
						if (listNodes.at(i).getBoneIndex() != indexBone)
							continue;

						isfound = true;

						size_t nblastKeyframe = 0;
						vector<LibXenoverse::EANKeyframedAnimation> &listKeyFramesAnim = listNodes.at(i).getKeyframed_animations();
						size_t nbKeyFramesAnim = listKeyFramesAnim.size();
						for (size_t j = 0; j < nbKeyFramesAnim; j++)
						{
							LibXenoverse::EANKeyframedAnimation &keyframeAnim = listKeyFramesAnim.at(j);

							vector<LibXenoverse::EANKeyframe> &listKeyFrames = keyframeAnim.getKeyframes();
							size_t nbKeyFrames = listKeyFrames.size();
							for (size_t k = 0; k < nbKeyFrames; k++)
							{
								size_t frameNumber = listKeyFrames.at(k).getFrame();
								if (frameNumber > nblastKeyframe)
									nblastKeyframe = frameNumber;
							}
						}

						original_Duration_inFrames = nblastKeyframe;
						break;
					}
					if (!isfound)
					{
						printf("animation %s don't have bone with index %i. Use GetBoneList command to look the bone present in this animation.\n", animation->getName().c_str(), indexBone);
						continue;
					}
				}

				double factor = ((double)target_Duration_inFrames) / ((double)original_Duration_inFrames);
				printf("Ok. it's about resample %i frames (%f seconds) into %i frames (%f seconds => factor of %f on keys)\nLet's go\n", original_Duration_inFrames, ((double)original_Duration_inFrames)*(1.0 / 60.0), target_Duration_inFrames, ((double)target_Duration_inFrames)*(1.0 / 60.0), factor);

				if (indexBone == (size_t)-1)
					animation->setFrameCount(target_Duration_inFrames);

				vector<LibXenoverse::EANAnimationNode> &listNodes = animation->getNodes();
				size_t nbBones = listNodes.size();
				for (size_t i = 0; i < nbBones; i++)
				{
					if ((indexBone != (size_t)-1) && (listNodes.at(i).getBoneIndex() != indexBone))
						continue;


					size_t nblastKeyframe = 0;
					vector<LibXenoverse::EANKeyframedAnimation> &listKeyFramesAnim = listNodes.at(i).getKeyframed_animations();
					size_t nbKeyFramesAnim = listKeyFramesAnim.size();
					for (size_t j = 0; j < nbKeyFramesAnim; j++)
					{
						LibXenoverse::EANKeyframedAnimation &keyframeAnim = listKeyFramesAnim.at(j);

						size_t flag = keyframeAnim.getFlag();



						//first, we have to create interpolation
						std::vector<InterpolatedKey> listInterpolKey;
						float x, y, z, w;
						for (size_t k = 0; k < target_Duration_inFrames; k++)
						{
							listNodes.at(i).getInterpolatedFrame((unsigned int)round(((double)k) / factor), flag, x, y, z, w);

							listInterpolKey.push_back(InterpolatedKey(x, y, z, w));
						}

						//next, we replace for the flag
						vector<LibXenoverse::EANKeyframe> &listKeyFrames = keyframeAnim.getKeyframes();		//get a copy
						listKeyFrames.clear();

						for (size_t k = 0; k < target_Duration_inFrames; k++)				//change the frame number with the factor
						{
							InterpolatedKey &pos = listInterpolKey.at(k);
							listKeyFrames.push_back(LibXenoverse::EANKeyframe(k, pos.x, pos.y, pos.z, pos.w));
						}
					}

					if (indexBone != (size_t)-1)
						break;
				}

				printf("Done.\n");

			}
			else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}





		}
		else if (line.substr(0, 13) == "LoopAnimation"){

			
			if (line.size() <= 14)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}

			std::vector<std::string> sv = split(line.substr(14), ' ');

			if (sv.size() <= 3)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}

			size_t indexFile = std::stoi(sv.at(0));
			size_t indexAnim = std::stoi(sv.at(1));

			int nbLoops = 2;
			size_t indexBone = (size_t)-1;
			if (sv.size() > 3)
			{
				indexBone = std::stoi(sv.at(2));
				nbLoops = std::stoi(sv.at(3));
			}
			else if (sv.size() > 2){
				nbLoops = std::stoi(sv.at(2));
			}

			if (nbLoops <= 1)
			{
				printf("minimum loop parameter is 1. skipped\n");
				continue;
			}


			if ((indexFile < listEanFile.size()) && (indexAnim < listEanFile.at(indexFile)->getAnimations().size()))
			{
				LibXenoverse::EAN *eanFile = listEanFile.at(indexFile);
				LibXenoverse::EANAnimation *animation = &(eanFile->getAnimations().at(indexAnim));

				size_t original_Duration_inFrames = (size_t)-1;
				
				if (indexBone == (size_t)-1)
				{
					original_Duration_inFrames = animation->getFrameCount();

				}
				else{

					vector<LibXenoverse::EANAnimationNode> &listNodes = animation->getNodes();
					size_t nbBones = listNodes.size();
					bool isfound = false;
					for (size_t i = 0; i < nbBones; i++)
					{
						if (listNodes.at(i).getBoneIndex() != indexBone)
							continue;

						isfound = true;

						size_t nblastKeyframe = 0;
						vector<LibXenoverse::EANKeyframedAnimation> &listKeyFramesAnim = listNodes.at(i).getKeyframed_animations();
						size_t nbKeyFramesAnim = listKeyFramesAnim.size();
						for (size_t j = 0; j < nbKeyFramesAnim; j++)
						{
							LibXenoverse::EANKeyframedAnimation &keyframeAnim = listKeyFramesAnim.at(j);

							vector<LibXenoverse::EANKeyframe> &listKeyFrames = keyframeAnim.getKeyframes();
							size_t nbKeyFrames = listKeyFrames.size();
							for (size_t k = 0; k < nbKeyFrames; k++)
							{
								size_t frameNumber = listKeyFrames.at(k).getFrame();
								if (frameNumber > nblastKeyframe)
									nblastKeyframe = frameNumber;
							}
						}

						original_Duration_inFrames = nblastKeyframe + 1;
						break;
					}
					if (!isfound)
					{
						printf("animation %s don't have bone with index %i. Use GetBoneList command to look the bone present in this animation.\n", animation->getName().c_str(), indexBone);
						continue;
					}
				}



				size_t target_Duration_inFrames = original_Duration_inFrames * nbLoops;
				if (target_Duration_inFrames == 0)
					target_Duration_inFrames = 1;

				
				if (target_Duration_inFrames>animation->getFrameCount())
					animation->setFrameCount(target_Duration_inFrames);

				vector<LibXenoverse::EANAnimationNode> &listNodes = animation->getNodes();
				size_t nbBones = listNodes.size();
				for (size_t i = 0; i < nbBones; i++)
				{
					if ((indexBone != (size_t)-1) && (listNodes.at(i).getBoneIndex() != indexBone))
						continue;


					size_t nblastKeyframe = 0;
					vector<LibXenoverse::EANKeyframedAnimation> &listKeyFramesAnim = listNodes.at(i).getKeyframed_animations();
					size_t nbKeyFramesAnim = listKeyFramesAnim.size();
					for (size_t j = 0; j < nbKeyFramesAnim; j++)
					{
						LibXenoverse::EANKeyframedAnimation &keyframeAnim = listKeyFramesAnim.at(j);

						size_t flag = keyframeAnim.getFlag();


						//next, we replace for the flag
						vector<LibXenoverse::EANKeyframe> &listKeyFrames = keyframeAnim.getKeyframes();		//get a copy

						size_t currentNbFrames = listKeyFrames.size();
						size_t targetNbFrames = currentNbFrames * nbLoops;
						size_t currentLoops = 0;

						for (size_t k = currentNbFrames; k < targetNbFrames; k++)				//change the frame number with the factor
						{
							LibXenoverse::EANKeyframe &originalFrame = listKeyFrames.at(k % currentNbFrames);							
							currentLoops = (size_t)floor(k / currentNbFrames);
							listKeyFrames.push_back(LibXenoverse::EANKeyframe(originalFrame.getFrame() + currentLoops * original_Duration_inFrames, originalFrame.x, originalFrame.y, originalFrame.z, originalFrame.w));
						}
					}

					if (indexBone != (size_t)-1)
						break;
				}

				printf("Done.\n");

			}
			else{
				printf("index %i is not in ean file list or indexAnim %i is not in list animations.\n", indexFile, indexAnim);
			}




		}else if (line.substr(0, 22) == "MatchAnimationDuration"){

			//'MatchAnimationDuration <indexEAN_src> <indexEAN_toMatch> <indexAnimation_Start> <indexAnimation_End> ' the second Ean will have the same duration of the same Named animation from the Ean src.For a range of animations.Notice the range is about animations of the second Ean.\
			
			if (line.size() <= 23)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}

			std::vector<std::string> sv = split(line.substr(23), ' ');
			
			if (sv.size() <= 3)
			{
				printf("You miss argument. try 'Help' command\n");
				continue;
			}
			
			size_t indexFile_src = std::stoi(sv.at(0));
			size_t indexFile_dest = std::stoi(sv.at(1));
			size_t indexAnim_start = std::stoi(sv.at(2));
			size_t indexAnim_end = std::stoi(sv.at(3));

			if (indexAnim_end < indexAnim_start)
			{
				size_t tmp = indexAnim_end;
				indexAnim_end = indexAnim_start;
				indexAnim_start = tmp;
			}

			struct InterpolatedKey
			{
				float x;
				float y;
				float z;
				float w;

				InterpolatedKey(float x, float y, float z, float w)
				{
					this->x = x;
					this->y = y;
					this->z = z;
					this->w = w;
				}
			};

			
			if ((indexFile_src < listEanFile.size()) && (indexFile_dest < listEanFile.size()) && (indexAnim_start < listEanFile.at(indexFile_dest)->getAnimations().size()) && (indexAnim_end < listEanFile.at(indexFile_dest)->getAnimations().size()))
			{
				LibXenoverse::EAN *eanFile_src = listEanFile.at(indexFile_src);
				LibXenoverse::EAN *eanFile_dest = listEanFile.at(indexFile_dest);


				for (size_t animIndex = indexAnim_start; animIndex <= indexAnim_end; animIndex++)
				{
					LibXenoverse::EANAnimation *animation = &(eanFile_dest->getAnimations().at(animIndex));
					string animationName = animation->getName();
					printf("******* animation %i : %s\n", animIndex, animationName.c_str());
					
					
					string animationName_withoutChara = animationName;					//we can't match animation by name because of chara part : "APL_xxx", so we skip the first part.
					if (animationName_withoutChara.find('_') != std::string::npos)
						animationName_withoutChara = animationName_withoutChara.substr(animationName_withoutChara.find('_'));
					
					// first: find duration
					// as we don't have filter on bones, we can take the frame number directly.
					vector<LibXenoverse::EANAnimation> &listSrcAnima = eanFile_src->getAnimations();
					LibXenoverse::EANAnimation *animation_src = 0;

					string animationName_withoutChara_b = "";
					size_t nbSrcAnim = listSrcAnima.size();
					for (size_t i = 0; i < nbSrcAnim; i++)
					{
						animationName_withoutChara_b = listSrcAnima.at(i).getName();
						if (animationName_withoutChara_b.find('_') != std::string::npos)
							animationName_withoutChara_b = animationName_withoutChara_b.substr(animationName_withoutChara_b.find('_'));

						if (animationName_withoutChara_b == animationName_withoutChara)
						{
							animation_src = &(listSrcAnima.at(i));
							break;
						}
					}

					if (animation_src == 0)
					{
						printf("Warning there isn't animation named %s in Ean_scr. skipped\n", animation->getName().c_str());
						continue;
					}
					else{
						printf("animation %s is found in Ean_scr and have %i frames (60 fps) so %f second\n", animation_src->getName().c_str(), animation_src->getFrameCount(), animation_src->getFrameCount() * (1.0 / 60.0));
					}
					

					
					



					size_t original_Duration_inFrames = animation->getFrameCount();			//same : no filter on bones, so is faster.
					size_t target_Duration_inFrames = animation_src->getFrameCount();
					if (target_Duration_inFrames == 0)
						target_Duration_inFrames = 1;
					if (original_Duration_inFrames == 0)
						original_Duration_inFrames = 1;
					
					double factor = ((double)target_Duration_inFrames) / ((double)original_Duration_inFrames);
					printf("Ok. it's about resample %i frames (%f seconds) into %i frames (%f seconds => factor of %f on keys)\nLet's go\n", original_Duration_inFrames, ((double)original_Duration_inFrames)*(1.0 / 60.0), target_Duration_inFrames, ((double)target_Duration_inFrames)*(1.0 / 60.0), factor);

					animation->setFrameCount(target_Duration_inFrames);

					vector<LibXenoverse::EANAnimationNode> &listNodes = animation->getNodes();
					size_t nbBones = listNodes.size();
					for (size_t i = 0; i < nbBones; i++)
					{
						size_t nblastKeyframe = 0;
						vector<LibXenoverse::EANKeyframedAnimation> &listKeyFramesAnim = listNodes.at(i).getKeyframed_animations();
						size_t nbKeyFramesAnim = listKeyFramesAnim.size();
						for (size_t j = 0; j < nbKeyFramesAnim; j++)
						{
							LibXenoverse::EANKeyframedAnimation &keyframeAnim = listKeyFramesAnim.at(j);

							size_t flag = keyframeAnim.getFlag();



							//first: we hae to create interpolation
							std::vector<InterpolatedKey> listInterpolKey;
							float x, y, z, w;
							for (size_t k = 0; k < target_Duration_inFrames; k++)
							{
								listNodes.at(i).getInterpolatedFrame((unsigned int)round(((double)k) / factor), flag, x, y, z, w);

								listInterpolKey.push_back(InterpolatedKey(x, y, z, w));
							}

							//next, we replace for the flag
							vector<LibXenoverse::EANKeyframe> &listKeyFrames = keyframeAnim.getKeyframes();		//get a copy
							listKeyFrames.clear();

							for (size_t k = 0; k < target_Duration_inFrames; k++)				//change the frame number with the factor
							{
								InterpolatedKey &pos = listInterpolKey.at(k);
								listKeyFrames.push_back(LibXenoverse::EANKeyframe(k, pos.x, pos.y, pos.z, pos.w));
							}
						}
					}


				}
				printf("Done.\n");

			}
			else{
				printf("index %i or %i is not in ean file list or indexAnim %i or %i is not in list animations.\n", indexAnim_start, indexAnim_end, indexFile_src, indexFile_dest);
			}

		}else{
			printf("Unknow command *%s*.\n Type 'Help' to see valid commands\n", line.c_str());
		}
	}
	

	printf("Ok. goodbye. See you next time.\n");
	return 0;
}