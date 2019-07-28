#include "EMDOgre.h"
#include "EMMOgre.h"
#include "EMBOgre.h"
#include "ESKOgre.h"
#include "EANOgre.h"
#include "SkeletonDebug.h"
#include "SkeletonDebug2.h"
#include "EMDRenderObjectListener.h"
#include "ViewerGrid.h"
#include "OgreWidget.h"

namespace QtOgre
{
	
	
	///////////////////////////////// build scene
	
	void OgreWidget::createScene(void)
	{
		LibXenoverse::initializeDebuggingLog();

		// Initialize
		current_animation_index = 0;
		skeleton_debug = NULL;
		skeleton_debug2 = NULL;
		current_animation_state = NULL;
		animation = NULL;
		entity = NULL;
		enable_spinning = false;
		enable_panning = false;
		spin_x = 0.0f;
		spin_y = 0.0f;
		resetCamera_Charac();

		haveMoved = false;
		mGeometryVisible = true;
		mSkeletonVisible = false;
		mRepereNode = 0;
		mGridNode = 0;
		mGridVisible = true;
		mRepereVisible = true;
	
		

		// Create a blank texture
		Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().createManual("Blank", XENOVIEWER_RESOURCE_GROUP, Ogre::TEX_TYPE_2D, 32, 32, 0, Ogre::PF_BYTE_BGRA, Ogre::TU_DEFAULT);
		Ogre::HardwarePixelBufferSharedPtr pixelBuffer = texture->getBuffer();
		pixelBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL);
		const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();
		Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);

		for (size_t j = 0; j < 32; j++)
		{
			for (size_t i = 0; i < 32; i++)
			{
				*pDest++ = 0;
				*pDest++ = 0;
				*pDest++ = 0;
				*pDest++ = 255;
			}

			pDest += pixelBox.getRowSkip() * Ogre::PixelUtil::getNumElemBytes(pixelBox.format);
		}
		pixelBuffer->unlock();

		
		// Create Lighting
		mSceneMgr->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.3f));				//NEVER USE THE SAME VALUE FOR AMBIENT AND BACGROUNDCOLOR !!!!!
		Ogre::Light *direct_light = mSceneMgr->createLight("Xenoviewer Direct Light");
		direct_light->setSpecularColour(Ogre::ColourValue::White);
		direct_light->setDiffuseColour(Ogre::ColourValue(1.0, 1.0, 1.0));
		direct_light->setType(Ogre::Light::LT_DIRECTIONAL);
		direct_light->setDirection(Ogre::Vector3(1, -1, 1).normalisedCopy());			//most beautiful


		// Load Shaders
		/*
		//version for only DBX. TODO remove
		vector<string> shader_names;
		shader_names.push_back("adam_shader/shader_age_ps.emb");
		shader_names.push_back("adam_shader/shader_age_vs.emb");
		shader_names.push_back("adam_shader/shader_default_ps.emb");
		shader_names.push_back("adam_shader/shader_default_vs.emb");

		bool needs_install_shaders = false;
		for (size_t i = 0; i < shader_names.size(); i++)
		{
			if (!LibXenoverse::fileCheck(shader_names[i]))
			{
				needs_install_shaders = true;
				break;
			}
		}
		*/


		bool needs_install_shaders = (!QDir().exists("adam_shader"));
		if (needs_install_shaders)
		{
			if (!installShaders())
				return;
		}


		
		QDir src_folder = QDir("adam_shader");
		string filename = "";
		QFileInfoList files = src_folder.entryInfoList();
		foreach(QFileInfo file, files)
		{
			string filename = file.fileName().toStdString();
			if (file.isDir())
				continue;
		
			filename = file.fileName().toStdString();

			if (file.suffix().toStdString() == "emb")
			{
				EMBOgre *shader_pack = new EMBOgre();
				if (shader_pack->load("adam_shader/" + filename))
				{
					shader_pack->createOgreShaders();
				}else{
					SHOW_ERROR(QString("Couldn't load Shader Pack %1. File is either aissing, open by another application, or corrupt.").arg(filename.c_str()));
				}

			}else if(file.suffix().toStdString() == "sds"){

				SDS* sds = new SDS();
				if (sds->load("adam_shader/" + filename))
				{
					sds_list.push_back(sds);
				}else{
					SHOW_ERROR(QString("Couldn't load ShaderProgram (SDS file) %1. File is either aissing, open by another application, or corrupt.").arg(filename.c_str()));
				}
			}
		}
	

		// Create a grid
		mGridNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity* ent = mSceneMgr->createEntity("Grid.mesh");
		mGridNode->attachObject(ent);


		//create repere
		mRepereNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

		Ogre::SceneNode* repNode = mRepereNode->createChildSceneNode();
		
		Ogre::SceneNode* node = repNode->createChildSceneNode();
		ent = mSceneMgr->createEntity("r.mesh");
		node->attachObject(ent);
		node->setScale(Ogre::Vector3::UNIT_SCALE * 0.1f);
		ent->setMaterialName("White", XENOVIEWER_RESOURCE_GROUP);
		node->setPosition(0, 0, 0);

		node = repNode->createChildSceneNode();
		ent = mSceneMgr->createEntity("r.mesh");
		node->attachObject(ent);
		node->setScale(Ogre::Vector3::UNIT_SCALE * 0.1f);
		ent->setMaterialName("Red", XENOVIEWER_RESOURCE_GROUP);
		node->setPosition(1, 0, 0);

		node = repNode->createChildSceneNode();
		ent = mSceneMgr->createEntity("r.mesh");
		node->attachObject(ent);
		node->setScale(Ogre::Vector3::UNIT_SCALE * 0.1f);
		ent->setMaterialName("Green", XENOVIEWER_RESOURCE_GROUP);
		node->setPosition(0, 1, 0);

		node = repNode->createChildSceneNode();
		ent = mSceneMgr->createEntity("r.mesh");
		node->attachObject(ent);
		node->setScale(Ogre::Vector3::UNIT_SCALE * 0.1f);
		ent->setMaterialName("Blue", XENOVIEWER_RESOURCE_GROUP);
		node->setPosition(0, 0, 1);


		ent = mSceneMgr->createEntity("Repere.mesh");
		ent->setRenderQueueGroup(70);
		mRepereNode->attachObject(ent);

		


		Ogre::ConfigFile cfg;

		try {
			// Don't trim whitespace
			cfg.load("resources/xenoConfig.cfg", "\t:=", false);
		}catch (Ogre::Exception& e){
			return;
		}

		string cfg_str = cfg.getSetting("Grid_visible", Ogre::StringUtil::BLANK, "true");
		mGridNode->setVisible( cfg_str == "true" );

		cfg_str = cfg.getSetting("Repere_visible", Ogre::StringUtil::BLANK, "true");
		mRepereNode->setVisible(cfg_str == "true");

		cfg_str = cfg.getSetting("AmbientColor", Ogre::StringUtil::BLANK, "");
		if(cfg_str.length()!=0)
			mSceneMgr->setAmbientLight( Ogre::StringConverter::parseColourValue(cfg_str) * Ogre::ColourValue(1.0 / 255.0, 1.0 / 255.0, 1.0 / 255.0, 1.0) );


		cfg_str = cfg.getSetting("Light_DifuseColor", Ogre::StringUtil::BLANK, "");
		if (cfg_str.length() != 0)
			direct_light->setDiffuseColour(Ogre::StringConverter::parseColourValue(cfg_str) * Ogre::ColourValue(1.0 / 255.0, 1.0 / 255.0, 1.0 / 255.0, 1.0));

		cfg_str = cfg.getSetting("Light_SpecularColor", Ogre::StringUtil::BLANK, "");
		if (cfg_str.length() != 0)
			direct_light->setSpecularColour(Ogre::StringConverter::parseColourValue(cfg_str) * Ogre::ColourValue(1.0 / 255.0, 1.0 / 255.0, 1.0 / 255.0, 1.0));





		cfg_str = cfg.getSetting("ListBackgroundColors", Ogre::StringUtil::BLANK, "Grey_0.5[128, 128, 128]; Empty[0, 0, 0, 0]");
		
		Ogre::String tmp = "";
		Ogre::StringVector sv = Ogre::StringUtil::split(cfg_str, ";");
		for (size_t i = 0, nb = sv.size(); i < nb; i++)
		{
			tmp = sv.at(i);
			Ogre::StringUtil::trim(tmp);
			if (tmp[tmp.length() - 1] == ']')
				tmp = tmp.substr(0, tmp.length() - 1);

			Ogre::StringVector sv_b = Ogre::StringUtil::split(tmp, "[");
			if (sv_b.size() != 2)
				continue;

			mListBackgroundColor.push_back( NamedColor(sv_b.at(0), Ogre::StringConverter::parseColourValue(sv_b.at(1)) * Ogre::ColourValue(1.0/255.0, 1.0 / 255.0, 1.0 / 255.0, 1.0)  ));
		}
		

		cfg_str = cfg.getSetting("Background_color", Ogre::StringUtil::BLANK, "");
		if (cfg_str.length() != 0)
		{
			for (size_t i = 0, nb = mListBackgroundColor.size(); i < nb; i++)
			{
				if(cfg_str== mListBackgroundColor.at(i).name)
					mViewport->setBackgroundColour(mListBackgroundColor.at(i).color);
			}
		}

		//loadDebugModels();				//redondant test.
	}





	void OgreWidget::loadDebugModels()
	{
		/*
		// Load Character Models/Skeletons/Animations/Materials/Textures
		string folder = "";
		string character_name = "GOK";
		string character_index = "000";
		string character_prefix = folder + character_name + "/" + character_name + "_" + character_index;
		string skeleton_filename = character_prefix + ".esk";
		string animation_filename = "GOK/GOK.ean";

		ESKOgre *skeleton = NULL;
		skeleton = new ESKOgre();
		if (skeleton->load(skeleton_filename))
		{
			skeleton->createOgreSkeleton(mSceneMgr);
		}else{
			delete skeleton;
			skeleton = NULL;
		}

		animation = new EANOgre();
		if (animation->load(animation_filename))
		{
			animation->createOgreAnimations(skeleton);
		}else {
			SHOW_SMSG("Couldn't load " + animation_filename + " for animation.");
			delete animation;
			animation = NULL;
		}

		vector<string> model_names;
		model_names.push_back(character_prefix + "_Bust");
		model_names.push_back(character_prefix + "_Boots");
		model_names.push_back(character_prefix + "_Face_base");
		model_names.push_back(character_prefix + "_Face_eye");
		model_names.push_back(character_prefix + "_Face_forehead");
		model_names.push_back(character_prefix + "_Pants");
		model_names.push_back(character_prefix + "_Rist");

		for (size_t i = 0; i < model_names.size(); i++)
		{
			string emb_filename = model_names[i] + ".emb";
			string emb_dyt_filename = model_names[i] + ".dyt.emb";

			EMBOgre *texture_pack = new EMBOgre();
			if (texture_pack->load(emb_filename))
			{
				texture_pack->createOgreTextures();
			}else {
				delete texture_pack;
				texture_pack = NULL;
			}

			EMBOgre *texture_dyt_pack = new EMBOgre();
			if (texture_dyt_pack->load(emb_dyt_filename))
			{
				texture_dyt_pack->createOgreTextures();
			}else {
				delete texture_dyt_pack;
				texture_dyt_pack = NULL;
			}

			EMMOgre *material = new EMMOgre();
			if (material->load(model_names[i] + ".emm"))
			{
				material->setTexturePack(texture_pack);
				material->setDYTTexturePack(texture_dyt_pack);
				material->createOgreMaterials(sds_list);
			}else {
				delete material;
				material = NULL;
			}

			EMDOgre *model = new EMDOgre();
			if (model->load(model_names[i] + ".emd"))
			{
				if (skeleton)
					model->setSkeleton(skeleton);

				model->setMaterialPack(material);

				Ogre::SceneNode *emd_root_node = model->build(mSceneMgr);
			}else {
				delete model;
				model = NULL;
			}

			delete model;
			delete texture_pack;
			delete texture_dyt_pack;
			delete material;
		}
		*/
	}

	



	////////////////////////////////////////////////////////////////////////////////////////////////// frame refresh.


	bool OgreWidget::frameRenderingQueued(const Ogre::FrameEvent& evt)
	{
		if((mAnimationPlaying)&&(!mAnimationStop))
			mCurrentTime += 1.0 / 60.0;				// one frame.
		
		
		try
		{

			for (list<EANOgre *>::iterator it = ean_list.begin(); it != ean_list.end(); it++)
			{
				EANOgreAnimation *force_animation = (*it)->toForceAnimation();
				EANOgreAnimation *force_animation2 = (*it)->toForceAnimation2();

				if ((force_animation)&&(force_animation->getEANAnimation()))
				{
					for (list<ESKOgre *>::iterator it = esk_list.begin(); it != esk_list.end(); it++)
						(*it)->tagAnimationChange(force_animation);
				}

				if ((force_animation2) && (force_animation2->getEANAnimation()))
				{
					for (list<ESKOgre *>::iterator it = esk_list.begin(); it != esk_list.end(); it++)
						(*it)->tagAnimationChange2(force_animation2);
				}

				
				if ((force_animation) && (force_animation->getEmaMaterialAnimation()))
				{
					EMMOgre* emmOgre = 0;
					EMA_Material_Animation* ema_matAnim = force_animation->getEmaMaterialAnimation();
					vector<EMA_Material_Material> &materials = ema_matAnim->getMaterials();

					for (list<EMDOgre *>::iterator it = emd_list.begin(); it != emd_list.end(); it++)
					{
						emmOgre = (*it)->getMaterialPack();

						for (size_t i = 0, nb = materials.size(); i < nb; i++)
						{
							if (emmOgre->getMaterial(materials.at(i).getName()))
							{
								emmOgre->tagAnimationChange(force_animation);
								break;
							}
						}
					}
				}

				if ((force_animation2) && (force_animation2->getEmaMaterialAnimation()))
				{
					EMMOgre* emmOgre = 0;
					EMA_Material_Animation* ema_matAnim = force_animation2->getEmaMaterialAnimation();
					vector<EMA_Material_Material> &materials = ema_matAnim->getMaterials();

					for (list<EMDOgre *>::iterator it = emd_list.begin(); it != emd_list.end(); it++)
					{
						emmOgre = (*it)->getMaterialPack();

						for (size_t i = 0, nb = materials.size(); i < nb; i++)
						{
							if (emmOgre->getMaterial(materials.at(i).getName()))
							{
								emmOgre->tagAnimationChange(force_animation2);
								break;
							}
						}
					}
				}
			}







			for (list<ESKOgre *>::iterator it = esk_list.begin(); it != esk_list.end();)
			{
				if ((*it)->toDelete())
				{
					for (list<EMDOgre *>::iterator itm = emd_list.begin(); itm != emd_list.end(); itm++)
					{
						if ((*itm)->getTag() == "EMD")
						{
							if ((*itm)->getSkeleton() == *it)
							{
								(*itm)->setSkeleton(NULL);
								(*itm)->tagForRebuild();
							}
						}
					}
					delete *it;
					it = esk_list.erase(it);
					continue;
				}

				if ((*it)->isRebuildOgreAnimationNeeded())
				{
					for (list<EANOgre *>::iterator it2 = ean_list.begin(); it2 != ean_list.end(); it2++)
						(*it2)->createOgreAnimations((*it));
					(*it)->refreshAnimations();
				}

				if ((*it)->changedAnimation())
				{
					(*it)->changeAnimation();
					mCurrentTime = 0.0;
				}

				if ((*it)->changedAnimation2())
					(*it)->changeAnimation2();

				if (mAnimationStop)
					(*it)->stopAnimation();


				Ogre::AnimationState *state = (*it)->getCurrentAnimationState();
				if (state)
				{					
					if (state->getLoop() != mAnimationLoopEnable)
						state->setLoop(mAnimationLoopEnable);

					if ((mAnimationPlaying)||(mAnimationForceOneUpdate))
						state->setTimePosition(mCurrentTime);

					mAnimationStatus = Ogre::StringConverter::toString( mCurrentTime * 60.0) + "/" + Ogre::StringConverter::toString(state->getLength() * 60.0);
				}


				state = (*it)->getCurrentAnimationState2();
				if (state)
				{
					if (state->getLoop() != mAnimationLoopEnable)
						state->setLoop(mAnimationLoopEnable);

					if ((mAnimationPlaying) || (mAnimationForceOneUpdate))
						state->setTimePosition(mCurrentTime);
				}


				if ((*it)->getOgreSkeleton())
				{

					if ((mSkeletonVisible) && (!skeleton_debug2))
					{
						skeleton_debug2 = new SkeletonDebug2((*it)->getEntity()->getSkeleton(), mSceneMgr, mCamera);
						skeleton_debug2->showAxes(true);
						skeleton_debug2->showBones(true);
						skeleton_debug2->showNames(true);
					}
				}

				it++;
			}

			

			for (list<EMDOgre *>::iterator it = emd_list.begin(); it != emd_list.end();)
			{
				if ((*it)->getTag() == "EMD")
				{
					if ((*it)->toDelete())
					{
						if ((skeleton_debug2) && ((*it)->getSkeleton()) && ((*it)->getSkeleton()->getOgreSkeleton()) && ((*it)->getSkeleton()->getOgreSkeleton() == skeleton_debug2->getSkeleton()))
						{
							delete skeleton_debug2;
							skeleton_debug2 = NULL;
						}

						delete *it;
						it = emd_list.erase(it);
						continue;
					}

					if ((*it)->toRebuild())
						(*it)->rebuild();

					(*it)->setAllVisible(mGeometryVisible);

				}

				EMMOgre* emmOgre = (*it)->getMaterialPack();

				if (emmOgre->changedAnimation())
				{
					emmOgre->changeAnimation();
					mCurrentTime = 0.0;
				}

				if (emmOgre->changedAnimation2())
					emmOgre->changeAnimation2();

				if (mAnimationStop)
					emmOgre->stopAnimation();

				emmOgre->setLoop(mAnimationLoopEnable);
				if ((mAnimationPlaying) || (mAnimationForceOneUpdate))
					emmOgre->updateAnimations(mCurrentTime);

				it++;
			}


			mAnimationForceOneUpdate = false;

			if ((!mSkeletonVisible) && (skeleton_debug2))
			{
				delete skeleton_debug2;
				skeleton_debug2 = NULL;
			}

			if (skeleton_debug2)
			{
				//visiblity for each bone
				for (list<ESKBone *>::iterator it = bone_list.begin(); it != bone_list.end(); it++)
					skeleton_debug2->showBones((*it)->getName(), (*it)->getVisible());

				skeleton_debug2->update( listSelectedBoneNames);
			}

			repositionCamera();

		} catch (...){
			
		}
		return true;
	}


	

	///////////////////////////////// inputs

	
	void OgreWidget::keyPressEvent(QKeyEvent* event)
	{
		//for fast navigation in stage.
		if(event->key() == Qt::Key_PageUp)
		{
			
		}
	}
	
	void OgreWidget::toggleFullScreen()
	{
		if (!mIsFullscreen)
		{
			//refresh parent and grand parent because there is lots of operation on widgets, hierarchy change
			mpParentWidget = this->parentWidget();
			mpGrandParentWidget = mpParentWidget->parentWidget();

			mpParentWidget->setParent(0);
			mpParentWidget->showFullScreen();
			mpParentWidget->show();

			QPoint position = this->pos();
			this->move(0, 0);

			QLayout* layout = mpParentWidget->layout();
			if (layout)
			{
				layout->setSpacing(0);
				layout->setMargin(0);
			}

			

			mIsFullscreen = true;

		}
		else {

			QLayout* layout = mpParentWidget->layout();
			if (layout)
			{
				layout->setSpacing(6);
				layout->setMargin(11);
			}
			mpParentWidget->showNormal();

			this->move(9, 9);

			if (mpGrandParentWidget->layout())
			{
				mpGrandParentWidget->layout()->addWidget(mpParentWidget);
			}
			else {
				mpParentWidget->setParent(mpGrandParentWidget);
			}

			mpParentWidget->show();
			mpGrandParentWidget->show();

			mIsFullscreen = false;
		}
	}

	void OgreWidget::cancelFullScreen()
	{
		if (mIsFullscreen)
		{
			QLayout* layout = mpParentWidget->layout();
			if (layout)
			{
				layout->setSpacing(6);
				layout->setMargin(11);
			}
			mpParentWidget->showNormal();

			this->move(9, 9);

			if (mpGrandParentWidget->layout())
			{
				mpGrandParentWidget->layout()->addWidget(mpParentWidget);
			}
			else {
				mpParentWidget->setParent(mpGrandParentWidget);
			}

			mpParentWidget->show();
			mpGrandParentWidget->show();

			mIsFullscreen = false;
		}
	}


	void OgreWidget::doScreenShot()
	{
		std::ostringstream ss;
		ss << "\\screenshot_" << ++mNumScreenShots << ".png";


		if ((mWindow->isActive()) && (mWindow->isVisible()))
		{
			uint w = mWindow->getWidth();
			uint h = mWindow->getHeight();
			Ogre::TexturePtr renderTexture = Ogre::TextureManager::getSingleton().createManual("renderTex",
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				Ogre::TextureType::TEX_TYPE_2D, w, h, 0, Ogre::PixelFormat::PF_A8R8G8B8, (int)Ogre::TextureUsage::TU_RENDERTARGET);

			Ogre::RenderTexture* renderTarget = renderTexture->getBuffer()->getRenderTarget();
			Ogre::Viewport* renderViewport = renderTarget->addViewport(mCamera);
			renderViewport->setOverlaysEnabled(false);
			renderViewport->setClearEveryFrame(true);
			renderViewport->setShadowsEnabled(false);
			renderViewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0, 0.0));

			renderTarget->setActive(true);
			renderTarget->setAutoUpdated(false);
			renderTarget->update();
			renderTarget->update();							//for double buffering
			renderTarget->writeContentsToFile(ss.str());

			//clean
			Ogre::TextureManager::getSingleton().remove(renderTexture->getName());
			

			//mWindow->writeContentsToFile(ss.str());
		}
	}


	void OgreWidget::mousePressEvent(QMouseEvent * event)
	{
		this->setFocus();
		
		if (event->button() == Qt::LeftButton)
			enable_spinning = true;

		if (event->button() == Qt::RightButton)
			enable_panning = true;

		haveMoved = false;
	}

	void OgreWidget::mouseMoveEvent(QMouseEvent * event)
	{
		int mouse_x = event->x();
		int mouse_y = event->y();
		int delta_x = mouse_x - last_mouse_x;
		int delta_y = mouse_y - last_mouse_y;
		float delta_f_x = (float)delta_x / (float)width();
		float delta_f_y = (float)delta_y / (float)height();

		if (enable_spinning)
			spinCamera(delta_f_x, delta_f_y);

		if (enable_panning)
			panCamera(delta_f_x * zoom, delta_f_y * zoom);

		last_mouse_x = mouse_x;
		last_mouse_y = mouse_y;
		haveMoved = true;
	}

	void OgreWidget::mouseReleaseEvent(QMouseEvent * event)
	{
		
		if (event->button() == Qt::LeftButton)
			enable_spinning = false;

		if (event->button() == Qt::RightButton)
		{
			enable_panning = false;

			if(haveMoved == false)
			{
				QMenu myMenu;
				QMenu* displayMenu = myMenu.addMenu("Display");

				QAction* displayFullscreen = NULL;
				if (mIsFullscreen)
					displayFullscreen = new QAction(QIcon(":/resources/icons/pause.png"), tr("&Windowed"), this);
				else
					displayFullscreen = new QAction(QIcon(":/resources/icons/play.png"), tr("&Fullscreen"), this);
				displayMenu->addAction(displayFullscreen);

				

				QAction* displayPolygoneModeAct = NULL;
				if (mCamera->getPolygonMode() == Ogre::PM_SOLID)
					displayPolygoneModeAct = new QAction(QIcon(":/resources/icons/pause.png"), tr("&Wireframe"), this);
				else if (mCamera->getPolygonMode() == Ogre::PM_WIREFRAME)
					displayPolygoneModeAct = new QAction(QIcon(":/resources/icons/play.png"), tr("&Point"), this);
				else
					displayPolygoneModeAct = new QAction(QIcon(":/resources/icons/stop.png"), tr("&Solid"), this);
				displayMenu->addAction(displayPolygoneModeAct);

				QAction* displayMeshAct = new QAction(QIcon(  ((mGeometryVisible) ? ":/resources/icons/play.png" : ":/resources/icons/stop.png")), tr("&Geometry"), this);
				displayMenu->addAction(displayMeshAct);

				QAction* displaySkeletonAct = new QAction(QIcon(((mSkeletonVisible) ? ":/resources/icons/play.png" : ":/resources/icons/stop.png")), tr("&Skeleton"), this);
				displayMenu->addAction(displaySkeletonAct);
				
				QAction* showAxesAct = NULL, *showBonesAct = NULL, *showNamesAct = NULL;
				if ((mSkeletonVisible) && (skeleton_debug2))
				{
					showAxesAct = new QAction(QIcon(((skeleton_debug2->axesShown()) ? ":/resources/icons/play.png" : ":/resources/icons/stop.png")), tr("&Sk Axes"), this);
					displayMenu->addAction(showAxesAct);

					showBonesAct = new QAction(QIcon(((skeleton_debug2->bonesShown()) ? ":/resources/icons/play.png" : ":/resources/icons/stop.png")), tr("&Sk bones"), this);
					displayMenu->addAction(showBonesAct);

					showNamesAct = new QAction(QIcon(((skeleton_debug2->namesShown()) ? ":/resources/icons/play.png" : ":/resources/icons/stop.png")), tr("&Sk Names"), this);
					displayMenu->addAction(showNamesAct);
				}

				QAction* displayGridAct = new QAction(QIcon(((mGridVisible) ? ":/resources/icons/play.png" : ":/resources/icons/stop.png")), tr("&Grid"), this);
				displayMenu->addAction(displayGridAct);


				QAction* displayRepereAct = new QAction(QIcon(((mRepereVisible) ? ":/resources/icons/play.png" : ":/resources/icons/stop.png")), tr("&Repere"), this);
				displayMenu->addAction(displayRepereAct);


				

				//////////////////////// debug of mesh by using adapted materials
				QMenu* displayMenu_materials = displayMenu->addMenu("Material");

				QAction* displayApplyMaterialAct_White = new QAction(QIcon(":/resources/icons/apply_material.png"), tr("&Mat White"), this);
				displayMenu_materials->addAction(displayApplyMaterialAct_White);
				QAction* displayApplyMaterialAct_Normals = new QAction(QIcon(":/resources/icons/apply_material.png"), tr("&Mat Normals"), this);
				displayMenu_materials->addAction(displayApplyMaterialAct_Normals);
				QAction* displayApplyMaterialAct_Tangents = new QAction(QIcon(":/resources/icons/apply_material.png"), tr("&Mat Tangents"), this);
				displayMenu_materials->addAction(displayApplyMaterialAct_Tangents);
				QAction* displayApplyMaterialAct_TexCoord = new QAction(QIcon(":/resources/icons/apply_material.png"), tr("&Mat UV"), this);
				displayMenu_materials->addAction(displayApplyMaterialAct_TexCoord);
				QAction* displayApplyMaterialAct_TexCoord_Neg = new QAction(QIcon(":/resources/icons/apply_material.png"), tr("&Mat -UV"), this);
				displayMenu_materials->addAction(displayApplyMaterialAct_TexCoord_Neg);
				QAction* displayApplyMaterialAct_TexCoord2 = new QAction(QIcon(":/resources/icons/apply_material.png"), tr("&Mat UV2"), this);
				displayMenu_materials->addAction(displayApplyMaterialAct_TexCoord2);
				QAction* displayApplyMaterialAct_TexCoord2_Neg = new QAction(QIcon(":/resources/icons/apply_material.png"), tr("&Mat -UV2"), this);
				displayMenu_materials->addAction(displayApplyMaterialAct_TexCoord2_Neg);
				QAction* displayApplyMaterialAct_TexCoord_Rep = new QAction(QIcon(":/resources/icons/apply_material.png"), tr("&Mat UV repet"), this);
				displayMenu_materials->addAction(displayApplyMaterialAct_TexCoord_Rep);
				QAction* displayApplyMaterialAct_TexCoord2_Rep = new QAction(QIcon(":/resources/icons/apply_material.png"), tr("&Mat UV2 repet"), this);
				displayMenu_materials->addAction(displayApplyMaterialAct_TexCoord2_Rep);
				QAction* displayApplyMaterialAct_Color = new QAction(QIcon(":/resources/icons/apply_material.png"), tr("&Mat Color"), this);
				displayMenu_materials->addAction(displayApplyMaterialAct_Color);
				QAction* displayApplyMaterialAct_ColorRGB = new QAction(QIcon(":/resources/icons/apply_material.png"), tr("&Mat ColorRGB"), this);
				displayMenu_materials->addAction(displayApplyMaterialAct_ColorRGB);
				QAction* displayApplyMaterialAct_ColorA = new QAction(QIcon(":/resources/icons/apply_material.png"), tr("&Mat ColorA"), this);
				displayMenu_materials->addAction(displayApplyMaterialAct_ColorA);
				
				QAction* displayApplyMaterialAct_TestOpacity = new QAction(QIcon(":/resources/icons/apply_material.png"), tr("&Mat TestOpacity"), this);
				displayMenu_materials->addAction(displayApplyMaterialAct_TestOpacity);
				QAction* displayApplyMaterialAct_Kmh_Bas_Cor = new QAction(QIcon(":/resources/icons/apply_material.png"), tr("&Mat Kmh_Bas_Cor"), this);
				displayMenu_materials->addAction(displayApplyMaterialAct_Kmh_Bas_Cor);

				

				//////////////////////// 
				QMenu* displayMenu_Camera = displayMenu->addMenu("Camera");

				QAction* displayCameraAct_Charac = new QAction(QIcon(":/resources/icons/apply_material.png"), tr("&Reset for Charac"), this);
				displayMenu_Camera->addAction( displayCameraAct_Charac );
				QAction* displayCameraAct_Stage = new QAction(QIcon(":/resources/icons/apply_material.png"), tr("&Reset for Stages"), this);
				displayMenu_Camera->addAction(displayCameraAct_Stage);


				
				
				
				
				
				////////////////////////		Light
				QMenu* displayMenu_Light = displayMenu->addMenu("Light");

				QAction* displayLightAct_XenoClassic = new QAction(tr("&XviewClassic"), this);
				displayMenu_Light->addAction(displayLightAct_XenoClassic);
				QAction* displayLightAct_CharacSelection = new QAction(tr("&Game Rooster"), this);
				displayMenu_Light->addAction(displayLightAct_CharacSelection);
				QAction* displayLightAct_StageXeno = new QAction(tr("&Xview Stage"), this);
				displayMenu_Light->addAction(displayLightAct_StageXeno);

				QMenu* displayMenu_Light_X = displayMenu_Light->addMenu("X");
				QMenu* displayMenu_Light_Y = displayMenu_Light->addMenu("Y");
				QMenu* displayMenu_Light_Z = displayMenu_Light->addMenu("Z");

				QAction* displayLightAct_X_n1 = new QAction(tr("&-1"), this);
				QAction* displayLightAct_X_0 = new QAction(tr("&0"), this);
				QAction* displayLightAct_X_1 = new QAction(tr("&1"), this);
				displayMenu_Light_X->addAction(displayLightAct_X_n1);
				displayMenu_Light_X->addAction(displayLightAct_X_0);
				displayMenu_Light_X->addAction(displayLightAct_X_1);

				QAction* displayLightAct_Y_n1 = new QAction(tr("&-1"), this);
				QAction* displayLightAct_Y_0 = new QAction(tr("&0"), this);
				QAction* displayLightAct_Y_1 = new QAction(tr("&1"), this);
				displayMenu_Light_Y->addAction(displayLightAct_Y_n1);
				displayMenu_Light_Y->addAction(displayLightAct_Y_0);
				displayMenu_Light_Y->addAction(displayLightAct_Y_1);

				QAction* displayLightAct_Z_n1 = new QAction(tr("&-1"), this);
				QAction* displayLightAct_Z_0 = new QAction(tr("&0"), this);
				QAction* displayLightAct_Z_1 = new QAction(tr("&1"), this);
				displayMenu_Light_Z->addAction(displayLightAct_Z_n1);
				displayMenu_Light_Z->addAction(displayLightAct_Z_0);
				displayMenu_Light_Z->addAction(displayLightAct_Z_1);


				////////////////////////		Background
				
				QMenu* displayMenu_Bg = displayMenu->addMenu("Backgrd");

				struct actionColor_tmp
				{
					QAction* action;
					Ogre::ColourValue color;

					actionColor_tmp(QAction* action, Ogre::ColourValue color) { this->action = action; this->color = color; }
				};


				std::vector<actionColor_tmp> listBkColorAction;
			
				for (size_t i = 0, nb = mListBackgroundColor.size(); i < nb; i++)
				{
					listBkColorAction.push_back(actionColor_tmp(new QAction(tr( (Ogre::String("&") + mListBackgroundColor.at(i).name).c_str()) , this), mListBackgroundColor.at(i).color));
					displayMenu_Bg->addAction(listBkColorAction.back().action);
				}
				

				


				//menuBar()->addSeparator();

				QAction* selectedItem = myMenu.exec(event->globalPos());

				if (selectedItem == displayFullscreen)
				{
					if (!mIsFullscreen)
					{
						//refresh parent and grand parent because there is lots of operation on widgets, hierarchy change
						mpParentWidget = this->parentWidget();
						mpGrandParentWidget = mpParentWidget->parentWidget();

						mpParentWidget->setParent(0);
						mpParentWidget->showFullScreen();
						mpParentWidget->show();

						QPoint position = this->pos();
						this->move(0, 0);

						QLayout* layout = mpParentWidget->layout();
						if (layout)
						{
							layout->setSpacing(0);
							layout->setMargin(0);
						}

						mIsFullscreen = true;

					} else {

						QLayout* layout = mpParentWidget->layout();
						if (layout)
						{
							layout->setSpacing(6);
							layout->setMargin(11);
						}
						mpParentWidget->showNormal();

						this->move(9, 9);

						if (mpGrandParentWidget->layout())
						{
							mpGrandParentWidget->layout()->addWidget(mpParentWidget);
						}
						else {
							mpParentWidget->setParent(mpGrandParentWidget);
						}

						mpParentWidget->show();
						mpGrandParentWidget->show();


						mIsFullscreen = false;
					}


				}else if (selectedItem == displayPolygoneModeAct){
					switch (mCamera->getPolygonMode())
					{
					case Ogre::PM_SOLID:
						mCamera->setPolygonMode(Ogre::PM_WIREFRAME);
						break;

					case Ogre::PM_WIREFRAME:
						mCamera->setPolygonMode(Ogre::PM_POINTS);
						break;

					case Ogre::PM_POINTS:
						mCamera->setPolygonMode(Ogre::PM_SOLID);
						break;
					}
				}else if (selectedItem == displayMeshAct)
					swapGeometryVisible();
				else if (selectedItem == displaySkeletonAct)
					swapSkeletonVisible();
				else if (selectedItem == displayGridAct)
				{
					mGridVisible = !mGridVisible;
					mGridNode->setVisible(mGridVisible);
				}
				else if (selectedItem == displayRepereAct)
				{
					mRepereVisible = !mRepereVisible;
					mRepereNode->setVisible(mRepereVisible);
				}
				else if ((selectedItem == displayApplyMaterialAct_White) 
					|| (selectedItem == displayApplyMaterialAct_Normals) 
					|| (selectedItem == displayApplyMaterialAct_Tangents)
					|| (selectedItem == displayApplyMaterialAct_TexCoord)
					|| (selectedItem == displayApplyMaterialAct_TexCoord2)
					|| (selectedItem == displayApplyMaterialAct_TexCoord_Neg)
					|| (selectedItem == displayApplyMaterialAct_TexCoord2_Neg)
					|| (selectedItem == displayApplyMaterialAct_TexCoord_Rep)
					|| (selectedItem == displayApplyMaterialAct_TexCoord2_Rep)
					|| (selectedItem == displayApplyMaterialAct_Color)
					|| (selectedItem == displayApplyMaterialAct_ColorRGB)
					|| (selectedItem == displayApplyMaterialAct_ColorA)
					|| (selectedItem == displayApplyMaterialAct_TestOpacity)
					|| (selectedItem == displayApplyMaterialAct_Kmh_Bas_Cor)
					)
				{
					Ogre::String materialName = "White";
					if (selectedItem == displayApplyMaterialAct_White)
						materialName = "White_Light";
					else if (selectedItem == displayApplyMaterialAct_Normals)
						materialName = "Normal";
					else if (selectedItem == displayApplyMaterialAct_Tangents)
						materialName = "Tangent";
					else if (selectedItem == displayApplyMaterialAct_TexCoord)
						materialName = "TexCoord";
					else if (selectedItem == displayApplyMaterialAct_TexCoord2)
						materialName = "TexCoord2";
					else if (selectedItem == displayApplyMaterialAct_TexCoord_Neg)
						materialName = "TexCoord_Neg";
					else if (selectedItem == displayApplyMaterialAct_TexCoord2_Neg)
						materialName = "TexCoord2_Neg";
					else if (selectedItem == displayApplyMaterialAct_TexCoord_Rep)
						materialName = "TexCoord_Rep";
					else if (selectedItem == displayApplyMaterialAct_TexCoord2_Rep)
						materialName = "TexCoord2_Rep";
					else if (selectedItem == displayApplyMaterialAct_Color)
						materialName = "Color";
					else if (selectedItem == displayApplyMaterialAct_ColorRGB)
						materialName = "Color_RGB";
					else if (selectedItem == displayApplyMaterialAct_ColorA)
						materialName = "Color_A";
					else if (selectedItem == displayApplyMaterialAct_TestOpacity)
						//materialName = "TestOpacity";
						materialName = "Test";
					else if (selectedItem == displayApplyMaterialAct_Kmh_Bas_Cor)
						materialName = "Kmh_Bas_Cor";
					

					for (list<EMDOgre *>::iterator itm = emd_list.begin(); itm != emd_list.end(); itm++)
					{
						if ((*itm)->getTag() == "EMD")
							(*itm)->setMaterialName(materialName);
					}



				}else if ((selectedItem == displayCameraAct_Charac) || (selectedItem == displayCameraAct_Stage) ) {
					if (selectedItem == displayCameraAct_Charac)
						resetCamera_Charac();
					else if (selectedItem == displayCameraAct_Stage)
						resetCamera_Stage();



				}else if ( (selectedItem == displayLightAct_XenoClassic) 
						|| (selectedItem == displayLightAct_CharacSelection)
						|| (selectedItem == displayLightAct_StageXeno)
						|| (selectedItem == displayLightAct_X_n1)
						|| (selectedItem == displayLightAct_X_0)
						|| (selectedItem == displayLightAct_X_1)
						|| (selectedItem == displayLightAct_Y_n1)
						|| (selectedItem == displayLightAct_Y_0)
						|| (selectedItem == displayLightAct_Y_1)
						|| (selectedItem == displayLightAct_Z_n1)
						|| (selectedItem == displayLightAct_Z_0)
						|| (selectedItem == displayLightAct_Z_1)
						)
				{
					Ogre::Light* light = mSceneMgr->getLight("Xenoviewer Direct Light");
					if (light)
					{
						Ogre::Vector3 lightPosition = -light->getDirection();
						Ogre::Real scalefactor = (lightPosition.length() > 10.0) ? 1000.0 : 1.0;
						if (lightPosition.x > 0.0001) { lightPosition.x = 1.0; } else if (lightPosition.x < 0.0001) { lightPosition.x = -1.0; } else { lightPosition.x = 0.0; }
						if (lightPosition.y > 0.0001) { lightPosition.y = 1.0; } else if (lightPosition.y < 0.0001) { lightPosition.y = -1.0; } else { lightPosition.y = 0.0; }
						if (lightPosition.z > 0.0001) { lightPosition.z = 1.0; } else if (lightPosition.z < 0.0001) { lightPosition.z = -1.0; } else { lightPosition.z = 0.0; }

						if (selectedItem == displayLightAct_XenoClassic)
						{
							lightPosition = Ogre::Vector3(-1, 1, -1);
							scalefactor = 1.0;
						}else if (selectedItem == displayLightAct_CharacSelection){
							lightPosition = Ogre::Vector3(1, -1, -1);
							scalefactor = 1.0;
						}else if (selectedItem == displayLightAct_StageXeno) {
							lightPosition = Ogre::Vector3(1, -1, -1);
							scalefactor = 1000.0;
						}else if (selectedItem == displayLightAct_X_n1)
							lightPosition.x = -1.0;
						else if (selectedItem == displayLightAct_X_0)
							lightPosition.x = 0.0;
						else if (selectedItem == displayLightAct_X_1)
							lightPosition.x = 1.0;
						else if (selectedItem == displayLightAct_Y_n1)
							lightPosition.y = -1.0;
						else if (selectedItem == displayLightAct_Y_0)
							lightPosition.y = 0.0;
						else if (selectedItem == displayLightAct_Y_1)
							lightPosition.y = 1.0;
						else if (selectedItem == displayLightAct_Z_n1)
							lightPosition.z = -1.0;
						else if (selectedItem == displayLightAct_Z_0)
							lightPosition.z = 0.0;
						else if (selectedItem == displayLightAct_Z_1)
							lightPosition.z = 1.0;

						

						light->setDirection(-lightPosition.normalisedCopy());
						light->setPosition( lightPosition.normalisedCopy() * scalefactor );
					}


				}else {

					for (size_t i = 0, nb = listBkColorAction.size(); i < nb; i++)
					{
						if (selectedItem == listBkColorAction.at(i).action)
						{
							mViewport->setBackgroundColour(listBkColorAction.at(i).color);
							break;
						}
					}

				}
				

				
				if ((mSkeletonVisible) && (skeleton_debug2))
				{
					if (selectedItem == showAxesAct)
						skeleton_debug2->showAxes(!skeleton_debug2->axesShown());
					else if (selectedItem == showBonesAct)
						skeleton_debug2->showBones(!skeleton_debug2->bonesShown());
					else if (selectedItem == showNamesAct)
						skeleton_debug2->showNames(!skeleton_debug2->namesShown());
				}
			}

		}
	}

	void OgreWidget::wheelEvent(QWheelEvent * event)
	{
		zoomCamera(event->delta());
	}

	




	///////////////////////////////// camera manipulations

	void OgreWidget::spinCamera(float delta_x, float delta_y)
	{
		viewer_angle_x += delta_x * -4.0f;
		viewer_angle_y += delta_y * -4.0f;

		if (viewer_angle_x >= Ogre::Math::TWO_PI) viewer_angle_x -= Ogre::Math::TWO_PI;
		if (viewer_angle_x < 0) viewer_angle_x += Ogre::Math::TWO_PI;

		if (viewer_angle_y >= Ogre::Math::HALF_PI - 0.1) viewer_angle_y = Ogre::Math::HALF_PI - 0.1;
		if (viewer_angle_y < -Ogre::Math::HALF_PI + 0.1) viewer_angle_y = -Ogre::Math::HALF_PI + 0.1;
	}

	void OgreWidget::panCamera(float delta_x, float delta_y)
	{
		viewer_center += mCamera->getOrientation() * Ogre::Vector3(delta_x * -2.0f, -delta_y * -2.0f, 0.0);
	}

	void OgreWidget::zoomCamera(float delta)
	{
		float factor = -0.001f;
		if (zoom > 10.0) factor = -0.01f;
		if (zoom > 100.0) factor = -0.1f;
		if (zoom > 1000.0) factor = -.1f;

		zoom += delta * factor;

		if (zoom < 0.01f) zoom = 0.01f;
	}

	void OgreWidget::repositionCamera()
	{
		Ogre::Quaternion rotation_x;
		Ogre::Quaternion rotation_y;
		rotation_x.FromAngleAxis(Ogre::Radian(viewer_angle_x), Ogre::Vector3::UNIT_Y);
		rotation_y.FromAngleAxis(Ogre::Radian(viewer_angle_y), Ogre::Vector3::UNIT_X);

		Ogre::Vector3 new_position = viewer_center + ((rotation_x * rotation_y) * Ogre::Vector3(0, 0, 3.0 * zoom));
		mCamera->setPosition(new_position);
		mCamera->lookAt(viewer_center);
	}

	void OgreWidget::resetCamera_Charac()
	{
		viewer_center = Ogre::Vector3(0, 0.5, 0);
		viewer_angle_x = Ogre::Math::PI;
		viewer_angle_y = 0.0f;
		zoom = 1.0f;
	}

	void OgreWidget::resetCamera_Stage()
	{
		viewer_center = Ogre::Vector3(0, 0.0, 0);
		viewer_angle_x = Ogre::Math::PI;
		viewer_angle_y = -Ogre::Math::PI/4.0;
		zoom = 50.0f;
	}

	void OgreWidget::swapGeometryVisible(void)
	{
		mGeometryVisible = !mGeometryVisible;
	}


	void OgreWidget::swapSkeletonVisible(void)
	{
		mSkeletonVisible = !mSkeletonVisible;
	}









	///////////////////////////////// Load Files


	void OgreWidget::addFiles(const QStringList& pathList, list<EMDOgre *> &target_emd_list, list<ESKOgre *> &target_esk_list, list<EANOgre *> &target_ean_list)
	{
		for (QStringList::const_iterator it = pathList.begin(); it != pathList.end(); it++)
		{
			string filename = (*it).toStdString();
			addFile(filename, target_emd_list, target_esk_list, target_ean_list);
		}
	}


	void OgreWidget::addFile(string filename, list<EMDOgre *> &target_emd_list, list<ESKOgre *> &target_esk_list, list<EANOgre *> &target_ean_list)
	{
		string extension = LibXenoverse::extensionFromFilename(filename, true);

		if (extension == "emd")
		{
			addFileEMD(filename, target_emd_list);
		}else if (extension == "esk"){
			addFileESK(filename, target_esk_list);
		}else if (extension == "nsk"){
			addFileNSK(filename, target_emd_list, target_esk_list);
		}else if (extension == "emo") {
			addFileEMO(filename, target_emd_list, target_esk_list);
		}else if ((extension == "ean")||(extension == "ema")){
			addFileEAN(filename, target_ean_list);
		}else if (extension == "emb"){
			SHOW_ERROR("EMB files are automatically loaded with the EMD file. Load the EMD file instead!");
		}else if (extension == "emm"){
			SHOW_ERROR("EMM files are automatically loaded with the EMD file. Load the EMD file instead!");
		}else{
			SHOW_ERROR("File Extension " + QString(extension.c_str()) + " for file " + QString(filename.c_str()) + " not recognized. Xenoviewer can't read this format.");
		}
	}

	

	void OgreWidget::addFileEAN(string filename, list<EANOgre *> &target_ean_list)
	{
		string ean_name = LibXenoverse::nameFromFilenameNoExtension(filename, true);

		// Search for an EAN with the same name
		for (list<EANOgre *>::iterator it = ean_list.begin(); it != ean_list.end(); it++)
		{
			if ((*it)->getName() == ean_name)
			{
				SHOW_ERROR("A EAN Animation Pack with the name " + QString(ean_name.c_str()) + " already exists! (FIXME: Implement prompt for replacing already loaded files)");
				return;
			}
		}

		EANOgre *animation = new EANOgre();
		if (animation->load(filename))
		{
			for (list<ESKOgre *>::iterator it = esk_list.begin(); it != esk_list.end(); it++)
			{
				animation->createOgreAnimations(*it);
				(*it)->refreshAnimations();
			}

			ean_list.push_back(animation);
			target_ean_list.push_back(animation);
		}else {
			delete animation;
			SHOW_ERROR("Invalid EAN Animation Pack. Is " + QString(filename.c_str()) + " valid?");
			return;
		}
	}


	void OgreWidget::addFileESK(string filename, list<ESKOgre *> &target_esk_list)
	{
		string esk_name = LibXenoverse::nameFromFilenameNoExtension(filename, true);

		// Search for an ESK with the same name
		for (list<ESKOgre *>::iterator it = esk_list.begin(); it != esk_list.end(); it++)
		{
			if ((*it)->getName() == esk_name)
			{
				SHOW_ERROR("A ESK Skeleton with the name " + QString(esk_name.c_str()) + " already exists! (FIXME: Implement prompt for replacing already loaded files)");
				return;
			}
		}

		ESKOgre *skeleton = new ESKOgre();
		if (skeleton->load(filename))
		{
			skeleton->createOgreSkeleton(mSceneMgr);
			esk_list.push_back(skeleton);
			target_esk_list.push_back(skeleton);

			for (list<EANOgre *>::iterator it = ean_list.begin(); it != ean_list.end(); it++)
				(*it)->createOgreAnimations(skeleton);

			skeleton->refreshAnimations();
		}else {
			delete skeleton;
			SHOW_ERROR("Invalid ESK Skeleton. Is " + QString(filename.c_str()) + " valid?");
			return;
		}
	}


	void OgreWidget::addFileEMD(string filename, list<EMDOgre *> &target_emd_list)
	{
		string emb_filename     = LibXenoverse::filenameNoExtension(filename) + ".emb";
		string emb_dyt_filename = LibXenoverse::filenameNoExtension(filename) + ".dyt.emb";
		string emm_filename     = LibXenoverse::filenameNoExtension(filename) + ".emm";
		string emd_name         = LibXenoverse::nameFromFilenameNoExtension(filename, true);

		// Search for an EMD with the same name
		for (list<EMDOgre *>::iterator it = emd_list.begin(); it != emd_list.end(); it++)
		{
			if ((*it)->getName() == emd_name)
			{
				SHOW_ERROR("A EMD Model Pack with the name " + QString(emd_name.c_str()) + " already exists! (FIXME: Implement prompt for replacing already loaded files)");
				return;
			}
		}

		EMBOgre *texture_pack = NULL;
		EMBOgre *texture_dyt_pack = NULL;
		EMMOgre *material = NULL;
		EMDOgre *model = NULL;

		if (!LibXenoverse::fileCheck(emb_filename))
		{
			emb_filename = "";
			//SHOW_ERROR("No EMB Pack with the name " + QString(emb_filename.c_str()) + " found. Make sure it's on the same folder as the EMD file you're adding and it's not open by any other application!");
			//return;
		}


		if (!LibXenoverse::fileCheck(emb_dyt_filename))
		{
			emb_dyt_filename = emb_filename;		//certain charac have only emb file , no dyt, or no need.
			//SHOW_ERROR("No EMB DYT Pack with the name " + QString(emb_dyt_filename.c_str()) + " found. Make sure it's on the same folder as the EMD file you're adding and it's not open by any other application!");
			//return;
		}

		if (!LibXenoverse::fileCheck(emm_filename))
		{
			emm_filename = "";
			//SHOW_ERROR("No EMM Pack with the name " + QString(emm_filename.c_str()) + " found. Make sure it's on the same folder as the EMD file you're adding and it's not open by any other application!");
			//return;
		}

		texture_pack = 0;

		if (emb_filename.length()!=0)
		{
			texture_pack = new EMBOgre();
			if (texture_pack->load(emb_filename))
			{
				texture_pack->createOgreTextures();
			}else{
				SHOW_ERROR("Invalid EMB Texture Pack. Is " + QString(emb_filename.c_str()) + " valid?");
				//goto abort_clean;
				delete texture_pack;
				texture_pack = 0;
			}
		}

		texture_dyt_pack = 0;
		if (emb_dyt_filename.length() != 0)
		{
			texture_dyt_pack = new EMBOgre();
			if (texture_dyt_pack->load(emb_dyt_filename))
			{
				texture_dyt_pack->createOgreTextures();
			}else {
				SHOW_ERROR("Invalid EMB DYT Texture Pack. Is " + QString(emb_dyt_filename.c_str()) + " valid?");
				//goto abort_clean;
				delete texture_dyt_pack;
				texture_dyt_pack = 0;
			}
		}

		
		material = 0;

		if (emm_filename.length() != 0)
		{
			material = new EMMOgre();
			if (material->load(emm_filename))
			{
				material->setTexturePack(texture_pack);
				material->setDYTTexturePack(texture_dyt_pack);
				material->createOgreMaterials(sds_list);
			}else {
				SHOW_ERROR("Invalid EMM Material Pack. Is " + QString(emm_filename.c_str()) + " valid?");
				//goto abort_clean;
				delete material;
				material = 0;
			}
		}

		

		
		model = new EMDOgre();

		if (model->load(filename))
		{
			model->setMaterialPack(material);
			Ogre::SceneNode *emd_root_node = model->build(mSceneMgr);
			emd_list.push_back(model);
			target_emd_list.push_back(model);
		}
		else {
			SHOW_ERROR("Invalid EMD Model Pack. Is " + QString(filename.c_str()) + " valid?");
			goto abort_clean;
		}

		
		return;

	abort_clean:
		delete material;
		delete texture_pack;
		delete texture_dyt_pack;
		delete model;
		return;
	}






	void OgreWidget::addFileNSK(string filename, list<EMDOgre *> &target_emd_list, list<ESKOgre *> &target_esk_list)
	{
		string emb_filename = LibXenoverse::filenameNoExtension(filename) + ".emb";
		string emb_dyt_filename = LibXenoverse::filenameNoExtension(filename) + ".dyt.emb";
		string emm_filename = LibXenoverse::filenameNoExtension(filename) + ".emm";
		string emd_name = LibXenoverse::nameFromFilenameNoExtension(filename, true);
		string esk_name = LibXenoverse::nameFromFilenameNoExtension(filename, true);

	
		// Search for an EMD with the same name
		for (list<EMDOgre *>::iterator it = emd_list.begin(); it != emd_list.end(); it++)
		{
			if ((*it)->getName() == emd_name)
			{
				SHOW_ERROR("A EMD Model Pack with the name " + QString(emd_name.c_str()) + " already exists! (FIXME: Implement prompt for replacing already loaded files)");
				return;
			}
		}
		// Search for an ESK with the same name
		for (list<ESKOgre *>::iterator it = esk_list.begin(); it != esk_list.end(); it++)
		{
			if ((*it)->getName() == esk_name)
			{
				SHOW_ERROR("A ESK Skeleton with the name " + QString(esk_name.c_str()) + " already exists! (FIXME: Implement prompt for replacing already loaded files)");
				return;
			}
		}


		EMBOgre *texture_pack = NULL;
		EMBOgre *texture_dyt_pack = NULL;
		EMMOgre *material = NULL;

		if (!LibXenoverse::fileCheck(emb_filename))
		{
			emb_filename = "";
			//SHOW_ERROR("No EMB Pack with the name " + QString(emb_filename.c_str()) + " found. Make sure it's on the same folder as the EMD file you're adding and it's not open by any other application!");
			//return;
		}


		if (!LibXenoverse::fileCheck(emb_dyt_filename))
		{
			emb_dyt_filename = emb_filename;		//certain charac have only emb file , no dyt, or no need.
			//SHOW_ERROR("No EMB DYT Pack with the name " + QString(emb_dyt_filename.c_str()) + " found. Make sure it's on the same folder as the EMD file you're adding and it's not open by any other application!");
			//return;
		}

		if (!LibXenoverse::fileCheck(emm_filename))
		{
			emm_filename = "";
			//SHOW_ERROR("No EMM Pack with the name " + QString(emm_filename.c_str()) + " found. Make sure it's on the same folder as the EMD file you're adding and it's not open by any other application!");
			//return;
		}

		texture_pack = 0;

		if (emb_filename.length() != 0)
		{
			texture_pack = new EMBOgre();
			if (texture_pack->load(emb_filename))
			{
				texture_pack->createOgreTextures();
			}
			else {
				SHOW_ERROR("Invalid EMB Texture Pack. Is " + QString(emb_filename.c_str()) + " valid?");
				//goto abort_clean;
				delete texture_pack;
				texture_pack = 0;
			}
		}

		texture_dyt_pack = 0;
		if (emb_dyt_filename.length() != 0)
		{
			texture_dyt_pack = new EMBOgre();
			if (texture_dyt_pack->load(emb_dyt_filename))
			{
				texture_dyt_pack->createOgreTextures();
			}
			else {
				SHOW_ERROR("Invalid EMB DYT Texture Pack. Is " + QString(emb_dyt_filename.c_str()) + " valid?");
				//goto abort_clean;
				delete texture_dyt_pack;
				texture_dyt_pack = 0;
			}
		}


		material = 0;

		if (emm_filename.length() != 0)
		{
			material = new EMMOgre();
			if (material->load(emm_filename))
			{
				material->setTexturePack(texture_pack);
				material->setDYTTexturePack(texture_dyt_pack);
				material->createOgreMaterials(sds_list);
			}
			else {
				SHOW_ERROR("Invalid EMM Material Pack. Is " + QString(emm_filename.c_str()) + " valid?");
				//goto abort_clean;
				delete material;
				material = 0;
			}
		}



		NSK* nsk = new NSK();
		if (!nsk->load(filename))
		{
			SHOW_ERROR("Invalid Nsk Model Pack. Is " + QString(filename.c_str()) + " valid?");
			goto abort_clean;

		}else{

			ESKOgre *skeleton = new ESKOgre(nsk->getEsk());
			skeleton->createOgreSkeleton(mSceneMgr);
			esk_list.push_back(skeleton);
			target_esk_list.push_back(skeleton);

			for (list<EANOgre *>::iterator it = ean_list.begin(); it != ean_list.end(); it++)
				(*it)->createOgreAnimations(skeleton);

			skeleton->refreshAnimations();


			EMDOgre* model = new EMDOgre(nsk->getEmd());
			model->setMaterialPack(material);
			
			model->setSkeleton(skeleton);

			Ogre::SceneNode *emd_root_node = model->build(mSceneMgr);
			emd_list.push_back(model);
			target_emd_list.push_back(model);

			//todo update item in tree, but I think they are not created yet ... what a mess the organisation of the tool.
			//src_model_pack_item->updateText();
			//dest_skeleton_item->setExpanded(true);
		}

		return;

	abort_clean:

		if (material)
			delete material;
		if (texture_pack)
			delete texture_pack;
		if (texture_dyt_pack)
			delete texture_dyt_pack;
		if(nsk)
			delete nsk;
		return;
	}





	void OgreWidget::addFileEMO(string filename, list<EMDOgre *> &target_emd_list, list<ESKOgre *> &target_esk_list)
	{
		string emb_filename = LibXenoverse::filenameNoExtension(filename) + ".emb";
		string emb_dyt_filename = LibXenoverse::filenameNoExtension(filename) + ".dyt.emb";
		string emm_filename = LibXenoverse::filenameNoExtension(filename) + ".emm";
		string emd_name = LibXenoverse::nameFromFilenameNoExtension(filename, true);
		string esk_name = LibXenoverse::nameFromFilenameNoExtension(filename, true);


		// Search for an EMD with the same name
		for (list<EMDOgre *>::iterator it = emd_list.begin(); it != emd_list.end(); it++)
		{
			if ((*it)->getName() == emd_name)
			{
				SHOW_ERROR("A EMD Model Pack with the name " + QString(emd_name.c_str()) + " already exists! (FIXME: Implement prompt for replacing already loaded files)");
				return;
			}
		}
		// Search for an ESK with the same name
		for (list<ESKOgre *>::iterator it = esk_list.begin(); it != esk_list.end(); it++)
		{
			if ((*it)->getName() == esk_name)
			{
				SHOW_ERROR("A ESK Skeleton with the name " + QString(esk_name.c_str()) + " already exists! (FIXME: Implement prompt for replacing already loaded files)");
				return;
			}
		}


		EMBOgre *texture_pack = NULL;
		EMBOgre *texture_dyt_pack = NULL;
		EMMOgre *material = NULL;

		if (!LibXenoverse::fileCheck(emb_filename))
		{
			emb_filename = "";
			//SHOW_ERROR("No EMB Pack with the name " + QString(emb_filename.c_str()) + " found. Make sure it's on the same folder as the EMD file you're adding and it's not open by any other application!");
			//return;
		}


		if (!LibXenoverse::fileCheck(emb_dyt_filename))
		{
			emb_dyt_filename = emb_filename;		//certain charac have only emb file , no dyt, or no need.
			//SHOW_ERROR("No EMB DYT Pack with the name " + QString(emb_dyt_filename.c_str()) + " found. Make sure it's on the same folder as the EMD file you're adding and it's not open by any other application!");
			//return;
		}

		if (!LibXenoverse::fileCheck(emm_filename))
		{
			emm_filename = "";
			//SHOW_ERROR("No EMM Pack with the name " + QString(emm_filename.c_str()) + " found. Make sure it's on the same folder as the EMD file you're adding and it's not open by any other application!");
			//return;
		}

		texture_pack = 0;

		if (emb_filename.length() != 0)
		{
			texture_pack = new EMBOgre();
			if (texture_pack->load(emb_filename))
			{
				texture_pack->createOgreTextures();
			}
			else {
				SHOW_ERROR("Invalid EMB Texture Pack. Is " + QString(emb_filename.c_str()) + " valid?");
				//goto abort_clean;
				delete texture_pack;
				texture_pack = 0;
			}
		}

		texture_dyt_pack = 0;
		if (emb_dyt_filename.length() != 0)
		{
			texture_dyt_pack = new EMBOgre();
			if (texture_dyt_pack->load(emb_dyt_filename))
			{
				texture_dyt_pack->createOgreTextures();
			}
			else {
				SHOW_ERROR("Invalid EMB DYT Texture Pack. Is " + QString(emb_dyt_filename.c_str()) + " valid?");
				//goto abort_clean;
				delete texture_dyt_pack;
				texture_dyt_pack = 0;
			}
		}


		material = 0;

		if (emm_filename.length() != 0)
		{
			material = new EMMOgre();
			if (material->load(emm_filename))
			{
				material->setTexturePack(texture_pack);
				material->setDYTTexturePack(texture_dyt_pack);
				material->createOgreMaterials(sds_list);
			}
			else {
				SHOW_ERROR("Invalid EMM Material Pack. Is " + QString(emm_filename.c_str()) + " valid?");
				//goto abort_clean;
				delete material;
				material = 0;
			}
		}



		EMO* emo = new EMO();
		if (!emo->load(filename))
		{
			SHOW_ERROR("Invalid Emo Model Pack. Is " + QString(filename.c_str()) + " valid?");
			goto abort_clean;

		}else {

			EMDOgre* model = new EMDOgre();
			ESKOgre* skeleton = new ESKOgre();
			emo->writeEmdEsk(model, skeleton);
			delete emo;
			emo = 0;

			skeleton->createOgreSkeleton(mSceneMgr);
			esk_list.push_back(skeleton);
			target_esk_list.push_back(skeleton);

			for (list<EANOgre *>::iterator it = ean_list.begin(); it != ean_list.end(); it++)
				(*it)->createOgreAnimations(skeleton);

			skeleton->refreshAnimations();


			
			model->setMaterialPack(material);
			model->setSkeleton(skeleton);
			Ogre::SceneNode *emd_root_node = model->build(mSceneMgr);
			emd_list.push_back(model);
			target_emd_list.push_back(model);

			//todo update item in tree, but I think they are not created yet ... what a mess the organisation of the tool.
			//src_model_pack_item->updateText();
			//dest_skeleton_item->setExpanded(true);
		}

		return;

	abort_clean:

		if (material)
			delete material;
		if (texture_pack)
			delete texture_pack;
		if (texture_dyt_pack)
			delete texture_dyt_pack;
		if (emo)
			delete emo;
		return;
	}


	bool OgreWidget::installShaders()
	{
		QFileDialog dialog(this);
		dialog.setFileMode(QFileDialog::Directory);

		QMessageBox::about(NULL, "Xenoviewer Installation", "For previewing materials correctly, Xenoviewer requires "
			"installing the game's shaders for the first time. Select the directory <b>adam_shader</b> "
			"inside the extracted contents of <b>data.cpk</b> after pressing 'Ok'.");

		QString dir = QFileDialog::getExistingDirectory(this, "Choose Shader Folder");

		if (dir.size())
		{
			// Make Shader Folder
			QDir().mkdir("adam_shader");

			string dir_std = dir.toStdString();

			
			//search for shader and sds (shader program) of any Game, not only for DBX.
			vector<string> shader_names;
			/*
			shader_names.push_back("shader_age_ps");
			shader_names.push_back("shader_age_vs");
			shader_names.push_back("shader_default_ps");
			shader_names.push_back("shader_default_vs");
			*/

			QDir src_folder = QDir(QString(dir_std.c_str()));
			QFileInfoList files = src_folder.entryInfoList();
			foreach(QFileInfo file, files)
			{
				if (file.isDir())
					continue;
				if (file.suffix().toStdString() != "emz")
					continue;

				shader_names.push_back(file.baseName().toStdString());
			}
			
			
			
			for (size_t i = 0; i < shader_names.size(); i++)
			{
				string full_path = dir_std + "/" + shader_names[i] + ".emz";

				EMZ *emz_pack = new EMZ();
				if (emz_pack->load(full_path))
				{
					string new_extension = emz_pack->detectNewExtension();
					string new_filename = "adam_shader/" + shader_names[i] + new_extension;
					emz_pack->saveUncompressed(new_filename);
				}
				delete emz_pack;
			}
		}
		else {
			return false;
		}

		return true;
	}
	

	

	void OgreWidget::getItemLists(list<EMDOgre *> &target_emd_list, list<ESKOgre *> &target_esk_list, list<EANOgre *> &target_ean_list)
	{
		target_emd_list = emd_list;
		target_esk_list = esk_list;
		target_ean_list = ean_list;
	}

	
}
