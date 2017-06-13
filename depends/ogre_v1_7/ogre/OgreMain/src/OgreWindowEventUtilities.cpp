/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2009 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "OgreStableHeaders.h"
#include "OgreWindowEventUtilities.h"
#include "OgreRenderWindow.h"
#include "OgreLogManager.h"
#include "OgreRoot.h"
#include "OgreException.h"
#include "OgreStringConverter.h"
#include <windowsx.h>
#include <shellapi.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#include <X11/Xlib.h>
void GLXProc( Ogre::RenderWindow *win, const XEvent &event );
#endif

using namespace Ogre;

WindowEventUtilities::WindowEventListeners WindowEventUtilities::_msListeners;
WindowEventUtilities::Windows WindowEventUtilities::_msWindows;
bool WindowEventUtilities::mMinimized = false;
bool WindowEventUtilities::mTrayModeDebug = false;


//--------------------------------------------------------------------------------//
void WindowEventUtilities::messagePump()
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	// Windows Message Loop (NULL means check all HWNDs belonging to this context)
	MSG  msg;
	while( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	//GLX Message Pump
	Windows::iterator win = _msWindows.begin();
	Windows::iterator end = _msWindows.end();

	Display* xDisplay = 0; // same for all windows
	
	for (; win != end; win++)
	{
	    XID xid;
	    XEvent event;

	    if (!xDisplay)
		(*win)->getCustomAttribute("XDISPLAY", &xDisplay);

	    (*win)->getCustomAttribute("WINDOW", &xid);

	    while (XCheckWindowEvent (xDisplay, xid, StructureNotifyMask | VisibilityChangeMask | FocusChangeMask, &event))
	    {
		GLXProc(*win, event);
	    }

	    // The ClientMessage event does not appear under any Event Mask
	    while (XCheckTypedWindowEvent (xDisplay, xid, ClientMessage, &event))
	    {
		GLXProc(*win, event);
	    }
	}
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE && !defined __OBJC__
	// OSX Message Pump
	EventRef event = NULL;
	EventTargetRef targetWindow;
	targetWindow = GetEventDispatcherTarget();
    
    // If we are unable to get the target then we no longer care about events.
    if( !targetWindow ) return;
    
    // Grab the next event, process it if it is a window event
	if( ReceiveNextEvent( 0, NULL, kEventDurationNoWait, true, &event ) == noErr )
	{
        // Dispatch the event
		SendEventToEventTarget( event, targetWindow );
   		ReleaseEvent( event );
	}
#endif
}

//--------------------------------------------------------------------------------//
void WindowEventUtilities::addWindowEventListener( RenderWindow* window, WindowEventListener* listener )
{
	_msListeners.insert(std::make_pair(window, listener));
}

//--------------------------------------------------------------------------------//
void WindowEventUtilities::removeWindowEventListener( RenderWindow* window, WindowEventListener* listener )
{
	WindowEventListeners::iterator i = _msListeners.begin(), e = _msListeners.end();

	for( ; i != e; ++i )
	{
		if( i->first == window && i->second == listener )
		{
			_msListeners.erase(i);
			break;
		}
	}
}

//--------------------------------------------------------------------------------//
void WindowEventUtilities::_addRenderWindow(RenderWindow* window)
{
	_msWindows.push_back(window);
}

