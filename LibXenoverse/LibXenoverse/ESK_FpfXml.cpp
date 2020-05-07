namespace LibXenoverse
{






/*-------------------------------------------------------------------------------\
|                             loadXml											 |
\-------------------------------------------------------------------------------*
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
void ESK::saveFpfXml_Extract(string filename, ESK* esk_sourceBonesName)
{
	if (!esk_sourceBonesName)
		return;

	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement* rootNode = exporFpftXml(esk_sourceBonesName);
	doc.LinkEndChild(rootNode);

	doc.SaveFile(filename);
}








/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*
bool ESK::importXml(TiXmlElement* xmlCurrentNode)
{
	xmlCurrentNode->QueryStringAttribute("name", &name);

	string str = "";
	
	xmlCurrentNode->QueryStringAttribute("flag", &str); flag = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow_offset_2", &str); unknow_offset_2 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow_offset_3", &str); unknow_offset_3 = EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryBoolAttribute("haveExtraBytesOnEachBone", &mHaveExtraBytesOnEachBone);


	TiXmlElement* bonesNode = xmlCurrentNode->FirstChildElement("Bones");
	if (!bonesNode)
	{
		printf("No 'ESKBones' tags find. skip.'\n");
		notifyError();
		return false;
	}

	//node will be organize into hieirarchy, to be more readable.
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
	
	delete treeRootNode;					//delete the hieirarchy organisation, but not the bones


	//read IK and Extra
	TiXmlElement* node_IK_list = xmlCurrentNode->FirstChildElement("InverseKinematic");
	if (node_IK_list)
	{
		for (TiXmlElement* node_group = node_IK_list->FirstChildElement("Group"); node_group; node_group = node_group->NextSiblingElement("Group"))
		{
			listInverseKinematic.push_back(Esk_IK_Group());

			for (TiXmlElement* node_ik = node_group->FirstChildElement("IK"); node_ik; node_ik = node_ik->NextSiblingElement("IK"))
			{
				listInverseKinematic.back().mListIK.push_back(Esk_IK_Relation());
				Esk_IK_Relation &ik = listInverseKinematic.back().mListIK.back();

				for (TiXmlElement* node = node_ik->FirstChildElement("Bone"); node; node = node->NextSiblingElement("Bone"))
				{
					string name = "";
					double value = 0.0;
					node->QueryStringAttribute("name", &name);
					node->QueryDoubleAttribute("value", &value);

					ESKBone* eskBone = 0;
					size_t nbBones = bones.size();
					for (size_t i = 0; i < nbBones; i++)
					{
						if (bones.at(i)->getName()== name)
						{
							eskBone = bones.at(i);
							break;
						}
					}

					if(eskBone)
						ik.mListBones.push_back(Esk_IK_Relation::IKR_Bone(eskBone, (float)value));
				}
			}
		}
	}


	return (bones.size() != 0);
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*
EskTreeNode* ESKBone::importXml(TiXmlElement* xmlCurrentNode, std::vector<ESKBone*> &bones, EskTreeNode* treeParentNode, bool haveExtraBytesOnEachBone)
{
	EskTreeNode* treeCurrentNode = new EskTreeNode(this, bones.size(), treeParentNode);
	bones.push_back(this);
	
	xmlCurrentNode->QueryStringAttribute("name", &name);

	size_t tmp = 0;
	xmlCurrentNode->QueryUnsignedAttribute("unknow_index_4", &tmp);
	index_4 = (unsigned short)tmp;

	if (haveExtraBytesOnEachBone)
	{
		xmlCurrentNode->QueryUnsignedAttribute("extra_unk_0", &unk_extraInfo_0);
		xmlCurrentNode->QueryUnsignedAttribute("extra_unk_1", &unk_extraInfo_1);
		xmlCurrentNode->QueryUnsignedAttribute("extra_unk_2", &unk_extraInfo_2);
		xmlCurrentNode->QueryUnsignedAttribute("extra_unk_3", &unk_extraInfo_3);
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

			lineNode0->QueryFloatAttribute("x", &absoluteMatrix[0]);
			lineNode0->QueryFloatAttribute("y", &absoluteMatrix[1]);
			lineNode0->QueryFloatAttribute("z", &absoluteMatrix[2]);
			lineNode0->QueryFloatAttribute("w", &absoluteMatrix[3]);

			lineNode1->QueryFloatAttribute("x", &absoluteMatrix[4]);
			lineNode1->QueryFloatAttribute("y", &absoluteMatrix[5]);
			lineNode1->QueryFloatAttribute("z", &absoluteMatrix[6]);
			lineNode1->QueryFloatAttribute("w", &absoluteMatrix[7]);

			lineNode2->QueryFloatAttribute("x", &absoluteMatrix[8]);
			lineNode2->QueryFloatAttribute("y", &absoluteMatrix[9]);
			lineNode2->QueryFloatAttribute("z", &absoluteMatrix[10]);
			lineNode2->QueryFloatAttribute("w", &absoluteMatrix[11]);

			lineNode3->QueryFloatAttribute("x", &absoluteMatrix[12]);
			lineNode3->QueryFloatAttribute("y", &absoluteMatrix[13]);
			lineNode3->QueryFloatAttribute("z", &absoluteMatrix[14]);
			lineNode3->QueryFloatAttribute("w", &absoluteMatrix[15]);
		}
	}


	TiXmlElement* relativeTransformNode = xmlCurrentNode->FirstChildElement("RelativeTransform");
	if (relativeTransformNode)
	{
		TiXmlElement* positionNode = relativeTransformNode->FirstChildElement("Position");
		TiXmlElement* orientationNode = relativeTransformNode->FirstChildElement("Orientation");
		TiXmlElement* scaleNode = relativeTransformNode->FirstChildElement("Scale");

		if (positionNode)
		{
			positionNode->QueryFloatAttribute("x", &relativeTransform[0]);
			positionNode->QueryFloatAttribute("y", &relativeTransform[1]);
			positionNode->QueryFloatAttribute("z", &relativeTransform[2]);
			positionNode->QueryFloatAttribute("w", &relativeTransform[3]);
		}
		if (orientationNode)
		{
			orientationNode->QueryFloatAttribute("x", &relativeTransform[4]);
			orientationNode->QueryFloatAttribute("y", &relativeTransform[5]);
			orientationNode->QueryFloatAttribute("z", &relativeTransform[6]);
			orientationNode->QueryFloatAttribute("w", &relativeTransform[7]);
		}
		if (scaleNode)
		{
			scaleNode->QueryFloatAttribute("x", &relativeTransform[8]);
			scaleNode->QueryFloatAttribute("y", &relativeTransform[9]);
			scaleNode->QueryFloatAttribute("z", &relativeTransform[10]);
			scaleNode->QueryFloatAttribute("w", &relativeTransform[11]);
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
|                             exporFpftXml										 |
\-------------------------------------------------------------------------------*/
TiXmlElement* ESK::exporFpftXml(ESK* esk_sourceBonesName)
{
	TiXmlElement* xmlMainNode = new TiXmlElement("FPF");
	TiXmlElement* xmlFpfEntriesNode = new TiXmlElement("FpfEntries");

	TiXmlElement* xmlCurrentNode = new TiXmlElement("FPF_Entry");
	xmlCurrentNode->SetAttribute("ID", "XXX");
	xmlCurrentNode->SetAttribute("I_00", "XXX");

	size_t nb2 = bones.size();
	string name = "";
	for (size_t i = 0, nb = esk_sourceBonesName->bones.size(); i < nb; i++)
	{
		name = esk_sourceBonesName->bones.at(i)->getName();
		for (size_t j = 0; j < nb2; j++)
		{
			if (bones.at(j)->getName() == name)
			{
				xmlCurrentNode->LinkEndChild(bones.at(j)->exporFpftXml(i, bones));
				break;
			}
		}
	}
	
	xmlFpfEntriesNode->LinkEndChild(xmlCurrentNode);
	xmlMainNode->LinkEndChild(xmlFpfEntriesNode);
	return xmlMainNode;
}
/*-------------------------------------------------------------------------------\
|                             exporFpftXml										 |
\-------------------------------------------------------------------------------*/
TiXmlElement* ESKBone::exporFpftXml(size_t index, std::vector<ESKBone *> &listBones)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("FpfSubEntry");
	xmlCurrentNode->SetAttribute("Index", index);


	std::vector<double> relativeBone = calculRelativeMatrixFromAbsoluteMatrix(listBones);

	TiXmlElement* node = 0;
	TiXmlElement* valueNode = 0;
	for (size_t i = 0; i < 4; i++)
	{
		node = new TiXmlElement("Transform"+ ToString(i+1));			//begin to 1 ...
		xmlCurrentNode->LinkEndChild(node);

		valueNode = new TiXmlElement("F_00"); valueNode->SetDoubleAttribute("value", relativeBone.at(0)); node->LinkEndChild(valueNode);
		valueNode = new TiXmlElement("F_04"); valueNode->SetDoubleAttribute("value", relativeBone.at(1)); node->LinkEndChild(valueNode);
		valueNode = new TiXmlElement("F_08"); valueNode->SetDoubleAttribute("value", relativeBone.at(2)); node->LinkEndChild(valueNode);
		valueNode = new TiXmlElement("F_12"); valueNode->SetDoubleAttribute("value", relativeBone.at(3)); node->LinkEndChild(valueNode);
		valueNode = new TiXmlElement("F_16"); valueNode->SetDoubleAttribute("value", relativeBone.at(4)); node->LinkEndChild(valueNode);
		valueNode = new TiXmlElement("F_20"); valueNode->SetDoubleAttribute("value", relativeBone.at(5)); node->LinkEndChild(valueNode);
		valueNode = new TiXmlElement("F_24"); valueNode->SetDoubleAttribute("value", relativeBone.at(6)); node->LinkEndChild(valueNode);
		valueNode = new TiXmlElement("F_28"); valueNode->SetDoubleAttribute("value", relativeBone.at(7)); node->LinkEndChild(valueNode);
		valueNode = new TiXmlElement("F_32"); valueNode->SetDoubleAttribute("value", relativeBone.at(8)); node->LinkEndChild(valueNode);
		valueNode = new TiXmlElement("F_36"); valueNode->SetDoubleAttribute("value", relativeBone.at(9)); node->LinkEndChild(valueNode);
		valueNode = new TiXmlElement("F_40"); valueNode->SetDoubleAttribute("value", relativeBone.at(10)); node->LinkEndChild(valueNode);
		valueNode = new TiXmlElement("F_44"); valueNode->SetDoubleAttribute("value", relativeBone.at(11)); node->LinkEndChild(valueNode);
		valueNode = new TiXmlElement("F_48"); valueNode->SetDoubleAttribute("value", relativeBone.at(12)); node->LinkEndChild(valueNode);
		valueNode = new TiXmlElement("F_52"); valueNode->SetDoubleAttribute("value", relativeBone.at(13)); node->LinkEndChild(valueNode);
		valueNode = new TiXmlElement("F_56"); valueNode->SetDoubleAttribute("value", relativeBone.at(14)); node->LinkEndChild(valueNode);
		valueNode = new TiXmlElement("F_60"); valueNode->SetDoubleAttribute("value", relativeBone.at(15)); node->LinkEndChild(valueNode);
	}


	//Transform5 is iddentity normally (or small move)
	node = new TiXmlElement("Transform5");
	xmlCurrentNode->LinkEndChild(node);
	valueNode = new TiXmlElement("F_00"); valueNode->SetDoubleAttribute("value", 1); node->LinkEndChild(valueNode);
	valueNode = new TiXmlElement("F_04"); valueNode->SetDoubleAttribute("value", 0); node->LinkEndChild(valueNode);
	valueNode = new TiXmlElement("F_08"); valueNode->SetDoubleAttribute("value", 0); node->LinkEndChild(valueNode);
	valueNode = new TiXmlElement("F_12"); valueNode->SetDoubleAttribute("value", 0); node->LinkEndChild(valueNode);
	valueNode = new TiXmlElement("F_16"); valueNode->SetDoubleAttribute("value", 0); node->LinkEndChild(valueNode);
	valueNode = new TiXmlElement("F_20"); valueNode->SetDoubleAttribute("value", 1); node->LinkEndChild(valueNode);
	valueNode = new TiXmlElement("F_24"); valueNode->SetDoubleAttribute("value", 0); node->LinkEndChild(valueNode);
	valueNode = new TiXmlElement("F_28"); valueNode->SetDoubleAttribute("value", 0); node->LinkEndChild(valueNode);
	valueNode = new TiXmlElement("F_32"); valueNode->SetDoubleAttribute("value", 0); node->LinkEndChild(valueNode);
	valueNode = new TiXmlElement("F_36"); valueNode->SetDoubleAttribute("value", 0); node->LinkEndChild(valueNode);
	valueNode = new TiXmlElement("F_40"); valueNode->SetDoubleAttribute("value", 1); node->LinkEndChild(valueNode);
	valueNode = new TiXmlElement("F_44"); valueNode->SetDoubleAttribute("value", 0); node->LinkEndChild(valueNode);
	valueNode = new TiXmlElement("F_48"); valueNode->SetDoubleAttribute("value", 0); node->LinkEndChild(valueNode);
	valueNode = new TiXmlElement("F_52"); valueNode->SetDoubleAttribute("value", 0); node->LinkEndChild(valueNode);
	valueNode = new TiXmlElement("F_56"); valueNode->SetDoubleAttribute("value", 0); node->LinkEndChild(valueNode);
	valueNode = new TiXmlElement("F_60"); valueNode->SetDoubleAttribute("value", 1); node->LinkEndChild(valueNode);

	
	return xmlCurrentNode;
}








}