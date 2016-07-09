#include "OglModel.h"
#include "Meshes.h"
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <random>
#include "parametry.h"

/**
water plane (I used Wasser everywhere because it sounds cooler)
*/
class Wasser : public OglModel{
public:
	void init( Bufferism *buf );
	glm::mat4 myTransform( glm::mat4 &rot );
	void step( glm::vec2 camPos2, float delta ); //! move water and get camera position
	void render( map<string, GLint> attrs );
private:
	MeshPlane plane;
	glm::vec2 camPos,offDir1,offDir2,off1,off2;
};