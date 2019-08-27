#include "EANOgre.h"






/*-------------------------------------------------------------------------------\
|                EANOgre										                 |
\-------------------------------------------------------------------------------*/
EANOgre::EANOgre()
{
	name = "";

	ean = 0;
	ema = 0;
	ema_material = 0;

	fps = 60.0f;
	force_animation = 0;
	force_animation2 = 0;
}
/*-------------------------------------------------------------------------------\
|                EANOgre										                 |
\-------------------------------------------------------------------------------*/
EANOgre::~EANOgre()
{
	clear();
}
/*-------------------------------------------------------------------------------\
|                clear											                 |
\-------------------------------------------------------------------------------*/
void EANOgre::clear()
{
	if (ean)
		delete ean;
	if (ema)
		delete ema;
	if (ema_material)
		delete ema_material;

	ean = 0;
	ema = 0;
	ema_material = 0;

	mListEANOgreAnimation.clear();
}
/*-------------------------------------------------------------------------------\
|                load											                 |
\-------------------------------------------------------------------------------*/
bool EANOgre::load(string filename)
{
	clear();

	string extension = LibXenoverse::extensionFromFilename(filename, true);
	string basefilename = filename.substr(0, filename.length() - (extension.size() + 1));
	
	
	if (extension=="ean")
	{
		ean = new EAN();
		if (!ean->load(filename))
		{
			delete ean;
			return false;
		}
		name = ean->getName();

		vector<EANAnimation> &animations = ean->getAnimations();
		for (size_t i = 0, nb = animations.size(); i < nb; i++)
			mListEANOgreAnimation.push_back(EANOgreAnimation(animations.at(i).getName(), this, &animations.at(i)));


	}else if (extension == "ema") {

		string extension2 = LibXenoverse::extensionFromFilename(basefilename, true);
		string basefilename2 = basefilename.substr(0, basefilename.length() - (extension2.size() + 1));

		if (extension2 == "mat") 
		{
			ema_material = new EMA_Material();
			if (!ema_material->load(filename))
			{
				delete ema_material;
				return false;
			}
			name = ema_material->getName();

			//todo the same for others case.
			vector<EMA_Material_Animation> &animations = ema_material->getAnimations();
			for (size_t i = 0, nb = animations.size(); i < nb; i++)
				mListEANOgreAnimation.push_back(EANOgreAnimation(animations.at(i).getName(), this, 0, &animations.at(i)));


		}else if (extension2 == "light") {

			return false;				//todo add this case.

		}else{							//obj or no second extension

			ema = new EMA();
			if (!ema->LoadFromFile(filename))
			{
				delete ema;
				return false;
			}

			//todo use direct values instead convert into ean.
			ean = new EAN();
			ema->writeEAN(ean);
			
			delete ema;
			ema = 0;
			name = ean->getName();

			vector<EANAnimation> &animations = ean->getAnimations();
			for (size_t i = 0, nb = animations.size(); i < nb; i++)
				mListEANOgreAnimation.push_back(EANOgreAnimation(animations.at(i).getName(), this, &animations.at(i)));
		}

	}else{
		return false;
	}

	return true;
}


