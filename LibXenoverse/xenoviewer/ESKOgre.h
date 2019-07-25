

#ifndef ESKOGRE_H_INCLUDED
#define ESKOGRE_H_INCLUDED


class EANOgre;
class EANOgreAnimation;



class ESKOgre : public ESK
{
protected:
	Ogre::Skeleton *skeleton;

	bool resources_created;
	Ogre::SceneNode *skeleton_node;
	Ogre::Entity *skeleton_entity;
	EANOgreAnimation *animation_to_change;
	EANOgreAnimation *animation_to_change2;
	Ogre::AnimationState *current_animation_state;
	Ogre::AnimationState *current_animation_state2;
	bool to_delete;
	bool rebuildOgreAnimationNeeded;


	ESKOgre* mOriginalVersion;			//before merging (ex: scd)

public:
	ESKOgre(ESK* esk = 0);
	virtual ~ESKOgre(void);

	Ogre::Vector3 giveAngleOrientationForThisOrientationTaitBryan(Ogre::Quaternion orient);

	void buildBone(unsigned short b, Ogre::Skeleton *ogre_skeleton, Ogre::Bone *parent_bone);
	Ogre::Skeleton *createOgreSkeleton(Ogre::SceneManager *mSceneMgr);
	void createFakeEntity(Ogre::SceneManager *mSceneMgr);
	void refreshAnimations();

	Ogre::Skeleton *getOgreSkeleton() { return skeleton; }
	Ogre::Entity *getEntity() { return skeleton_entity; }

	void tagAnimationChange(EANOgreAnimation *new_animation) { animation_to_change = new_animation; }
	void tagAnimationChange2(EANOgreAnimation *new_animation) { animation_to_change2 = new_animation; }
	void tagForDeletion() { to_delete = true; }
	bool toDelete() { return to_delete; }
	bool changedAnimation() { return animation_to_change; }
	bool changedAnimation2() { return animation_to_change2; }
	bool isRebuildOgreAnimationNeeded() { return rebuildOgreAnimationNeeded; }
	
	void changeAnimation();
	void changeAnimation2();
	void stopAnimation();
	Ogre::AnimationState *getCurrentAnimationState() { return current_animation_state; }
	Ogre::AnimationState *getCurrentAnimationState2() { return current_animation_state2; }

	void destroyResources();


	void merge_scd(ESKOgre* parent_skeleton);
	void reverte_merge(void);
	void applyBinding(Ogre::SkeletonInstance* skelInst);
	void applyBinding_recursive(Ogre::Bone* bone, EskTreeNode* skeletonFofBinding);
};

#endif