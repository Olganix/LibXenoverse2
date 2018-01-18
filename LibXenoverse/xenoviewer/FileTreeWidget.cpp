#include "FileTreeWidget.h"
#include "FileTreeItemWidget.h"

#include "EMDOgre.h"
#include "EMMOgre.h"
#include "ESKOgre.h"
#include "EANOgre.h"


FileTreeWidget::FileTreeWidget(QWidget *parent) : QTreeWidget(parent) 
{
	setFocusPolicy(Qt::StrongFocus);
	setContextMenuPolicy(Qt::CustomContextMenu);
}

void FileTreeWidget::dropEvent(QDropEvent *event) 
{
	//MimeData: application/x-qabstractitemmodeldatalist"
	const QMimeData* qMimeData = event->mimeData();

	QByteArray encoded = qMimeData->data("application/x-qabstractitemmodeldatalist");
	QDataStream stream(&encoded, QIODevice::ReadOnly);

	while (!stream.atEnd())
	{
		int row, col;
		QMap<int, QVariant> roleDataMap;
		stream >> row >> col >> roleDataMap;
		QString dropped = roleDataMap[0].toString();

		// FIXME: There has to be a better way to get the dropped widget than just a name match
		QList<QTreeWidgetItem *> match_items = findItems(dropped, Qt::MatchExactly | Qt::MatchRecursive);
		if (match_items.count()) 
		{
			QTreeWidgetItem *first_match = match_items[0];
			FileTreeItemWidget *source_item = static_cast<FileTreeItemWidget *>(first_match);
			FileTreeItemWidget *dest_item = static_cast<FileTreeItemWidget *>(this->itemAt(event->pos()));
			dropItemOnItem(source_item, dest_item);
		}
	}

	QTreeWidget::dropEvent(event);
}


