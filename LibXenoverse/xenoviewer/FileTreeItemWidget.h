#ifndef FILE_TREE_ITEM_WIDGET_H_INCLUDED
#define FILE_TREE_ITEM_WIDGET_H_INCLUDED

class ESKOgre;
class EMDOgre;
class EMOOgre;
class EANOgre;
class EMMOgre;
class EMBOgre;
class FBXOgre;

#include <qobjectdefs.h>
#include <QApplication>
#include <QSlider>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>

#include "OgreWidget.h"
#include "EMDOgre.h"
#include "EMMOgre.h"
#include "EANOgre.h"


class LibXenoverse::EMMMaterial;
class LibXenoverse::EMBFile;



class DoubleSlider : public QSlider
{
	Q_OBJECT

	bool mDontNotify;

public:
	DoubleSlider(QWidget *parent = 0) : QSlider(parent)
	{
		mDontNotify = false;
		connect(this, SIGNAL(valueChanged(int)), this, SLOT(notifyValueChanged(int)));
	}

	double getDoubleValue(void) { return (this->value() / 100.0); }
	void setDoubleValue(double value) { mDontNotify = true;  setValue(int(value * 100.0)); mDontNotify = false; }

signals:
	void doubleValueChanged(double value);

public slots:
	void notifyValueChanged(int value)
	{
		if (mDontNotify)
			return;

		double doubleValue = value / 100.0;
		emit doubleValueChanged(doubleValue);
	}
};


class FileTreeItemWidget : public QTreeWidgetItem 
{
public:
	FileTreeItemWidget(QTreeWidget *parent);

	enum FileTreeItemType 
	{
		ItemSkeleton,
		ItemAnimationPack,
		ItemAnimation,
		ItemModelPack,
		ItemModel,
		ItemMesh,
		ItemSubmesh,
		ItemMaterialPack,
		ItemMaterial,
		ItemMaterialParameter,
		ItemMaterialComponentParameter,
		ItemTexturePack,
		ItemTexture,
		ItemBone
	};

	FileTreeItemType getType() { return type; }

	FileTreeItemWidget *getMatchType(FileTreeItemType match_type) 
	{
		if (match_type == type) 
			return this;
		else
			return NULL;
	}

protected:
	FileTreeItemType type;
};

class SkeletonItemWidget : public FileTreeItemWidget 
{
protected:
	ESKOgre *data_ptr;
public:
	SkeletonItemWidget(ESKOgre *data, QTreeWidget *parent);
	~SkeletonItemWidget();

	ESKOgre *getData() { return data_ptr; }
};

class ModelPackItemWidget : public FileTreeItemWidget 
{
protected:
	EMDOgre *data_ptr;
public:
	ModelPackItemWidget(EMDOgre *data, QTreeWidget *parent);
	~ModelPackItemWidget();

	EMDOgre *getData() { return data_ptr; }

	void updateText();
};


class MaterialPackItemWidget : public FileTreeItemWidget 
{
protected:
	EMMOgre *data_ptr;
public:
	MaterialPackItemWidget(EMMOgre *data, QTreeWidget *parent);
	~MaterialPackItemWidget();

	EMMOgre *getData() { return data_ptr; }

	void updateText();
};


class MaterialItemWidget : public FileTreeItemWidget 
{
protected:
	EMMMaterial *data_ptr;
	bool visible;

public:
	MaterialItemWidget(EMMMaterial *data, QTreeWidget *parent);
	~MaterialItemWidget();

	EMMMaterial *getData() { return data_ptr; }
	bool	getVisible() { return visible; }
	void	setVisible(bool visible) { this->visible = visible; }

	void updateText();
};

class MaterialParameterItemWidget : public QWidget
{
	Q_OBJECT

public:
	string mName;
	EMMMaterial* mEmmMaterial;
	EMMOgre* mEmmOgre;
	EMMOgre::EmmMaterialParameter mParam;
	QtOgre::OgreWidget* mOgreWidget;
	QCheckBox *checkbox;


public:
	MaterialParameterItemWidget(string name, EMMOgre::EmmMaterialParameter& param, EMMMaterial* emmMaterial, EMMOgre* emmOgre, QtOgre::OgreWidget* ogreWidget, QWidget* parent = 0);

public slots:
	void checkBoxStateChanged(int)
	{
		if(checkbox)
			componentValueChanged("x", checkbox->isChecked() ? 1.0 : 0.0);
	}

