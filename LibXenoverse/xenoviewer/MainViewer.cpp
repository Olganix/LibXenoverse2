#include "MainViewer.h"
#include "OgreWidget.h"
#include "FileTreeItemWidget.h"
#include "EANOgre.h"
#include "EMBOgre.h"
#include "EMMOgre.h"


MainViewer::MainViewer(QWidget *parent, QtOgre::OgreWidget* ogre_widget)
	: QWidget(parent)
{
	mOgre_widget = ogre_widget;
	
	setupUi(this);										//use MainViewer.ui definition

	FileTree->acceptDrops();
	FileTree->setDragEnabled(true);

	connect(FileTree, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenuFileTree(const QPoint&)));							//add (rigth click for) context menu event
	connect(FileTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(fileItemDoubleClicked(QTreeWidgetItem *, int)));				//add double click event

	connect(BoneTree, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenuBoneTree(const QPoint&)));							//add (rigth click for) context menu event																																				//connect(BoneTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(boneItemDoubleClicked(QTreeWidgetItem *, int)));				//add double click event on bone parts
	connect(BoneTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(boneItemDoubleClicked(QTreeWidgetItem *, int)));				//add double click event on bone parts
	//connect(BoneTree, SIGNAL(itemSelectionChanged(QTreeWidgetItem*, int)), this, SLOT(boneItemSelectionChanged(QTreeWidgetItem*, int)));				//add double click event on bone parts


	AnimationTree->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(AnimationTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(animationItemDoubleClicked(QTreeWidgetItem *, int)));		//add double click event on animations parts.
	connect(AnimationTree, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenuAnimationTree(const QPoint&)));							//add (rigth click for) context menu event


	//button
	connect(ClearFileTreeButton, SIGNAL(released()), this, SLOT(clearFileTree()));
	connect(GenerateClusterButton, SIGNAL(released()), this, SLOT(paintCluster()));

	//play animations
	connect(PlayButton, SIGNAL(released()), this, SLOT(playAnimation()));
	connect(PauseButton, SIGNAL(released()), this, SLOT(pauseAnimation()));
	connect(StopButton, SIGNAL(released()), this, SLOT(stopAnimation()));
	connect(PreviousFrameButton, SIGNAL(released()), this, SLOT(previousFrameAnimation()));
	connect(NextFrameButton, SIGNAL(released()), this, SLOT(nextFrameAnimation()));
	connect(PlaybackLoopCheck, SIGNAL(released()), this, SLOT(loopAnimation()));

	current_ogre_texture = NULL;
}


void  MainViewer::playAnimation() { mOgre_widget->playAnimation(); }
void  MainViewer::pauseAnimation() { mOgre_widget->pauseAnimation(); }
void  MainViewer::stopAnimation() { mOgre_widget->stopAnimation(); }
void  MainViewer::previousFrameAnimation() { mOgre_widget->previousFrameAnimation(); }
void  MainViewer::nextFrameAnimation()  { mOgre_widget->nextFrameAnimation(); }
void  MainViewer::loopAnimation() 
{ 
	//mOgre_widget->loopAnimation(PlaybackLoopCheck.getChecked()); 
}


/*
void MainViewer::keyPressEvent(QKeyEvent * event)
{
	QWidget::keyPressEvent(event);
	FileTree->keyPressEvent(event);
	//AnimationTree->keyPressEvent(event);
}
*/

