#ifndef MESHESCITY_F6V98C6C87D78687C6
#define MESHESCITY_F6V98C6C87D78687C6

#include "OglMesh.h"
//#include "OglRenderer.h"
#include <glm/gtx/rotate_vector.hpp>

#include "Meshes.h"
#include "pgr.h"
#include <random>
#include <math.h>
#include <chrono>
#include <iostream>
using namespace glm;
using namespace std;

#ifndef PI
#define PI 3.14159265359f;
#define DEGTORAD 0.01745329252f
#endif

struct Point {
	Point(GLfloat x_, GLfloat y_) : x(x_), y(y_) {}
	GLfloat x, y;
};

class BuildShape {
public: 
	BuildShape( );
	Point operator[]( size_t i );

	vector<Point> points;
	size_t size;
};

/**
meshes for city, not used in final pgr
*/
class MeshBuilding : public OglMesh {
public:
	void init(uint flags, float size=0.0f);
};

#endif