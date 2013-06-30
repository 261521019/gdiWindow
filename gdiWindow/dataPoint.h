// dataPoint.h
// Authored by Alex Shows
// Released under the MIT License (http://opensource.org/licenses/mit-license.php) 
// 
// Header declaring the CDataPoint class
// 
// Encapsulates interesting information for a particular observation

#define CDP_X_LOWER_BOUND 0
#define CDP_X_UPPER_BOUND 500
#define CDP_Y_LOWER_BOUND 0
#define CDP_Y_UPPER_BOUND 500
#define CDP_COLOR_LOWER_BOUND 0
#define CDP_COLOR_UPPER_BOUND 255

class CDataPoint
{
public:
	CDataPoint();
	CDataPoint(const int x, const int y);
	CDataPoint(const int x, const int y, const int size);
	CDataPoint(const int x, const int y, const int size, const int r, const int g, const int b);
	~CDataPoint();
	int get_clusterIndex(){ return clusterIndex;};
	int get_x(){ return x;};
	int get_y(){ return y;};
	int get_x_bounds(){ return CDP_X_UPPER_BOUND;};
	int get_y_bounds(){ return CDP_Y_UPPER_BOUND;};
	int get_size(){ return size;};
	int get_r(){ return r;};
	int get_g(){ return g;};
	int get_b(){ return b;};
	int set_clusterIndex(const int idx = 0);
	int set_x(const int xVal = 0);
	int set_y(const int yVal = 0);
	int set_size(const int sizeVal = 0);
	int set_r(const int rVal = 0);
	int set_g(const int gVal = 0);
	int set_b(const int bVal = 0);
private:
	int clusterIndex; // Cluster to which the data point is currently assigned
						// TODO: Consider using a pointer to the parent instead, perhaps children as well?
	int x; // Offset in pixels from left edge
	int y; // Offset in pixels from top edge
	int size; // Size in pixels
	int r; // red color component
	int g; // green color component
	int b; // blue color component
};