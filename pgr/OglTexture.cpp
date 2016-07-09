#include "OglTex.h"

OglTex::OglTex()
{
	intFormat = GL_RGB;
	format = GL_RGB;
	type = GL_UNSIGNED_BYTE;
	minFilter = GL_NEAREST;
	magFilter = GL_NEAREST;
	clampBorder = false;
	compMode = GL_NONE;
	color[0] = color[1] = color[2] = color[3] = 0;
}

void OglTex::init()
{
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minFilter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, magFilter );
	if ( compMode != GL_NONE )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, compMode );

	if ( clampBorder )
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
		glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color );
	}
}

void OglTexTarget::init(uint wi, uint he)
{
	glGenTextures( 1, &id );
	glBindTexture( GL_TEXTURE_2D, id );
	glTexImage2D( GL_TEXTURE_2D, 0, intFormat, wi, he, 0, format, type, 0 );
	
	OglTex::init( );
}

void OglTex::init( void* data, uint wi, uint he )
{
	glGenTextures( 1, &id );
	glBindTexture( GL_TEXTURE_2D, id );
	glTexImage2D( GL_TEXTURE_2D, 0, intFormat, wi, he, 0, format, type, data );

	OglTex::init( );
}

void OglTex::init( string file )
{
	id = pgr::createTexture( file );
}