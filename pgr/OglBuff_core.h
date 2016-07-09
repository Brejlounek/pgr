#ifndef OGLBUFF_6FD5H4564FH564DFG654H65F4TH56T456RT4
#define OGLBUFF_6FD5H4564FH564DFG654H65F4TH56T456RT4

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
//#include <GL/glew.h>

//#include <gl/gl.h>
//#include <gl/glu.h>
#include "pgr.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

#define TYPE_P 0
#define TYPE_PC 1

#define uint unsigned int

class MyType {
public:
	MyType(int type, int len, string name) :type(type), len(len), name(name) { position = -1; }
	int type, len, position;
	string name;
};

/**
basic buffer manipulation
*/
template<class SType>
class OglBuff {
public:
	OglBuff( );
	~OglBuff( );
	void bind( );
	void addType( int type, int len, string name ); //! adds another attribute
	void enable( map<string, GLint> attrs );
	void disable( );
	void render( );
	int allocsize( );
	uint getSize();

	void fill( SType *d, int size ); //! fills buffer with ready data
	void add( SType d );
	void add( SType *d, int size );
	void add( vector<SType> d );
	void minus( int cnt ); //! deletes cnt things from buffer

	void finalize( ); //! sets up buffer
	void update( );
	void reset( );
	void updateAll( vector<SType> newdata );

	vector<SType> vertexdata;
protected:
	uint buffer, vao;
	void* data;
	vector<MyType> types;
	int len, sz, cnt, glen, typesz;
	int buftype, usage;
	bool updatable;
};

#endif