//--------------------------------------------------------------------------------//
void WindowEventUtilities::_removeRenderWindow(RenderWindow* window)
{
	Windows::iterator i = std::find(_msWindows.begin(), _msWindows.end(), window);
	if( i != _msWindows.end() )
		_msWindows.erase( i );
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
//--------------------------------------------------------------------------------//
LRESULT CALLBACK WindowEventUtilities::_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{	// Store pointer to Win32Window in user data area
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)(((LPCREATESTRUCT)lParam)->lpCreateParams));
		return 0;
	}

	// look up window instance
	// note: it is possible to get a WM_SIZE before WM_CREATE
	RenderWindow* win = (RenderWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (!win)
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	LogManager* log = LogManager::getSingletonPtr();
	//Iterator of all listeners registered to this RenderWindow
	WindowEventListeners::iterator index,
        start = _msListeners.lower_bound(win),
        end = _msListeners.upper_bound(win);

	switch( uMsg )
	{
	case WM_ACTIVATE:
	{
        bool active = (LOWORD(wParam) != WA_INACTIVE);
        if( active )
        {
		    win->setActive( true );
        }
        else
        {
            if( win->isDeactivatedOnFocusChange() )
            {
    		    win->setActive( false );
            }
        }

	    for( ; start != end; ++start )
		    (start->second)->windowFocusChange(win);
		break;
	}
	case WM_SYSKEYDOWN:
		switch( wParam )
		{
		case VK_CONTROL:
		case VK_SHIFT:
		case VK_MENU: //ALT
			//return zero to bypass defProc and signal we processed the message
			return 0;
		}
		break;
	case WM_SYSKEYUP:
		switch( wParam )
		{
		case VK_CONTROL:
		case VK_SHIFT:
		case VK_MENU: //ALT
		case VK_F10:
			//return zero to bypass defProc and signal we processed the message
			return 0;
		}
		break;
	case WM_SYSCHAR:
		// return zero to bypass defProc and signal we processed the message, unless it's an ALT-space
		if (wParam != VK_SPACE)
			return 0;
		break;
	case WM_ENTERSIZEMOVE:
		log->logMessage("WM_ENTERSIZEMOVE");
		break;
	case WM_EXITSIZEMOVE:
		log->logMessage("WM_EXITSIZEMOVE");
		break;
	case WM_MOVE:
		log->logMessage("WM_MOVE");
		win->windowMovedOrResized();
		for(index = start; index != end; ++index)
			(index->second)->windowMoved(win);
		break;
	case WM_DISPLAYCHANGE:
		win->windowMovedOrResized();
		for(index = start; index != end; ++index)
			(index->second)->windowResized(win);
		break;
	case WM_SIZE:
		log->logMessage("WM_SIZE");
		win->windowMovedOrResized();
		for(index = start; index != end; ++index)
			(index->second)->windowResized(win);
		break;
	case WM_GETMINMAXINFO:
		// Prevent the window from going smaller than some minimu size
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
		break;


	case MSG_MINIMIZEASK:	minimize(hWnd, wParam, lParam);	break;
	case MSG_RESTOREASK:	restore(hWnd, wParam, lParam);	break;
        

	case MSG_DEBUGASK:		WindowEventUtilities::mTrayModeDebug = true;	break;
	case MSG_NODEBUGASK:	WindowEventUtilities::mTrayModeDebug = false;	break;

	case MSG_MINTRAYICON:
		{
			if (wParam != IDI_TRAYICON)
				break;											//if it's not the icon we planted, then go away

			if(lParam == WM_LBUTTONUP)
			{
				if(WindowEventUtilities::mTrayModeDebug)
				{
					if(WindowEventUtilities::mMinimized)
						restore(hWnd, wParam, lParam);
					else
						minimize(hWnd, wParam, lParam);
				}
					
			}else if(lParam == WM_RBUTTONUP){					//time to display a menu.
                HMENU myMenu = NULL;
                myMenu = CreatePopupMenu();						//create our menu. You'll want to error-check this, because if it fails the next few functions may produce segmentation faults, and your menu won't work.
                
                //IDM_TRAYEXIT, IDM_TRAYRESTORE, and IDM_TRAYHELP are #defined constants.
                AppendMenu(myMenu, MF_STRING, IDM_TRAYABOUT, "A Propos");
				if(WindowEventUtilities::mTrayModeDebug)
				{
					AppendMenu(myMenu, MF_STRING, IDM_TRAYRESTORE, "Restore");
					AppendMenu(myMenu, MF_STRING, IDM_TRAYHIDE, "Hide");
				}
				AppendMenu(myMenu, MF_STRING, IDM_TRAYEXIT, "Quitter");
                
                tagPOINT cursorPos;
				GetCursorPos(&cursorPos);
				int x_tmp = (int) cursorPos.x;
				int y_tmp = (int) cursorPos.y;

                SetForegroundWindow(hWnd);						//even though the window is hidden, we must set it to the foreground window because of popup-menu peculiarities. See the Remarks section of the MSDN page for TrackPopupMenu.
                UINT clicked = TrackPopupMenu(myMenu, TPM_RETURNCMD | TPM_NONOTIFY /*don't send me WM_COMMAND messages about this window, instead return the identifier of the clicked menu item*/, x_tmp, y_tmp, 0, hWnd, NULL); //display the menu. you MUST #include <windowsx.h> to use those two macros.
                SendMessage(hWnd, WM_NULL, 0, 0); //send benign message to window to make sure the menu goes away.
                
				if(clicked == IDM_TRAYEXIT)
				{
					SendMessage(hWnd, WM_CLOSE, 0, 0);
				}else if(clicked == IDM_TRAYHIDE){
					minimize(hWnd, wParam, lParam);
				}else if(clicked == IDM_TRAYRESTORE){
					restore(hWnd, wParam, lParam);
				}else if(clicked == IDM_TRAYABOUT){

					LPSTR m_moduleFileName = new char[MAX_PATH];
					HMODULE hModule = GetModuleHandle(NULL);
					GetModuleFileNameA(hModule, m_moduleFileName, MAX_PATH);
					String moduleName = StringUtil::split( StringUtil::split( Ogre::String(m_moduleFileName) ,"\\").back() ,"/").back();	//on ne garde que le nom du module: on vire le path.
					
					Ogre::String version_str = "unknow";
					
					DWORD dwFileVerInfStat;
					DWORD m_versionInfoSize = GetFileVersionInfoSizeA(m_moduleFileName, &dwFileVerInfStat);
					if(m_versionInfoSize>0)
					{
						LPVOID m_versionInfo = HeapAlloc(GetProcessHeap(), 0, sizeof(BYTE) * m_versionInfoSize);
						if(GetFileVersionInfoA(m_moduleFileName, 0, m_versionInfoSize, m_versionInfo))
						{
							UINT uLen;
							VS_FIXEDFILEINFO *lpFfi;

							VerQueryValueA( m_versionInfo , "\\" , (LPVOID *)&lpFfi , &uLen );

							DWORD dwFileVersionMS = lpFfi->dwFileVersionMS;
							DWORD dwFileVersionLS = lpFfi->dwFileVersionLS;

							DWORD dwLeftMost     = HIWORD(dwFileVersionMS);
							DWORD dwSecondLeft   = LOWORD(dwFileVersionMS);
							DWORD dwSecondRight  = HIWORD(dwFileVersionLS);
							DWORD dwRightMost    = LOWORD(dwFileVersionLS);

							version_str = StringConverter::toString((unsigned long)dwLeftMost) +"."+ StringConverter::toString((unsigned long)dwSecondLeft) +"."+ StringConverter::toString((unsigned long)dwSecondRight) +"."+ StringConverter::toString((unsigned long)dwRightMost);

							HeapFree(GetProcessHeap(), 0, m_versionInfo);
						}else{
							HeapFree(GetProcessHeap(), 0, m_versionInfo);
						}
					}
					delete m_moduleFileName;
					m_moduleFileName = NULL;

					MessageBox(hWnd, (moduleName +" version "+ version_str).c_str(), ("A propos "+ moduleName).c_str(), MB_OK | MB_ICONINFORMATION);
				}
            }
		}
		break;

	case WM_CLOSE:
	{
		log->logMessage("WM_CLOSE");
		removeIcon(hWnd, wParam, lParam);

		bool close = true;
		for(index = start; index != end; ++index)
		{
			if (!(index->second)->windowClosing(win))
				close = false;
		}
		if (!close) return 0;

		for(index = _msListeners.lower_bound(win); index != end; ++index)
			(index->second)->windowClosed(win);
		win->destroy();
		return 0;
	}
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

void WindowEventUtilities::minimize(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    NOTIFYICONDATA nid = { 0 };
    nid.cbSize = sizeof(NOTIFYICONDATA);							//this helps the OS determine stuff. (I have no idea, but it is necessary.
    nid.hWnd = hwnd;												//the hWnd and uID members allow the OS to uniquely identify your icon. One window (the hWnd) can have more than one icon, as long as they have unique uIDs.
    nid.uID = IDI_TRAYICON;											//sorry, had forgotten this in my original example. but without, the function probably wouldn't work
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;					//some flags that determine the tray's behavior: we're adding an icon, we want the tray to send a message to the window identified by hWnd when something happens to our icon (see uCallbackMesage member below). our icon has a tooltip.
    nid.uCallbackMessage = MSG_MINTRAYICON;							//this message must be handled in hwnd's window procedure. more info below.
    nid.hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(wParam), IMAGE_ICON, 16, 16, 0);	//load up the icon:get the HINSTANCE to this program, grab the icon out of our resource file, tells the versatile LoadImage function that we are loading an icon, x and y values. we want a 16x16-pixel icon for the tray. no flags necessary. these flags specify special behavior, such as loading the icon from a file instead of a resource. see source list below for MSDN docs on LoadImage.
	strcpy(nid.szTip, "GeoRender");				//this string cannot be longer than 64 characters including the NULL terminator (which is added by default to string literals).
    //There are some more members of the NOTIFYICONDATA struct that are for advanced features we aren't using. See sources below for MSDN docs if you want to use balloon tips (only Win2000/XP).
    Shell_NotifyIcon(NIM_ADD, &nid);
    ShowWindow(hwnd, SW_HIDE);
    WindowEventUtilities::mMinimized = true;
}

void WindowEventUtilities::restore(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    NOTIFYICONDATA nid = { 0 };
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = IDI_TRAYICON;
    //Shell_NotifyIcon(NIM_DELETE, &nid);
    ShowWindow(hwnd, SW_SHOW);
    WindowEventUtilities::mMinimized = false;
}

void WindowEventUtilities::removeIcon(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    NOTIFYICONDATA nid = { 0 };
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = IDI_TRAYICON;
    Shell_NotifyIcon(NIM_DELETE, &nid);
    WindowEventUtilities::mMinimized = false;
}

#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
//--------------------------------------------------------------------------------//
void GLXProc( RenderWindow *win, const XEvent &event )
{
	//An iterator for the window listeners
	WindowEventUtilities::WindowEventListeners::iterator index,
		start = WindowEventUtilities::_msListeners.lower_bound(win),
		end   = WindowEventUtilities::_msListeners.upper_bound(win);

	switch(event.type)
	{
	case ClientMessage:
	{
		::Atom atom;
		win->getCustomAttribute("ATOM", &atom);
		if(event.xclient.format == 32 && event.xclient.data.l[0] == (long)atom)
		{	//Window closed by window manager
			//Send message first, to allow app chance to unregister things that need done before
			//window is shutdown
			bool close = true;
            for(index = start ; index != end; ++index)
			{
				if (!(index->second)->windowClosing(win))
					close = false;
			}
			if (!close) return;

            for(index = start ; index != end; ++index)
                (index->second)->windowClosed(win);
			win->destroy();
		}
		break;
	}
	case DestroyNotify:
	{
		if (!win->isClosed())
		{
			// Window closed without window manager warning.
            for(index = start ; index != end; ++index)
                (index->second)->windowClosed(win);
			win->destroy();
		}
		break;
	}
	case ConfigureNotify:
	{    
        // This could be slightly more efficient if windowMovedOrResized took arguments:
		unsigned int oldWidth, oldHeight, oldDepth;
		int oldLeft, oldTop;
		win->getMetrics(oldWidth, oldHeight, oldDepth, oldLeft, oldTop);
		win->windowMovedOrResized();

		unsigned int newWidth, newHeight, newDepth;
		int newLeft, newTop;
		win->getMetrics(newWidth, newHeight, newDepth, newLeft, newTop);

		if (newLeft != oldLeft || newTop != oldTop)
		{
            for(index = start ; index != end; ++index)
                (index->second)->windowMoved(win);
		}

		if (newWidth != oldWidth || newHeight != oldHeight)
		{
            for(index = start ; index != end; ++index)
                (index->second)->windowResized(win);
		}
		break;
	}
	case FocusIn:     // Gained keyboard focus
	case FocusOut:    // Lost keyboard focus
        for(index = start ; index != end; ++index)
            (index->second)->windowFocusChange(win);
		break;
	case MapNotify:   //Restored
		win->setActive( true );
        for(index = start ; index != end; ++index)
            (index->second)->windowFocusChange(win);
		break;
	case UnmapNotify: //Minimised
		win->setActive( false );
		win->setVisible( false );
        for(index = start ; index != end; ++index)
            (index->second)->windowFocusChange(win);
		break;
	case VisibilityNotify:
		switch(event.xvisibility.state)
		{
		case VisibilityUnobscured:
			win->setActive( true );
			win->setVisible( true );
			break;
		case VisibilityPartiallyObscured:
			win->setActive( true );
			win->setVisible( true );
			break;
		case VisibilityFullyObscured:
			win->setActive( false );
			win->setVisible( false );
			break;
		}
        for(index = start ; index != end; ++index)
            (index->second)->windowFocusChange(win);
		break;
	default:
		break;
	} //End switch event.type
}
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE && !defined __OBJC__
//--------------------------------------------------------------------------------//
OSStatus WindowEventUtilities::_CarbonWindowHandler(EventHandlerCallRef nextHandler, EventRef event, void* wnd)
{
    OSStatus status = noErr;
    
    // Only events from our window should make it here
    // This ensures that our user data is our WindowRef
    RenderWindow* curWindow = (RenderWindow*)wnd;
    if(!curWindow) return eventNotHandledErr;
    
    //Iterator of all listeners registered to this RenderWindow
	WindowEventListeners::iterator index,
        start = _msListeners.lower_bound(curWindow),
        end = _msListeners.upper_bound(curWindow);
    
    // We only get called if a window event happens
    UInt32 eventKind = GetEventKind( event );

    switch( eventKind )
    {
        case kEventWindowActivated:
            curWindow->setActive( true );
            for( ; start != end; ++start )
                (start->second)->windowFocusChange(curWindow);
            break;
        case kEventWindowDeactivated:
            if( curWindow->isDeactivatedOnFocusChange() )
            {
                curWindow->setActive( false );
            }

            for( ; start != end; ++start )
                (start->second)->windowFocusChange(curWindow);

            break;
        case kEventWindowShown:
        case kEventWindowExpanded:
            curWindow->setActive( true );
            curWindow->setVisible( true );
            for( ; start != end; ++start )
                (start->second)->windowFocusChange(curWindow);
            break;
        case kEventWindowHidden:
        case kEventWindowCollapsed:
            curWindow->setActive( false );
            curWindow->setVisible( false );
            for( ; start != end; ++start )
                (start->second)->windowFocusChange(curWindow);
            break;            
        case kEventWindowDragCompleted:
            curWindow->windowMovedOrResized();
            for( ; start != end; ++start )
				(start->second)->windowMoved(curWindow);
            break;
        case kEventWindowBoundsChanged:
            curWindow->windowMovedOrResized();
            for( ; start != end; ++start )
				(start->second)->windowResized(curWindow);
            break;
		case kEventWindowClose:
		{
			bool close = true;
			for( ; start != end; ++start )
			{
				if (!(start->second)->windowClosing(curWindow))
					close = false;
			}
			if (close)
				// This will cause event handling to continue on to the standard handler, which calls
				// DisposeWindow(), which leads to the 'kEventWindowClosed' event
				status = eventNotHandledErr;
			break;
		}
        case kEventWindowClosed:
            curWindow->destroy();
            for( ; start != end; ++start )
				(start->second)->windowClosed(curWindow);
            break;
        default:
            status = eventNotHandledErr;
            break;
    }
    
    return status;
}
#endif
