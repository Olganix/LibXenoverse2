
//#include "fbxsdk/core/arch/fbxtypes.h"
#include <fstream>

namespace LibXenoverse
{


#ifdef LIBXENOVERSE_FBX_SUPPORT








/*-------------------------------------------------------------------------------\
|                             exportFBX											 |
\-------------------------------------------------------------------------------*/
void EMD::exportFBX(FbxScene *scene, std::vector<ESK::FbxBonesInstance_DBxv> &global_fbx_bones, std::vector<EMB*> listTexturePackEMB, EMM* emmMaterial, bool wantNoTexture)
{
	FbxNode *node = FbxNode::Create(scene, name.c_str());
	scene->GetRootNode()->AddChild(node);


	node->LclTranslation.Set(FbxVector4(0, 0, 0));
	node->LclRotation.Set(FbxVector4(0, 0, 0, 1));
	node->LclScaling.Set(FbxVector4(1, 1, 1));


	size_t nbModels = models.size();
	for (size_t i = 0; i < nbModels; i++)
		models.at(i)->exportFBX(scene, global_fbx_bones, node, i, listTexturePackEMB, emmMaterial, wantNoTexture);
}
/*-------------------------------------------------------------------------------\
|                             exportFBX											 |
\-------------------------------------------------------------------------------*/
void EMDModel::exportFBX(FbxScene *scene, std::vector<ESK::FbxBonesInstance_DBxv> &global_fbx_bones, FbxNode* parentNode, size_t indexModel, std::vector<EMB*> listTexturePackEMB, EMM* emmMaterial, bool wantNoTexture)
{
	FbxNode *node = FbxNode::Create(scene, name.c_str());
	//FbxNode *node = FbxNode::Create(scene, (name +"_test").c_str());			//test todo remove
	node->LclTranslation.Set(FbxVector4(0, 0, 0));
	node->LclRotation.Set(FbxVector4(0, 0, 0));
	node->LclScaling.Set(FbxVector4(1, 1, 1));

	parentNode->AddChild(node);

	
	//Test Todo remove. to have Node animation (not just on bones).
	FbxNode* forceBoneNode = 0;
	if (false)
	{
		const int lNodeCount = scene->GetSrcObjectCount<FbxNode>();
		for (int lIndex = 0; lIndex < lNodeCount; lIndex++)
		{
			FbxNode* fbxNode = scene->GetSrcObject<FbxNode>(lIndex);
			if ((!fbxNode) || (!fbxNode->GetSkeleton()) || (string(fbxNode->GetName()) != name))	//in this case the bones have the same name of a EmdModel
				continue;

			forceBoneNode = fbxNode;
			break;
		}
	}

	size_t nbMesh = meshes.size();
	for (size_t i = 0; i < nbMesh; i++)
		meshes.at(i)->exportFBX(scene, global_fbx_bones, node, listTexturePackEMB, emmMaterial, wantNoTexture, forceBoneNode);
}
/*-------------------------------------------------------------------------------\
|                             exportFBX											 |
\-------------------------------------------------------------------------------*/
void EMDMesh::exportFBX(FbxScene *scene, std::vector<ESK::FbxBonesInstance_DBxv> &global_fbx_bones, FbxNode* parentNode, std::vector<EMB*> listTexturePackEMB, EMM* emmMaterial, bool wantNoTexture, FbxNode* forceBoneNode)
{
	FbxNode *node = FbxNode::Create(scene, name.c_str());
	//FbxNode *node = FbxNode::Create(scene, (name + "_test").c_str());			//test todo remove
	node->LclTranslation.Set(FbxVector4(0, 0, 0));
	node->LclRotation.Set(FbxVector4(0, 0, 0));
	node->LclScaling.Set(FbxVector4(1, 1, 1));

	parentNode->AddChild(node);

	size_t nbSubMesh = submeshes.size();
	for (size_t i = 0; i < nbSubMesh; i++)
		submeshes.at(i)->exportFBX(scene, global_fbx_bones, node, listTexturePackEMB, emmMaterial, wantNoTexture, forceBoneNode);
}
/*-------------------------------------------------------------------------------\
|                             EMDSubmesh										 |
\-------------------------------------------------------------------------------*/
void EMDSubmesh::exportFBX(FbxScene *scene, std::vector<ESK::FbxBonesInstance_DBxv> &global_fbx_bones, FbxNode* parentNode, std::vector<EMB*> listTexturePackEMB, EMM* emmMaterial, bool wantNoTexture, FbxNode* forceBoneNode)
{
	FbxNode *node = FbxNode::Create(scene, name.c_str());
	node->LclTranslation.Set(FbxVector4(0, 0, 0));
	node->LclRotation.Set(FbxVector4(0, 0, 0));
	node->LclScaling.Set(FbxVector4(1, 1, 1));

	if (!forceBoneNode)
	{
		parentNode->AddChild(node);
	}else {
		forceBoneNode->AddChild(node);
		forceBoneNode = 0;
	}


	FbxMesh* fbxMesh = FbxMesh::Create(scene, name.c_str());
	if (!fbxMesh)
		return;
	node->SetNodeAttribute(fbxMesh);					//attach mesh to node.
	


	// Materials part
	FbxGeometryElementMaterial* lMaterialElement = fbxMesh->CreateElementMaterial();			//add a material use.
	lMaterialElement->SetMappingMode(FbxGeometryElement::eByPolygon);
	lMaterialElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);
	FbxSurfaceMaterial*	fxbMaterial = exportFBXMaterial(scene, name, listTexturePackEMB, emmMaterial, wantNoTexture);	//in submesh , name is also materialName
	node->AddMaterial(fxbMaterial);




