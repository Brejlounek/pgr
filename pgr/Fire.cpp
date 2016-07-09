#include "Fire.h"

void Fire::init(Bufferism *buf, uint i)
{
	uint seed = chrono::high_resolution_clock::now( ).time_since_epoch().count();
	default_random_engine gen(seed+i);
	uniform_real_distribution<double> dist(0.0, 1.0);

	MeshPlane plane;
	plane.init(BUF_POS | BUF_NORMAL | BUF_UV | BUF_COLOR);
	plane.toBuffs(buf);
	meshes.push_back(MeshMat(plane,-1));

	//randoms.resize(randN);
	for (uint i = 0; i < randN; i++)
	{
		float fl = dist(gen);
		randoms.push_back(fl);
	}
	frame = dist(gen)*frames;
}
glm::mat4 Fire::myTransform(glm::mat4 &rot)
{
	rot = glm::mat4(1);
	glm::mat4 m(1);
	m = m*shipMat;

	glm::vec3 randV = glm::vec3( randoms[(n + 4) % randN], randoms[(n + 5) % randN], randoms[(n + 6) % randN] );

	glm::vec3 flPos = intp( fireOffset, fireOffset2 + (randV-glm::vec3(0.5))*rSize, P );
	m = m*glm::translate(flPos);
	m = m*glm::scale(glm::vec3(fireScale*(randoms[(n+0)%randN]*0.7 + 0.5)));
	m = m*glm::rotate( 270.0f, glm::vec3( 1, 0, 0 ) );
	return m;
}

void Fire::step(float delta, Ship* ship)
{
	frame += delta*fireSpeed;
	P = fmod((frame - 8) / 32, 1);
	n = (frame - 8) / 32;

	glm::mat4 rot2;
	shipMat = ship->myTransform(rot2);
	//shipMat = shipMat*glm::inverse(rot2);
}

void Fire::get(float &frame2)
{
	frame2 = frame;
}

float Fire::intp(float v1, float v2, float p)
{
	return v2*p + v1*(1 - p);
}
glm::vec3 Fire::intp(glm::vec3 v1, glm::vec3 v2, float p)
{
	glm::vec3 vysl;
	vysl.x = intp(v1.x, v2.x, p);
	vysl.y = intp(v1.y, v2.y, p);
	vysl.z = intp(v1.z, v2.z, p);
	return vysl;
}

void Fire::render( map<string, GLint> attrs )
{
	renderer->activeShader->importValue( "frame", frame );
	OglModel::render( attrs );
}