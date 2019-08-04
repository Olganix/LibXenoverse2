#include "MainWindow.h"

#ifdef _MSC_VER
#    pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main(int argc, char *argv[])
{
	char ownPth[MAX_PATH];

	// When NULL is passed to GetModuleHandle, the handle of the exe itself is returned
	HMODULE hModule = GetModuleHandle(NULL);
	if (hModule != NULL)
	{
		// Use GetModuleFileName() with module handle to get the path
		GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));
		string workdir = string(ownPth);
		
		workdir = Ogre::StringUtil::replaceAll(Ogre::StringUtil::replaceAll( Ogre::StringUtil::replaceAll( Ogre::StringUtil::replaceAll( Ogre::StringUtil::replaceAll( Ogre::StringUtil::replaceAll(workdir, "bin\\", ""), "bin/", ""), "Debug\\", ""), "Debug/", ""), "Release\\", ""), "Release/", "");
		workdir = folderFromFilename(workdir);

		::SetCurrentDirectoryA((LPCSTR)(workdir.c_str()));
	}
	
	
	
	QApplication app(argc, argv);
	app.setApplicationName("Xenoviewer");
	MainWindow mainWin;
	qApp->installEventFilter(&mainWin);

#if defined(Q_OS_SYMBIAN)
	mainWin.showMaximized();
#else
	mainWin.show();
	mainWin.setWindowState(Qt::WindowActive);
	mainWin.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, mainWin.size(), app.desktop()->availableGeometry()));
#endif

	if (argc > 1)
	{
		QStringList pathList;

		for (int i = 1; i < argc; i++)
			pathList.append( QString(argv[i]) );

		mainWin.openFiles(pathList);
	}

	return app.exec();
}
