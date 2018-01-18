#include "ui_MainViewer.h"
#include "OgreWidget.h"

class EMDOgre;
class EMOOgre;
class ESKOgre;
class EANOgre;

class MainViewer : public QWidget, private Ui::MainViewer
{
	Q_OBJECT

public:
	Ogre::Texture *current_ogre_texture;
	QtOgre::OgreWidget* mOgre_widget;

	MainViewer(QWidget *parent = 0, QtOgre::OgreWidget* ogre_widget = 0);

	//events
	//void	keyPressEvent(QKeyEvent * event);

	private slots:

	void	fileItemDoubleClicked(QTreeWidgetItem * item, int column);
	void	animationItemDoubleClicked(QTreeWidgetItem * item, int column);
	void	boneItemDoubleClicked(QTreeWidgetItem * item, int column);
	void	boneItemSelectionChanged(QTreeWidgetItem* item, int column);
	
	void	contextMenuFileTree(const QPoint& point);
	void	contextMenuBoneTree(const QPoint& point);
	void	contextMenuAnimationTree(const QPoint& point);

	//actions
	void	enableTab(int index);
	void	disableTabs();

	public slots:
	void	createFileTreeItems(list<EMDOgre *> &new_emd_list, list<ESKOgre *> &new_esk_list, list<EANOgre *> &new_ean_list);
	private slots:
	void	clearFileTree();

	void	playAnimation();
	void	pauseAnimation();
	void	stopAnimation();
	void	previousFrameAnimation();
	void	nextFrameAnimation();
	void	loopAnimation();

	void	paintCluster();

	void	recursiveDeleteQwidgetChildren(QWidget* widget);
};