	fbxMesh->InitControlPoints( vertices.size() );		//resize by the numbers of Vertex.



	//Add declaration of Vertex format.
	uint16_t flags = vertex_type_flag;
		
	//(flags & EMD_VTX_FLAG_POS)						//position is always into Fbx mesh declaration. no need to do anythings.
	FbxVector4* fbxControlPointsPosition = fbxMesh->GetControlPoints();						//pointer of array , for position only


	FbxGeometryElementNormal* fbxGeometryElementNormal = NULL;
	if (flags & EMD_VTX_FLAG_NORM)
	{
		fbxGeometryElementNormal = fbxMesh->CreateElementNormal();
		fbxGeometryElementNormal->SetMappingMode(FbxGeometryElement::eByControlPoint);
		fbxGeometryElementNormal->SetReferenceMode(FbxGeometryElement::eDirect);
	}

	FbxGeometryElementTangent* fbxGeometryElementTangent = NULL;
	if (flags & EMD_VTX_FLAG_TANGENT)
	{
		fbxGeometryElementTangent = fbxMesh->CreateElementTangent();
		fbxGeometryElementTangent->SetMappingMode(FbxGeometryElement::eByControlPoint);
		fbxGeometryElementTangent->SetReferenceMode(FbxGeometryElement::eDirect);
	}

	FbxGeometryElementVertexColor* fbxGeometryElementColor = NULL;
	if (flags & EMD_VTX_FLAG_COLOR)
	{
		fbxGeometryElementColor = fbxMesh->CreateElementVertexColor();
		fbxGeometryElementColor->SetMappingMode(FbxGeometryElement::eByControlPoint);
		fbxGeometryElementColor->SetReferenceMode(FbxGeometryElement::eDirect);
	}
	
	FbxGeometryElementUV* fbxGeometryElementUV = NULL;
	if (flags & EMD_VTX_FLAG_TEX)
	{
		fbxGeometryElementUV = fbxMesh->CreateElementUV("UV");
		fbxGeometryElementUV->SetMappingMode(FbxGeometryElement::eByControlPoint);
		fbxGeometryElementUV->SetReferenceMode(FbxGeometryElement::eDirect);
	}

	FbxGeometryElementUV* fbxGeometryElementUV2 = NULL;
	if (flags & EMD_VTX_FLAG_TEX2)
	{
		fbxGeometryElementUV2 = fbxMesh->CreateElementUV("UV2");
		fbxGeometryElementUV2->SetMappingMode(FbxGeometryElement::eByControlPoint);
		fbxGeometryElementUV2->SetReferenceMode(FbxGeometryElement::eDirect);
	}

	


	if (forceBoneNode)
		int aa = 42;

	
	/*
	if (((flags & EMD_VTX_FLAG_BLEND_WEIGHT) == 0) && (forceBoneNode))				//if it use bones influence. case force to match
		flags = flags | EMD_VTX_FLAG_BLEND_WEIGHT;
	else
		forceBoneNode = false;														//if allready have blend, stop force.
	*/
	
	FbxSkin* lSkin = NULL;
	if (flags & EMD_VTX_FLAG_BLEND_WEIGHT)				//if it use bones influence.
	{
		lSkin = FbxSkin::Create(scene, ("Skin_"+ name).c_str());
		if (lSkin != NULL)
			fbxMesh->AddDeformer(lSkin);													//use Skin on mesh
	}

	



