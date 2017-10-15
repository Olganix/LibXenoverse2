#ifndef EMMOGRE_H_INCLUDED
#define EMMOGRE_H_INCLUDED

class EMBOgre;

class EMMOgre : public EMM
{
protected:
	bool material_resources_created;
	EMBOgre *texture_pack;
	EMBOgre *texture_dyt_pack;

	std::vector<Ogre::String> created_materials;
	std::vector<std::vector<size_t>> created_materials_reg_for_textTile;
public:
	EMMOgre();
	~EMMOgre();

	std::vector<size_t> setUpMaterialParameters(string shader_name, Ogre::GpuProgramParametersSharedPtr fp_parameters, Ogre::Pass* pass, EMMMaterial *emm_material);
	Ogre::Material *createOgreMaterial(EMMMaterial *emm_material, std::vector<SDS*> &sds_list);
	void createOgreMaterials(std::vector<SDS*> &sds_list);

	size_t getReg(string ogreMaterialName, size_t textTileIndex);

	void setTexturePack(EMBOgre *v) { texture_pack = v; }
	void setDYTTexturePack(EMBOgre *v) { texture_dyt_pack = v; }
	EMBOgre *getTexturePack() { return texture_pack; }
	EMBOgre *getDYTTexturePack() { return texture_dyt_pack; }

	void destroyResources();
};

#endif