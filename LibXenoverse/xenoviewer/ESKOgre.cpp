#include "ESKOgre.h"
#include "OgreCommon.h"

ESKOgre::ESKOgre()
{
	skeleton = NULL;
	resources_created = false;
	skeleton_entity = NULL;
	animation_to_change = NULL;
	current_animation_state = NULL;
	animation_to_change2 = NULL;
	current_animation_state2 = NULL;
	to_delete = false;
	skeleton_node = NULL;
	mOriginalVersion = NULL;		//BeforeMerging
	rebuildOgreAnimationNeeded = false;
}



/*-------------------------------------------------------------------------------\
|                giveAngleOrientationForThisOrientationTaitBryan                 |
\-------------------------------------------------------------------------------*/
Ogre::Vector3 ESKOgre::giveAngleOrientationForThisOrientationTaitBryan(Ogre::Quaternion orient)
{
	Ogre::Vector3 q = Ogre::Vector3::ZERO;

	Ogre::Vector3 dir = orient * Ogre::Vector3::UNIT_X;

	//1) calcul yaw
	Ogre::Vector2 vectproj = Ogre::Vector2(dir.x, -dir.z);		//projection of the result on (O,x,-z) plane
	if (vectproj.length() > 0.000001)							//if undefined => par defaut 0;
	{
		vectproj = vectproj.normalisedCopy();

		q.x = Ogre::Math::ACos(vectproj.x).valueDegrees();
		if (vectproj.y<0)
			q.x = -q.x;
	}

	//2) calcul pitch
	Ogre::Vector3 dir_tmp = (Ogre::Quaternion(Ogre::Degree(-q.x), Ogre::Vector3::UNIT_Y) * orient) * Ogre::Vector3::UNIT_X;		//cancel yaw rotation, the point must be into (O,x,y) plane
	q.y = Ogre::Math::ACos(dir_tmp.x).valueDegrees();
	if (dir_tmp.y<0)
		q.y = -q.y;


	//3) calcul roll
	dir_tmp = (Ogre::Quaternion(Ogre::Degree(-q.y), Ogre::Vector3::UNIT_Z) * Ogre::Quaternion(Ogre::Degree(-q.x), Ogre::Vector3::UNIT_Y) * orient) * Ogre::Vector3::UNIT_Z;		//cancel yaw and pitch rotations, the point Vector3::UNIT_Y, after rotation, must be into (O,x,z) plane
	q.z = Ogre::Math::ACos(dir_tmp.z).valueDegrees();
	if (dir_tmp.y>0)		// the direct direction from Oy to Oz
		q.z = -q.z;

	return q;
}



void ESKOgre::buildBone(unsigned short b, Ogre::Skeleton *ogre_skeleton, Ogre::Bone *parent_bone)
{
	ESKBone* bone = bones.at(b);
	Ogre::Bone* mBone = ogre_skeleton->createBone(bone->name);
	mBone->setUseNoUniformScale(true);

	if (parent_bone)
		parent_bone->addChild(mBone);


	float* relativeTransform = bone->skinning_matrix;
	
	Ogre::Vector3 mPos = Ogre::Vector3(relativeTransform[0], relativeTransform[1], relativeTransform[2]) * relativeTransform[3];		//multiplication by w if scaled.
	Ogre::Quaternion mRot = Ogre::Quaternion(relativeTransform[7], relativeTransform[4], relativeTransform[5], relativeTransform[6]);
	Ogre::Vector3 mScale = Ogre::Vector3(relativeTransform[8], relativeTransform[9], relativeTransform[10]) * relativeTransform[11];		//multiplication by w if scaled.
	if ((mScale.x == 0)|| (mScale.y == 0)|| (mScale.z == 0))
	{
		Ogre::LogManager::getSingleton().logMessage("Warning: Bone " + std::to_string(b) + " " + bones.at(b)->getName() + " Have a Scale at 0. it's should be a mistake (may be because of the current keyframe when you export from Blender or 3dsmax) Ogre don't like this. so we use Scale at 1 instead.");
		mScale = Ogre::Vector3::UNIT_SCALE;
	}
	

	mBone->setPosition(mPos);
	mBone->setOrientation(mRot);
	mBone->setScale(mScale);								// no need to do scale( Vector3(-1,1,1)) for relativeTransform
	

	mBone->setInitialState();

	mBone->setManuallyControlled(false);

	for (size_t i = 0; i < bones.size(); i++)
	{
		if (bones.at(i)->parent_index == b)
		{
			
			if (i < OGRE_MAX_NUM_BONES)
			{
				buildBone(i, ogre_skeleton, mBone);
			}else{
				Ogre::LogManager::getSingleton().logMessage("Warning: Bone "+ std::to_string(i) +" " + bones.at(i)->getName() + " is up to the limit of bone for Ogre ("+ std::to_string(OGRE_MAX_NUM_BONES) +". skipped.");
			}
		}
	}

	mBone->_update(true, true);
}





