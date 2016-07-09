#ifndef HEIGHTMAP_SD54G65SD456G464SD65F4G564
#define HEIGHTMAP_SD54G65SD456G464SD65F4G564

#include "Noise2D.h"
#include "Oglmesh.h"

/**
one block of heightmap
*/
class HeightMap : public OglMesh {
public:
	void init( float X, float Y, float width, float height, uint size_, uint layersOffs, uint flags, uint seed = -1 );
	void initNoises( uint layers_, float layerMax_, float layerCoef_, float X, float Y, uint layersOffs ); //! generates noises for quick lookups
	float sampleNoises( glm::vec2 pos, uint layersOffs ); //! samples one heigt using multiple lookups for layers
	uint getSize() { return size; }
private:
	vector<Noise2D> noises;
	uint size, layers;
	float layerMax, layerCoef, smooth;
	float X, Y;
};

#endif