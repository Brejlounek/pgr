#include "OglBuff_core.h"

template<class SType>
OglBuff<SType>::OglBuff( )
{
	usage = GL_STATIC_DRAW;
}
template<class SType>
void OglBuff<SType>::addType( int type, int len, string name )
{
	types.push_back( MyType( type, len, name ) );
}
template<class SType>
void OglBuff<SType>::bind( )
{
	int allocsz = vertexdata.capacity( )*typesz;

	glGenBuffers( 1, &buffer );
	glBindBuffer( buftype, buffer );
	
	if ( !updatable )
		glBufferData( buftype, sz, data, usage );
	else
	{
		glBufferData( buftype, allocsz, 0, usage );
		glBufferData( buftype, allocsz, data, usage );
	}

	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
}
template<class SType>
void OglBuff<SType>::enable( map<string, GLint> attrs )
{
	for ( unsigned int i = 0; i < types.size(); i++ )
	{
		types[i].position = attrs[types[i].name];
		glEnableVertexAttribArray( types[i].position );
		//int err = glGetError( );
		//int u = 0;
	}
	glBindVertexArray( vao );
	glBindBuffer(buftype, buffer);

	int nowlen = 0;
	for ( unsigned int i = 0; i<types.size( ); i++ )
	{
		glVertexAttribPointer( types[i].position, types[i].len, types[i].type, GL_FALSE, typesz*glen, (void*) (typesz*nowlen) );
		//int err = glGetError(); 
		//cout << gluErrorString(glGetError());
		nowlen += types[i].len;
	}

	int u = 0;
}
template<class SType>
void OglBuff<SType>::disable( )
{
	for ( unsigned int i = 0; i<types.size( ); i++ )
		glDisableVertexAttribArray( i );
}
template<class SType>
void OglBuff<SType>::render( )
{
	glDrawArrays( GL_TRIANGLES, 0, cnt / len );
}
template<class SType>
OglBuff<SType>::~OglBuff( )
{
	int u = 0;
	//glDeleteBuffers(1, &buffer);
}
template<class SType>
uint OglBuff<SType>::getSize()
{
	return vertexdata.size();
}

///////////////////////////////////////////////

template<class SType>
void OglBuff<SType>::fill( SType* d, int size )
{
	glen = 0;
	for ( unsigned int i = 0; i<types.size( ); i++ )
		glen += types[i].len;

	for ( unsigned int i = 0; i<(unsigned int) size; i++ )
		vertexdata.push_back( d[i] );

	data = &vertexdata[0];
	len = 3;
	cnt = size;
	sz = size*sizeof(float);
	typesz = sizeof(float);
}
template<class SType>
void OglBuff<SType>::add( SType d )
{
	vertexdata.push_back( d );
}
template<class SType>
void OglBuff<SType>::add( SType *d, int size )
{
	for(int i=0;i<size;i++)
		vertexdata.push_back( d[i] );
}
template<class SType>
void OglBuff<SType>::add( vector<SType> d )
{
	add( &d[0], d.size() );
}
template<class SType>
void OglBuff<SType>::minus( int cnt )
{
	for ( int i = 0; i<cnt; i++ )
	if ( vertexdata.size( )>0 )
		vertexdata.erase( vertexdata.begin( ) + vertexdata.size( ) - 1 );
}
template<class SType>
void OglBuff<SType>::finalize( )
{
	glen = 0;
	for ( unsigned int i = 0; i<types.size( ); i++ )
		glen += types[i].len;

	data = &vertexdata[0];
	len = 3;
	cnt = vertexdata.size( );
	sz = vertexdata.size( )*sizeof(float);
	typesz = sizeof(float);
}
template<class SType>
void OglBuff<SType>::updateAll( vector<SType> newdata )
{
	vertexdata = newdata;
}
template<class SType>
void OglBuff<SType>::reset( )
{
	vertexdata.clear( );
}

//////////////////////////////////////

template<class SType>
int OglBuff<SType>::allocsize( )
{
	int alloccnt = 100;
	while ( alloccnt<cnt )
		alloccnt *= 4;
	int typesize = sz / cnt;
	int allocsz = typesize*alloccnt;
	return allocsz;
}

template<class SType>
void OglBuff<SType>::update( )
{
	int allocsz = vertexdata.capacity( )*typesz;

	glBindBuffer( buftype, buffer );
	//glBufferData(buftype, allocsz,0, usage); //hack from net
	glBufferData( buftype, allocsz, data, usage );
}