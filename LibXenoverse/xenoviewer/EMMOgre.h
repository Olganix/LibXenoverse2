#ifndef EMMOGRE_H_INCLUDED
#define EMMOGRE_H_INCLUDED

class EMBOgre;

class EMMOgre : public EMM
{
public:

	// shaders are compilated with loosing name of parameter, only register (we get them by look at the source)
	// So, for futur live editions, we need to keep the link between name of parameter and the register.
	struct EmmMaterialParameter
	{
		string name;
		string type;
		size_t reg;
		string shaderType;
		Ogre::Vector4 defaultValue;			// limited to under float5 parameters
		Ogre::Vector4 currentValue;

		EmmMaterialParameter(string name, string type, size_t reg, string shaderType, Ogre::Vector4 defaultValue = Ogre::Vector4::ZERO) { this->name = name; this->type = type; this->reg = reg; this->shaderType = shaderType; this->defaultValue = defaultValue; currentValue = defaultValue; }
		EmmMaterialParameter(const EmmMaterialParameter& other) { this->name = other.name; this->type = other.type; this->reg = other.reg; this->shaderType = other.shaderType; this->defaultValue = other.defaultValue; currentValue = defaultValue; }
		void	operator=(const EmmMaterialParameter& other) { this->name = other.name; this->type = other.type; this->reg = other.reg; this->shaderType = other.shaderType; this->defaultValue = other.defaultValue; currentValue = defaultValue; }
	};

	struct EmmMaterialCreated
	{
		string name;
		std::vector<EmmMaterialParameter> parameter;

		EmmMaterialCreated(string name, std::vector<EmmMaterialParameter> parameter) { this->name = name; this->parameter = parameter; }
	};

protected:
	bool material_resources_created;
	EMBOgre *texture_pack;
	EMBOgre *texture_dyt_pack;

	std::vector<EmmMaterialCreated> created_materials;
	bool emdPartVisible;

public:
	EMMOgre();
	~EMMOgre();

	bool	getEmdPartVisible() { return emdPartVisible; }
	void	setEmdPartVisible(bool visible) { emdPartVisible = visible; }

	std::vector<size_t> setUpMaterialParameters(string shader_name, Ogre::GpuProgramParametersSharedPtr fp_parameters, Ogre::Pass* pass, EMMMaterial *emm_material, std::vector<EmmMaterialParameter> &materialParameters, string shaderType);
	Ogre::Material *createOgreMaterial(EMMMaterial *emm_material, std::vector<SDS*> &sds_list);
	void createOgreMaterials(std::vector<SDS*> &sds_list);

	std::vector<EmmMaterialParameter>*	getShaderParameters(string ogreMaterialName);
	EmmMaterialParameter	getShaderParameter(string ogreMaterialName, string parameterName);
	bool	setShaderParameter(string ogreMaterialName, string name, bool value);
	bool	setShaderParameter(string ogreMaterialName, string name, float value);
	bool	setShaderParameter(string ogreMaterialName, string name, Ogre::Vector3 value);
	bool	setShaderParameter(string ogreMaterialName, string name, Ogre::Vector4 value);
	bool	setShaderParameter(string ogreMaterialName, string name, string value);
	void	setShaderParameter(string name, Ogre::Vector4 value);
	std::vector<EmmMaterialCreated> &getCreatedMaterials() { return created_materials; }
	string	getCreatedMaterialName(string materialName);

	void setTexturePack(EMBOgre *v) { texture_pack = v; }
	void setDYTTexturePack(EMBOgre *v) { texture_dyt_pack = v; }
	EMBOgre *getTexturePack() { return texture_pack; }
	EMBOgre *getDYTTexturePack() { return texture_dyt_pack; }

	void destroyResources();
};

#endif