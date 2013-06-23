// winMain.cpp
// Authored by Alex Shows
// Released under the MIT License (http://opensource.org/licenses/mit-license.php) 
// 
// A test harness to work with GDI+ on a pure Win32 window

#include "winMain.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	CGDIWindow simWin(500, 500);
	simWin.create_window();
	simWin.message_loop();

	return 0;
}