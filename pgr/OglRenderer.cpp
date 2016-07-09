#include <random>
#include <chrono>
#include <dirent.h>
#include <string>

#include "OglRenderer.h"
#include "Meshes.h"

uint seed = chrono::system_clock::now( ).time_since_epoch( ).count( );
default_random_engine gen( seed );
uniform_real_distribution<double> dist( 0.0, 1.0 );

void OglRenderer::init(uint wi,uint he)
{
	createRandomTexture();
	transformReady = true;
	camMat = glm::mat4(1);
	camMatPrev = glm::mat4(1);
	camMatNoTransl = glm::mat4(1);
	objMat = glm::mat4(1);

	DIR *d;
	struct dirent *dir;
	d = opendir( "shaders" );
	int i = 0;
	if ( d )
	{
		while ( (dir = readdir( d )) != NULL )
		{
			if ( i > 1 )
			{
				string name = dir->d_name;
				string subname = name.substr( name.length() - 4, 4 );
				if ( subname == ".vsh" )
				{
					cout << "vertex: " << name << endl;
					string compileName = "shaders\\" + name;
					OglVertexShader vsh; vsh.compile( compileName.c_str() );
					vsh.notDelete();
					shVertex[name] = vsh;
				}
				else if ( subname == ".fsh" )
				{
					cout << "fragment: " << name << endl;
					string compileName = "shaders\\" + name;
					OglFragmentShader fsh; fsh.compile( compileName.c_str( ) );
					fsh.notDelete();
					shFragment[name] = fsh;
				}
				else
					cout << "error reading shader " << name << endl;
			}
			i++;
		}

		closedir( d );
	}

	buildShader( "terr", "terr.vsh", "terr.fsh" );
	buildShader( "dirL", "dirL.vsh", "dirL.fsh" );
	buildShader( "skybox", "skybox.vsh", "skybox.fsh" );
	buildShader( "fire", "fire.vsh", "fire.fsh" );
	buildShader( "wasser", "wasser.vsh", "wasser.fsh" );

	fireTex.init( "gfx/explosion.jpg" );
	wasserTex.init( "gfx/wasser.png" );

	activeShader = 0;
	wasRendering = false;

	MeshFullscreenQuad quad;
	quad.init( BUF_POS | BUF_UV );
	bufQuad.init( BUF_POS | BUF_UV );

	quad.toBuffs( &bufQuad );
	bufQuad.postInit( );
}

void OglRenderer::createRandomTexture( )
{
	vector<uchar> data;
	for ( uint i = 0; i < 64 * 64; i++ )
	{
		uchar r = dist( gen )*256;
		float angle = dist( gen ) * 2 * PI;
		uchar g = cos( angle )*128+128;
		uchar b = sin( angle )*128+128;

		data.push_back( r );
		data.push_back( g );
		data.push_back( b );
	}

	randomTex.intFormat = GL_RGB;
	randomTex.format = GL_RGB;
	randomTex.clampBorder = false;
	randomTex.init( &data[0], 64, 64 );
}

void OglRenderer::buildShader( string strProg, string strVertex, string strFragment )
{
	OglShaderProg prog;
	prog.link( shVertex[strVertex], shFragment[strFragment] );
	shProg[strProg] = prog;
}

void OglRenderer::setCamera( glm::mat4 camMat_, bool wiggle )
{
	//activeShader->importValue( "MVP", camMat_ );
		transformReady = false;
	camMat = camMat_;
}
void OglRenderer::setCameraPrev( glm::mat4 camMat_, bool wiggle )
{
	//activeShader->importValue( "MVPprev", camMat );
	if (camMat_ != camMatPrev)
		transformReady = false;
	camMatPrev = camMat_;
}
void OglRenderer::setCameraNoTransl( glm::mat4 camMat_, bool wiggle )
{
	if (camMat_ != camMatPrev)
		transformReady = false;
	camMatNoTransl = camMat_;
}
void OglRenderer::setObj(glm::mat4 objMat_)
{
	if (objMat_ != objMat)
		transformReady = false;
	objMat = objMat_;
}
void OglRenderer::addObj(glm::mat4 objMat_)
{
	objMat = objMat_ * objMat;
	transformReady = false;
}
void OglRenderer::setRot( glm::mat4 rotMat_ )
{
	activeShader->importValue( "MVProt", rotMat_ );
}

void OglRenderer::enableBuffer( OglFrameBuffer *buffer, bool clear )
{
	if ( buffer )
		buffer->enable();
	else
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	if ( clear )
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		glClearStencil( 0 );
	}
}

void OglRenderer::transformUpdate()
{
	if (!transformReady)
	{
		activeShader->importValue("MVP", camMat*objMat);
	}
}

void OglRenderer::resetTransform()
{
	camMat = objMat = camMatPrev = camMatNoTransl = glm::mat4(1);
}


/*void OglRenderer::setMaterial( OglMaterial* mat )
{
	activeShader->importValue("mCol",mat->color);
	if ( mat->hasTexture )
		activeShader->importValue( "tex", &mat->texture );
}*/





void OglRenderer::AmbientDir( glm::vec3 ambient, LightData dirL, uint wi, uint he, string shaderName )
{
	if (!wasRendering)
		enableBuffer( 0 );
	activeShader = &shProg[shaderName];

	activeShader->enable( );
	//activeShader->importValue( "colorTexture", &texColor );
	activeShader->importValue( "ambient", ambient );
	activeShader->importValue( "dirL", dirL.pos );
	activeShader->importValue( "dirLcol", dirL.color );
	//bufQuad.render(activeShader ->ProgramID);
}

void OglRenderer::Wasser( glm::vec3 ambient, uint wi, uint he, LightData dirL, string shaderName )
{
	if ( !wasRendering )
		enableBuffer( 0 );
	activeShader = &shProg[shaderName];

	activeShader->enable( );
	//activeShader->importValue( "colorTexture", &texColor );
	activeShader->importValue( "ambient", ambient );
	activeShader->importValue( "dirL", dirL.pos );
	activeShader->importValue( "dirLcol", dirL.color );
	//bufQuad.render(activeShader ->ProgramID);
}

void OglRenderer::Skybox( uint wi, uint he, LightData dirL, string shaderName )
{
	if (!wasRendering)
		enableBuffer(0);
	activeShader = &shProg[shaderName];

	activeShader->enable();
	activeShader->importValue( "dirL", dirL.pos );
	//activeShader->importValue( "colorTexture", &texColor );
	//bufQuad.render(activeShader ->ProgramID);
}

void OglRenderer::Fire( uint wi, uint he, string shaderName )
{
	if ( !wasRendering )
		enableBuffer( 0 );
	activeShader = &shProg[shaderName];

	activeShader->enable( );
	//activeShader->importValue( "colorTexture", &texColor );
	//bufQuad.render(activeShader ->ProgramID);
}