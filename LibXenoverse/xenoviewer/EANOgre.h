#include "EAN.h"
#include "ESKOgre.h"

#ifndef EANOGRE_H_INCLUDED
#define EANOGRE_H_INCLUDED

class EANOgre : public EAN
{
protected:
	bool animation_resources_created;
	ESKOgre* esk_skeleton;
	float fps;

	EANAnimation* force_animation;
	EANAnimation* force_animation2;

public:
	EANOgre();

	Ogre::Vector3 giveAngleOrientationForThisOrientationTaitBryan(Ogre::Quaternion orient);

	void	createOgreAnimationTracks(Ogre::Animation* mAnim, Ogre::Skeleton *ogre_skeleton, EANAnimation* animation);
	Ogre::Animation *createOgreAnimation(EANAnimation* animation);
	void createOgreAnimations(ESKOgre* v);

	void tagForceAnimation(EANAnimation *v) { force_animation = v; }
	void tagForceAnimation2(EANAnimation *v) { force_animation2 = v; }

	EANAnimation* toForceAnimation()
	{
		EANAnimation* c = force_animation;
		force_animation = NULL;
		return c;
	}

	EANAnimation* toForceAnimation2()
	{
		EANAnimation* c = force_animation2;
		force_animation2 = NULL;
		return c;
	}
};

#endif