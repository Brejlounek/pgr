#include "OglMesh.h"
#include <map>
#include <vector>
using namespace std;

void OglMesh::toBuffs( Bufferism *buff )
{
	buf = buff;
	bufid = buf->newMesh(inds.size());
	buf->add( verts );
	buf->add( inds );
}

void OglMesh::color( glm::vec3 color )
{
	for ( uint i = 0; i < verts.size(); i += stepSize )
	{
		verts[i + 3] = color.x;
		verts[i + 4] = color.y;
		verts[i + 5] = color.z;
	}
}

void OglMesh::transform( glm::mat4 matrix )
{
	for ( uint i = 0; i < verts.size(); i += stepSize )
	{
		glm::vec4 pos = glm::vec4( verts[i], verts[i + 1], verts[i + 2], 1 );
		pos = matrix * pos;
		verts[i] = pos.x;
		verts[i + 1] = pos.y;
		verts[i + 2] = pos.z;
	}
}

void OglMesh::rotateNormals( glm::mat4 matrix )
{
	uint startid = 3;
	if ( fl & BUF_COLOR )
		startid += 3;

	for ( uint i = 0; i < verts.size( ); i += stepSize )
	{
		glm::vec4 pos = glm::vec4( verts[i + startid], verts[i + 1 + startid], verts[i + 2 + startid], 1 );
		pos = matrix * pos;
		verts[i + startid] = pos.x;
		verts[i + 1 + startid] = pos.y;
		verts[i + 2 + startid] = pos.z;
	}
}

void OglMesh::flip()
{
	for ( uint i = 0; i < inds.size(); i += 3 )
		swap( inds[i], inds[i + 1] );
	/*for ( uint i = 0; i < verts.size(); i += stepSize )
	{
		verts[i + 6] *= -1;
		verts[i + 7] *= -1;
		verts[i + 8] *= -1;
	}*/
}

void OglMesh::calcNormals()
{

	//REDO 
	uint startid = 3;
	if ( fl & BUF_COLOR )
		startid+=3;

	vector<glm::vec3> normal( verts.size( )/stepSize );
	for ( uint i = 0; i < verts.size(); i += stepSize )
		normal[i / stepSize] = glm::vec3( 0, 0, 0 );

	vector<glm::vec3> triverts( 3 );
	if ( inds.size() > 0 )
	for ( uint i = 0; i < inds.size(); i+=3 )
	{
		for ( uint q = 0; q < 3; q++ )
		{
			glm::vec3 pos;
			pos.x = verts[inds[i + q] * stepSize + 0];
			pos.y = verts[inds[i + q] * stepSize + 1];
			pos.z = verts[inds[i + q] * stepSize + 2];
			triverts[q] = pos;
		}

		glm::vec3 cross = glm::cross( triverts[0] - triverts[1], triverts[1] - triverts[2] );
		cross = glm::normalize( cross );
		if (cross != cross)
			int u = 0;
		normal[inds[i + 0]] += cross;
		normal[inds[i + 1]] += cross;
		normal[inds[i + 2]] += cross;
	}

	for ( uint i = 0; i < normal.size(); i++ )
	{
		if ( normal[i] == glm::vec3( 0, 0, 0 ) )
			continue;
		normal[i] = glm::normalize( normal[i] );
	}
	for ( uint i = 0; i < normal.size(); i++ )
	{
		verts[i*stepSize + startid + 0] = normal[i].x;
		verts[i*stepSize + startid + 1] = normal[i].y;
		verts[i*stepSize + startid + 2] = normal[i].z;
	}
}

