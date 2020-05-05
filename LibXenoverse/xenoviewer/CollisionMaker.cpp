

#include "CollisionMaker.h"



#define VOXELIZER_IMPLEMENTATION
#include "../../depends/voxelizer/voxelizer.h"
// https://github.com/karimnaaji/voxelizer

using namespace Ogre;


void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}


CollisionMaker::CollisionMaker(Ogre::SceneManager* sceneMgr, Ogre::Camera *cam)
{
	uniqueInc = 0;
	mVoxelSize = 0.4;

	mSceneMgr = sceneMgr;
	mCamera = cam;
	mEntity = 0;
	mVisible = true;
	mIsOpaque = true;
	mIsReduced = false;
	mCouldHaveBoxIntersection = false;

	if(!mSceneMgr->hasSceneNode("VoxelRoot")) 
		mSceneMgr->getRootSceneNode()->createChildSceneNode("VoxelRoot");
}



CollisionMaker::~CollisionMaker()
{
	destroyVoxel();

	if (mSceneMgr->hasSceneNode("VoxelRoot"))
		mSceneMgr->destroySceneNode("VoxelRoot");
}


void CollisionMaker::setVisible(bool enable)
{ 
	mVisible = enable; 
	
	checkEntityCreationOnVisible();
	
	mSceneMgr->getSceneNode("VoxelRoot")->setVisible(mVisible); 	
}

void CollisionMaker::setIsOpaque(bool enable)
{
	mIsOpaque = enable;

	checkEntityCreationOnVisible();

	if(mEntity)
		mEntity->setMaterialName(mIsOpaque ? "Voxel" : "Voxel_Tranparent");
}




void CollisionMaker::buildVoxel(std::list<EMDOgre*> &emdOgre_list)
{
	destroyVoxel();

	std::vector<vx_vertex_t> vertices;
	std::vector<size_t> faceIndexes;
	size_t startIndex = 0;

	

	EMDOgre* emdOgre;
	for (size_t i = 0, nbObject = emdOgre_list.size(); i < nbObject; i++)
	{
		emdOgre = (i==0) ? (*emdOgre_list.begin()) : (*std::next(emdOgre_list.begin(), i) );

		if (emdOgre->getVisible() == false)
			continue;

		std::vector<Ogre::SceneNode*> listNodes;
		for (std::list<Ogre::SceneNode*>::iterator it = emdOgre->getListSceneNodes().begin(); it != emdOgre->getListSceneNodes().end(); it++)
		{
			listNodes.push_back((*it));
		}

		for (size_t j = 0; j < listNodes.size(); j++)					//that will growing because of recurrence.
		{
			for (size_t k = 0, nbChild = listNodes.at(j)->numChildren(); k < nbChild; k++)
				listNodes.push_back((Ogre::SceneNode*)listNodes.at(j)->getChild(k));
		}

		for (size_t n = 0; n < listNodes.size(); n++)
		{
			Ogre::SceneNode* node = listNodes.at(n);

			size_t nbObj = node->numAttachedObjects();
			if (nbObj == 0)
				continue;

			Ogre::Matrix4 nodeMatrix = node->_getFullTransform();

			for (size_t j = 0; j < nbObj; j++)
			{
				Ogre::Entity* ent = ((Ogre::Entity*)(node->getAttachedObject(j)));
				const Ogre::MeshPtr &mesh = ent->getMesh();


				for (size_t k = 0, nbSubMesh = mesh->getNumSubMeshes(); k < nbSubMesh; k++)
				{
					Ogre::SubMesh* submesh = mesh->getSubMesh(k);



					Ogre::VertexData* vertex_data = (submesh->useSharedVertices) ? mesh->sharedVertexData : submesh->vertexData;
					Ogre::IndexData* index_data = submesh->indexData;

					const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
					Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
					unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
					float* pReal;


					startIndex = vertices.size();


					for (size_t m = 0; m < vertex_data->vertexCount; ++m, vertex += vbuf->getVertexSize())
					{
						posElem->baseVertexPointerToElement((void*)vertex, &pReal);

						Vector3 pt;
						pt.x = (*pReal++);
						pt.y = (*pReal++);
						pt.z = (*pReal++);

						pt = nodeMatrix * pt;

						vx_vertex_t vx_vertex;
						vx_vertex.x = pt.x; vx_vertex.y = pt.y; vx_vertex.z = pt.z;
						vertices.push_back(vx_vertex);	
					}
					vbuf->unlock();

					
					unsigned short* pShort = 0;
					unsigned int* pInt = 0;
					
					Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
					bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
					if (use32bitindexes) 
						pInt = static_cast<unsigned int*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
					else 
						pShort = static_cast<unsigned short*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

					

					for (size_t m = 0; m < index_data->indexCount; ++m)
					{
						if(use32bitindexes)
							faceIndexes.push_back(startIndex + (unsigned int)pInt[m]);
						else
							faceIndexes.push_back(startIndex + (unsigned int)pShort[m]);
					}
					ibuf->unlock();	
				}
			}
		}
	}

	


	
	size_t nbVertex = vertices.size();
	size_t nbFaceIndex = faceIndexes.size();

	vx_mesh_t* vxMesh = vx_mesh_alloc(nbVertex, nbFaceIndex);

	for (size_t i = 0; i < nbVertex; i++)
	{
		vxMesh->vertices[i].x = vertices.at(i).x;
		vxMesh->vertices[i].y = vertices.at(i).y;
		vxMesh->vertices[i].z = vertices.at(i).z;
	}

	for (size_t i = 0; i < nbFaceIndex; i++)
		vxMesh->indices[i] = faceIndexes.at(i);

	//vx_point_cloud_t* vxResult = vx_voxelize_pc((vx_mesh_t const*)vxMesh, (float)mVoxelSize, (float)mVoxelSize, (float)mVoxelSize, (float)mVoxelSize / 2.5);	// Run voxelization. 0.01 Precision factor to reduce "holes" artifact
	//vx_point_cloud_t* vxResult = vx_voxelize_pc((vx_mesh_t const*)vxMesh, (float)mVoxelSize, (float)mVoxelSize, (float)mVoxelSize, (float)mVoxelSize / 2.0);	// Run voxelization. 0.01 Precision factor to reduce "holes" artifact
	vx_point_cloud_t* vxResult = vx_voxelize_pc((vx_mesh_t const*)vxMesh, (float)mVoxelSize, (float)mVoxelSize, (float)mVoxelSize, 0.0f);	// Run voxelization. 0.01 Precision factor to reduce "holes" artifact


	Ogre::SceneNode* voxelRoot = (mSceneMgr->hasSceneNode("VoxelRoot")) ? mSceneMgr->getSceneNode("VoxelRoot") : mSceneMgr->getRootSceneNode()->createChildSceneNode("VoxelRoot");

	Ogre::Vector3 fullWidth(mVoxelSize, mVoxelSize, mVoxelSize);
	Ogre::Vector3 halfWidth(mVoxelSize / 2.0, mVoxelSize / 2.0, mVoxelSize / 2.0);

	for (size_t i = 0; i < vxResult->nvertices; i++)
	{
		vx_vertex_t vertex = vxResult->vertices[i];

		string name = "Voxel_" + Ogre::StringConverter::toString(uniqueInc);
		OgreVoxel* voxel = new OgreVoxel(uniqueInc++, name, fullWidth, Ogre::Vector3(vertex.x, vertex.y, vertex.z) - halfWidth);		// - halfWidth because of scalling the voxel on reduceVoxel operation, the origine of the cube is on minXYZ, not on middle of the voxel. 
		mListVoxels.push_back(voxel);
	}

	vx_point_cloud_free(vxResult);
	vx_mesh_free(vxMesh);


	checkEntityCreationOnVisible();

	QMessageBox::about(NULL, "Collision Create Voxels", "Creation Voxels Done : "+ QString(Ogre::StringConverter::toString(mListVoxels.size()).c_str()) + " voxels");
}














