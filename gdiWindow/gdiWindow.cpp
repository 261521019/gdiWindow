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
	initialize_data();
	analyze_data();
}

CGDIWindow::CGDIWindow(const int w, const int h)
{
	appName = "GDI Window";
	hWnd = NULL;
	width = w;
	height = h;
	initialize_data();
	analyze_data();
}

CGDIWindow::CGDIWindow(string name, const int w, const int h)
{
	appName = name;
	hWnd = NULL;
	width = w;
	height = h;
	initialize_data();
	analyze_data();
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
	case WM_ERASEBKGND:
		break;
	case WM_LBUTTONDOWN:
		initialize_data();
		analyze_data();
		InvalidateRect(hWnd, NULL, NULL);
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
	if(vPoints.size() < 1)
		return;

	// Create a memory device context and compatible bitmap to draw into
	HDC originalHDC = hdc;
	hdc = CreateCompatibleDC(originalHDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(originalHDC, width, height);
	SelectObject(hdc, hBitmap);

	Graphics graphics(hdc);

	// Offset within the main window where the cluster of points go
	int clusterOffsetX = 50; // in pixels
	int clusterOffsetY = 50; // in pixels
	int clusterXbounds = vPoints[0].get_x_bounds() + 6; // 6 is padding for size
	int clusterYbounds = vPoints[0].get_y_bounds() + 6; // 6 is padding for size

	// Background fill
	SolidBrush bgFill(Color(255,255,255,255));
	graphics.FillRectangle(&bgFill, 0, 0, width, height);

	// Inset fill 
	SolidBrush insetFill(Color(255,240,240,240));
	graphics.FillRectangle(&insetFill, clusterOffsetX, clusterOffsetY, clusterXbounds, clusterYbounds);

	// Outline the inset region
	Pen insetOutline(Color(255,0,0,0));
	graphics.DrawRectangle(&insetOutline, clusterOffsetX, clusterOffsetY, clusterXbounds, clusterYbounds);
	
	// Heading
	SolidBrush  brush(Color(255, 0, 0, 255));
	FontFamily  fontFamily(L"Lucida Sans");
	Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
	PointF      pointF(50.0f, 10.0f);
	graphics.DrawString(L"Cluster Analysis", -1, &font, pointF, &brush);

	// Draw each data point
	for (vector<CDataPoint>::iterator it = vPoints.begin() ; it != vPoints.end(); ++it)
	{
		CDataPoint &dataPoint = *it;
		draw_point(hdc, &dataPoint, clusterOffsetX, clusterOffsetY);
	}

	// Draw each cluster center
	for (vector<CDataPoint>::iterator cIt = vClusters.begin() ; cIt != vClusters.end(); ++cIt)
	{
		CDataPoint &dataPoint = *cIt;
		draw_cluster(hdc, &dataPoint, clusterOffsetX, clusterOffsetY);
	}

	// Copy from the memory DC to the original
	BitBlt(originalHDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY); 

	// Clean up
	DeleteObject(hBitmap);
	DeleteDC(hdc);
	ReleaseDC(hWnd, hdc);
}

void CGDIWindow::initialize_data()
{
	vPoints.clear();

	srand((unsigned int)time(NULL));

	for(int i=0; i < MAX_DATAPOINTS; i++)
	{
		// Load a randomized data point into the vector
		vPoints.push_back(CDataPoint(rand()%CDP_X_UPPER_BOUND, 
									rand()%CDP_Y_UPPER_BOUND,
									rand()%6,
									rand()%CDP_COLOR_UPPER_BOUND,
									rand()%CDP_COLOR_UPPER_BOUND,
									rand()%CDP_COLOR_UPPER_BOUND));
	}

	vClusters.clear();

	for(int j=0; j < MAX_CLUSTERS; j++)
	{
		// Load a randomized data point into the vector
		vClusters.push_back(CDataPoint(rand()%CDP_X_UPPER_BOUND, 
									rand()%CDP_Y_UPPER_BOUND,
									10, // No need to randomize, this is overridden elsewhere
									rand()%(CDP_COLOR_UPPER_BOUND-100),
									rand()%(CDP_COLOR_UPPER_BOUND-100),
									rand()%(CDP_COLOR_UPPER_BOUND-100)));
	}
}

// Draw a single data point, which is a circle filled with a transparent color
// based on the data in the CDataPoint passed
void CGDIWindow::draw_point(HDC hdc, CDataPoint* pDP, const int xOffset, const int yOffset)
{
	Graphics gfx(hdc);

	Pen pen(Color(pDP->get_r(), 
				pDP->get_g(), 
				pDP->get_b()));
	
	// Ellipse with same width/height so a circle of the color of the pen
	gfx.DrawEllipse(&pen, (const int)(pDP->get_x() + (pDP->get_size()/2) + xOffset),
						(const int)(pDP->get_y() + (pDP->get_size()/2) + yOffset),
						(const int)(pDP->get_size()), 
						(const int)(pDP->get_size()));

	// 50% transparent but otherwise the same color
	SolidBrush solidBrush(Color(50, pDP->get_r(),
									pDP->get_g(),
									pDP->get_b()));

	// Fill the same region with the 50% transparent color
	gfx.FillEllipse(&solidBrush, (const int)(pDP->get_x() + (pDP->get_size()/2) + xOffset),
						(const int)(pDP->get_y() + (pDP->get_size()/2) + yOffset),
						(const int)(pDP->get_size()), 
						(const int)(pDP->get_size()));


}

// Draw a single cluster, which is a sqaure filled with a transparent color
// based on the data in the CDataPoint passed
// The size parameter isn't used - rather the size of the rectangles are hard coded within
void CGDIWindow::draw_cluster(HDC hdc, CDataPoint* pDP, const int xOffset, const int yOffset)
{
	int clusterSize = 10;

	Graphics gfx(hdc);

	Pen pen(Color(pDP->get_r(), 
				pDP->get_g(), 
				pDP->get_b()));
	
	// Rectangle marker
	gfx.DrawRectangle(&pen, (const int)(pDP->get_x() + (clusterSize/2) + xOffset),
						(const int)(pDP->get_y() + (clusterSize/2) + yOffset),
						clusterSize, 
						clusterSize);

	// 80% transparent but otherwise the same color
	SolidBrush solidBrush(Color(80, pDP->get_r(),
									pDP->get_g(),
									pDP->get_b()));

	// Fill the same region with the transparent color
	gfx.FillRectangle(&solidBrush, (const int)(pDP->get_x() + (clusterSize/2) + xOffset),
						(const int)(pDP->get_y() + (clusterSize/2) + yOffset),
						clusterSize, 
						clusterSize);
}

// Analyze the data points to associate data points with clusters, and color code accordingly
// TODO: the clusters are random, but need to work on a least squares approach to minimizing and 
//		finding the best cluster centers
void CGDIWindow::analyze_data()
{
	// For each data point, measure the distance to each cluster and color the data point 
	// according to the closest cluster
	for(vector<CDataPoint>::iterator it = vPoints.begin(); it != vPoints.end(); ++it)
	{
		float lastDistance = CDP_X_UPPER_BOUND + CDP_Y_UPPER_BOUND;

		for(vector<CDataPoint>::iterator cIt = vClusters.begin(); cIt != vClusters.end(); ++cIt)
		{
			float X2minusX1 = (float)(it->get_x() - (float)(cIt->get_x())); 
			float Y2minusY1 = (float)(it->get_y() - (float)(cIt->get_y()));

			float squaredXdiff = X2minusX1*X2minusX1;
			float squaredYdiff = Y2minusY1*Y2minusY1;
		
			float distance = sqrt(squaredXdiff + squaredYdiff);

			// If this cluster is closer than the last, color code to this cluster
			if(distance < lastDistance)
			{
				lastDistance = distance;
				it->set_r(cIt->get_r());
				it->set_b(cIt->get_b());
				it->set_g(cIt->get_g());
			}
		} // end FOR each cluster 
	} // end FOR each data point
}