void FileTreeWidget::dropItemOnItem(FileTreeItemWidget *source_item, FileTreeItemWidget *dest_item)
{
	ModelPackItemWidget *src_model_pack_item = NULL;
	SkeletonItemWidget *src_skeleton_item = NULL;
	ModelPackItemWidget *dest_model_pack_item = NULL;
	SkeletonItemWidget *dest_skeleton_item = NULL;

	if (source_item) 
	{
		src_model_pack_item = static_cast<ModelPackItemWidget *>(source_item->getMatchType(FileTreeItemWidget::ItemModelPack));
		src_skeleton_item = static_cast<SkeletonItemWidget *>(source_item->getMatchType(FileTreeItemWidget::ItemSkeleton));
	}

	if (dest_item) 
	{
		dest_model_pack_item = static_cast<ModelPackItemWidget *>(dest_item->getMatchType(FileTreeItemWidget::ItemModelPack));
		dest_skeleton_item = static_cast<SkeletonItemWidget *>(dest_item->getMatchType(FileTreeItemWidget::ItemSkeleton));
	}

	// Dragging a model pack into a skeleton
	if (src_model_pack_item && dest_skeleton_item) 
	{
		EMDOgre *model_pack = src_model_pack_item->getData();
		ESKOgre *skeleton = dest_skeleton_item->getData();

		if (model_pack->getTag() == "EMD")
		{
			model_pack->setSkeleton(skeleton);
			model_pack->tagForRebuild();
		}

		src_model_pack_item->updateText();
		dest_skeleton_item->setExpanded(true);
	}

	// Dragging a model pack into something that isn't a skeleton
	if (src_model_pack_item && !dest_skeleton_item) 
	{
		// Dragging into nothing
		if (!dest_item) 
		{
			EMDOgre *model_pack = src_model_pack_item->getData();

			if (model_pack->getTag() == "EMD")
			{
				if (model_pack->getSkeleton())
				{
					model_pack->setSkeleton(NULL);
					model_pack->tagForRebuild();
					src_model_pack_item->updateText();
				}
			}
		}
		// Dragging into a model, verify that its parent is not the same skeleton
		else if (dest_model_pack_item) 
		{
			EMDOgre *model_pack = src_model_pack_item->getData();
			EMDOgre *dest_model_pack = dest_model_pack_item->getData();

			if (model_pack->getTag() == "EMD")
			{
				if (model_pack->getSkeleton() != dest_model_pack->getSkeleton())
				{
					model_pack->setSkeleton(dest_model_pack->getSkeleton());
					model_pack->tagForRebuild();
					src_model_pack_item->updateText();
				}
			}
		}
		else {
			SHOW_ERROR("FIXME: Unhandled Drag and Drop for Model Pack");
		}
	}





	//Solution for scd.esk : link a scd on classic esk.
	
	// Dragging a skelton (scd) into a skeleton
	if (src_skeleton_item && dest_skeleton_item)
	{
		ESKOgre *skeleton_scd = src_skeleton_item->getData();
		ESKOgre *skeleton = dest_skeleton_item->getData();

		
		//merge of skeleton in the src, matching scd's bones with body esk's bones.
		skeleton_scd->merge_scd(skeleton);


		//if there is allready many Emd on the scd.esk, we have to update them, and rebuild them.
		ModelPackItemWidget* model_pack_item = 0;
		EMDOgre *model_pack = 0;
		size_t nbbChildEMD = src_skeleton_item->childCount();
		for (size_t i = 0; i < nbbChildEMD; i++)
		{
			model_pack_item = static_cast<ModelPackItemWidget *>(src_skeleton_item->child(i));
			model_pack = model_pack_item->getData();

			if (model_pack->getTag() == "EMD")
			{
				model_pack->setSkeleton(skeleton_scd);
				model_pack->tagForRebuild();
			}
		}
		
		dest_skeleton_item->setExpanded(true);
	}

	// Dragging a model pack into something that isn't a skeleton
	if (src_skeleton_item && !dest_skeleton_item)
	{
		//the question is to know how get back the original skeleton (before merging).
		
		
		// Dragging into nothing
		if (!dest_item)
		{
			ESKOgre* skeleton_scd = src_skeleton_item->getData();
			
			//getback in the previous state, before the merge.
			skeleton_scd->reverte_merge();

			//if there is allready many emd on the scd.esk, we have to update and rebuild them.
			ModelPackItemWidget* model_pack_item = 0;
			EMDOgre *model_pack = 0;
			size_t nbbChildEMD = src_skeleton_item->childCount();
			for (size_t i = 0; i < nbbChildEMD; i++)
			{
				model_pack_item = static_cast<ModelPackItemWidget *>(src_skeleton_item->child(i));
				model_pack = model_pack_item->getData();

				if (model_pack->getTag() == "EMD")
				{
					model_pack->setSkeleton(NULL);
					model_pack->tagForRebuild();
					model_pack_item->updateText();
				}
			}
			
		}else if (dest_model_pack_item){							// Dragging into a model, verify that its parent is not the same skeleton
			
			ESKOgre* skeleton_scd = src_skeleton_item->getData();
			SkeletonItemWidget* src_parent_skeleton = static_cast<SkeletonItemWidget *>(src_skeleton_item->parent());
			ESKOgre* skeleton = (src_parent_skeleton) ? src_parent_skeleton->getData() : 0;
			

			EMDOgre* dest_model_pack = dest_model_pack_item->getData();
			SkeletonItemWidget* dest_parent_skeleton = static_cast<SkeletonItemWidget *>(dest_model_pack_item->parent());
			ESKOgre* dest_skeleton = (dest_parent_skeleton) ? dest_parent_skeleton->getData() : 0;

			
			if ((skeleton_scd != dest_model_pack->getSkeleton())&& (skeleton != dest_model_pack->getSkeleton()))
			{
				skeleton = dest_skeleton;
				
				//get back in the state before the merge, and after, merge skeletons in scr, matching scd's bones with body esk's bones.
				skeleton_scd->merge_scd(skeleton);

				//if there is allready many emd on the scd.esk, we hae to update and rebuild them.
				ModelPackItemWidget* model_pack_item = 0;
				EMDOgre *model_pack = 0;
				size_t nbbChildEMD = src_skeleton_item->childCount();
				for (size_t i = 0; i < nbbChildEMD; i++)
				{
					model_pack_item = static_cast<ModelPackItemWidget *>(src_skeleton_item->child(i));
					model_pack = model_pack_item->getData();

					if (model_pack->getTag() == "EMD")
					{
						model_pack->setSkeleton(skeleton_scd);
						model_pack->tagForRebuild();
					}
				}
			}
		}
		else {
			SHOW_ERROR("FIXME: Unhandled Drag and Drop for Model Pack");
		}
	}
}
/*
void FileTreeWidget::keyPressEvent(QKeyEvent * event) 
{
	QTreeWidget::keyPressEvent(event);

	if (event->key() == Qt::Key_Delete)
		SHOW_ERROR("Key Detected!");
}
*/