void CollisionMaker::checkEntityCreationOnVisible()
{
	if ((mVisible) && (!mEntity))			//try to create entity only if visible one time.
	{
		float *vertices = 0;
		unsigned short* faces_16 = 0;
		unsigned int* faces_32 = 0;

		try
		{

			size_t nbVoxels = mListVoxels.size();
			size_t nVertices = nbVoxels * 24;
			size_t ibufCount = nbVoxels * 12 * 3;

			Ogre::MeshPtr ogre_mesh = Ogre::MeshManager::getSingleton().createManual("VoxelRoot", XENOVIEWER_RESOURCE_GROUP);

			//------- Create Vertex Pool
			Ogre::AxisAlignedBox mesh_aabb;

			// Create Shared Vertex Data for all submeshes
			Ogre::VertexData* vertex_data = new Ogre::VertexData();
			ogre_mesh->sharedVertexData = vertex_data;
			vertex_data->vertexCount = nVertices;
			size_t flags = EMD_VTX_FLAG_POS | EMD_VTX_FLAG_NORM | EMD_VTX_FLAG_TEX;

			// declarations 
			Ogre::VertexDeclaration* decl = vertex_data->vertexDeclaration;
			size_t offset = 0;

			if (flags & EMD_VTX_FLAG_POS)
			{
				decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
				offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
			}
			if (flags & EMD_VTX_FLAG_NORM)
			{
				decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
				offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
			}
			if (flags & EMD_VTX_FLAG_TEX)
			{
				decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
				offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
			}


			// fill datas of vertices
			size_t vertexSize = offset / 4;				// one float = 4 octets = 32 bits
			size_t index = 0;
			const size_t vbufCount = vertexSize * nVertices;
			vertices = (float *)malloc(sizeof(float)*vbufCount);


			if (nVertices < 65535)
				faces_16 = (unsigned short*)malloc(sizeof(unsigned short) * ibufCount);
			else
				faces_32 = (unsigned int*)malloc(sizeof(unsigned int) * ibufCount);


			for (size_t i = 0; i < nbVoxels; i++)
			{
				OgreVoxel* vox = mListVoxels.at(i);

				Ogre::Vector3 pos = vox->mPosition;
				Ogre::Vector3 width = vox->mScale;
				mesh_aabb.merge(Ogre::AxisAlignedBox(pos, pos + width));

				std::vector<EMDVertex> listVertex;
				EMDVertex v;
				v.norm_x = 0.0f; v.norm_y = -1.0f; v.norm_z = 0.0f;
				v.pos_x = (float)pos.x;				v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z + width.z);	v.text_u = 1.0f;	v.text_v = 1.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)pos.x;				v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z);			v.text_u = 1.0f;	v.text_v = 0.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)(pos.x + width.x);	v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z);			v.text_u = 0.0f;	v.text_v = 0.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)(pos.x + width.x);	v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z + width.z);	v.text_u = 0.0f;	v.text_v = 1.0f;	listVertex.push_back(EMDVertex(&v));

				v.norm_x = 0.0f; v.norm_y = 1.0f;  v.norm_z = 0.0f;
				v.pos_x = (float)pos.x;				v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z + width.z);	v.text_u = 0.0f;	v.text_v = 1.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)(pos.x + width.x);	v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z + width.z);	v.text_u = 1.0f;	v.text_v = 1.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)(pos.x + width.x);	v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z);			v.text_u = 1.0f;	v.text_v = 0.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)(pos.x);			v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z);			v.text_u = 0.0f;	v.text_v = 0.0f;	listVertex.push_back(EMDVertex(&v));

				v.norm_x = 0.0f; v.norm_y = 0.0f;  v.norm_z = 1.0f;
				v.pos_x = (float)pos.x;				v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z + width.z);	v.text_u = 0.0f;	v.text_v = 1.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)(pos.x + width.x);	v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z + width.z);	v.text_u = 1.0f;	v.text_v = 1.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)(pos.x + width.x);	v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z + width.z);	v.text_u = 1.0f;	v.text_v = 0.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)(pos.x);			v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z + width.z);	v.text_u = 0.0f;	v.text_v = 0.0f;	listVertex.push_back(EMDVertex(&v));

				v.norm_x = 1.0f; v.norm_y = 0.0f;  v.norm_z = 0.0f;
				v.pos_x = (float)(pos.x + width.x);	v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z + width.z);	v.text_u = 0.0f;	v.text_v = 1.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)(pos.x + width.x);	v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z);			v.text_u = 1.0f;	v.text_v = 1.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)(pos.x + width.x);	v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z);			v.text_u = 1.0f;	v.text_v = 0.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)(pos.x + width.x);	v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z + width.z);	v.text_u = 0.0f;	v.text_v = 0.0f;	listVertex.push_back(EMDVertex(&v));

				v.norm_x = 0.0f; v.norm_y = 0.0f;		v.norm_z = -1.0f;
				v.pos_x = (float)(pos.x + width.x);	v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z);			v.text_u = 0.0f;	v.text_v = 1.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)(pos.x);			v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z);			v.text_u = 1.0f;	v.text_v = 1.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)(pos.x);			v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z);			v.text_u = 1.0f;	v.text_v = 0.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)(pos.x + width.x);	v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z);			v.text_u = 0.0f;	v.text_v = 0.0f;	listVertex.push_back(EMDVertex(&v));

				v.norm_x = -1.0f; v.norm_y = 0.0f;  v.norm_z = 0.0f;
				v.pos_x = (float)(pos.x);			v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z);			v.text_u = 0.0f;	v.text_v = 1.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)(pos.x);			v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z + width.z);	v.text_u = 1.0f;	v.text_v = 1.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)(pos.x);			v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z + width.z);	v.text_u = 1.0f;	v.text_v = 0.0f;	listVertex.push_back(EMDVertex(&v));
				v.pos_x = (float)(pos.x);			v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z);			v.text_u = 0.0f;	v.text_v = 0.0f;	listVertex.push_back(EMDVertex(&v));


				std::vector<size_t> faces;
				faces.push_back(0); faces.push_back(1); faces.push_back(2);
				faces.push_back(2); faces.push_back(3); faces.push_back(0);
				faces.push_back(4); faces.push_back(5); faces.push_back(6);
				faces.push_back(6); faces.push_back(7); faces.push_back(4);
				faces.push_back(8); faces.push_back(9); faces.push_back(10);
				faces.push_back(10); faces.push_back(11); faces.push_back(8);
				faces.push_back(12); faces.push_back(13); faces.push_back(14);
				faces.push_back(14); faces.push_back(15); faces.push_back(12);
				faces.push_back(16); faces.push_back(17); faces.push_back(18);
				faces.push_back(18); faces.push_back(19); faces.push_back(16);
				faces.push_back(20); faces.push_back(21); faces.push_back(22);
				faces.push_back(22); faces.push_back(23); faces.push_back(20);


				size_t startOffsetVertex = 24 * i;
				size_t startOffsetFacesIndex = 12 * i * 3;

				for (size_t j = 0; j < 24; j++)
				{
					EMDVertex &v = listVertex.at(j);

					index = (startOffsetVertex + j) * vertexSize;

					Ogre::Vector3 pos(v.pos_x, v.pos_y, v.pos_z);

					if (flags & EMD_VTX_FLAG_POS)
					{
						vertices[index] = pos.x;
						vertices[index + 1] = pos.y;
						vertices[index + 2] = pos.z;
						index += 3;
					}

					if (flags & EMD_VTX_FLAG_NORM)
					{
						Ogre::Vector3 norm(v.norm_x, v.norm_y, v.norm_z);
						vertices[index] = norm.x;
						vertices[index + 1] = norm.y;
						vertices[index + 2] = norm.z;
						index += 3;
					}


					if (flags & EMD_VTX_FLAG_TEX)
					{
						vertices[index] = v.text_u;
						vertices[index + 1] = v.text_v;
						index += 2;
					}
				}


				for (size_t j = 0; j < 12 * 3; j++)
				{
					if (nVertices < 65535)
						faces_16[j + startOffsetFacesIndex] = (unsigned short)(faces.at(j) + startOffsetVertex);
					else
						faces_32[j + startOffsetFacesIndex] = faces.at(j) + startOffsetVertex;
				}
			}




			// create the hardwareBuffer for vertices.
			Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(offset, nVertices, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
			vbuf->writeData(0, vbuf->getSizeInBytes(), vertices, true);
			Ogre::VertexBufferBinding* bind = vertex_data->vertexBufferBinding;
			bind->setBinding(0, vbuf);


			Ogre::SubMesh *sub = ogre_mesh->createSubMesh();
			Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer((nVertices < 65535) ? Ogre::HardwareIndexBuffer::IT_16BIT : Ogre::HardwareIndexBuffer::IT_32BIT, ibufCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
			if ((nVertices < 65535))
				ibuf->writeData(0, ibuf->getSizeInBytes(), faces_16, true);
			else
				ibuf->writeData(0, ibuf->getSizeInBytes(), faces_32, true);
			sub->useSharedVertices = true;
			sub->indexData->indexBuffer = ibuf;
			sub->indexData->indexCount = ibufCount;
			sub->indexData->indexStart = 0;









			ogre_mesh->_setBounds(mesh_aabb); //not working well because it forget about animation states.
			//ogre_mesh->_setBounds(Ogre::AxisAlignedBox(-10000, -10000, -10000, 10000, 10000, 10000));
			ogre_mesh->_setBoundingSphereRadius(mesh_aabb.getSize().length());
			ogre_mesh->load();

			



			mEntity = mSceneMgr->createEntity("VoxelRoot", "VoxelRoot");
			mEntity->setMaterialName(mIsOpaque ? "Voxel" : "Voxel_Tranparent");

			mSceneMgr->getSceneNode("VoxelRoot")->attachObject(mEntity);

		}catch (...){

			QMessageBox::about(NULL, "Collision Voxel Creation", "There is problem on trying to create visual of voxel (may be too much voxel).\nYou can continue same as they are not visible.");
			mVisible = false;
		}

		if(vertices)
			::free(vertices);
		if (faces_16)
			::free(faces_16);
		if(faces_32)
			::free(faces_32);
	}
}












