#include "OgreWidget.h"
#include "OgreFreeImageCodec.h"
#include <QLabel>



namespace QtOgre
{
	OgreWidget::OgreWidget(QWidget* pParentWidget, Qt::WindowFlags f)
		:QWidget(pParentWidget, f | Qt::MSWindowsOwnDC)
		, mWindow(0)
		, mpParentWidget(pParentWidget)
	{
		QWidget *q_parent = dynamic_cast <QWidget *> (parent());

		setMouseTracking(true);
		grabKeyboard();

		setAttribute(Qt::WA_PaintOnScreen);
		setContextMenuPolicy(Qt::CustomContextMenu);

		Ogre::FreeImageCodec::PREMULTIPLYONDECODE = false;

		mRoot = new Ogre::Root();
		initialiseOgre();

		

		Ogre::NameValuePairList ogreWindowParams;
		ogreWindowParams["FSAA"] = "8 (Quality)";
		ogreWindowParams["vsync"] = "true";
		ogreWindowParams["parentWindowHandle"] = Ogre::StringConverter::toString((unsigned long)this->parentWidget()->winId());

		mWindow = Ogre::Root::getSingletonPtr()->createRenderWindow("OgreWindow", width(), height(), false, &ogreWindowParams);
		mViewport = 0;

		mWindow->setActive(true);
		mWindow->setVisible(true);
		mWindow->setAutoUpdated(false);

		WId window_id;
		mWindow->getCustomAttribute("HWND", &window_id);

		QWidget::create(window_id);
		moveAndResize();

		mUpdateTimer = new QTimer;
		QObject::connect(mUpdateTimer, SIGNAL(timeout()), this, SLOT(update()));
		mUpdateTimer->start(10);

		// Create Ogre Scene
		chooseSceneManager();
		createCamera();
		createViewports();
		loadResources();
		createScene();

		mCurrentTime = 0.0;
		mAnimationStop = false;
		mAnimationPlaying = true;
		mAnimationLoopEnable = true;
		mAnimationForceOneUpdate = false;
		mAnimationStatus = "";
	}

	OgreWidget::~OgreWidget()
	{
	}


	void OgreWidget::createDebug(QWidget* pParentWidget)
	{
		mAnimationStatusWidget = pParentWidget->findChild<QLabel *>("PlayAnimationText");
		if(mAnimationStatusWidget)
			mAnimationStatusWidget->setText(QString(mAnimationStatus.c_str()));
	}

	

	Ogre::RenderWindow* OgreWidget::getOgreRenderWindow() const
	{
		return mWindow;
	}



	void OgreWidget::paintEvent(QPaintEvent*)
	{
		Ogre::Root::getSingleton()._fireFrameStarted();
		mWindow->update();
		Ogre::Root::getSingleton()._fireFrameRenderingQueued();
		Ogre::Root::getSingleton()._fireFrameEnded();

		if (mAnimationStatusWidget)
			mAnimationStatusWidget->setText(QString(mAnimationStatus.c_str()));
	}

	void OgreWidget::resizeEvent(QResizeEvent*)
	{
		moveAndResize();
	}

	void OgreWidget::moveAndResize()
	{
		mWindow->reposition(x(), y());
		mWindow->resize(width(), height());
		mWindow->windowMovedOrResized();

		for (int ct = 0; ct < mWindow->getNumViewports(); ++ct)
		{
			Ogre::Viewport* pViewport = mWindow->getViewport(ct);

			Ogre::Camera* pCamera = pViewport->getCamera();
			pCamera->setAspectRatio(static_cast<Ogre::Real>(pViewport->getActualWidth()) / static_cast<Ogre::Real>(pViewport->getActualHeight()));
		}
	}

	void OgreWidget::initialiseOgre(void)
	{
		Ogre::RenderSystem* Direct3D9RenderSystem = 0;

		try
		{
#if _DEBUG
			mRoot->loadPlugin("RenderSystem_Direct3D9");
#else
			mRoot->loadPlugin("RenderSystem_Direct3D9");
#endif
		}
		catch (...)
		{
			qWarning("Failed to load Direct3D9 plugin");
		}

		Ogre::RenderSystemList list = Ogre::Root::getSingletonPtr()->getAvailableRenderers();
		Ogre::RenderSystemList::iterator i = list.begin();

		while (i != list.end())
		{
			if ((*i)->getName() == "Direct3D9 Rendering Subsystem")
			{
				Direct3D9RenderSystem = *i;
			}
			i++;
		}

		if (!Direct3D9RenderSystem)
		{
			qCritical("No rendering subsystems found");
			exit(0);
		}

		if (Direct3D9RenderSystem != 0)
		{
			mActiveRenderSystem = Direct3D9RenderSystem;
		}

		Ogre::Root::getSingletonPtr()->setRenderSystem(mActiveRenderSystem);
		Ogre::Root::getSingletonPtr()->initialise(false);

		mRoot->addFrameListener(this);
	}

	void OgreWidget::chooseSceneManager(void)
	{
		mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
	}

	void OgreWidget::createCamera(void)
	{
		mCamera = mSceneMgr->createCamera("PlayerCam");
		mCamera->setPosition(Ogre::Vector3(0, 0.5, -3));
		mCamera->lookAt(Ogre::Vector3(0, 0.5, 0));
		mCamera->setFOVy(Ogre::Degree(45));
		mCamera->setNearClipDistance(0.01f);
	}

	void OgreWidget::destroyScene(void)
	{
	}

	void OgreWidget::createViewports(void)
	{
		Ogre::Viewport* viewport = mWindow->addViewport(mCamera);
		viewport->setBackgroundColour(Ogre::ColourValue(0.5, 0.5, 0.5, 0.0));
		mCamera->setAspectRatio(Ogre::Real(viewport->getActualWidth()) / Ogre::Real(viewport->getActualHeight()));
	}

	void OgreWidget::loadResources(void)
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./resources", "FileSystem");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./resources/SdkTrays.zip", "Zip");

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}

	QPaintEngine *OgreWidget::paintEngine() const
	{
		return 0;
	}
}
