// gdiWindow.h
// Authored by Alex Shows
// Released under the MIT License (http://opensource.org/licenses/mit-license.php) 
// 
// Header declaring the CGDIWindow class
// 
// An child class which inherits window creation from CSimpleWindow
// and provides its own window procedure for handling messages

#pragma once

#include "simpleWindow.h"

class CGDIWindow:CSimpleWindow
{
public:
	CGDIWindow();
	CGDIWindow(const int width, const int height);
	~CGDIWindow(void);
	void create_window();
	void message_loop();
private:
	LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};