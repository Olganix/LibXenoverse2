namespace LibXenoverse
{



#include "EMD.h"


/*-------------------------------------------------------------------------------\
|                             loadXml											 |
\-------------------------------------------------------------------------------*/
bool EMD::loadXml(string filename)
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
	TiXmlElement* rootNode = hDoc.FirstChildElement("EMD").Element();
	if (!rootNode)
	{
		printf("%s don't have 'EMD' tags. skip.'\n", filename.c_str());
		notifyError();
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
	size_t tmp = 0;
	string str = "";

	xmlCurrentNode->QueryStringAttribute("name", &name);

	xmlCurrentNode->QueryStringAttribute("version", &version);
	xmlCurrentNode->QueryUnsignedAttribute("unk_0", &unknow_0);			//always 0
	xmlCurrentNode->QueryUnsignedAttribute("unk_1", &tmp); unknow_1 = (uint16_t)tmp;		//always 0

	xmlCurrentNode->QueryStringAttribute("paddingForCompressedVertex", &str); paddingForCompressedVertex = EMO_BaseFile::GetUnsigned(str);
	

	TiXmlElement* modelsNode = xmlCurrentNode->FirstChildElement("EmdModels");
	if (!modelsNode)
	{
		printf("No 'EmdModels' tags find. skip.'\n");
		notifyError();
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

	size_t tmp = 0;
	xmlCurrentNode->QueryUnsignedAttribute("unk_0", &tmp);
	unknow_0 = (unsigned short)tmp;


	TiXmlElement* meshNode = xmlCurrentNode->FirstChildElement("Meshes");
	if (!meshNode)
	{
		printf("No 'EmdMeshs' tags find. skip.'\n");
		notifyError();
		return false;
	}

	for (TiXmlElement* xmlNode = meshNode->FirstChildElement("Mesh"); xmlNode; xmlNode = xmlNode->NextSiblingElement("Mesh"))
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

	size_t tmp = 0;
	xmlCurrentNode->QueryUnsignedAttribute("unk_0", &tmp);
	unknow_0 = (unsigned short)tmp;

	TiXmlElement* aabb = xmlCurrentNode->FirstChildElement("AABB");			//axis boundingBox
	if (aabb)
	{
		TiXmlElement* aabb_min = aabb->FirstChildElement("Min");
		TiXmlElement* aabb_max = aabb->FirstChildElement("Max");
		TiXmlElement* aabb_center = aabb->FirstChildElement("Center");

		string str = "";
		if (aabb_min)
		{
			aabb_min->QueryStringAttribute("x", &str); aabb_min_x = StringToFloat(str);
			aabb_min->QueryStringAttribute("y", &str); aabb_min_y = StringToFloat(str);
			aabb_min->QueryStringAttribute("z", &str); aabb_min_z = StringToFloat(str);
			aabb_min->QueryStringAttribute("w", &str); aabb_min_w = StringToFloat(str);
		}

		if (aabb_max)
		{
			aabb_max->QueryStringAttribute("x", &str); aabb_max_x = StringToFloat(str);
			aabb_max->QueryStringAttribute("y", &str); aabb_max_y = StringToFloat(str);
			aabb_max->QueryStringAttribute("z", &str); aabb_max_z = StringToFloat(str);
			aabb_max->QueryStringAttribute("w", &str); aabb_max_w = StringToFloat(str);
		}

		if (aabb_center)
		{
			aabb_center->QueryStringAttribute("x", &str); aabb_center_x = StringToFloat(str);
			aabb_center->QueryStringAttribute("y", &str); aabb_center_y = StringToFloat(str);
			aabb_center->QueryStringAttribute("z", &str); aabb_center_z = StringToFloat(str);
			aabb_center->QueryStringAttribute("w", &str); aabb_center_w = StringToFloat(str);
		}
	}


	TiXmlElement* submeshNode = xmlCurrentNode->FirstChildElement("SubMeshes");
	if (!submeshNode)
	{
		printf("No 'EmdSubMeshs' tags find. skip.'\n");
		notifyError();
		return false;
	}

	for (TiXmlElement* xmlNode = submeshNode->FirstChildElement("Submesh"); xmlNode; xmlNode = xmlNode->NextSiblingElement("Submesh"))
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
	size_t tmp = 0;
	xmlCurrentNode->QueryUnsignedAttribute("unk_0", &tmp);
	unknow_0 = (uint8_t)tmp;

	// Vertex Definition
	TiXmlElement* flagsNode = xmlCurrentNode->FirstChildElement("VertexDefinition");
	if (!flagsNode)
	{
		printf("No 'VertexDefinition' tags find. Please take a look at a '.emd.xml' file or document about Emd.'\n");
		notifyError();
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

		string str = "";
		if (aabb_min)
		{
			aabb_min->QueryStringAttribute("x", &str); aabb_min_x = StringToFloat(str);
			aabb_min->QueryStringAttribute("y", &str); aabb_min_y = StringToFloat(str);
			aabb_min->QueryStringAttribute("z", &str); aabb_min_z = StringToFloat(str);
			aabb_min->QueryStringAttribute("w", &str); aabb_min_w = StringToFloat(str);
		}

		if (aabb_max)
		{
			aabb_max->QueryStringAttribute("x", &str); aabb_max_x = StringToFloat(str);
			aabb_max->QueryStringAttribute("y", &str); aabb_max_y = StringToFloat(str);
			aabb_max->QueryStringAttribute("z", &str); aabb_max_z = StringToFloat(str);
			aabb_max->QueryStringAttribute("w", &str); aabb_max_w = StringToFloat(str);
		}

		if (aabb_center)
		{
			aabb_center->QueryStringAttribute("x", &str); aabb_center_x = StringToFloat(str);
			aabb_center->QueryStringAttribute("y", &str); aabb_center_y = StringToFloat(str);
			aabb_center->QueryStringAttribute("z", &str); aabb_center_z = StringToFloat(str);
			aabb_center->QueryStringAttribute("w", &str); aabb_center_w = StringToFloat(str);
		}
	}

	TiXmlElement* definitionNode = xmlCurrentNode->FirstChildElement("TextureDefinitions");
	if (!definitionNode)
	{
		printf("No 'TextureDefinitions' tags find. skip.'\n");
		notifyError();
		return false;
	}
	TiXmlElement* vertexNode = xmlCurrentNode->FirstChildElement("Vertices");
	if (!vertexNode)
	{
		printf("No 'Vertices' tags find. skip.'\n");
		notifyError();
		return false;
	}
	TiXmlElement* triangleNode = xmlCurrentNode->FirstChildElement("TriangleList");
	if (!triangleNode)
	{
		printf("No 'TriangleList' tags find. skip.'\n");
		notifyError();
		return false;
	}
	
	

	for (TiXmlElement* xmlNode = definitionNode->FirstChildElement("TextureUnitState"); xmlNode; xmlNode = xmlNode->NextSiblingElement("TextureUnitState"))
	{
		EMDTextureUnitState emdTextureUnitState;
		if (emdTextureUnitState.importXml(xmlNode))
			definitions.push_back(emdTextureUnitState);
	}
	for (TiXmlElement* xmlNode = vertexNode->FirstChildElement("vertex"); xmlNode; xmlNode = xmlNode->NextSiblingElement("vertex"))
	{
		EMDVertex emdVertex;
		if (emdVertex.importXml(xmlNode, vertex_type_flag))
			vertices.push_back(emdVertex);
	}
	for (TiXmlElement* xmlNode = triangleNode->FirstChildElement("Triangles"); xmlNode; xmlNode = xmlNode->NextSiblingElement("Triangles"))
	{
		EMDTriangles emdTriangles;
		if (emdTriangles.importXml(xmlNode, vertices))
			triangles.push_back(emdTriangles);
	}
	

	return true;						// it could have 0 vertex, and 0 triangle but textures (ex: TE1_002_Face_base.emd)
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

	string str = "";
	if ((flags & EMD_VTX_FLAG_POS) && (positionNode))
	{
		positionNode->QueryStringAttribute("x", &str); pos_x = StringToFloat(str);
		positionNode->QueryStringAttribute("y", &str); pos_y = StringToFloat(str);
		positionNode->QueryStringAttribute("z", &str); pos_z = StringToFloat(str);
	}

	if ((flags & EMD_VTX_FLAG_NORM) && (normalNode))
	{
		normalNode->QueryStringAttribute("x", &str); norm_x = StringToFloat(str);
		normalNode->QueryStringAttribute("y", &str); norm_y = StringToFloat(str);
		normalNode->QueryStringAttribute("z", &str); norm_z = StringToFloat(str);
	}

	if ((flags & EMD_VTX_FLAG_TANGENT) && (tangentNode))
	{
		tangentNode->QueryStringAttribute("x", &str); tang_x = StringToFloat(str);
		tangentNode->QueryStringAttribute("y", &str); tang_y = StringToFloat(str);
		tangentNode->QueryStringAttribute("z", &str); tang_z = StringToFloat(str);
	}

	if ((flags & EMD_VTX_FLAG_TEX) && (texcoordNode))
	{
		texcoordNode->QueryStringAttribute("u", &str); text_u = StringToFloat(str);
		texcoordNode->QueryStringAttribute("v", &str); text_v = StringToFloat(str);
	}

	if ((flags & EMD_VTX_FLAG_TEX2) && (texcoord2Node))
	{
		texcoord2Node->QueryStringAttribute("u", &str); text2_u = StringToFloat(str);
		texcoord2Node->QueryStringAttribute("v", &str); text2_v = StringToFloat(str);
	}

	if ((flags & EMD_VTX_FLAG_COLOR) && (colorNode))
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 0.0f;
		colorNode->QueryStringAttribute("r", &str); r = StringToFloat(str);
		colorNode->QueryStringAttribute("g", &str); g = StringToFloat(str);
		colorNode->QueryStringAttribute("b", &str); b = StringToFloat(str);
		colorNode->QueryStringAttribute("a", &str); a = StringToFloat(str);

		setColorFromRGBAFloat(r, g, b, a);
	}

	if ((flags & EMD_VTX_FLAG_BLEND_WEIGHT) && (blendIndiceNode))
	{
		size_t tmp = 0;
		if(blendIndiceNode->QueryUnsignedAttribute("i0", &tmp) == TIXML_SUCCESS) blend[0] = tmp;
		if(blendIndiceNode->QueryUnsignedAttribute("i1", &tmp) == TIXML_SUCCESS) blend[1] = tmp;
		if(blendIndiceNode->QueryUnsignedAttribute("i2", &tmp) == TIXML_SUCCESS) blend[2] = tmp;
		if(blendIndiceNode->QueryUnsignedAttribute("i3", &tmp) == TIXML_SUCCESS) blend[3] = tmp;
	}

	if ((flags & EMD_VTX_FLAG_BLEND_WEIGHT) && (blendWeightNode))
	{
		blendWeightNode->QueryStringAttribute("w0", &str); blend_weight[0] = StringToFloat(str);
		blendWeightNode->QueryStringAttribute("w1", &str); blend_weight[1] = StringToFloat(str);
		blendWeightNode->QueryStringAttribute("w2", &str); blend_weight[2] = StringToFloat(str);
		blendWeightNode->QueryStringAttribute("w3", &str); blend_weight[3] = StringToFloat(str);
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EMDTriangles::importXml(TiXmlElement* xmlCurrentNode, vector<EMDVertex> &vertices)
{
	TiXmlElement* bonesNode = xmlCurrentNode->FirstChildElement("SkinningBones");
	if (!bonesNode)
	{
		printf("No 'Bones' tags find. skip.'\n");
		notifyError();
		return false;
	}
	TiXmlElement* facesNode = xmlCurrentNode->FirstChildElement("Faces");
	if (!facesNode)
	{
		printf("No 'Faces' tags find. skip.'\n");
		notifyError();
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

		faces.push_back(v1);
		faces.push_back(v2);
		faces.push_back(v3);
	}

	return (faces.size()!=0);
}
/*-------------------------------------------------------------------------------\
|                             importXml											 |
\-------------------------------------------------------------------------------*/
bool EMDTextureUnitState::importXml(TiXmlElement* xmlCurrentNode)
{

	size_t tmp;
	if (xmlCurrentNode->QueryUnsignedAttribute("textureindex", &tmp) == TIXML_SUCCESS)
		texIndex = (unsigned char)tmp;


	string str = "";
	textScale_u = textScale_v = 1;
	xmlCurrentNode->QueryStringAttribute("textScale_uv", &str);
	std::vector<char> listSeparators;
	listSeparators.push_back(' ');
	listSeparators.push_back(',');
	std::vector<string> sv = multiSplit(str, listSeparators);
	size_t inc = 0;
	for (size_t i = 0, nb = sv.size(); i < nb; i++)
	{
		if (sv.at(i).length() == 0)
			continue;

		if (inc == 0)
			textScale_u = StringToFloat(sv.at(i));
		else
			textScale_v = StringToFloat(sv.at(i));

		inc++;
		if (inc == 2)
			break;
	}


	adressMode_u = adressMode_v = EMD_TUS_ADRESSMODE_WRAP;
	xmlCurrentNode->QueryStringAttribute("adressMode_uv", &str);
	sv = multiSplit(str, listSeparators);
	inc = 0;
	for (size_t i = 0, nb = sv.size(); i < nb; i++)
	{
		str = sv.at(i);
		if (str.length() == 0)
			continue;

		if (inc == 0)
			adressMode_u = ((str == "Wrap") ? EMD_TUS_ADRESSMODE_WRAP : ((str == "Mirror") ? EMD_TUS_ADRESSMODE_MIRROR : ((str == "Clamp") ? EMD_TUS_ADRESSMODE_CLAMP : (uint8_t)stoi(str))));
		else
			adressMode_v = ((str == "Wrap") ? EMD_TUS_ADRESSMODE_WRAP : ((str == "Mirror") ? EMD_TUS_ADRESSMODE_MIRROR : ((str == "Clamp") ? EMD_TUS_ADRESSMODE_CLAMP : (uint8_t)stoi(str))));

		inc++;
		if (inc == 2)
			break;
	}


	filtering_minification = filtering_magnification = EMD_TUS_FILTERING_NONE;
	xmlCurrentNode->QueryStringAttribute("filtering_min_mag", &str);
	sv = multiSplit(str, listSeparators);
	inc = 0;
	for (size_t i = 0, nb = sv.size(); i < nb; i++)
	{
		str = sv.at(i);
		if (str.length() == 0)
			continue;

		if (inc == 0)
			filtering_minification = ((str == "None") ? EMD_TUS_FILTERING_NONE : ((str == "Point") ? EMD_TUS_FILTERING_POINT : ((str == "Linear") ? EMD_TUS_FILTERING_LINEAR : (uint8_t)stoi(str))));
		else
			filtering_magnification = ((str == "None") ? EMD_TUS_FILTERING_NONE : ((str == "Point") ? EMD_TUS_FILTERING_POINT : ((str == "Linear") ? EMD_TUS_FILTERING_LINEAR : (uint8_t)stoi(str))));

		inc++;
		if (inc == 2)
			break;
	}
	
	if (xmlCurrentNode->QueryUnsignedAttribute("unk_0", &tmp) == TIXML_SUCCESS)
		flag0 = (unsigned char)tmp;

	return true;
}






/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMD::exportXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("EMD");

	xmlCurrentNode->SetAttribute("name", name);
	xmlCurrentNode->SetAttribute("version", version);
	xmlCurrentNode->SetAttribute("paddingForCompressedVertex", EMO_BaseFile::UnsignedToString(paddingForCompressedVertex, true));

	xmlCurrentNode->SetAttribute("unk_0", unknow_0);							//always 0
	xmlCurrentNode->SetAttribute("unk_1", (size_t)unknow_1);					//always 0

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
	xmlCurrentNode->SetAttribute("unk_0", (size_t)unknow_0);

	TiXmlElement* meshNode = new TiXmlElement("Meshes");
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
	TiXmlElement* xmlCurrentNode = new TiXmlElement("Mesh");

	xmlCurrentNode->SetAttribute("name", name);
	xmlCurrentNode->SetAttribute("unk_0", (size_t)unknow_0);
	


	// AABB
	TiXmlElement* aabb = new TiXmlElement("AABB");
	TiXmlElement* aabb_min = new TiXmlElement("Min");
	TiXmlElement* aabb_max = new TiXmlElement("Max");
	TiXmlElement* aabb_center = new TiXmlElement("Center");

	aabb_min->SetAttribute("x", FloatToString(aabb_min_x) );
	aabb_min->SetAttribute("y", FloatToString(aabb_min_y) );
	aabb_min->SetAttribute("z", FloatToString(aabb_min_z) );
	aabb_min->SetAttribute("w", FloatToString(aabb_min_w) );

	aabb_max->SetAttribute("x", FloatToString(aabb_max_x) );
	aabb_max->SetAttribute("y", FloatToString(aabb_max_y) );
	aabb_max->SetAttribute("z", FloatToString(aabb_max_z) );
	aabb_max->SetAttribute("w", FloatToString(aabb_max_w) );

	aabb_center->SetAttribute("x", FloatToString(aabb_center_x) );
	aabb_center->SetAttribute("y", FloatToString(aabb_center_y) );
	aabb_center->SetAttribute("z", FloatToString(aabb_center_z) );
	aabb_center->SetAttribute("w", FloatToString(aabb_center_w) );

	aabb->LinkEndChild(aabb_min);
	aabb->LinkEndChild(aabb_max);
	aabb->LinkEndChild(aabb_center);
	xmlCurrentNode->LinkEndChild(aabb);



	// submesh
	TiXmlElement* submeshNode = new TiXmlElement("SubMeshes");
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
	TiXmlElement* xmlCurrentNode = new TiXmlElement("Submesh");

	xmlCurrentNode->SetAttribute("name", name);
	xmlCurrentNode->SetAttribute("unk_0", unknow_0);
	


	//vertex Definition
	TiXmlElement* flagsNode = new TiXmlElement("VertexDefinition");
	//xmlCurrentNode->SetAttribute("vertex_type_flag", EMO_BaseFile::UnsignedToString(vertex_type_flag, true));

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




	TiXmlElement* definitionNode = new TiXmlElement("TextureDefinitions");
	size_t nbDef = definitions.size();
	for (size_t i = 0; i < nbDef; i++)
		definitionNode->LinkEndChild(definitions.at(i).exportXml());
	xmlCurrentNode->LinkEndChild(definitionNode);



	// AABB
	TiXmlElement* aabb = new TiXmlElement("AABB");
	TiXmlElement* aabb_min = new TiXmlElement("Min");
	TiXmlElement* aabb_max = new TiXmlElement("Max");
	TiXmlElement* aabb_center = new TiXmlElement("Center");

	aabb_min->SetAttribute("x", FloatToString(aabb_min_x));
	aabb_min->SetAttribute("y", FloatToString(aabb_min_y));
	aabb_min->SetAttribute("z", FloatToString(aabb_min_z));
	aabb_min->SetAttribute("w", FloatToString(aabb_min_w));

	aabb_max->SetAttribute("x", FloatToString(aabb_max_x));
	aabb_max->SetAttribute("y", FloatToString(aabb_max_y));
	aabb_max->SetAttribute("z", FloatToString(aabb_max_z));
	aabb_max->SetAttribute("w", FloatToString(aabb_max_w));

	aabb_center->SetAttribute("x", FloatToString(aabb_center_x));
	aabb_center->SetAttribute("y", FloatToString(aabb_center_y));
	aabb_center->SetAttribute("z", FloatToString(aabb_center_z));
	aabb_center->SetAttribute("w", FloatToString(aabb_center_w));

	aabb->LinkEndChild(aabb_min);
	aabb->LinkEndChild(aabb_max);
	aabb->LinkEndChild(aabb_center);
	xmlCurrentNode->LinkEndChild(aabb);


	TiXmlElement* vertexNode = new TiXmlElement("Vertices");
	size_t nbVertex = vertices.size();
	vertexNode->SetAttribute("nbVertex", nbVertex);							//test todo remove
	for (size_t i = 0; i < nbVertex; i++)
		vertexNode->LinkEndChild(vertices.at(i).exportXml());
	xmlCurrentNode->LinkEndChild(vertexNode);


	TiXmlElement* triangleNode = new TiXmlElement("TriangleList");
	size_t nbTriangle = triangles.size();
	for (size_t i = 0; i < nbTriangle; i++)
		triangleNode->LinkEndChild(triangles.at(i).exportXml(vertices));
	xmlCurrentNode->LinkEndChild(triangleNode);

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
		positionNode->SetAttribute("x", FloatToString(pos_x) );
		positionNode->SetAttribute("y", FloatToString(pos_y) );
		positionNode->SetAttribute("z", FloatToString(pos_z) );
		xmlCurrentNode->LinkEndChild(positionNode);
	}

	if (flags & EMD_VTX_FLAG_NORM)
	{
		TiXmlElement* normalNode = new TiXmlElement("normal");
		normalNode->SetAttribute("x", FloatToString(norm_x) );
		normalNode->SetAttribute("y", FloatToString(norm_y) );
		normalNode->SetAttribute("z", FloatToString(norm_z) );
		xmlCurrentNode->LinkEndChild(normalNode);
	}
	if (flags & EMD_VTX_FLAG_TANGENT)
	{
		TiXmlElement* tangentNode = new TiXmlElement("tangent");
		tangentNode->SetAttribute("x", FloatToString(tang_x) );
		tangentNode->SetAttribute("y", FloatToString(tang_y) );
		tangentNode->SetAttribute("z", FloatToString(tang_z) );
		xmlCurrentNode->LinkEndChild(tangentNode);
	}


	if ((flags & EMD_VTX_FLAG_TEX) || (flags & EMD_VTX_FLAG_TEX2))
	{
		TiXmlElement* texcoordNode = new TiXmlElement("texcoord");
		texcoordNode->SetAttribute("u", FloatToString(text_u) );
		texcoordNode->SetAttribute("v", FloatToString(text_v) );
		xmlCurrentNode->LinkEndChild(texcoordNode);
	}
	if (flags & EMD_VTX_FLAG_TEX2)
	{
		TiXmlElement* texcoord2Node = new TiXmlElement("texcoord");
		texcoord2Node->SetAttribute("u", FloatToString(text2_u) );
		texcoord2Node->SetAttribute("v", FloatToString(text2_v) );
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
		colorNode->SetAttribute("r", FloatToString(r) );
		colorNode->SetAttribute("g", FloatToString(g) );
		colorNode->SetAttribute("b", FloatToString(b) );
		colorNode->SetAttribute("a", FloatToString(a) );
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
		blendWeightNode->SetAttribute("w0", FloatToString(blend_weight[0]) );
		blendWeightNode->SetAttribute("w1", FloatToString(blend_weight[1]) );
		blendWeightNode->SetAttribute("w2", FloatToString(blend_weight[2]) );
		blendWeightNode->SetAttribute("w3", FloatToString(blend_weight[3]) );
		xmlCurrentNode->LinkEndChild(blendWeightNode);
	}

	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportXml											 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMDTriangles::exportXml(vector<EMDVertex> &vertices)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("Triangles");
	xmlCurrentNode->SetAttribute("nbFaces", faces.size());
	
	TiXmlElement* bonesNode = new TiXmlElement("SkinningBones");
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
TiXmlElement* EMDTextureUnitState::exportXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("TextureUnitState");

	/*
	xmlCurrentNode->SetAttribute("textureindex", texIndex);
	xmlCurrentNode->SetDoubleAttribute("textScale_u", textScale_u);
	xmlCurrentNode->SetDoubleAttribute("textScale_v", textScale_v);

	xmlCurrentNode->SetAttribute("adressMode_u", ((adressMode_u & EMD_TUS_ADRESSMODE_CLAMP) ? "Clamp" : ((adressMode_u & EMD_TUS_ADRESSMODE_MIRROR) ? "Mirror" : "Wrap")) );
	xmlCurrentNode->SetAttribute("adressMode_v", ((adressMode_v & EMD_TUS_ADRESSMODE_CLAMP) ? "Clamp" : ((adressMode_v & EMD_TUS_ADRESSMODE_MIRROR) ? "Mirror" : "Wrap")));

	xmlCurrentNode->SetAttribute("filtering_minification", ((filtering_minification & EMD_TUS_FILTERING_LINEAR) ? "Linear" : ((filtering_minification & EMD_TUS_FILTERING_POINT) ? "Point" : ((filtering_minification == EMD_TUS_FILTERING_NONE) ? "None" : ToString(filtering_minification)))));
	xmlCurrentNode->SetAttribute("filtering_magnification", ((filtering_magnification & EMD_TUS_FILTERING_LINEAR) ? "Linear" : ((filtering_magnification & EMD_TUS_FILTERING_POINT) ? "Point" : ((filtering_magnification == EMD_TUS_FILTERING_NONE) ? "None" : ToString(filtering_magnification)))));

	xmlCurrentNode->SetAttribute("unknow0", flag0);
	*/

	xmlCurrentNode->SetAttribute("textureindex", texIndex);
	xmlCurrentNode->SetAttribute("textScale_uv", FloatToString(textScale_u) +" "+ FloatToString(textScale_v));
	xmlCurrentNode->SetAttribute("adressMode_uv", string((adressMode_u & EMD_TUS_ADRESSMODE_CLAMP) ? "Clamp" : ((adressMode_u & EMD_TUS_ADRESSMODE_MIRROR) ? "Mirror" : "Wrap")) +" "+ ((adressMode_v & EMD_TUS_ADRESSMODE_CLAMP) ? "Clamp" : ((adressMode_v & EMD_TUS_ADRESSMODE_MIRROR) ? "Mirror" : "Wrap")) );
	xmlCurrentNode->SetAttribute("filtering_min_mag", string((filtering_minification & EMD_TUS_FILTERING_LINEAR) ? "Linear" : ((filtering_minification & EMD_TUS_FILTERING_POINT) ? "Point" : ((filtering_minification == EMD_TUS_FILTERING_NONE) ? "None" : ToString(filtering_minification)))) +" "+ ((filtering_magnification & EMD_TUS_FILTERING_LINEAR) ? "Linear" : ((filtering_magnification & EMD_TUS_FILTERING_POINT) ? "Point" : ((filtering_magnification == EMD_TUS_FILTERING_NONE) ? "None" : ToString(filtering_magnification)))));
	xmlCurrentNode->SetAttribute("unk_0", flag0);

	return xmlCurrentNode;
}














































/*-------------------------------------------------------------------------------\
|                             loadTextureDefXml									 |
\-------------------------------------------------------------------------------*/
bool EMD::loadTextureDefXml(string filename)
{
	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
	{
		printf("Loading xml %s fail. skip.'\n", filename.c_str());
		notifyError();
		return false;
	}

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);
	TiXmlElement* rootNode = hDoc.FirstChildElement("EMD").Element();
	if (!rootNode)
	{
		printf("%s don't have 'EMD' tags. skip.'\n", filename.c_str());
		notifyError();
		return false;
	}

	return importTextureDefXml(rootNode);
}
/*-------------------------------------------------------------------------------\
|                             saveTextureDefXml									 |
\-------------------------------------------------------------------------------*/
void EMD::saveTextureDefXml(string filename)
{
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement* rootNode = exportTextureDefXml();
	doc.LinkEndChild(rootNode);

	doc.SaveFile(filename);
}









