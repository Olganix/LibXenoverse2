namespace LibXenoverse
{






/*-------------------------------------------------------------------------------\
|                             loadXml											 |
\-------------------------------------------------------------------------------*/
bool EMD::loadXml(string filename)
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
	TiXmlElement* rootNode = hDoc.FirstChildElement("EMD").Element();
	if (!rootNode)
	{
		printf("%s don't have 'EMD' tags. skip.'\n", filename);
		getchar();
		return false;
	}

	return importXml(rootNode);
}
/*-------------------------------------------------------------------------------\
|                             saveXml											 |
\-------------------------------------------------------------------------------*/
void EMD::saveXml(string filename)
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
bool EMD::importXml(TiXmlElement* xmlCurrentNode)
{
	xmlCurrentNode->QueryStringAttribute("name", &name);

	size_t unknown_total_tmp = 0;
	xmlCurrentNode->QueryUnsignedAttribute("unknown_total", &unknown_total_tmp);
	unknown_total = (unsigned short)unknown_total_tmp;

	TiXmlElement* modelsNode = xmlCurrentNode->FirstChildElement("EmdModels");
	if (!modelsNode)
	{
		printf("No 'EmdModels' tags find. skip.'\n");
		getchar();
		return false;
	}

	for (TiXmlElement* xmlNode = modelsNode->FirstChildElement("EmdModel"); xmlNode; xmlNode = xmlNode->NextSiblingElement("EmdModel"))
	{
		EMDModel* emdModel = new EMDModel();
		if (emdModel->importXml(xmlNode))
			models.push_back(emdModel);
		else
			delete emdModel;
	}

	return (models.size() != 0);
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EMDModel::importXml(TiXmlElement* xmlCurrentNode)
{
	xmlCurrentNode->QueryStringAttribute("name", &name);

	size_t unknown_total_tmp = 0;
	xmlCurrentNode->QueryUnsignedAttribute("unknown_total", &unknown_total_tmp);
	unknown_total = (unsigned short)unknown_total_tmp;


	TiXmlElement* meshNode = xmlCurrentNode->FirstChildElement("EmdMeshs");
	if (!meshNode)
	{
		printf("No 'EmdMeshs' tags find. skip.'\n");
		getchar();
		return false;
	}

	for (TiXmlElement* xmlNode = meshNode->FirstChildElement("EmdMesh"); xmlNode; xmlNode = xmlNode->NextSiblingElement("EmdMesh"))
	{
		EMDMesh* emdMesh = new EMDMesh();
		if (emdMesh->importXml(xmlNode))
			meshes.push_back(emdMesh);
		else
			delete emdMesh;
	}

	return (meshes.size() != 0);
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EMDMesh::importXml(TiXmlElement* xmlCurrentNode)
{
	xmlCurrentNode->QueryStringAttribute("name", &name);

	size_t unknown_total_tmp = 0;
	xmlCurrentNode->QueryUnsignedAttribute("unknown_total", &unknown_total_tmp);
	unknown_total = (unsigned short)unknown_total_tmp;

	TiXmlElement* aabb = xmlCurrentNode->FirstChildElement("AABB");			//axis boundingBox
	if (aabb)
	{
		TiXmlElement* aabb_min = aabb->FirstChildElement("Min");
		TiXmlElement* aabb_max = aabb->FirstChildElement("Max");
		TiXmlElement* aabb_center = aabb->FirstChildElement("Center");

		if (aabb_min)
		{
			aabb_min->QueryFloatAttribute("x", &aabb_min_x);
			aabb_min->QueryFloatAttribute("y", &aabb_min_y);
			aabb_min->QueryFloatAttribute("z", &aabb_min_z);
			aabb_min->QueryFloatAttribute("w", &aabb_min_w);
		}

		if (aabb_max)
		{
			aabb_max->QueryFloatAttribute("x", &aabb_max_x);
			aabb_max->QueryFloatAttribute("y", &aabb_max_y);
			aabb_max->QueryFloatAttribute("z", &aabb_max_z);
			aabb_max->QueryFloatAttribute("w", &aabb_max_w);
		}

		if (aabb_center)
		{
			aabb_center->QueryFloatAttribute("x", &aabb_center_x);
			aabb_center->QueryFloatAttribute("y", &aabb_center_y);
			aabb_center->QueryFloatAttribute("z", &aabb_center_z);
			aabb_center->QueryFloatAttribute("w", &aabb_center_w);
		}
	}


	TiXmlElement* submeshNode = xmlCurrentNode->FirstChildElement("EmdSubMeshs");
	if (!submeshNode)
	{
		printf("No 'EmdSubMeshs' tags find. skip.'\n");
		getchar();
		return false;
	}

	for (TiXmlElement* xmlNode = submeshNode->FirstChildElement("EMDSubmesh"); xmlNode; xmlNode = xmlNode->NextSiblingElement("EMDSubmesh"))
	{
		EMDSubmesh* emdSubMesh = new EMDSubmesh();
		if (emdSubMesh->importXml(xmlNode))
			submeshes.push_back(emdSubMesh);
		else
			delete emdSubMesh;
	}

	return (submeshes.size()!=0);
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EMDSubmesh::importXml(TiXmlElement* xmlCurrentNode)
{
	xmlCurrentNode->QueryStringAttribute("name", &name);


	// Vertex Definition
	TiXmlElement* flagsNode = xmlCurrentNode->FirstChildElement("VertexDefinition");
	if (!flagsNode)
	{
		printf("No 'VertexDefinition' tags find. Please take a look at a '.emd.xml' file or document about Emd.'\n");
		getchar();
		return false;
	}

	
	string vertexDefintion = string(flagsNode->GetText());
	std::vector<string> sv = split(vertexDefintion, '|');

	vertex_type_flag = 0;
	size_t nbArg = sv.size();
	for (size_t i = 0; i < nbArg; i++)
	{
		string &arg = sv.at(i);
		if (arg == "Position")
			vertex_type_flag = vertex_type_flag | EMD_VTX_FLAG_POS;
		else if (arg == "Normal")
			vertex_type_flag = vertex_type_flag | EMD_VTX_FLAG_NORM;
		else if (arg == "Tangent")
			vertex_type_flag = vertex_type_flag | EMD_VTX_FLAG_TANGENT;
		else if (arg == "UV")
			vertex_type_flag = vertex_type_flag | EMD_VTX_FLAG_TEX;
		else if (arg == "UV2")
			vertex_type_flag = vertex_type_flag | EMD_VTX_FLAG_TEX2;
		else if (arg == "Color")
			vertex_type_flag = vertex_type_flag | EMD_VTX_FLAG_COLOR;
		else if (arg == "BoneBlendWeight")
			vertex_type_flag = vertex_type_flag | EMD_VTX_FLAG_BLEND_WEIGHT;
		else if (arg == "CompressedFormat")
			vertex_type_flag = vertex_type_flag | EMD_VTX_FLAG_COMPRESSED_FORMAT;
	}
	vertex_size = EMDVertex::getSizeFromFlags(vertex_type_flag, true);


	TiXmlElement* aabb = xmlCurrentNode->FirstChildElement("AABB");			//axis boundingBox
	if (aabb)
	{
		TiXmlElement* aabb_min = aabb->FirstChildElement("Min");
		TiXmlElement* aabb_max = aabb->FirstChildElement("Max");
		TiXmlElement* aabb_center = aabb->FirstChildElement("Center");

		if (aabb_min)
		{
			aabb_min->QueryFloatAttribute("x", &aabb_min_x);
			aabb_min->QueryFloatAttribute("y", &aabb_min_y);
			aabb_min->QueryFloatAttribute("z", &aabb_min_z);
			aabb_min->QueryFloatAttribute("w", &aabb_min_w);
		}

		if (aabb_max)
		{
			aabb_max->QueryFloatAttribute("x", &aabb_max_x);
			aabb_max->QueryFloatAttribute("y", &aabb_max_y);
			aabb_max->QueryFloatAttribute("z", &aabb_max_z);
			aabb_max->QueryFloatAttribute("w", &aabb_max_w);
		}

		if (aabb_center)
		{
			aabb_center->QueryFloatAttribute("x", &aabb_center_x);
			aabb_center->QueryFloatAttribute("y", &aabb_center_y);
			aabb_center->QueryFloatAttribute("z", &aabb_center_z);
			aabb_center->QueryFloatAttribute("w", &aabb_center_w);
		}
	}


	TiXmlElement* vertexNode = xmlCurrentNode->FirstChildElement("Vertices");
	if (!vertexNode)
	{
		printf("No 'Vertices' tags find. skip.'\n");
		getchar();
		return false;
	}
	TiXmlElement* triangleNode = xmlCurrentNode->FirstChildElement("TriangleList");
	if (!triangleNode)
	{
		printf("No 'TriangleList' tags find. skip.'\n");
		getchar();
		return false;
	}
	TiXmlElement* definitionNode = xmlCurrentNode->FirstChildElement("TextureDefinitions");
	if (!definitionNode)
	{
		printf("No 'TextureDefinitions' tags find. skip.'\n");
		getchar();
		return false;
	}
	

	
	for (TiXmlElement* xmlNode = vertexNode->FirstChildElement("vertex"); xmlNode; xmlNode = xmlNode->NextSiblingElement("vertex"))
	{
		EMDVertex emdVertex;
		if (emdVertex.importXml(xmlNode, vertex_type_flag))
			vertices.push_back(emdVertex);
	}
	for (TiXmlElement* xmlNode = triangleNode->FirstChildElement("EMDTriangles"); xmlNode; xmlNode = xmlNode->NextSiblingElement("EMDTriangles"))
	{
		EMDTriangles emdTriangles;
		if (emdTriangles.importXml(xmlNode, vertices))
			triangles.push_back(emdTriangles);
	}
	for (TiXmlElement* xmlNode = definitionNode->FirstChildElement("EMDSubmeshDefinition"); xmlNode; xmlNode = xmlNode->NextSiblingElement("EMDSubmeshDefinition"))
	{
		EMDSubmeshDefinition emdSubmeshDefinition;
		if (emdSubmeshDefinition.importXml(xmlNode))
			definitions.push_back(emdSubmeshDefinition);
	}

	return ( (triangles.size()!=0) && (vertices.size()!=0) );
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EMDVertex::importXml(TiXmlElement* xmlCurrentNode, uint16_t parentflags)
{
	flags = parentflags;


	TiXmlElement* positionNode = xmlCurrentNode->FirstChildElement("position");				//notice: I give a definition near the xml format of Ogre.
	TiXmlElement* normalNode = xmlCurrentNode->FirstChildElement("normal");
	TiXmlElement* tangentNode = xmlCurrentNode->FirstChildElement("tangent");
	TiXmlElement* texcoordNode = xmlCurrentNode->FirstChildElement("texcoord");
	TiXmlElement* texcoord2Node = ((texcoordNode) ? texcoordNode->NextSiblingElement("texcoord") : 0);
	TiXmlElement* colorNode = xmlCurrentNode->FirstChildElement("color");
	TiXmlElement* blendIndiceNode = xmlCurrentNode->FirstChildElement("blendIndice");
	TiXmlElement* blendWeightNode = xmlCurrentNode->FirstChildElement("blendWeight");


	if ((flags & EMD_VTX_FLAG_POS) && (positionNode))
	{
		positionNode->QueryFloatAttribute("x", &pos_x);
		positionNode->QueryFloatAttribute("y", &pos_y);
		positionNode->QueryFloatAttribute("z", &pos_z);
	}

	if ((flags & EMD_VTX_FLAG_NORM) && (normalNode))
	{
		normalNode->QueryFloatAttribute("x", &norm_x);
		normalNode->QueryFloatAttribute("y", &norm_y);
		normalNode->QueryFloatAttribute("z", &norm_z);
	}

	if ((flags & EMD_VTX_FLAG_TANGENT) && (tangentNode))
	{
		tangentNode->QueryFloatAttribute("x", &tang_x);
		tangentNode->QueryFloatAttribute("y", &tang_y);
		tangentNode->QueryFloatAttribute("z", &tang_z);
	}

	if ((flags & EMD_VTX_FLAG_TEX) && (texcoordNode))
	{
		texcoordNode->QueryFloatAttribute("u", &text_u);
		texcoordNode->QueryFloatAttribute("v", &text_v);
	}

	if ((flags & EMD_VTX_FLAG_TEX2) && (texcoord2Node))
	{
		texcoord2Node->QueryFloatAttribute("u", &text2_u);
		texcoord2Node->QueryFloatAttribute("v", &text2_v);
	}

	if ((flags & EMD_VTX_FLAG_COLOR) && (colorNode))
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 0.0f;
		colorNode->QueryFloatAttribute("r", &r);
		colorNode->QueryFloatAttribute("g", &g);
		colorNode->QueryFloatAttribute("b", &b);
		colorNode->QueryFloatAttribute("b", &a);

		setColorFromRGBAFloat(r, g, b, a);
	}

	if ((flags & EMD_VTX_FLAG_BLEND_WEIGHT) && (blendIndiceNode))
	{
		size_t tmp = 0;
		if(blendIndiceNode->QueryUnsignedAttribute("i0", &tmp) == TIXML_SUCCESS)
			blend[0] = tmp;
		if(blendIndiceNode->QueryUnsignedAttribute("i1", &tmp) == TIXML_SUCCESS)
			blend[1] = tmp;
		if(blendIndiceNode->QueryUnsignedAttribute("i2", &tmp) == TIXML_SUCCESS)
			blend[2] = tmp;
		if(blendIndiceNode->QueryUnsignedAttribute("i3", &tmp) == TIXML_SUCCESS)
			blend[3] = tmp;
	}

	if ((flags & EMD_VTX_FLAG_BLEND_WEIGHT) && (blendWeightNode))
	{
		blendWeightNode->QueryFloatAttribute("w0", &blend_weight[0]);
		blendWeightNode->QueryFloatAttribute("w1", &blend_weight[1]);
		blendWeightNode->QueryFloatAttribute("w2", &blend_weight[2]);
		blendWeightNode->QueryFloatAttribute("w3", &blend_weight[3]);
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EMDTriangles::importXml(TiXmlElement* xmlCurrentNode, vector<EMDVertex> &vertices)
{
	TiXmlElement* bonesNode = xmlCurrentNode->FirstChildElement("Bones");
	if (!bonesNode)
	{
		printf("No 'Bones' tags find. skip.'\n");
		getchar();
		return false;
	}
	TiXmlElement* facesNode = xmlCurrentNode->FirstChildElement("Faces");
	if (!facesNode)
	{
		printf("No 'Faces' tags find. skip.'\n");
		getchar();
		return false;
	}


	string name = "";
	for (TiXmlElement* xmlNode = bonesNode->FirstChildElement("Bone"); xmlNode; xmlNode = xmlNode->NextSiblingElement("Bone"))
		if(xmlNode->QueryStringAttribute("name", &name)== TIXML_SUCCESS)
			bone_names.push_back(name);


	size_t nbVertex = vertices.size();
	size_t v1, v2, v3;
	v1 = 0;
	v2 = 0;
	v3 = 0;
	for (TiXmlElement* xmlNode = facesNode->FirstChildElement("face"); xmlNode; xmlNode = xmlNode->NextSiblingElement("face"))
	{
		if((xmlNode->QueryUnsignedAttribute("v1", &v1) != TIXML_SUCCESS) || (v1 >= nbVertex))
			continue;
		if((xmlNode->QueryUnsignedAttribute("v2", &v2) != TIXML_SUCCESS) || (v2 >= nbVertex))
			continue;
		if((xmlNode->QueryUnsignedAttribute("v3", &v3) != TIXML_SUCCESS) || (v3 >= nbVertex))
			continue;

		faces.push_back((unsigned short)v1);
		faces.push_back((unsigned short)v2);
		faces.push_back((unsigned short)v3);
	}

	return (faces.size()!=0);
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EMDSubmeshDefinition::importXml(TiXmlElement* xmlCurrentNode)
{

	size_t tmp;
	if (xmlCurrentNode->QueryUnsignedAttribute("unknow0", &tmp) == TIXML_SUCCESS)
		flag0 = (unsigned char)tmp;

	if (xmlCurrentNode->QueryUnsignedAttribute("textureindex", &tmp) == TIXML_SUCCESS)
		texIndex = (unsigned char)tmp;

	if (xmlCurrentNode->QueryUnsignedAttribute("unknow1", &tmp) == TIXML_SUCCESS)
		flag1 = (unsigned char)tmp;

	if (xmlCurrentNode->QueryUnsignedAttribute("unknow2", &tmp) == TIXML_SUCCESS)
		flag2 = (unsigned char)tmp;

	xmlCurrentNode->QueryFloatAttribute("textScale_u", &textScale_u);
	xmlCurrentNode->QueryFloatAttribute("textScale_v", &textScale_v);


	return true;
}






/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMD::exportXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("EMD");

	xmlCurrentNode->SetAttribute("name", name);
	xmlCurrentNode->SetAttribute("unknown_total", (size_t)unknown_total);
	
	TiXmlElement* modelsNode = new TiXmlElement("EmdModels");
	size_t nbmodel = models.size();
	for (size_t i = 0; i < nbmodel; i++)
		modelsNode->LinkEndChild(models.at(i)->exportXml());

	xmlCurrentNode->LinkEndChild(modelsNode);


	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMDModel::exportXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("EmdModel");

	xmlCurrentNode->SetAttribute("name", name);
	xmlCurrentNode->SetAttribute("unknown_total", (size_t)unknown_total);

	TiXmlElement* meshNode = new TiXmlElement("EmdMeshs");
	size_t nbMesh = meshes.size();
	for (size_t i = 0; i < nbMesh; i++)
		meshNode->LinkEndChild(meshes.at(i)->exportXml());

	xmlCurrentNode->LinkEndChild(meshNode);


	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMDMesh::exportXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("EmdMesh");

	xmlCurrentNode->SetAttribute("name", name);
	xmlCurrentNode->SetAttribute("unknown_total", (size_t)unknown_total);



	// AABB
	TiXmlElement* aabb = new TiXmlElement("AABB");
	TiXmlElement* aabb_min = new TiXmlElement("Min");
	TiXmlElement* aabb_max = new TiXmlElement("Max");
	TiXmlElement* aabb_center = new TiXmlElement("Center");

	aabb_min->SetDoubleAttribute("x", aabb_min_x);
	aabb_min->SetDoubleAttribute("y", aabb_min_y);
	aabb_min->SetDoubleAttribute("z", aabb_min_z);
	aabb_min->SetDoubleAttribute("w", aabb_min_w);

	aabb_max->SetDoubleAttribute("x", aabb_max_x);
	aabb_max->SetDoubleAttribute("y", aabb_max_y);
	aabb_max->SetDoubleAttribute("z", aabb_max_z);
	aabb_max->SetDoubleAttribute("w", aabb_max_w);

	aabb_center->SetDoubleAttribute("x", aabb_center_x);
	aabb_center->SetDoubleAttribute("y", aabb_center_y);
	aabb_center->SetDoubleAttribute("z", aabb_center_z);
	aabb_center->SetDoubleAttribute("w", aabb_center_w);

	aabb->LinkEndChild(aabb_min);
	aabb->LinkEndChild(aabb_max);
	aabb->LinkEndChild(aabb_center);
	xmlCurrentNode->LinkEndChild(aabb);



	// submesh
	TiXmlElement* submeshNode = new TiXmlElement("EmdSubMeshs");
	size_t nbsubMesh = submeshes.size();
	for (size_t i = 0; i < nbsubMesh; i++)
		submeshNode->LinkEndChild(submeshes.at(i)->exportXml());

	xmlCurrentNode->LinkEndChild(submeshNode);


	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMDSubmesh::exportXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("EMDSubmesh");

	xmlCurrentNode->SetAttribute("name", name);
	


	//vertex Definition
	TiXmlElement* flagsNode = new TiXmlElement("VertexDefinition");
	size_t flags = vertex_type_flag;
	string flagsStr = "";
	if (flags & EMD_VTX_FLAG_POS)
		flagsStr += "Position|";
	if (flags & EMD_VTX_FLAG_NORM)
		flagsStr += "Normal|";
	if (flags & EMD_VTX_FLAG_TANGENT)
		flagsStr += "Tangent|";
	if (flags & EMD_VTX_FLAG_TEX)
		flagsStr += "UV|";
	if (flags & EMD_VTX_FLAG_TEX2)
		flagsStr += "UV2|";
	if (flags & EMD_VTX_FLAG_COLOR)
		flagsStr += "Color|";
	if (flags & EMD_VTX_FLAG_BLEND_WEIGHT)
		flagsStr += "BoneBlendWeight|";
	if (flags & EMD_VTX_FLAG_COMPRESSED_FORMAT)
		flagsStr += "CompressedFormat|";
	if (flagsStr.length() != 0)
		flagsStr = flagsStr.substr(0, flagsStr.length() - 1);

	TiXmlText* flagsValue = new TiXmlText("flags");
	flagsValue->SetValue(flagsStr);
	flagsNode->LinkEndChild(flagsValue);
	xmlCurrentNode->LinkEndChild(flagsNode);


	// AABB
	TiXmlElement* aabb = new TiXmlElement("AABB");
	TiXmlElement* aabb_min = new TiXmlElement("Min");
	TiXmlElement* aabb_max = new TiXmlElement("Max");
	TiXmlElement* aabb_center = new TiXmlElement("Center");

	aabb_min->SetDoubleAttribute("x", aabb_min_x);
	aabb_min->SetDoubleAttribute("y", aabb_min_y);
	aabb_min->SetDoubleAttribute("z", aabb_min_z);
	aabb_min->SetDoubleAttribute("w", aabb_min_w);

	aabb_max->SetDoubleAttribute("x", aabb_max_x);
	aabb_max->SetDoubleAttribute("y", aabb_max_y);
	aabb_max->SetDoubleAttribute("z", aabb_max_z);
	aabb_max->SetDoubleAttribute("w", aabb_max_w);

	aabb_center->SetDoubleAttribute("x", aabb_center_x);
	aabb_center->SetDoubleAttribute("y", aabb_center_y);
	aabb_center->SetDoubleAttribute("z", aabb_center_z);
	aabb_center->SetDoubleAttribute("w", aabb_center_w);

	aabb->LinkEndChild(aabb_min);
	aabb->LinkEndChild(aabb_max);
	aabb->LinkEndChild(aabb_center);
	xmlCurrentNode->LinkEndChild(aabb);


	TiXmlElement* vertexNode = new TiXmlElement("Vertices");
	size_t nbVertex = vertices.size();
	for (size_t i = 0; i < nbVertex; i++)
		vertexNode->LinkEndChild(vertices.at(i).exportXml());
	xmlCurrentNode->LinkEndChild(vertexNode);


	TiXmlElement* triangleNode = new TiXmlElement("TriangleList");
	size_t nbTriangle = triangles.size();
	for (size_t i = 0; i < nbTriangle; i++)
		triangleNode->LinkEndChild(triangles.at(i).exportXml());
	xmlCurrentNode->LinkEndChild(triangleNode);

	TiXmlElement* definitionNode = new TiXmlElement("TextureDefinitions");
	size_t nbDef = definitions.size();
	for (size_t i = 0; i < nbDef; i++)
		definitionNode->LinkEndChild(definitions.at(i).exportXml());
	xmlCurrentNode->LinkEndChild(definitionNode);

	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMDVertex::exportXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("vertex");

	if (flags & EMD_VTX_FLAG_POS)
	{
		TiXmlElement* positionNode = new TiXmlElement("position");
		positionNode->SetDoubleAttribute("x", pos_x);
		positionNode->SetDoubleAttribute("y", pos_y);
		positionNode->SetDoubleAttribute("z", pos_z);
		xmlCurrentNode->LinkEndChild(positionNode);
	}

	if (flags & EMD_VTX_FLAG_NORM)
	{
		TiXmlElement* normalNode = new TiXmlElement("normal");
		normalNode->SetDoubleAttribute("x", norm_x);
		normalNode->SetDoubleAttribute("y", norm_y);
		normalNode->SetDoubleAttribute("z", norm_z);
		xmlCurrentNode->LinkEndChild(normalNode);
	}
	if (flags & EMD_VTX_FLAG_TANGENT)
	{
		TiXmlElement* tangentNode = new TiXmlElement("tangent");
		tangentNode->SetDoubleAttribute("x", tang_x);
		tangentNode->SetDoubleAttribute("y", tang_y);
		tangentNode->SetDoubleAttribute("z", tang_z);
		xmlCurrentNode->LinkEndChild(tangentNode);
	}


	if ((flags & EMD_VTX_FLAG_TEX) || (flags & EMD_VTX_FLAG_TEX2))
	{
		TiXmlElement* texcoordNode = new TiXmlElement("texcoord");
		texcoordNode->SetDoubleAttribute("u", text_u);
		texcoordNode->SetDoubleAttribute("v", text_v);
		xmlCurrentNode->LinkEndChild(texcoordNode);
	}
	if (flags & EMD_VTX_FLAG_TEX2)
	{
		TiXmlElement* texcoord2Node = new TiXmlElement("texcoord");
		texcoord2Node->SetDoubleAttribute("u", text2_u);
		texcoord2Node->SetDoubleAttribute("v", text2_v);
		xmlCurrentNode->LinkEndChild(texcoord2Node);
	}


	if (flags & EMD_VTX_FLAG_COLOR)
	{
		TiXmlElement* colorNode = new TiXmlElement("color");
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 0.0f;
		getColorRGBAFloat(r, g, b, a);
		colorNode->SetDoubleAttribute("r", r);
		colorNode->SetDoubleAttribute("g", g);
		colorNode->SetDoubleAttribute("b", b);
		colorNode->SetDoubleAttribute("a", a);
		xmlCurrentNode->LinkEndChild(colorNode);
	}


	if (flags & EMD_VTX_FLAG_BLEND_WEIGHT)
	{
		TiXmlElement* blendIndiceNode = new TiXmlElement("blendIndice");
		blendIndiceNode->SetAttribute("i0", blend[0]);
		blendIndiceNode->SetAttribute("i1", blend[1]);
		blendIndiceNode->SetAttribute("i2", blend[2]);
		blendIndiceNode->SetAttribute("i3", blend[3]);
		xmlCurrentNode->LinkEndChild(blendIndiceNode);

		TiXmlElement* blendWeightNode = new TiXmlElement("blendWeight");
		blendWeightNode->SetDoubleAttribute("w0", blend_weight[0]);
		blendWeightNode->SetDoubleAttribute("w1", blend_weight[1]);
		blendWeightNode->SetDoubleAttribute("w2", blend_weight[2]);
		blendWeightNode->SetDoubleAttribute("w3", blend_weight[3]);
		xmlCurrentNode->LinkEndChild(blendWeightNode);
	}

	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMDTriangles::exportXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("EMDTriangles");

	
	TiXmlElement* bonesNode = new TiXmlElement("Bones");
	TiXmlElement* xmlNode;
	size_t nbBones = bone_names.size();
	for (size_t i = 0; i < nbBones; i++)
	{
		xmlNode = new TiXmlElement("Bone");
		xmlNode->SetAttribute("name", bone_names.at(i));
		bonesNode->LinkEndChild(xmlNode);
	}
	xmlCurrentNode->LinkEndChild(bonesNode);
	

	TiXmlElement* facesNode = new TiXmlElement("Faces");
	size_t nbFaces = faces.size();
	for (size_t i = 0; i+2 < nbFaces; i+=3)
	{
		xmlNode = new TiXmlElement("face");
		xmlNode->SetAttribute("v1", faces.at(i));
		xmlNode->SetAttribute("v2", faces.at(i + 1));
		xmlNode->SetAttribute("v3", faces.at(i + 2));
		facesNode->LinkEndChild(xmlNode);
	}
	xmlCurrentNode->LinkEndChild(facesNode);

	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMDSubmeshDefinition::exportXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("EMDSubmeshDefinition");

	xmlCurrentNode->SetAttribute("unknow0", flag0);
	xmlCurrentNode->SetAttribute("textureindex", texIndex);
	xmlCurrentNode->SetAttribute("unknow1", flag1);
	xmlCurrentNode->SetAttribute("unknow2", flag2);
	xmlCurrentNode->SetDoubleAttribute("textScale_u", textScale_u);
	xmlCurrentNode->SetDoubleAttribute("textScale_v", textScale_v);

	return xmlCurrentNode;
}









}