#include "HeightMap.h"

void HeightMap::initNoises( uint layers_, float layerMax_, float layerCoef_, float X, float Y, uint layersOffs )
{
	layers = layers_;
	layerMax = layerMax_;
	layerCoef = layerCoef_;
	float magn = layerMax;
	for ( uint i = 0; i < layers; i++ )
	{
		Noise2D noise;
		uint noiseSize = ceil( size / magn );

		float coordCoef = 1.0f*size / noiseSize;
		if ( i>layersOffs-1 ) //ugly hack
		{
			noise.init( X / magn, Y / magn, i, noiseSize );
			noises.push_back( noise );
		}
		magn /= layerCoef;
	}
}

float HeightMap::sampleNoises( glm::vec2 pos, uint layersOffs )
{
	float sum = 0,coef=1.0f;
	float magn = layerMax;
	for ( uint i = 0; i < layers; i++ )
	{
		//if (i==1)
		glm::vec3 normal;
		if ( i>layersOffs - 1 ) //ugly hack
			sum += noises[i - layersOffs].sample( pos / magn )*coef;
		magn /= layerCoef;
		coef /= layerCoef*smooth;
	}
	return sum;
}

void HeightMap::init( float X, float Y, float width, float height, uint size_, uint layersOffs, uint flags, uint seed )
{
	fl = flags;
	size = size_;
	float oneWidth = width / size;

	layers = 0; uint size2 = size;
	while ( size2 != 1 )
	{
		layers++;
		size2 /= 2;
	}
	layerCoef = 2;
	layerMax = size;
	smooth = 1.2;

	initNoises( layers, layerMax, layerCoef, X, Y, layersOffs );
	vector<glm::vec3> zs( (size+2)*(size+2) );
	for ( uint y = 0; y < size+2; y++ )
	for ( uint x = 0; x < size+2; x++ )
	{
		//float z = noise.sample( glm::vec2( x/magn, y/magn ) );
		glm::vec3 normal = glm::vec3(0,1,0);

		float xx = (size + 2)*(1.0*x / ((size + 2) - 1));
		float yy = (size + 2)*(1.0*y / ((size + 2) - 1));

		float z = sampleNoises( glm::vec2(xx,yy), layersOffs );
		zs[y*(size + 2) + x] = glm::vec3( x*oneWidth, z*height, y*oneWidth );

		/*glm::vec3 pos( x*oneWidth, z*height, y*oneWidth );
		addVertex( flags, pos, glm::vec3(0.5,0.5,0.5), normal, glm::vec2(0) );*/
	}
	//cout << endl;

	for ( uint y = 1; y < size+1; y++ )
	for ( uint x = 1; x < size+1; x++ )
	{
		uint index = y*(size+2) + x;
		glm::vec3 cross00 = -glm::cross( zs[index - 1] - zs[index], zs[index - size-2] - zs[index] );
		glm::vec3 cross10 = glm::cross( zs[index + 1] - zs[index], zs[index - size-2] - zs[index] );
		glm::vec3 cross01 = glm::cross( zs[index - 1] - zs[index], zs[index + size+2] - zs[index] );
		glm::vec3 cross11 = -glm::cross( zs[index + 1] - zs[index], zs[index + size+2] - zs[index] );
		glm::vec3 normal = glm::normalize( cross00 + cross10 + cross01 + cross11 );

		//fix for seams
		if ( x == 1 || x == size )
			normal.x = 0;
		if ( y == 1 || y == size )
			normal.z = 0;

		addVertex( flags, glm::vec3( zs[index].x, zs[index].y, zs[index].z), glm::vec3( 1, 1, 1 ), normal, glm::vec2( 0 ) );
	}

	for ( uint y = 0; y < size - 1; y++ )
	for ( uint x = 0; x < size - 1; x++ )
	{
		uint baseIndex = y*size + x;
		addIndex( baseIndex + 1, baseIndex, baseIndex + size + 1 );
		addIndex( baseIndex + size + 1, baseIndex, baseIndex + size );
	}

	stepSize = 0;
	if ( flags & BUF_POS )
		stepSize += 3;
	if ( flags & BUF_COLOR )
		stepSize += 3;
	if ( flags & BUF_NORMAL )
		stepSize += 3;
	if ( flags & BUF_UV )
		stepSize += 2;
	/*if ( flags & BUF_NORMAL )
		calcNormals( );*/
}