void MainViewer::fileItemDoubleClicked(QTreeWidgetItem *item, int column)
{
	FileTreeItemWidget *item_cast = static_cast<FileTreeItemWidget *>(item);


	//Display TextureInformations in textureTabs when doubleClicked on Texture in tree, like dds files. (TODO finish)
	if (item_cast->getType() == FileTreeItemWidget::ItemTexture)
	{
		TextureItemWidget *texture_item = static_cast<TextureItemWidget *>(item);
		TexturePackItemWidget *texture_pack_item = static_cast<TexturePackItemWidget *>(texture_item->parent());
		if (texture_pack_item)
		{
			EMBOgre *emb = texture_pack_item->getData();
			EMBFile *emb_file = texture_item->getData();

			if (emb && emb_file)
			{
				current_ogre_texture = emb->getOgreTexture(emb_file->getIndex());
				enableTab(7);
			}
		}
	}
	else if (item_cast->getType() == FileTreeItemWidget::ItemSkeleton)
	{
		SkeletonItemWidget *skeleton_item = static_cast<SkeletonItemWidget*>(item);
		if (skeleton_item)
		{
			ESKOgre *esk = skeleton_item->getData();

			if (esk)
			{
				//build Bone Tree
				BoneTree->clear();
				mOgre_widget->clear_boneList();

				std::vector<ESKBone*> listbones = esk->getBones();

				std::vector<BoneItemWidget *> listBoneWidget;

				size_t nbBones = listbones.size();
				for (size_t i = 0; i < nbBones; i++)
				{
					BoneItemWidget *item_widget = new BoneItemWidget(listbones.at(i), ((i == 0) ? BoneTree : NULL));
					item_widget->mOgreWidget = mOgre_widget;
					listBoneWidget.push_back(item_widget);
					mOgre_widget->add_boneList(listbones.at(i));
				}

				size_t parent_index;
				for (size_t i = 0; i < nbBones; i++)
				{
					parent_index = listbones.at(i)->parent_index;
					if (parent_index == i)
						continue;

					if (parent_index < nbBones)									//filter for root
					{
						if (listBoneWidget.at(i)->parent())
							listBoneWidget.at(i)->parent()->removeChild(listBoneWidget.at(i));
						listBoneWidget.at(parent_index)->addChild(listBoneWidget.at(i));
						listBoneWidget.at(parent_index)->setExpanded(true);
					}
				}

				enableTab(0);
			}

		}


	}else if (item_cast->getType() == FileTreeItemWidget::ItemMaterial){


		MaterialItemWidget *material_item = static_cast<MaterialItemWidget*>(item);
		MaterialPackItemWidget *material_pack_item = 0;
		ModelPackItemWidget *model_pack_item = 0;

		if (material_item)
			material_pack_item = static_cast<MaterialPackItemWidget *>(material_item->parent());
				
		if (material_pack_item)
			model_pack_item = static_cast<ModelPackItemWidget *>(material_pack_item->parent());

		if (model_pack_item)
		{
			EMMMaterial *emmMat = material_item->getData();
			string materialName = emmMat->getName();

			EMMOgre* emmOgre = material_pack_item->getData();
			EMDOgre* endOgre = model_pack_item->getData();
			

			//clear les layouts proprement.
			recursiveDeleteQwidgetChildren(ParametersTree);
			recursiveDeleteQwidgetChildren(ParametersTree2);


			//reconstruction
			QVBoxLayout* layout = new QVBoxLayout(ParametersTree);
			std::vector<EMMOgre::EmmMaterialParameter>* listParam = emmOgre->getShaderParameters(materialName);
			if (listParam)
			{
				size_t nbParams = listParam->size();
				for (size_t i = 0; i < nbParams; i++)
				{
					EMMOgre::EmmMaterialParameter &param = listParam->at(i);
					if ((param.type != "bool") && (param.type != "float4"))
						continue;

					MaterialParameterItemWidget* item_widget = new MaterialParameterItemWidget(param.name, param, emmMat, emmOgre, mOgre_widget);
					layout->addWidget(item_widget);
				}
			}
			ParametersTree->setLayout(layout);



			//all Material
			QVBoxLayout* layout2 = new QVBoxLayout(ParametersTree2);
			EMMOgre::EmmMaterialParameter param_charac("Character", "float4", (size_t)-1, "vertex");
			MaterialParameterItemWidget *charac_item_widget = new MaterialParameterItemWidget("special to Characters", param_charac, 0, 0, mOgre_widget);
			layout2->addWidget(charac_item_widget);

			std::vector<string> allreadyUsed;
			for (list<EMDOgre *>::iterator it = mOgre_widget->emd_list.begin(); it != mOgre_widget->emd_list.end(); it++)
			{
				EMMOgre* emmOgre = (*it)->getMaterialPack();

				std::vector<EMMOgre::EmmMaterialCreated> &created_materials = emmOgre->getCreatedMaterials();
				size_t nbMat = created_materials.size();
				for (size_t i = 0; i < nbMat; i++)
				{
					std::vector<EMMOgre::EmmMaterialParameter> &parameter = created_materials.at(i).parameter;

					size_t nbParam = parameter.size();
					for (size_t j = 0; j < nbParam; j++)
					{
						EMMOgre::EmmMaterialParameter &param = parameter.at(j);
						string paramName = param.name;

						bool isfound = false;
						size_t nbParamAllReady = allreadyUsed.size();
						for (size_t k = 0; k < nbParamAllReady; k++)
						{
							if (paramName  == allreadyUsed.at(k))
							{
								isfound = true;
								break;
							}
						}

						if (!isfound)
						{
							MaterialParameterItemWidget* item_widget = new MaterialParameterItemWidget(param.name, param, 0, 0, mOgre_widget);
							layout2->addWidget(item_widget);
							allreadyUsed.push_back(paramName);
						}
					}
				}
			}

			ParametersTree2->setLayout(layout2);


			enableTab(6);
		}
	}
}

