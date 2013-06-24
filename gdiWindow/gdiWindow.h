// gdiWindow.h
// Authored by Alex Shows
// Released under the MIT License (http://opensource.org/licenses/mit-license.php) 
// 
// Header declaring the CGDIWindow class
// 
// An child class which inherits window creation from CSimpleWindow
// and provides its own window procedure for handling messages

#pragma once

#define MAX_DATAPOINTS 300

#include "simpleWindow.h"
#include "dataPoint.h"
#include <vector>
#include <time.h>
#include <sstream>

#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

class CGDIWindow:CSimpleWindow
{
public:
	CGDIWindow();
	CGDIWindow(const int width, const int height);
	CGDIWindow(string name, const int width, const int height);
	~CGDIWindow(void);
	void create_window();
	void message_loop();
	void update_window(HDC hdc);
	void draw_point(HDC hdc, CDataPoint* pDP, const int xOffset = 0, const int yOffset = 0);
private:
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	vector<CDataPoint> vPoints;
	LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void initialize_data();
	void analyze_data();
};