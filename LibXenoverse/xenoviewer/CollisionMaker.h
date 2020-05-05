

#ifndef CollisionMaker_H_INCLUDED
#define CollisionMaker_H_INCLUDED

#include <Ogre.h>
#include "EMDOgre.h"

class CollisionMaker
{
public:
	struct OgreVoxel
	{
		size_t mIndex;
		string mName;
		Ogre::Vector3 mPosition;
		Ogre::Vector3 mScale;

		bool allreadyUsed;					// for reduceVoxel algo
		bool tagToDelete;					// on end of reduceVoxel operation.

		OgreVoxel(size_t index, const string &name, Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE, Ogre::Vector3 position = Ogre::Vector3::ZERO) { mIndex = index; mName = name; mScale = scale; mPosition = position;  tagToDelete = false; allreadyUsed = false; }
	};

private:
	Ogre::SceneManager *mSceneMgr;
	Ogre::Camera *mCamera;
	Ogre::Entity* mEntity;						//only present if there is a build of all voxel for 3D.
	bool mVisible;
	bool mIsOpaque;
	bool mIsReduced;
	bool mCouldHaveBoxIntersection;				// = false : (better with shell) first version with removing voxel to be used one time	

	Ogre::Real mVoxelSize;
	std::vector<OgreVoxel*> mListVoxels;
	size_t uniqueInc;

	
												// = true  : (better with inside) second version, try to reduce the number of box by allow to RE-USE voxel ONLY FOR EXTEND (Notice: it's because we don't care havok box have intersect space, so box could have intersection) 

public:
	CollisionMaker(Ogre::SceneManager *sceneMgr, Ogre::Camera *cam);
	~CollisionMaker();

	void setVoxelSize(Ogre::Real value) { mVoxelSize = value; }
	Ogre::Real getVoxelSize() { return mVoxelSize; }
	void setVisible(bool enable);
	bool getVisible() { return mVisible; }
	void setIsOpaque(bool enable);
	bool getIsOpaque() { return mIsOpaque; }
	bool getIsReduced() { return mIsReduced; }	
	void setCouldHaveBoxIntersection(bool enable) { mCouldHaveBoxIntersection = enable; }
	bool getCouldHaveBoxIntersection() { return mCouldHaveBoxIntersection; }
	

	void buildVoxel(std::list<EMDOgre *> &emdOgre_list);
	void destroyVoxel();
	bool haveVoxel() { return (mListVoxels.size() != 0); }
	void checkEntityCreationOnVisible();

	void reduceVoxel();

	void exportVoxel_Fbx(string basename = "");
	void exportVoxel_Emd(string basename = "");
	void exportVoxel_Havok(string basename = "");
	EMD* exportVoxelEmd(string basename);
};

#endif // CollisionMaker_H_INCLUDED