void CollisionMaker::destroyVoxel()
{
	if (mEntity)
		mSceneMgr->destroyEntity(mEntity);
	mEntity = 0;

	if (!Ogre::MeshManager::getSingleton().getByName("VoxelRoot").isNull())
		Ogre::MeshManager::getSingleton().remove("VoxelRoot");

	for (size_t i = 0, nbVoxels = mListVoxels.size(); i < nbVoxels; i++)
		delete mListVoxels.at(i);

	mListVoxels.clear();
	uniqueInc = 0;
	mIsReduced = false;
}













void CollisionMaker::reduceVoxel()
{
	size_t nbVoxels = mListVoxels.size();
	if ((nbVoxels == 0) || (mIsReduced))
	{
		QMessageBox::about(NULL, "Collision Voxel Export", (!mIsReduced) ? "No Voxels created. use BuildVoxel when there is a emd file loaded." : "ReduceVoxel could be done only once.");
		return;
	}

	//first try to get the MinMax
	Ogre::Vector3 fullWidth = Ogre::Vector3(mVoxelSize, mVoxelSize, mVoxelSize);
	Ogre::Vector3 halfWidth = Ogre::Vector3(mVoxelSize / 2.0, mVoxelSize / 2.0, mVoxelSize / 2.0);

	Ogre::Vector3 pos = mListVoxels.at(0)->mPosition;
	Ogre::AxisAlignedBox aabb(pos, pos + fullWidth);

	for (size_t i = 0; i < nbVoxels; i++)
	{
		pos = mListVoxels.at(i)->mPosition;
		aabb.merge( Ogre::AxisAlignedBox(pos, pos + fullWidth) );
	}

	Ogre::Vector3 aabbSize = aabb.getSize();
	Ogre::Vector3 aabbMin = aabb.getMinimum();
	Ogre::Vector3 nbElements( Ogre::Math::Floor(aabbSize.x / mVoxelSize) +1 , Ogre::Math::Floor(aabbSize.y / mVoxelSize) +1, Ogre::Math::Floor(aabbSize.z / mVoxelSize) + 1);







	// now we create a 3d Grid
	std::vector< std::vector< std::vector< OgreVoxel* > > > grid3D;
	for (size_t i = 0; i < nbElements.x; i++)
	{
		std::vector< std::vector< OgreVoxel* > > grid3D_x;

		for (size_t j = 0; j < nbElements.y; j++)
		{
			std::vector< OgreVoxel* > grid3D_y;

			for (size_t k = 0; k < nbElements.z; k++)
				grid3D_y.push_back(0);

			grid3D_x.push_back(grid3D_y);
		}
		grid3D.push_back(grid3D_x);
	}

	//and we fill of voxel on the right place in grid3D
	Ogre::Vector3 posIndex;
	for (size_t i = 0; i < nbVoxels; i++)
	{
		pos = mListVoxels.at(i)->mPosition + halfWidth;					// + halfWidth to be on middle of voxel and avoid float precision trouble with floor.
		posIndex = Ogre::Vector3(Ogre::Math::Floor((pos.x - aabbMin.x) / mVoxelSize), Ogre::Math::Floor((pos.y - aabbMin.y) / mVoxelSize), Ogre::Math::Floor((pos.z - aabbMin.z) / mVoxelSize));
		
		if (grid3D.at((size_t)posIndex.x).at((size_t)posIndex.y).at((size_t)posIndex.z) != 0)					//todo solve the trouble in algo of Voxelisation , thereis flaot presision trouble
		{
			mListVoxels.at(i)->allreadyUsed = true;
			mListVoxels.at(i)->tagToDelete = true;
			continue;
		}

		grid3D.at((size_t)posIndex.x).at((size_t)posIndex.y).at((size_t)posIndex.z) = mListVoxels.at(i);
	}




	// NOW we can try the greedy methode.
	

	size_t inc = nbVoxels;
	while (inc-- >= 0)
	{
		OgreVoxel* vox;
		Ogre::Vector3 startPos(-1, -1, -1);
		bool isfound = false;
		for (size_t j = 0; j < nbElements.y; j++)				// for many reason in ours majority cases, the best should be begin from bottom voxel first(guessing / subjective)
		{
			for (size_t i = 0; i < nbElements.x; i++)
			{
				for (size_t k = 0; k < nbElements.z; k++)
				{
					vox = grid3D.at(i).at(j).at(k);
					if ((vox) && (!vox->allreadyUsed))
					{
						startPos = Ogre::Vector3(i, j, k);
						vox->allreadyUsed = true;			//to not re-use a box
						isfound = true;
						break;
					}
				}
				if (isfound)
					break;
			}
			if (isfound)
				break;
		}
		if (!isfound)											// all box are used, is Finished
			break;

		Ogre::Vector3 endPos(startPos);
		


		

		
		for (size_t i = (size_t)(startPos.x) + 1; i < nbElements.x; i++)				// here the X extend
		{
			vox = grid3D.at(i).at( startPos.y ).at( startPos.z );

			if ((vox) && ((mCouldHaveBoxIntersection) || (!vox->allreadyUsed)) )
			{
				endPos.x = i;
				vox->allreadyUsed = true;
				vox->tagToDelete = true;												// OK now we have to remove all the box except the first at startPos, witch we make it bigger
			}else {
				break;
			}
		}


		for (size_t k = (size_t)(startPos.z) + 1; k < nbElements.z; k++)				// here the Z extend
		{
			bool isOkForZExtend = true;
			for (size_t i = (size_t)(startPos.x); i <= (size_t)(endPos.x); i++)
			{
				vox = grid3D.at(i).at(startPos.y).at(k);

				if ((!vox) || ((!mCouldHaveBoxIntersection) && (vox->allreadyUsed)))
				{
					isOkForZExtend = false;
					break;
				}
			}
			if (isOkForZExtend)
			{
				endPos.z = k;

				for (size_t i = (size_t)(startPos.x); i <= (size_t)(endPos.x); i++)
				{
					grid3D.at(i).at(startPos.y).at(k)->allreadyUsed = true;
					grid3D.at(i).at(startPos.y).at(k)->tagToDelete = true;
				}
			}else {
				break;
			}
		}


		for (size_t j = (size_t)(startPos.y) + 1; j < nbElements.y; j++)				// here the Y extend
		{
			bool isOkForYExtend = true;
			for (size_t i = (size_t)(startPos.x); i <= (size_t)(endPos.x); i++)
			{
				for (size_t k = (size_t)(startPos.z); k <= (size_t)(endPos.z); k++)
				{
					vox = grid3D.at(i).at(j).at(k);

					if ((!vox) || ((!mCouldHaveBoxIntersection) && (vox->allreadyUsed)))
					{
						isOkForYExtend = false;
						break;
					}
				}
				if (!isOkForYExtend)
					break;
			}

			if (isOkForYExtend)
			{
				endPos.y = j;

				for (size_t i = (size_t)(startPos.x); i <= (size_t)(endPos.x); i++)
				{
					for (size_t k = (size_t)(startPos.z); k <= (size_t)(endPos.z); k++)
					{
						grid3D.at(i).at(j).at(k)->allreadyUsed = true;
						grid3D.at(i).at(j).at(k)->tagToDelete = true;
					}
				}
			}else {
				break;
			}
		}

		vox = grid3D.at(startPos.x).at(startPos.y).at(startPos.z);
		vox->mScale = Ogre::Vector3((endPos.x - startPos.x + 1) * mVoxelSize, (endPos.y - startPos.y + 1) * mVoxelSize, (endPos.z - startPos.z + 1) * mVoxelSize);

		mIsReduced = true;
	}


	for (size_t i = 0; i < mListVoxels.size(); i++)
	{
		if (!mListVoxels.at(i)->tagToDelete)
			continue;

		delete mListVoxels.at(i);
		mListVoxels.erase(mListVoxels.begin() + i);
		i--;
	}


	//to rebuild 3d visualisation if visible.
	{
		if (mEntity)
			mSceneMgr->destroyEntity(mEntity);
		mEntity = 0;

		if (!Ogre::MeshManager::getSingleton().getByName("VoxelRoot").isNull())
			Ogre::MeshManager::getSingleton().remove("VoxelRoot");

		checkEntityCreationOnVisible();
	}

	QMessageBox::about(NULL, "Collision ReducteVoxel", "ReduceVoxel Done : " + QString( Ogre::StringConverter::toString(nbVoxels).c_str()) + " -> "+ QString(Ogre::StringConverter::toString(mListVoxels.size()).c_str()) + " voxels");
}



