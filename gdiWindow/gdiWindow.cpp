// gdiWindow.cpp
// Authored by Alex Shows
// Released under the MIT License (http://opensource.org/licenses/mit-license.php) 
//
// Implementation of the CGDIWindow class 
// An child class which inherits window creation from CSimpleWindow
// and provides its own window procedure to respond to window messages

#include "gdiWindow.h"

CGDIWindow::CGDIWindow()
{
}

CGDIWindow::CGDIWindow(const int w, const int h)
{
	appName = "GDI Window";
	hWnd = NULL;
	width = w;
	height = h;
}

CGDIWindow::~CGDIWindow(void)
{
}

void CGDIWindow::create_window()
{
	CSimpleWindow::create_window();
}

void CGDIWindow::message_loop()
{
	CSimpleWindow::message_loop();
}

LRESULT CALLBACK CGDIWindow::windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		MessageBox(NULL, "GDI Window Saw A Mouse Down", "Mouse Event", MB_OK);
		break;
	case WM_CREATE:
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}