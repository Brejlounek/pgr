#ifndef TEXTURE_DF654GH65D4FH456H4456FG4JH56G4F
#define TEXTURE_DF654GH65D4FH456H4456FG4JH56G4F

//#include <GL/glew.h>
//#include <glfw3.h>
#include "pgr.h"
#include <string>
using namespace std;

#define uint unsigned int

/**
one texture
*/
class OglTex {
public:
	OglTex();
	void init(); //! init a empty texture
	void init( void* data, uint wi, uint he ); //! init texture from data
	void init( string file ); //! load texture from file using pgr framework
	uint getId() { return id; }

	int format, type, intFormat, minFilter, magFilter, compMode;
	bool clampBorder; float color[4];
protected:
	uint id;
};

/**
texture usable for render to texture (not used in pgr)
*/
class OglTexTarget : public OglTex {
public:
	void init(uint wi, uint he);
};

#endif