void ESKOgre::createFakeEntity(Ogre::SceneManager *mSceneMgr)
{
	Ogre::MeshPtr msh = Ogre::MeshManager::getSingleton().createManual(name + "_skeleton", XENOVIEWER_RESOURCE_GROUP);
	msh->setSkeletonName(name);

	Ogre::SubMesh* sub = msh->createSubMesh();
	const size_t nVertices = 3;
	const size_t nVertCount = 3;
	const size_t vbufCount = nVertCount*nVertices;
	float *vertices = (float *)malloc(sizeof(float)*vbufCount);

	for (size_t i = 0; i < nVertices; i++)
	{
		vertices[i*nVertCount] = 0.0;
		vertices[i*nVertCount + 1] = 0.0;
		vertices[i*nVertCount + 2] = 0.0;
	}

	const size_t ibufCount = 3;
	unsigned short *faces = (unsigned short *)malloc(sizeof(unsigned short) * ibufCount);

	for (size_t i = 0; i < ibufCount; i++)
		faces[i] = i;


	msh->sharedVertexData = new Ogre::VertexData();
	msh->sharedVertexData->vertexCount = nVertices;

	Ogre::VertexDeclaration* decl = msh->sharedVertexData->vertexDeclaration;
	size_t offset = 0;

	decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

	Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(offset, msh->sharedVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	vbuf->writeData(0, vbuf->getSizeInBytes(), vertices, true);
	Ogre::VertexBufferBinding* bind = msh->sharedVertexData->vertexBufferBinding;
	bind->setBinding(0, vbuf);
	Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, ibufCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	ibuf->writeData(0, ibuf->getSizeInBytes(), faces, true);
	sub->useSharedVertices = true;
	sub->indexData->indexBuffer = ibuf;
	sub->indexData->indexCount = ibufCount;
	sub->indexData->indexStart = 0;

	msh->_setBounds(Ogre::AxisAlignedBox(-100, -100, -100, 100, 100, 100));
	msh->_setBoundingSphereRadius(100);
	msh->load();

	free(faces);
	free(vertices);

	skeleton_entity = mSceneMgr->createEntity(name + "_FE", name + "_skeleton");
	skeleton_node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	skeleton_node->attachObject(skeleton_entity);
	skeleton_node->setVisible(false);
}

void ESKOgre::refreshAnimations()
{
	if (skeleton_entity)
		skeleton_entity->refreshAvailableAnimationState();
}


Ogre::Skeleton *ESKOgre::createOgreSkeleton(Ogre::SceneManager *mSceneMgr)
{
	stopAnimation();
	
	Ogre::SkeletonPtr mSkel = Ogre::SkeletonManager::getSingleton().create(name, XENOVIEWER_RESOURCE_GROUP, true);
	if (bones.size())
		buildBone(0, mSkel.getPointer(), NULL);

	mSkel->setBlendMode(Ogre::SkeletonAnimationBlendMode::ANIMBLEND_CUMULATIVE);

	resources_created = true;
	createFakeEntity(mSceneMgr);
	skeleton = mSkel.getPointer();
	return mSkel.getPointer();
}

void ESKOgre::changeAnimation()
{
	if (current_animation_state) 
	{
		current_animation_state->setEnabled(false);
		current_animation_state = NULL;
	}

	if (animation_to_change)
	{
		string animation_name = animation_to_change->getName();

		if (skeleton_entity->getAnimationState(animation_name))
		{
			current_animation_state = skeleton_entity->getAnimationState(animation_name);
			current_animation_state->setLoop(true);
			current_animation_state->setEnabled(true);
		}else {
			SHOW_ERROR("The skeleton doesn't have this animation for some reason!");
		}
	}

	animation_to_change = NULL;
}

void ESKOgre::changeAnimation2()
{
	if (current_animation_state2)
	{
		current_animation_state2->setEnabled(false);
		current_animation_state2 = NULL;
	}

	if (animation_to_change2)
	{
		string animation_name = animation_to_change2->getName();

		if (skeleton_entity->getAnimationState(animation_name))
		{
			current_animation_state2 = skeleton_entity->getAnimationState(animation_name);
			current_animation_state2->setLoop(true);
			current_animation_state2->setEnabled(true);
		}
		else {
			SHOW_ERROR("The skeleton doesn't have this animation for some reason!");
		}
	}

	animation_to_change2 = NULL;
}

void ESKOgre::stopAnimation()
{
	if (current_animation_state)
	{
		current_animation_state->setEnabled(false);
		current_animation_state = NULL;
	}

	if (current_animation_state2)
	{
		current_animation_state2->setEnabled(false);
		current_animation_state2 = NULL;
	}
}



void ESKOgre::destroyResources()
{
	stopAnimation();

	if (resources_created)
	{
		Ogre::SkeletonManager::getSingleton().remove(name);
		Ogre::MeshManager::getSingleton().remove(name + "_skeleton");
		OgreUtil::destroySceneNodeChildren(skeleton_node);
		skeleton_node->getCreator()->destroySceneNode(skeleton_node);
	}

	resources_created = false;
}

ESKOgre::~ESKOgre()
{
	destroyResources();
}





void ESKOgre::merge_scd(ESKOgre* parent_skeleton)
{
	bool haveTorecreate = resources_created;
	Ogre::SceneManager* sceneMgr = skeleton_node->getCreator();

	if (mOriginalVersion)
		reverte_merge();									//back to the initial state.
	destroyResources();										//Ogre part

	mOriginalVersion = new ESKOgre();
	mOriginalVersion->clone(this);

	this->clone(parent_skeleton);						//we take a copy of full skelton from EskOgre parent.
	this->merge_byAttach(mOriginalVersion);				//and we merge with scd informations.

	if (haveTorecreate)
		createOgreSkeleton(sceneMgr);

	rebuildOgreAnimationNeeded = true;
}

void ESKOgre::reverte_merge(void)
{
	if (!mOriginalVersion)
		return;

	bool haveTorecreate = resources_created;
	Ogre::SceneManager* sceneMgr = skeleton_node->getCreator();

	destroyResources();						//Ogre part

	this->clone(mOriginalVersion);			//clone esk to go back.

	if(haveTorecreate)
		createOgreSkeleton(sceneMgr);

	delete mOriginalVersion;
	mOriginalVersion = NULL;

	rebuildOgreAnimationNeeded = true;
}



void ESKOgre::applyBinding(Ogre::SkeletonInstance* skelInst)
{
	if ((!mOriginalVersion)||(!skelInst))
		return;

	EskTreeNode* skeletonFofBinding = mOriginalVersion->getTreeOrganisation();

	Ogre::Bone* root = skelInst->getRootBone();

	size_t nbbones = root->numChildren();
	for (size_t i = 0; i < nbbones; i++)
		applyBinding_recursive((Ogre::Bone*)root->getChild(i), skeletonFofBinding);
}

void ESKOgre::applyBinding_recursive(Ogre::Bone* bone, EskTreeNode* skeletonFofBinding)
{
	Ogre::Vector3 animInitPosition = bone->getInitialPosition();
	Ogre::Quaternion animInitOrientation = bone->getInitialOrientation();
	Ogre::Vector3 animInitScale = bone->getInitialScale();

	EskTreeNode* boneForBinding = skeletonFofBinding->getBoneWithName(bone->getName());
	if ((boneForBinding)&&(boneForBinding->mBone))
	{
		string parentBindName = ((boneForBinding->mParent)&&(boneForBinding->mParent->mBone)) ? boneForBinding->mParent->mBone->getName() : "";

		Ogre::Matrix4 matrixFromBindParentBone = Ogre::Matrix4::IDENTITY;
		Ogre::Matrix4 mat_tmp = Ogre::Matrix4::IDENTITY;

		
		Ogre::Bone* boneParent = (Ogre::Bone*)bone->getParent();					//we begin from parent because the last transformation don't have interess for this case, she will be replace by the transform of scd.esk.
		if ((boneParent) && (boneParent->getName() == parentBindName))
			boneParent = 0;
		
		while (boneParent)
		{
			mat_tmp.makeTransform(boneParent->getPosition(), boneParent->getScale(), boneParent->getOrientation());

			matrixFromBindParentBone = mat_tmp * matrixFromBindParentBone;

			boneParent = (Ogre::Bone*)boneParent->getParent();
			if ((boneParent)&&(boneParent->getName() == parentBindName))
				boneParent = 0;
		}


		Ogre::Vector3 pos_tmp;
		Ogre::Quaternion orient_tmp;
		Ogre::Vector3 scale_tmp;
		matrixFromBindParentBone.decomposition(pos_tmp, scale_tmp, orient_tmp);

		LibXenoverse::ESKBone* eskbone = boneForBinding->mBone;
		pos_tmp = (Ogre::Vector3(eskbone->skinning_matrix[0], eskbone->skinning_matrix[1], eskbone->skinning_matrix[2]) / eskbone->skinning_matrix[3]) - pos_tmp;
		orient_tmp = orient_tmp.Inverse() * Ogre::Quaternion(eskbone->skinning_matrix[7], eskbone->skinning_matrix[4], eskbone->skinning_matrix[5], eskbone->skinning_matrix[6]);
		scale_tmp = (Ogre::Vector3(eskbone->skinning_matrix[8], eskbone->skinning_matrix[9], eskbone->skinning_matrix[10]) / eskbone->skinning_matrix[11]) / scale_tmp;


		bone->setPosition(pos_tmp);
		bone->setOrientation(orient_tmp);
		bone->setScale(scale_tmp);
	}

	size_t nbbones = bone->numChildren();
	for (size_t i = 0; i < nbbones; i++)
		applyBinding_recursive((Ogre::Bone*)bone->getChild(i), skeletonFofBinding);

	bone->setBindingPose();

	if ((boneForBinding) && (boneForBinding->mBone))
	{
		bone->setPosition(animInitPosition);
		bone->setOrientation(animInitOrientation);
		bone->setScale(animInitScale);

		bone->setInitialState();
	}

}