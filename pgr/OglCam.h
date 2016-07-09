#ifndef OGLCAM_3FG5H564DF654H564DF564HG54EFH565DF456H4
#define OGLCAM_3FG5H564DF654H564DF564HG54EFH565DF456H4

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
//#include <GL/glew.h>
#include "pgr.h"

#include <gl/gl.h>
#include <gl/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "OglOIS.h"
#include "frameinfo.h"

using namespace std;
using namespace OIS;

#ifndef PI
#define PI 3.14159265359f
#define DEGTORAD 0.01745329252f
#define RADTODEG 57.295779513f
#endif

struct CamID {
	CamID( GLint id_pos ) :id_pos( id_pos ) {}
	GLint id_pos;
};

struct CamData {
	glm::mat4 invMatrix;
	glm::vec3 pos;
	glm::vec2 zClip;
	float zClipRatio;
};

/**
basic camera
*/
class OglCam {
public:
	OglCam( );
	virtual glm::mat4 update(frameinfo frinf);

	//! position, rotation etc., I din't do it with glm at the time...
	float x, y, z;
	float xt, yt, zt;
	float rotx, roty;
	float fov, nearz, farz, roll;
	float wi, he;
	glm::mat4 matrix, invMatrix, prevMatrix, noTranslMatrix; //! computed matrices
	glm::vec3 look, up_preroll, right, up;
	bool moveUp;

	static void lookat( float &xo, float &yo, float &zo, float rotxi, float rotyi );
	virtual void lookat( ); //! looks at xt,yt,zt
	virtual glm::vec3 getGeneric( ); //! gets some camera-random vector
	virtual void init( const char* name );
	virtual CamData getData( ); //! describes camera position
protected:
	const float sens = 200; //! mouse sensitivity
	const float speed = 150.0f; //! camera speed
	vector<CamID> IDs;
	string namestr;
};

/**
ortographic camera
*/
class OglCamOrtho : public OglCam {
public:
	virtual glm::mat4 update( frameinfo frinf );
};

/**
free camera (WASDRF + mouse)
*/
class CamWASDRF : public OglCam {
public:
	CamWASDRF( );
	void doinput( MouseInfo &mi, Keyboard *keyb, double delta ); //! manages input from ois
protected:
	void move( KeyCode code, bool shift );
	bool lpressed;
	double delta;
};

#endif