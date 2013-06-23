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

CGDIWindow::CGDIWindow(string name, const int w, const int h)
{
	appName = name;
	hWnd = NULL;
	width = w;
	height = h;
}

CGDIWindow::~CGDIWindow(void)
{
	// Where's the best place for this? Here or after the parent's message_loop returns?
//	GdiplusShutdown(gdiplusToken); 
}

void CGDIWindow::create_window()
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	CSimpleWindow::create_window();
}

void CGDIWindow::message_loop()
{
	CSimpleWindow::message_loop();

	GdiplusShutdown(gdiplusToken);
}

LRESULT CALLBACK CGDIWindow::windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch(uMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		update_window(hdc);
		EndPaint(hWnd, &ps);
		break;
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

void CGDIWindow::update_window(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen(Color(255,0,0,255));
	graphics.DrawLine(&pen, 0, 0, 200, 100);
/*
	SolidBrush  brush(Color(255, 0, 0, 255));
	FontFamily  fontFamily(L"Times New Roman");
	Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
	PointF      pointF(10.0f, 20.0f);
	
	graphics.DrawString(L"Hello World!", -1, &font, pointF, &brush);
*/
}