	//Export Vertices.
	float r = 0;
	float g = 0;
	float b = 0;
	float a = 0;
	size_t nbVertex = vertices.size();
	for (size_t i = 0; i < nbVertex; i++)
	{
		EMDVertex &v = vertices.at(i);

		fbxControlPointsPosition[i] = FbxVector4(v.pos_x, v.pos_y, v.pos_z);

		if (fbxGeometryElementNormal)
			fbxGeometryElementNormal->GetDirectArray().Add( FbxVector4(v.norm_x, v.norm_y, v.norm_z) );
		if (fbxGeometryElementTangent)
			fbxGeometryElementTangent->GetDirectArray().Add(FbxVector4(v.tang_x, v.tang_y, v.tang_z));

		if (fbxGeometryElementUV)
			fbxGeometryElementUV->GetDirectArray().Add(FbxVector2(v.text_u, 1.0 - v.text_v));
		if (fbxGeometryElementUV2)
			fbxGeometryElementUV2->GetDirectArray().Add(FbxVector2(v.text2_u, 1.0 - v.text2_v));

		if (fbxGeometryElementColor)
		{
			v.getColorRGBAFloat(r, g, b, a);
			fbxGeometryElementColor->GetDirectArray().Add(FbxColor((double)r, (double)g, (double)b, (double)a));
		}
	}



	//Export Faces/triangle/IndexBuffer.
	size_t nbFaces = 0;
	vector<FbxCluster*> triangle_shortBoneListCluster;					//triangle use a short list of bone (not all bone per material), so we make a list of index from the real list of bones
	
	vector<FbxCluster*> clusterList;
	vector<string> boneAllareadyHaveClusterList;
	size_t nbBones;
	size_t nbAllBones = global_fbx_bones.size();
	

