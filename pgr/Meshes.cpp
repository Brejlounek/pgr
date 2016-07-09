#include "Meshes.h"

/////////////////////////////////////////////////
////////////////// cube

float cubeVerts[] = {
	-1, 1, 1,
	1, 1, 1,
	1, 1, -1,
	-1, 1, -1,
	-1, -1, 1,
	1, -1, 1,
	1, -1, -1,
	-1, -1, -1,
};

float cubeNormals[] = {
	1, 0, 0,
	-1, 0, 0,
	0, 1, 0,
	0, -1, 0,
	0, 0, 1,
	0, 0, -1,
};

float cubeUV[] = {
	0, 0,
	0, 1,
	1, 1,
	1, 0,
};

uint cubeInds[] = {
	//X
	1, 5, 6,
	1, 6, 2,
	3, 7, 4,
	3, 4, 0,
	//Y
	1, 2, 3,
	1, 3, 0,
	4, 7, 6,
	4, 6, 5,
	//Z
	0, 4, 5,
	0, 5, 1,
	2, 6, 7,
	2, 7, 3,
};

void MeshCube::init( uint flags )
{
	fl = flags;
	for ( int i = 0; i < 8*3; i++ )
	{
		uint si = i % 8;
		uint face = i / 8;
		uint normalface = i / 4;

		glm::vec3 pos(cubeVerts[si * 3 + 0],
			cubeVerts[si * 3 + 1],
			cubeVerts[si * 3 + 2]);
		glm::vec3 color(1, 0, 1);
		glm::vec3 normal(0, 0, 0);
		glm::vec2 uv(0,0);

		addVertex(flags, pos, color, normal, uv);
	}

	for ( int i = 0; i < 12; i++ )
	{
		uint face = i / 4;

		addIndex(cubeInds[i * 3 + 0] + face * 8,
			cubeInds[i * 3 + 1] + face * 8,
			cubeInds[i * 3 + 2] + face * 8);
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

	if ( flags & BUF_NORMAL )
		calcNormals( );
}





/////////////////////////////////////////////////
////////////////// plane

float planeVerts[] = {
	-1, 0, -1,
	1, 0, -1,
	1, 0, 1,
	-1, 0, 1,

	/*-1, 0, -1,
	1, 0, -1,
	1, 0, 1,
	-1, 0, 1,*/
};

float planeUV[] = {
	0, 0,
	1, 0,
	1, 1,
	0, 1,

	/*0, 0,
	1, 0,
	1, 1,
	0, 1,*/
};

uint planeInds[] = {
	0, 2, 1,
	0, 3, 2,
	/*4, 5, 6,
	4, 6, 7,*/
};

void MeshPlane::init( uint flags )
{
	fl = flags;
	for ( int i = 0; i < 4; i++ )
	{
		glm::vec3 pos(planeVerts[i * 3 + 0],
			planeVerts[i * 3 + 1],
			planeVerts[i * 3 + 2]);
		glm::vec3 color(1,0,1);
		glm::vec3 normal(0,0,0);
		glm::vec2 uv(planeUV[i * 2 + 0],
			planeUV[i * 2 + 1]);

		addVertex(flags, pos, color, normal, uv);
	}

	for ( int i = 0; i < 2; i++ )
	{
		addIndex(planeInds[i * 3 + 0],
			planeInds[i * 3 + 1],
			planeInds[i * 3 + 2]);
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
	if ( flags & BUF_NORMAL )
		calcNormals();
}






/////////////////////////////////////////////////
////////////////// fullscreen quad

float fsquadVerts[] = {
	-1, -1, 0,
	1, -1, 0,
	1, 1, 0,
	-1, 1, 0,
};

float fsquadUV[] = {
	0, 0,
	1, 0,
	1, 1,
	0, 1,
};

uint fsquadInds[] = {
	0, 1, 2,
	0, 2, 3,
};

void MeshFullscreenQuad::init( uint flags )
{
	fl = flags;
	if ( flags != (BUF_POS | BUF_UV) )
		cout << "bad fullscreen quad flags" << endl;
	for ( int i = 0; i < 4; i++ )
	{
		if ( flags & BUF_POS )
		{
			verts.push_back( fsquadVerts[i * 3 + 0] );
			verts.push_back( fsquadVerts[i * 3 + 1] );
			verts.push_back( fsquadVerts[i * 3 + 2] );
		}
		if ( flags & BUF_UV )
		{
			verts.push_back( fsquadUV[i * 2 + 0] );
			verts.push_back( fsquadUV[i * 2 + 1] );
		}
	}

	for ( int i = 0; i < 2; i++ )
	{
		inds.push_back( fsquadInds[i * 3 + 0] );
		inds.push_back( fsquadInds[i * 3 + 1] );
		inds.push_back( fsquadInds[i * 3 + 2] );
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
}