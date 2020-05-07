namespace LibXenoverse
{






/*-------------------------------------------------------------------------------\
|                             loadXml											 |
\-------------------------------------------------------------------------------*/
bool ESK::loadXml(string filename)
{
	TiXmlDocument doc(filename);
	if(!doc.LoadFile())
	{
		printf("Loading xml %s fail. skip.'\n", filename.c_str());
		notifyError();
		return false;
	}

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);
	TiXmlElement* rootNode = hDoc.FirstChildElement("ESK").Element();
	if (!rootNode)
	{
		printf("%s don't have 'ESK' tags. skip.'\n", filename.c_str());
		notifyError();
		return false;
	}

	return importXml(rootNode);
}
/*-------------------------------------------------------------------------------\
|                             saveXml											 |
\-------------------------------------------------------------------------------*/
void ESK::saveXml(string filename)
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
bool ESK::importXml(TiXmlElement* xmlCurrentNode)
{
	xmlCurrentNode->QueryStringAttribute("name", &name);
	xmlCurrentNode->QueryStringAttribute("version", &version);

	string str = "";
	xmlCurrentNode->QueryStringAttribute("flag", &str); flag = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("skeletonUniqueId", &str); skeletonUniqueId = EMO_BaseFile::GetUnsigned64(str);

	xmlCurrentNode->QueryStringAttribute("unk_0", &str); unknow_0 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unk_1", &str); unknow_1 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unk_2", &str); unknow_2 = EMO_BaseFile::GetUnsigned(str);
	


	TiXmlElement* bonesNode = xmlCurrentNode->FirstChildElement("Bones");
	if (!bonesNode)
	{
		printf("No 'Bones' tags find. skip.'\n");
		notifyError();
		return false;
	}

	//node will be organize into hierarchy, to be more readable.
	EskTreeNode* treeRootNode = new EskTreeNode(nullptr, (size_t)-1, nullptr);

	
	for (TiXmlElement* xmlNode = bonesNode->FirstChildElement("Bone"); xmlNode; xmlNode = xmlNode->NextSiblingElement("Bone"))
	{
		ESKBone* eskBone = new ESKBone();
		EskTreeNode* treeNode = eskBone->importXml(xmlNode, bones, treeRootNode, mHaveExtraBytesOnEachBone);
		if (treeNode)
		{
			treeRootNode->mChildren.push_back(treeNode);
		}else {
			delete eskBone;
		}
	}
	
	setTreeOrganisation(treeRootNode);					//read hieirarchy to make the list of bones.
	
	delete treeRootNode;					//delete the hierarchy organisation, but not the bones


	//read IK and Extra
	TiXmlElement* node_IK_list = xmlCurrentNode->FirstChildElement("InverseKinematic");
	if (node_IK_list)
	{
		string str = "";
		uint32_t tmp = 0;

		for (TiXmlElement* node_group = node_IK_list->FirstChildElement("Group"); node_group; node_group = node_group->NextSiblingElement("Group"))
		{
			listInverseKinematic.push_back(Esk_IK_Group());

			for (TiXmlElement* node_ik = node_group->FirstChildElement("IK"); node_ik; node_ik = node_ik->NextSiblingElement("IK"))
			{
				listInverseKinematic.back().mListIK.push_back(Esk_IK_Relation());
				Esk_IK_Relation &ik = listInverseKinematic.back().mListIK.back();				
				node_ik->QueryUnsignedAttribute("unk_0", &tmp); ik.unknow_0 = (uint8_t)tmp;
				
				TiXmlElement* node_influenced = node_ik->FirstChildElement("BoneInfluenced");
				if (!node_influenced)
				{
					printf("No 'BoneInfluenced' tags find. skip.'\n");
					notifyError();
					listInverseKinematic.pop_back();
					continue;
				}
				
				string name = "";
				double value = 0.0;
				ESKBone* eskBone = 0;
				size_t nbBones = bones.size();

				node_influenced->QueryStringAttribute("name", &name);
				node_influenced->QueryStringAttribute("value", &str);
				value = StringToFloat(str);

				for (size_t i = 0; i < nbBones; i++)
				{
					if (bones.at(i)->getName() == name)
					{
						eskBone = bones.at(i);
						break;
					}
				}

				if (eskBone)
				{
					ik.mListBones.push_back(Esk_IK_Relation::IKR_Bone(eskBone, (float)value));
				}else {
					printf("'BoneInfluenced' not found in bones declaration. skip.'\n");
					notifyError();
					listInverseKinematic.pop_back();
					continue;
				}


				for (TiXmlElement* node = node_ik->FirstChildElement("Bone"); node; node = node->NextSiblingElement("Bone"))
				{
					name = "";
					value = 0.0;
					node->QueryStringAttribute("name", &name);
					node->QueryStringAttribute("value", &str); 
					value = StringToFloat(str);

					for (size_t i = 0; i < nbBones; i++)
					{
						if (bones.at(i)->getName()== name)
						{
							ik.mListBones.push_back(Esk_IK_Relation::IKR_Bone(bones.at(i), (float)value));
							break;
						}
					}						
				}
			}
		}
	}


	return (bones.size() != 0);
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
EskTreeNode* ESKBone::importXml(TiXmlElement* xmlCurrentNode, std::vector<ESKBone*> &bones, EskTreeNode* treeParentNode, bool haveExtraBytesOnEachBone)
{
	EskTreeNode* treeCurrentNode = new EskTreeNode(this, bones.size(), treeParentNode);
	bones.push_back(this);
	
	xmlCurrentNode->QueryStringAttribute("name", &name);
	
	size_t tmp = 0;
	xmlCurrentNode->QueryUnsignedAttribute("ik_flag", &tmp); ik_flag = (uint16_t)tmp;				//Test todo remove.
	

	string str = "";
	if (haveExtraBytesOnEachBone)
	{
		xmlCurrentNode->QueryUnsignedAttribute("exUnk_0", &unk_extraInfo_0);
		xmlCurrentNode->QueryUnsignedAttribute("exUnk_1", &unk_extraInfo_1);
		xmlCurrentNode->QueryStringAttribute("exUnk_2", &str); unk_extraInfo_2 = EMO_BaseFile::GetUnsigned(str);
		xmlCurrentNode->QueryUnsignedAttribute("exUnk_3", &unk_extraInfo_3);
	}

	str = "";
	if(bones.size()==1)
		xmlCurrentNode->QueryStringAttribute("childIndex_is0xFFFF", &str); childIndex_is0xFFFF = (str=="true");

	str = "";
	xmlCurrentNode->QueryStringAttribute("hierarchy", &str);
	if (str.length() == 0)
	{
		if((treeParentNode)&&(treeParentNode->mBone) && (treeParentNode->mBone->hierarchyIndex == (LIBXENOVERSE_ESK_BONE_HIERARCHY_FIRST | LIBXENOVERSE_ESK_BONE_HIERARCHY_SECOND) ))
			hierarchyIndex = LIBXENOVERSE_ESK_BONE_HIERARCHY_FIRST;
	}else{
		hierarchyIndex = 0;
		std::vector<string> sv = split(str, ',');
		for (size_t i = 0; i < sv.size(); i++)
		{
			if (sv.at(i) == "0")
				hierarchyIndex |= LIBXENOVERSE_ESK_BONE_HIERARCHY_FIRST;
			else if (sv.at(i) == "1")
				hierarchyIndex |= LIBXENOVERSE_ESK_BONE_HIERARCHY_SECOND;
		}
	}
	


	TiXmlElement* absoluteTransformMatrixNode = xmlCurrentNode->FirstChildElement("AbsoluteTransformMatrix");
	haveAbsoluteMatrix = (absoluteTransformMatrixNode != 0);
	if (haveAbsoluteMatrix)
	{
		TiXmlElement* lineNode0 = absoluteTransformMatrixNode->FirstChildElement("Line");
		TiXmlElement* lineNode1 = (lineNode0) ? lineNode0->NextSiblingElement("Line") : 0;
		TiXmlElement* lineNode2 = (lineNode1) ? lineNode1->NextSiblingElement("Line") : 0;
		TiXmlElement* lineNode3 = (lineNode2) ? lineNode2->NextSiblingElement("Line") : 0;

		if (!lineNode3)
		{
			printf("AbsoluteTransformMatrix incomplete number of 'Line'. skip.'\n");
			notifyError();
		}else{

			string str = "";
			lineNode0->QueryStringAttribute("x", &str); absoluteMatrix[0] = StringToFloat(str);
			lineNode0->QueryStringAttribute("y", &str); absoluteMatrix[1] = StringToFloat(str);
			lineNode0->QueryStringAttribute("z", &str); absoluteMatrix[2] = StringToFloat(str);
			lineNode0->QueryStringAttribute("w", &str); absoluteMatrix[3] = StringToFloat(str);

			lineNode1->QueryStringAttribute("x", &str); absoluteMatrix[4] = StringToFloat(str);
			lineNode1->QueryStringAttribute("y", &str); absoluteMatrix[5] = StringToFloat(str);
			lineNode1->QueryStringAttribute("z", &str); absoluteMatrix[6] = StringToFloat(str);
			lineNode1->QueryStringAttribute("w", &str); absoluteMatrix[7] = StringToFloat(str);

			lineNode2->QueryStringAttribute("x", &str); absoluteMatrix[8] = StringToFloat(str);
			lineNode2->QueryStringAttribute("y", &str); absoluteMatrix[9] = StringToFloat(str);
			lineNode2->QueryStringAttribute("z", &str); absoluteMatrix[10] = StringToFloat(str);
			lineNode2->QueryStringAttribute("w", &str); absoluteMatrix[11] = StringToFloat(str);

			lineNode3->QueryStringAttribute("x", &str); absoluteMatrix[12] = StringToFloat(str);
			lineNode3->QueryStringAttribute("y", &str); absoluteMatrix[13] = StringToFloat(str);
			lineNode3->QueryStringAttribute("z", &str); absoluteMatrix[14] = StringToFloat(str);
			lineNode3->QueryStringAttribute("w", &str); absoluteMatrix[15] = StringToFloat(str);
		}
	}


	TiXmlElement* relativeTransformNode = xmlCurrentNode->FirstChildElement("RelativeTransform");
	if (relativeTransformNode)
	{
		TiXmlElement* positionNode = relativeTransformNode->FirstChildElement("Position");
		TiXmlElement* orientationNode = relativeTransformNode->FirstChildElement("Orientation");
		TiXmlElement* scaleNode = relativeTransformNode->FirstChildElement("Scale");
		string str = "";

		if (positionNode)
		{
			positionNode->QueryStringAttribute("x", &str); relativeTransform[0] = StringToFloat(str);
			positionNode->QueryStringAttribute("y", &str); relativeTransform[1] = StringToFloat(str);
			positionNode->QueryStringAttribute("z", &str); relativeTransform[2] = StringToFloat(str);
			positionNode->QueryStringAttribute("w", &str); relativeTransform[3] = StringToFloat(str);
		}
		if (orientationNode)
		{
			orientationNode->QueryStringAttribute("x", &str); relativeTransform[4] = StringToFloat(str);
			orientationNode->QueryStringAttribute("y", &str); relativeTransform[5] = StringToFloat(str);
			orientationNode->QueryStringAttribute("z", &str); relativeTransform[6] = StringToFloat(str);
			orientationNode->QueryStringAttribute("w", &str); relativeTransform[7] = StringToFloat(str);
		}
		if (scaleNode)
		{
			scaleNode->QueryStringAttribute("x", &str); relativeTransform[8] = StringToFloat(str);
			scaleNode->QueryStringAttribute("y", &str); relativeTransform[9] = StringToFloat(str);
			scaleNode->QueryStringAttribute("z", &str); relativeTransform[10] = StringToFloat(str);
			scaleNode->QueryStringAttribute("w", &str); relativeTransform[11] = StringToFloat(str);
		}
	}
	
	


	// Same for children
	for (TiXmlElement* xmlNode = xmlCurrentNode->FirstChildElement("Bone"); xmlNode; xmlNode = xmlNode->NextSiblingElement("Bone"))
	{
		ESKBone* eskBone = new ESKBone();
		EskTreeNode* treeNode = eskBone->importXml(xmlNode, bones, treeCurrentNode, haveExtraBytesOnEachBone);
		if (treeNode)
			treeCurrentNode->mChildren.push_back(treeNode);
		else
			delete eskBone;
	}

	return treeCurrentNode;
}







