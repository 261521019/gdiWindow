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
	assign_data();
}

CGDIWindow::CGDIWindow(const int w, const int h)
{
	appName = "GDI Window";
	hWnd = NULL;
	width = w;
	height = h;
	initialize_data();
	assign_data();
}

CGDIWindow::CGDIWindow(string name, const int w, const int h)
{
	appName = name;
	hWnd = NULL;
	width = w;
	height = h;
	initialize_data();
	assign_data();
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
	case WM_KEYDOWN:
		handle_key((const char)wParam);
        break;
	case WM_LBUTTONDOWN:
		assign_data();
		compute_centroids();
		InvalidateRect(hWnd, NULL, NULL);
		break;
	case WM_RBUTTONDOWN:
		initialize_data();
		//assign_data();
		//compute_centroids();
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
	int insetOffsetX = 50; // in pixels
	int insetOffsetY = 50; // in pixels
	int insetXbounds = vPoints[0].get_x_bounds() + 6; // 6 is padding for size
	int insetYbounds = vPoints[0].get_y_bounds() + 6; // 6 is padding for size

	// Background fill
	SolidBrush bgFill(Color(255,255,255,255));
	graphics.FillRectangle(&bgFill, 0, 0, width, height);

	// Inset fill 
	SolidBrush insetFill(Color(255,240,240,240));
	graphics.FillRectangle(&insetFill, insetOffsetX, insetOffsetY, insetXbounds, insetYbounds);

	// Outline the inset region
	Pen insetOutline(Color(255,0,0,0));
	graphics.DrawRectangle(&insetOutline, insetOffsetX, insetOffsetY, insetXbounds, insetYbounds);
	
	// Heading
	SolidBrush  brush(Color(255, 0, 0, 255));
	FontFamily  fontFamily(L"Lucida Sans");
	Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
	PointF      pointF(50.0f, 10.0f);
	graphics.DrawString(L"K-Means Cluster Analysis", -1, &font, pointF, &brush);

	// Draw each data point
	for (vector<CDataPoint>::iterator it = vPoints.begin() ; it != vPoints.end(); ++it)
	{
		CDataPoint &dataPoint = *it;
		draw_point(hdc, &dataPoint, insetOffsetX, insetOffsetY);
	}

	// Draw each cluster center
	for (vector<CDataPoint>::iterator cIt = vClusters.begin() ; cIt != vClusters.end(); ++cIt)
	{
		CDataPoint &dataPoint = *cIt;
		draw_cluster(hdc, &dataPoint, insetOffsetX, insetOffsetY);
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
		int modulo = i%4;
		// Artificial steering of clusters
		switch(modulo)
		{
		case 0:
			// Upper left quadrant
			vPoints.push_back(CDataPoint(rand()%CDP_X_UPPER_BOUND/6 + 20, 
										rand()%CDP_Y_UPPER_BOUND/6 + 20,
										3,
										rand()%CDP_COLOR_UPPER_BOUND,
										rand()%CDP_COLOR_UPPER_BOUND,
										rand()%CDP_COLOR_UPPER_BOUND));
			break;
		case 1:
			// Upper right quadrant
			vPoints.push_back(CDataPoint(rand()%CDP_X_UPPER_BOUND/6 + 300, 
										rand()%CDP_Y_UPPER_BOUND/6 + 20,
										3,
										rand()%CDP_COLOR_UPPER_BOUND,
										rand()%CDP_COLOR_UPPER_BOUND,
										rand()%CDP_COLOR_UPPER_BOUND));
			break;
		case 2:
			// Lower left quadrant
			vPoints.push_back(CDataPoint(rand()%CDP_X_UPPER_BOUND/6 + 20, 
										rand()%CDP_Y_UPPER_BOUND/6 + 300,
										3,
										rand()%CDP_COLOR_UPPER_BOUND,
										rand()%CDP_COLOR_UPPER_BOUND,
										rand()%CDP_COLOR_UPPER_BOUND));
			break;
		case 3:
			// Lower right quadrant
			vPoints.push_back(CDataPoint(rand()%CDP_X_UPPER_BOUND/6 + 300, 
										rand()%CDP_Y_UPPER_BOUND/6 + 300,
										3,
										rand()%CDP_COLOR_UPPER_BOUND,
										rand()%CDP_COLOR_UPPER_BOUND,
										rand()%CDP_COLOR_UPPER_BOUND));
			break;
		default:
			vPoints.push_back(CDataPoint(rand()%CDP_X_UPPER_BOUND, 
										rand()%CDP_Y_UPPER_BOUND,
										3, /*rand()%6,*/
										rand()%CDP_COLOR_UPPER_BOUND,
										rand()%CDP_COLOR_UPPER_BOUND,
										rand()%CDP_COLOR_UPPER_BOUND));
			break;
		} // end case
	} // end for each data point

	vClusters.clear();

	for(int j=0; j < MAX_CLUSTERS; j++)
	{
		// Use RGBY for the first 4 colors
		if(j<4)
		{
			int redVal, greenVal, blueVal;
			redVal = greenVal = blueVal = 0;

			switch(j)
			{
			case 0:
				redVal = 255;
				break;
			case 1:
				greenVal = 150;
				break;
			case 2:
				blueVal = 255;
				break;
			case 3: // Yellow
				redVal = 200;
				greenVal = 200;
				break;
			default:
				// Should never arrive here
				break;
			}

			vClusters.push_back(CDataPoint(rand()%CDP_X_UPPER_BOUND,
										   rand()%CDP_Y_UPPER_BOUND,
										   10, // No need to randomize, this is overriden elsewhere
										   redVal,
										   greenVal,
										   blueVal));
		} // end if j<4
		else // else j >= 4, so generate a random color
		{
			// Load a randomized data point into the vector
			vClusters.push_back(CDataPoint(rand()%CDP_X_UPPER_BOUND, 
										rand()%CDP_Y_UPPER_BOUND,
										10, // No need to randomize, this is overridden elsewhere
										rand()%(CDP_COLOR_UPPER_BOUND-100),
										rand()%(CDP_COLOR_UPPER_BOUND-100),
										rand()%(CDP_COLOR_UPPER_BOUND-100)));
		} // end else
	} // end for each cluster
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

	solidBrush.SetColor(Color(30, pDP->get_r(),
								  pDP->get_g(),
								  pDP->get_b()));

	// Fill in an outer region with a lower transparency version on the same color
	gfx.FillEllipse(&solidBrush, (const int)(pDP->get_x() + (pDP->get_size()/2) -10 + xOffset),
								 (const int)(pDP->get_y() + (pDP->get_size()/2) -10 + yOffset),
								 (const int)(pDP->get_size() + 20),
								 (const int)(pDP->get_size() + 20));
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

// Assign each data point to the closest cluster and color code accordingly
void CGDIWindow::assign_data()
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

			// If this cluster is closer than the last, assign and color code to this cluster
			if(distance < lastDistance)
			{
				lastDistance = distance;
				it->set_r(cIt->get_r());
				it->set_b(cIt->get_b());
				it->set_g(cIt->get_g());
				it->set_clusterIndex(cIt - vClusters.begin());
			}
		} // end FOR each cluster 
	} // end FOR each data point
}

