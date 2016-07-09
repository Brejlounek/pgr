#include "Ship.h"

void Ship::init(OglRenderer* renderer_)
{
	fromFile("meshes/ship_.3ds", renderer_, 0);
	pos = firstPos;
	scale.x = scale.y = scale.z = 0.1;

	uint seed = chrono::high_resolution_clock::now( ).time_since_epoch( ).count( );
	default_random_engine gen( seed );
	uniform_real_distribution<double> dist( 0.0, 1.0 );

	rot.x = 0; rot.y = 0; rot.z = 0;
	shRot.x = 0; shRot.y = 0; shRot.z = 0;
	roll = 0;
	speed = shipSpeed;
	sens = 200;
	vSpeed = 0;
	P = 0;
	autodrive = false;
	for ( uint i = 0; i < points; i++ )
	{
		glm::vec3 rPoint(dist(gen)*rWi - rWi*0.5f, autodriveHeight*dist(gen), dist(gen)*rWi - rWi*0.5f);
		point.push_back(rPoint);
	}

	rotationMat = glm::mat4( 1 );
	upV = glm::vec4( 0, 1, 0, 0 );
	rightV = glm::vec4( 1, 0, 0, 0 );
	vDir = glm::vec3( 0, 0, 0 );
}

void Ship::doInput( MouseInfo &mi, Keyboard* keyb, double delta_, frameinfo frinf )
{
	delta = delta_;
	if ( !autodrive )
	{
		if ( mi.l )
		{
			rot.x -= 1.*mi.x / sens;
			rot.y += 1.*mi.y / sens;
			roll -= (1.*mi.x / sens) * 50;
		}
		roll *= stiffness;

		bool shift = keyb->isModifierDown( Keyboard::Shift );
		if ( keyb->isKeyDown( KC_UP ) || keyb->isKeyDown( KC_W ) )
			move( KC_W );
		if ( keyb->isKeyDown( KC_LEFT ) || keyb->isKeyDown( KC_A ) )
			move( KC_A );
		if ( keyb->isKeyDown( KC_DOWN ) || keyb->isKeyDown( KC_S ) )
			move( KC_S );
		if ( keyb->isKeyDown( KC_RIGHT ) || keyb->isKeyDown( KC_D ) )
			move( KC_D );
		if ( keyb->isKeyDown( KC_R ) )
			move( KC_R );
		if ( keyb->isKeyDown( KC_F ) )
			move( KC_F );

		MoveFromTerrain();

		vDir.x = 1.0*sin( shRot.x )*cos( shRot.y );
		vDir.z = 1.0*cos( shRot.x )*cos( shRot.y );
		vDir.y = 1.0*sin( shRot.y );
		pos += vDir*vSpeed*delta;

		if ( pos.y > 300 )
			pos.y = 300;

		behindShip.x = sin( rot.x )*cos( rot.y );
		behindShip.z = cos( rot.x )*cos( rot.y );
		behindShip.y = sin( rot.y );

		glm::vec3 diff = rot - shRot;
		diff *= stiffness / ((1 / delta) / 60);
		shRot = shRot + diff;

		shBehind.x = sin( shRot.x )*cos( shRot.y );
		shBehind.z = cos( shRot.x )*cos( shRot.y );
		shBehind.y = sin( shRot.y );
	}
	else
	{
		P += delta / timeAuto;
		//P = fmod( P, 1 );
		
		float indexF = P*points;
		uint index = indexF;
		glm::vec3 v0 = point[(index - 1) % points];
		glm::vec3 v1 = point[(index) % points];
		glm::vec3 v2 = point[(index + 1) % points];
		glm::vec3 v3 = point[(index + 2) % points];

		pos = intp( v0, v1, v2, v3, indexF - index );
		glm::vec3 der = -intpd( v0, v1, v2, v3, indexF - index );
		der = glm::normalize( der );

		MoveFromTerrain( );

		behindShip = der;
		shBehind = der;
	}

	//cout << rot.x << " " << rot.y << " " << rot.z << endl;

	glm::vec4 camPos = glm::vec4(0,10,0,0);
	up = glm::vec3( 0, 1, 0 );
	//if we are rotated upside down odd times, invert up vector
	float denom = floor( (rot.y + PI / 2.) / PI ) / 2.;
	if ( floor( denom ) != denom )
		up = -up;

	camPos = alignObject( glm::vec3( 0, 0, 0 ), behindShip, up ) * camPos;
	float dist2 = dist*(1 - camDist*min( 0.0f, vSpeed ));
	//cam.fov = 45 - 0.01f*min( 0.0f, vSpeed );

	cam.x = pos.x + behindShip.x * dist2 + camPos.x;
	cam.y = pos.y + behindShip.y * dist2 + camPos.y;
	cam.z = pos.z + behindShip.z * dist2 + camPos.z;
	cam.xt = pos.x + camPos.x*heightRatio;
	cam.yt = pos.y + camPos.y*heightRatio;
	cam.zt = pos.z + +camPos.z*heightRatio;
	
	glm::vec4 upN = glm::normalize( camPos );
	glm::vec3 upN3( upN.x, upN.y, upN.z );
	cam.up = upN3;
	cam.setMoveUp( false );
}

