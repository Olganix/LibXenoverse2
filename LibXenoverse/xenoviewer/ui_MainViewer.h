/********************************************************************************
** Form generated from reading UI file 'MainViewer.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINVIEWER_H
#define UI_MAINVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "FileTreeWidget.h"
#include "gradLib/gradView.h"

QT_BEGIN_NAMESPACE

class Ui_MainViewer
{
public:
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_4;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    FileTreeWidget *FileTree;
    QPushButton *ClearFileTreeButton;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_2;
    QTabWidget *tabWidget;
    QWidget *SkeletonTab;
    QVBoxLayout *verticalLayout_3;
    QTreeWidget *BoneTree;
    QWidget *MatrixTab;
    QVBoxLayout *verticalLayout_20;
    QVBoxLayout *verticalLayout_19;
    QGroupBox *groupBox_3;
    QTableWidget *tableW_OriginalFile_initialTransf;
    QTableWidget *tableW_OriginalFile_animation;
    QGroupBox *groupBox_4;
    QTableWidget *tableW_Ogre_initialTransf;
    QTableWidget *tableW_Ogre_animation;
    QWidget *ModelPackTab;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QFrame *frame_7;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_4;
    QLabel *label_5;
    QFrame *frame_3;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QComboBox *ModelSkeletonCombo;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label;
    QComboBox *ModelBoneCombo;
    QWidget *ModelTab;
    QWidget *MeshTab;
    QWidget *SubmeshTab;
    QWidget *MaterialTab;
    QWidget *TextureTab;
    QHBoxLayout *horizontalLayout_9;
    QFrame *frame_9;
    QVBoxLayout *verticalLayout_11;
    QTabWidget *tabWidget_2;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_9;
    CGradientColorsView *Cluster1_1;
    CGradientColorsView *Cluster1_2;
    CGradientColorsView *Cluster1_3;
    CGradientColorsView *Cluster1_4;
    QWidget *tab_10;
    QVBoxLayout *verticalLayout_18;
    CGradientColorsView *Cluster2_1;
    CGradientColorsView *Cluster2_2;
    CGradientColorsView *Cluster2_3;
    CGradientColorsView *Cluster2_4;
    QWidget *tab_11;
    QVBoxLayout *verticalLayout_17;
    CGradientColorsView *Cluster3_1;
    CGradientColorsView *Cluster3_2;
    CGradientColorsView *Cluster3_3;
    CGradientColorsView *Cluster3_4;
    QWidget *tab_12;
    QVBoxLayout *verticalLayout_16;
    CGradientColorsView *Cluster4_1;
    CGradientColorsView *Cluster4_2;
    CGradientColorsView *Cluster4_3;
    CGradientColorsView *Cluster4_4;
    QWidget *tab_13;
    QVBoxLayout *verticalLayout_15;
    CGradientColorsView *Cluster5_1;
    CGradientColorsView *Cluster5_2;
    CGradientColorsView *Cluster5_3;
    CGradientColorsView *Cluster5_4;
    QWidget *tab_14;
    QVBoxLayout *verticalLayout_14;
    CGradientColorsView *Cluster6_1;
    CGradientColorsView *Cluster6_2;
    CGradientColorsView *Cluster6_3;
    CGradientColorsView *Cluster6_4;
    QWidget *tab_15;
    QVBoxLayout *verticalLayout_13;
    CGradientColorsView *Cluster7_1;
    CGradientColorsView *Cluster7_2;
    CGradientColorsView *Cluster7_3;
    CGradientColorsView *Cluster7_4;
    QWidget *tab_9;
    QVBoxLayout *verticalLayout_12;
    CGradientColorsView *Cluster8_1;
    CGradientColorsView *Cluster8_2;
    CGradientColorsView *Cluster8_3;
    CGradientColorsView *Cluster8_4;
    QFrame *frame_4;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *GenerateClusterButton;
    QPushButton *ResetClusterButton;
    QFrame *frame_8;
    QVBoxLayout *verticalLayout_10;
    QGraphicsView *ddsTextureView;
    QFrame *frame_6;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *LoadTextureButton;
    QPushButton *SaveTextureButton;
    QFrame *frame_5;
    QVBoxLayout *verticalLayout_6;
    QTreeWidget *AnimationTree;
    QPushButton *ClearAnimationListButton;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *PlayButton;
    QPushButton *PauseButton;
    QPushButton *StopButton;
    QPushButton *PreviousFrameButton;
    QPushButton *NextFrameButton;
    QHBoxLayout *horizontalLayout_10;
    QCheckBox *PlaybackLoopCheck;
    QLabel *PlayAnimationText;

    void setupUi(QWidget *MainViewer)
    {
        if (MainViewer->objectName().isEmpty())
            MainViewer->setObjectName(QStringLiteral("MainViewer"));
        MainViewer->resize(890, 757);
        MainViewer->setStyleSheet(QStringLiteral(""));
        horizontalLayout_3 = new QHBoxLayout(MainViewer);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        frame = new QFrame(MainViewer);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setMaximumSize(QSize(16777215, 16777215));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        FileTree = new FileTreeWidget(frame);
        FileTree->setObjectName(QStringLiteral("FileTree"));
        FileTree->setEditTriggers(QAbstractItemView::DoubleClicked);
        FileTree->setDragEnabled(true);
        FileTree->setDragDropMode(QAbstractItemView::InternalMove);
        FileTree->setDefaultDropAction(Qt::IgnoreAction);
        FileTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
        FileTree->setIconSize(QSize(24, 24));

        verticalLayout_2->addWidget(FileTree);

        ClearFileTreeButton = new QPushButton(frame);
        ClearFileTreeButton->setObjectName(QStringLiteral("ClearFileTreeButton"));

        verticalLayout_2->addWidget(ClearFileTreeButton);


        verticalLayout_4->addWidget(frame);

        frame_2 = new QFrame(MainViewer);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        tabWidget = new QTabWidget(frame_2);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        SkeletonTab = new QWidget();
        SkeletonTab->setObjectName(QStringLiteral("SkeletonTab"));
        verticalLayout_3 = new QVBoxLayout(SkeletonTab);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        BoneTree = new QTreeWidget(SkeletonTab);
        BoneTree->setObjectName(QStringLiteral("BoneTree"));
        BoneTree->setEditTriggers(QAbstractItemView::DoubleClicked);
        BoneTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
        BoneTree->setIconSize(QSize(24, 24));

        verticalLayout_3->addWidget(BoneTree);

        tabWidget->addTab(SkeletonTab, QString());
        MatrixTab = new QWidget();
        MatrixTab->setObjectName(QStringLiteral("MatrixTab"));
        verticalLayout_20 = new QVBoxLayout(MatrixTab);
        verticalLayout_20->setObjectName(QStringLiteral("verticalLayout_20"));
        verticalLayout_19 = new QVBoxLayout();
        verticalLayout_19->setSpacing(6);
        verticalLayout_19->setObjectName(QStringLiteral("verticalLayout_19"));
        groupBox_3 = new QGroupBox(MatrixTab);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        tableW_OriginalFile_initialTransf = new QTableWidget(groupBox_3);
        if (tableW_OriginalFile_initialTransf->columnCount() < 4)
            tableW_OriginalFile_initialTransf->setColumnCount(4);
        if (tableW_OriginalFile_initialTransf->rowCount() < 4)
            tableW_OriginalFile_initialTransf->setRowCount(4);
        tableW_OriginalFile_initialTransf->setObjectName(QStringLiteral("tableW_OriginalFile_initialTransf"));
        tableW_OriginalFile_initialTransf->setGeometry(QRect(10, 20, 261, 131));
        QFont font;
        font.setPointSize(9);
        tableW_OriginalFile_initialTransf->setFont(font);
        tableW_OriginalFile_initialTransf->setRowCount(4);
        tableW_OriginalFile_initialTransf->setColumnCount(4);
        tableW_OriginalFile_initialTransf->horizontalHeader()->setVisible(false);
        tableW_OriginalFile_initialTransf->horizontalHeader()->setDefaultSectionSize(64);
        tableW_OriginalFile_initialTransf->verticalHeader()->setVisible(false);
        tableW_OriginalFile_initialTransf->verticalHeader()->setDefaultSectionSize(32);
        tableW_OriginalFile_initialTransf->verticalHeader()->setHighlightSections(true);
        tableW_OriginalFile_initialTransf->verticalHeader()->setMinimumSectionSize(23);
        tableW_OriginalFile_animation = new QTableWidget(groupBox_3);
        if (tableW_OriginalFile_animation->columnCount() < 4)
            tableW_OriginalFile_animation->setColumnCount(4);
        if (tableW_OriginalFile_animation->rowCount() < 4)
            tableW_OriginalFile_animation->setRowCount(4);
        tableW_OriginalFile_animation->setObjectName(QStringLiteral("tableW_OriginalFile_animation"));
        tableW_OriginalFile_animation->setGeometry(QRect(270, 20, 261, 131));
        tableW_OriginalFile_animation->setFont(font);
        tableW_OriginalFile_animation->setRowCount(4);
        tableW_OriginalFile_animation->setColumnCount(4);
        tableW_OriginalFile_animation->horizontalHeader()->setVisible(false);
        tableW_OriginalFile_animation->horizontalHeader()->setDefaultSectionSize(64);
        tableW_OriginalFile_animation->verticalHeader()->setVisible(false);
        tableW_OriginalFile_animation->verticalHeader()->setDefaultSectionSize(32);
        tableW_OriginalFile_animation->verticalHeader()->setHighlightSections(true);
        tableW_OriginalFile_animation->verticalHeader()->setMinimumSectionSize(23);

        verticalLayout_19->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(MatrixTab);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        tableW_Ogre_initialTransf = new QTableWidget(groupBox_4);
        if (tableW_Ogre_initialTransf->columnCount() < 4)
            tableW_Ogre_initialTransf->setColumnCount(4);
        if (tableW_Ogre_initialTransf->rowCount() < 4)
            tableW_Ogre_initialTransf->setRowCount(4);
        tableW_Ogre_initialTransf->setObjectName(QStringLiteral("tableW_Ogre_initialTransf"));
        tableW_Ogre_initialTransf->setGeometry(QRect(10, 20, 261, 131));
        tableW_Ogre_initialTransf->setFont(font);
        tableW_Ogre_initialTransf->setRowCount(4);
        tableW_Ogre_initialTransf->setColumnCount(4);
        tableW_Ogre_initialTransf->horizontalHeader()->setVisible(false);
        tableW_Ogre_initialTransf->horizontalHeader()->setDefaultSectionSize(64);
        tableW_Ogre_initialTransf->verticalHeader()->setVisible(false);
        tableW_Ogre_initialTransf->verticalHeader()->setDefaultSectionSize(32);
        tableW_Ogre_initialTransf->verticalHeader()->setHighlightSections(true);
        tableW_Ogre_initialTransf->verticalHeader()->setMinimumSectionSize(23);
        tableW_Ogre_animation = new QTableWidget(groupBox_4);
        if (tableW_Ogre_animation->columnCount() < 4)
            tableW_Ogre_animation->setColumnCount(4);
        if (tableW_Ogre_animation->rowCount() < 4)
            tableW_Ogre_animation->setRowCount(4);
        tableW_Ogre_animation->setObjectName(QStringLiteral("tableW_Ogre_animation"));
        tableW_Ogre_animation->setGeometry(QRect(270, 20, 261, 131));
        tableW_Ogre_animation->setFont(font);
        tableW_Ogre_animation->setRowCount(4);
        tableW_Ogre_animation->setColumnCount(4);
        tableW_Ogre_animation->horizontalHeader()->setVisible(false);
        tableW_Ogre_animation->horizontalHeader()->setDefaultSectionSize(64);
        tableW_Ogre_animation->verticalHeader()->setVisible(false);
        tableW_Ogre_animation->verticalHeader()->setDefaultSectionSize(32);
        tableW_Ogre_animation->verticalHeader()->setHighlightSections(true);
        tableW_Ogre_animation->verticalHeader()->setMinimumSectionSize(23);

        verticalLayout_19->addWidget(groupBox_4);


        verticalLayout_20->addLayout(verticalLayout_19);

        tabWidget->addTab(MatrixTab, QString());
        ModelPackTab = new QWidget();
        ModelPackTab->setObjectName(QStringLiteral("ModelPackTab"));
        verticalLayout_5 = new QVBoxLayout(ModelPackTab);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        groupBox = new QGroupBox(ModelPackTab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frame_7 = new QFrame(groupBox);
        frame_7->setObjectName(QStringLiteral("frame_7"));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        horizontalLayout_7 = new QHBoxLayout(frame_7);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_4 = new QLabel(frame_7);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMinimumSize(QSize(130, 0));

        horizontalLayout_7->addWidget(label_4);

        label_5 = new QLabel(frame_7);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setMinimumSize(QSize(130, 0));

        horizontalLayout_7->addWidget(label_5);


        verticalLayout->addWidget(frame_7);

        frame_3 = new QFrame(groupBox);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        verticalLayout_8 = new QVBoxLayout(frame_3);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_2 = new QLabel(frame_3);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        ModelSkeletonCombo = new QComboBox(frame_3);
        ModelSkeletonCombo->setObjectName(QStringLiteral("ModelSkeletonCombo"));

        horizontalLayout->addWidget(ModelSkeletonCombo);


        verticalLayout_8->addLayout(horizontalLayout);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label = new QLabel(frame_3);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_5->addWidget(label);

        ModelBoneCombo = new QComboBox(frame_3);
        ModelBoneCombo->setObjectName(QStringLiteral("ModelBoneCombo"));

        horizontalLayout_5->addWidget(ModelBoneCombo);


        verticalLayout_8->addLayout(horizontalLayout_5);


        verticalLayout->addWidget(frame_3);


        verticalLayout_5->addWidget(groupBox);

        tabWidget->addTab(ModelPackTab, QString());
        ModelTab = new QWidget();
        ModelTab->setObjectName(QStringLiteral("ModelTab"));
        tabWidget->addTab(ModelTab, QString());
        MeshTab = new QWidget();
        MeshTab->setObjectName(QStringLiteral("MeshTab"));
        tabWidget->addTab(MeshTab, QString());
        SubmeshTab = new QWidget();
        SubmeshTab->setObjectName(QStringLiteral("SubmeshTab"));
        tabWidget->addTab(SubmeshTab, QString());
        MaterialTab = new QWidget();
        MaterialTab->setObjectName(QStringLiteral("MaterialTab"));
        tabWidget->addTab(MaterialTab, QString());
        TextureTab = new QWidget();
        TextureTab->setObjectName(QStringLiteral("TextureTab"));
        horizontalLayout_9 = new QHBoxLayout(TextureTab);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        frame_9 = new QFrame(TextureTab);
        frame_9->setObjectName(QStringLiteral("frame_9"));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        verticalLayout_11 = new QVBoxLayout(frame_9);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        tabWidget_2 = new QTabWidget(frame_9);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tabWidget_2->setMinimumSize(QSize(242, 0));
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_9 = new QVBoxLayout(tab_2);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        Cluster1_1 = new CGradientColorsView(tab_2);
        Cluster1_1->setObjectName(QStringLiteral("Cluster1_1"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Cluster1_1->sizePolicy().hasHeightForWidth());
        Cluster1_1->setSizePolicy(sizePolicy);
        Cluster1_1->setMinimumSize(QSize(0, 50));
        Cluster1_1->setMaximumSize(QSize(16777215, 50));

        verticalLayout_9->addWidget(Cluster1_1);

        Cluster1_2 = new CGradientColorsView(tab_2);
        Cluster1_2->setObjectName(QStringLiteral("Cluster1_2"));
        sizePolicy.setHeightForWidth(Cluster1_2->sizePolicy().hasHeightForWidth());
        Cluster1_2->setSizePolicy(sizePolicy);
        Cluster1_2->setMinimumSize(QSize(0, 50));
        Cluster1_2->setMaximumSize(QSize(16777215, 50));

        verticalLayout_9->addWidget(Cluster1_2);

        Cluster1_3 = new CGradientColorsView(tab_2);
        Cluster1_3->setObjectName(QStringLiteral("Cluster1_3"));
        sizePolicy.setHeightForWidth(Cluster1_3->sizePolicy().hasHeightForWidth());
        Cluster1_3->setSizePolicy(sizePolicy);
        Cluster1_3->setMinimumSize(QSize(0, 50));
        Cluster1_3->setMaximumSize(QSize(16777215, 50));

        verticalLayout_9->addWidget(Cluster1_3);

        Cluster1_4 = new CGradientColorsView(tab_2);
        Cluster1_4->setObjectName(QStringLiteral("Cluster1_4"));
        sizePolicy.setHeightForWidth(Cluster1_4->sizePolicy().hasHeightForWidth());
        Cluster1_4->setSizePolicy(sizePolicy);
        Cluster1_4->setMinimumSize(QSize(0, 50));
        Cluster1_4->setMaximumSize(QSize(16777215, 50));

        verticalLayout_9->addWidget(Cluster1_4);

        tabWidget_2->addTab(tab_2, QString());
        tab_10 = new QWidget();
        tab_10->setObjectName(QStringLiteral("tab_10"));
        verticalLayout_18 = new QVBoxLayout(tab_10);
        verticalLayout_18->setObjectName(QStringLiteral("verticalLayout_18"));
        Cluster2_1 = new CGradientColorsView(tab_10);
        Cluster2_1->setObjectName(QStringLiteral("Cluster2_1"));
        sizePolicy.setHeightForWidth(Cluster2_1->sizePolicy().hasHeightForWidth());
        Cluster2_1->setSizePolicy(sizePolicy);
        Cluster2_1->setMinimumSize(QSize(0, 50));
        Cluster2_1->setMaximumSize(QSize(16777215, 50));

        verticalLayout_18->addWidget(Cluster2_1);

        Cluster2_2 = new CGradientColorsView(tab_10);
        Cluster2_2->setObjectName(QStringLiteral("Cluster2_2"));
        sizePolicy.setHeightForWidth(Cluster2_2->sizePolicy().hasHeightForWidth());
        Cluster2_2->setSizePolicy(sizePolicy);
        Cluster2_2->setMinimumSize(QSize(0, 50));
        Cluster2_2->setMaximumSize(QSize(16777215, 50));

        verticalLayout_18->addWidget(Cluster2_2);

        Cluster2_3 = new CGradientColorsView(tab_10);
        Cluster2_3->setObjectName(QStringLiteral("Cluster2_3"));
        sizePolicy.setHeightForWidth(Cluster2_3->sizePolicy().hasHeightForWidth());
        Cluster2_3->setSizePolicy(sizePolicy);
        Cluster2_3->setMinimumSize(QSize(0, 50));
        Cluster2_3->setMaximumSize(QSize(16777215, 50));

        verticalLayout_18->addWidget(Cluster2_3);

        Cluster2_4 = new CGradientColorsView(tab_10);
        Cluster2_4->setObjectName(QStringLiteral("Cluster2_4"));
        sizePolicy.setHeightForWidth(Cluster2_4->sizePolicy().hasHeightForWidth());
        Cluster2_4->setSizePolicy(sizePolicy);
        Cluster2_4->setMinimumSize(QSize(0, 50));
        Cluster2_4->setMaximumSize(QSize(16777215, 50));

        verticalLayout_18->addWidget(Cluster2_4);

        tabWidget_2->addTab(tab_10, QString());
        tab_11 = new QWidget();
        tab_11->setObjectName(QStringLiteral("tab_11"));
        verticalLayout_17 = new QVBoxLayout(tab_11);
        verticalLayout_17->setObjectName(QStringLiteral("verticalLayout_17"));
        Cluster3_1 = new CGradientColorsView(tab_11);
        Cluster3_1->setObjectName(QStringLiteral("Cluster3_1"));
        sizePolicy.setHeightForWidth(Cluster3_1->sizePolicy().hasHeightForWidth());
        Cluster3_1->setSizePolicy(sizePolicy);
        Cluster3_1->setMinimumSize(QSize(0, 50));
        Cluster3_1->setMaximumSize(QSize(16777215, 50));

        verticalLayout_17->addWidget(Cluster3_1);

        Cluster3_2 = new CGradientColorsView(tab_11);
        Cluster3_2->setObjectName(QStringLiteral("Cluster3_2"));
        sizePolicy.setHeightForWidth(Cluster3_2->sizePolicy().hasHeightForWidth());
        Cluster3_2->setSizePolicy(sizePolicy);
        Cluster3_2->setMinimumSize(QSize(0, 50));
        Cluster3_2->setMaximumSize(QSize(16777215, 50));

        verticalLayout_17->addWidget(Cluster3_2);

        Cluster3_3 = new CGradientColorsView(tab_11);
        Cluster3_3->setObjectName(QStringLiteral("Cluster3_3"));
        sizePolicy.setHeightForWidth(Cluster3_3->sizePolicy().hasHeightForWidth());
        Cluster3_3->setSizePolicy(sizePolicy);
        Cluster3_3->setMinimumSize(QSize(0, 50));
        Cluster3_3->setMaximumSize(QSize(16777215, 50));

        verticalLayout_17->addWidget(Cluster3_3);

        Cluster3_4 = new CGradientColorsView(tab_11);
        Cluster3_4->setObjectName(QStringLiteral("Cluster3_4"));
        sizePolicy.setHeightForWidth(Cluster3_4->sizePolicy().hasHeightForWidth());
        Cluster3_4->setSizePolicy(sizePolicy);
        Cluster3_4->setMinimumSize(QSize(0, 50));
        Cluster3_4->setMaximumSize(QSize(16777215, 50));

        verticalLayout_17->addWidget(Cluster3_4);

        tabWidget_2->addTab(tab_11, QString());
        tab_12 = new QWidget();
        tab_12->setObjectName(QStringLiteral("tab_12"));
        verticalLayout_16 = new QVBoxLayout(tab_12);
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        Cluster4_1 = new CGradientColorsView(tab_12);
        Cluster4_1->setObjectName(QStringLiteral("Cluster4_1"));
        sizePolicy.setHeightForWidth(Cluster4_1->sizePolicy().hasHeightForWidth());
        Cluster4_1->setSizePolicy(sizePolicy);
        Cluster4_1->setMinimumSize(QSize(0, 50));
        Cluster4_1->setMaximumSize(QSize(16777215, 50));

        verticalLayout_16->addWidget(Cluster4_1);

        Cluster4_2 = new CGradientColorsView(tab_12);
        Cluster4_2->setObjectName(QStringLiteral("Cluster4_2"));
        sizePolicy.setHeightForWidth(Cluster4_2->sizePolicy().hasHeightForWidth());
        Cluster4_2->setSizePolicy(sizePolicy);
        Cluster4_2->setMinimumSize(QSize(0, 50));
        Cluster4_2->setMaximumSize(QSize(16777215, 50));

        verticalLayout_16->addWidget(Cluster4_2);

        Cluster4_3 = new CGradientColorsView(tab_12);
        Cluster4_3->setObjectName(QStringLiteral("Cluster4_3"));
        sizePolicy.setHeightForWidth(Cluster4_3->sizePolicy().hasHeightForWidth());
        Cluster4_3->setSizePolicy(sizePolicy);
        Cluster4_3->setMinimumSize(QSize(0, 50));
        Cluster4_3->setMaximumSize(QSize(16777215, 50));

        verticalLayout_16->addWidget(Cluster4_3);

        Cluster4_4 = new CGradientColorsView(tab_12);
        Cluster4_4->setObjectName(QStringLiteral("Cluster4_4"));
        sizePolicy.setHeightForWidth(Cluster4_4->sizePolicy().hasHeightForWidth());
        Cluster4_4->setSizePolicy(sizePolicy);
        Cluster4_4->setMinimumSize(QSize(0, 50));
        Cluster4_4->setMaximumSize(QSize(16777215, 50));

        verticalLayout_16->addWidget(Cluster4_4);

        tabWidget_2->addTab(tab_12, QString());
        tab_13 = new QWidget();
        tab_13->setObjectName(QStringLiteral("tab_13"));
        verticalLayout_15 = new QVBoxLayout(tab_13);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        Cluster5_1 = new CGradientColorsView(tab_13);
        Cluster5_1->setObjectName(QStringLiteral("Cluster5_1"));
        sizePolicy.setHeightForWidth(Cluster5_1->sizePolicy().hasHeightForWidth());
        Cluster5_1->setSizePolicy(sizePolicy);
        Cluster5_1->setMinimumSize(QSize(0, 50));
        Cluster5_1->setMaximumSize(QSize(16777215, 50));

        verticalLayout_15->addWidget(Cluster5_1);

        Cluster5_2 = new CGradientColorsView(tab_13);
        Cluster5_2->setObjectName(QStringLiteral("Cluster5_2"));
        sizePolicy.setHeightForWidth(Cluster5_2->sizePolicy().hasHeightForWidth());
        Cluster5_2->setSizePolicy(sizePolicy);
        Cluster5_2->setMinimumSize(QSize(0, 50));
        Cluster5_2->setMaximumSize(QSize(16777215, 50));

        verticalLayout_15->addWidget(Cluster5_2);

        Cluster5_3 = new CGradientColorsView(tab_13);
        Cluster5_3->setObjectName(QStringLiteral("Cluster5_3"));
        sizePolicy.setHeightForWidth(Cluster5_3->sizePolicy().hasHeightForWidth());
        Cluster5_3->setSizePolicy(sizePolicy);
        Cluster5_3->setMinimumSize(QSize(0, 50));
        Cluster5_3->setMaximumSize(QSize(16777215, 50));

        verticalLayout_15->addWidget(Cluster5_3);

        Cluster5_4 = new CGradientColorsView(tab_13);
        Cluster5_4->setObjectName(QStringLiteral("Cluster5_4"));
        sizePolicy.setHeightForWidth(Cluster5_4->sizePolicy().hasHeightForWidth());
        Cluster5_4->setSizePolicy(sizePolicy);
        Cluster5_4->setMinimumSize(QSize(0, 50));
        Cluster5_4->setMaximumSize(QSize(16777215, 50));

        verticalLayout_15->addWidget(Cluster5_4);

        tabWidget_2->addTab(tab_13, QString());
        tab_14 = new QWidget();
        tab_14->setObjectName(QStringLiteral("tab_14"));
        verticalLayout_14 = new QVBoxLayout(tab_14);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        Cluster6_1 = new CGradientColorsView(tab_14);
        Cluster6_1->setObjectName(QStringLiteral("Cluster6_1"));
        sizePolicy.setHeightForWidth(Cluster6_1->sizePolicy().hasHeightForWidth());
        Cluster6_1->setSizePolicy(sizePolicy);
        Cluster6_1->setMinimumSize(QSize(0, 50));
        Cluster6_1->setMaximumSize(QSize(16777215, 50));

        verticalLayout_14->addWidget(Cluster6_1);

        Cluster6_2 = new CGradientColorsView(tab_14);
        Cluster6_2->setObjectName(QStringLiteral("Cluster6_2"));
        sizePolicy.setHeightForWidth(Cluster6_2->sizePolicy().hasHeightForWidth());
        Cluster6_2->setSizePolicy(sizePolicy);
        Cluster6_2->setMinimumSize(QSize(0, 50));
        Cluster6_2->setMaximumSize(QSize(16777215, 50));

        verticalLayout_14->addWidget(Cluster6_2);

        Cluster6_3 = new CGradientColorsView(tab_14);
        Cluster6_3->setObjectName(QStringLiteral("Cluster6_3"));
        sizePolicy.setHeightForWidth(Cluster6_3->sizePolicy().hasHeightForWidth());
        Cluster6_3->setSizePolicy(sizePolicy);
        Cluster6_3->setMinimumSize(QSize(0, 50));
        Cluster6_3->setMaximumSize(QSize(16777215, 50));

        verticalLayout_14->addWidget(Cluster6_3);

        Cluster6_4 = new CGradientColorsView(tab_14);
        Cluster6_4->setObjectName(QStringLiteral("Cluster6_4"));
        sizePolicy.setHeightForWidth(Cluster6_4->sizePolicy().hasHeightForWidth());
        Cluster6_4->setSizePolicy(sizePolicy);
        Cluster6_4->setMinimumSize(QSize(0, 50));
        Cluster6_4->setMaximumSize(QSize(16777215, 50));

        verticalLayout_14->addWidget(Cluster6_4);

        tabWidget_2->addTab(tab_14, QString());
        tab_15 = new QWidget();
        tab_15->setObjectName(QStringLiteral("tab_15"));
        verticalLayout_13 = new QVBoxLayout(tab_15);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        Cluster7_1 = new CGradientColorsView(tab_15);
        Cluster7_1->setObjectName(QStringLiteral("Cluster7_1"));
        sizePolicy.setHeightForWidth(Cluster7_1->sizePolicy().hasHeightForWidth());
        Cluster7_1->setSizePolicy(sizePolicy);
        Cluster7_1->setMinimumSize(QSize(0, 50));
        Cluster7_1->setMaximumSize(QSize(16777215, 50));

        verticalLayout_13->addWidget(Cluster7_1);

        Cluster7_2 = new CGradientColorsView(tab_15);
        Cluster7_2->setObjectName(QStringLiteral("Cluster7_2"));
        sizePolicy.setHeightForWidth(Cluster7_2->sizePolicy().hasHeightForWidth());
        Cluster7_2->setSizePolicy(sizePolicy);
        Cluster7_2->setMinimumSize(QSize(0, 50));
        Cluster7_2->setMaximumSize(QSize(16777215, 50));

        verticalLayout_13->addWidget(Cluster7_2);

        Cluster7_3 = new CGradientColorsView(tab_15);
        Cluster7_3->setObjectName(QStringLiteral("Cluster7_3"));
        sizePolicy.setHeightForWidth(Cluster7_3->sizePolicy().hasHeightForWidth());
        Cluster7_3->setSizePolicy(sizePolicy);
        Cluster7_3->setMinimumSize(QSize(0, 50));
        Cluster7_3->setMaximumSize(QSize(16777215, 50));

        verticalLayout_13->addWidget(Cluster7_3);

        Cluster7_4 = new CGradientColorsView(tab_15);
        Cluster7_4->setObjectName(QStringLiteral("Cluster7_4"));
        sizePolicy.setHeightForWidth(Cluster7_4->sizePolicy().hasHeightForWidth());
        Cluster7_4->setSizePolicy(sizePolicy);
        Cluster7_4->setMinimumSize(QSize(0, 50));
        Cluster7_4->setMaximumSize(QSize(16777215, 50));

        verticalLayout_13->addWidget(Cluster7_4);

        tabWidget_2->addTab(tab_15, QString());
        tab_9 = new QWidget();
        tab_9->setObjectName(QStringLiteral("tab_9"));
        verticalLayout_12 = new QVBoxLayout(tab_9);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        Cluster8_1 = new CGradientColorsView(tab_9);
        Cluster8_1->setObjectName(QStringLiteral("Cluster8_1"));
        sizePolicy.setHeightForWidth(Cluster8_1->sizePolicy().hasHeightForWidth());
        Cluster8_1->setSizePolicy(sizePolicy);
        Cluster8_1->setMinimumSize(QSize(0, 50));
        Cluster8_1->setMaximumSize(QSize(16777215, 50));

        verticalLayout_12->addWidget(Cluster8_1);

        Cluster8_2 = new CGradientColorsView(tab_9);
        Cluster8_2->setObjectName(QStringLiteral("Cluster8_2"));
        sizePolicy.setHeightForWidth(Cluster8_2->sizePolicy().hasHeightForWidth());
        Cluster8_2->setSizePolicy(sizePolicy);
        Cluster8_2->setMinimumSize(QSize(0, 50));
        Cluster8_2->setMaximumSize(QSize(16777215, 50));

        verticalLayout_12->addWidget(Cluster8_2);

        Cluster8_3 = new CGradientColorsView(tab_9);
        Cluster8_3->setObjectName(QStringLiteral("Cluster8_3"));
        sizePolicy.setHeightForWidth(Cluster8_3->sizePolicy().hasHeightForWidth());
        Cluster8_3->setSizePolicy(sizePolicy);
        Cluster8_3->setMinimumSize(QSize(0, 50));
        Cluster8_3->setMaximumSize(QSize(16777215, 50));

        verticalLayout_12->addWidget(Cluster8_3);

        Cluster8_4 = new CGradientColorsView(tab_9);
        Cluster8_4->setObjectName(QStringLiteral("Cluster8_4"));
        sizePolicy.setHeightForWidth(Cluster8_4->sizePolicy().hasHeightForWidth());
        Cluster8_4->setSizePolicy(sizePolicy);
        Cluster8_4->setMinimumSize(QSize(0, 50));
        Cluster8_4->setMaximumSize(QSize(16777215, 50));

        verticalLayout_12->addWidget(Cluster8_4);

        tabWidget_2->addTab(tab_9, QString());

        verticalLayout_11->addWidget(tabWidget_2);

        frame_4 = new QFrame(frame_9);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame_4->sizePolicy().hasHeightForWidth());
        frame_4->setSizePolicy(sizePolicy1);
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        horizontalLayout_6 = new QHBoxLayout(frame_4);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        GenerateClusterButton = new QPushButton(frame_4);
        GenerateClusterButton->setObjectName(QStringLiteral("GenerateClusterButton"));

        horizontalLayout_6->addWidget(GenerateClusterButton);

        ResetClusterButton = new QPushButton(frame_4);
        ResetClusterButton->setObjectName(QStringLiteral("ResetClusterButton"));

        horizontalLayout_6->addWidget(ResetClusterButton);


        verticalLayout_11->addWidget(frame_4);


        horizontalLayout_9->addWidget(frame_9);

        frame_8 = new QFrame(TextureTab);
        frame_8->setObjectName(QStringLiteral("frame_8"));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        verticalLayout_10 = new QVBoxLayout(frame_8);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        ddsTextureView = new QGraphicsView(frame_8);
        ddsTextureView->setObjectName(QStringLiteral("ddsTextureView"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(ddsTextureView->sizePolicy().hasHeightForWidth());
        ddsTextureView->setSizePolicy(sizePolicy2);
        ddsTextureView->setMinimumSize(QSize(256, 256));
        ddsTextureView->setMaximumSize(QSize(256, 256));

        verticalLayout_10->addWidget(ddsTextureView);

        frame_6 = new QFrame(frame_8);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        sizePolicy1.setHeightForWidth(frame_6->sizePolicy().hasHeightForWidth());
        frame_6->setSizePolicy(sizePolicy1);
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        horizontalLayout_8 = new QHBoxLayout(frame_6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        LoadTextureButton = new QPushButton(frame_6);
        LoadTextureButton->setObjectName(QStringLiteral("LoadTextureButton"));

        horizontalLayout_8->addWidget(LoadTextureButton);

        SaveTextureButton = new QPushButton(frame_6);
        SaveTextureButton->setObjectName(QStringLiteral("SaveTextureButton"));

        horizontalLayout_8->addWidget(SaveTextureButton);


        verticalLayout_10->addWidget(frame_6);


        horizontalLayout_9->addWidget(frame_8);

        tabWidget->addTab(TextureTab, QString());

        horizontalLayout_2->addWidget(tabWidget);


        verticalLayout_4->addWidget(frame_2);


        horizontalLayout_3->addLayout(verticalLayout_4);

        frame_5 = new QFrame(MainViewer);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(frame_5->sizePolicy().hasHeightForWidth());
        frame_5->setSizePolicy(sizePolicy3);
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        verticalLayout_6 = new QVBoxLayout(frame_5);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        AnimationTree = new QTreeWidget(frame_5);
        AnimationTree->setObjectName(QStringLiteral("AnimationTree"));

        verticalLayout_6->addWidget(AnimationTree);

        ClearAnimationListButton = new QPushButton(frame_5);
        ClearAnimationListButton->setObjectName(QStringLiteral("ClearAnimationListButton"));

        verticalLayout_6->addWidget(ClearAnimationListButton);

        groupBox_2 = new QGroupBox(frame_5);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_7 = new QVBoxLayout(groupBox_2);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        PlayButton = new QPushButton(groupBox_2);
        PlayButton->setObjectName(QStringLiteral("PlayButton"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        PlayButton->setIcon(icon);

        horizontalLayout_4->addWidget(PlayButton);

        PauseButton = new QPushButton(groupBox_2);
        PauseButton->setObjectName(QStringLiteral("PauseButton"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        PauseButton->setIcon(icon1);

        horizontalLayout_4->addWidget(PauseButton);

        StopButton = new QPushButton(groupBox_2);
        StopButton->setObjectName(QStringLiteral("StopButton"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        StopButton->setIcon(icon2);

        horizontalLayout_4->addWidget(StopButton);

        PreviousFrameButton = new QPushButton(groupBox_2);
        PreviousFrameButton->setObjectName(QStringLiteral("PreviousFrameButton"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/previous_frame.png"), QSize(), QIcon::Normal, QIcon::Off);
        PreviousFrameButton->setIcon(icon3);

        horizontalLayout_4->addWidget(PreviousFrameButton);

        NextFrameButton = new QPushButton(groupBox_2);
        NextFrameButton->setObjectName(QStringLiteral("NextFrameButton"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/next_frame.png"), QSize(), QIcon::Normal, QIcon::Off);
        NextFrameButton->setIcon(icon4);

        horizontalLayout_4->addWidget(NextFrameButton);


        verticalLayout_7->addLayout(horizontalLayout_4);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        PlaybackLoopCheck = new QCheckBox(groupBox_2);
        PlaybackLoopCheck->setObjectName(QStringLiteral("PlaybackLoopCheck"));
        PlaybackLoopCheck->setChecked(true);

        horizontalLayout_10->addWidget(PlaybackLoopCheck);

        PlayAnimationText = new QLabel(groupBox_2);
        PlayAnimationText->setObjectName(QStringLiteral("PlayAnimationText"));

        horizontalLayout_10->addWidget(PlayAnimationText);


        verticalLayout_7->addLayout(horizontalLayout_10);


        verticalLayout_6->addWidget(groupBox_2);


        horizontalLayout_3->addWidget(frame_5);


        retranslateUi(MainViewer);

        tabWidget->setCurrentIndex(1);
        tabWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainViewer);
    } // setupUi

    void retranslateUi(QWidget *MainViewer)
    {
        MainViewer->setWindowTitle(QApplication::translate("MainViewer", "Form", 0));
        QTreeWidgetItem *___qtreewidgetitem = FileTree->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("MainViewer", "File Tree", 0));
        ClearFileTreeButton->setText(QApplication::translate("MainViewer", "Clear list", 0));
        QTreeWidgetItem *___qtreewidgetitem1 = BoneTree->headerItem();
        ___qtreewidgetitem1->setText(0, QApplication::translate("MainViewer", "Bone Tree", 0));
        tabWidget->setTabText(tabWidget->indexOf(SkeletonTab), QApplication::translate("MainViewer", "Skeleton", 0));
        groupBox_3->setTitle(QApplication::translate("MainViewer", "Original File (emd esk nsk ean fbx)", 0));
        groupBox_4->setTitle(QApplication::translate("MainViewer", "Xenoviewer (Ogre)", 0));
        tabWidget->setTabText(tabWidget->indexOf(MatrixTab), QApplication::translate("MainViewer", "Matrix", 0));
        groupBox->setTitle(QApplication::translate("MainViewer", "Global", 0));
        label_4->setText(QApplication::translate("MainViewer", "Vertex Count:", 0));
        label_5->setText(QApplication::translate("MainViewer", "Polygon Count:", 0));
        label_2->setText(QApplication::translate("MainViewer", "Attach to Skeleton:", 0));
        label->setText(QApplication::translate("MainViewer", "Attach to bone:", 0));
        tabWidget->setTabText(tabWidget->indexOf(ModelPackTab), QApplication::translate("MainViewer", "Model Pack", 0));
        tabWidget->setTabText(tabWidget->indexOf(ModelTab), QApplication::translate("MainViewer", "Model", 0));
        tabWidget->setTabText(tabWidget->indexOf(MeshTab), QApplication::translate("MainViewer", "Mesh", 0));
        tabWidget->setTabText(tabWidget->indexOf(SubmeshTab), QApplication::translate("MainViewer", "Submesh", 0));
        tabWidget->setTabText(tabWidget->indexOf(MaterialTab), QApplication::translate("MainViewer", "Material", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_2), QApplication::translate("MainViewer", "1", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_10), QApplication::translate("MainViewer", "2", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_11), QApplication::translate("MainViewer", "3", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_12), QApplication::translate("MainViewer", "4", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_13), QApplication::translate("MainViewer", "5", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_14), QApplication::translate("MainViewer", "6", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_15), QApplication::translate("MainViewer", "7", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_9), QApplication::translate("MainViewer", "8", 0));
        GenerateClusterButton->setText(QApplication::translate("MainViewer", "Apply", 0));
        ResetClusterButton->setText(QApplication::translate("MainViewer", "Reset", 0));
        LoadTextureButton->setText(QApplication::translate("MainViewer", "Load from file...", 0));
        SaveTextureButton->setText(QApplication::translate("MainViewer", "Save to file...", 0));
        tabWidget->setTabText(tabWidget->indexOf(TextureTab), QApplication::translate("MainViewer", "Texture", 0));
        QTreeWidgetItem *___qtreewidgetitem2 = AnimationTree->headerItem();
        ___qtreewidgetitem2->setText(0, QApplication::translate("MainViewer", "Animation List", 0));
        ClearAnimationListButton->setText(QApplication::translate("MainViewer", "Clear list", 0));
        groupBox_2->setTitle(QApplication::translate("MainViewer", "Playback Controls", 0));
        PlayButton->setText(QString());
        PauseButton->setText(QString());
        StopButton->setText(QString());
        PreviousFrameButton->setText(QString());
        NextFrameButton->setText(QString());
        PlaybackLoopCheck->setText(QApplication::translate("MainViewer", "Playback Animation Loop", 0));
        PlayAnimationText->setText(QApplication::translate("MainViewer", "0/0", 0));
    } // retranslateUi

};

namespace Ui {
    class MainViewer: public Ui_MainViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINVIEWER_H
