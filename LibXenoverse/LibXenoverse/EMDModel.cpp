namespace LibXenoverse
{
	





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
void EMDModel::read(File *file)
{
	unsigned int base_model_address = file->getCurrentAddress();
	unsigned int address = 0;

	unsigned short mesh_total = 0;
	unsigned int mesh_table_address = 0;
	file->readInt16E(&unknown_total);
	file->readInt16E(&mesh_total);
	file->readInt32E(&mesh_table_address);

	LOG_DEBUG("\n\nModel - Mesh Count: %d\n", mesh_total);

	for (size_t j = 0; j < mesh_total; j++)
	{
		file->goToAddress(base_model_address + mesh_table_address + j * 4);
		file->readInt32E(&address);

		unsigned int base_mesh_address = base_model_address + address;
		file->goToAddress(base_mesh_address);

		EMDMesh *emd_mesh = new EMDMesh();
		emd_mesh->read(file);
		meshes.push_back(emd_mesh);
	}
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void EMDModel::write(File *file)
{
	unsigned int base_model_address = file->getCurrentAddress();
	unsigned short mesh_total = meshes.size();
	unsigned int mesh_table_address = 8;
	file->writeInt16E(&unknown_total);
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
		meshes[j]->write(file);
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



}