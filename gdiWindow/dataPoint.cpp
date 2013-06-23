// dataPoint.cpp
// Authored by Alex Shows
// Released under the MIT License (http://opensource.org/licenses/mit-license.php) 
// 
// Implementation of the CDataPoint class
// 
// Encapsulates interesting information for a particular observation

#include "dataPoint.h"

CDataPoint::CDataPoint()
{
	x = y = size = r = g = b = 0;
}

CDataPoint::~CDataPoint()
{
}

// Check bounds and assign the value if param is within bounds
// Returns the value assigned on success, -1 otherwise
int CDataPoint::set_x(int xVal)
{
	if(xVal >= CDP_X_LOWER_BOUND && xVal <= CDP_X_UPPER_BOUND)
		x = xVal;
	else
		return -1;

	return x;
}

// Check bounds and assign the value if param is within bounds
// Returns the value assigned on success, -1 otherwise
int CDataPoint::set_y(int yVal)
{
	if(yVal >= CDP_X_LOWER_BOUND && yVal <= CDP_X_UPPER_BOUND)
		y = yVal;
	else
		return -1;

	return y;
}

// Check bounds and assign the value if param is within bounds
// Returns the value assigned on success, -1 otherwise
int CDataPoint::set_size(int sizeVal)
{
	if(sizeVal > 0)
		size = sizeVal;
	else
		return -1;

	return size;
}

// Check bounds and assign the value if param is within bounds
// Returns the value assigned on success, -1 otherwise
int CDataPoint::set_r(int rVal)
{
	if(rVal >= CDP_COLOR_LOWER_BOUND && rVal <= CDP_COLOR_UPPER_BOUND)
		r = rVal;
	else
		return -1;

	return r;
}

// Check bounds and assign the value if param is within bounds
// Returns the value assigned on success, -1 otherwise
int CDataPoint::set_g(int gVal)
{
	if(gVal >= CDP_COLOR_LOWER_BOUND && gVal <= CDP_COLOR_UPPER_BOUND)
		g = gVal;
	else
		return -1;

	return g;
}

// Check bounds and assign the value if param is within bounds
// Returns the value assigned on success, -1 otherwise
int CDataPoint::set_b(int bVal)
{
	if(bVal >= CDP_COLOR_LOWER_BOUND && bVal <= CDP_COLOR_UPPER_BOUND)
		b = bVal;
	else
		return -1;

	return b;
}
