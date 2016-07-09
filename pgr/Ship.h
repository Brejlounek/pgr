#ifndef SHIP_LKHI757RZDTZ5576RFUFCJLKI76576RC
#define SHIP_LKHI757RZDTZ5576RFUFCJLKI76576RC

#include "OglModel.h"
#include "HeightMap.h"
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <random>
#include "parametry.h"

class CamShip : public OglCam {
public:
	void setMoveUp( bool b );
};

/**
plane
*/
class Ship : public OglModel {
public:
	void init(OglRenderer* renderer_);
	OglCam* getCam();

	glm::mat4 myTransform( glm::mat4 &rot );

	void doInput(MouseInfo &mi, Keyboard* keyb, double delta_, frameinfo frinf);
	void move(KeyCode code);
	bool autodrive;
	HeightMap *hmap;
private:
	float delta,sens,speed,roll,vSpeed;
	CamShip cam;

	glm::mat4 rotationMat;
	glm::vec4 upV, rightV;
	glm::vec3 behindShip, up;
	glm::vec3 shRot, shBehind,vDir;

	float P;
	vector<glm::vec3> point;
	void MoveFromTerrain();
	float getHmap( glm::vec2 pos2 );

	glm::vec3 intp( glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, float p );
	glm::vec3 intpd( glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, float p );
};

#endif