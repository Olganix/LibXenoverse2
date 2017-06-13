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
		printf("Loading xml %s fail. skip.'\n", filename);
		getchar();
		return false;
	}

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);
	TiXmlElement* rootNode = hDoc.FirstChildElement("ESK").Element();
	if (!rootNode)
	{
		printf("%s don't have 'ESK' tags. skip.'\n", filename);
		getchar();
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

	size_t tmp;
	if (xmlCurrentNode->QueryUnsignedAttribute("flag", &tmp) == TIXML_SUCCESS)
		flag = (unsigned short)tmp;

	xmlCurrentNode->QueryUnsignedAttribute("unknown_offset_0", &unknown_offset_0);
	xmlCurrentNode->QueryUnsignedAttribute("unknown_offset_1", &unknown_offset_1);
	xmlCurrentNode->QueryUnsignedAttribute("unknown_offset_2", &unknown_offset_2);
	xmlCurrentNode->QueryUnsignedAttribute("unknown_offset_3", &unknown_offset_3);
	xmlCurrentNode->QueryBoolAttribute("have128unknownBytes", &mHave128unknownBytes);


	TiXmlElement* bonesNode = xmlCurrentNode->FirstChildElement("ESKBones");
	if (!bonesNode)
	{
		printf("No 'ESKBones' tags find. skip.'\n");
		getchar();
		return false;
	}

	//node will be organize into hieirarchy, to be more readable.
	EskTreeNode* treeRootNode = new EskTreeNode(nullptr, (size_t)-1, nullptr);

	
	for (TiXmlElement* xmlNode = bonesNode->FirstChildElement("ESKBone"); xmlNode; xmlNode = xmlNode->NextSiblingElement("ESKBone"))
	{
		ESKBone* eskBone = new ESKBone();
		EskTreeNode* treeNode = eskBone->importXml(xmlNode, bones, treeRootNode);
		if (treeNode)
		{
			treeRootNode->mChildren.push_back(treeNode);
		}else {
			delete eskBone;
		}
	}
	
	setTreeOrganisation(treeRootNode);					//read hieirarchy to make the list of bones.
	
	
	delete treeRootNode;					//delete the hieirarchy organisation, but not the bones

	return (bones.size() != 0);
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
EskTreeNode* ESKBone::importXml(TiXmlElement* xmlCurrentNode, std::vector<ESKBone*> &bones, EskTreeNode* treeParentNode)
{
	EskTreeNode* treeCurrentNode = new EskTreeNode(this, bones.size(), treeParentNode);
	bones.push_back(this);
	
	xmlCurrentNode->QueryStringAttribute("name", &name);

	size_t tmp = 0;
	xmlCurrentNode->QueryUnsignedAttribute("unknown_index_4", &tmp);
	index_4 = (unsigned short)tmp;


	TiXmlElement* absoluteTransformMatrixNode = xmlCurrentNode->FirstChildElement("AbsoluteTransformMatrix");
	haveTransformMatrix = (absoluteTransformMatrixNode != 0);
	if (haveTransformMatrix)
	{
		TiXmlElement* lineNode0 = absoluteTransformMatrixNode->FirstChildElement("Line");
		TiXmlElement* lineNode1 = (lineNode0) ? lineNode0->NextSiblingElement("Line") : 0;
		TiXmlElement* lineNode2 = (lineNode1) ? lineNode1->NextSiblingElement("Line") : 0;
		TiXmlElement* lineNode3 = (lineNode2) ? lineNode2->NextSiblingElement("Line") : 0;

		if (!lineNode3)
		{
			printf("AbsoluteTransformMatrix incomplete number of 'Line'. skip.'\n");
			getchar();
		}else{

			lineNode0->QueryFloatAttribute("x", &transform_matrix[0]);
			lineNode0->QueryFloatAttribute("y", &transform_matrix[1]);
			lineNode0->QueryFloatAttribute("z", &transform_matrix[2]);
			lineNode0->QueryFloatAttribute("w", &transform_matrix[3]);

			lineNode1->QueryFloatAttribute("x", &transform_matrix[4]);
			lineNode1->QueryFloatAttribute("y", &transform_matrix[5]);
			lineNode1->QueryFloatAttribute("z", &transform_matrix[6]);
			lineNode1->QueryFloatAttribute("w", &transform_matrix[7]);

			lineNode2->QueryFloatAttribute("x", &transform_matrix[8]);
			lineNode2->QueryFloatAttribute("y", &transform_matrix[9]);
			lineNode2->QueryFloatAttribute("z", &transform_matrix[10]);
			lineNode2->QueryFloatAttribute("w", &transform_matrix[11]);

			lineNode3->QueryFloatAttribute("x", &transform_matrix[12]);
			lineNode3->QueryFloatAttribute("y", &transform_matrix[13]);
			lineNode3->QueryFloatAttribute("z", &transform_matrix[14]);
			lineNode3->QueryFloatAttribute("w", &transform_matrix[15]);
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
			positionNode->QueryFloatAttribute("x", &skinning_matrix[0]);
			positionNode->QueryFloatAttribute("y", &skinning_matrix[1]);
			positionNode->QueryFloatAttribute("z", &skinning_matrix[2]);
			positionNode->QueryFloatAttribute("w", &skinning_matrix[3]);
		}
		if (orientationNode)
		{
			orientationNode->QueryFloatAttribute("x", &skinning_matrix[4]);
			orientationNode->QueryFloatAttribute("y", &skinning_matrix[5]);
			orientationNode->QueryFloatAttribute("z", &skinning_matrix[6]);
			orientationNode->QueryFloatAttribute("w", &skinning_matrix[7]);
		}
		if (scaleNode)
		{
			scaleNode->QueryFloatAttribute("x", &skinning_matrix[8]);
			scaleNode->QueryFloatAttribute("y", &skinning_matrix[9]);
			scaleNode->QueryFloatAttribute("z", &skinning_matrix[10]);
			scaleNode->QueryFloatAttribute("w", &skinning_matrix[11]);
		}
	}
	
	


	// Same for children
	for (TiXmlElement* xmlNode = xmlCurrentNode->FirstChildElement("ESKBone"); xmlNode; xmlNode = xmlNode->NextSiblingElement("ESKBone"))
	{
		ESKBone* eskBone = new ESKBone();
		EskTreeNode* treeNode = eskBone->importXml(xmlNode, bones, treeCurrentNode);
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
	xmlCurrentNode->SetAttribute("flag", (size_t)flag);
	xmlCurrentNode->SetAttribute("unknown_offset_0", (size_t)unknown_offset_0);
	xmlCurrentNode->SetAttribute("unknown_offset_1", (size_t)unknown_offset_1);
	xmlCurrentNode->SetAttribute("unknown_offset_2", (size_t)unknown_offset_2);
	xmlCurrentNode->SetAttribute("unknown_offset_3", (size_t)unknown_offset_3);
	xmlCurrentNode->SetAttribute("have128unknownBytes", mHave128unknownBytes);

	
	
	TiXmlElement* bonesNode = new TiXmlElement("ESKBones");

	EskTreeNode* rootTreeNode = getTreeOrganisation();		//get by hieirarchy.

	EskTreeNode* childTreeNode;
	size_t nbchild = rootTreeNode->mChildren.size();
	for (size_t i = 0; i < nbchild; i++)
	{
		childTreeNode = rootTreeNode->mChildren.at(i);
		bonesNode->LinkEndChild(childTreeNode->mBone->exportXml(childTreeNode));
	}

	xmlCurrentNode->LinkEndChild(bonesNode);

	delete rootTreeNode;

	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* ESKBone::exportXml(EskTreeNode* treeNode)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("ESKBone");

	xmlCurrentNode->SetAttribute("name", name);

	xmlCurrentNode->SetAttribute("unknown_index_4", (size_t)index_4);

	if (haveTransformMatrix)
	{
		TiXmlElement* absoluteTransformMatrixNode = new TiXmlElement("AbsoluteTransformMatrix");

		TiXmlElement* lineNode0 = new TiXmlElement("Line");
		TiXmlElement* lineNode1 = new TiXmlElement("Line");
		TiXmlElement* lineNode2 = new TiXmlElement("Line");
		TiXmlElement* lineNode3 = new TiXmlElement("Line");


		lineNode0->SetDoubleAttribute("x", transform_matrix[0]);
		lineNode0->SetDoubleAttribute("y", transform_matrix[1]);
		lineNode0->SetDoubleAttribute("z", transform_matrix[2]);
		lineNode0->SetDoubleAttribute("w", transform_matrix[3]);

		lineNode1->SetDoubleAttribute("x", transform_matrix[4]);
		lineNode1->SetDoubleAttribute("y", transform_matrix[5]);
		lineNode1->SetDoubleAttribute("z", transform_matrix[6]);
		lineNode1->SetDoubleAttribute("w", transform_matrix[7]);

		lineNode2->SetDoubleAttribute("x", transform_matrix[8]);
		lineNode2->SetDoubleAttribute("y", transform_matrix[9]);
		lineNode2->SetDoubleAttribute("z", transform_matrix[10]);
		lineNode2->SetDoubleAttribute("w", transform_matrix[11]);

		lineNode3->SetDoubleAttribute("x", transform_matrix[12]);
		lineNode3->SetDoubleAttribute("y", transform_matrix[13]);
		lineNode3->SetDoubleAttribute("z", transform_matrix[14]);
		lineNode3->SetDoubleAttribute("w", transform_matrix[15]);

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


		positionNode->SetDoubleAttribute("x", skinning_matrix[0]);
		positionNode->SetDoubleAttribute("y", skinning_matrix[1]);
		positionNode->SetDoubleAttribute("z", skinning_matrix[2]);
		positionNode->SetDoubleAttribute("w", skinning_matrix[3]);

		orientationNode->SetDoubleAttribute("x", skinning_matrix[4]);
		orientationNode->SetDoubleAttribute("y", skinning_matrix[5]);
		orientationNode->SetDoubleAttribute("z", skinning_matrix[6]);
		orientationNode->SetDoubleAttribute("w", skinning_matrix[7]);

		scaleNode->SetDoubleAttribute("x", skinning_matrix[8]);
		scaleNode->SetDoubleAttribute("y", skinning_matrix[9]);
		scaleNode->SetDoubleAttribute("z", skinning_matrix[10]);
		scaleNode->SetDoubleAttribute("w", skinning_matrix[11]);

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
		xmlCurrentNode->LinkEndChild(childTreeNode->mBone->exportXml(childTreeNode));
	}

	return xmlCurrentNode;
}








}