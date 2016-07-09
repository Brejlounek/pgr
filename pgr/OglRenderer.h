#ifndef RENDERER_KFJSHKJFHHKJFHKGHFKJHJHJ
#define RENDERER_KFJSHKJFHHKJFHKGHFKJHJHJ

#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>

#include "OglShader.h"
#include "OglLight.h"
#include "OglCam.h"
#include "OglTex.h"
#include "OglMesh.h"
#include "OglFrameBuffer.h"

static bool finish = true;

#define uint unsigned int
#define uchar unsigned char

/**
big class managing rendering resources with different rendering methods
*/
class OglRenderer {
public:
	void init(uint wi,uint he);
	void createRandomTexture(); //! creates generic noise texture

	void setCamera( glm::mat4 camMat, bool wiggle = true ); //! sets camera trasform into the shaders
	void setCameraPrev( glm::mat4 camMat, bool wiggle = true );
	void setCameraNoTransl( glm::mat4 camMat, bool wiggle = true );
	void setObj( glm::mat4 objMat_ ); //! sets other trasforms into the shaders
	void setRot( glm::mat4 rotMat_ );
	void addObj( glm::mat4 objMat_ );
	void transformUpdate();
	void resetTransform();

	//void setMaterial(OglMaterial* mat);

	void buildShader( string strProg, string strVertex, string strFragment );
	void enableBuffer( OglFrameBuffer *buffer, bool clear = true );

	//! custom rendering mthods
	void AmbientDir( glm::vec3 ambient, LightData dirL, uint wi, uint he, string shaderName );
	void Skybox( uint wi, uint he, LightData dirL, string shaderName );
	void Fire( uint wi, uint he, string shaderName );
	void Wasser( glm::vec3 ambient, uint wi, uint he, LightData dirL, string shaderName );

	//! custom rendering mthods start/end
	map<string, GLint> preRenderDir( OglCam *cam, glm::vec3 ambient, OglLightDir *dirLight, float viewDist, frameinfo frinf, string shaderName );
	map<string, GLint> preRenderSkybox( frameinfo frinf, OglLightDir *dirLight, string shaderName );
	map<string, GLint> preRenderFire( OglCam *cam, frameinfo frinf, string shaderName );
	map<string, GLint> preRenderWasser( OglCam *cam, glm::vec3 ambient, OglLightDir *dirLight, float viewDist, frameinfo frinf, string shaderName );
	void postRenderFire();
	void postRender();

	glm::mat4 camMat;
	OglShaderProg *activeShader;

private:
	Bufferism bufQuad;
	glm::mat4  objMat, camMatPrev, camMatNoTransl;

	unordered_map<string, OglVertexShader> shVertex;
	unordered_map<string, OglFragmentShader> shFragment;
	unordered_map<string, OglShaderProg> shProg;

	bool transformReady,wasRendering;
	OglTex randomTex, fireTex, wasserTex;
};

#endif