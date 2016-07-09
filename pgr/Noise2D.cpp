#include "Noise2D.h"

void Noise2D::init(uint x, uint y, uint lod, uint size_)
{
	/*if ( seed == -1 )
		seed = chrono::system_clock::now( ).time_since_epoch( ).count( );
	default_random_engine gen( seed );
	uniform_real_distribution<double> dist( 0.0, 1.0 );

	size2 = size_-1;
	size = ceil((size_+2)/4.0)*4;
	grid.clear( );
	grid.resize( size*size );
	for ( uint i = 0; i < size*size; i++ )
	{
		grid[i] = dist( gen ) * 2 - 1;
	}*/
	size = size_ + 8;
	startX = x + 1000 * (lod + 1);
	startY = y;
	grid.clear();
	grid.resize(size*size);
	for (uint y = 0; y < size; y++)
	for (uint x = 0; x < size; x++) //seems ok
	{
		grid[y*size+x] = lookup.get(startX + x, startY + y, size_);
	}
}

float cubicInt(float y0,float y1,float y2,float y3,float mu)
{
	float a0 = -0.5*y0 + 1.5*y1 - 1.5*y2 + 0.5*y3;
	float a1 = y0 - 2.5*y1 + 2 * y2 - 0.5*y3;
	float a2 = -0.5*y0 + 0.5*y2;
	float a3 = y1;

	float mu2 = mu*mu;
	return a0*mu*mu2 + a1*mu2 + a2*mu + a3;
}
glm::vec3 linearInt( glm::vec3 y0, glm::vec3 y1, float mu )
{
	return mu*y1 + (1 - mu)*y0;
}

float Noise2D::sample( glm::vec2 pos)
{
	uint baseIndex = floor( pos.y )*size + floor( pos.x );

	glm::vec2 ratio = glm::vec2( pos.x - floor( pos.x ),
		pos.y - floor( pos.y ) );
	glm::vec2 ratioInv = glm::vec2( 1, 1 ) - ratio;

	float ratioX = pos.x - (int) (pos.x);
	float ratioY = pos.y - (int) (pos.y);
	float ratioXInv = 1 - ratioX;
	float ratioYInv = 1 - ratioY;

	float y0 = cubicInt( grid[baseIndex], grid[baseIndex + 1], grid[baseIndex + 2], grid[baseIndex + 3], ratioX );
	float y1 = cubicInt( grid[baseIndex + size], grid[baseIndex + 1 + size], grid[baseIndex + 2 + size], grid[baseIndex + 3 + size], ratioX );
	float y2 = cubicInt( grid[baseIndex + size * 2], grid[baseIndex + 1 + size * 2], grid[baseIndex + 2 + size * 2], grid[baseIndex + 3 + size * 2], ratioX );
	float y3 = cubicInt( grid[baseIndex + size * 3], grid[baseIndex + 1 + size * 3], grid[baseIndex + 2 + size * 3], grid[baseIndex + 3 + size * 3], ratioX );

	float val = cubicInt(y0,y1,y2,y3,ratioY);
	return val;
}

float Noise2DLookup::get(uint x, uint y, float sizeBl)
{
	pair<uint, uint> coord100 = make_pair(x / 100, y / 100);
	if (val.find(coord100) == val.end())
	{
		uint seed = chrono::system_clock::now().time_since_epoch().count();
		default_random_engine gen(seed);
		uniform_real_distribution<double> dist(0.0, 1.0);

		vector<float> newVals(10000);
		for (uint i = 0; i < 10000; i++)
			newVals[i] = dist(gen)*2-1;
		val[coord100] = newVals;
	}

	//vector<float> vals = val[coord100];
	uint x2 = x - floor(x / 100) * 100;
	uint y2 = y - floor(y / 100) * 100;
	return val[coord100][100 * y2 + x2];
}