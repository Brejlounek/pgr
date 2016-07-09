#include "OglCam.h"

glm::mat4 OglCam::update( frameinfo frinf )
{
	wi = frinf.rwidth;
	he = frinf.rheight;

	GLfloat xAngle = fov*(1.0*wi / he)*DEGTORAD;
	GLfloat yAngle = fov*DEGTORAD;

	look = glm::normalize( glm::vec3( xt, yt, zt ) - glm::vec3( x, y, z ) )*nearz;
	right = glm::cross( look, glm::vec3( 0, 1, 0 ) );
	right = glm::normalize( right ) * nearz * tan( xAngle / 2 );
	if ( moveUp )
	{
		/*up_preroll = -glm::cross( look, right );
		up = glm::rotate( up_preroll, roll, look );
		up = glm::normalize( up ) * nearz * tan( yAngle / 2 );*/
		up = glm::vec3(0, 1, 0);
	}

	//if we are rotated upside down odd times, invert up vector
	//up = glm::vec3( 0, 1, 0 );
	float denom = floor( (roty + PI / 2.) / PI ) / 2.;
	if ( floor( denom ) != denom )
		up = -up;

	glm::mat4 Projection = glm::perspective( fov, 1.f*wi / he, nearz, farz );
	glm::mat4 View = glm::lookAt(
		glm::vec3( x, y, z ),
		glm::vec3( xt, yt, zt ),
		up
		);
	glm::mat4 ViewOrigin = glm::lookAt(
		glm::vec3( 0, 0, 0 ),
		glm::vec3( xt - x, yt - y, zt - z ),
		up
		);

	glm::mat4 MVP = Projection*View;
	glm::mat4 MVPOrigin = Projection*ViewOrigin;

	matrix = MVP;
	invMatrix = glm::inverse( MVPOrigin );
	prevMatrix = MVP;
	noTranslMatrix = MVPOrigin;
	return MVP;
}

void OglCam::lookat( float &xo, float &yo, float &zo, float rotxi, float rotyi )
{
	xo = sin( rotxi )*cos( rotyi );
	zo = cos( rotxi )*cos( rotyi );
	yo = sin( rotyi );
}
void OglCam::lookat( )
{
	xt = x + sin( rotx )*cos( roty );
	zt = z + cos( rotx )*cos( roty );
	yt = y + sin( roty );
}
void OglCam::init( const char* name )
{
	namestr = name;
	//namestr+="_pos";
}
glm::vec3 OglCam::getGeneric( )
{
	return right - look + up;
}
OglCam::OglCam( )
{
	x = 4; y = 3; z = 4;
	x = 0; y = 0; z = 0;
	xt = 0; yt = 0; zt = 0;
	/*x = 10.5114;
	y = 52.615;
	z = 1.2199;
	xt = 10.2639;
	yt = 51.6512;
	zt = 1.11835;
	x = 5.9;
	y = 9.9;
	z = 6.4;
	rotx = -2.25;
	roty = -0.9;*/
	fov = 45; nearz = 1; farz = 10000; roll = 0;
	wi = 640; he = 480;
	//rotx = -2.44500089; roty = -0.475000024;
	rotx = 0; roty = 0.5;
	//rotx = -2.16; roty = -1.37;
	look = right = up = up_preroll = glm::vec3( 1, 1, 1 ); up = glm::vec3( 0, 1, 0 );
	up_preroll = glm::vec3( 0, 1, 0 );
	prevMatrix = glm::mat4( 1 );
	moveUp = false;
}
CamData OglCam::getData()
{
	CamData data;
	data.pos = glm::vec3( x, y, z );
	data.zClip = glm::vec2( nearz, farz );
	data.zClipRatio = 1.0f * farz / nearz;
	data.invMatrix = invMatrix;

	return data;
}

//////////////////////

CamWASDRF::CamWASDRF( )
{
	OglCam( );
	lpressed = false;
}
void CamWASDRF::doinput( MouseInfo &mi, Keyboard* keyb, double delta_ )
{
	//return;
	//cout << rotx << " " << roty << endl;

	delta = delta_;
	if ( mi.l )
	{
		rotx -= 1.*mi.x / sens;
		roty -= 1.*mi.y / sens;
	}
	//roty = min( roty, PI/2-0.0001f );
	//roty = max( roty, -PI/2+0.0001f );

	bool shift = keyb->isModifierDown( Keyboard::Shift );
	if ( keyb->isKeyDown( KC_UP ) || keyb->isKeyDown( KC_W ) )
		move( KC_W, shift );
	if ( keyb->isKeyDown( KC_LEFT ) || keyb->isKeyDown( KC_A ) )
		move( KC_A, shift );
	if ( keyb->isKeyDown( KC_DOWN ) || keyb->isKeyDown( KC_S ) )
		move( KC_S, shift );
	if ( keyb->isKeyDown( KC_RIGHT ) || keyb->isKeyDown( KC_D ) )
		move( KC_D, shift );
	if ( keyb->isKeyDown( KC_R ) )
		move( KC_R, shift );
	if ( keyb->isKeyDown( KC_F ) )
		move( KC_F, shift );

	if (y > 350)
		y = 350;

	//lookat( xt, yt, zt, rotx, roty );
	this->lookat();
}
void CamWASDRF::move( KeyCode code, bool shift )
{
	switch ( code )
	{
		case KC_W:
			x += sin( rotx )*cos( roty )*speed*delta;
			z += cos( rotx )*cos( roty )*speed*delta;
			y += sin( roty )*speed*delta;
			break;
		case KC_S:
			x -= sin(rotx)*cos(roty)*speed*delta;
			z -= cos(rotx)*cos(roty)*speed*delta;
			y -= sin( roty )*speed*delta;
			break;
		case KC_A:
			x += sin(rotx + PI / 2)*speed*delta;
			z += cos(rotx + PI / 2)*speed*delta;
			break;
		case KC_D:
			x += sin(rotx - PI / 2)*speed*delta;
			z += cos(rotx - PI / 2)*speed*delta;
			break;
		case KC_R:
			y += speed*delta;
			break;
		case KC_F:
			y -= speed*delta;
			break;
	}
}


////////

glm::mat4 OglCamOrtho::update( frameinfo frinf )
{
	wi = frinf.rwidth;
	he = frinf.rheight;

	float left = -10, right = 10, bottom = -10, top = 10, nearr = -30, farr = 30;
	glm::mat4 Projection = glm::ortho( left, right, bottom, top, nearr, farr );
	glm::mat4 ViewOrigin = glm::lookAt(
		glm::vec3( 0, 0, 0 ),
		glm::vec3( xt - x, yt - y, zt - z ),
		up
		);

	prevMatrix = matrix;
	glm::mat4 MVPOrigin = Projection*ViewOrigin;
	invMatrix = glm::inverse( MVPOrigin );
	matrix = MVPOrigin;
	return MVPOrigin;
}