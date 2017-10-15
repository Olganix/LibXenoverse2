namespace LibXenoverse
{


#ifdef LIBXENOVERSE_FBX_SUPPORT


/*-------------------------------------------------------------------------------\
|                             importFBX											 |
\-------------------------------------------------------------------------------*/
void EMD::importFBX(FbxNode* fbxNode, bool compressedFlag)
{
	if (!fbxNode)
		return;

	if (fbxNode->GetMesh())					//oldVersion, to deal with no emd hierarchy in fbx.
	{
		EMDModel* model = new EMDModel();
		model->importFBX(fbxNode, compressedFlag);
		models.push_back(model);
	}

	size_t nbChild = fbxNode->GetChildCount();
	for (size_t i = 0; i < nbChild; i++)
	{
		EMDModel* model = new EMDModel();
		model->importFBX(fbxNode->GetChild(i), compressedFlag);
		models.push_back(model);
	}
}
/*-------------------------------------------------------------------------------\
|                             importFBX											 |
\-------------------------------------------------------------------------------*/
void EMDModel::importFBX(FbxNode* fbxNode, bool compressedFlag)
{
	if (!fbxNode)
		return;
	name = ToString(fbxNode->GetName());

	if (fbxNode->GetMesh())							//old version, to deal with no emd hierarchy in fbx.
	{
		EMDMesh *mesh = new EMDMesh();
		mesh->importFBX(fbxNode, compressedFlag);
		meshes.push_back(mesh);
	}

	size_t nbChild = fbxNode->GetChildCount();
	for (size_t i = 0; i < nbChild; i++)
	{
		EMDMesh *mesh = new EMDMesh();
		mesh->importFBX(fbxNode->GetChild(i), compressedFlag);
		meshes.push_back(mesh);
	}
}
/*-------------------------------------------------------------------------------\
|                             importFBX											 |
\-------------------------------------------------------------------------------*/
void EMDMesh::importFBX(FbxNode* fbxNode, bool compressedFlag)
{
	if (!fbxNode)
		return;
	name = ToString(fbxNode->GetName());


	if (fbxNode->GetMesh())
		importFBX_OldMethode(fbxNode, compressedFlag);
	

	size_t nbChild = fbxNode->GetChildCount();
	for (size_t i = 0; i < nbChild; i++)
	{
		EMDSubmesh* subMesh = new EMDSubmesh();
		subMesh->importFBX(fbxNode->GetChild(i), compressedFlag);
		submeshes.push_back(subMesh);

		//merge boundingBox
		if (subMesh->aabb_min_x < aabb_min_x) aabb_min_x = subMesh->aabb_min_x;
		if (subMesh->aabb_min_y < aabb_min_y) aabb_min_y = subMesh->aabb_min_y;
		if (subMesh->aabb_min_z < aabb_min_z) aabb_min_z = subMesh->aabb_min_z;
		if (subMesh->aabb_min_w < aabb_min_w) aabb_min_w = subMesh->aabb_min_w;
		if (subMesh->aabb_max_x > aabb_max_x) aabb_max_x = subMesh->aabb_max_x;
		if (subMesh->aabb_max_y > aabb_max_y) aabb_max_y = subMesh->aabb_max_y;
		if (subMesh->aabb_max_z > aabb_max_z) aabb_max_z = subMesh->aabb_max_z;
		if (subMesh->aabb_max_w > aabb_max_w) aabb_max_w = subMesh->aabb_max_w;
	}
	aabb_center_x = (aabb_max_x + aabb_min_x) / 2.0f;
	aabb_center_y = (aabb_max_y + aabb_min_y) / 2.0f;
	aabb_center_z = (aabb_max_z + aabb_min_z) / 2.0f;
	aabb_center_w = (aabb_max_w + aabb_min_w) / 2.0f;
}
/*-------------------------------------------------------------------------------\
|                             importFBX_OldMethode								 |
\-------------------------------------------------------------------------------*/
void EMDMesh::importFBX_OldMethode(FbxNode* fbxNode, bool compressedFlag)
{
	if ((!fbxNode) || (!fbxNode->GetMesh()))
		return;

	name = ToString(fbxNode->GetName());
	
	
	


	FbxMesh* fbxMesh = fbxNode->GetMesh();
	int control_points_count = fbxMesh->GetControlPointsCount();				//number of vertices.
	FbxAMatrix transform_matrix = fbxNode->EvaluateGlobalTransform();




	// Get All bone blend indice (vertex Index for now) and blend weights.
	vector<vector<pair<double, FbxNode *>>> control_points_skin_bindings;
	control_points_skin_bindings.resize(control_points_count);

	FbxSkin* lSkin;
	FbxCluster* fbxCluster;
	int lClusterCount, lIndexCount;
	int* lIndices;
	double* lWeights;
	pair<double, FbxNode *> lSkinLink;
	bool added;
	int lSkinCount = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int i = 0; i < lSkinCount; i++)
	{
		lSkin = (FbxSkin*)fbxMesh->GetDeformer(i, FbxDeformer::eSkin);

		// Iterate through the clusters of the skin deformer
		lClusterCount = lSkin->GetClusterCount();
		for(int j = 0; j != lClusterCount; j++)
		{
			fbxCluster = lSkin->GetCluster(j);

			lIndexCount = fbxCluster->GetControlPointIndicesCount();
			lIndices = fbxCluster->GetControlPointIndices();
			lWeights = fbxCluster->GetControlPointWeights();

			for (int k = 0; k < lIndexCount; k++)
			{
				int lControlIndex = lIndices[k];
				if ((lControlIndex < control_points_count) && (fbxCluster->GetLink()))
				{
					lSkinLink.first = lWeights[k];
					lSkinLink.second = fbxCluster->GetLink();

					added = false;
					vector<pair<double, FbxNode *>> &skin_bindings = control_points_skin_bindings.at(lControlIndex);
					for (size_t w = 0; w < skin_bindings.size(); w++)
					{
						if (skin_bindings.at(w).first < lSkinLink.first)
						{
							skin_bindings.insert(skin_bindings.begin() + w, lSkinLink);
							added = true;
							break;
						}
					}
					if (!added)
						skin_bindings.push_back(lSkinLink);
				}
			}
		}
	}

	
	
	
	//for all materials , we have a submesh
	aabb_min_x = aabb_min_y = aabb_min_z = LIBXENOVERSE_AABB_MAX_START;
	aabb_max_x = aabb_max_y = aabb_max_z = LIBXENOVERSE_AABB_MIN_END;

	int material_count = fbxNode->GetMaterialCount();
	for (int m = 0; m < material_count; m++)
	{
		EMDSubmesh* subMesh = new EMDSubmesh();
		if (!subMesh->importFBX_OldMethode(fbxNode, fbxMesh, m, transform_matrix, string(fbxNode->GetMaterial(m)->GetName()), control_points_skin_bindings, compressedFlag))
		{
			delete subMesh;
			continue;
		}

		submeshes.push_back(subMesh);


		// After Submeshes are imported, calculate new AABB
		if (subMesh->aabb_min_x < aabb_min_x) aabb_min_x = subMesh->aabb_min_x;
		if (subMesh->aabb_min_y < aabb_min_y) aabb_min_y = subMesh->aabb_min_y;
		if (subMesh->aabb_min_z < aabb_min_z) aabb_min_z = subMesh->aabb_min_z;
		if (subMesh->aabb_min_w < aabb_min_w) aabb_min_w = subMesh->aabb_min_w;
		if (subMesh->aabb_max_x > aabb_max_x) aabb_max_x = subMesh->aabb_max_x;
		if (subMesh->aabb_max_y > aabb_max_y) aabb_max_y = subMesh->aabb_max_y;
		if (subMesh->aabb_max_z > aabb_max_z) aabb_max_z = subMesh->aabb_max_z;
		if (subMesh->aabb_max_w > aabb_max_w) aabb_max_w = subMesh->aabb_max_w;
	}
	aabb_center_x = (aabb_max_x + aabb_min_x) / 2.0f;
	aabb_center_y = (aabb_max_y + aabb_min_y) / 2.0f;
	aabb_center_z = (aabb_max_z + aabb_min_z) / 2.0f;
	aabb_center_w = (aabb_max_w + aabb_min_w) / 2.0f;
}
/*-------------------------------------------------------------------------------\
|                             importFBX_OldMethode								 |
\-------------------------------------------------------------------------------*/
bool EMDSubmesh::importFBX_OldMethode(FbxNode* fbxNode, FbxMesh *fbxMesh, int material_index, FbxAMatrix transform_matrix, string material_names, vector<vector<pair<double, FbxNode *>>> &control_points_skin_bindings, bool compressedFlag)
{
	name = material_names;


	FbxVector4 nodeScale = fbxNode->LclScaling.Get();
	if (nodeScale[0] == 0.0) nodeScale[0] = 1.0f;
	if (nodeScale[1] == 0.0) nodeScale[1] = 1.0f;
	if (nodeScale[2] == 0.0) nodeScale[2] = 1.0f;




	// Scan FBX Mesh for vertices on the current material index
	FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
	size_t lPolygonCount = fbxMesh->GetPolygonCount();
	if (fbxNode->GetMaterialCount() != 1)			//if we have only 1 or none material, no need of mapping for material index on each vertex. Else :
	{
		FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;
		
		lMaterialIndice = &fbxMesh->GetElementMaterial()->GetIndexArray();	//if we have a layer of material for the index (normally is true)
		if (!lMaterialIndice)
		{
			printf("Mesh has no material index element. skip.\n");
			return false;
		}
			
		lMaterialMappingMode = fbxMesh->GetElementMaterial()->GetMappingMode();
		if (lMaterialMappingMode != FbxGeometryElement::eByPolygon)						// the material mpping index must be on polygone mode
		{
			printf("material index element of Mesh isn't on eByPolygon. skip.\n");
			return false;
		}
		
		if(lMaterialIndice->GetCount() != lPolygonCount)					//and on all polygones
		{
			printf("material index element of Mesh isn't on all polygone. skip.\n");
			return false;
		}
	}







	//search for Mesh Vertex definition, and get list of values.
	uint16_t flags = EMD_VTX_FLAG_POS;
	FbxVector4* listPositions = fbxMesh->GetControlPoints();
	size_t nbVertex = fbxMesh->GetControlPointsCount();

	FbxArray< FbxVector4 > listNormals;
	if (fbxMesh->GetPolygonVertexNormals(listNormals))
		flags = flags | EMD_VTX_FLAG_NORM;
	size_t nbNormals = listNormals.GetCount();


	FbxLayerElementArrayTemplate< FbxVector4 >* listTangentes = 0;
	FbxGeometryElementTangent* geoElemTang = fbxMesh->GetElementTangent(0);		//fbx miss the equivalente fonction of GetPolygonVertexNormals. so use own.
	size_t nbTangentes = 0;
	if (geoElemTang)
	{
		flags = flags | EMD_VTX_FLAG_TANGENT;
		listTangentes = &geoElemTang->GetDirectArray();
		nbTangentes = listTangentes->GetCount();
	}

	

	FbxStringList uv_sets;
	fbxMesh->GetUVSetNames(uv_sets);											// get the number and the names of Uv layers
	size_t nbUvs = min(uv_sets.GetCount(), 2);	
	std::vector<FbxArray< FbxVector2 >> listUVs;
	listUVs.resize(nbUvs);

	for (size_t i = 0; i < nbUvs; i++)
	{
		if (fbxMesh->GetPolygonVertexUVs(uv_sets[i], listUVs.at(i)))
		{
			if(i==0)
				flags = flags | EMD_VTX_FLAG_TEX;
			else
				flags = flags | EMD_VTX_FLAG_TEX2;
		}
	}

	FbxLayerElementArrayTemplate< FbxColor >* listVertexColors = 0;
	FbxGeometryElementVertexColor* geoElemColor = fbxMesh->GetElementVertexColor(0);		//fbx miss the equivalente fonction of GetPolygonVertexNormals. so use own.
	size_t nbVertexColor = 0;
	if (geoElemColor)
	{
		flags = flags | EMD_VTX_FLAG_COLOR;
		listVertexColors = &geoElemColor->GetDirectArray();
		nbVertexColor = listVertexColors->GetCount();
	}
	
	

	if (control_points_skin_bindings.size())
	{
		flags = flags | EMD_VTX_FLAG_BLEND_WEIGHT;
	}

	if(compressedFlag)
		flags = flags | EMD_VTX_FLAG_COMPRESSED_FORMAT;

	vertex_type_flag = flags;
	vertex_size = EMDVertex::getSizeFromFlags(vertex_type_flag);






	// Create Vertices
	AABB vertex_aabb;
	FbxVector4 position, normal, tangent;
	FbxVector2 uv;
	FbxColor color;

	FbxAMatrix rotation_matrix = transform_matrix;
	rotation_matrix.SetT(FbxVector4(0.0, 0.0, 0.0, 0.0));
	rotation_matrix.SetS(FbxVector4(1.0, 1.0, 1.0, 1.0));

	vertices.clear();
	vector<EMDVertex> vertices_tmp;

	for (size_t i = 0; i < nbVertex; i++)
	{
		EMDVertex v;
		v.flags = flags;

		position = transform_matrix.MultT(listPositions[i]);
		v.pos_x = (float)(position[0] / nodeScale[0]);				// because of apply the hierarchy. may be we have to consider positoin rotation, and also in absolute.
		v.pos_y = (float)(position[1] / nodeScale[1]);
		v.pos_z = (float)(position[2] / nodeScale[2]);


		if ((nbNormals == nbVertex) && (i < nbNormals))				//per-vertex normals
		{
			normal = rotation_matrix.MultT(listNormals[i]);
			v.norm_x = (float)normal[0];
			v.norm_y = (float)normal[1];
			v.norm_z = (float)normal[2];
		}

		if ((nbTangentes == nbVertex) && (i < nbTangentes))				//per-vertex tangentes
		{
			tangent = rotation_matrix.MultT(listTangentes->GetAt(i));
			v.tang_x = (float)tangent[0];
			v.tang_y = (float)tangent[1];
			v.tang_z = (float)tangent[2];
		}
		


		for (size_t j=0; j < nbUvs; j++)
		{
			size_t nbVertexUv = listUVs.at(j).GetCount();
			
			if ((nbVertexUv == nbVertex)&&(i < nbVertexUv))						//per-vertex uv
			{
				uv = listUVs.at(j)[i];
				if (j == 0)
				{
					v.text_u = (float)uv[0];
					v.text_v = 1.0f - (float)uv[1];
				}else{
					v.text2_u = (float)uv[0];
					v.text2_v = 1.0f - (float)uv[1];
				}
			}
		}



		if ((nbVertexColor == nbVertex) && (i < nbVertexColor))						//per-vertex colors
		{
			color = listVertexColors->GetAt(i);
			v.setColorFromRGBAFloat((float)color.mRed, (float)color.mGreen, (float)color.mBlue, (float)color.mAlpha);
		}

		vertices_tmp.push_back(v);
		vertex_aabb.addPoint(v.pos_x, v.pos_y, v.pos_z);
	}


	aabb_center_x = vertex_aabb.centerX();
	aabb_center_y = vertex_aabb.centerY();
	aabb_center_z = vertex_aabb.centerZ();
	//aabb_center_w = vertex_aabb.sizeX();			// FIXME: The Ws need to be figured out to know what they actually are
	aabb_center_w = 1.0f;
	aabb_min_x = vertex_aabb.start_x;
	aabb_min_y = vertex_aabb.start_y;
	aabb_min_z = vertex_aabb.start_z;
	//aabb_min_w = vertex_aabb.sizeY();				// FIXME: The Ws need to be figured out to know what they actually are
	aabb_min_w = 1.0f;
	aabb_max_x = vertex_aabb.end_x;
	aabb_max_y = vertex_aabb.end_y;
	aabb_max_z = vertex_aabb.end_z;
	//aabb_max_w = vertex_aabb.sizeZ();				// FIXME: The Ws need to be figured out to know what they actually are
	aabb_max_w = 1.0f;
	

	





	// Create Triangles : it's a serie of polygone/faces, with the same Bones influences.
	EMDTriangles* triangle_list_ptr;
	vector<string> polygon_bone_names;
	size_t nbSkinBind;
	string bone_name;
	size_t index_tmp;
	size_t nbVertexWithFaceDef = fbxMesh->GetPolygonVertexCount();
	size_t offsetVertex = 0;

	for (size_t i = 0; i < lPolygonCount; ++i)
	{
		if (((lMaterialIndice) && (lMaterialIndice->GetAt(i) != material_index)) || (fbxMesh->GetPolygonSize(i) != 3))	//the oldmethode oblige us to split fbxmesh by material, and keep only the current material
		{
			offsetVertex += fbxMesh->GetPolygonSize(i);
			continue;												//only keep polygone of material (or all polygone if there only one or none material)
		}
		

		polygon_bone_names.clear();
		if (flags & EMD_VTX_FLAG_BLEND_WEIGHT)
		{

			//get List of Bones Names
			
			for (int j = 0; j < 3; j++)
			{
				index_tmp = fbxMesh->GetPolygonVertex(i, j);

				vector<pair<double, FbxNode *>> &skin_bindings = control_points_skin_bindings.at(index_tmp);

				nbSkinBind = skin_bindings.size();
				for (size_t k = 0; k < nbSkinBind; k++)
				{
					if (k >= 4)					//limit to 4 influences of bones on vertex.
						continue;

					bone_name = skin_bindings.at(k).second->GetName();

					bool isfound = false;
					size_t nbElement = polygon_bone_names.size();
					for (size_t m = 0; m < nbElement; m++)
					{
						if (polygon_bone_names.at(m) == bone_name)
						{
							isfound = true;
							break;
						}
					}
					if (!isfound)
						polygon_bone_names.push_back(bone_name);
				}
			}
		}


		triangle_list_ptr = getTriangleForBoneNameCombinaison(polygon_bone_names);					// Detect appropriate triangle for these 3 vertices
		polygon_bone_names = triangle_list_ptr->bone_names;
		size_t nbBoneNames = polygon_bone_names.size();
		
		

		for (int j = 0; j < 3; j++)
		{
			index_tmp = fbxMesh->GetPolygonVertex(i, j);
			if (index_tmp >= nbVertex)
			{
				printf("error: vertex index out of list. use 0.\n");
				index_tmp = 0;
			}
				

			EMDVertex &v = vertices_tmp.at(index_tmp);
			EMDVertex v_copy = EMDVertex(v);


			//here, we hae to deal with per-face information, duplication of vertex, and so , new indexes
			if ((nbNormals == nbVertexWithFaceDef) && (offsetVertex + j < nbNormals))				//per-face normals
			{
				normal = rotation_matrix.MultT(listNormals[offsetVertex + j]);
				v_copy.norm_x = (float)normal[0];
				v_copy.norm_y = (float)normal[1];
				v_copy.norm_z = (float)normal[2];
			}

			if ((nbTangentes == nbVertexWithFaceDef) && (offsetVertex + j < nbTangentes))				//per-face tangentes
			{
				tangent = rotation_matrix.MultT(listTangentes->GetAt(offsetVertex + j));
				v_copy.tang_x = (float)tangent[0];
				v_copy.tang_y = (float)tangent[1];
				v_copy.tang_z = (float)tangent[2];
			}

			for (size_t k = 0; k < nbUvs; k++)
			{
				size_t nbVertexUv = listUVs.at(k).GetCount();

				if ((nbVertexUv == nbVertexWithFaceDef) && (offsetVertex + j < nbVertexUv))						//per-face uv
				{
					uv = listUVs.at(k)[offsetVertex + j];
					if (k == 0)
					{
						v_copy.text_u = (float)uv[0];
						v_copy.text_v = 1.0f - (float)uv[1];
					}else {
						v_copy.text2_u = (float)uv[0];
						v_copy.text2_v = 1.0f - (float)uv[1];
					}
				}
			}

			if ((nbVertexColor == nbVertexWithFaceDef) && (offsetVertex + j < nbVertexColor))						//per-face colors
			{
				color = listVertexColors->GetAt(offsetVertex + j);
				v_copy.setColorFromRGBAFloat((float)color.mRed, (float)color.mGreen, (float)color.mBlue, (float)color.mAlpha);
			}




			// apply bone influence on vertex
			if (flags & EMD_VTX_FLAG_BLEND_WEIGHT)
			{
				vector<pair<double, FbxNode *>> &skin_bindings = control_points_skin_bindings.at(index_tmp);
				for (size_t k = 0; k < skin_bindings.size(); k++)
				{
					bone_name = skin_bindings.at(k).second->GetName();

					size_t isFound = (size_t)-1;
					for (size_t m = 0; m < nbBoneNames; m++)
					{
						if (polygon_bone_names.at(m) == bone_name)
						{
							isFound = m;
							break;
						}
					}

					if (isFound != (size_t)-1)
					{
						v_copy.blend[3 - k] = isFound;
						v_copy.blend_weight[k] = (float)skin_bindings.at(k).first;
					}
				}
			}
			
			size_t newIndex = mergeVertex(v_copy);							//try to find a similar vertex, or add into the list (could be long for bug meshes, but reduce the number of duplicate vertex , and resolve the per-face informations)

			triangle_list_ptr->faces.push_back(newIndex);						//fill faces
		}
		
		offsetVertex += 3;
	}




	// Create Definitions (TextureUnit)
	EMDSubmeshDefinition definition;
	
	// Add 2 of these
	definitions.push_back(definition);								//notice, the old case is hard coded.
	definitions.push_back(definition);

	return true;
}
/*-------------------------------------------------------------------------------\
|                             importFBX											 |
\-------------------------------------------------------------------------------*/
void EMDSubmesh::importFBX(FbxNode* fbxNode, bool compressedFlag)
{
	if ((!fbxNode) || (!fbxNode->GetMesh()))
		return;

	name = ToString(fbxNode->GetName());


	FbxVector4 nodeScale = fbxNode->LclScaling.Get();
	if (nodeScale[0] == 0.0) nodeScale[0] = 1.0f;
	if (nodeScale[1] == 0.0) nodeScale[1] = 1.0f;
	if (nodeScale[2] == 0.0) nodeScale[2] = 1.0f;

	


	FbxMesh* fbxMesh = fbxNode->GetMesh();
	if (fbxNode->GetMaterialCount() != 1)			//if we have only 1 or none material, no need of mapping for material index on each vertex. Else :
		printf("Warning Mesh %s have many materialIndex (not good on EmdSubmesh definition). We considere as only one material %s.\n", name.c_str(), name.c_str());




	//search for Mesh Vertex definition, and get list of values.
	uint16_t flags = EMD_VTX_FLAG_POS;
	FbxVector4* listPositions = fbxMesh->GetControlPoints();
	size_t nbVertex = fbxMesh->GetControlPointsCount();




	FbxLayerElementArrayTemplate< FbxVector4 >* listNormals = 0;
	FbxGeometryElementNormal* geoElemNorm = fbxMesh->GetElementNormal(0);		//fbx miss the equivalente fonction of GetPolygonVertexNormals. so use own.
	size_t nbNormals = 0;
	if (geoElemNorm)
	{
		flags = flags | EMD_VTX_FLAG_NORM;
		if (geoElemNorm->GetReferenceMode() == FbxLayerElement::eIndexToDirect)				// take care of indirect index array.
		{
			FbxLayerElementArrayTemplate<int> &listIndex = geoElemNorm->GetIndexArray();
			FbxLayerElementArrayTemplate< FbxVector4 > &listNorms_base = geoElemNorm->GetDirectArray();

			listNormals = new FbxLayerElementArrayTemplate<FbxVector4>(eFbxDouble4);

			size_t nbElements = listIndex.GetCount();
			listNormals->Resize(nbElements);

			size_t nbValues = listNorms_base.GetCount();
			for (size_t j = 0; j < nbElements; j++)
			{
				size_t index = listIndex.GetAt(j);
				if (index < nbValues)
				{
					listNormals->SetAt(j, listNorms_base.GetAt(index));
				}
			}

		}else {
			listNormals = &geoElemNorm->GetDirectArray();
		}
		nbNormals = listNormals->GetCount();
	}

	

	FbxLayerElementArrayTemplate< FbxVector4 >* listTangentes = 0;
	FbxGeometryElementTangent* geoElemTang = fbxMesh->GetElementTangent(0);		//fbx miss the equivalente fonction of GetPolygonVertexNormals. so use own.
	size_t nbTangentes = 0;
	if (geoElemTang)
	{
		flags = flags | EMD_VTX_FLAG_TANGENT;
		if (geoElemTang->GetReferenceMode() == FbxLayerElement::eIndexToDirect)				// take care of indirect index array.
		{
			FbxLayerElementArrayTemplate<int> &listIndex = geoElemTang->GetIndexArray();
			FbxLayerElementArrayTemplate< FbxVector4 > &listTangs_base = geoElemTang->GetDirectArray();

			listTangentes = new FbxLayerElementArrayTemplate<FbxVector4>(eFbxDouble4);

			size_t nbElements = listIndex.GetCount();
			listTangentes->Resize(nbElements);

			size_t nbValues = listTangs_base.GetCount();
			for (size_t j = 0; j < nbElements; j++)
			{
				size_t index = listIndex.GetAt(j);
				if (index < nbValues)
				{
					listTangentes->SetAt(j, listTangs_base.GetAt(index));
				}
			}
		}
		else {
			listTangentes = &geoElemTang->GetDirectArray();
		}
		nbTangentes = listTangentes->GetCount();
	}



	FbxStringList uv_sets;
	fbxMesh->GetUVSetNames(uv_sets);											// get the number and the names of Uv layers
	size_t nbUvs = min(uv_sets.GetCount(), 2);
	std::vector<FbxLayerElementArrayTemplate< FbxVector2 >*> listUVs;

	for (size_t i = 0; i < nbUvs; i++)
	{
		FbxGeometryElementUV* geoElemUV = fbxMesh->GetElementUV(uv_sets[i]);
		

		if (geoElemUV->GetReferenceMode() == FbxLayerElement::eIndexToDirect)				//todo: deal with this case on others flags.
		{
			FbxLayerElementArrayTemplate<int> &listIndex = geoElemUV->GetIndexArray();
			FbxLayerElementArrayTemplate< FbxVector2 > &listUVs_base = geoElemUV->GetDirectArray();

			FbxLayerElementArrayTemplate<FbxVector2>* uv_list = new FbxLayerElementArrayTemplate<FbxVector2>(eFbxDouble2);

			size_t nbElements = listIndex.GetCount();
			uv_list->Resize(nbElements);

			size_t nbValues = listUVs_base.GetCount();
			for (size_t j = 0; j < nbElements; j++)
			{
				size_t index = listIndex.GetAt(j);
				if (index < nbValues)
				{
					uv_list->SetAt(j, listUVs_base.GetAt(index));
				}
			}

			listUVs.push_back(uv_list);
		}else {
			listUVs.push_back(&geoElemUV->GetDirectArray());
		}

		if (i == 0)
			flags = flags | EMD_VTX_FLAG_TEX;
		else
			flags = flags | EMD_VTX_FLAG_TEX2;
	}

	FbxLayerElementArrayTemplate< FbxColor >* listVertexColors = 0;
	FbxGeometryElementVertexColor* geoElemColor = fbxMesh->GetElementVertexColor(0);		//fbx miss the equivalente fonction of GetPolygonVertexNormals. so use own.
	size_t nbVertexColor = 0;
	if (geoElemColor)
	{
		flags = flags | EMD_VTX_FLAG_COLOR;
		
		if (geoElemColor->GetReferenceMode() == FbxLayerElement::eIndexToDirect)				// take care of indirect index array.
		{
			FbxLayerElementArrayTemplate<int> &listIndex = geoElemColor->GetIndexArray();
			FbxLayerElementArrayTemplate< FbxColor > &listColors_base = geoElemColor->GetDirectArray();

			listVertexColors = new FbxLayerElementArrayTemplate<FbxColor>(eFbxDouble4);

			size_t nbElements = listIndex.GetCount();
			listVertexColors->Resize(nbElements);

			size_t nbValues = listColors_base.GetCount();
			for (size_t j = 0; j < nbElements; j++)
			{
				size_t index = listIndex.GetAt(j);
				if (index < nbValues)
					listVertexColors->SetAt(j, listColors_base.GetAt(index));
			}

		}else{
			listVertexColors = &geoElemColor->GetDirectArray();
		}
		nbVertexColor = listVertexColors->GetCount();
	}

	


	vector<vector<pair<double, FbxNode *>>> control_points_skin_bindings;
	size_t lPolygonCount = fbxMesh->GetPolygonCount();
	control_points_skin_bindings.resize(nbVertex);



	if (fbxMesh->GetDeformerCount(FbxDeformer::eSkin) != 0)
	{
		flags = flags | EMD_VTX_FLAG_BLEND_WEIGHT;

		// Get All bone blend indice (vertex Index for now) and blend weights.
		FbxSkin* lSkin;
		FbxCluster* fbxCluster;
		int lClusterCount, lIndexCount;
		int* lIndices;
		double* lWeights;
		bool added;
		int lSkinCount = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);
		for (int i = 0; i < lSkinCount; i++)
		{
			lSkin = (FbxSkin*)fbxMesh->GetDeformer(i, FbxDeformer::eSkin);

			lClusterCount = lSkin->GetClusterCount();
			for (int j = 0; j < lClusterCount; j++)			// Iterate through the clusters of the skin deformer
			{
				fbxCluster = lSkin->GetCluster(j);
				if (!fbxCluster->GetLink())
					continue;

				
				lIndices = fbxCluster->GetControlPointIndices();
				lWeights = fbxCluster->GetControlPointWeights();

				lIndexCount = fbxCluster->GetControlPointIndicesCount();
				for (int k = 0; k < lIndexCount; k++)
				{
					size_t lControlIndex = lIndices[k];
					if (lControlIndex >= nbVertex)
						continue;

					pair<double, FbxNode *> lSkinLink;
					lSkinLink.first = lWeights[k];
					lSkinLink.second = fbxCluster->GetLink();

					added = false;
					vector<pair<double, FbxNode *>> &skin_bindings = control_points_skin_bindings.at(lControlIndex);

					//avoid double 
					string clusterName = fbxCluster->GetName();
					bool isfound = false;
					size_t nbSkinBind = skin_bindings.size();
					for (size_t m = 0; m < nbSkinBind; m++)
					{
						if (skin_bindings.at(m).second->GetName() == clusterName)
						{
							isfound = true;
							break;
						}
					}
					if (isfound)
						continue;

					for (size_t m = 0; m < nbSkinBind; m++)
					{
						if (skin_bindings.at(m).first < lSkinLink.first)
						{
							skin_bindings.insert(skin_bindings.begin() + m, lSkinLink);						//we order them by the bigger weight first.
							added = true;
							break;
						}
					}
					if (!added)
						skin_bindings.push_back(lSkinLink);
				}
			}
		}
	}

	if(compressedFlag)
		flags = flags | EMD_VTX_FLAG_COMPRESSED_FORMAT;

	vertex_type_flag = flags;
	vertex_size = EMDVertex::getSizeFromFlags(vertex_type_flag);






	





	// Create Vertices
	AABB vertex_aabb;
	FbxVector4 position, normal, tangent;
	FbxVector2 uv;
	FbxColor color;

	FbxAMatrix transform_matrix = fbxNode->EvaluateGlobalTransform();
	FbxAMatrix rotation_matrix = transform_matrix;
	rotation_matrix.SetT(FbxVector4(0.0, 0.0, 0.0, 0.0));
	rotation_matrix.SetS(FbxVector4(1.0, 1.0, 1.0, 1.0));

	vertices.clear();
	vector<EMDVertex> vertices_tmp;

	for (size_t i = 0; i < nbVertex; i++)
	{
		EMDVertex v;
		v.flags = flags;

		position = transform_matrix.MultT(listPositions[i]);
		v.pos_x = (float)(position[0] / nodeScale[0]);				// because of apply the hierarchy. may be we have to consider positoin rotation, and also in absolute.
		v.pos_y = (float)(position[1] / nodeScale[1]);
		v.pos_z = (float)(position[2] / nodeScale[2]);

		if ((nbNormals == nbVertex) && (i < nbNormals))				//per-vertex normals
		{
			normal = rotation_matrix.MultT(listNormals->GetAt(i));
			v.norm_x = (float)normal[0];
			v.norm_y = (float)normal[1];
			v.norm_z = (float)normal[2];
		}

		if ((nbTangentes == nbVertex) && (i < nbTangentes))				//per-vertex tangentes
		{
			tangent = rotation_matrix.MultT(listTangentes->GetAt(i));
			v.tang_x = (float)tangent[0];
			v.tang_y = (float)tangent[1];
			v.tang_z = (float)tangent[2];
		}



		for (size_t j = 0; j < nbUvs; j++)
		{
			size_t nbVertexUv = listUVs.at(j)->GetCount();

			if ((nbVertexUv == nbVertex) && (i < nbVertexUv))						//per-vertex uv
			{
				uv = listUVs.at(j)->GetAt(i);
				if (j == 0)
				{
					v.text_u = (float)uv[0];
					v.text_v = 1.0f - (float)uv[1];
				}else {
					v.text2_u = (float)uv[0];
					v.text2_v = 1.0f - (float)uv[1];
				}
			}
		}



		if ((nbVertexColor == nbVertex) && (i < nbVertexColor))						//per-vertex colors
		{
			color = listVertexColors->GetAt(i);
			v.setColorFromRGBAFloat((float)color.mRed, (float)color.mGreen, (float)color.mBlue, (float)color.mAlpha);
		}

		vertices_tmp.push_back(v);
		vertex_aabb.addPoint(v.pos_x, v.pos_y, v.pos_z);
	}

	aabb_center_x = vertex_aabb.centerX();
	aabb_center_y = vertex_aabb.centerY();
	aabb_center_z = vertex_aabb.centerZ();
	//aabb_center_w = vertex_aabb.sizeX();			// FIXME: The Ws need to be figured out to know what they actually are
	aabb_center_w = 1.0f;
	aabb_min_x = vertex_aabb.start_x;
	aabb_min_y = vertex_aabb.start_y;
	aabb_min_z = vertex_aabb.start_z;
	//aabb_min_w = vertex_aabb.sizeY();				// FIXME: The Ws need to be figured out to know what they actually are
	aabb_min_w = 1.0f;
	aabb_max_x = vertex_aabb.end_x;
	aabb_max_y = vertex_aabb.end_y;
	aabb_max_z = vertex_aabb.end_z;
	//aabb_max_w = vertex_aabb.sizeZ();				// FIXME: The Ws need to be figured out to know what they actually are
	aabb_max_w = 1.0f;






	// Create Triangles : it's a serie of polygone/faces, with the same Bones influences.
	EMDTriangles* triangle_list_ptr;
	vector<string> polygon_bone_names;
	size_t nbSkinBind, index_tmp;
	string bone_name;
	size_t nbVertexWithFaceDef = fbxMesh->GetPolygonVertexCount();
	size_t offsetVertex = 0;


	for (size_t i = 0; i < lPolygonCount; ++i)
	{
		if (fbxMesh->GetPolygonSize(i) != 3)
		{
			offsetVertex += fbxMesh->GetPolygonSize(i);
			continue;
		}

		polygon_bone_names.clear();
		if (flags & EMD_VTX_FLAG_BLEND_WEIGHT)
		{

			//get List of Bones Names
			for (int j = 0; j < 3; j++)
			{
				index_tmp = fbxMesh->GetPolygonVertex(i, j);

				vector<pair<double, FbxNode *>> &skin_bindings = control_points_skin_bindings.at(index_tmp);

				nbSkinBind = skin_bindings.size();
				for (size_t k = 0; k < nbSkinBind; k++)
				{
					if (k >= 4)												//limit to 4 influences of bones on vertex.
						continue;

					bone_name = skin_bindings.at(k).second->GetName();

					bool isfound = false;
					size_t nbElement = polygon_bone_names.size();
					for (size_t m = 0; m < nbElement; m++)
					{
						if (polygon_bone_names.at(m) == bone_name)
						{
							isfound = true;
							break;
						}
					}
					if (!isfound)
						polygon_bone_names.push_back(bone_name);
				}
			}
		}


		triangle_list_ptr = getTriangleForBoneNameCombinaison(polygon_bone_names);					// Detect appropriate triangle for these 3 vertices
		polygon_bone_names = triangle_list_ptr->bone_names;
		size_t nbBoneNames = polygon_bone_names.size();



		for (int j = 0; j < 3; j++)
		{
			index_tmp = fbxMesh->GetPolygonVertex(i, j);
			if (index_tmp >= nbVertex)
			{
				printf("error: vertex index out of list. use 0.\n");
				index_tmp = 0;
			}


			EMDVertex &v = vertices_tmp.at(index_tmp);
			EMDVertex v_copy = EMDVertex(v);


			//here, we have to deal with per-face informations, duplication of vertex, and so , new indexes.
			if ((nbNormals == nbVertexWithFaceDef) && (offsetVertex + j < nbNormals))				//per-face normals
			{
				normal = rotation_matrix.MultT(listNormals->GetAt(offsetVertex + j));
				v_copy.norm_x = (float)normal[0];
				v_copy.norm_y = (float)normal[1];
				v_copy.norm_z = (float)normal[2];
			}

			if ((nbTangentes == nbVertexWithFaceDef) && (offsetVertex + j < nbTangentes))				//per-face tangentes
			{
				tangent = rotation_matrix.MultT(listTangentes->GetAt(offsetVertex + j));
				v_copy.tang_x = (float)tangent[0];
				v_copy.tang_y = (float)tangent[1];
				v_copy.tang_z = (float)tangent[2];
			}

			for (size_t k = 0; k < nbUvs; k++)
			{
				size_t nbVertexUv = listUVs.at(k)->GetCount();

				if ((nbVertexUv == nbVertexWithFaceDef) && (offsetVertex + j < nbVertexUv))						//per-face uv
				{
					uv = listUVs.at(k)->GetAt(offsetVertex + j);
					if (k == 0)
					{
						v_copy.text_u = (float)uv[0];
						v_copy.text_v = 1.0f - (float)uv[1];
					}else {
						v_copy.text2_u = (float)uv[0];
						v_copy.text2_v = 1.0f - (float)uv[1];
					}
				}
			}

			if ((nbVertexColor == nbVertexWithFaceDef) && (offsetVertex + j < nbVertexColor))						//per-face colors
			{
				color = listVertexColors->GetAt(offsetVertex + j);
				v_copy.setColorFromRGBAFloat((float)color.mRed, (float)color.mGreen, (float)color.mBlue, (float)color.mAlpha);
			}

			// apply bone influence on vertex
			if (flags & EMD_VTX_FLAG_BLEND_WEIGHT)
			{
				vector<pair<double, FbxNode *>> &skin_bindings = control_points_skin_bindings.at(index_tmp);

				for (size_t k = 0; k < skin_bindings.size(); k++)
				{
					bone_name = skin_bindings.at(k).second->GetName();

					size_t isFound = (size_t)-1;
					for (size_t m = 0; m < nbBoneNames; m++)
					{
						if (polygon_bone_names.at(m) == bone_name)
						{
							isFound = m;
							break;
						}
					}

					if (isFound != (size_t)-1)
					{
						v_copy.blend[3 - k] = isFound;
						v_copy.blend_weight[k] = (float)skin_bindings.at(k).first;
					}
				}
			}

			size_t newIndex = mergeVertex(v_copy);							//try to find a similar vertex, or add into the list (could be long for bug meshes, but reduce the number of duplicate vertex , and resolve the per-face informations)

			triangle_list_ptr->faces.push_back(newIndex);						//fill faces
		}

		offsetVertex += 3;
	}

	










	// Create Definitions (TextureUnit)
	FbxSurfaceMaterial * mat;
	FbxTexture* texture;
	string texName;
	size_t posIndex;

	size_t nbMat = fbxNode->GetMaterialCount();
	for (size_t i=0; i<nbMat; ++i)
	{
		mat = fbxNode->GetMaterial(i);

		//normally, we have only one material in this case.
		definitions.clear();									//so we cleanning.

		texture = getTexture(mat, FbxSurfaceMaterial::sNormalMap);
		if (texture)
		{
			texName = LibXenoverse::nameFromFilenameNoExtension(texture->GetName(), true);
			
			posIndex = texName.find_last_of('_');
			if (posIndex != std::string::npos)
			{
				texName = texName.substr(posIndex+1);
				size_t textIndex = std::stoi(texName);
				definitions.push_back(EMDSubmeshDefinition(textIndex, (float)texture->GetScaleU(), (float)texture->GetScaleV()) );
			}
		}

		texture = getTexture(mat, FbxSurfaceMaterial::sEmissive);
		if (texture)
		{
			texName = LibXenoverse::nameFromFilenameNoExtension(texture->GetName(), true);

			posIndex = texName.find_last_of('_');
			if (posIndex != std::string::npos)
			{
				texName = texName.substr(posIndex + 1);
				size_t textIndex = std::stoi(texName);
				definitions.push_back(EMDSubmeshDefinition(textIndex, (float)texture->GetScaleU(), (float)texture->GetScaleV()));
			}
		}

		texture = getTexture(mat, FbxSurfaceMaterial::sReflection);
		if (texture)
		{
			texName = LibXenoverse::nameFromFilenameNoExtension(texture->GetName(), true);

			posIndex = texName.find_last_of('_');
			if (posIndex != std::string::npos)
			{
				texName = texName.substr(posIndex + 1);
				size_t textIndex = std::stoi(texName);
				definitions.push_back(EMDSubmeshDefinition(textIndex, (float)texture->GetScaleU(), (float)texture->GetScaleV()));
			}
		}

		texture = getTexture(mat, FbxSurfaceMaterial::sBump);
		if (texture)
		{
			texName = LibXenoverse::nameFromFilenameNoExtension(texture->GetName(), true);

			posIndex = texName.find_last_of('_');
			if (posIndex != std::string::npos)
			{
				texName = texName.substr(posIndex + 1);
				size_t textIndex = std::stoi(texName);
				definitions.push_back(EMDSubmeshDefinition(textIndex, (float)texture->GetScaleU(), (float)texture->GetScaleV()));
			}
		}


		texture = getTexture(mat, FbxSurfaceMaterial::sDisplacementColor);
		if (texture)
		{
			texName = LibXenoverse::nameFromFilenameNoExtension(texture->GetName(), true);

			posIndex = texName.find_last_of('_');
			if (posIndex != std::string::npos)
			{
				texName = texName.substr(posIndex + 1);
				size_t textIndex = std::stoi(texName);
				definitions.push_back(EMDSubmeshDefinition(textIndex, (float)texture->GetScaleU(), (float)texture->GetScaleV()));
			}
		}

		texture = getTexture(mat, FbxSurfaceMaterial::sVectorDisplacementColor);
		if (texture)
		{
			texName = LibXenoverse::nameFromFilenameNoExtension(texture->GetName(), true);

			posIndex = texName.find_last_of('_');
			if (posIndex != std::string::npos)
			{
				texName = texName.substr(posIndex + 1);
				size_t textIndex = std::stoi(texName);
				definitions.push_back(EMDSubmeshDefinition(textIndex, (float)texture->GetScaleU(), (float)texture->GetScaleV()));
			}
		}
	}

	//hack to fix in lot of case the fact that blender export only few textures, so we lost some.
	if (definitions.size() == 1)
		definitions.push_back(definitions.back());

	if (definitions.size() == 0)
	{
		definitions.push_back(EMDSubmeshDefinition(0));
		definitions.push_back(EMDSubmeshDefinition(0));
	}










	//clean for indirect array

	if((geoElemNorm) && (geoElemNorm->GetReferenceMode() == FbxLayerElement::eIndexToDirect))
	{
		delete listNormals;
		listNormals = 0;
	}
	
		

	for (size_t i = 0; i < nbUvs; i++)
	{
		FbxGeometryElementUV* geoElemUV = fbxMesh->GetElementUV(uv_sets[i]);

		if((geoElemUV) && (geoElemUV->GetReferenceMode() == FbxLayerElement::eIndexToDirect))				//todo: deal with this case on others flags.
		{
			delete listUVs.at(i);
			listUVs.at(i) = 0;
		}
	}

	if ((geoElemColor) &&(geoElemColor->GetReferenceMode() == FbxLayerElement::eIndexToDirect))
	{
		delete listVertexColors;
		listVertexColors = 0;
	}
}
/*-------------------------------------------------------------------------------\
|                             getTexture										 |
\-------------------------------------------------------------------------------*/
FbxTexture* EMDSubmesh::getTexture(FbxSurfaceMaterial * mat, const char* typeMap)
{
	FbxProperty prop = mat->FindProperty(typeMap);
	
	size_t nbLayerTextures = prop.GetSrcObjectCount<FbxLayeredTexture>();
	if (nbLayerTextures>0)
	{
		FbxLayeredTexture* layered_texture;
		for (size_t j = 0; j < nbLayerTextures; j++)
		{
			layered_texture = FbxCast<FbxLayeredTexture>(prop.GetSrcObject<FbxLayeredTexture>(j));

			if (layered_texture->GetSrcObjectCount<FbxTexture>()>0)
				return FbxCast<FbxTexture>(layered_texture->GetSrcObject<FbxTexture>(0));
		}

	}else{
		if (prop.GetSrcObjectCount<FbxTexture>()>0)
			return FbxCast<FbxTexture>(prop.GetSrcObject<FbxTexture>(0));
	}

	return NULL;
}





#endif

}