/*-------------------------------------------------------------------------------\
|                createOgreAnimations							                 |
\-------------------------------------------------------------------------------*/
void EANOgre::createOgreAnimations(ESKOgre *v)
{
	esk_skeleton = v;

	if (ean)
	{
		vector<EANAnimation> &animations = ean->getAnimations();
		for (size_t i = 0, nb=animations.size(); i < nb; i++)
			createOgreAnimation(&animations[i]);
	}
}
/*-------------------------------------------------------------------------------\
|                createOgreAnimation							                 |
\-------------------------------------------------------------------------------*/
Ogre::Animation *EANOgre::createOgreAnimation(EANAnimation *animation)
{
	int keyframes_count = animation->getFrameCount();
	vector<Ogre::NodeAnimationTrack *> node_tracks;

	Ogre::Skeleton *ogre_skeleton = esk_skeleton->getOgreSkeleton();
	if (!ogre_skeleton)
		return NULL;

	if (ogre_skeleton->hasAnimation(animation->getName())) {
		return NULL;
	}

	Ogre::Animation* mAnim = ogre_skeleton->createAnimation(animation->getName(), (float)keyframes_count / fps);

	LOG_DEBUG("********************************************************************************************************\n********************************************************************************************************\n********************************************************************************************************\n********************************************************************************************************\nAnimation %s \n", animation->getName().c_str());

	createOgreAnimationTracks(mAnim, ogre_skeleton, animation);

	return mAnim;
}
/*-------------------------------------------------------------------------------\
|                createOgreAnimationTracks						                 |
\-------------------------------------------------------------------------------*/
void EANOgre::createOgreAnimationTracks(Ogre::Animation* mAnim, Ogre::Skeleton *ogre_skeleton, EANAnimation* animation)
{
	// -Ean animation key frame have relative values, relatives from parent Bones (not relative to the initial transformation of the bone)
	// -Ogre keyFrame Animation is relative to initial Bone transform.
	
	// -Ean have is own Skeleton definition (for relativeTrnsform from parent). So it could be different to Esk definition, but must be match a good result.
	// => So, we have to get the initiale transform of EanBone, and do the substract of EanKeyFrame values with this initial tranform, to push the resultat into Ogre keyframeAnim.


	// esk for scd, there is some holes on hierarchie (if you look with body esk into ean's file).
	// the thing is there is 2 initial pose:
	// -one for binding, to link vertex to 1-4 bones for skinning. like the A-pose or T-pose.
	// -one for animations.
	// normmally this two initial pose are the same. but I make change on Ogre to not doing them in the same time.
	// So here, we could considere only the initial pose for animations.



	// To deal with hierarchy holes of scd, the hidden part is also move with animation, so we have to work on each frame.
	//first, we will do the match between skeletons, to avoid to do that on each frame, and reduce number of operations
	EskTreeNode* skeleton_tree = (ean->getSkeleton()) ? ean->getSkeleton()->getTreeOrganisation() : 0;

	struct Bonelink
	{
		EskTreeNode* treeNode;
		EANAnimationNode* eanNode;
		Ogre::Bone* ogre_Bone;
		Ogre::NodeAnimationTrack* node_track;


		Bonelink(EskTreeNode* treeNode, EANAnimationNode* eanNode, Ogre::Bone* ogre_Bone, Ogre::NodeAnimationTrack* node_track)
		{
			this->treeNode = treeNode;
			this->eanNode = eanNode;
			this->ogre_Bone = ogre_Bone;
			this->node_track = node_track;
		}
	};


	EskTreeNode* treeNode = 0;
	ESKBone* eanBone = 0;
	EANAnimationNode* eanNode = 0;
	string bone_name = "";
	Ogre::Bone* ogre_Bone = 0;
	std::vector<Bonelink> bonelink_list;

	vector<EANAnimationNode> &nodes = animation->getNodes();
	size_t nbNodes = nodes.size();
	for (size_t j = 0; j < nbNodes; j++)											//for all bone in esk part of ean		//TODO optimise by following hierarchy, with keep middle matrix.
	{
		treeNode = skeleton_tree->getBoneWithIndex(nodes[j].getBoneIndex());		//we get the current bone in ean
		if ((!treeNode) || (!treeNode->mBone))
			continue;
		bone_name = treeNode->mBone->getName();


		if (!ogre_skeleton->hasBone(bone_name))										//the same in ogreSkeleton.
			continue;
		ogre_Bone = ogre_skeleton->getBone(bone_name);


		Ogre::NodeAnimationTrack* node_track = mAnim->createNodeTrack(ogre_Bone->getHandle(), ogre_Bone);

		bonelink_list.push_back(Bonelink(treeNode, &nodes[j], ogre_Bone, node_track));
	}











	float frame, px, py, pz, pw, rx, ry, rz, rw, sx, sy, sz, sw;
	Ogre::TransformKeyFrame* ogreKeyFrame;
	Ogre::Vector3 translate, translate_mem;
	Ogre::Quaternion rotation, rotation_mem;
	Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE;
	Ogre::Vector3 scale_mem = Ogre::Vector3::UNIT_SCALE;
	Ogre::Matrix4 eanBone_relative_matrix;					//initiale position relative from parent.
	Ogre::Matrix4 eanAnim_matrix;							//relative to initial position of bone : eanBone_matrix
	Ogre::Matrix4 eanBone_relative_Anim_matrix;				//animated result, relative from parent. 
	Ogre::Matrix4 ogreBone_relative_matrix;					//initial position for ogre bone link the mesh
	Ogre::Matrix4 ogreBone_relative_Anim_matrix;			//result animation for Ogre Bone, relative from parent.




	size_t frame_count = animation->getFrameCount();
	size_t nbBoneLink = bonelink_list.size();
	for (size_t i = 0; i < frame_count; i++)																			//we will now working on the current frame.
	{
		frame = (float)i / fps;


		for (size_t j = 0; j < nbBoneLink; j++)
		{
			Bonelink &bonelink = bonelink_list.at(j);
			treeNode = bonelink.treeNode;


			string ogreParentBoneName = bonelink.ogre_Bone->getParent() ? bonelink.ogre_Bone->getParent()->getName() : "";		//as the parent bone is also animated, we have to stop recursive on it.


			eanBone_relative_Anim_matrix = Ogre::Matrix4::IDENTITY;

			while (treeNode)						//we look after all parent, to get the true absolute position (until a matching bone).
			{
				eanBone = treeNode->mBone;



				//make a matrix from relative transform componantes
				eanBone_relative_matrix = Ogre::Matrix4::IDENTITY;

				if (eanBone)
				{
					translate_mem = Ogre::Vector3(eanBone->skinning_matrix[0], eanBone->skinning_matrix[1], eanBone->skinning_matrix[2]) / eanBone->skinning_matrix[3];
					rotation_mem = Ogre::Quaternion(eanBone->skinning_matrix[7], eanBone->skinning_matrix[4], eanBone->skinning_matrix[5], eanBone->skinning_matrix[6]);
					scale_mem = Ogre::Vector3(eanBone->skinning_matrix[8], eanBone->skinning_matrix[9], eanBone->skinning_matrix[10]) / eanBone->skinning_matrix[11];
					eanBone_relative_matrix.makeTransform(translate_mem, scale_mem, rotation_mem);
				}




				//make interpolations from animation
				eanNode = 0;
				for (size_t k = 0; k < nbNodes; k++)												//todo: opimise
				{
					if (nodes[k].getBoneIndex() == treeNode->mIndex)
					{
						eanNode = &(nodes[k]);
						break;
					}
				}

				eanAnim_matrix = Ogre::Matrix4::IDENTITY;

				if (eanNode)
				{
					px = py = pz = rx = ry = rz = 0.0f;
					pw = rw = sx = sy = sz = sw = 1.0f;

					if (eanNode->getInterpolatedFrame(i, LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION, px, py, pz, pw))
					{
						if (isinf(px)) px = 99999.0;											//Ema could have "inf" floats.
						if (isinf(py)) py = 99999.0;
						if (isinf(pz)) pz = 99999.0;
						if (isinf(pw)) pw = 99999.0;
						
						
						translate = Ogre::Vector3(px, py, pz);
						if ((pw != 1.0f) && (pw != 0.0f))
							translate /= pw;
					}
					else {																		//if there isn't keyFrames for position, we will use the initial value for bone, so when we make the difference, you will have a 0 difference for Ogre::Animation
						translate = translate_mem;
					}

					if (eanNode->getInterpolatedFrame(i, LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION, rx, ry, rz, rw))
					{
						if (isinf(rx)) rx = 99999.0;
						if (isinf(ry)) ry = 99999.0;
						if (isinf(rz)) rz = 99999.0;
						if (isinf(rw)) rw = 99999.0;
						
						rotation = Ogre::Quaternion(rw, rx, ry, rz);
					}else {
						rotation = rotation_mem;
					}

					if (eanNode->getInterpolatedFrame(i, LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE, sx, sy, sz, sw))
					{
						if (isinf(sx)) sx = 99999.0;
						if (isinf(sy)) sy = 99999.0;
						if (isinf(sz)) sz = 99999.0;
						if (isinf(sw)) sw = 99999.0;
						
						scale = Ogre::Vector3(sx, sy, sz);
						if ((sw != 1.0f) && (sw != 0.0f))
							scale /= sw;
					}
					else {
						scale = scale_mem;
					}


					//solve picoloo's arms (instead using matrix inverse, on orientation)
					translate -= translate_mem;
					rotation = rotation_mem.Inverse() * rotation;
					if ((scale_mem != Ogre::Vector3::UNIT_SCALE) && (scale_mem != Ogre::Vector3::ZERO))
						scale /= scale_mem;

					if (scale.x <= 0.0000001) scale.x = 0.0000001;
					if (scale.y <= 0.0000001) scale.y = 0.0000001;
					if (scale.z <= 0.0000001) scale.z = 0.0000001;


					eanAnim_matrix.makeTransform(translate, scale, rotation);		// Get the relative values, by matrix operation
				}



				eanBone_relative_Anim_matrix = (eanAnim_matrix)* eanBone_relative_Anim_matrix;
				






				treeNode = treeNode->mParent;																	//next parent.

				if ((treeNode) && (treeNode->mBone) && (treeNode->mBone->getName() == ogreParentBoneName))		//as the parent bone is also animated, we have to stop recursive on it (not the case in holes in hierarchy on scd.esk).
					treeNode = 0;
			}



			eanBone_relative_Anim_matrix.decomposition(translate, scale, rotation);
			

			ogreKeyFrame = bonelink.node_track->createNodeKeyFrame(frame);
			ogreKeyFrame->setTranslate(translate);
			ogreKeyFrame->setRotation(rotation);
			ogreKeyFrame->setScale(scale);
		}
	}

}