	size_t nbTriangles = triangles.size();
	for (size_t i = 0; i < nbTriangles; i++)
	{
		std::vector<unsigned int> &faces = triangles.at(i).faces;


		// Skinning prepartion part : make a list of bones for triangle.
		triangle_shortBoneListCluster.clear();
		if (flags & EMD_VTX_FLAG_BLEND_WEIGHT)
		{
			std::vector<std::string> &bone_names = triangles.at(i).bone_names;

			if (forceBoneNode)
			{
				bone_names.clear();
				bone_names.push_back(forceBoneNode->GetName());

				/*
				//Test because problem on declaration of parent on bindpose , for 3dsmax.
				FbxNode* bone_tmp = forceBoneNode->GetParent();
				while (bone_tmp)
				{
					string name_tmp = string(bone_tmp->GetName());
					if(name_tmp.length())
						bone_names.push_back(name_tmp);

					bone_tmp = bone_tmp->GetParent();
				}
				*/
			}


			FbxCluster* fbxCluster;
			nbBones = bone_names.size();
			for (size_t j = 0; j < nbBones; j++)
			{
				string &bone_name = bone_names.at(j);

				//avoid Cluster double
				size_t nbCluster = boneAllareadyHaveClusterList.size();
				bool isfound = false;
				for (size_t k = 0; k < nbCluster; k++)
				{
					if (boneAllareadyHaveClusterList.at(k) == bone_name)
					{
						triangle_shortBoneListCluster.push_back(clusterList.at(k));
						isfound = true;
						break;
					}
				}
				if (isfound)
					continue;


				//Create the Fbx cluster for the skin (same if there isn't bones, you could have influence)
				fbxCluster = FbxCluster::Create(scene, bone_name.c_str());
				fbxCluster->SetLinkMode(FbxCluster::eTotalOne);
				fbxCluster->SetTransformMatrix(scene->GetRootNode()->EvaluateGlobalTransform());
				lSkin->AddCluster(fbxCluster);						//attache to Skin
				
				clusterList.push_back(fbxCluster);
				boneAllareadyHaveClusterList.push_back(bone_name);
				triangle_shortBoneListCluster.push_back(fbxCluster);
				

				if (global_fbx_bones.size() != 0)
				{
					for (size_t k = 0; k < nbAllBones; k++)
					{
						if (bone_name == global_fbx_bones.at(k).mName)
						{
							ESK::FbxBonesInstance_DBxv	&fbxBoneInst = global_fbx_bones.at(k);
							fbxCluster->SetLink(fbxBoneInst.mNode);
							
							
							if (!forceBoneNode)
							{
								fbxCluster->SetTransformLinkMatrix(fbxBoneInst.mNode->EvaluateGlobalTransform());
							}else {
								FbxAMatrix matrix;
								matrix.SetIdentity();
								fbxCluster->SetTransformLinkMatrix(matrix);
							}

							break;
						}
					}
				}
			}
		}



		

		size_t bone_index;
		nbFaces = faces.size();
		for (size_t j = 0; j+2 < nbFaces; j += 3)
		{
			if ((faces.at(j) >= nbVertex) || (faces.at(j+1) >= nbVertex) || (faces.at(j+2) >= nbVertex))
			{
				printf("Problem on submesh %s : triangle/face/polygone/IndexBuffer target a wrong index %i (or %i or %i) into the vertexlist (nbvertex: %i). may be corrupt file. just skip polygone.\n", name.c_str(), faces.at(j), faces.at(j+1), faces.at(j+2), nbVertex);
				notifyError();
				continue;
			}


			//make polygone
			fbxMesh->BeginPolygon(0);

			fbxMesh->AddPolygon(faces.at(j));
			fbxMesh->AddPolygon(faces.at(j + 1));
			fbxMesh->AddPolygon(faces.at(j + 2));

			fbxMesh->EndPolygon();



			// Skinning Apply on Vertex
			if (!(flags & EMD_VTX_FLAG_BLEND_WEIGHT))
				continue;

			
			for (size_t k = 0; k < 3; k++)
			{
				EMDVertex &v = vertices.at(faces.at(j + k));

				

				/*
				//version witch work well with Blender, but there is weird things on Unity. that strange. fbx -> blender -> fbx solve this for Unity (but you loose material).
				for (size_t m = 0; m < 4; m++)								//check the 4 bone influences.
				//for (int m = 3; m >= 0; m--)				//test from check difference between fbx from emdfbx and the cleaned version from blender reexport.
				{
					//if (v.blend_weight[m] <= 0.0f)
					//	continue;
					
					bone_index = v.blend[m];
					if(bone_index >= triangle_shortBoneListCluster.size())
					{
						printf("Invalid Bone Index %d compared to Bone FBX Index Map size %d. just skip.\n", bone_index, triangle_shortBoneListCluster.size());
						notifyError();
						continue;
					}

					float blend_weight = ((m != 3) ? v.blend_weight[m] : (1.0f - (v.blend_weight[0] + v.blend_weight[1] + v.blend_weight[2])));

					triangle_shortBoneListCluster.at(bone_index)->AddControlPointIndex(faces.at(j + k), blend_weight);
				}
				*/
				
				
				
				//test version better for 3dsmax, but still trouble on unity
				std::vector<size_t> blendIndiceUnique;
				std::vector<float> blendweightlinked;
				for (size_t m = 0; m < 4; m++)				//check the 4 bones influences.
				{
					bone_index = v.blend[m];
					if (bone_index >= triangle_shortBoneListCluster.size())
					{
						printf("Invalid Bone Index %d compared to Bone FBX Index Map size %d. just skip.\n", bone_index, triangle_shortBoneListCluster.size());
						notifyError();
						continue;
					}
					float blend_weight = ((m != 3) ? v.blend_weight[m] : (1.0f - (v.blend_weight[0] + v.blend_weight[1] + v.blend_weight[2])));					

					if (forceBoneNode)
					{
						bone_index = 0;
						blend_weight = (m==0) ? 1.0f : 0.0f;
					}


					bool isfound_tmp = false;
					size_t nbIndex_tmp = blendIndiceUnique.size();
					for (size_t n = 0; n < nbIndex_tmp; n++)
					{
						if (blendIndiceUnique.at(n) == bone_index)
						{
							isfound_tmp = true;
							blendweightlinked.at(n) += blend_weight;
							break;
						}
					}
					if (!isfound_tmp)
					{
						blendIndiceUnique.push_back(bone_index);
						blendweightlinked.push_back(blend_weight);
					}
				}

				size_t nbIndex_tmp = blendIndiceUnique.size();
				for (size_t n = 0; n < nbIndex_tmp; n++)
					triangle_shortBoneListCluster.at(blendIndiceUnique.at(n))->AddControlPointIndex(faces.at(j + k), blendweightlinked.at(n));
				////////////
			}

		}
	}
}







