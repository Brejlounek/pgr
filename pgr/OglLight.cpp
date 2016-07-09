#include "OglLight.h"

OglLightDir::OglLightDir()
{
	rotx = 0; roty = 0;
	lookat();
	color = glm::vec3( 1, 1, 1 );

	shadow = false;
	cam.x = 0;
	cam.y = 0;
	cam.z = 0;
}

glm::vec3 OglLightDir::input( MouseInfo &mi, frameinfo frinf )
{
	if ( mi.r )
	{
		rotx -= 1.*mi.x / sens;
		roty -= 1.*mi.y / sens;
		lookat();
	}
	cam.rotx = rotx;
	cam.roty = roty;
	cam.lookat();
	cam.xt *= -1; cam.yt *= -1; cam.zt *= -1;
	MVP = cam.update( frinf );
	return pos;
}

void OglLightDir::lookat() 
{
	pos.x = sin( rotx )*cos( roty );
	pos.z = cos( rotx )*cos( roty );
	pos.y = sin( roty );
}

LightData OglLightDir::getData()
{
	LightData data( pos, color, MVP, cam.prevMatrix, cam.invMatrix );
	data.shadow = shadow;
	return data;
}