/*-------------------------------------------------------------------------------\
|                giveAngleOrientationForThisOrientationTaitBryan                 |
\-------------------------------------------------------------------------------*/
Ogre::Vector3 EANOgre::giveAngleOrientationForThisOrientationTaitBryan(Ogre::Quaternion orient)
{
	Ogre::Vector3 q = Ogre::Vector3::ZERO;

	Ogre::Vector3 dir = orient * Ogre::Vector3::UNIT_X;

	//1) calcul yaw
	Ogre::Vector2 vectproj = Ogre::Vector2(dir.x, -dir.z);		//projection of the result on (O,x,-z) plane
	if (vectproj.length() > 0.000001)							//else, if undefined => by defaut 0;
	{
		vectproj = vectproj.normalisedCopy();

		q.x = Ogre::Math::ACos(vectproj.x).valueDegrees();
		if (vectproj.y < 0)
			q.x = -q.x;
	}

	//2) calcul pitch
	Ogre::Vector3 dir_tmp = (Ogre::Quaternion(Ogre::Degree(-q.x), Ogre::Vector3::UNIT_Y) * orient) * Ogre::Vector3::UNIT_X;		//we cancel the yaw rotation. normally, the point must be into (O,x,y) plane
	q.y = Ogre::Math::ACos(dir_tmp.x).valueDegrees();
	if (dir_tmp.y < 0)
		q.y = -q.y;


	//3) calcul rool
	dir_tmp = (Ogre::Quaternion(Ogre::Degree(-q.y), Ogre::Vector3::UNIT_Z) * Ogre::Quaternion(Ogre::Degree(-q.x), Ogre::Vector3::UNIT_Y) * orient) * Ogre::Vector3::UNIT_Z;		//we cancel the yaw rotation and after the pitch rotation. normally, the point Vector3::UNIT_Y, after rotation must be in (O,x,z) plane.
	q.z = Ogre::Math::ACos(dir_tmp.z).valueDegrees();
	if (dir_tmp.y > 0)		// the direct direction is from Oy to Oz
		q.z = -q.z;

	return q;
}





/*-------------------------------------------------------------------------------\
|                toForceAnimation								                 |
\-------------------------------------------------------------------------------*/
EANOgreAnimation* EANOgre::toForceAnimation()
{
	EANOgreAnimation* c = force_animation;
	force_animation = NULL;
	return c;
}
EANOgreAnimation* EANOgre::toForceAnimation2()
{
	EANOgreAnimation* c = force_animation2;
	force_animation2 = NULL;
	return c;
}