void FileTreeWidget::processContextMenuModelPack(ModelPackItemWidget *item, const QPoint& point) 
{
	QMenu myMenu;
	QAction* deleteAct = myMenu.addAction("Delete");

	EMDOgre* endOgre = item->getData();

	if (endOgre->getTag() == "EMD")
	{
		QAction* visibleAct = myMenu.addAction(((endOgre->getVisible()) ? "Hide" : "Show"));

		QAction* selectedItem = myMenu.exec(point);
		if (selectedItem == deleteAct)
		{
			delete item;
		}else if (selectedItem == visibleAct){
			endOgre->setVisible(!endOgre->getVisible());
		}
	}
}

void FileTreeWidget::processContextMenuMaterial(MaterialItemWidget *item, const QPoint& point)
{
	QMenu myMenu;
	
	MaterialPackItemWidget *material_pack_item = static_cast<MaterialPackItemWidget *>(item->parent());
	if (!material_pack_item)
		return;

	ModelPackItemWidget *model_pack_item = static_cast<ModelPackItemWidget *>(material_pack_item->parent());
	if (!model_pack_item)
		return;

	EMMMaterial *emmMat = item->getData();
	string materialName = emmMat->getName();

	EMMOgre* emmOgre = material_pack_item->getData();
	EMDOgre* endOgre = model_pack_item->getData();	
	if (endOgre->getTag() != "EMD")
		return;

	
	QAction* visibleAct = myMenu.addAction(((item->getVisible()) ? "Hide" : "Show"));

	QAction* selectedItem = myMenu.exec(point);
	if (selectedItem == visibleAct)
	{
		item->setVisible(!item->getVisible());

		endOgre->setVisible(materialName, item->getVisible());
	}
}



void FileTreeWidget::processContextMenuSkeleton(SkeletonItemWidget *item, const QPoint& point) 
{
	QMenu myMenu;
	myMenu.addAction("Delete");

	QAction* selectedItem = myMenu.exec(point);
	if (selectedItem) 
	{
		delete item;
	}
	else {
	}
}

void FileTreeWidget::processContextMenuBone(BoneItemWidget *item, const QPoint& point)
{
	QMenu myMenu;
	QAction* visibleAct = myMenu.addAction("Show/Hide");

	QAction* selectedItem = myMenu.exec(point);
	if(selectedItem == visibleAct)
		((ESKBone*)item->getData())->setVisible(!((ESKBone*)item->getData())->getVisible());
}




void FileTreeWidget::processContextMenuEmpty(const QPoint& point)
{
	
}

void FileTreeWidget::processContextMenu(const QPoint& point)
{
	FileTreeItemWidget *item = static_cast<FileTreeItemWidget *>(itemAt(point));
	QPoint globalPos = mapToGlobal(point);

	if (item)
	{
		ModelPackItemWidget *model_pack_item = static_cast<ModelPackItemWidget *>(item->getMatchType(FileTreeItemWidget::ItemModelPack));
		MaterialItemWidget  *material_item = static_cast<MaterialItemWidget *>(item->getMatchType(FileTreeItemWidget::ItemMaterial));
		SkeletonItemWidget *skeleton_item = static_cast<SkeletonItemWidget *>(item->getMatchType(FileTreeItemWidget::ItemSkeleton));
		BoneItemWidget *bone_item = static_cast<BoneItemWidget *>(item->getMatchType(FileTreeItemWidget::ItemBone));
		
		if (model_pack_item)
			processContextMenuModelPack(model_pack_item, globalPos);
		else if(material_item)
			processContextMenuMaterial(material_item, globalPos);
		else if (skeleton_item)
			processContextMenuSkeleton(skeleton_item, globalPos);
		else if (bone_item)
			processContextMenuBone(bone_item, globalPos);
		
	}else{
		processContextMenuEmpty(globalPos);
	}
}


void FileTreeWidget::boneItemSelectionChanged(QTreeWidgetItem* item)
{
	BoneItemWidget *bone_item = static_cast<BoneItemWidget *>(item);	
	QtOgre::OgreWidget* ogreWidget = bone_item->mOgreWidget;


	QList<QTreeWidgetItem*> listSelected = item->treeWidget()->selectedItems();
	ogreWidget->listSelectedBoneNames.clear();

	BoneItemWidget* bone_item_tmp;
	size_t nbSelected = listSelected.count();
	for (size_t i = 0; i < nbSelected; i++)
	{
		bone_item_tmp = static_cast<BoneItemWidget *>(listSelected.takeAt(i));
		ogreWidget->listSelectedBoneNames.push_back(bone_item_tmp->getData()->getName());
	}
}