/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* ESK::exportXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("ESK");

	xmlCurrentNode->SetAttribute("name", name);
	xmlCurrentNode->SetAttribute("version", version);
	xmlCurrentNode->SetAttribute("flag", EMO_BaseFile::UnsignedToString(flag, true));

	xmlCurrentNode->SetAttribute("flagA", ((flag & 0x8) >> 3) );				//test Todo remove.
	xmlCurrentNode->SetAttribute("flagB", ((flag & 0x4) >> 2));
	xmlCurrentNode->SetAttribute("flagC", ((flag & 0x2) >> 1));
	xmlCurrentNode->SetAttribute("flagD", (flag & 0x1));

	xmlCurrentNode->SetAttribute("skeletonUniqueId", EMO_BaseFile::Unsigned64ToString(skeletonUniqueId, true));

	xmlCurrentNode->SetAttribute("unk_0", EMO_BaseFile::UnsignedToString(unknow_0, true));
	xmlCurrentNode->SetAttribute("unk_1", EMO_BaseFile::UnsignedToString(unknow_1, true));
	xmlCurrentNode->SetAttribute("unk_2", EMO_BaseFile::UnsignedToString(unknow_2, true));




	{										//test todo remove
		string duplicatedBoneName = "";
		string boneName;
		for (size_t i = 0, nbBone = bones.size(); i < nbBone; i++)
		{
			string boneName = bones.at(i)->getName();
			for (size_t j = i + 1; j < nbBone; j++)
			{
				if (bones.at(j)->getName() == boneName)
				{
					duplicatedBoneName += boneName + ",";
					continue;
				}
			}
		}
		if(duplicatedBoneName.length())
			xmlCurrentNode->SetAttribute("duplicatedBoneName", duplicatedBoneName);
	}


	
	TiXmlElement* bonesNode = new TiXmlElement("Bones");
	xmlCurrentNode->LinkEndChild(bonesNode);

	EskTreeNode* rootTreeNode = getTreeOrganisation();		//get by hierarchy.

	EskTreeNode* childTreeNode;
	size_t nbchild = rootTreeNode->mChildren.size();
	for (size_t i = 0; i < nbchild; i++)
	{
		childTreeNode = rootTreeNode->mChildren.at(i);
		bonesNode->LinkEndChild(new TiXmlComment((string("Index: ") + ToString(childTreeNode->mIndex)).c_str()));
		bonesNode->LinkEndChild(childTreeNode->mBone->exportXml(childTreeNode, mHaveExtraBytesOnEachBone));
	}
	delete rootTreeNode;




	if (listInverseKinematic.size())
	{
		TiXmlElement* node_IK_list = new TiXmlElement("InverseKinematic");
		xmlCurrentNode->LinkEndChild(node_IK_list);
		
		size_t nbBones = listInverseKinematic.size();
		for (size_t i=0;i<nbBones;i++)
		{
			Esk_IK_Group &group = listInverseKinematic.at(i);
			
			TiXmlElement* node_group = new TiXmlElement("Group");
			node_IK_list->LinkEndChild(node_group);

			size_t nbIk = group.mListIK.size();
			for (size_t j = 0; j < nbIk; j++)
			{
				Esk_IK_Relation &ik = group.mListIK.at(j);
				TiXmlElement* node_ik = new TiXmlElement("IK");
				node_ik->SetAttribute("unk_0", ik.unknow_0);
				node_group->LinkEndChild(node_ik);

				size_t nbBones = ik.mListBones.size();
				for (size_t k = 0; k < nbBones; k++)
				{
					TiXmlElement* node = new TiXmlElement( (k==0) ? "BoneInfluenced" : "Bone");
					node_ik->LinkEndChild(node);

					node->SetAttribute("name", ik.mListBones.at(k).bone->getName());
					node->SetAttribute("value", FloatToString(ik.mListBones.at(k).value));
				}
			}
		}
	}


	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* ESKBone::exportXml(EskTreeNode* treeNode, bool haveExtraBytesOnEachBone)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("Bone");

	xmlCurrentNode->SetAttribute("name", name);
	xmlCurrentNode->SetAttribute("ik_flag", ik_flag);				//Test todo remove.

	if ((treeNode->mIndex == 0)&&(childIndex_is0xFFFF))
		xmlCurrentNode->SetAttribute("childIndex_is0xFFFF", "true");

	if ((hierarchyIndex & LIBXENOVERSE_ESK_BONE_HIERARCHY_SECOND) != 0)			//if hierarchyIndex==0 or there is only the first, don't need to display it.
		xmlCurrentNode->SetAttribute("hierarchy", (hierarchyIndex & LIBXENOVERSE_ESK_BONE_HIERARCHY_FIRST) ? "0,1" : "1");		//the root will have both because is on both in the same time.


	if (haveExtraBytesOnEachBone)
	{
		xmlCurrentNode->SetAttribute("exUnk_0", unk_extraInfo_0);
		xmlCurrentNode->SetAttribute("exUnk_1", unk_extraInfo_1);
		xmlCurrentNode->SetAttribute("exUnk_2", EMO_BaseFile::UnsignedToString(unk_extraInfo_2,true));
		xmlCurrentNode->SetAttribute("exUnk_3", unk_extraInfo_3);
	}

	


	if (haveAbsoluteMatrix)
	{
		TiXmlElement* absoluteTransformMatrixNode = new TiXmlElement("AbsoluteTransformMatrix");

		TiXmlElement* lineNode0 = new TiXmlElement("Line");
		TiXmlElement* lineNode1 = new TiXmlElement("Line");
		TiXmlElement* lineNode2 = new TiXmlElement("Line");
		TiXmlElement* lineNode3 = new TiXmlElement("Line");


		lineNode0->SetAttribute("x", FloatToString(absoluteMatrix[0]));
		lineNode0->SetAttribute("y", FloatToString(absoluteMatrix[1]));
		lineNode0->SetAttribute("z", FloatToString(absoluteMatrix[2]));
		lineNode0->SetAttribute("w", FloatToString(absoluteMatrix[3]));

		lineNode1->SetAttribute("x", FloatToString(absoluteMatrix[4]));
		lineNode1->SetAttribute("y", FloatToString(absoluteMatrix[5]));
		lineNode1->SetAttribute("z", FloatToString(absoluteMatrix[6]));
		lineNode1->SetAttribute("w", FloatToString(absoluteMatrix[7]));

		lineNode2->SetAttribute("x", FloatToString(absoluteMatrix[8]));
		lineNode2->SetAttribute("y", FloatToString(absoluteMatrix[9]));
		lineNode2->SetAttribute("z", FloatToString(absoluteMatrix[10]));
		lineNode2->SetAttribute("w", FloatToString(absoluteMatrix[11]));

		lineNode3->SetAttribute("x", FloatToString(absoluteMatrix[12]));
		lineNode3->SetAttribute("y", FloatToString(absoluteMatrix[13]));
		lineNode3->SetAttribute("z", FloatToString(absoluteMatrix[14]));
		lineNode3->SetAttribute("w", FloatToString(absoluteMatrix[15]));

		absoluteTransformMatrixNode->LinkEndChild(lineNode0);
		absoluteTransformMatrixNode->LinkEndChild(lineNode1);
		absoluteTransformMatrixNode->LinkEndChild(lineNode2);
		absoluteTransformMatrixNode->LinkEndChild(lineNode3);
		xmlCurrentNode->LinkEndChild(absoluteTransformMatrixNode);
	}


	{
		TiXmlElement* relativeTransformNode = new TiXmlElement("RelativeTransform");

		TiXmlElement* positionNode = new TiXmlElement("Position");
		TiXmlElement* orientationNode = new TiXmlElement("Orientation");
		TiXmlElement* scaleNode = new TiXmlElement("Scale");


		positionNode->SetAttribute("x", FloatToString(relativeTransform[0]));
		positionNode->SetAttribute("y", FloatToString(relativeTransform[1]));
		positionNode->SetAttribute("z", FloatToString(relativeTransform[2]));
		positionNode->SetAttribute("w", FloatToString(relativeTransform[3]));

		orientationNode->SetAttribute("x", FloatToString(relativeTransform[4]));
		orientationNode->SetAttribute("y", FloatToString(relativeTransform[5]));
		orientationNode->SetAttribute("z", FloatToString(relativeTransform[6]));
		orientationNode->SetAttribute("w", FloatToString(relativeTransform[7]));

		scaleNode->SetAttribute("x", FloatToString(relativeTransform[8]));
		scaleNode->SetAttribute("y", FloatToString(relativeTransform[9]));
		scaleNode->SetAttribute("z", FloatToString(relativeTransform[10]));
		scaleNode->SetAttribute("w", FloatToString(relativeTransform[11]));

		relativeTransformNode->LinkEndChild(positionNode);
		relativeTransformNode->LinkEndChild(orientationNode);
		relativeTransformNode->LinkEndChild(scaleNode);
		xmlCurrentNode->LinkEndChild(relativeTransformNode);
	}


	//same for child
	EskTreeNode* childTreeNode;
	size_t nbchild = treeNode->mChildren.size();
	for (size_t i = 0; i < nbchild; i++)
	{
		childTreeNode = treeNode->mChildren.at(i);
		xmlCurrentNode->LinkEndChild(new TiXmlComment((string("Index: ") + ToString(childTreeNode->mIndex)).c_str()));
		xmlCurrentNode->LinkEndChild(childTreeNode->mBone->exportXml(childTreeNode, haveExtraBytesOnEachBone));
	}

	return xmlCurrentNode;
}








}