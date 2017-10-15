/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include "MainWindow.h"
#include "MainViewer.h"

MainWindow::MainWindow()
{
	//All mainWindows
	QWidget *central_widget = new QWidget(this);



	

	// Build Right Panel, the visualisation
	QWidget *view_widget = new QWidget(central_widget);
	
	//Ogre Parts
	ogre_widget = new QtOgre::OgreWidget(view_widget);
	ogre_widget->setMinimumWidth(500);
	ogre_widget->setMinimumHeight(500);
	ogre_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	
	//organize the Right panel
	QVBoxLayout *view_widget_layout = new QVBoxLayout;
	view_widget_layout->addWidget(ogre_widget);
	view_widget->setLayout(view_widget_layout);




	// Build Left Panel, the configuration
	main_viewer = new MainViewer(central_widget, ogre_widget);				//look at MainViewer definition

	
	//organize the main windows
	QHBoxLayout *main_horizontal_layout = new QHBoxLayout;
	main_horizontal_layout->addWidget(main_viewer);
	main_horizontal_layout->addWidget(view_widget);
	central_widget->setLayout(main_horizontal_layout);

	//declare the root widget
	setCentralWidget(central_widget);
	
	//create actions and menu withc use actions
	createActions();
	createMenus();
	ogre_widget->createDebug(central_widget);

	resize(QSize(1298, 800));
}

void MainWindow::createActions()
{
	openAct = new QAction(QIcon(":/resources/icons/open_files.png"), tr("&Add Files..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	/*
	saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save..."), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the document to disk"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
	*/

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	aboutAct = new QAction(tr("&About"), this);
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAct = new QAction(tr("About &Qt"), this);
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
}

void MainWindow::closeEvent(QCloseEvent *event)
{

}

void MainWindow::open()
{
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::ExistingFiles);
	dialog.setNameFilter(trUtf8("Xenoviewer read (*.emd *.emo *.esk *.nsk *.ean *.fbx);;DRAGON BALL XENOVERSE Formats (*.emd *.emo *.esk *.nsk *.ean);; Fbx files (*.fbx)"));
	QStringList fileNames;

	if (dialog.exec())
		fileNames = dialog.selectedFiles();

	if (!fileNames.isEmpty())
		openFiles(fileNames);
}

bool MainWindow::save()
{
	QString fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
		return false;

	//Todo

	return true;
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("About Xenoviewer"),
		tr("<b>Xenoviewer</b> is an open-source Model/Skeleton/Animation viewer for the PC version of DRAGON BALL XENOVERSE. Dependencies used:"
		"<ul>"
		"<li><b>LibXenoverse</b>: <a href=\"https://github.com/DarioSamo/LibXenoverse\">Github Repository</a></li>"
		"<li><b>Ogre3D 2.0</b>: <a href=\"http://www.ogre3d.org/\">Homepage</a></li>"
		"<li><b>QtProject 5.4.1</b>: <a href=\"http://qt-project.org/\">Homepage</a></li>"
		"</ul>"
		));
}



void MainWindow::resetCamera_Charac() { ogre_widget->resetCamera_Charac(); }
void MainWindow::resetCamera_Stage() { ogre_widget->resetCamera_Stage(); }

bool MainWindow::openFiles(const QStringList& pathList)
{
	bool success = true;

	list<EMDOgre *> new_emd_list;
	list<ESKOgre *> new_esk_list;
	list<EANOgre *> new_ean_list;

	ogre_widget->addFiles(pathList, new_emd_list, new_esk_list, new_ean_list);
	main_viewer->createFileTreeItems(new_emd_list, new_esk_list, new_ean_list);
	return success;
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

		if (obj == main_viewer)
			main_viewer->keyPressEvent(keyEvent);
	}
	return QObject::eventFilter(obj, event);
}