void MainViewer::animationItemDoubleClicked(QTreeWidgetItem *item, int column)
{
	FileTreeItemWidget *item_cast = static_cast<FileTreeItemWidget *>(item);

	//use double clicked animation (TODO solve bug on more than one animation on mesh with the same name)
	if (item_cast->getType() == FileTreeItemWidget::ItemAnimation)
	{
		AnimationItemWidget *animation_item_cast = static_cast<AnimationItemWidget *>(item_cast);
		EANOgreAnimation *anim = animation_item_cast->getData();
		EANOgre *anim_pack = static_cast<EANOgre *>(anim->getParent());
		anim_pack->tagForceAnimation(anim);
	}
}

void MainViewer::boneItemDoubleClicked(QTreeWidgetItem *item, int column)
{
	FileTreeItemWidget *item_cast = static_cast<FileTreeItemWidget *>(item);

	//use double clicked
	if (item_cast->getType() == FileTreeItemWidget::ItemBone)
	{
		BoneItemWidget* bone_item_cast = static_cast<BoneItemWidget*>(item_cast);
		
		

		QTreeWidget* boneTree = findChild<QTreeWidget*>("BoneTree");

		if (!boneTree)
			return;
		
		QTreeWidgetItemIterator it(boneTree);
		while (*it)
		{
			BoneItemWidget* bone_item_cast = static_cast<BoneItemWidget*>(*it);

			bool visible = true;
			QTreeWidgetItem* bone_item_cast_tmp = bone_item_cast->parent();
			while (bone_item_cast_tmp)
			{
				if (!bone_item_cast_tmp->isExpanded())
				{
					visible = false;
					break;
				}
				bone_item_cast_tmp = bone_item_cast_tmp->parent();
			}


			ESKBone* bone = bone_item_cast->getData();
			bone->setVisible(visible);

			++it;
		}
	}
}


void MainViewer::boneItemSelectionChanged(QTreeWidgetItem* item, int column)
{
	FileTree->boneItemSelectionChanged(item);
}



void MainViewer::contextMenuFileTree(const QPoint& point)
{
	FileTree->processContextMenu(point);
}

void MainViewer::contextMenuBoneTree(const QPoint& point)
{
	FileTree->processContextMenu(point);
}

void MainViewer::contextMenuAnimationTree(const QPoint& point)
{
	AnimationItemWidget *item = static_cast<AnimationItemWidget *>(AnimationTree->itemAt(point));
	QPoint globalPos = mapToGlobal(point);

	if (item)
	{	
		QMenu myMenu(AnimationTree);
		QAction* visibleAct = myMenu.addAction("Play Second Animation");

		QAction* selectedItem = myMenu.exec(globalPos);
		if (selectedItem == visibleAct)
		{
			EANOgreAnimation *anim = item->getData();
			EANOgre *anim_pack = static_cast<EANOgre *>(anim->getParent());
			anim_pack->tagForceAnimation2(anim);
		}
	}
}



void MainViewer::enableTab(int index)
{
	//disableTabs();
	tabWidget->setTabEnabled(index, true);
	tabWidget->setCurrentIndex(index);
}

void MainViewer::disableTabs()
{
	tabWidget->setTabEnabled(0, false);
	tabWidget->setTabEnabled(1, false);
	tabWidget->setTabEnabled(2, false);
	tabWidget->setTabEnabled(3, false);
	tabWidget->setTabEnabled(4, false);
	tabWidget->setTabEnabled(5, false);
	tabWidget->setTabEnabled(6, false);
}