/*-------------------------------------------------------------------------------\
|                             importTextureDefXml								 |
\-------------------------------------------------------------------------------*/
bool EMD::importTextureDefXml(TiXmlElement* xmlCurrentNode)
{
	TiXmlElement* modelsNode = xmlCurrentNode->FirstChildElement("EmdModels");
	if (!modelsNode)
	{
		printf("No 'EmdModels' tags find. skip.'\n");
		notifyError();
		return false;
	}

	for (TiXmlElement* xmlNode = modelsNode->FirstChildElement("EmdModel"); xmlNode; xmlNode = xmlNode->NextSiblingElement("EmdModel"))
	{
		EMDModel* emdModel = new EMDModel();
		if (emdModel->importTextureDefXml(xmlNode))
			models.push_back(emdModel);
		else
			delete emdModel;
	}

	return (models.size() != 0);
}
/*-------------------------------------------------------------------------------\
|                             importTextureDefXml								 |
\-------------------------------------------------------------------------------*/
bool EMDModel::importTextureDefXml(TiXmlElement* xmlCurrentNode)
{
	xmlCurrentNode->QueryStringAttribute("name", &name);

	TiXmlElement* meshNode = xmlCurrentNode->FirstChildElement("Meshes");
	if (!meshNode)
	{
		printf("No 'EmdMeshs' tags find. skip.'\n");
		notifyError();
		return false;
	}

	for (TiXmlElement* xmlNode = meshNode->FirstChildElement("Mesh"); xmlNode; xmlNode = xmlNode->NextSiblingElement("Mesh"))
	{
		EMDMesh* emdMesh = new EMDMesh();
		if (emdMesh->importTextureDefXml(xmlNode))
			meshes.push_back(emdMesh);
		else
			delete emdMesh;
	}

	return (meshes.size() != 0);
}
/*-------------------------------------------------------------------------------\
|                             importTextureDefXml								 |
\-------------------------------------------------------------------------------*/
bool EMDMesh::importTextureDefXml(TiXmlElement* xmlCurrentNode)
{
	xmlCurrentNode->QueryStringAttribute("name", &name);

	TiXmlElement* submeshNode = xmlCurrentNode->FirstChildElement("SubMeshes");
	if (!submeshNode)
	{
		printf("No 'EmdSubMeshs' tags find. skip.'\n");
		notifyError();
		return false;
	}

	for (TiXmlElement* xmlNode = submeshNode->FirstChildElement("Submesh"); xmlNode; xmlNode = xmlNode->NextSiblingElement("Submesh"))
	{
		EMDSubmesh* emdSubMesh = new EMDSubmesh();
		if (emdSubMesh->importTextureDefXml(xmlNode))
			submeshes.push_back(emdSubMesh);
		else
			delete emdSubMesh;
	}

	return (submeshes.size() != 0);
}
/*-------------------------------------------------------------------------------\
|                             importTextureDefXml								 |
\-------------------------------------------------------------------------------*/
bool EMDSubmesh::importTextureDefXml(TiXmlElement* xmlCurrentNode)
{
	xmlCurrentNode->QueryStringAttribute("name", &name);
	
	// Vertex Definition
	TiXmlElement* flagsNode = xmlCurrentNode->FirstChildElement("VertexDefinition");
	if (!flagsNode)
	{
		printf("No 'VertexDefinition' tags find. Please take a look at a '.emd.xml' file or document about Emd.'\n");
		notifyError();
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






	TiXmlElement* definitionNode = xmlCurrentNode->FirstChildElement("TextureDefinitions");
	if (!definitionNode)
	{
		printf("No 'TextureDefinitions' tags find. skip.'\n");
		notifyError();
		return false;
	}
	
	for (TiXmlElement* xmlNode = definitionNode->FirstChildElement("TextureUnitState"); xmlNode; xmlNode = xmlNode->NextSiblingElement("TextureUnitState"))
	{
		EMDTextureUnitState emdTextureUnitState;
		if (emdTextureUnitState.importTextureDefXml(xmlNode))
			definitions.push_back(emdTextureUnitState);
	}
	return true;						// it could have 0 vertex, and 0 triangle but textures (ex: TE1_002_Face_base.emd)
}
/*-------------------------------------------------------------------------------\
|                             importTextureDefXml								 |
\-------------------------------------------------------------------------------*/
bool EMDTextureUnitState::importTextureDefXml(TiXmlElement* xmlCurrentNode)
{

	size_t tmp;
	if (xmlCurrentNode->QueryUnsignedAttribute("textureindex", &tmp) == TIXML_SUCCESS)
		texIndex = (unsigned char)tmp;


	string str = "";
	textScale_u = textScale_v = 1;
	xmlCurrentNode->QueryStringAttribute("textScale_uv", &str);
	std::vector<char> listSeparators;
	listSeparators.push_back(' ');
	listSeparators.push_back(',');
	std::vector<string> sv = multiSplit(str, listSeparators);
	size_t inc = 0;
	for (size_t i = 0, nb = sv.size(); i < nb; i++)
	{
		if (sv.at(i).length() == 0)
			continue;

		if (inc == 0)
			textScale_u = StringToFloat(sv.at(i));
		else
			textScale_v = StringToFloat(sv.at(i));

		inc++;
		if (inc == 2)
			break;
	}


	adressMode_u = adressMode_v = EMD_TUS_ADRESSMODE_WRAP;
	xmlCurrentNode->QueryStringAttribute("adressMode_uv", &str);
	sv = multiSplit(str, listSeparators);
	inc = 0;
	for (size_t i = 0, nb = sv.size(); i < nb; i++)
	{
		str = sv.at(i);
		if (str.length() == 0)
			continue;

		if (inc == 0)
			adressMode_u = ((str == "Wrap") ? EMD_TUS_ADRESSMODE_WRAP : ((str == "Mirror") ? EMD_TUS_ADRESSMODE_MIRROR : ((str == "Clamp") ? EMD_TUS_ADRESSMODE_CLAMP : (uint8_t)stoi(str))));
		else
			adressMode_v = ((str == "Wrap") ? EMD_TUS_ADRESSMODE_WRAP : ((str == "Mirror") ? EMD_TUS_ADRESSMODE_MIRROR : ((str == "Clamp") ? EMD_TUS_ADRESSMODE_CLAMP : (uint8_t)stoi(str))));

		inc++;
		if (inc == 2)
			break;
	}


	filtering_minification = filtering_magnification = EMD_TUS_FILTERING_NONE;
	xmlCurrentNode->QueryStringAttribute("filtering_min_mag", &str);
	sv = multiSplit(str, listSeparators);
	inc = 0;
	for (size_t i = 0, nb = sv.size(); i < nb; i++)
	{
		str = sv.at(i);
		if (str.length() == 0)
			continue;

		if (inc == 0)
			filtering_minification = ((str == "None") ? EMD_TUS_FILTERING_NONE : ((str == "Point") ? EMD_TUS_FILTERING_POINT : ((str == "Linear") ? EMD_TUS_FILTERING_LINEAR : (uint8_t)stoi(str))));
		else
			filtering_magnification = ((str == "None") ? EMD_TUS_FILTERING_NONE : ((str == "Point") ? EMD_TUS_FILTERING_POINT : ((str == "Linear") ? EMD_TUS_FILTERING_LINEAR : (uint8_t)stoi(str))));

		inc++;
		if (inc == 2)
			break;
	}

	if (xmlCurrentNode->QueryUnsignedAttribute("unknow0", &tmp) == TIXML_SUCCESS)
		flag0 = (unsigned char)tmp;

	return true;
}



















/*-------------------------------------------------------------------------------\
|                             exportTextureDefXml								 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMD::exportTextureDefXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("EMD");

	TiXmlElement* modelsNode = new TiXmlElement("EmdModels");
	size_t nbmodel = models.size();
	for (size_t i = 0; i < nbmodel; i++)
		modelsNode->LinkEndChild(models.at(i)->exportTextureDefXml());

	xmlCurrentNode->LinkEndChild(modelsNode);
	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportTextureDefXml								 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMDModel::exportTextureDefXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("EmdModel");

	xmlCurrentNode->SetAttribute("name", name);
	
	TiXmlElement* meshNode = new TiXmlElement("Meshes");
	size_t nbMesh = meshes.size();
	for (size_t i = 0; i < nbMesh; i++)
		meshNode->LinkEndChild(meshes.at(i)->exportTextureDefXml());

	xmlCurrentNode->LinkEndChild(meshNode);
	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportTextureDefXml								 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMDMesh::exportTextureDefXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("Mesh");

	xmlCurrentNode->SetAttribute("name", name);
	
	// submesh
	TiXmlElement* submeshNode = new TiXmlElement("SubMeshes");
	size_t nbsubMesh = submeshes.size();
	for (size_t i = 0; i < nbsubMesh; i++)
		submeshNode->LinkEndChild(submeshes.at(i)->exportTextureDefXml());

	xmlCurrentNode->LinkEndChild(submeshNode);
	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportTextureDefXml								 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMDSubmesh::exportTextureDefXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("Submesh");

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




	TiXmlElement* definitionNode = new TiXmlElement("TextureDefinitions");
	size_t nbDef = definitions.size();
	for (size_t i = 0; i < nbDef; i++)
		definitionNode->LinkEndChild(definitions.at(i).exportTextureDefXml());
	xmlCurrentNode->LinkEndChild(definitionNode);

	return xmlCurrentNode;
}
/*-------------------------------------------------------------------------------\
|                             exportTextureDefXml								 |
\-------------------------------------------------------------------------------*/
TiXmlElement* EMDTextureUnitState::exportTextureDefXml(void)
{
	TiXmlElement* xmlCurrentNode = new TiXmlElement("TextureUnitState");

	xmlCurrentNode->SetAttribute("textureindex", texIndex);
	xmlCurrentNode->SetAttribute("textScale_uv", FloatToString(textScale_u) + " " + FloatToString(textScale_v));
	xmlCurrentNode->SetAttribute("adressMode_uv", string((adressMode_u & EMD_TUS_ADRESSMODE_CLAMP) ? "Clamp" : ((adressMode_u & EMD_TUS_ADRESSMODE_MIRROR) ? "Mirror" : "Wrap")) + " " + ((adressMode_v & EMD_TUS_ADRESSMODE_CLAMP) ? "Clamp" : ((adressMode_v & EMD_TUS_ADRESSMODE_MIRROR) ? "Mirror" : "Wrap")));
	xmlCurrentNode->SetAttribute("filtering_min_mag", string((filtering_minification & EMD_TUS_FILTERING_LINEAR) ? "Linear" : ((filtering_minification & EMD_TUS_FILTERING_POINT) ? "Point" : ((filtering_minification == EMD_TUS_FILTERING_NONE) ? "None" : ToString(filtering_minification)))) + " " + ((filtering_magnification & EMD_TUS_FILTERING_LINEAR) ? "Linear" : ((filtering_magnification & EMD_TUS_FILTERING_POINT) ? "Point" : ((filtering_magnification == EMD_TUS_FILTERING_NONE) ? "None" : ToString(filtering_magnification)))));
	xmlCurrentNode->SetAttribute("unknow0", flag0);

	return xmlCurrentNode;
}















}