#include "Terrain.h"

void Terrain::toBuffs( Bufferism *buff )
{
	for ( uint i = 0; i < hmaps.size( ); i++ )
	{
		hmaps[i].toBuffs( buff );
		hmapsInv[i].toBuffs( buff );
	}
}

void Terrain::render( attrMap attr, glm::vec3 pos, float viewDist, OglRenderer *renderer )
{
	int minx = floor( (pos.x - viewDist) / sizeBl );
	int miny = floor( (pos.z - viewDist) / sizeBl );
	int maxx = ceil( (pos.x + viewDist) / sizeBl );
	int maxy = ceil( (pos.z + viewDist) / sizeBl );

	//renderer->activeShader->importValue( "viewDist", viewDist );
	renderer->activeShader->importValue( "texGrass", &texGrass );
	renderer->activeShader->importValue( "texRock", &texRock );

	for ( int y = miny; y <= maxy; y++ )
	for ( int x = minx; x <= maxx; x++ )
	{
		int xn = x; int yn = y;
		int xSc = 1, ySc = 1;
		while ( xn > (int) (n - 1) )
		{
			xn -= n;
			xSc *= -1;
		}
		while ( xn < 0 )
		{
			xn += n;
			xSc *= -1;
		}
		while ( yn >( int )(n - 1) )
		{
			yn -= n;
			ySc *= -1;
		}
		while ( yn < 0 )
		{
			yn += n;
			ySc *= -1;
		}

		float over = 1.1;
		glm::mat4 tr = glm::translate( glm::vec3( (sizeBl - sizeBl / size)*x, 0, (sizeBl - sizeBl / size)*y ) );
		tr = tr * glm::scale( glm::vec3( xSc, 1, ySc ) );
		float trX = (sizeBl - sizeBl / size)*x;
		float trY = (sizeBl - sizeBl / size)*y;

		if ( xSc == -1 )
		{
			tr = glm::translate( glm::vec3( (sizeBl + sizeBl / size), 0, 0 ) ) * tr;
			trX += (sizeBl + sizeBl / size* over);
		}
		if ( ySc == -1 )
		{
			tr = glm::translate( glm::vec3( 0, 0, (sizeBl + sizeBl / size) ) ) * tr;
			trY += (sizeBl + sizeBl / size* over);
		}

		if ( xSc == -1 )
			xn = n - xn - 1;
		if ( ySc == -1 )
			yn = n - yn - 1;

		renderer->setObj( tr );
		renderer->transformUpdate( );
		renderer->activeShader->importValue( "normalS", glm::vec3( xSc, 1, ySc ) );
		renderer->activeShader->importValue( "patchCoord", glm::vec3(trX, 0, trY) );

		if ( xSc*ySc == 1 )
			hmaps[yn*n + xn].render( attr );
		else
			hmapsInv[yn*n + xn].render( attr );
	}
}

HeightMap* Terrain::init( uint n, uint size )
{
	this->size = size;
	this->n = n;
	for ( uint y = 0; y < n; y++ )
	for ( uint x = 0; x < n; x++ )
	{
		HeightMap hmap,hmapInv;
		hmap.init( size*x, size*y, sizeBl, terrHeight, size, 2, BUF_POS | BUF_NORMAL | BUF_UV | BUF_COLOR );
		hmapInv = hmap;
		hmapInv.flip();
		//hmap.transform( glm::translate( glm::vec3( (100 - 100.0f / size)*x, 0, (100 - 100.0f / size)*y ) ) );
		hmaps.push_back( hmap );
		hmapsInv.push_back( hmapInv );
	}

	texGrass.init( "gfx/grass.png" );
	texRock.init( "gfx/rock.png" );
	return &hmaps[0];
}