	void componentValueChanged(string name, float value)
	{
		if (mParam.name == "Character")
		{
			if (name == "damage")
				mParam.currentValue.x = value;
			else if (name == "blood")
				mParam.currentValue.y = value;

			if (!mEmmOgre)
			{
				for (list<EMDOgre *>::iterator it = mOgreWidget->emd_list.begin(); it != mOgreWidget->emd_list.end(); it++)
				{
					(*it)->setMaterialParameter("g_MaterialCol3_PS", mParam.currentValue);
					(*it)->setMaterialParameter("g_MaterialCol3_VS", mParam.currentValue);
				}					
			}else {
				mEmmOgre->setShaderParameter(mEmmOgre->getCreatedMaterialName(mEmmMaterial->getName()), "g_MaterialCol3_PS", mParam.currentValue);
				mEmmOgre->setShaderParameter(mEmmOgre->getCreatedMaterialName(mEmmMaterial->getName()), "g_MaterialCol3_VS", mParam.currentValue);
			}
			return;
		}
		
		if(name=="x")
			mParam.currentValue.x = value;
		else if (name == "y")
			mParam.currentValue.y = value;
		else if (name == "z")
			mParam.currentValue.z = value;
		else if (name == "w")
			mParam.currentValue.w = value;

		if (!mEmmOgre)
		{
			for (list<EMDOgre *>::iterator it = mOgreWidget->emd_list.begin(); it != mOgreWidget->emd_list.end(); it++)
				(*it)->setMaterialParameter(mName, mParam.currentValue);
		}else{
			mEmmOgre->setShaderParameter(mEmmOgre->getCreatedMaterialName(mEmmMaterial->getName()), mName, mParam.currentValue);
		}
	}
};






class MaterialParameterComponentItemWidget : public QWidget
{
	Q_OBJECT

public:
	string mName;
	float mValue;

	DoubleSlider* slider;
	QLineEdit* label;

	bool mDontNotify;

public:
	MaterialParameterComponentItemWidget(string name, float value, QWidget* parent = 0);

signals:
	void valueChanged(string name, float value);

public slots:
	void updateLabelValue(double value)
	{
		mDontNotify = true;
		
		mValue = value;
		label->setText(QString::number(value, 'f', 4));
		emit valueChanged(mName, mValue);

		mDontNotify = false;
	}

	void updateSliderValue()
	{
		if (mDontNotify)
			return;

		QString text = label->text();
		bool isOk = false;
		float value = text.toFloat(&isOk);
		if (isOk)
			slider->setDoubleValue(value);

		mValue = value;

		emit valueChanged(mName, mValue);
	}
};




class TexturePackItemWidget : public FileTreeItemWidget 
{
protected:
	EMBOgre *data_ptr;
public:
	TexturePackItemWidget(EMBOgre *data, QTreeWidget *parent);
	~TexturePackItemWidget();

	EMBOgre *getData() { return data_ptr; }

	void updateText();
};

class TextureItemWidget : public FileTreeItemWidget
{
protected:
	EMBFile *data_ptr;
public:
	TextureItemWidget(EMBFile *data, QTreeWidget *parent);
	~TextureItemWidget();

	EMBFile *getData() { return data_ptr; }

	void updateText();
};

class AnimationPackItemWidget : public FileTreeItemWidget
{
protected:
	EANOgre *data_ptr;
public:
	AnimationPackItemWidget(EANOgre *data, QTreeWidget *parent);

	EANOgre *getData() { return data_ptr; }
};


class AnimationItemWidget : public FileTreeItemWidget
{
protected:
	EANOgreAnimation *data_ptr;
public:
	AnimationItemWidget(EANOgreAnimation *data, QTreeWidget *parent);

	EANOgreAnimation *getData() { return data_ptr; }
};

class BoneItemWidget : public FileTreeItemWidget
{
protected:
	ESKBone *data_ptr;
public:
	QtOgre::OgreWidget* mOgreWidget;


public:
	BoneItemWidget(ESKBone *data, QTreeWidget *parent);

	ESKBone *getData() { return data_ptr; }
};



#endif