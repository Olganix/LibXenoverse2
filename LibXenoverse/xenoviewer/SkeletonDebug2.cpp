/*
	Code taken from: www.ogre3d.org/tikiwiki/tiki-index.php?page=Skeleton+Debugger
*/

#include "SkeletonDebug2.h"

using namespace Ogre;

SkeletonDebug2::SkeletonDebug2(Ogre::Skeleton* skeleton, Ogre::SceneManager *man, Ogre::Camera *cam, float boneSize)
{
	mSkeleton = skeleton;
	mSceneMan = man;
	mCamera = cam;

	mScaleAxes = 0.5;

	mBoneSize = boneSize;

	mShowAxes = true;
	mShowBones = true;
	mShowNames = true;

	int numBones = mSkeleton->getNumBones();

	for (unsigned short int iBone = 0; iBone < numBones; ++iBone)
	{
		Ogre::Bone* pBone = mSkeleton->getBone(iBone);
		if (!pBone)
		{
			assert(false);
			continue;
		}

		mListBoneDebug.push_back(new BoneDebug(iBone, pBone->getName(), man, pBone));
	}

	showAxes(false);
	showBones(false);
	showNames(false);
}



SkeletonDebug2::~SkeletonDebug2()
{
	size_t nbBoneDebug = mListBoneDebug.size();
	for (size_t i = 0; i < nbBoneDebug; i++)
		delete mListBoneDebug.at(i);

	mListBoneDebug.clear();
}

void SkeletonDebug2::update(std::vector<string> &listSelectedBoneNames)
{
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

void SkeletonDebug2::showAxes(bool show)
{
	// Don't change anything if we are already in the proper state
	if (mShowAxes == show)
		return;

	mShowAxes = show;

	size_t nbBoneDebug = mListBoneDebug.size();
	for (size_t i = 0; i < nbBoneDebug; i++)
		mListBoneDebug.at(i)->setAxesVisible(mShowAxes);
}

void SkeletonDebug2::showBones(bool show)
{
	// Don't change anything if we are already in the proper state
	if (mShowBones == show)
		return;

	mShowBones = show;

	size_t nbBoneDebug = mListBoneDebug.size();
	for (size_t i = 0; i < nbBoneDebug; i++)
		mListBoneDebug.at(i)->setBoneVisible(mShowBones);
}

void SkeletonDebug2::showBones(string name, bool show)
{
	size_t nbBoneDebug = mListBoneDebug.size();
	for (size_t i = 0; i < nbBoneDebug; i++)
	{
		if (mListBoneDebug.at(i)->mName != name)
			continue;
		mListBoneDebug.at(i)->setBoneVisible(show);
		mListBoneDebug.at(i)->setAxesVisible(show);
	}
}


void SkeletonDebug2::showNames(bool show)
{
	// Don't change anything if we are already in the proper state
	if (mShowNames == show)
		return;

	mShowNames = show;
}