void CollisionMaker::exportVoxel_Emd(string basename)
{
	EMD* emd = exportVoxelEmd(nameFromFilenameNoExtension(basename));
	if (emd)
	{
		emd->save(basename + ".emd");
		delete emd;

		QMessageBox::about(NULL, "Collision Voxel Export", "Emd Export Done ("+ QString(Ogre::StringConverter::toString(mListVoxels.size()).c_str()) +" voxels)" );
	}
}

void CollisionMaker::exportVoxel_Fbx(string basename)
{
	EMD* emd = exportVoxelEmd(nameFromFilenameNoExtension(basename));
	if (!emd)
		return;

	bool exportAscii = false;
	string export_filename = basename + ".fbx";

	// Create FBX Manager
	FbxManager *sdk_manager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(sdk_manager, IOSROOT);
	ios->SetBoolProp(EXP_FBX_EMBEDDED, true);
	if(exportAscii)
		ios->SetBoolProp(EXP_ASCIIFBX, true);										//test to have debug on fbx. Todo comment
	sdk_manager->SetIOSettings(ios);

	// Create Scene
	FbxScene *scene = FbxScene::Create(sdk_manager, "EMDFBXScene");
	scene->GetGlobalSettings().SetCustomFrameRate(60.0);				//specify the frameRate, the number of image by second. (not working well in blender fbx importer. so, I had to modify default value into blender fbx importer).
	FbxTime::EProtocol protocol = FbxTime::GetGlobalTimeProtocol();
	FbxTime::SetGlobalTimeProtocol(FbxTime::EProtocol::eDefaultProtocol);
	protocol = FbxTime::GetGlobalTimeProtocol();
	scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::eOpenGL);	//eOpenGL,			!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded
	scene->GetGlobalSettings().SetSystemUnit(FbxSystemUnit::m);
	FbxNode *lRootNode = scene->GetRootNode();

	
	
	
	std::vector<ESK::FbxBonesInstance_DBxv> global_fbx_bones;
	std::vector<EMB*> mListEmb;
	emd->exportFBX(scene, global_fbx_bones, mListEmb, 0, true);
	delete emd;




	int lFileFormat = sdk_manager->GetIOPluginRegistry()->GetNativeWriterFormat();
	FbxExporter* lExporter = FbxExporter::Create(sdk_manager, "");
	bool lExportStatus = lExporter->Initialize(export_filename.c_str(), lFileFormat, sdk_manager->GetIOSettings());
	if (!lExportStatus)
	{
		printf("Call to FbxExporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
		LibXenoverse::notifyError();
		LibXenoverse::waitOnEnd();

		QMessageBox::about(NULL, "Collision Voxel Export", "Fail to open file for writting. Stopped");
		return;
	}
	lExporter->Export(scene);												// Export scene
	lExporter->Destroy();


	if (exportAscii)												//test to have the Ascci version, that could help.
	{
		int lFileFormat_Ascii = sdk_manager->GetIOPluginRegistry()->FindWriterIDByDescription("FBX ascii (*.fbx)");

		FbxExporter* lExporter = FbxExporter::Create(sdk_manager, "");
		bool lExportStatus = lExporter->Initialize((export_filename.substr(0, export_filename.length() - 4) + "_Ascii.fbx").c_str(), lFileFormat_Ascii, sdk_manager->GetIOSettings());
		if (lExportStatus)
		{
			lExporter->Export(scene);												// Export scene
			lExporter->Destroy();
		}
	}

	QMessageBox::about(NULL, "Collision Voxel Export", "Fbx Export Done (" + QString(Ogre::StringConverter::toString(mListVoxels.size()).c_str()) + " voxels)" );
}




