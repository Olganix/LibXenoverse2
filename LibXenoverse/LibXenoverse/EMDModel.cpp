namespace LibXenoverse
{
	



/*-------------------------------------------------------------------------------\
|                             EMDModel				                             |
\-------------------------------------------------------------------------------*/
EMDModel::EMDModel(EMDModel* emdModel)
{
	name = "";
	unknow_0 = 0;

	if (emdModel)
	{
		name = emdModel->name;
		unknow_0 = emdModel->unknow_0;

		for (size_t i = 0, nb = emdModel->meshes.size(); i < nb; i++)
			meshes.push_back(new EMDMesh(emdModel->meshes.at(i)));
	}
}
/*-------------------------------------------------------------------------------\
|                             EMDModel				                             |
\-------------------------------------------------------------------------------*/
EMDModel::~EMDModel(void)
{
	size_t nbMesh = meshes.size();
	for (size_t i = 0; i < nbMesh; i++)
		delete meshes.at(i);
	meshes.clear();
}



/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void EMDModel::read(File *file, uint16_t &paddingForCompressedVertex)
{
	unsigned int base_model_address = file->getCurrentAddress();
	unsigned int address = 0;

	unsigned short mesh_total = 0;
	unsigned int mesh_table_address = 0;
	file->readInt16E(&unknow_0);
	file->readInt16E(&mesh_total);
	file->readInt32E(&mesh_table_address);

	for (size_t j = 0; j < mesh_total; j++)
	{
		file->goToAddress(base_model_address + mesh_table_address + j * 4);
		file->readInt32E(&address);

		unsigned int base_mesh_address = base_model_address + address;
		file->goToAddress(base_mesh_address);

		EMDMesh *emd_mesh = new EMDMesh();
		emd_mesh->read(file, paddingForCompressedVertex);
		meshes.push_back(emd_mesh);
	}
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void EMDModel::write(File *file, uint16_t paddingForCompressedVertex)
{
	unsigned int base_model_address = file->getCurrentAddress();
	unsigned short mesh_total = meshes.size();
	unsigned int mesh_table_address = 8;
	file->writeInt16E(&unknow_0);
	file->writeInt16E(&mesh_total);
	file->writeInt32E(&mesh_table_address);

	file->writeNull(mesh_total * 4);
		
	for (size_t j = 0; j < mesh_total; j++)
	{
		file->fixPadding(16);
		unsigned int mesh_address = file->getCurrentAddress() - base_model_address;
		file->goToAddress(base_model_address + mesh_table_address + j * 4);
		file->writeInt32E(&mesh_address);
		file->goToAddress(base_model_address + mesh_address);
		meshes[j]->write(file, paddingForCompressedVertex);
	}
}
/*-------------------------------------------------------------------------------\
|                             setVertexScale		                             |
\-------------------------------------------------------------------------------*/
void EMDModel::setVertexScale(float scale)
{
	for (size_t i = 0; i < meshes.size(); i++)
		meshes[i]->setVertexScale(scale);
}
/*-------------------------------------------------------------------------------\
|                             getMeshNames			                             |
\-------------------------------------------------------------------------------*/
vector<string> EMDModel::getMeshNames(void)
{
	vector<string> names;
	for (size_t i = 0; i < meshes.size(); i++)
		names.push_back(meshes[i]->getName());
	return names;
}
/*-------------------------------------------------------------------------------\
|                             getBonesNames									     |
\-------------------------------------------------------------------------------*/
void EMDModel::getBonesNames(vector<string> &bones_names)
{
	for (size_t i = 0; i < meshes.size(); i++)
		meshes[i]->getBonesNames(bones_names);
}
/*-------------------------------------------------------------------------------\
|                             replaceBonesNames									 |
\-------------------------------------------------------------------------------*/
void EMDModel::replaceBonesNames(const string &oldName, const string &newName)
{
	for (size_t i = 0; i < meshes.size(); i++)
		meshes[i]->replaceBonesNames(oldName, newName);
}
/*-------------------------------------------------------------------------------\
|                             mergeTriangles									 |
\-------------------------------------------------------------------------------*/
void EMDModel::mergeTriangles()
{
	for (size_t i = 0; i < meshes.size(); i++)
		meshes[i]->mergeTriangles();
}



/*-------------------------------------------------------------------------------\
|                             useTextureDefTemplate								 |
\-------------------------------------------------------------------------------*/
void EMDModel::useTextureDefTemplate(EMDModel* other)
{
	string name = "";
	for (size_t i = 0; i < meshes.size(); i++)
	{
		name = meshes.at(i)->getName();

		for (size_t j = 0; j < other->meshes.size(); j++)
		{
			if (other->meshes.at(j)->getName() == name)
			{
				meshes.at(i)->useTextureDefTemplate(other->meshes.at(j));
				break;
			}
		}
	}
}



}