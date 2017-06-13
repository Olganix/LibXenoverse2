#ifndef QTOGRE_OGREWIDGET_H_
#define QTOGRE_OGREWIDGET_H_

class EMDOgre;
class ESKOgre;
class EANOgre;
class FBXOgre;
class SkeletonDebug;
class SkeletonDebug2;
class EMDRenderObjectListener;
class OgreWidget;

namespace QtOgre
{
	class InputEventHandler;

	class OgreWidget : public QWidget, public Ogre::FrameListener
	{
		Q_OBJECT

	public:
		OgreWidget(QWidget* pParentWidget=0, Qt::WindowFlags f=0);
		~OgreWidget();

		void	createDebug(QWidget* pParentWidget);
		Ogre::RenderWindow*	getOgreRenderWindow() const;
	
		Ogre::RenderWindow*	renderWindow()
		{
			return mWindow;
		}
	
		void	addFile(string filename, list<EMDOgre *> &target_emd_list, list<ESKOgre *> &target_esk_list, list<EANOgre *> &target_ean_list);
		void	addFileEAN(string filename, list<EANOgre *> &target_ean_list);
		void	addFileESK(string filename, list<ESKOgre *> &target_esk_list);
		void	addFileEMD(string filename, list<EMDOgre *> &target_emd_list);
		void	addFiles(const QStringList& pathList, list<EMDOgre *> &target_emd_list, list<ESKOgre *> &target_esk_list, list<EANOgre *> &target_ean_list);

		void	clear_boneList(void) { bone_list.clear(); }
		void	add_boneList(ESKBone *bone) { bone_list.push_back(bone); }

		void	getItemLists(list<EMDOgre *> &target_emd_list, list<ESKOgre *> &target_esk_list, list<EANOgre *> &target_ean_list);

		void	checkRebuild();
		bool	installShaders();
		void	resetCamera_Charac();
		void	resetCamera_Stage();

	protected:
		void	paintEvent(QPaintEvent* evt);
		void	resizeEvent(QResizeEvent* evt);

		void	keyPressEvent(QKeyEvent* event);
		void	doScreenShot();
		void	mousePressEvent(QMouseEvent * event);
		void	mouseMoveEvent(QMouseEvent * event);
		void	mouseReleaseEvent(QMouseEvent * event);
		void	wheelEvent(QWheelEvent * event);

	private:
		void	initialiseOgre();
		void	chooseSceneManager();
		void	moveAndResize();
		void	createCamera();
		void	createViewports();
		void	loadResources();


		virtual void	createScene();
		void	loadDebugModels();
		virtual void	destroyScene();

		virtual bool	frameRenderingQueued(const Ogre::FrameEvent& evt);
		
		void	repositionCamera();
		QPaintEngine*	paintEngine() const;
		void	spinCamera(float delta_x, float delta_y);
		void	panCamera(float delta_x, float delta_y);
		void	zoomCamera(float delta);

	private slots:
		void	swapGeometryVisible(void);
		void	swapSkeletonVisible(void);

	public slots:
	void	playAnimation() { mAnimationStop = false;	mAnimationPlaying = true;  mCurrentTime = 0.0;}
	void	stopAnimation() { mAnimationStop = true;	mAnimationPlaying = false; }
	void	pauseAnimation() { mAnimationPlaying = !mAnimationPlaying; }
	void	previousFrameAnimation()	{ mCurrentTime -= 1.0 / 60.0;	mAnimationStop = false; mAnimationForceOneUpdate = true; }
	void	nextFrameAnimation()		{ mCurrentTime += 1.0 / 60.0;	mAnimationStop = false; mAnimationForceOneUpdate = true; }
	void	loopAnimation(bool checked) { mAnimationLoopEnable = checked; }

	public:
		Ogre::RenderWindow* mWindow;
		Ogre::Viewport* mViewport;

	private:
		QWidget* mpParentWidget;
		Ogre::RenderSystem* mActiveRenderSystem;
		Ogre::Root* mRoot;
		Ogre::Camera* mCamera;
		Ogre::SceneManager* mSceneMgr;
		QTimer* mUpdateTimer;
		uint mNumScreenShots;

		bool enable_spinning;
		bool enable_panning;
		float spin_x;
		float spin_y;
		int last_mouse_x;
		int last_mouse_y;
		bool haveMoved;
		float zoom;
		bool mGeometryVisible;
		bool mSkeletonVisible;
		bool mGridVisible;
		bool mRepereVisible;

		Ogre::Vector3 viewer_center;
		float viewer_angle_x;
		float viewer_angle_y;

		Ogre::Real mCurrentTime;
		bool mAnimationPlaying;
		bool mAnimationStop;
		bool mAnimationLoopEnable;
		bool mAnimationForceOneUpdate;
		string mAnimationStatus;
		QLabel* mAnimationStatusWidget;

		int current_animation_index;
		SkeletonDebug *skeleton_debug;
		SkeletonDebug2 *skeleton_debug2;
		Ogre::AnimationState *current_animation_state;
		EANOgre *animation;
		Ogre::Entity *entity;
		EMDRenderObjectListener *emd_render_object_listener;
		Ogre::SceneNode* mRepereNode;
		Ogre::SceneNode* mGridNode;

		list<EMDOgre *> emd_list;
		list<ESKOgre *> esk_list;
		list<EANOgre *> ean_list;
		list<ESKBone *> bone_list;
		std::vector<SDS *> sds_list;

public:
		std::vector<std::string> listSelectedBoneNames;
	};
}

#endif