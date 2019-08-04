
namespace LibXenoverse
{












/*-------------------------------------------------------------------------------\
|                             load								                 |
\-------------------------------------------------------------------------------*/
bool EMA_Material::load(string filename)
{
	name = nameFromFilenameNoExtension(filename, true);
	
	string extension = LibXenoverse::extensionFromFilename(filename, true);
	string filename_NoExt = filename.substr(0, filename.length() - extension.length() -1);
	string extension2 = LibXenoverse::extensionFromFilename(filename_NoExt, true);
	filename_NoExt = filename.substr(0, filename_NoExt.length() - extension2.length() - 1);


	if ((extension != "ema") || (extension2 != "mat"))
	{
		printf("Error on %s : only .mat.ema extension is valid.\n", filename.c_str());
		LibXenoverse::notifyError();
		return false;
	}


	EMA ema;
	EMM emm;
	if(!ema.LoadFromFile(filename))
		return false;
	
	if (!emm.load(filename_NoExt + ".emm"))
	{
		printf("Error on %s : the emm file needed in the same directory.\n", (filename_NoExt + ".emm").c_str());
		LibXenoverse::notifyError();
		return false;
	}

	readEmaEmm(&ema, &emm);

	return true;
}
/*-------------------------------------------------------------------------------\
|                             save					                             |
\-------------------------------------------------------------------------------*/
void EMA_Material::save(string filename, bool big_endian)
{
	string extension = LibXenoverse::extensionFromFilename(filename, true);
	string filename_NoExt = filename.substr(0, filename.length() - extension.length() - 1);
	string extension2 = LibXenoverse::extensionFromFilename(filename_NoExt, true);
	filename_NoExt = filename.substr(0, filename_NoExt.length() - extension2.length() - ((extension2.length()!=0) ? 1 : 0) );
	
	if ((extension != "ema") || (extension2 != "mat"))
	{
		printf("Error on %s : only .mat.ema.xml extension is valid.\n", filename.c_str());
		LibXenoverse::notifyError();
		return;
	}

	
	EMM emm;
	if (!emm.load(filename_NoExt + ".emm"))
	{
		printf("Error on %s : the emm file needed in the same directory.\n", (filename_NoExt + ".emm").c_str());
		LibXenoverse::notifyError();
		return;
	}

	EMA ema;
	writeEma(&ema, &emm);

	ema.SaveToFile(filename);
}








/*-------------------------------------------------------------------------------\
|                             readEmaEmm			                             |
\-------------------------------------------------------------------------------*/
void EMA_Material::readEmaEmm(EMA* ema, EMM* emm)
{
	unk_08 = ema->unk_08;
	unk_12 = ema->unk_12;

	size_t nbAnimations = ema->animations.size();
	for (size_t i = 0; i < nbAnimations; i++)
	{
		animations.push_back(EMA_Material_Animation());
		animations.back().readEmaEmm(&ema->animations.at(i), &ema->bones, emm);
	}
}
/*-------------------------------------------------------------------------------\
|                             writeEma				                             |
\-------------------------------------------------------------------------------*/
void EMA_Material::writeEma(EMA* ema, EMM* emm)
{
	ema->unk_08 = unk_08;
	ema->unk_12 = unk_12;

	std::vector<EMO_Bone> &bones = ema->bones;

	size_t nbMat = 0;
	size_t nbAnimations = animations.size();
	for (size_t i = 0; i < nbAnimations; i++)
	{
		nbMat = animations.at(i).materials.size();
		for (size_t j = 0; j < nbMat; j++)
		{
			string boneName = animations.at(i).materials.at(j).name;

			size_t nbBones = bones.size();
			bool isfound = false;
			for (size_t k = 0; k < nbBones; k++)
			{
				if (bones.at(k).GetName()==boneName)
				{
					isfound = true;
					break;
				}
			}
			if (!isfound)
			{
				bones.push_back(EMO_Bone());
				bones.back().SetName(boneName);
			}
		}
	}

	for (size_t i = 0; i < nbAnimations; i++)
	{
		ema->animations.push_back(EmaAnimation());
		animations.at(i).writeEma(&ema->animations.back(), ema, emm);
	}
}







/*-------------------------------------------------------------------------------\
|                             readEmaEmm			                             |
\-------------------------------------------------------------------------------*/
void EMA_Material_Animation::readEmaEmm(EmaAnimation* ema, std::vector<EMO_Bone>* bones, EMM* emm)
{
	name = ema->name;
	frame_count = ema->duration;
	type = ema->type;
	frame_float_size = ema->frame_float_size;


	//it could have differences between the name of the material into Ema and Emm (Ex: Bas_B2/BasB2 or aCor/Cor).
	//to do the link, it apparently deal with order on bones for ema and on material into emm.
	//to do the good for after, we will use the name into emm for EMA_Material.
	std::vector<EMMMaterial*> &emmMaterials = emm->getMaterials();
	size_t nbEmmMaterial = emmMaterials.size();
	for (size_t i = 0; i < nbEmmMaterial; i++)
	{
		printf("Emm Material founded : %s\n", emmMaterials.at(i)->getName().c_str());
		materials.push_back(EMA_Material_Material(emmMaterials.at(i)->getName()));
	}


	size_t nbBones_tmp = bones->size();
	for (size_t j = 0; j < nbBones_tmp; j++)
		printf("mat.Ema's bones (Material) founded : %s\n", bones->at(j).GetName().c_str());
	






	//Command is about a couple Bone+Transform+TransformComponent,
	//but Ema_material is only material, and under, is about parameter (MatColor0, MatColor1, textureScroll0, textureScroll1).
	//so the first we have to do is to regroupe command by Bone, transform and transformComponent. witch will easiers to use.
	struct CmdBoneTransformComponent
	{
		size_t transformComponentId;								//0:X or R, 1:Y or G, 2:Z or B, 3:W or A
		EmaCommand* command;
	};
	struct CmdBoneTransform
	{
		size_t transformId;											//0:MatColor0, 1:MatColor1, 4:textureScroll0, 5:textureScroll1 as we know
		std::vector<CmdBoneTransformComponent> list;
	};
	struct CmdBone
	{
		EMO_Bone* emo_Bone;
		std::vector<CmdBoneTransform> list;
	};
	std::vector<CmdBone> listByBone;



	EmaCommand* command;
	CmdBone* cmdBone;
	CmdBoneTransform* cmdBoneTransform;
	CmdBoneTransformComponent* cmdBoneTransformComponent;
	EMO_Bone* emo_Bone;
	size_t boneIndex;
	size_t nbBones;
	size_t nbTransform;
	size_t nbTransformComponent;
	size_t nbElements = ema->commands.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		command = &(ema->commands.at(i));
		emo_Bone = command->bone;

		cmdBone = 0;
		nbBones = listByBone.size();
		for (size_t j = 0; j < nbBones; j++)
		{
			if (listByBone.at(j).emo_Bone == emo_Bone)
			{
				cmdBone = &(listByBone.at(j));
				break;
			}
		}
		if (!cmdBone)
		{
			listByBone.push_back(CmdBone());
			cmdBone = &listByBone.back();
			cmdBone->emo_Bone = emo_Bone;

			cmdBone->list.push_back(CmdBoneTransform());
			cmdBoneTransform = &cmdBone->list.back();
			cmdBoneTransform->transformId = command->transform;

			cmdBoneTransform->list.push_back(CmdBoneTransformComponent());
			cmdBoneTransformComponent = &cmdBoneTransform->list.back();
			cmdBoneTransformComponent->transformComponentId = command->transformComponent & 0x7;		//0x7 is limiting about 3 component (auras have a 0x9 for 0x1 + a flag at 0x8)
			cmdBoneTransformComponent->command = command;

			continue;
		}


		cmdBoneTransform = 0;
		nbTransform = cmdBone->list.size();
		for (size_t j = 0; j < nbTransform; j++)
		{
			if (cmdBone->list.at(j).transformId == command->transform)
			{
				cmdBoneTransform = &(cmdBone->list.at(j));
				break;
			}
		}
		if (!cmdBoneTransform)
		{
			cmdBone->list.push_back(CmdBoneTransform());
			cmdBoneTransform = &cmdBone->list.back();
			cmdBoneTransform->transformId = command->transform;

			cmdBoneTransform->list.push_back(CmdBoneTransformComponent());
			cmdBoneTransformComponent = &cmdBoneTransform->list.back();
			cmdBoneTransformComponent->transformComponentId = command->transformComponent;
			cmdBoneTransformComponent->command = command;
			continue;
		}


		cmdBoneTransformComponent = 0;
		nbTransformComponent = cmdBoneTransform->list.size();
		for (size_t j = 0; j < nbTransformComponent; j++)
		{
			if (cmdBoneTransform->list.at(j).transformComponentId == command->transformComponent)
			{
				cmdBoneTransformComponent = &(cmdBoneTransform->list.at(j));
				break;
			}
		}
		if (!cmdBoneTransformComponent)
		{
			cmdBoneTransform->list.push_back(CmdBoneTransformComponent());
			cmdBoneTransformComponent = &cmdBoneTransform->list.back();
			cmdBoneTransformComponent->transformComponentId = command->transformComponent;
			cmdBoneTransformComponent->command = command;
			continue;
		}

		//not a good case because there is a duplicate component. TODO add a warning.
		int aa = 42;
		//assert(false);
	}



