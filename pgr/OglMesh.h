#ifndef OGLMESH_DFG654R6D54G654DF564GH
#define OGLMESH_DFG654R6D54G654DF564GH

#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "pgr.h"
using namespace std;

#include "OglBuff.h"

//#include <glfw3.h>

#define uint unsigned int

#define BUF_POS (1<<0)
#define BUF_COLOR (1<<1)
#define BUF_NORMAL (1<<2)
#define BUF_UV (1<<3)

struct BuffPart {
	uint start;
	uint size;
};

/**
uniting vertex and index buffer for future mesh
*/
class Bufferism {
public:
	void init( uint flags );
	void postInit( ); //! finalizes and binds buffers
	uint newMesh(uint size);

	void render( map<string, GLint> attrs ); //! renders everything
	void render(map<string, GLint> attrs, uint i); //! renders one part

	//! adding vertices
	void add( float v );
	void add( float* v, uint size );
	void add( vector<float> v);

	//! adding indices
	void add( uint v );
	void add( uint* v, uint size );
	void add( vector<uint> v );
private:
	//! vertex and index buffer
	OglBuffVertex vbuf;
	OglBuffIndex ibuf;

	//! has information about parts of buffers
	vector<BuffPart> parts;
};

/**
one mesh, can be added to Bufferism
*/
class OglMesh {
public:
	void toBuffs(Bufferism *buff); //! put to Bufferism
	void render(map<string, GLint> attrs); //! renders from Bufferism

	void addVertex(uint flags, glm::vec3 pos, glm::vec3 color, glm::vec3 normal, glm::vec2 uv);
	void addIndex(uint v1, uint v2, uint v3);

	void calcNormals(); //! recalculate all smooth normals
	void flip(); //! flips Y and Z for compatibility
	void color( glm::vec3 color ); //! colors vertices|
	void transform( glm::mat4 matrix ); //! transforms vertices|
	void rotateNormals( glm::mat4 matrix );
	void init( uint Index, const aiMesh* paiMesh, bool flipz );
	OglMesh& operator+(OglMesh &mesh); //! unites with another mesh
protected:
	vector<float> verts;
	vector<unsigned int> inds;
	uint stepSize,fl,bufid;
	Bufferism *buf;
};

#endif