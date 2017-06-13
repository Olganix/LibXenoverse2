#ifndef SKELETONDEBUG_H_INCLUDED
#define SKELETONDEBUG_H_INCLUDED

#include <Ogre.h>
#include <OgreTagPoint.h>
#include <vector>
#include "ObjectTextDisplay.h"

class SkeletonDebug
{
public:
	struct BoneDebug
	{
		size_t mIndex;
		string mName;
		Ogre::Bone* mBone;

		Ogre::SceneManager* mSceneMgr;
		Ogre::SceneNode *mSceneNode;
		Ogre::SceneNode *mSceneNode_Axes;
		Ogre::Entity *mAxesEntity;

		std::vector<Ogre::SceneNode *> mListChild_SceneNode;

		BoneDebug(size_t index, const string &name, Ogre::SceneManager* sceneMgr, Ogre::Bone* bone)
		{
			mIndex = index;
			mName = name;
			mBone = bone;

			mSceneMgr = sceneMgr;
			mSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
			
			mSceneNode_Axes = mSceneNode->createChildSceneNode();
			mAxesEntity = sceneMgr->createEntity("Repere.mesh");
			mSceneNode_Axes->attachObject(mAxesEntity);

			Ogre::SceneNode* node = NULL;
			Ogre::Entity* ent = NULL;

			size_t numChildren = bone->numChildren();
			if (numChildren == 0)
			{
				node = mSceneNode->createChildSceneNode();
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
					ent = sceneMgr->createEntity("r.mesh");
					node->attachObject(ent);

					node->setScale(Ogre::Vector3(0.01f, pos.length(), 0.01f));
					node->setOrientation(Ogre::Vector3::UNIT_Y.getRotationTo(pos));		//r.mesh have center on the middle of the bottom face but it's Xaxe the source of bones.

					mListChild_SceneNode.push_back(node);
				}
			}
		}

		~BoneDebug(void)
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

		void	setAxesVisible(bool enable) { mSceneNode_Axes->setVisible(enable);  }
		void	setBoneVisible(bool enable) 
		{
			size_t nbNode = mListChild_SceneNode.size();
			for (size_t i = 0; i < nbNode; i++)
				mListChild_SceneNode.at(i)->setVisible(enable);
		}

		void	update(void)
		{
			Ogre::Vector3 position = mBone->_getDerivedPosition();
			Ogre::Quaternion orientation = mBone->_getDerivedOrientation();
			mSceneNode->setPosition(position);
			mSceneNode->setOrientation(orientation);
		}

	};

	SkeletonDebug(Ogre::Entity *entity, Ogre::SceneManager *man, Ogre::Camera *cam, float boneSize = 0.1f);
	~SkeletonDebug();

	void setAxesScale(Ogre::Real scale){ mScaleAxes = scale; }
	Ogre::Real getAxesScale(){ return mScaleAxes; }
	Ogre::Entity* getEntity(void){ return mEntity; }

	void showAxes(bool show);
	void showNames(bool show);
	void showBones(bool show);
	void showBones(string name, bool show);
	bool axesShown(){ return mShowAxes; }
	bool namesShown(){ return mShowNames; }
	bool bonesShown(){ return mShowBones; }

	void update();

private:
	std::vector<Ogre::Entity*> mAxisEntities;
	std::vector<Ogre::Entity*> mBoneEntities;
	std::vector<ObjectTextDisplay*> mTextOverlays;

	std::vector<BoneDebug*> mListBoneDebug;

	float mBoneSize;

	Ogre::Entity *mEntity;
	Ogre::MaterialPtr mAxisMatPtr;
	Ogre::MaterialPtr mBoneMatPtr;
	Ogre::MeshPtr mBoneMeshPtr;
	Ogre::MeshPtr mAxesMeshPtr;
	Ogre::SceneManager *mSceneMan;
	Ogre::Camera *mCamera;

	Ogre::Real mScaleAxes;

	bool mShowAxes;
	bool mShowBones;
	bool mShowNames;

	void createAxesMaterial();
	void createBoneMaterial();
	void createAxesMesh();
	void createBoneMesh();
};

#endif // SKELETONDEBUG_H_INCLUDED