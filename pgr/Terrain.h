#include "pgr.h"
#include "HeightMap.h"
#include "OglRenderer.h"
#include "OglTex.h"
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "parametry.h"

#ifndef attrMap
#define attrMap map<string,GLint>
#endif

/**
one terrain consisting of more heightmaps
*/
class Terrain {
public:
	HeightMap* init( uint n, uint size );
	void toBuffs( Bufferism *buff );
	void render( attrMap attr, glm::vec3 pos, float viewDist, OglRenderer *renderer ); //! rendering terrain with right mirroring
private:
	vector<HeightMap> hmaps,hmapsInv;
	uint size,n;
	OglTex texRock,texGrass;
};