void OglMesh::init( uint Index, const aiMesh* paiMesh, bool flipz )
{
	uint flags = 0;
	if ( paiMesh->HasPositions( ) )
		flags |= BUF_POS;
	if ( paiMesh->HasNormals( ) )
		flags |= BUF_NORMAL;
	if ( paiMesh->HasTextureCoords( 0 ) )
		flags |= BUF_UV;
	flags = 15; //TODO
	
	fl = flags;
	stepSize = 0;
	if ( flags & BUF_POS )
		stepSize += 3;
	if ( flags & BUF_COLOR )
		stepSize += 3;
	if ( flags & BUF_NORMAL )
		stepSize += 3;
	if ( flags & BUF_UV )
		stepSize += 2;

	for ( uint i = 0; i < paiMesh->mNumVertices; i++ )
	{
		if ( flags & BUF_POS )
		{
			verts.push_back( paiMesh->mVertices[i].x );
			if (flipz)
			{
				verts.push_back(paiMesh->mVertices[i].z);
				verts.push_back(paiMesh->mVertices[i].y);
			}
			else
			{
				verts.push_back(paiMesh->mVertices[i].y);
				verts.push_back(paiMesh->mVertices[i].z);
			}
		}
		if ( flags & BUF_COLOR ) //TODO
		{
			verts.push_back( 1 );
			verts.push_back( 1 );
			verts.push_back( 1 );
		}
		if ( flags & BUF_NORMAL )
		{
			verts.push_back( paiMesh->mNormals[i].x );
			if (flipz)
			{
				verts.push_back(paiMesh->mNormals[i].z);
				verts.push_back(paiMesh->mNormals[i].y);
			}
			else
			{
				verts.push_back(paiMesh->mNormals[i].y);
				verts.push_back(paiMesh->mNormals[i].z);
			}
		}
		if ( flags & BUF_UV )
		{
			if ( paiMesh->HasTextureCoords( 0 ) )
			{
				verts.push_back( paiMesh->mTextureCoords[0][i].x );
				verts.push_back( paiMesh->mTextureCoords[0][i].y );
			}
			else
			{
				verts.push_back( 0 );
				verts.push_back( 0 );
			}
		}
	}

	for ( unsigned int i = 0; i < paiMesh->mNumFaces; i++ ) {
		const aiFace& face = paiMesh->mFaces[i];
		assert( face.mNumIndices == 3 );
		if (flipz)
		{
			inds.push_back(face.mIndices[1]);
			inds.push_back(face.mIndices[0]);
			inds.push_back(face.mIndices[2]);
		}
		else
		{
			inds.push_back(face.mIndices[0]);
			inds.push_back(face.mIndices[1]);
			inds.push_back(face.mIndices[2]);
		}
	}

	if ( flags & BUF_NORMAL )
		calcNormals( );
}

void OglMesh::render(map<string,GLint> attrs)
{
	buf->render(attrs, bufid);
}

OglMesh& OglMesh::operator+(OglMesh &mesh)
{
	uint maxvert = verts.size() / stepSize;
	for (uint i = 0; i < mesh.verts.size(); i++)
		verts.push_back(mesh.verts[i]);
	for (uint i = 0; i < mesh.inds.size(); i++)
		inds.push_back(maxvert+mesh.inds[i]);
	return *this;
}

void OglMesh::addVertex(uint flags, glm::vec3 pos, glm::vec3 color, glm::vec3 normal, glm::vec2 uv)
{
	if (flags & BUF_POS)
	{
		verts.push_back(pos.x);
		verts.push_back(pos.y);
		verts.push_back(pos.z);
	}
	if (flags & BUF_COLOR)
	{
		verts.push_back(color.x);
		verts.push_back(color.y);
		verts.push_back(color.z);
	}
	if (flags & BUF_NORMAL)
	{
		verts.push_back(normal.x);
		verts.push_back(normal.y);
		verts.push_back(normal.z);
	}
	if (flags & BUF_UV)
	{
		verts.push_back(uv.x);
		verts.push_back(uv.y);
	}
}
void OglMesh::addIndex(uint v1, uint v2, uint v3)
{
	inds.push_back(v1);
	inds.push_back(v2);
	inds.push_back(v3);
}



void Bufferism::init( uint flags )
{
	if ( flags & BUF_POS )
		vbuf.addType( GL_FLOAT, 3, "position" );
	if ( flags & BUF_COLOR )
		vbuf.addType( GL_FLOAT, 3, "vcolor" );
	if ( flags & BUF_NORMAL )
		vbuf.addType( GL_FLOAT, 3, "normal" );
	if ( flags & BUF_UV )
		vbuf.addType( GL_FLOAT, 2, "uv" );

	ibuf.addType( 0, 1, "index" );
}

void Bufferism::postInit( )
{
	vbuf.finalize( );
	vbuf.bind( );
	ibuf.finalize( );
	ibuf.bind( );
}

uint Bufferism::newMesh(uint size)
{
	BuffPart part;
	part.size = size;
	part.start = ibuf.getSize();
	parts.push_back(part);
	return parts.size() - 1;
}

void Bufferism::render( map<string, GLint> attrs )
{
	vbuf.enable(attrs);
	ibuf.enable();
	ibuf.render();
	vbuf.disable();
}
void Bufferism::render(map<string, GLint> attrs, uint i)
{
	vbuf.enable(attrs);
	ibuf.enable();
	ibuf.render(parts[i].start,parts[i].size);
	vbuf.disable();
}




void Bufferism::add( float v )
{
	vbuf.add( v );
}
void Bufferism::add( float* v, uint size )
{
	vbuf.add( v, size );
}
void Bufferism::add( vector<float> v )
{
	vbuf.add( v );
}

void Bufferism::add( uint v )
{
	ibuf.add( v );
}
void Bufferism::add( uint* v, uint size )
{
	ibuf.add( v, size );
}
void Bufferism::add( vector<uint> v )
{
	ibuf.add( v );
}