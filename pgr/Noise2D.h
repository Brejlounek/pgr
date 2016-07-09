#ifndef NOISE2D_53DF4H54TH45FG54J54FG564JH
#define NOISE2D_53DF4H54TH45FG54J54FG564JH

#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <math.h>
#include <glm/glm.hpp>
using namespace std;

#define uint unsigned int

#define GRID(x,y) grid[y*size+x]
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

#ifndef PI
#define PI 3.14159265359f
#define DEGTORAD 0.01745329252f
#endif

class Noise2DLookup {
public:
	float get(uint x,uint y,float sizeBl);
private:
	map<pair<uint, uint>, vector<float>> val;
};

static Noise2DLookup lookup;

/**
2D noise for terrain lookup
*/
class Noise2D {
public:
	void init( uint x, uint y, uint lod, uint size_ ); //! inits noise with the right lod
	float sample( glm::vec2 pos ); //! samples noise position
private:
	float size;
	vector<float> grid;
	uint startX,startY;
};

#endif