OglCam* Ship::getCam()
{
	return &cam;
}

void Ship::move(KeyCode code)
{
	switch (code)
	{
	case KC_W:
		vSpeed += speed*delta;
		break;
	case KC_S:
		vSpeed /= 1+(brakeFactor-1)*delta;
		break;
	/*case KC_A:
		vSpeed.x += sin( shRot.x + PI / 2 )*speed*delta;
		vSpeed.z += cos( shRot.x + PI / 2 )*speed*delta;
		break;
	case KC_D:
		vSpeed.x += sin( shRot.x - PI / 2 )*speed*delta;
		vSpeed.z += cos( shRot.x - PI / 2 )*speed*delta;
		break;
	case KC_R:
		vSpeed.y -= speed*delta;
		break;
	case KC_F:
		vSpeed.y += speed*delta;
		break;*/
	}
}

glm::mat4 Ship::myTransform(glm::mat4 &rot)
{
	up = glm::vec3( 0, 1, 0 );
	//if we are rotated upside down odd times, invert up vector
	float denom = floor( (shRot.y + PI / 2.) / PI ) / 2.;
	if ( floor( denom ) != denom )
		up = -up;

	glm::mat4 mat = glm::mat4( 1 );
	mat = glm::translate( mat, pos );
	rot = alignObject( glm::vec3( 0, 0, 0 ), shBehind, up );
	rot = glm::rotate( rot, roll, shBehind );
	mat = mat * rot;
	mat = glm::scale( mat, scale );

	//rot = rot * cam.noTranslMatrix;
	return mat;
}

void CamShip::setMoveUp(bool b)
{
	moveUp = b;
}

glm::vec3 Ship::intp( glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, float p )
{
	glm::vec3 result;

	float t = p;
	float t2 = t*t;
	float t3 = t*t*t;

	float p0 = -t3 + 2 * t2 - t;
	float p1 = 3 * t3 - 5 * t2 + 2;
	float p2 = -3 * t3 + 4 * t2 + t;
	float p3 = t3 - t2;
	p0 *= 0.5; p1 *= 0.5;
	p2 *= 0.5; p3 *= 0.5;

	result += p0*v0;
	result += p1*v1;
	result += p2*v2;
	result += p3*v3;
	return result;
}
glm::vec3 Ship::intpd( glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, float p )
{
	glm::vec3 result;

	float t = p;
	float t2 = t*t;
	float t3 = t*t*t;

	float p0 = -3 * t2 + 2 * 2 * t - 1;
	float p1 = 3 * 3 * t2 - 5 * 2 * t;
	float p2 = -3 * 3 * t2 + 4 * 2 * t + 1;
	float p3 = 3 * t2 - 2 * t;
	p0 *= 0.5; p1 *= 0.5;
	p2 *= 0.5; p3 *= 0.5;

	result += p0*v0;
	result += p1*v1;
	result += p2*v2;
	result += p3*v3;
	return result;
}

float Ship::getHmap( glm::vec2 pos2 )
{
	uint size = hmap->getSize( );
	bool evenX = true;
	bool evenY = true;

	while ( pos2.x > sizeH - sizeH / size )
	{
		pos2.x -= sizeH - sizeH / size;
		evenX = !evenX;
	}
	while ( pos2.x<0 )
	{
		pos2.x += sizeH - sizeH / size;
		evenX = !evenX;
	}
	while ( pos2.y>sizeH - sizeH / size )
	{
		pos2.y -= sizeH - sizeH / size;
		evenY = !evenY;
	}
	while ( pos2.y < 0 )
	{
		pos2.y += sizeH - sizeH / size;
		evenY = !evenY;
	}

	pos2 /= sizeH - sizeH / size;
	if ( !evenX )
		pos2.x = 1 - pos2.x;
	if ( !evenY )
		pos2.y = 1 - pos2.y;
	pos2 *= size+2;
	return hmap->sampleNoises(pos2, 2) * terrHeight;
}
void Ship::MoveFromTerrain()
{
	float y0 = getHmap( glm::vec2( pos.x + shipOff, pos.z + shipOff ) );
	float y1 = getHmap( glm::vec2( pos.x - shipOff, pos.z + shipOff ) );
	float y2 = getHmap( glm::vec2( pos.x + shipOff, pos.z - shipOff ) );
	float y3 = getHmap( glm::vec2( pos.x - shipOff, pos.z - shipOff ) );
	float y = max(y0, max(y1, max(y2, max(y3, wasserMinus))));

	if ( pos.y < y + shipOff )
		pos.y = y + shipOff;

	//cout << y << " " << pos.y << endl;
}