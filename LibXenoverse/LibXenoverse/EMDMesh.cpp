namespace LibXenoverse
{


/*-------------------------------------------------------------------------------\
|                             EMDMesh				                             |
\-------------------------------------------------------------------------------*/
EMDMesh::EMDMesh(EMDMesh* emdMesh)
{
	zero();

	if (emdMesh)
	{
		name = emdMesh->name;
		unknow_0 = emdMesh->unknow_0;
		
		aabb_center_x = emdMesh->aabb_center_x;
		aabb_center_y = emdMesh->aabb_center_y;
		aabb_center_z = emdMesh->aabb_center_z;
		aabb_center_w = emdMesh->aabb_center_w;
		aabb_min_x = emdMesh->aabb_min_x;
		aabb_min_y = emdMesh->aabb_min_y;
		aabb_min_z = emdMesh->aabb_min_z;
		aabb_min_w = emdMesh->aabb_min_w;
		aabb_max_x = emdMesh->aabb_max_x;
		aabb_max_y = emdMesh->aabb_max_y;
		aabb_max_z = emdMesh->aabb_max_z;
		aabb_max_w = emdMesh->aabb_max_w;
		
		for (size_t i = 0, nb = emdMesh->submeshes.size(); i < nb; i++)
			submeshes.push_back(new EMDSubmesh(emdMesh->submeshes.at(i)));
	}
}
/*-------------------------------------------------------------------------------\
|                             EMDMesh				                             |
\-------------------------------------------------------------------------------*/
EMDMesh::~EMDMesh(void)
{
	size_t nbSubMesh = submeshes.size();
	for (size_t i = 0; i < nbSubMesh; i++)
		delete submeshes.at(i);
	submeshes.clear();
}
/*-------------------------------------------------------------------------------\
|                             zero					                             |
\-------------------------------------------------------------------------------*/
void EMDMesh::zero(void)
{
	aabb_center_x = aabb_center_y = aabb_center_z = aabb_center_w = aabb_min_x = aabb_min_y = aabb_min_z = aabb_min_w = aabb_max_x = aabb_max_y = aabb_max_z = aabb_max_w = 0.0f;
	unknow_0 = 0;
	name = "";
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void EMDMesh::read(File *file, uint16_t &paddingForCompressedVertex)
{
	unsigned int address = 0;
	unsigned int base_mesh_address = file->getCurrentAddress();

	file->readFloat32E(&aabb_center_x);
	file->readFloat32E(&aabb_center_y);
	file->readFloat32E(&aabb_center_z);
	file->readFloat32E(&aabb_center_w);
	file->readFloat32E(&aabb_min_x);
	file->readFloat32E(&aabb_min_y);
	file->readFloat32E(&aabb_min_z);
	file->readFloat32E(&aabb_min_w);
	file->readFloat32E(&aabb_max_x);
	file->readFloat32E(&aabb_max_y);
	file->readFloat32E(&aabb_max_z);
	file->readFloat32E(&aabb_max_w);

	unsigned int mesh_name_offset = 0;
	unsigned short submesh_total = 0;
	unsigned int submesh_table_address = 0;
	file->readInt32E(&mesh_name_offset);
	file->readInt16E(&unknow_0);
	file->readInt16E(&submesh_total);
	file->readInt32E(&submesh_table_address);

	file->goToAddress(base_mesh_address + mesh_name_offset);
	file->readString(&name);

	for (size_t k = 0; k < submesh_total; k++)
	{
		file->goToAddress(base_mesh_address + submesh_table_address + k * 4);
		file->readInt32E(&address);
		file->goToAddress(base_mesh_address + address);

		unsigned int base_submesh_address = base_mesh_address + address;
		file->goToAddress(base_submesh_address);

		EMDSubmesh *emd_submesh = new EMDSubmesh();
		emd_submesh->read(file, paddingForCompressedVertex);
		submeshes.push_back(emd_submesh);
	}
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void EMDMesh::write(File *file, uint16_t paddingForCompressedVertex)
{
	unsigned int base_mesh_address = file->getCurrentAddress();

	file->writeFloat32E(&aabb_center_x);
	file->writeFloat32E(&aabb_center_y);
	file->writeFloat32E(&aabb_center_z);
	file->writeFloat32E(&aabb_center_w);
	file->writeFloat32E(&aabb_min_x);
	file->writeFloat32E(&aabb_min_y);
	file->writeFloat32E(&aabb_min_z);
	file->writeFloat32E(&aabb_min_w);
	file->writeFloat32E(&aabb_max_x);
	file->writeFloat32E(&aabb_max_y);
	file->writeFloat32E(&aabb_max_z);
	file->writeFloat32E(&aabb_max_w);

	unsigned int mesh_name_offset = 0;
	unsigned short submesh_total = submeshes.size();
	unsigned int submesh_table_address = 0;
	file->writeNull(12);

	// Write Name
	mesh_name_offset = file->getCurrentAddress() - base_mesh_address;
	file->writeString(&name);
	file->fixPadding(0x4);

	// Write Submeshes
	submesh_table_address = file->getCurrentAddress() - base_mesh_address;
	file->writeNull(submesh_total * 4);
		
	for (size_t k = 0; k < submesh_total; k++)
	{
		file->fixPadding(16);
		unsigned int submesh_address = file->getCurrentAddress() - base_mesh_address;
		file->goToAddress(base_mesh_address + submesh_table_address + k * 4);
		file->writeInt32E(&submesh_address);
		file->goToAddress(base_mesh_address + submesh_address);
		submeshes[k]->write(file, paddingForCompressedVertex);
	}

	// Fix Offsets
	file->goToAddress(base_mesh_address + 0x30);
	file->writeInt32E(&mesh_name_offset);
	file->writeInt16E(&unknow_0);
	file->writeInt16E(&submesh_total);
	file->writeInt32E(&submesh_table_address);
	file->goToEnd();
}
/*-------------------------------------------------------------------------------\
|                             setVertexScale		                             |
\-------------------------------------------------------------------------------*/
void EMDMesh::setVertexScale(float scale)
{
	for (size_t i = 0; i < submeshes.size(); i++)
		submeshes[i]->setVertexScale(scale);
}
/*-------------------------------------------------------------------------------\
|                             getBonesNames						                 |
\-------------------------------------------------------------------------------*/
void EMDMesh::getBonesNames(vector<string> &bones_names)
{
	for (size_t i = 0; i < submeshes.size(); i++)
		submeshes[i]->getBonesNames(bones_names);
}
/*-------------------------------------------------------------------------------\
|                             replaceBonesNames				                     |
\-------------------------------------------------------------------------------*/
void EMDMesh::replaceBonesNames(const string &oldName, const string &newName)
{
	for (size_t i = 0; i < submeshes.size(); i++)
		submeshes[i]->replaceBonesNames(oldName, newName);
}
/*-------------------------------------------------------------------------------\
|                             mergeTriangles				                     |
\-------------------------------------------------------------------------------*/
void EMDMesh::mergeTriangles()
{
	for (size_t i = 0; i < submeshes.size(); i++)
		submeshes[i]->mergeTriangles();
}

/*-------------------------------------------------------------------------------\
|                             useTextureDefTemplate								 |
\-------------------------------------------------------------------------------*/
void EMDMesh::useTextureDefTemplate(EMDMesh* other)
{
	string name = "";
	for (size_t i = 0; i < submeshes.size(); i++)
	{
		name = submeshes.at(i)->getMaterialName();

		for (size_t j = 0; j < other->submeshes.size(); j++)
		{
			if (other->submeshes.at(j)->getMaterialName() == name)
			{
				submeshes.at(i)->useTextureDefTemplate(other->submeshes.at(j));
				break;
			}
		}
	}
}


}