// Update each cluster's position by computing the centroid of all data points associated with the cluster
void CGDIWindow::compute_centroids()
{
	// For each cluster...
	for(vector<CDataPoint>::iterator cIt = vClusters.begin(); cIt != vClusters.end(); ++cIt)
	{
		int xAccum = 0; // x position accumulator
		int yAccum = 0; // y position accumulator
		int dpCount = 0; // how many data points 
		int currentClusterIndex = cIt - vClusters.begin();

		// For each data point...
		for(vector<CDataPoint>::iterator it = vPoints.begin(); it != vPoints.end(); ++it)
		{
			if(it->get_clusterIndex() == currentClusterIndex)
			{
				xAccum += it->get_x();
				yAccum += it->get_y();
				dpCount++;
			}

		} // end FOR each data point

		// If there are no data points in this cluster, something went wrong, 
		// so move the cluster center to the center of the x and y range
		if(!dpCount)
		{
			cIt->set_x(CDP_X_UPPER_BOUND/2);
			cIt->set_y(CDP_Y_UPPER_BOUND/2);
		}
		else
		{
			// Once through all data points, compute the centroid as the mean of x and y
			float xMean = (float) xAccum / (float) dpCount;
			if((xMean - (int)xMean) > 0.5f)
				xMean++;

			float yMean = (float) yAccum / (float) dpCount;
			if((yMean - (int)yMean) > 0.5f)
				yMean++;

			cIt->set_x((const int)xMean);
			cIt->set_y((const int)yMean);
		}
	} // end for each cluster
}

// Without touching the data sets, or the colors of the clusters, randomize
// the positions of the clusters
void CGDIWindow::randomize_cluster_positions()
{
	for(vector<CDataPoint>::iterator cIt = vClusters.begin(); cIt != vClusters.end(); ++cIt)
	{
		cIt->set_x(rand()%CDP_X_UPPER_BOUND);
		cIt->set_y(rand()%CDP_Y_UPPER_BOUND);
	} // end FOR each cluster
}

// Handle a key passed from the WM_KEYDOWN message handler
void CGDIWindow::handle_key(const char key)
{
	switch(key)
	{
	case 0x52: // r
		randomize_cluster_positions();
		//InvalidateRect(hWnd, NULL, NULL);
		break;
	case 0x43: // c
		compute_centroids();
		//InvalidateRect(hWnd, NULL, NULL);
		break;
	case 0x41: // a
		assign_data();
		//InvalidateRect(hWnd, NULL, NULL);
		break;
	case 0x49: // i
		initialize_data();
		//InvalidateRect(hWnd, NULL, NULL);
		break;
	case 0x20: // space bar
		InvalidateRect(hWnd, NULL, NULL);
		break;
	default:
		break;
	}
}