/*-------------------------------------------------------------------------------\
|                             CreateTableEntry									 |
\-------------------------------------------------------------------------------*/
void CreateTableEntry(FbxBindingTable* pTable, FbxProperty& pProp)
{
	// create entry
	FbxBindingTableEntry& lEntry = pTable->AddNewEntry();
	// set src to the fbx property, in this sample, fbx properties have the same name with shader parameters
	FbxPropertyEntryView lSrc(&lEntry, true, true);
	// Because CgFX uses compound property, so do not use pProp.GetName()
	lSrc.SetProperty(pProp.GetHierarchicalName());

	// set dst to the shader parameter
	FbxSemanticEntryView lDst(&lEntry, false, true);
	lDst.SetSemantic(pProp.GetName());
}
/*-------------------------------------------------------------------------------\
|                             CustomParameterProp								 |
\-------------------------------------------------------------------------------*/
void CustomParameterProp(FbxSurfaceMaterial* lMaterial, FbxBindingTable* lTable, string paramName, FbxDouble4 val)
{
	FbxProperty lProp = FbxProperty::Create(lMaterial, FbxDouble4DT, paramName.c_str(), paramName.c_str());
	lProp.ModifyFlag(FbxPropertyFlags::eUserDefined, true);
	lProp.Set(val);
	CreateTableEntry(lTable, lProp);
}
/*-------------------------------------------------------------------------------\
|                             CustomTextureProp									 |
\-------------------------------------------------------------------------------*/
void CustomTextureProp(FbxScene *scene, FbxSurfaceMaterial* lMaterial, FbxBindingTable* lTable, string paramName, string filename)
{
	FbxProperty lProp = FbxProperty::Create(lMaterial, FbxDouble3DT, paramName.c_str(), paramName.c_str());
	lProp.ModifyFlag(FbxPropertyFlags::eUserDefined, true);
	FbxDouble3 lMapVal(0, 1, 0);
	lProp.Set(lMapVal);
	CreateTableEntry(lTable, lProp);

	FbxFileTexture* lTexture = FbxFileTexture::Create(scene, paramName.c_str());
	if (filename.size() != 0)
	{
		if (LibXenoverse::fileCheck(filename))
			lTexture->SetFileName(filename.c_str());
		else
			printf("Image %s not found\n", filename.c_str());
	}
	lTexture->SetTextureUse(FbxTexture::eStandard);
	lTexture->SetMappingType(FbxTexture::eUV);
	lTexture->ConnectDstProperty(lProp);
}
/*-------------------------------------------------------------------------------\
|                             exportFBXMaterial									 |
\-------------------------------------------------------------------------------*/
FbxSurfaceMaterial* EMDSubmesh::exportFBXMaterial(FbxScene *scene, string material_name, std::vector<EMB*> listTexturePackEMB, EMM* emmMaterial, bool wantNoTexture)
{
	
	
	if (wantNoTexture)						//case with a basic material, with no texture.
	{
		printf("Some problem of loading into blender a fbx using fbxMaterial with shader, may oblige to use classical material with only diffuse map. for material %s\n", material_name.c_str());

		FbxSurfacePhong* fbxMaterial = FbxSurfacePhong::Create(scene, FbxString(material_name.c_str()).Buffer());

		fbxMaterial->ShadingModel.Set(FbxString("Phong"));
		fbxMaterial->Emissive.Set(FbxDouble3(0.0, 0.0, 0.0));
		fbxMaterial->Ambient.Set(FbxDouble3(0.0, 0.0, 0.0));
		fbxMaterial->AmbientFactor.Set(1.0);
		fbxMaterial->Diffuse.Set(FbxDouble3(1.0, 1.0, 1.0));
		fbxMaterial->DiffuseFactor.Set(1.0);
		fbxMaterial->Shininess.Set(0.0);
		fbxMaterial->Specular.Set(FbxDouble3(0.0, 0.0, 0.0));
		fbxMaterial->SpecularFactor.Set(0.0);

		return (FbxSurfaceMaterial*)fbxMaterial;
	}
	
	
	
	
	
	//search current material.
	EMMMaterial* emm_mat = 0;
	float dytLineIndex = 0.0f;
	if (emmMaterial != NULL)
	{
		size_t isFound = (size_t)-1;

		vector<EMMMaterial *> listMaterial = emmMaterial->getMaterials();
		for (size_t i = 0; i < listMaterial.size(); i++)							//search for material definition
		{
			if (listMaterial.at(i)->getName() == material_name)
			{
				isFound = i;
				break;
			}
		}

		if (isFound != (size_t)-1)
		{
			emm_mat = emmMaterial->getMaterials().at(isFound);
			EMMParameter* parameter = emm_mat->getParameter("MatScale1X");

			if(parameter)
				dytLineIndex = parameter->float_value;
		}
	}





	//a problem on using shader into blender via fbx definition, may crash the loading of fbx file into blender. 
	// so we only use a classic Material of Fbx, with some texture.
	//if (!emmMaterial)										
	{
		FbxSurfacePhong* fbxMaterial = FbxSurfacePhong::Create(scene, FbxString(material_name.c_str()).Buffer());

		fbxMaterial->ShadingModel.Set(FbxString("Phong"));
		fbxMaterial->Emissive.Set( FbxDouble3(0.0, 0.0, 0.0) );
		fbxMaterial->Ambient.Set( FbxDouble3(0.0, 0.0, 0.0) );
		fbxMaterial->AmbientFactor.Set(1.0);
		fbxMaterial->Diffuse.Set( FbxDouble3(1.0, 1.0, 1.0) );
		fbxMaterial->DiffuseFactor.Set(1.0);
		fbxMaterial->Shininess.Set(0.0);
		fbxMaterial->Specular.Set( FbxDouble3(0.0, 0.0, 0.0) );
		fbxMaterial->SpecularFactor.Set(0.0);


		
		
		string textureName = material_name + "_dyt";
		string filename = ((listTexturePackEMB.size() >= 2) ? (listTexturePackEMB.at(1)->getName() + "\\DATA000.dds") : "");
		
		//in Unity, the material have the name of texture. and as texturea have the same name for all emd part, it's not working. so we have to duplicate textures files with a custom name.
		if (FILE *file = fopen(filename.c_str(), "r"))
		{
			fclose(file);

			string filenameNew = ((listTexturePackEMB.size() >= 2) ? (listTexturePackEMB.at(1)->getName() + "\\" + textureName + ".dds") : "");
			
			//void copy_file(const char* srce_file, const char* dest_file)
			{
				std::ifstream srce(filename, std::ios::binary);
				std::ofstream dest(filenameNew, std::ios::binary);
				dest << srce.rdbuf();
			}

			filename = filenameNew;
		}
		
		
		
		FbxFileTexture* lTexture = FbxFileTexture::Create(scene, textureName.c_str());

		

		// Set texture properties.
		if (filename.size() != 0)
		{
			if (LibXenoverse::fileCheck(filename))
			{
				lTexture->SetFileName(filename.c_str());
			}else {
				printf("Image %s not found\n", filename.c_str());
				notifyError();
			}
		}
		lTexture->SetTextureUse(FbxTexture::eStandard);
		lTexture->SetMappingType(FbxTexture::eUV);
		lTexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
		lTexture->SetSwapUV(false);
		lTexture->SetWrapMode(FbxTexture::EWrapMode::eClamp, FbxTexture::EWrapMode::eClamp);
		lTexture->SetRotation(0.0, 0.0);
		
		
		//case of Unity :
		lTexture->SetScale(1.0/128.0, 1.0/ 1142);						// 1 line of 32, one pixel of 128 (on the line), 1142 is because of blur of filtering into Unity.
		lTexture->SetTranslation(0.85, (32.0 - (dytLineIndex * 4 + 0.5)) / 32.0 );		// on the middle of the 1st line of the block, on the right-end part.
		
		/*
		//case for Blender
		lTexture->SetScale(1.0, 1.0/128.0);						// one pixel of 128
		lTexture->SetTranslation(0.0, 0.47 - (dytLineIndex * 4) / 32.0);		// on the middle of the 1st line of the block, on the right-end part.
		*/


		// don't forget to connect the texture to the corresponding property of the material
		if (fbxMaterial)
		{
			fbxMaterial->Diffuse.ConnectSrcObject(lTexture);
			fbxMaterial->Ambient.ConnectSrcObject(lTexture);
		}




		
		size_t nbDefs = definitions.size();
		for(size_t i=0;i<nbDefs;i++)
		{
			EMDTextureUnitState &def = definitions.at(i);
			string indexStr = std::to_string(def.texIndex);

			while(indexStr.length() < 3)
				indexStr = "0" + indexStr;

			string textureName = material_name + "_"+ indexStr;
			string filename = ((listTexturePackEMB.size() >= 1) ? (listTexturePackEMB.at(0)->getName() + "\\DATA"+ indexStr +".dds") : "");


			if (FILE *file = fopen(filename.c_str(), "r"))
			{
				fclose(file);

				string filenameNew = ((listTexturePackEMB.size() >= 1) ? (listTexturePackEMB.at(0)->getName() + "\\" + textureName + ".dds") : "");
				
				std::ifstream srce(filename, std::ios::binary);
				std::ofstream dest(filenameNew, std::ios::binary);
				dest << srce.rdbuf();

				filename = filenameNew;
			}

			FbxFileTexture* lTexture = FbxFileTexture::Create(scene, textureName.c_str());



			// Set texture properties.
			if (filename.size() != 0)
			{
				if (LibXenoverse::fileCheck(filename))
				{
					lTexture->SetFileName(filename.c_str());
				}else {
					printf("Image %s not found", filename.c_str());
					notifyError();
				}
			}
			lTexture->SetTextureUse(FbxTexture::eStandard);
			lTexture->SetMappingType(FbxTexture::eUV);
			lTexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
			lTexture->SetSwapUV(false);
			lTexture->SetRotation(0.0, 0.0);
			lTexture->SetScale(def.textScale_u, def.textScale_v);
			lTexture->SetTranslation(0.0, 0.0);

			if (fbxMaterial)
			{
				if (i == 0)
					fbxMaterial->NormalMap.ConnectSrcObject(lTexture);
				else if (i == 1)
					fbxMaterial->Emissive.ConnectSrcObject(lTexture);
				else if (i == 2)
					fbxMaterial->Reflection.ConnectSrcObject(lTexture);
				else if (i == 3)
					fbxMaterial->Bump.ConnectSrcObject(lTexture);
				else if (i == 4)
					fbxMaterial->DisplacementColor.ConnectSrcObject(lTexture);
				else if (i == 5)
					fbxMaterial->VectorDisplacementColor.ConnectSrcObject(lTexture);
				else {
					printf("Definition up to 6. skipped");
					notifyError();
				}
			}
		}
		

		return (FbxSurfaceMaterial*)fbxMaterial;
	}


	/*
	//a Test version , with shader (in fx conversion) integrate to the material, could work with 3dsmax but , still need so correction by hand because of fbx import in 3dsmax, and it's crash on blender, so ...

	

	if (isFound == (size_t)-1)						//default case if there is a problem.
	{
		printf("DBX Material %s not found. Put a default material in fbx.\n", material_name.c_str());
		notifyError();

		FbxSurfacePhong* fbxMaterial = FbxSurfacePhong::Create(scene, FbxString(material_name.c_str()).Buffer());

		fbxMaterial->ShadingModel.Set(FbxString("Phong"));
		fbxMaterial->Emissive.Set(FbxDouble3(0.0, 0.0, 0.0));
		fbxMaterial->Ambient.Set(FbxDouble3(0.0, 0.0, 0.0));
		fbxMaterial->AmbientFactor.Set(1.0);
		fbxMaterial->Diffuse.Set(FbxDouble3(1.0, 1.0, 1.0));
		fbxMaterial->DiffuseFactor.Set(1.0);
		fbxMaterial->Shininess.Set(0.0);
		fbxMaterial->Specular.Set(FbxDouble3(0.0, 0.0, 0.0));
		fbxMaterial->SpecularFactor.Set(0.0);

		return (FbxSurfaceMaterial*)fbxMaterial;
	}


	//**** inspired by FBX_2015_1\samples\ExportShader\main.cxx
	string &shaderName = emm_mat->getShaderName();

	FbxSurfaceMaterial* fbxMaterial = FbxSurfaceMaterial::Create(scene, FbxString(material_name.c_str()).Buffer());
	FbxImplementation* lImpl = FbxImplementation::Create(scene, FbxString((material_name + "_Implementation").c_str()));

	fbxMaterial->AddImplementation(lImpl);
	fbxMaterial->SetDefaultImplementation(lImpl);
	lImpl->RenderAPI = FBXSDK_RENDERING_API_DIRECTX;
	lImpl->RenderAPIVersion = "9.0";
	lImpl->Language = FBXSDK_SHADING_LANGUAGE_HLSL;
	lImpl->LanguageVersion = "3.0";									//TODO avant il y a avait "1.0", mais ca aurait du etre 3.0. todo verifier si ce n'etait pas ca qui faisait merder le truc, notamment dans le 3dsmax, et dans le blender.

	FbxBindingTable* lTable = lTable = lImpl->AddNewTable("root", "shader");
	lImpl->RootBindingName = "root";
	lTable->DescAbsoluteURL = (shaderName + ".fx").c_str();			// shader file
	lTable->DescTAG = "dx9";										// technique name

	//////////////////////// custom shader parameters for this material.		
	CustomTextureProp(scene, fbxMaterial, lTable, "g_ImageSampler1",		((listTexturePackEMB.size()>=1) ? (listTexturePackEMB.at(0)->getName() + "\\DATA000.dds") : ""));	// Property g_ImageSampler1, the property type is sample, so connect a texture to it
	CustomTextureProp(scene, fbxMaterial, lTable, "g_SamplerToon",			((listTexturePackEMB.size()>1) ? (listTexturePackEMB.at(1)->getName() + "\\DATA000.dds") : ""));
	CustomTextureProp(scene, fbxMaterial, lTable, "g_ImageSamplerTemp14",	((listTexturePackEMB.size()>1) ? (listTexturePackEMB.at(1)->getName() + "\\DATA001.dds") : ""));

	FbxDouble4 MatCol0(0, 0, 0, 0);
	FbxDouble4 MatCol1(0, 0, 0, 0);
	FbxDouble4 MatCol2(0, 0, 0, 0);
	FbxDouble4 MatCol3(0, 0, 0, 0);
	FbxDouble4 MatScale0(1, 1, 1, 1);
	FbxDouble4 MatScale1(1, 1, 1, 1);

	vector<EMMParameter *> &parameters = emm_mat->getParameters();
	for (size_t i = 0; i < parameters.size(); i++)
	{
		string parameter_name = parameters[i]->name;
		if (parameter_name == "MatCol0R") MatCol0[0] = parameters[i]->float_value;
		if (parameter_name == "MatCol0G") MatCol0[1] = parameters[i]->float_value;
		if (parameter_name == "MatCol0B") MatCol0[2] = parameters[i]->float_value;
		if (parameter_name == "MatCol0A") MatCol0[3] = parameters[i]->float_value;

		if (parameter_name == "MatCol1R") MatCol1[0] = parameters[i]->float_value;
		if (parameter_name == "MatCol1G") MatCol1[1] = parameters[i]->float_value;
		if (parameter_name == "MatCol1B") MatCol1[2] = parameters[i]->float_value;
		if (parameter_name == "MatCol1A") MatCol1[3] = parameters[i]->float_value;

		if (parameter_name == "MatCol2R") MatCol2[0] = parameters[i]->float_value;
		if (parameter_name == "MatCol2G") MatCol2[1] = parameters[i]->float_value;
		if (parameter_name == "MatCol2B") MatCol2[2] = parameters[i]->float_value;
		if (parameter_name == "MatCol2A") MatCol2[3] = parameters[i]->float_value;

		if (parameter_name == "MatCol3R") MatCol3[0] = parameters[i]->float_value;
		if (parameter_name == "MatCol3G") MatCol3[1] = parameters[i]->float_value;
		if (parameter_name == "MatCol3B") MatCol3[2] = parameters[i]->float_value;
		if (parameter_name == "MatCol3A") MatCol3[3] = parameters[i]->float_value;

		if (parameter_name == "MatScale0X") MatScale0[0] = parameters[i]->float_value;
		if (parameter_name == "MatScale0Y") MatScale0[1] = parameters[i]->float_value;
		if (parameter_name == "MatScale0Z") MatScale0[2] = parameters[i]->float_value;
		if (parameter_name == "MatScale0W") MatScale0[3] = parameters[i]->float_value;

		if (parameter_name == "MatScale1X") MatScale1[0] = parameters[i]->float_value;
		if (parameter_name == "MatScale1Y") MatScale1[1] = parameters[i]->float_value;
		if (parameter_name == "MatScale1Z") MatScale1[2] = parameters[i]->float_value;
		if (parameter_name == "MatScale1W") MatScale1[3] = parameters[i]->float_value;
	}

	// Override Battle Damage
	MatCol3[0] = 0.0; // Scratch Mark Multiplier
	MatCol3[1] = 0.0; // Blood Mark Multiplier

	/*
	FbxDouble4 val(0, 0, 0, 0);
	CustomParameterProp(fbxMaterial, lTable, "g_vFadeMulti_PS", val);
	CustomParameterProp(fbxMaterial, lTable, "g_vFadeRim_PS", val);
	CustomParameterProp(fbxMaterial, lTable, "g_vFadeAdd_PS", val);
	*//*

	CustomParameterProp(fbxMaterial, lTable, "MatCol0", MatCol0);
	CustomParameterProp(fbxMaterial, lTable, "MatCol1", MatCol1);
	CustomParameterProp(fbxMaterial, lTable, "MatCol2", MatCol2);
	CustomParameterProp(fbxMaterial, lTable, "MatCol3", MatCol3);
	CustomParameterProp(fbxMaterial, lTable, "MatScale0", MatScale0);
	CustomParameterProp(fbxMaterial, lTable, "MatScale1", MatScale1);

	/*
	CustomParameterProp(fbxMaterial, lTable, "g_vColor0_PS", val);
	CustomParameterProp(fbxMaterial, lTable, "g_vColor1_PS", val);
	CustomParameterProp(fbxMaterial, lTable, "g_vColor2_PS", val);
	CustomParameterProp(fbxMaterial, lTable, "g_vColor12_PS", val);
	val = FbxDouble4(1,1,1,1);
	CustomParameterProp(fbxMaterial, lTable, "g_Color_Multiplier", val);
	val = FbxDouble4(0,0,0,0);
	CustomParameterProp(fbxMaterial, lTable, "g_vParam4_PS", val);
	CustomParameterProp(fbxMaterial, lTable, "g_vParam5_PS", val);
	val = FbxDouble4(0, 23.2558, 0.04587, 0);
	CustomParameterProp(fbxMaterial, lTable, "g_Toon_Detail", val);
	

	return fbxMaterial;
	*/
}




#endif
}