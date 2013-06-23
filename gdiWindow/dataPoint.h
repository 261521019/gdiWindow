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
	~CDataPoint();
	int get_x(){ return x;};
	int get_y(){ return y;};
	int get_size(){ return size;};
	int get_r(){ return r;};
	int get_g(){ return g;};
	int get_b(){ return b;};
	int set_x(int xVal = 0);
	int set_y(int yVal = 0);
	int set_size(int sizeVal = 0);
	int set_r(int rVal = 0);
	int set_g(int gVal = 0);
	int set_b(int bVal = 0);
private:
	int x;
	int y;
	int size;
	int r;
	int g;
	int b;
};