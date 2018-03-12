namespace LibXenoverse
{






/*-------------------------------------------------------------------------------\
|                             loadXml											 |
\-------------------------------------------------------------------------------*/
bool EAN::loadXml(string filename)
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
	TiXmlElement* rootNode = hDoc.FirstChildElement("EAN").Element();
	if (!rootNode)
	{
		printf("%s don't have 'EAN' tags. skip.'\n", filename);
		getchar();
		return false;
	}

	return importXml(rootNode);
}
/*-------------------------------------------------------------------------------\
|                             saveXml											 |
\-------------------------------------------------------------------------------*/
void EAN::saveXml(string filename)
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
bool EAN::importXml(TiXmlElement* xmlCurrentNode)
{
	xmlCurrentNode->QueryStringAttribute("name", &name);

	size_t unknown_total_tmp = 0;
	xmlCurrentNode->QueryUnsignedAttribute("unknown_total", &unknown_total_tmp);
	unknown_total = (unsigned int)unknown_total_tmp;

	string str = "";
	xmlCurrentNode->QueryStringAttribute("type", &str);
	type = (unsigned short)EMO_BaseFile::GetUnsigned(str);

	TiXmlElement* eskNode = xmlCurrentNode->FirstChildElement("ESK");
	if (!eskNode)
	{
		printf("No 'ESK' tags find. skip.'\n");
		getchar();
		return false;
	}
	skeleton = new ESK();
	if (!skeleton->importXml(eskNode))
	{
		delete skeleton;
		skeleton = 0;
	}


	TiXmlElement* eanAnimsNode = xmlCurrentNode->FirstChildElement("EANAnimations");
	if (!eanAnimsNode)
	{
		printf("No 'EANAnimations' tags find. skip.'\n");
		getchar();
		return false;
	}

	for (TiXmlElement* xmlNode = eanAnimsNode->FirstChildElement("EANAnimation"); xmlNode; xmlNode = xmlNode->NextSiblingElement("EANAnimation"))
	{
		EANAnimation eanAnimation;
		if (eanAnimation.importXml(xmlNode, this))
			animations.push_back(eanAnimation);
	}

	return ((skeleton) && (animations.size() != 0));
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EANAnimation::importXml(TiXmlElement* xmlCurrentNode, EAN* eanParent)
{
	xmlCurrentNode->QueryStringAttribute("name", &name);
	parent = eanParent;

	size_t tmp;
	xmlCurrentNode->QueryUnsignedAttribute("frameCount", &frame_count);

	if (xmlCurrentNode->QueryUnsignedAttribute("indexSize", &tmp) == TIXML_SUCCESS)
		frame_index_size = tmp;
	if (xmlCurrentNode->QueryUnsignedAttribute("floatSize", &tmp) == TIXML_SUCCESS)
		frame_float_size = tmp;

	if (frame_index_size > 1)					//security on implementation
		frame_index_size = 1;
	if (frame_float_size < 1)
		frame_float_size = 1;
	if (frame_float_size > 2)
		frame_float_size = 2;
	


	TiXmlElement* eanNodeNode = xmlCurrentNode->FirstChildElement("EANAnimationNodes");
	if (!eanNodeNode)
	{
		printf("No 'EANAnimationNodes' tags find. skip.'\n");
		getchar();
		return false;
	}

	for (TiXmlElement* xmlNode = eanNodeNode->FirstChildElement("EANAnimationNode"); xmlNode; xmlNode = xmlNode->NextSiblingElement("EANAnimationNode"))
	{
		EANAnimationNode eanAnimationNode;
		if (eanAnimationNode.importXml(xmlNode, eanParent->getSkeleton()))
			nodes.push_back(eanAnimationNode);
	}

	return (nodes.size() != 0);
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EANAnimationNode::importXml(TiXmlElement* xmlCurrentNode, ESK* esk)
{
	if (!esk)
		return false;

	string boneName;
	xmlCurrentNode->QueryStringAttribute("BoneName", &boneName);

	bone_index = esk->getBoneIndex(boneName);
	if (bone_index==(size_t)-1)
	{
		printf("Bone with name '%s' not found. skip.'\n", boneName.c_str());
		getchar();
		return false;
	}

	
	TiXmlElement* eanKFANode = xmlCurrentNode->FirstChildElement("EANKeyframedAnimations");
	if (!eanKFANode)
	{
		printf("No 'EANKeyframedAnimations' tags find. skip.'\n");
		getchar();
		return false;
	}

	for (TiXmlElement* xmlNode = eanKFANode->FirstChildElement("EANKeyframedAnimation"); xmlNode; xmlNode = xmlNode->NextSiblingElement("EANKeyframedAnimation"))
	{
		EANKeyframedAnimation eanKeyframedAnimation;
		if (eanKeyframedAnimation.importXml(xmlNode))
			keyframed_animations.push_back(eanKeyframedAnimation);
	}

	return (keyframed_animations.size()!=0);
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EANKeyframedAnimation::importXml(TiXmlElement* xmlCurrentNode)
{
	string typeName = "";
	xmlCurrentNode->QueryStringAttribute("type", &typeName);

	if (typeName == "position")
	{
		flag = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION;	//1792
	}else if ((typeName == "orientation")|| (typeName == "targetCameraPosition")) {
		flag = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION;	//1793
	}else if (typeName == "scale") {
		flag = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE;		//1794
	}else if (typeName == "camera") {
		flag = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_CAMERA;	//0770
	}else if(isNumber(typeName) ){
		flag = stoi(typeName.c_str());
	}else{
		printf("'type' needed. only could be 'position', 'orientation', 'scale' or 'camera' skip.'\n");
		getchar();
		return false;
	}



	TiXmlElement* eanKFNode = xmlCurrentNode->FirstChildElement("EANKeyframes");
	if (!eanKFNode)
	{
		printf("No 'EANKeyframes' tags find. skip.'\n");
		getchar();
		return false;
	}

	for (TiXmlElement* xmlNode = eanKFNode->FirstChildElement("EANKeyframe"); xmlNode; xmlNode = xmlNode->NextSiblingElement("EANKeyframe"))
	{
		EANKeyframe eanKeyframe;
		if (eanKeyframe.importXml(xmlNode, flag))
			keyframes.push_back(eanKeyframe);
	}

	return (keyframes.size() != 0);
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EANKeyframe::importXml(TiXmlElement* xmlCurrentNode, size_t flag)
{
	xmlCurrentNode->QueryUnsignedAttribute("frameIndex", &frame);

	double tmp;
	if (flag != LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_CAMERA)
	{
		if (xmlCurrentNode->QueryDoubleAttribute("x", &tmp) == TIXML_SUCCESS)
			x = (float)tmp;
		if (xmlCurrentNode->QueryDoubleAttribute("y", &tmp) == TIXML_SUCCESS)
			y = (float)tmp;

	}else {
		if (xmlCurrentNode->QueryDoubleAttribute("Roll", &tmp) == TIXML_SUCCESS)
			x = (float)tmp * 3.14159265358979f / 180.0f;
		if (xmlCurrentNode->QueryDoubleAttribute("Focale", &tmp) == TIXML_SUCCESS)
			y = (float)tmp * 3.14159265358979f / 180.0f;
	}
	if (xmlCurrentNode->QueryDoubleAttribute("z", &tmp) == TIXML_SUCCESS)
		z = (float)tmp;
	if (xmlCurrentNode->QueryDoubleAttribute("w", &tmp) == TIXML_SUCCESS)
		w = (float)tmp;

	return true;
}






/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EAN::exportXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("EAN");

	xmlCurrentNode->SetAttribute("name", name);
	xmlCurrentNode->SetAttribute("unknown_total", (size_t)unknown_total);
	xmlCurrentNode->SetAttribute("type", EMO_BaseFile::UnsignedToString(type,true));
	
	if(skeleton)
		xmlCurrentNode->LinkEndChild(skeleton->exportXml());


	TiXmlElement* eanAnimsNode = new TiXmlElement("EANAnimations");
	size_t nbAnim = animations.size();
	for (size_t i = 0; i < nbAnim; i++)
	{
		eanAnimsNode->LinkEndChild(new TiXmlComment(("index:" + std::to_string(i)).c_str()));
		eanAnimsNode->LinkEndChild(animations.at(i).exportXml(type));
	}

	xmlCurrentNode->LinkEndChild(eanAnimsNode);


	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EANAnimation::exportXml(size_t typeAnim)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("EANAnimation");

	xmlCurrentNode->SetAttribute("name", name);
	xmlCurrentNode->SetAttribute("frameCount", frame_count);

	xmlCurrentNode->SetAttribute("indexSize", frame_index_size);
	xmlCurrentNode->SetAttribute("floatSize", frame_float_size);


	TiXmlElement* eanNodeNode = new TiXmlElement("EANAnimationNodes");
	size_t nbNode = nodes.size();
	for (size_t i = 0; i < nbNode; i++)
		eanNodeNode->LinkEndChild(nodes.at(i).exportXml(parent->getSkeleton(), typeAnim));

	xmlCurrentNode->LinkEndChild(eanNodeNode);


	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EANAnimationNode::exportXml(ESK* esk, size_t typeAnim)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("EANAnimationNode");

	xmlCurrentNode->SetAttribute("BoneName", ((bone_index!=0xffff) ? esk->getBones().at(bone_index)->getName() : "") );


	TiXmlElement* eanKFANode = new TiXmlElement("EANKeyframedAnimations");
	
	size_t nbKeyframeAnim = keyframed_animations.size();
	for (size_t i = 0; i < nbKeyframeAnim; i++)
		eanKFANode->LinkEndChild(keyframed_animations.at(i).exportXml(typeAnim));

	xmlCurrentNode->LinkEndChild(eanKFANode);


	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EANKeyframedAnimation::exportXml(size_t typeAnim)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("EANKeyframedAnimation");

	switch (flag)
	{
	case LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION: xmlCurrentNode->SetAttribute("type", "position"); break;
	case LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION: xmlCurrentNode->SetAttribute("type", (typeAnim != 0x401) ? "orientation" : "targetCameraPosition"); break;
	case LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE: xmlCurrentNode->SetAttribute("type", "scale"); break;
	case LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_CAMERA: xmlCurrentNode->SetAttribute("type", "camera"); break;
	default: xmlCurrentNode->SetAttribute("type", flag); break;
	}
	

	TiXmlElement* eanKFNode = new TiXmlElement("EANKeyframes");

	size_t nbKeyframe = keyframes.size();
	for (size_t i = 0; i < nbKeyframe; i++)
		eanKFNode->LinkEndChild(keyframes.at(i).exportXml(flag));

	xmlCurrentNode->LinkEndChild(eanKFNode);

	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EANKeyframe::exportXml(size_t flag)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("EANKeyframe");

	xmlCurrentNode->SetAttribute("frameIndex", frame);
	if (flag != LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_CAMERA)
	{
		xmlCurrentNode->SetAttribute("x", FloatToString(x));
		xmlCurrentNode->SetAttribute("y", FloatToString(y));
	}else {
		xmlCurrentNode->SetAttribute("Roll", FloatToString(x * 180.0f / 3.14159265358979f));
		xmlCurrentNode->SetAttribute("Focale", FloatToString(y * 180.0f / 3.14159265358979f));
	}
	xmlCurrentNode->SetAttribute("z", FloatToString(z));
	xmlCurrentNode->SetAttribute("w", FloatToString(w));

	return xmlCurrentNode;
}










}