void CollisionMaker::exportVoxel_Havok(string basename)
{
	size_t nbVoxels = mListVoxels.size();
	if (nbVoxels == 0)
	{
		QMessageBox::about(NULL, "Collision Voxel Export", "No Voxels created. use BuildVoxel when there is a emd file loaded.");
		return;
	}

	if (basename.length() == 0)
		basename = "Voxels";

	string name = nameFromFilenameNoExtension(basename);

	char buf_tmp[MAX_PATH + 1];
	GetModuleFileName(NULL, buf_tmp, MAX_PATH);
	string exeFilename = string(buf_tmp);
	string exePath = LibXenoverse::folderFromFilename(exeFilename);


	//we have to load the texte file , template for box.
	char *buf;
	size_t size;
	buf = (char*)LibXenoverse::EMO_BaseFile::ReadFile(exePath + "/resources/box_template.hkx.xml", &size, true);
	if (!buf)
	{
		printf("Error loading file : resources/box_template.hkx.xml");
		LibXenoverse::notifyError();
		QMessageBox::about(NULL, "Collision Voxel Export", "Fail to load /resources/box_template.hkx.xml. Export Fail.");
		return;
	}
	string xmlBoxTemplate = string(buf);
	delete[] buf;








	//first try to get the MinMax
	Ogre::Vector3 fullWidth = Ogre::Vector3(mVoxelSize, mVoxelSize, mVoxelSize);
	Ogre::Vector3 halfWidth = Ogre::Vector3(mVoxelSize / 2.0, mVoxelSize / 2.0, mVoxelSize / 2.0);
	double eps = 0.00001;

	string listHitboxXmlCouple_str = "";
	string listHitboxXml_str = "";

	OgreVoxel* vox;
	Ogre::Vector3 pos; Ogre::Vector3 scale; Ogre::Vector3 width;
	for (size_t i = 0; i < nbVoxels; i++)
	{
		vox = mListVoxels.at(i);

		pos = vox->mPosition;
		width = vox->mScale;
		Ogre::AxisAlignedBox aabb_submesh(pos, pos + width);
		Ogre::AxisAlignedBox aabb_submesh_eps(pos - Ogre::Vector3(eps, eps, eps), pos + width + Ogre::Vector3(eps, eps, eps));

		string xmlBoxTemplate_tmp = xmlBoxTemplate;
		replaceAll(xmlBoxTemplate_tmp, "template_Xmin_sub_eps", FloatToString(-(float)aabb_submesh_eps.getMaximum().x));		//because of names to replace, keep longer first.
		replaceAll(xmlBoxTemplate_tmp, "template_Ymin_sub_eps", FloatToString((float)aabb_submesh_eps.getMinimum().y));
		replaceAll(xmlBoxTemplate_tmp, "template_Zmin_sub_eps", FloatToString((float)aabb_submesh_eps.getMinimum().z));
		replaceAll(xmlBoxTemplate_tmp, "template_Xmax_add_eps", FloatToString(-(float)aabb_submesh_eps.getMinimum().x));
		replaceAll(xmlBoxTemplate_tmp, "template_Ymax_add_eps", FloatToString((float)aabb_submesh_eps.getMaximum().y));
		replaceAll(xmlBoxTemplate_tmp, "template_Zmax_add_eps", FloatToString((float)aabb_submesh_eps.getMaximum().z));
		replaceAll(xmlBoxTemplate_tmp, "template_Xmin", FloatToString(-(float)aabb_submesh.getMaximum().x));
		replaceAll(xmlBoxTemplate_tmp, "template_Ymin", FloatToString((float)aabb_submesh.getMinimum().y));
		replaceAll(xmlBoxTemplate_tmp, "template_Zmin", FloatToString((float)aabb_submesh.getMinimum().z));
		replaceAll(xmlBoxTemplate_tmp, "template_Xmax", FloatToString(-(float)aabb_submesh.getMinimum().x));
		replaceAll(xmlBoxTemplate_tmp, "template_Ymax", FloatToString((float)aabb_submesh.getMaximum().y));
		replaceAll(xmlBoxTemplate_tmp, "template_Zmax", FloatToString((float)aabb_submesh.getMaximum().z));


		string newName = name + "_" + Ogre::StringConverter::toString(i) + ".hkx";
		string newfilename = basename + "_" + Ogre::StringConverter::toString(i) + ".hkx";

		/*
		listHitboxXml_str += "\n\
					<Destruction>\n\
						<unk_0 u32=\"0x3\" />\n\
						<unk_1 u32=\"0xc0\" />\n\
						<unk_2 u32=\"0x0\" />\n\
						<unk_5 u32=\"0x0\" />\n\
						<unk_6 u32=\"0x40\" />\n\
						<unk_7 float=\"0.00999999978\" />\n\
						<DestructionSubPart />\n\
						<HavokFile filename=\"" + newName + "\" />\n\
					</Destruction>\n\
				";
		*/


		//you can walk on this.
		listHitboxXml_str += "\n\
					<Destruction>\n\
						<unk_0 u32=\"0xd\" />\n\
						<unk_1 u32=\"0x60\" />\n\
						<unk_2 u32=\"0x0\" />\n\
						<unk_5 u32=\"0xc0\" />\n\
						<unk_6 u32=\"0x20\" />\n\
						<unk_7 float=\"0.00999999978\" />\n\
						<DestructionSubPart />\n\
						<HavokFile filename=\"" + newName + "\" />\n\
					</Destruction>\n\
				";

		/*
		//you fly up to this one 
		listHitboxXml_str += "\n\
					<Destruction>\n\
						<unk_0 u32=\"0x10\" />\n\
						<unk_1 u32=\"0x1b0\" />\n\
						<unk_2 u32=\"0x0\" />\n\
						<unk_5 u32=\"0x0\" />\n\
						<unk_6 u32=\"0x90\" />\n\
						<unk_7 float=\"0.00999999978\" />\n\
						<DestructionSubPart />\n\
						<HavokFile filename=\"" + newName + "\" />\n\
					</Destruction>\n\
				";
		*/

		listHitboxXmlCouple_str += "<IndexCouple Index0 = \"" + std::to_string(i) + "\" Index1 = \"4\" / >\n";

		TiXmlDocument doc;
		if (!doc.Parse(xmlBoxTemplate_tmp.c_str()))
		{
			LibXenoverse::notifyError();
			continue;
		}

		LibXenoverse::Havok *havok = new LibXenoverse::Havok();
		if (!havok->Compile(&doc))
		{
			printf("Error on parsing havok");
			LibXenoverse::notifyError();
			LibXenoverse::waitOnEnd();
		}

		//havok->importFBX(fbxNode);				//by the name could update values on a Object hierarchy. => TODO check
		havok->SaveToFile(newfilename);
		delete havok;
	}

	



	listHitboxXml_str = "\n\
	<ListIndexCouple>\n\
			" + listHitboxXmlCouple_str + "\n\
	</ListIndexCouple>\n\
	\n\
	\n\
	\n\
	<Hitbox>\n\
		<name value = \"TodoChangeName\" />\n\
		<child u16 = \"0xffff\" />\n\
		<unk_a0 u16 = \"0xffff\" />\n\
		<sibling u16 = \"0x3\" />\n\
		<parent u16 = \"0x0\" />\n\
		<ListOnDestruction>\n\
			<DestructionGroup>\n\
			" + listHitboxXml_str + "\n\
			</DestructionGroup>\n\
		</ ListOnDestruction>\n\
		<ListHavokFiles />\n\
		<CollisionGeometry filename=\"\">\n\
		</ CollisionGeometry>\n\
	</ Hitbox>\
		";

	LibXenoverse::EMO_BaseFile::WriteFile(basename + "_extract_map.xml", (const uint8_t*)listHitboxXml_str.c_str(), listHitboxXml_str.length() + 1, true, false);

	QMessageBox::about(NULL, "Collision Voxel Export", "Havok Export Done (" + QString(Ogre::StringConverter::toString(mListVoxels.size()).c_str()) + " voxels)");
}