	//now we could make ema_Material.
	EMMParameter* emmParam;
	nbBones = listByBone.size();
	for (size_t i = 0; i < nbBones; i++)
	{
		boneIndex = -1;
		cmdBone = &listByBone.at(i);
		emo_Bone = cmdBone->emo_Bone;
		if (!emo_Bone)
			continue;

		//so here , we have to find the bone index to link with material (ordered as in emm)
		string emaBoneName = emo_Bone->GetName();
		nbBones_tmp = bones->size();
		size_t isfound = (size_t)-1;
		for (size_t j = 0; j < nbBones_tmp; j++)
		{
			if (bones->at(j).GetName() == emaBoneName)
			{
				isfound = j;
				break;
			}
		}
		if (isfound == (size_t)-1)
		{
			printf("warning: bone name %s not found. skipped\n", emaBoneName.c_str());
			LibXenoverse::notifyWarning();
			continue;
		}

		if (isfound >= materials.size())
		{
			printf("warning: bone name %s is up to the materiallist. did you have the right .emm witch is for this mat.ema ? skipped\n", emaBoneName.c_str());
			LibXenoverse::notifyWarning();
			continue;
		}

		EMA_Material_Material &mat = materials.at(isfound);


		EMA_Material_MaterialParameter::EMA_MaterialParameter_Type emaParamType = EMA_Material_MaterialParameter::EMPT_Float;

		nbTransform = cmdBone->list.size();
		for (size_t j = 0; j < nbTransform; j++)
		{
			cmdBoneTransform = &cmdBone->list.at(j);

			float defaultX = 0.0;
			float defaultY = 0.0;
			float defaultZ = 0.0;
			float defaultW = 0.0;

			string parameter = "";
			switch (cmdBoneTransform->transformId)
			{
			case 0:
				{
					parameter = "MatCol0";
					emaParamType = EMA_Material_MaterialParameter::EMPT_Vector4;
			
					emmParam = emmMaterials.at(isfound)->getParameter(parameter + "R");
					defaultX = (emmParam) ? emmParam->float_value : 0.0f;

					emmParam = emmMaterials.at(isfound)->getParameter(parameter + "G");
					defaultY = (emmParam) ? emmParam->float_value : 0.0f;

					emmParam = emmMaterials.at(isfound)->getParameter(parameter + "B");
					defaultZ = (emmParam) ? emmParam->float_value : 0.0f;

					emmParam = emmMaterials.at(isfound)->getParameter(parameter + "A");
					defaultW = (emmParam) ? emmParam->float_value : 0.0f;
				}
				break;

			case 1:
				{
					parameter = "MatCol1";
					emaParamType = EMA_Material_MaterialParameter::EMPT_Vector4;
					emmParam = emmMaterials.at(isfound)->getParameter(parameter + "R");
					defaultX = (emmParam) ? emmParam->float_value : 0.0f;
					emmParam = emmMaterials.at(isfound)->getParameter(parameter + "G");
					defaultY = (emmParam) ? emmParam->float_value : 0.0f;
					emmParam = emmMaterials.at(isfound)->getParameter(parameter + "B");
					defaultZ = (emmParam) ? emmParam->float_value : 0.0f;
					emmParam = emmMaterials.at(isfound)->getParameter(parameter + "A");
					defaultW = (emmParam) ? emmParam->float_value : 0.0f;
				}
				break;

			case 4:
				{
					parameter = "TexScrl0";
					emaParamType = EMA_Material_MaterialParameter::EMPT_Vector2;
					emmParam = emmMaterials.at(isfound)->getParameter(parameter + "U");
					defaultX = (emmParam) ? emmParam->float_value : 0.0f;
					emmParam = emmMaterials.at(isfound)->getParameter(parameter + "V");
					defaultY = (emmParam) ? emmParam->float_value : 0.0f;
				}
				break;

			case 5:
				{
					parameter = "TexScrl1";
					emaParamType = EMA_Material_MaterialParameter::EMPT_Vector2;
					emmParam = emmMaterials.at(isfound)->getParameter(parameter + "U");
					defaultX = (emmParam) ? emmParam->float_value : 0.0f;
					emmParam = emmMaterials.at(isfound)->getParameter(parameter + "V");
					defaultY = (emmParam) ? emmParam->float_value : 0.0f;
				}
				break;


			default:
				{
					//todo add a warning.
					int aa = 42;
					//assert(false);
				}
			}

			



			mat.materialParamters.push_back(EMA_Material_MaterialParameter(parameter, emaParamType));
			EMA_Material_MaterialParameter &matParam = mat.materialParamters.back();


			//here, the trouble is about one animation by channel, so you could not have the same keyframe on all channel.
			//so we have to merge list of frameIndex on each components.
			std::vector<size_t> listIndexMerged;
			nbTransformComponent = cmdBoneTransform->list.size();
			for (size_t k = 0; k < nbTransformComponent; k++)
			{
				command = cmdBoneTransform->list.at(k).command;

				size_t nbFrames = command->steps.size();
				for (size_t m = 0; m < nbFrames; m++)
				{
					size_t time = command->steps.at(m).time;

					size_t nbIndex = listIndexMerged.size();
					bool isfound = false;
					for (size_t n = 0; n < nbIndex; n++)
					{
						if (listIndexMerged.at(n) == time)
						{
							isfound = true;
							break;
						}
					}
					if (!isfound)
						listIndexMerged.push_back(time);
				}
			}

			struct TestToOrder { bool operator() (const size_t & a, const size_t & b) const { return ((a == b) || (a < b)); } };
			std::sort(listIndexMerged.begin(), listIndexMerged.end(), TestToOrder());



			size_t nbIndex = listIndexMerged.size();
			for (size_t n = 0; n < nbIndex; n++)
			{
				size_t time = listIndexMerged.at(n);
				
				matParam.keyframes.push_back(EMA_Material_KeyFrame(time, defaultX, defaultY, defaultZ, defaultW));
				EMA_Material_KeyFrame &kf = matParam.keyframes.back();

				for (size_t k = 0; k < nbTransformComponent; k++)
				{
					cmdBoneTransformComponent = &cmdBoneTransform->list.at(k);
					EmaCommand* cmd = cmdBoneTransformComponent->command;

					uint8_t transformComponent_tmp = cmd->transformComponent & 0x7;

					size_t nbElements = cmd->steps.size();
					if (nbElements == 0)
						continue;

					size_t index_p = (size_t)-1;					//prev
					size_t index_n = (size_t)-1;					//next
					float factor = 0.0;								//between prev and next.
					if (time <= cmd->steps.at(0).time)
					{
						index_p = 0;
						index_n = 0;
					}
					if (time >= cmd->steps.back().time)
					{
						index_p = cmd->steps.size() - 1;
						index_n = index_p;
					}

					if (index_p == (size_t)-1)
					{
						for (size_t m = 1; m < nbElements; m++)						//Command is about a couple Bone+Transform, but EanAnimationNode is only bone, and under, EanKeyframeAnimation is about transform (position, rotation, scale).
						{
							if (time == cmd->steps.at(m).time)
							{
								index_p = m;
								index_n = m;
								factor = 0.0;
								break;

							}
							else if (time < cmd->steps.at(m).time) {
								index_p = m - 1;
								index_n = m;
								factor = ((float)time - (float)cmd->steps.at(m).time) / ((float)cmd->steps.at(m + 1).time - (float)cmd->steps.at(m).time);
								break;
							}
						}
					}


					float result = 0.0;

					size_t indexValue_p = cmd->steps.at(index_p).index;
					size_t indexValue_n = cmd->steps.at(index_n).index;


					size_t nbValues = ema->values.size();
					uint32_t mask = (!cmd->indexesByteSize) ? 0x3FFF : 0x3FFFFFFF;			//we remove the 2 last bits, seam the solution to remove upper index
																							//todo try to find what is about the last 2 bits.

					if (indexValue_p >= nbValues)
					{
						indexValue_p = indexValue_p & mask;

						if (indexValue_p >= nbValues)
							assert(false);
					}
					if (indexValue_n >= nbValues)
					{
						indexValue_n = indexValue_n & mask;

						if (indexValue_n >= nbValues)
							assert(false);
					}



					if (indexValue_p == indexValue_n)
					{
						if (indexValue_p >= nbValues)								//saddly , so index are not into array range.
							continue;

						result = ema->values.at(indexValue_p);

					} else {

						if (indexValue_n >= nbValues)
						{
							indexValue_n = indexValue_p;
							factor = 0.0;
						}

						if (indexValue_p >= nbValues)
							continue;

						float value_p = ema->values.at(indexValue_p);
						float value_n = ema->values.at(indexValue_n);

						result = value_p + factor * (value_n - value_p);
					}


					//now, it still to have find the component for 
					if (transformComponent_tmp == 0)
						kf.x = result;
					else if (transformComponent_tmp == 1)
						kf.y = result;
					else if (transformComponent_tmp == 2)
						kf.z = result;
					else if (transformComponent_tmp == 3)
						kf.w = result;
					//else
					//assert(false);
				}
			}
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             writeEma				                             |
\-------------------------------------------------------------------------------*/
void EMA_Material_Animation::writeEma(EmaAnimation* ema, EMO_Skeleton* emoSkeleton, EMM* emm)
{
	ema->name = name;
	ema->duration = frame_count;
	ema->type = type;
	ema->frame_float_size = frame_float_size;


	EMO_Bone* emo_Bone;
	size_t nbElements = materials.size();
	string currentTransform = "";
	size_t currentTransformId = 0;

	for (size_t m = 0; m < 6; m++)								//we have 0, 1, 4, 5
	{
		switch (m)
		{
		case 0: currentTransform = "MatCol0"; currentTransformId = 0; break;
		case 1: currentTransform = "MatCol1"; currentTransformId = 1; break;
		case 4: currentTransform = "TexScrl0"; currentTransformId = 4; break;
		case 5: currentTransform = "TexScrl1"; currentTransformId = 5; break;

		default:  currentTransform = ""; break;
		}

		if (currentTransform.length() == 0)
			continue;


		for (size_t i = 0; i < nbElements; i++)						//Command is about a couple Bone+Transform+transformComponent, but Ema_Material is only Material, and under,  is about parameters( MatCol0, MatCol1, TextureScroll0, TextureScroll1, etc ...).
		{
			emo_Bone = emoSkeleton->GetBone(materials.at(i).name);

			size_t nbKeyFramAnim = materials.at(i).materialParamters.size();
			for (size_t j = 0; j < nbKeyFramAnim; j++)
			{
				EMA_Material_MaterialParameter &matParam = materials.at(i).materialParamters.at(j);
				
				if (currentTransform != matParam.name)			//it will do the transform's order.
					continue;


				size_t nbComponent = 1;
				switch (matParam.emaParamType)
				{
				case EMA_Material_MaterialParameter::EMPT_Float:	nbComponent = 1; break;
				case EMA_Material_MaterialParameter::EMPT_Vector2:	nbComponent = 2; break;
				case EMA_Material_MaterialParameter::EMPT_Vector3:	nbComponent = 3; break;
				case EMA_Material_MaterialParameter::EMPT_Vector4:	nbComponent = 4; break;
				}

				for (size_t k = 0; k < nbComponent; k++)					//so the part for each component of the transform component.
				{
					ema->commands.push_back(EmaCommand());
					EmaCommand &cmd = ema->commands.back();
					cmd.bone = emo_Bone;
					cmd.transformComponent = k;			//0 for X or R, 1 one Y or G, 2 for Z or B, 3 for W or A.
					cmd.transform = currentTransformId;

					cmd.timesByteSize = ((frame_count > 0xff) ? 0x20 : 0x0);


					size_t nbElements = matParam.keyframes.size();
					for (size_t n = 0; n < nbElements; n++)						//Command is about a couple Bone+Transform, but EanAnimationNode is only bone, and under, EanKeyframeAnimation is about transform (position, rotation, scale).
					{
						cmd.steps.push_back(EmaStep());

						EMA_Material_KeyFrame &kf = matParam.keyframes.at(n);

						cmd.steps.back().time = kf.frameIndex;

						float value = 0.0;
						

						if (k == 0)
							value = kf.x;
						else if (k == 1)
							value = kf.y;
						else if (k == 2)
							value = kf.z;
						else if (k == 3)
							value = kf.w;
						


						//to reduce the number of Values (goal of this format)
						size_t isfound = (size_t)-1;
						size_t nbValues = ema->values.size();
						for (size_t p = 0; p < nbValues; p++)
						{
							if (ema->values.at(p) == value)
							{
								isfound = p;
								break;
							}
						}
						if (isfound == (size_t)-1)
						{
							isfound = nbValues;
							ema->values.push_back(value);
						}

						cmd.steps.back().index = isfound;
					}


					cmd.indexesByteSize = ((ema->values.size() > 0xffff) ? 0x40 : 0x0);
				}
			}
		}
	}
}


























/*-------------------------------------------------------------------------------\
|                             loadXml											 |
\-------------------------------------------------------------------------------*/
bool EMA_Material::loadXml(string filename)
{
	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
	{
		printf("Loading xml %s fail. skip.'\n", filename);
		getchar();
		return false;
	}

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);
	TiXmlElement* rootNode = hDoc.FirstChildElement("EMA_Material").Element();
	if (!rootNode)
	{
		printf("%s don't have 'EMA_Material' tags. skip.'\n", filename);
		getchar();
		return false;
	}

	return importXml(rootNode);
}
/*-------------------------------------------------------------------------------\
|                             saveXml											 |
\-------------------------------------------------------------------------------*/
void EMA_Material::saveXml(string filename)
{
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement* rootNode = exportXml();
	doc.LinkEndChild(rootNode);

	doc.SaveFile(filename);
}








/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EMA_Material::importXml(TiXmlElement* xmlCurrentNode)
{
	xmlCurrentNode->QueryStringAttribute("name", &name);

	size_t unknown_total_tmp = 0;
	xmlCurrentNode->QueryUnsignedAttribute("unk_08", &unknown_total_tmp);
	unk_08 = (unsigned int)unknown_total_tmp;
	xmlCurrentNode->QueryUnsignedAttribute("unk_12", &unknown_total_tmp);
	unk_12 = (unsigned int)unknown_total_tmp;

	TiXmlElement* animsNode = xmlCurrentNode->FirstChildElement("Animations");
	if (!animsNode)
	{
		printf("No 'Animations' tags find. skip.'\n");
		getchar();
		return false;
	}

	for (TiXmlElement* xmlNode = animsNode->FirstChildElement("Animation"); xmlNode; xmlNode = xmlNode->NextSiblingElement("Animation"))
	{
		EMA_Material_Animation animation;
		if (animation.importXml(xmlNode))
			animations.push_back(animation);
	}

	return (animations.size() != 0);
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EMA_Material_Animation::importXml(TiXmlElement* xmlCurrentNode)
{
	xmlCurrentNode->QueryStringAttribute("name", &name);

	size_t tmp;
	xmlCurrentNode->QueryUnsignedAttribute("frameCount", &frame_count);

	if (xmlCurrentNode->QueryUnsignedAttribute("type", &tmp) == TIXML_SUCCESS)
		type = tmp;
	if (xmlCurrentNode->QueryUnsignedAttribute("frame_float_size", &tmp) == TIXML_SUCCESS)
		frame_float_size = tmp;

	TiXmlElement* materialsNode = xmlCurrentNode->FirstChildElement("Materials");
	if (!materialsNode)
	{
		printf("No 'Materials' tags find. skip.'\n");
		getchar();
		return false;
	}

	for (TiXmlElement* xmlNode = materialsNode->FirstChildElement("Material"); xmlNode; xmlNode = xmlNode->NextSiblingElement("Material"))
	{
		EMA_Material_Material material;
		if (material.importXml(xmlNode))
			materials.push_back(material);
	}

	return (materials.size() != 0);
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EMA_Material_Material::importXml(TiXmlElement* xmlCurrentNode)
{
	xmlCurrentNode->QueryStringAttribute("name", &name);

	
	TiXmlElement* parametersNode = xmlCurrentNode->FirstChildElement("Parameters");
	if (!parametersNode)
	{
		printf("No 'Parameters' tags find. skip.'\n");
		getchar();
		return false;
	}

	for (TiXmlElement* xmlNode = parametersNode->FirstChildElement("Parameter"); xmlNode; xmlNode = xmlNode->NextSiblingElement("Parameter"))
	{
		EMA_Material_MaterialParameter parameter;
		if (parameter.importXml(xmlNode))
			materialParamters.push_back(parameter);
	}

	return (materialParamters.size() != 0);
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EMA_Material_MaterialParameter::importXml(TiXmlElement* xmlCurrentNode)
{
	xmlCurrentNode->QueryStringAttribute("name", &name);
	
	string typeName = "";
	xmlCurrentNode->QueryStringAttribute("type", &typeName);

	if (typeName == "Float")
		emaParamType = EMPT_Float;
	else if (typeName == "Vector2")
		emaParamType = EMPT_Vector2;
	else if (typeName == "Vector3")
		emaParamType = EMPT_Vector3;
	else if (typeName == "Vector4")
		emaParamType = EMPT_Vector4;
	else
		return false;


	TiXmlElement* KFNode = xmlCurrentNode->FirstChildElement("Keyframes");
	if (!KFNode)
	{
		printf("No 'Keyframes' tags find. skip.'\n");
		getchar();
		return false;
	}

	for (TiXmlElement* xmlNode = KFNode->FirstChildElement("Keyframe"); xmlNode; xmlNode = xmlNode->NextSiblingElement("Keyframe"))
	{
		EMA_Material_KeyFrame Keyframe;
		if (Keyframe.importXml(xmlNode))
			keyframes.push_back(Keyframe);
	}

	return (keyframes.size() != 0);
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EMA_Material_KeyFrame::importXml(TiXmlElement* xmlCurrentNode)
{
	xmlCurrentNode->QueryUnsignedAttribute("frameIndex", &frameIndex);
	double tmp;
	if (xmlCurrentNode->QueryDoubleAttribute("x", &tmp) == TIXML_SUCCESS)
		x = (float)tmp;
	if (xmlCurrentNode->QueryDoubleAttribute("y", &tmp) == TIXML_SUCCESS)
		y = (float)tmp;
	if (xmlCurrentNode->QueryDoubleAttribute("z", &tmp) == TIXML_SUCCESS)
		z = (float)tmp;
	if (xmlCurrentNode->QueryDoubleAttribute("w", &tmp) == TIXML_SUCCESS)
		w = (float)tmp;

	return true;
}






/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMA_Material::exportXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("EMA_Material");

	xmlCurrentNode->SetAttribute("name", name);
	xmlCurrentNode->SetAttribute("unk_08", (size_t)unk_08);
	xmlCurrentNode->SetAttribute("unk_12", (size_t)unk_12);


	TiXmlElement* animsNode = new TiXmlElement("Animations");
	size_t nbAnim = animations.size();
	for (size_t i = 0; i < nbAnim; i++)
		animsNode->LinkEndChild(animations.at(i).exportXml());

	xmlCurrentNode->LinkEndChild(animsNode);


	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMA_Material_Animation::exportXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("Animation");

	xmlCurrentNode->SetAttribute("name", name);
	xmlCurrentNode->SetAttribute("frameCount", frame_count);

	xmlCurrentNode->SetAttribute("type", type);
	xmlCurrentNode->SetAttribute("frame_float_size", frame_float_size);


	TiXmlElement* materialsNode = new TiXmlElement("Materials");
	size_t nbNode = materials.size();
	for (size_t i = 0; i < nbNode; i++)
		materialsNode->LinkEndChild(materials.at(i).exportXml());

	xmlCurrentNode->LinkEndChild(materialsNode);


	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMA_Material_Material::exportXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("Material");

	xmlCurrentNode->SetAttribute("name", name);


	TiXmlElement* parametersNode = new TiXmlElement("Parameters");

	size_t nbKeyframeAnim = materialParamters.size();
	for (size_t i = 0; i < nbKeyframeAnim; i++)
		parametersNode->LinkEndChild(materialParamters.at(i).exportXml());

	xmlCurrentNode->LinkEndChild(parametersNode);


	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMA_Material_MaterialParameter::exportXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("Parameter");

	switch (emaParamType)
	{
	case EMPT_Float:	xmlCurrentNode->SetAttribute("type", "Float"); break;
	case EMPT_Vector2:	xmlCurrentNode->SetAttribute("type", "Vector2"); break;
	case EMPT_Vector3:	xmlCurrentNode->SetAttribute("type", "Vector3"); break;
	case EMPT_Vector4:	xmlCurrentNode->SetAttribute("type", "Vector4"); break;
	default: xmlCurrentNode->SetAttribute("type", ""); break;
	}
	xmlCurrentNode->SetAttribute("name", name);


	TiXmlElement* KFNode = new TiXmlElement("Keyframes");

	size_t nbKeyframe = keyframes.size();
	for (size_t i = 0; i < nbKeyframe; i++)
		KFNode->LinkEndChild(keyframes.at(i).exportXml((size_t)emaParamType));

	xmlCurrentNode->LinkEndChild(KFNode);

	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMA_Material_KeyFrame::exportXml(size_t emaParamType)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("Keyframe");

	xmlCurrentNode->SetAttribute("frameIndex", frameIndex);
	xmlCurrentNode->SetDoubleAttribute("x", x);
	if(emaParamType == 0)
		return xmlCurrentNode;
	
	xmlCurrentNode->SetDoubleAttribute("y", y);
	if (emaParamType == 1)
		return xmlCurrentNode;

	xmlCurrentNode->SetDoubleAttribute("z", z);
	if (emaParamType == 2)
		return xmlCurrentNode;

	xmlCurrentNode->SetDoubleAttribute("w", w);
	return xmlCurrentNode;
}






/*-------------------------------------------------------------------------------\
|                             getInterpolatedKeyframe							 |
\-------------------------------------------------------------------------------*/
EMA_Material_KeyFrame EMA_Material_MaterialParameter::getInterpolatedKeyframe(double frame_f)
{
	EMA_Material_KeyFrame interpolKf((size_t)floor(frame_f));
	if (keyframes.size() == 0)
		return interpolKf;

	if (frame_f <= (double)keyframes.at(0).frameIndex)
	{
		interpolKf.x = keyframes.at(0).x;
		interpolKf.y = keyframes.at(0).y;
		interpolKf.z = keyframes.at(0).z;
		interpolKf.w = keyframes.at(0).w;
		return interpolKf;
	}
	if (frame_f >= (double)keyframes.back().frameIndex)
	{
		interpolKf.x = keyframes.back().x;
		interpolKf.y = keyframes.back().y;
		interpolKf.z = keyframes.back().z;
		interpolKf.w = keyframes.back().w;
		return interpolKf;
	}

	for (size_t i = 1, nb = keyframes.size(); i < nb; i++)
	{
		if ((double)(keyframes.at(i).frameIndex) == frame_f)
		{
			interpolKf.x = keyframes.at(i).x;
			interpolKf.y = keyframes.at(i).y;
			interpolKf.z = keyframes.at(i).z;
			interpolKf.w = keyframes.at(i).w;
			return interpolKf;
		}

		if ((double)(keyframes.at(i).frameIndex) > frame_f)
		{
			double factor = (frame_f - (double)(keyframes.at(i - 1).frameIndex)) / ((double)(keyframes.at(i).frameIndex) - (double)(keyframes.at(i - 1).frameIndex));
			interpolKf.x = (float)((keyframes.at(i).x - keyframes.at(i - 1).x) * factor + keyframes.at(i - 1).x);
			interpolKf.y = (float)((keyframes.at(i).y - keyframes.at(i - 1).y) * factor + keyframes.at(i - 1).y);
			interpolKf.z = (float)((keyframes.at(i).z - keyframes.at(i - 1).z) * factor + keyframes.at(i - 1).z);
			interpolKf.w = (float)((keyframes.at(i).w - keyframes.at(i - 1).w) * factor + keyframes.at(i - 1).w);

			return interpolKf;
		}
	}

	return interpolKf;
}


}