#ifndef EMM_FBX_H_INCLUDED
#define EMM_FBX_H_INCLUDED

class EMM_FBX;

class EMMOgre : public EMM {
protected:
	bool material_resources_created;
	EMM_FBX *texture_pack;
	EMM_FBX *texture_dyt_pack;

	list<string> created_materials;
public:
	EMMOgre();
	~EMMOgre();

	FbxSurfacePhong *createFBXMaterial(EMMMaterial *emm_material);
	void createOgreMaterials();

	void setTexturePack(EMM_FBX *v) {
		texture_pack = v;
	}

	void setDYTTexturePack(EMM_FBX *v) {
		texture_dyt_pack = v;
	}

	EMM_FBX *getTexturePack() {
		return texture_pack;
	}

	EMM_FBX *getDYTTexturePack() {
		return texture_dyt_pack;
	}

	void destroyResources();
};

#endif