EMD*  CollisionMaker::exportVoxelEmd(string basename)
{
	size_t nbVoxels = mListVoxels.size();
	if (nbVoxels == 0)
	{
		QMessageBox::about(NULL, "Collision Voxel Export", "No Voxels created. use BuildVoxel when there is a emd file loaded.");
		return 0;
	}

	if (basename.length() == 0)
		basename = "Voxels";

	EMD* emd = new EMD();
	emd->setName(basename +".emd");

	EMDModel* emdModel = new EMDModel();
	emd->getModel().push_back(emdModel);
	emdModel->setName(basename);

	EMDMesh* emdMesh = new EMDMesh();
	emdModel->getMeshes().push_back(emdMesh);
	emdMesh->setName(basename);


	//first try to get the MinMax
	Ogre::Vector3 fullWidth = Ogre::Vector3(mVoxelSize, mVoxelSize, mVoxelSize);
	Ogre::Vector3 halfWidth = Ogre::Vector3(mVoxelSize / 2.0, mVoxelSize / 2.0, mVoxelSize / 2.0);

	Ogre::Vector3 pos = mListVoxels.at(0)->mPosition;
	Ogre::AxisAlignedBox aabb(pos, (pos + fullWidth));


	Ogre::Vector3 scale;
	Ogre::Vector3 width;
	for (size_t i = 0; i < nbVoxels; i++)
	{
		OgreVoxel* vox = mListVoxels.at(i);

		EMDSubmesh* emdSubMesh = new EMDSubmesh();
		emdMesh->getSubmeshes().push_back(emdSubMesh);
		emdSubMesh->setMaterialName("Box_" + Ogre::StringConverter::toString(i) );
		emdSubMesh->setVertexTypeFlags(EMD_VTX_FLAG_POS | EMD_VTX_FLAG_NORM | EMD_VTX_FLAG_TEX);

		EMDTriangles triangle;

		pos = vox->mPosition;
		width = vox->mScale;
		Ogre::AxisAlignedBox aabb_submesh(pos, pos + width);
		emdSubMesh->setAABB(-aabb_submesh.getCenter().x, aabb_submesh.getCenter().y, aabb_submesh.getCenter().z, 1.0f, -aabb_submesh.getMaximum().x, aabb_submesh.getMinimum().y, aabb_submesh.getMinimum().z, 1.0f, -aabb_submesh.getMinimum().x, aabb_submesh.getMaximum().y, aabb_submesh.getMaximum().z, 1.0);
		aabb.merge(aabb_submesh);


		EMDVertex v; v.flags = emdSubMesh->getVertexTypeFlags();

		v.norm_x = 0.0f; v.norm_y = -1.0f; v.norm_z = 0.0f;
		v.pos_x = -(float)pos.x;				v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z + width.z);	v.text_u = 1.0f;	v.text_v = 1.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)pos.x;				v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z);			v.text_u = 1.0f;	v.text_v = 0.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)(pos.x + width.x);	v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z);			v.text_u = 0.0f;	v.text_v = 0.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)(pos.x + width.x);	v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z + width.z);	v.text_u = 0.0f;	v.text_v = 1.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));

		v.norm_x = 0.0f; v.norm_y = 1.0f;  v.norm_z = 0.0f;
		v.pos_x = -(float)pos.x;				v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z + width.z);	v.text_u = 0.0f;	v.text_v = 1.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)(pos.x + width.x);	v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z + width.z);	v.text_u = 1.0f;	v.text_v = 1.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)(pos.x + width.x);	v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z);			v.text_u = 1.0f;	v.text_v = 0.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)(pos.x);				v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z);			v.text_u = 0.0f;	v.text_v = 0.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));

		v.norm_x = 0.0f; v.norm_y = 0.0f;  v.norm_z = 1.0f;
		v.pos_x = -(float)pos.x;				v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z + width.z);	v.text_u = 0.0f;	v.text_v = 1.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)(pos.x + width.x);	v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z + width.z);	v.text_u = 1.0f;	v.text_v = 1.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)(pos.x + width.x);	v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z + width.z);	v.text_u = 1.0f;	v.text_v = 0.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)(pos.x);				v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z + width.z);	v.text_u = 0.0f;	v.text_v = 0.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));

		v.norm_x = -1.0f; v.norm_y = 0.0f;  v.norm_z = 0.0f;
		v.pos_x = -(float)(pos.x + width.x);	v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z + width.z);	v.text_u = 0.0f;	v.text_v = 1.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)(pos.x + width.x);	v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z);			v.text_u = 1.0f;	v.text_v = 1.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)(pos.x + width.x);	v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z);			v.text_u = 1.0f;	v.text_v = 0.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)(pos.x + width.x);	v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z + width.z);	v.text_u = 0.0f;	v.text_v = 0.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));

		v.norm_x = 0.0f; v.norm_y = 0.0f;		v.norm_z = -1.0f;
		v.pos_x = -(float)(pos.x + width.x);	v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z);			v.text_u = 0.0f;	v.text_v = 1.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)(pos.x);				v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z);			v.text_u = 1.0f;	v.text_v = 1.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)(pos.x);				v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z);			v.text_u = 1.0f;	v.text_v = 0.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)(pos.x + width.x);	v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z);			v.text_u = 0.0f;	v.text_v = 0.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));

		v.norm_x = 1.0f; v.norm_y = 0.0f;  v.norm_z = 0.0f;
		v.pos_x = -(float)(pos.x);				v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z);			v.text_u = 0.0f;	v.text_v = 1.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)(pos.x);				v.pos_y = (float)pos.y;				v.pos_z = (float)(pos.z + width.z);	v.text_u = 1.0f;	v.text_v = 1.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)(pos.x);				v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z + width.z);	v.text_u = 1.0f;	v.text_v = 0.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));
		v.pos_x = -(float)(pos.x);				v.pos_y = (float)(pos.y + width.y);	v.pos_z = (float)(pos.z);			v.text_u = 0.0f;	v.text_v = 0.0f;	emdSubMesh->getVertices().push_back(EMDVertex(&v));


		
		triangle.faces.push_back(0); triangle.faces.push_back(2); triangle.faces.push_back(1);
		triangle.faces.push_back(2); triangle.faces.push_back(0); triangle.faces.push_back(3);
		triangle.faces.push_back(4); triangle.faces.push_back(6); triangle.faces.push_back(5);
		triangle.faces.push_back(6); triangle.faces.push_back(4); triangle.faces.push_back(7);
		triangle.faces.push_back(8); triangle.faces.push_back(10); triangle.faces.push_back(9);
		triangle.faces.push_back(10); triangle.faces.push_back(8); triangle.faces.push_back(11);
		triangle.faces.push_back(12); triangle.faces.push_back(14); triangle.faces.push_back(13);
		triangle.faces.push_back(14); triangle.faces.push_back(12); triangle.faces.push_back(15);
		triangle.faces.push_back(16); triangle.faces.push_back(18); triangle.faces.push_back(17);
		triangle.faces.push_back(18); triangle.faces.push_back(16); triangle.faces.push_back(19);
		triangle.faces.push_back(20); triangle.faces.push_back(22); triangle.faces.push_back(21);
		triangle.faces.push_back(22); triangle.faces.push_back(20); triangle.faces.push_back(23);
		emdSubMesh->getTriangles().push_back(triangle);
	}
	emdMesh->setAABB(-aabb.getCenter().x, aabb.getCenter().y, aabb.getCenter().z, 1.0f, -aabb.getMaximum().x, aabb.getMinimum().y, aabb.getMinimum().z, 1.0f, -aabb.getMinimum().x, aabb.getMaximum().y, aabb.getMaximum().z, 1.0);

	return emd;
}




