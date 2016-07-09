#ifndef LIGHT_FD654GH564FDH54DF4
#define LIGHT_FD654GH564FDH54DF4

#include <glm/glm.hpp>
#include "OglCam.h"
#include "OglOIS.h"
//#include <glfw3.h>
#include "pgr.h"

struct LightData {
	glm::vec3 pos, color;
	glm::mat4 MVP, MVPprev, invMVP;
	bool shadow;
	LightData( glm::vec3 pos, glm::vec3 color, glm::mat4 MVP,
		glm::mat4 MVPprev, glm::mat4 invMVP ) :
		pos( pos ), color( color ), MVP( MVP ),
		MVPprev(MVPprev), invMVP( invMVP ) {}
};

/**
basic light
*/
class OglLight {
public:
	glm::vec3 pos;
	glm::vec3 color;
};

/**
directional light
*/
class OglLightDir : public OglLight {
public:
	OglLightDir();
	glm::vec3 input( MouseInfo &mi, frameinfo frinf ); //! manipulating light trough ois
	LightData getData();

	bool shadow;
private:
	void lookat();
	float rotx, roty;
	OglCamOrtho cam; glm::mat4 MVP; //! camera for shadowmaps

	static const int sens = 200;
};

#endif