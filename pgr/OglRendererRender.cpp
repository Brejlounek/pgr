#include "pgr.h"
#include <random>
#include <chrono>
#include <dirent.h>
#include <string>

#include "OglRenderer.h"
#include "Meshes.h"

#define DEBUG

map<string, GLint> OglRenderer::preRenderDir( OglCam *cam, glm::vec3 ambient, OglLightDir *dirLight, float viewDist, frameinfo frinf, string shaderName )
{
	resetTransform();
	AmbientDir( ambient, dirLight->getData( ), frinf.rwidth, frinf.rheight, shaderName );
	setCamera(cam->update(frinf));
	activeShader->importValue("camPos", glm::vec3(cam->x, cam->y, cam->z));
	activeShader->importValue("viewDist", viewDist);
	wasRendering = true;
	return activeShader->getAttrs();
}
map<string, GLint> OglRenderer::preRenderSkybox( frameinfo frinf, OglLightDir *dirLight, string shaderName )
{
	resetTransform();
	Skybox( frinf.rwidth, frinf.rheight, dirLight->getData( ), shaderName );
	wasRendering = true;
	return activeShader->getAttrs();
}
map<string, GLint> OglRenderer::preRenderWasser( OglCam *cam, glm::vec3 ambient, OglLightDir *dirLight, float viewDist, frameinfo frinf, string shaderName )
{
	resetTransform( );
	Wasser( ambient, frinf.rwidth, frinf.rheight, dirLight->getData( ), shaderName );
	setCamera( cam->update( frinf ) );
	activeShader->importValue( "MVPworld", glm::translate(objMat,glm::vec3(cam->x,cam->y,cam->z)) );
	activeShader->importValue( "camPos", glm::vec3( cam->x, cam->y, cam->z ) );
	activeShader->importValue( "viewDist", viewDist );
	activeShader->importValue( "tex", &wasserTex );
	wasRendering = true;
	return activeShader->getAttrs( );
}
map<string, GLint> OglRenderer::preRenderFire( OglCam *cam, frameinfo frinf, string shaderName )
{
	resetTransform( );
	Fire( frinf.rwidth, frinf.rheight, shaderName );
	setCamera( cam->update( frinf ) );
	//activeShader->importValue( "camPos", glm::vec3( cam->x, cam->y, cam->z ) );
	activeShader->importValue("tex", &fireTex);
	activeShader->importValue("size", glm::vec2(8, 4));
	wasRendering = true;

	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ONE );
	glDepthMask( GL_FALSE );
	//glBlendEquation( GL_FUNC_ADD );
	return activeShader->getAttrs( );
}
void OglRenderer::postRenderFire( )
{
	glDisable( GL_BLEND );
	glDepthMask( GL_TRUE );
}
void OglRenderer::postRender()
{
	wasRendering = false;
	glutSwapBuffers( );
}