#include "Wasser.h"

void Wasser::init(Bufferism *buf)
{
	uint seed = chrono::high_resolution_clock::now( ).time_since_epoch( ).count( );
	default_random_engine gen( seed );
	uniform_real_distribution<double> dist( 0.0, 1.0 );

	plane.init(BUF_POS | BUF_NORMAL | BUF_UV | BUF_COLOR);
	plane.transform( glm::scale( glm::mat4(1), glm::vec3(wSc) ) );
	plane.toBuffs( buf );
	meshes.push_back( MeshMat( plane, -1 ) );

	camPos = glm::vec2( 0 );
	offDir1 = glm::vec2( dist( gen ), dist( gen ) );
	offDir2 = glm::vec2( dist( gen ), dist( gen ) );
	offDir1 = glm::normalize( offDir1 );
	offDir2 = glm::normalize( offDir2 );
	off1 = glm::vec2( 0 );
	off2 = glm::vec2( 0 );
}

glm::mat4 Wasser::myTransform( glm::mat4 &rot )
{
	rot = glm::mat4( 1 );
	glm::mat4 m( 1 );
	m = m*glm::translate(glm::vec3(camPos.x,wasserMinus,camPos.y));
	return m;
}
void Wasser::step( glm::vec2 camPos2, float delta )
{
	off1 += offDir1*delta;
	off2 += offDir2*delta;
	camPos = camPos2;
}

void Wasser::render( map<string, GLint> attrs )
{
	renderer->activeShader->importValue( "off1", off1 );
	renderer->activeShader->importValue( "off2", off2 );
	OglModel::render( attrs );
}