void MainViewer::createFileTreeItems(list<EMDOgre *> &new_emd_list, list<ESKOgre *> &new_esk_list, list<EANOgre *> &new_ean_list)
{
	for (list<ESKOgre *>::iterator it = new_esk_list.begin(); it != new_esk_list.end(); it++)
		SkeletonItemWidget *item_widget = new SkeletonItemWidget(*it, FileTree);

	for (list<EMDOgre *>::iterator it = new_emd_list.begin(); it != new_emd_list.end(); it++)
		ModelPackItemWidget *item_widget = new ModelPackItemWidget(*it, FileTree);

	for (list<EANOgre *>::iterator it = new_ean_list.begin(); it != new_ean_list.end(); it++)
		AnimationPackItemWidget *item_widget = new AnimationPackItemWidget(*it, AnimationTree);
}

void MainViewer::clearFileTree()
{
	FileTree->clear();
}





void MainViewer::paintCluster()
{
	if (!current_ogre_texture)
		return;

	Ogre::Image image;
	size_t width = 128;
	size_t height = 128;
	size_t image_byte_size = width * height * 3;
	uchar *image_data = new uchar[image_byte_size];
	memset(image_data, 0x00, image_byte_size);

	Cluster1_1->paintOgreTexture(image_data, width, 0);
	Cluster1_2->paintOgreTexture(image_data, width, 4);
	Cluster1_3->paintOgreTexture(image_data, width, 8);
	Cluster1_4->paintOgreTexture(image_data, width, 12);
	Cluster2_1->paintOgreTexture(image_data, width, 16);
	Cluster2_2->paintOgreTexture(image_data, width, 20);
	Cluster2_3->paintOgreTexture(image_data, width, 24);
	Cluster2_4->paintOgreTexture(image_data, width, 28);
	Cluster3_1->paintOgreTexture(image_data, width, 32);
	Cluster3_2->paintOgreTexture(image_data, width, 36);
	Cluster3_3->paintOgreTexture(image_data, width, 40);
	Cluster3_4->paintOgreTexture(image_data, width, 44);
	Cluster4_1->paintOgreTexture(image_data, width, 48);
	Cluster4_2->paintOgreTexture(image_data, width, 52);
	Cluster4_3->paintOgreTexture(image_data, width, 56);
	Cluster4_4->paintOgreTexture(image_data, width, 60);
	Cluster5_1->paintOgreTexture(image_data, width, 64);
	Cluster5_2->paintOgreTexture(image_data, width, 68);
	Cluster5_3->paintOgreTexture(image_data, width, 72);
	Cluster5_4->paintOgreTexture(image_data, width, 76);
	Cluster6_1->paintOgreTexture(image_data, width, 80);
	Cluster6_2->paintOgreTexture(image_data, width, 84);
	Cluster6_3->paintOgreTexture(image_data, width, 88);
	Cluster6_4->paintOgreTexture(image_data, width, 92);
	Cluster7_1->paintOgreTexture(image_data, width, 96);
	Cluster7_2->paintOgreTexture(image_data, width, 100);
	Cluster7_3->paintOgreTexture(image_data, width, 104);
	Cluster7_4->paintOgreTexture(image_data, width, 108);
	Cluster8_1->paintOgreTexture(image_data, width, 112);
	Cluster8_2->paintOgreTexture(image_data, width, 116);
	Cluster8_3->paintOgreTexture(image_data, width, 120);
	Cluster8_4->paintOgreTexture(image_data, width, 124);

	image.loadDynamicImage(image_data, width, height, 1, Ogre::PF_BYTE_BGR);
	current_ogre_texture->unload();
	current_ogre_texture->loadImage(image);

	delete [] image_data;
}


void MainViewer::recursiveDeleteQwidgetChildren(QWidget* widget)
{
	QWidget* child = ((widget->children().size()!=0) ? ((QWidget*)widget->children().at(0)) : 0);
	
	while (child)
	{
		recursiveDeleteQwidgetChildren(child);					//recursive.
		delete child;

		child = ((widget->children().size() != 0) ? ((QWidget*)widget->children().at(0)) : 0);
	}
}