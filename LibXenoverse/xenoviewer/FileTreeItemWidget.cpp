#include "EMDOgre.h"
#include "ESKOgre.h"
#include "EANOgre.h"
#include "EMMogre.h"
#include "EMBOgre.h"
#include "FileTreeItemWidget.h"



FileTreeItemWidget::FileTreeItemWidget(QTreeWidget *parent) : QTreeWidgetItem(parent) 
{
	setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
}

SkeletonItemWidget::SkeletonItemWidget(ESKOgre *data, QTreeWidget *parent) : FileTreeItemWidget(parent) 
{
	data_ptr = data;
	setText(0, (data->getName() + " (Skeleton)").c_str());
	setIcon(0, QIcon(":/resources/icons/esk.png"));
	setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDropEnabled | (data->getFlag()!=1 ? Qt::ItemIsDragEnabled : Qt::NoItemFlags));

	type = ItemSkeleton;
}

SkeletonItemWidget::~SkeletonItemWidget() 
{
	data_ptr->tagForDeletion();
}

ModelPackItemWidget::ModelPackItemWidget(EMDOgre *data, QTreeWidget *parent) : FileTreeItemWidget(parent)
{
	if (data->getTag() == "EMD")
	{
		data_ptr = data;
		updateText();
		setIcon(0, QIcon(":/resources/icons/emd.png"));
		setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled);

		type = ItemModelPack;

		EMMOgre *material_pack = data->getMaterialPack();
		MaterialPackItemWidget *material_pack_item = new MaterialPackItemWidget(material_pack, NULL);
		addChild(material_pack_item);

		EMBOgre *texture_pack = material_pack->getTexturePack();
		TexturePackItemWidget *texture_pack_item = new TexturePackItemWidget(texture_pack, NULL);
		addChild(texture_pack_item);

		EMBOgre *texture_dyt_pack = material_pack->getDYTTexturePack();
		TexturePackItemWidget *texture_dyt_pack_item = new TexturePackItemWidget(texture_dyt_pack, NULL);
		addChild(texture_dyt_pack_item);
	}
}

ModelPackItemWidget::~ModelPackItemWidget() 
{
	if (data_ptr->getTag() == "EMD")
		data_ptr->tagForDeletion();
}

void ModelPackItemWidget::updateText()
{
	
	if (data_ptr)
	{
		if (data_ptr->getTag() == "EMD")
		{
			if (data_ptr->getSkeleton())
				setText(0, (data_ptr->getName() + " (Model Pack / Skinned)").c_str());
			else
				setText(0, (data_ptr->getName() + " (Model Pack / No Skin)").c_str());
		}
	}
	
}

AnimationPackItemWidget::AnimationPackItemWidget(EANOgre *data, QTreeWidget *parent) : FileTreeItemWidget(parent)
{
	data_ptr = data;
	setText(0, (data->getName() + " (Animation Pack)").c_str());
	setIcon(0, QIcon(":/resources/icons/animation_pack.png"));
	setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

	type = ItemAnimationPack;

	vector<EANAnimation> &animations = data->getAnimations();
	for (size_t i = 0; i < animations.size(); i++) {
		AnimationItemWidget *animation_item = new AnimationItemWidget(&animations[i], NULL);
		addChild(animation_item);
	}

	setExpanded(true);
}

AnimationItemWidget::AnimationItemWidget(EANAnimation *data, QTreeWidget *parent) : FileTreeItemWidget(parent)
{
	data_ptr = data;
	setText(0, (data->getName()).c_str());
	setIcon(0, QIcon(":/resources/icons/animation.png"));
	setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

	type = ItemAnimation;
}

BoneItemWidget::BoneItemWidget(ESKBone *data, QTreeWidget *parent) : FileTreeItemWidget(parent)
{
	data_ptr = data;
	setText(0, (data->getName()).c_str());
	setIcon(0, QIcon(":/resources/icons/esk.png"));
	setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDropEnabled);

	type = ItemBone;
}