/*
void CollisionMaker::update(std::vector<string> &listSelectedBoneNames)
{

	mSceneMgr = sceneMgr;
	mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();

	mSceneNode->setScale(-1, 1, 1);				//dbvx symetry. TODO REMETTRE


	//Axes
	mSceneNode_Axes = mSceneNode->createChildSceneNode();
	mAxesEntity = sceneMgr->createEntity("Repere.mesh");
	mSceneNode_Axes->attachObject(mAxesEntity);

	bool isLeftBone = (bone->getName().find("_L_") != string::npos);

	Ogre::SceneNode* node = NULL;
	Ogre::Entity* ent = NULL;

	size_t numChildren = bone->numChildren();
	if (numChildren == 0)
	{
		node = mSceneNode->createChildSceneNode();
		node->setUseNoUniformScale(true);
		ent = sceneMgr->createEntity("r.mesh");
		node->attachObject(ent);
		node->setScale(Ogre::Vector3::UNIT_SCALE * 0.01f);
		mListChild_SceneNode.push_back(node);
	}else{
				
		Ogre::Vector3 pos;
		for (size_t i = 0; i < numChildren; ++i)
		{
			pos = bone->getChild(i)->getPosition();
					
			node = mSceneNode->createChildSceneNode();
			node->setUseNoUniformScale(true);
			ent = sceneMgr->createEntity("r.mesh");
			node->attachObject(ent);

			if (isLeftBone)
				ent->setMaterialName("Yellow");

					
					
			Ogre::Matrix4 transform = bone->_getFullTransform();
			Ogre::Vector3 position;
			Ogre::Quaternion orientation;
			Ogre::Vector3 scale;
			transform.decomposition(position, scale, orientation);



			node->setPosition(position);
			node->setScale(Ogre::Vector3(0.01f, pos.length(), 0.01f));


			Ogre::Vector3 pos_norm = pos.normalisedCopy();

			Ogre::Quaternion quad = Ogre::Quaternion::IDENTITY;
			if (Ogre::Vector3::UNIT_Y != pos_norm)
			{
				Ogre::Vector3 normal = pos_norm.crossProduct(Ogre::Vector3::UNIT_Y);
				Ogre::Real angle = Ogre::Math::ACos(pos_norm.dotProduct(Ogre::Vector3::UNIT_Y)).valueDegrees();

				if (Ogre::Vector3::UNIT_Y.crossProduct(normal).dotProduct(pos_norm)<0)
					angle *= -1.0;

				quad = Ogre::Quaternion(Ogre::Degree(angle), normal);
			}
			node->setOrientation(quad);		//r.mesh have center on the middle of the bottom face but it's Xaxe the source of bones.	notice: rotation 180 because of symetry

			mListChild_SceneNode.push_back(node);
		}
	}

	~OgreVoxel(void)
		{
			mSceneMgr->destroySceneNode(mSceneNode);
			mSceneMgr->destroySceneNode(mSceneNode_Axes);
			mSceneMgr->destroyEntity(mAxesEntity);


			size_t nbNode = mListChild_SceneNode.size();
			for (size_t i = 0; i < nbNode; i++)
			{
				mSceneMgr->destroyEntity( (Ogre::Entity*)mListChild_SceneNode.at(i)->getAttachedObject(0) );
				mSceneMgr->destroySceneNode(mListChild_SceneNode.at(i));
			}
		}





		void	update(bool isSelected)
		{
			mBone->_update(true, true);

			Ogre::Matrix4 transform = mBone->_getFullTransform();

			Ogre::Vector3 position;
			Ogre::Quaternion orientation;
			Ogre::Vector3 scale;

			transform.decomposition(position, scale, orientation);

			mSceneNode_Axes->setPosition(position);
			mSceneNode_Axes->setOrientation(orientation);
			mSceneNode_Axes->setScale(scale);





			size_t numChildren = mBone->numChildren();
			if (numChildren==0)
			{
				mListChild_SceneNode.back()->setPosition(position);
				mListChild_SceneNode.back()->setOrientation(orientation);
				mListChild_SceneNode.back()->setScale(scale * 0.01f);
				return;
			}


			Ogre::Vector3 pos;
			Ogre::Quaternion orient;
			for (size_t i = 0; i < numChildren; ++i)
			{
				Ogre::Node* bone = mBone->getChild(i);

				Ogre::Matrix4 transform_b = bone->_getFullTransform();
				transform_b.decomposition(pos, scale, orient);

				Ogre::Node* node = mListChild_SceneNode.at(i);
				node->setPosition(position);

				Ogre::Vector3 diff_pos = pos - position;
				node->setScale(Ogre::Vector3(0.01f, diff_pos.length(), 0.01f));

				Ogre::Vector3 pos_norm = diff_pos.normalisedCopy();
				Ogre::Quaternion quad = Ogre::Quaternion::IDENTITY;
				if (Ogre::Math::Abs(1 - Ogre::Math::Abs((pos_norm.dotProduct(Ogre::Vector3::UNIT_Y))) > 0.001)&&(pos_norm.length() > 0.001))
				{
					Ogre::Vector3 normal = pos_norm.crossProduct(Ogre::Vector3::UNIT_Y);

					quad = Ogre::Quaternion(normal, pos_norm, normal.crossProduct(pos_norm));
				}
				node->setOrientation(quad);		//r.mesh have center on the middle of the bottom face but it's Xaxe the source of bones.	notice: rotation 180 because of symetry
			}


		}



	//TODO solve problem after remove in tree of first item (witchi is the source Entity to find Skeleton). 

	//has manualObject AND tagPoint didn't work on my situation, I will use lodaed meshes and update position manually.
	string name;
	size_t nbBoneDebug = mListBoneDebug.size();
	size_t nbSelected = listSelectedBoneNames.size();
	for (size_t i = 0; i < nbBoneDebug; i++)
	{
		name = mListBoneDebug.at(i)->mName;
		
		
		bool isfound = false;
		for (size_t j = 0; j < nbSelected; j++)
		{
			if (name == listSelectedBoneNames.at(j))
			{
				isfound = true;
				break;
			}
		}
		mListBoneDebug.at(i)->update(isfound);
	}
}
*/
