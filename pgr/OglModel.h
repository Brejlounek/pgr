#ifndef OGLMODEL_987TZRO76RF6REI76RTF64R
#define OGLMODEL_987TZRO76RF6REI76RTF64R

#include "pgr.h"
#include <vector>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include "OglMesh.h"
#include "OglRenderer.h"
using namespace Assimp;

#define uint unsigned int
#define FLIPZ 1<<0
#define CALCNORMALS 1<<1

struct MeshMat {
	MeshMat( OglMesh mesh, uint matId ):mesh(mesh),matId(matId) {}
	OglMesh mesh;
	uint matId;
};

class OglMaterial {
public:
	OglMaterial(glm::vec3 color) :color(color) { hasTexture = false; }
	OglMaterial(glm::vec3 color, string texString);
	//OglMaterial() { color = glm::vec3(1, 1, 1); hasTexture = false; }
//private:
	glm::vec3 color;
	OglTex texture;
	bool hasTexture;
};

/**
one model has more meshes and can be transformed
*/
class OglModel {
public:
	OglModel();

	void fromFile(string fname, OglRenderer* renderer_, uint flags); //! loads meshes from file
	void toBuffs( Bufferism &buf ); //! puts all models to a Bufferism
	void render(map<string, GLint> attrs);
	void skyboxRender(map<string, GLint> attrs); //! hack for material-less skybox with a texture
	virtual glm::mat4 myTransform( glm::mat4 &rot ); //! method is overwritten by custom models
	glm::mat4 alignObject( glm::vec3& position, const glm::vec3& front, const glm::vec3& up ); //! nice method from pgr :)

	glm::vec3 pos, rot, scale;
	OglRenderer* renderer; //! pointer to renderer to set up activeShader
protected:
	vector<MeshMat> meshes;
	//vector<OglMaterial> materials;
	vector<OglMaterial> materials;
};

#endif