MaterialParameterItemWidget::MaterialParameterItemWidget(string name, EMMOgre::EmmMaterialParameter& param, EMMMaterial* emmMaterial, EMMOgre* emmOgre, QtOgre::OgreWidget* ogreWidget, QWidget* parent) : QWidget(parent),
mParam(param)
{
	mName = name;
	mEmmMaterial = emmMaterial;
	mEmmOgre = emmOgre;
	mOgreWidget = ogreWidget;
	

	//visual
	QHBoxLayout *layout = new QHBoxLayout(this);
	
	QLabel* qlName = new QLabel((mName).c_str());
	qlName->setAlignment(Qt::AlignTop);
	layout->addWidget(qlName);

	if (mParam.type == "bool")
	{
		checkbox = new QCheckBox(QString("enable"));
		checkbox->setChecked( mParam.currentValue.x != 0.0f );
		layout->addWidget(checkbox);

		QObject::connect(checkbox, SIGNAL(stateChanged(int)), (QWidget*)this, SLOT(checkBoxStateChanged(int)));

	}else if (mParam.type == "float4"){

		QVBoxLayout *layout2 = new QVBoxLayout;
		layout2->setSpacing(0);

		MaterialParameterComponentItemWidget* comp;
		if (mParam.name == "Character")
		{
			comp = new MaterialParameterComponentItemWidget("damage", mParam.currentValue.x);
			layout2->addWidget(comp);
			QObject::connect(comp, SIGNAL(valueChanged(string, float)), (QWidget*)this, SLOT(componentValueChanged(string, float)));

			comp = new MaterialParameterComponentItemWidget("blood", mParam.currentValue.y);
			layout2->addWidget(comp);
			QObject::connect(comp, SIGNAL(valueChanged(string, float)), (QWidget*)this, SLOT(componentValueChanged(string, float)));
		}else {

			comp = new MaterialParameterComponentItemWidget("x", mParam.currentValue.x);
			layout2->addWidget(comp);
			QObject::connect(comp, SIGNAL(valueChanged(string, float)), (QWidget*)this, SLOT(componentValueChanged(string, float)));

			comp = new MaterialParameterComponentItemWidget("y", mParam.currentValue.y);
			layout2->addWidget(comp);
			QObject::connect(comp, SIGNAL(valueChanged(string, float)), (QWidget*)this, SLOT(componentValueChanged(string, float)));

			comp = new MaterialParameterComponentItemWidget("z", mParam.currentValue.z);
			layout2->addWidget(comp);
			QObject::connect(comp, SIGNAL(valueChanged(string, float)), (QWidget*)this, SLOT(componentValueChanged(string, float)));

			comp = new MaterialParameterComponentItemWidget("w", mParam.currentValue.w);
			layout2->addWidget(comp);
			QObject::connect(comp, SIGNAL(valueChanged(string, float)), (QWidget*)this, SLOT(componentValueChanged(string, float)));
		}

		layout->addLayout(layout2);
	}

	this->setLayout(layout);
}


MaterialParameterComponentItemWidget::MaterialParameterComponentItemWidget(string name, float value, QWidget* parent) : QWidget(parent)
{
	mName = name;
	mValue = value;
	mDontNotify = false;
	
	QHBoxLayout *layout = new QHBoxLayout(this);

	QLabel* qlName = new QLabel((mName).c_str());
	layout->addWidget(qlName);


	label = new QLineEdit();
	label->setMaximumWidth(50);
	layout->addWidget(label);
	
	slider = new DoubleSlider();
	slider->setOrientation(Qt::Horizontal);
	slider->setRange(-200, 200);
	layout->addWidget(slider);

	QObject::connect(label, SIGNAL(editingFinished()), (QWidget*)this, SLOT(updateSliderValue()));
	QObject::connect(slider, SIGNAL(doubleValueChanged(double)), (QWidget*)this, SLOT(updateLabelValue(double)));


	updateLabelValue(slider->value());
	updateSliderValue();

	this->setLayout(layout);
}


MaterialPackItemWidget::MaterialPackItemWidget(EMMOgre *data, QTreeWidget *parent) : FileTreeItemWidget(parent) {
	data_ptr = data;
	updateText();
	setIcon(0, QIcon(":/resources/icons/emd.png"));
	setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);

	type = ItemMaterialPack;

	vector<EMMMaterial *> &materials = data->getMaterials();
	for (size_t i = 0; i < materials.size(); i++) {
		MaterialItemWidget *material_item = new MaterialItemWidget(materials[i], NULL);
		addChild(material_item);
	}
}

MaterialPackItemWidget::~MaterialPackItemWidget() 
{

}

void MaterialPackItemWidget::updateText()
{
	if (data_ptr)
		setText(0, (data_ptr->getName() + " (Material Pack)").c_str());
}


MaterialItemWidget::MaterialItemWidget(EMMMaterial *data, QTreeWidget *parent) : FileTreeItemWidget(parent) 
{
	data_ptr = data;
	visible = true;
	updateText();
	setIcon(0, QIcon(":/resources/icons/emd.png"));
	setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);

	type = ItemMaterial;
}

MaterialItemWidget::~MaterialItemWidget() 
{

}

void MaterialItemWidget::updateText() 
{
	if (data_ptr)
		setText(0, (data_ptr->getName() + " (Material)").c_str());
}



TexturePackItemWidget::TexturePackItemWidget(EMBOgre *data, QTreeWidget *parent) : FileTreeItemWidget(parent) 
{
	data_ptr = data;
	updateText();
	setIcon(0, QIcon(":/resources/icons/emd.png"));
	setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);

	type = ItemTexturePack;

	vector<EMBFile *> &files = data->getFiles();
	for (size_t i = 0; i < files.size(); i++) 
	{
		TextureItemWidget *texture_item = new TextureItemWidget(files[i], NULL);
		addChild(texture_item);
	}
}

TexturePackItemWidget::~TexturePackItemWidget() 
{

}

void TexturePackItemWidget::updateText() 
{
	if (data_ptr)
		setText(0, (data_ptr->getName() + " (Texture Pack)").c_str());
}



TextureItemWidget::TextureItemWidget(EMBFile *data, QTreeWidget *parent) : FileTreeItemWidget(parent) 
{
	data_ptr = data;
	updateText();
	setIcon(0, QIcon(":/resources/icons/emd.png"));
	setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);

	type = ItemTexture;
}

TextureItemWidget::~TextureItemWidget() 
{

}

void TextureItemWidget::updateText() 
{
	if (data_ptr) 
	{
		string filename = data_ptr->getName();
		if (!filename.size())
			filename = "DATA" + ToString(data_ptr->getIndex()) + ".dds";

		filename += " (Texture)";

		setText(0, filename.c_str());
	}
}