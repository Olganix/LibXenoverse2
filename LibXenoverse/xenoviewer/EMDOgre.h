#ifndef EMDOGRE_H_INCLUDED
#define EMDOGRE_H_INCLUDED

class EMBOgre;
class ESKOgre;
class EMMOgre;
class EMDRenderObject;

class EMDOgre : public EMD
{
	public:
		struct LinkBetweenBoneAndNode			//to update node from bone position (for animation)
		{
			Ogre::SceneNode* node;
			Ogre::Bone* bone;
			LinkBetweenBoneAndNode(Ogre::SceneNode* node, Ogre::Bone* bone) { this->node = node;  this->bone = bone; }
		};

	protected:
		bool mesh_resources_created;
		EMMOgre* material_pack;
		ESKOgre* skeleton;
		bool to_rebuild;
		bool to_delete;
		bool mVisible;
		bool mAllVisible;

		list<Ogre::SceneNode *> scene_nodes;
		list<EMDRenderObject *> created_render_objects;

		list<Ogre::String> created_meshes;
		std::vector<LinkBetweenBoneAndNode> mListLinkBones;
		Ogre::String tag;
public:
		string fullName;


	public:
		EMDOgre(EMD* emd = 0);
		~EMDOgre();


		Ogre::SceneNode*	build(Ogre::SceneManager *scene_manager);
		void	rebuild();
		void	createOgreEMD(Ogre::SceneNode *parent, Ogre::SceneManager *scene_manager);
		Ogre::SceneNode*	createOgreEmdModel(EMDModel *model, Ogre::SceneNode *parent, Ogre::SceneManager *scene_manager);
		void	createOgreMesh_EmdSubMesh(EMDSubmesh *submesh, string mesh_name, Ogre::SceneNode* model_node);
		Ogre::SubMesh*	createOgreIndexBuffer_EmdSubMesh(EMDTriangles *triangles, Ogre::MeshPtr mesh, size_t nVertices);
		void	createOgreEntity_EmdSubMesh(EMDSubmesh *submesh, string mesh_name, Ogre::SceneNode *model_node, Ogre::SceneManager *scene_manager, string modelName);
		
		list<Ogre::SceneNode *>	&getListSceneNodes() { return scene_nodes; }
		void	cleanNodes(bool parent = false);
		void	destroyResources();
		void	updateBonesLinks();

		void	setSkeleton(ESKOgre *v) { skeleton = v; }
		ESKOgre*	getSkeleton() { return skeleton; }
		
		bool	getVisible(void) { return mVisible; }
		void	setVisible(bool enable);
		void	setVisible(string materialName, bool enable);
		void	setAllVisible(bool enable);
		void	setMaterialName(const Ogre::String &materialName, Ogre::SceneNode *node = 0);			//debug by using a material on all the entities.
		void	setMaterialPack(EMMOgre *v) { material_pack = v; }
		EMMOgre*	getMaterialPack() { return material_pack; }
		void	setMaterialParameter(const Ogre::String &paramName, Ogre::Vector4 value);
		const	Ogre::String&	getTag() { return tag; }
		void	tagForRebuild() { to_rebuild = true; }
		bool	toRebuild() { return to_rebuild; }
		void	tagForDeletion(){ to_delete = true; }
		bool	toDelete() { return to_delete; }
};

#endif