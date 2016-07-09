#include "OglModel.h"
#include "Meshes.h"
#include "Ship.h"
#include <chrono>
#include <random>
#include <math.h>
#include "parametry.h"

/**
fire behind the nozzle
*/
class Fire : public OglModel {
public:
	void init(Bufferism *buf,uint i);
	void step(float delta, Ship* ship); //! moves planes in position
	void get(float &frame);
	glm::vec3 intp(glm::vec3 v1, glm::vec3 v2, float p); //! interpolates positions
	float intp(float v1, float v2, float p);
	void render( map<string, GLint> attrs );

	glm::mat4 myTransform(glm::mat4 &rot);
private:
	float frame, P;
	vector<float> randoms;
	uint n;
	glm::mat4 shipMat;
};