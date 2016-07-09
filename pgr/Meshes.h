#ifndef MESHES_DF354FH554DF565H654FH
#define MESHES_DF354FH554DF565H654FH

#include "OglMesh.h"

/**
a few basic mesh meshes
*/
class MeshFullscreenQuad : public OglMesh {
public:
	void init( uint flags );
};

class MeshPlane : public OglMesh {
public:
	void init( uint flags );
};

class MeshCube : public OglMesh {
public:
	void init( uint flags );
};

#endif