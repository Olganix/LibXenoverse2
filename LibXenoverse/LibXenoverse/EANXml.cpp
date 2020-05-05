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
		printf("Loading xml %s fail. skip.'\n", filename.c_str());
		getchar();
		return false;
	}

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);
	TiXmlElement* rootNode = hDoc.FirstChildElement("EAN").Element();
	if (!rootNode)
	{
		printf("%s don't have 'EAN' tags. skip.'\n", filename.c_str());
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
	xmlCurrentNode->QueryStringAttribute("version", &version);
	xmlCurrentNode->QueryUnsignedAttribute("unknow_0", &unknow_0);

	string str = "";
	xmlCurrentNode->QueryStringAttribute("type", &str);
	if (str == "Object")
		type = LIBXENOVERSE_EAN_ANIMATION_TYPE_NORMAL;
	else if (str == "Camera")
		type = LIBXENOVERSE_EAN_ANIMATION_TYPE_CAMERA;
	else
		type = (uint8_t)EMO_BaseFile::GetUnsigned(str);


	TiXmlElement* eskNode = xmlCurrentNode->FirstChildElement("ESK");
	if (!eskNode)
	{
		printf("No 'ESK' tags find. skip.'\n");
		getchar();
		return false;
	}
	skeleton = new ESK();
	skeleton->mHaveExtraBytesOnEachBone = (type==LIBXENOVERSE_EAN_ANIMATION_TYPE_NORMAL);		//camera have not extra.

	if (!skeleton->importXml(eskNode))
	{
		delete skeleton;
		skeleton = 0;
	}


	TiXmlElement* eanAnimsNode = xmlCurrentNode->FirstChildElement("Animations");
	if (!eanAnimsNode)
	{
		printf("No 'Animations' tags find. skip.'\n");
		getchar();
		return false;
	}

	for (TiXmlElement* xmlNode = eanAnimsNode->FirstChildElement("Animation"); xmlNode; xmlNode = xmlNode->NextSiblingElement("Animation"))
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
	xmlCurrentNode->QueryUnsignedAttribute("duration", &duration);

	string str = "";
	xmlCurrentNode->QueryStringAttribute("floatSize", &str);
	frame_float_size = ((str == "32bits") ? 2 : ((str == "16bits") ? 1 : std::stoi(str)));

	xmlCurrentNode->QueryUnsignedAttribute("unknow_0", &tmp);
	unknow_0 = (uint16_t)tmp;

	for (TiXmlElement* xmlNode = xmlCurrentNode->FirstChildElement("AnimationNode"); xmlNode; xmlNode = xmlNode->NextSiblingElement("AnimationNode"))
	{
		EANAnimationNode eanAnimationNode;
		if (eanAnimationNode.importXml(xmlNode, eanParent->getSkeleton(), eanParent->getType()))
			nodes.push_back(eanAnimationNode);
	}

	return true;							//case sdbh BBABL_01_CAM.ean, no nodes, but keep animations. (also strange because it's the same name for many animations)
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EANAnimationNode::importXml(TiXmlElement* xmlCurrentNode, ESK* esk, size_t type)
{
	if (!esk)
		return false;

	string boneName;
	xmlCurrentNode->QueryStringAttribute("name", &boneName);

	bone_index = esk->getBoneIndex(boneName);
	if (bone_index==(size_t)-1)
	{
		printf("Bone with name '%s' not found. skip.'\n", boneName.c_str());
		getchar();
		return false;
	}

	for (TiXmlElement* xmlNode = xmlCurrentNode->FirstChildElement("KeyframedAnimation"); xmlNode; xmlNode = xmlNode->NextSiblingElement("KeyframedAnimation"))
	{
		EANKeyframedAnimation eanKeyframedAnimation;
		if (eanKeyframedAnimation.importXml(xmlNode, type))
			keyframed_animations.push_back(eanKeyframedAnimation);
	}

	return true;						//case sdbh bc16gl00.ean, there is a animNode without keyframe.
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EANKeyframedAnimation::importXml(TiXmlElement* xmlCurrentNode, size_t type)
{
	uint32_t tmp = 0;
	string str = "";
	string typeName = "";
	xmlCurrentNode->QueryStringAttribute("type", &typeName);

	if (typeName == "position")
	{
		flag = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION;
	}else if ((typeName == "orientation")|| (typeName == "targetCameraPosition")) {
		flag = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION_or_TargetPosition;
	}else if ((typeName == "scale") || (typeName == "camera")) {
		flag = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE_or_CAMERA;
	}else{
		flag = EMO_BaseFile::GetUnsigned(typeName.c_str());
	}

	xmlCurrentNode->QueryStringAttribute("unknow_0", &str); unknow_0 = (uint8_t)EMO_BaseFile::GetUnsigned(str);
	xmlCurrentNode->QueryStringAttribute("unknow_1", &str); unknow_1 = (uint16_t)EMO_BaseFile::GetUnsigned(str);
	
	for (TiXmlElement* xmlNode = xmlCurrentNode->FirstChildElement("Keyframe"); xmlNode; xmlNode = xmlNode->NextSiblingElement("Keyframe"))
	{
		EANKeyframe eanKeyframe;
		if (eanKeyframe.importXml(xmlNode, flag, type))
			keyframes.push_back(eanKeyframe);
	}

	return (keyframes.size() != 0);
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EANKeyframe::importXml(TiXmlElement* xmlCurrentNode, size_t flag, size_t type)
{
	xmlCurrentNode->QueryUnsignedAttribute("frame", &frame);

	string str = "";
	if ((type!= LIBXENOVERSE_EAN_ANIMATION_TYPE_CAMERA) || (flag != LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE_or_CAMERA))
	{
		if (xmlCurrentNode->QueryStringAttribute("x", &str) == TIXML_SUCCESS)
			x = StringToFloat(str);
		if (xmlCurrentNode->QueryStringAttribute("y", &str) == TIXML_SUCCESS)
			y = StringToFloat(str);

	}else {
		/*
		if (xmlCurrentNode->QueryStringAttribute("Roll", &str) == TIXML_SUCCESS)
			x = StringToFloat(str) * 3.14159265358979f / 180.0f;
		if (xmlCurrentNode->QueryStringAttribute("Focale", &str) == TIXML_SUCCESS)
			y = StringToFloat(str) * 3.14159265358979f / 180.0f;
		*/

		if (xmlCurrentNode->QueryStringAttribute("Roll", &str) == TIXML_SUCCESS)				//todo remove and get previous (to test prefect rebuild we keep radians)
			x = StringToFloat(str);
		if (xmlCurrentNode->QueryStringAttribute("Focale", &str) == TIXML_SUCCESS)
			y = StringToFloat(str);
	}
	if (xmlCurrentNode->QueryStringAttribute("z", &str) == TIXML_SUCCESS)
		z = StringToFloat(str);
	if (xmlCurrentNode->QueryStringAttribute("w", &str) == TIXML_SUCCESS)
		w = StringToFloat(str);

	return true;
}






/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EAN::exportXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("EAN");

	xmlCurrentNode->SetAttribute("name", name);
	xmlCurrentNode->SetAttribute("version", version);

	switch (type)
	{
	case LIBXENOVERSE_EAN_ANIMATION_TYPE_NORMAL: xmlCurrentNode->SetAttribute("type", "Object"); break;
	case LIBXENOVERSE_EAN_ANIMATION_TYPE_CAMERA: xmlCurrentNode->SetAttribute("type", "Camera"); break;
	default: xmlCurrentNode->SetAttribute("type", EMO_BaseFile::UnsignedToString(type, true)); break;
	}

	xmlCurrentNode->SetAttribute("unknow_1", unknow_1);
	xmlCurrentNode->SetAttribute("unknow_0", unknow_0);	
	
	if(skeleton)
		xmlCurrentNode->LinkEndChild(skeleton->exportXml());


	TiXmlElement* eanAnimsNode = new TiXmlElement("Animations");
	size_t nbAnim = animations.size();
	for (size_t i = 0; i < nbAnim; i++)
	{
		eanAnimsNode->LinkEndChild(new TiXmlComment(("index:" + std::to_string(i)).c_str()));
		eanAnimsNode->LinkEndChild(animations.at(i).exportXml(type, version, i+1 == nbAnim));
	}

	xmlCurrentNode->LinkEndChild(eanAnimsNode);


	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EANAnimation::exportXml(size_t typeAnim, string version, bool isLast)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("Animation");

	xmlCurrentNode->SetAttribute("name", name);
	xmlCurrentNode->SetAttribute("duration", duration);
	xmlCurrentNode->SetAttribute("floatSize", ((frame_float_size == 2) ? "32bits" : ((frame_float_size == 1) ? "16bits" : ToString(frame_float_size))));
	xmlCurrentNode->SetAttribute("unknow_0", (size_t)unknow_0);


	for (size_t i = 0, nbNode = nodes.size(); i < nbNode; i++)
		xmlCurrentNode->LinkEndChild(nodes.at(i).exportXml(parent->getSkeleton(), typeAnim));

	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EANAnimationNode::exportXml(ESK* esk, size_t typeAnim)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("AnimationNode");

	string aa = ((bone_index != 0xffff) ? esk->getBones().at(bone_index)->getName() : "");				//todo remove.
	xmlCurrentNode->SetAttribute("name", ((bone_index!=0xffff) ? esk->getBones().at(bone_index)->getName() : "") );
	
	size_t nbKeyframeAnim = keyframed_animations.size();
	for (size_t i = 0; i < nbKeyframeAnim; i++)
		xmlCurrentNode->LinkEndChild(keyframed_animations.at(i).exportXml(typeAnim));

	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EANKeyframedAnimation::exportXml(size_t typeAnim)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("KeyframedAnimation");

	switch (flag)
	{
	case LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION: xmlCurrentNode->SetAttribute("type", "position"); break;
	case LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION_or_TargetPosition: xmlCurrentNode->SetAttribute("type", (typeAnim != LIBXENOVERSE_EAN_ANIMATION_TYPE_CAMERA) ? "orientation" : "targetCameraPosition"); break;
	case LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE_or_CAMERA: xmlCurrentNode->SetAttribute("type", (typeAnim != LIBXENOVERSE_EAN_ANIMATION_TYPE_CAMERA) ? "scale" : "camera"); break;
	default: xmlCurrentNode->SetAttribute("type", EMO_BaseFile::UnsignedToString(flag, true) ); break;
	}
	xmlCurrentNode->SetAttribute("unknow_0", EMO_BaseFile::UnsignedToString(unknow_0, true));
	xmlCurrentNode->SetAttribute("unknow_1", EMO_BaseFile::UnsignedToString(unknow_1, true));

	size_t nbKeyframe = keyframes.size();
	for (size_t i = 0; i < nbKeyframe; i++)
		xmlCurrentNode->LinkEndChild(keyframes.at(i).exportXml(flag, typeAnim));

	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EANKeyframe::exportXml(size_t flag, size_t typeAnim)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("Keyframe");

	xmlCurrentNode->SetAttribute("frame", frame);
	if ((typeAnim!= LIBXENOVERSE_EAN_ANIMATION_TYPE_CAMERA)||(flag != LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE_or_CAMERA))
	{
		xmlCurrentNode->SetAttribute("x", FloatToString(x));
		xmlCurrentNode->SetAttribute("y", FloatToString(y));
	}else {
		/*
		xmlCurrentNode->SetAttribute("Roll", FloatToString(x * 180.0f / 3.14159265358979f));
		xmlCurrentNode->SetAttribute("Focale", FloatToString(y * 180.0f / 3.14159265358979f));
		*/
		xmlCurrentNode->SetAttribute("Roll", FloatToString(x));				//todo remove and get previous (to test prefect rebuild we keep radians)
		xmlCurrentNode->SetAttribute("Focale", FloatToString(y));
	}
	xmlCurrentNode->SetAttribute("z", FloatToString(z));
	xmlCurrentNode->SetAttribute("w", FloatToString(w));

	return xmlCurrentNode;
}










}