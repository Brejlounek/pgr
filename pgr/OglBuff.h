#ifndef OGLBUFFNOTCORE_6FD5H4564FH564DFG654H65F4TH56T456RT4
#define OGLBUFFNOTCORE_6FD5H4564FH564DFG654H65F4TH56T456RT4

#include "OglBuff_core.h"
#include "OglBuff_core.cpp"

#define uint unsigned int

/**
a few special buffer types
*/

class OglBuffVertex : public OglBuff<float> {
public:
	OglBuffVertex( );
	~OglBuffVertex( );

	void add3( glm::vec3 pos ); //! add vertex
	void addv( vector<glm::vec3> data ); //! add vertex
	void updateAll( vector<glm::vec3> posdata );

private:
};

class OglBuffIndex : public OglBuff<unsigned int> {
public:
	OglBuffIndex( );
	void enable( );
	void render( );
	void render(uint start,uint size);

	void add( uint d );
	void add( uint *d, int size );
	void add( vector<uint> d );
private:
	uint maxvert;
};

class OglBuffLine : public OglBuffVertex {
public:
	OglBuffLine( ) {
		buftype = GL_ARRAY_BUFFER;
		usage = GL_DYNAMIC_DRAW;
		updatable = true;
		list = false;
	}
	void render( );
	bool list;
};

#endif