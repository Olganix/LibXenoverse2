#ifndef SKELETONDEBUG2_H_INCLUDED
#define SKELETONDEBUG2_H_INCLUDED

#include <Ogre.h>
#include <OgreTagPoint.h>
#include <vector>
#include "ObjectTextDisplay.h"

class SkeletonDebug2
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

			mSceneNode->setScale(-1, 1, 1);				//dbvx symetry.


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

	};

	SkeletonDebug2(Ogre::Skeleton *skeleton, Ogre::SceneManager *man, Ogre::Camera *cam, float boneSize = 0.1f);
	~SkeletonDebug2();

	void setAxesScale(Ogre::Real scale){ mScaleAxes = scale; }
	Ogre::Real getAxesScale(){ return mScaleAxes; }
	Ogre::Skeleton* getSkeleton(void){ return mSkeleton; }

	void showAxes(bool show);
	void showNames(bool show);
	void showBones(bool show);
	void showBones(string name, bool show);
	bool axesShown(){ return mShowAxes; }
	bool namesShown(){ return mShowNames; }
	bool bonesShown(){ return mShowBones; }

	void update(std::vector<string> &listSelectedBoneNames);

private:
	std::vector<BoneDebug*> mListBoneDebug;

	float mBoneSize;
	Ogre::SceneManager *mSceneMan;
	Ogre::Camera *mCamera;
	Ogre::Skeleton* mSkeleton;

	Ogre::Real mScaleAxes;

	bool mShowAxes;
	bool mShowBones;
	bool mShowNames;
};

#endif // SKELETONDEBUG2_H_INCLUDED