#include "MeshesCity.h"

void MeshBuilding::init(uint flags, float size)
{
	uint seed = chrono::system_clock::now().time_since_epoch().count();
	//seed = 3246687674;
	default_random_engine gen(seed);
	uniform_real_distribution<double> dist(0.0, 1.0);

	GLfloat degr = dist(gen) * 360;
	glm::mat4 transl = glm::translate(glm::vec3(0, 1, 0));
	double basesc = 0.3+dist(gen)*0.3;
	glm::mat4 scale;
	if (size != 0.0f)
		scale = glm::scale(glm::vec3(size,
		basesc + dist(gen) * dist(gen) * 3 * size,
		size));
	else
		scale = glm::scale(glm::vec3(basesc + dist(gen),
		basesc + dist(gen) * dist(gen) * 3,
		basesc + dist(gen)));
	glm::mat4 rotate = glm::rotate(degr, glm::vec3(0, 1, 0));

	BuildShape sh;
	fl = flags;

	stepSize = 0;
	if ( flags & BUF_POS )
		stepSize += 3;
	if ( flags & BUF_COLOR )
		stepSize += 3;
	if ( flags & BUF_NORMAL )
		stepSize += 3;
	if ( flags & BUF_UV )
		stepSize += 2;

	//sides
	for ( int q = 0; q < 2; q++ ) //two floors
	for ( int i = 0; i < sh.size; i++ )
	{
		for ( int w = 0; w < 2; w++ ) //double horizontal normals
		{
			glm::vec3 pos(sh[i].x,
				q * 2 - 1,
				sh[i].y);
			glm::vec3 color(1, 0, 1);
			glm::vec3 normal(0, 0, 0);
			glm::vec2 uv(0,0);

			addVertex(flags, pos, color, normal, uv);
		}
	}

	uint offs = verts.size() / stepSize;
	//roof
	for ( int i = 0; i < sh.size; i++ )
	{
		glm::vec3 pos(sh[i].x,
			1,
			sh[i].y);
		glm::vec3 color(1, 0, 1);
		glm::vec3 normal(0, 0, 0);
		glm::vec2 uv(0, 0);

		addVertex(flags, pos, color, normal, uv);
	}

	uint offs2 = verts.size() / stepSize;

	//center roof
	glm::vec3 pos_(0,1,0);
	glm::vec3 color_(1, 0, 1);
	glm::vec3 normal_(0, 0, 0);
	glm::vec2 uv_(0, 0);

	addVertex(flags, pos_, color_, normal_, uv_);

	uint top = sh.size;
	for ( int i = 0; i < sh.size; i++ )
	{
		uint nexti = i + 1;
		if ( nexti == top ) nexti = 0;

		addIndex(nexti * 2 + 1, i * 2, (i + top) * 2);
		addIndex(nexti * 2 + 1, (i + top) * 2, (nexti + top) * 2 + 1);
	}

	for ( int i = 0; i < sh.size; i++ )
	{
		uint nexti = i + 1;
		if ( nexti == top ) nexti = 0;

		addIndex(offs + nexti, offs + i, offs2);
	}

	if (flags & BUF_NORMAL)
		calcNormals();

	color( glm::vec3( 1.0*dist( gen ), 1.0*dist( gen ), 1.0*dist( gen ) ) );
	transform( rotate*scale*transl );
	rotateNormals( rotate );

	//operator+(cube);
}







BuildShape::BuildShape( )
{
	uint seed = chrono::system_clock::now( ).time_since_epoch( ).count( );
	//seed = 3246687674;
	default_random_engine gen( seed );
	uniform_real_distribution<double> dist( 0.0, 1.0 );

	uint shape = ((int) (dist( gen ) * 2)) % 2;
	uint n = dist( gen ) * 3 + 3;
	GLfloat rad = 0.5f + dist(gen)*0.5f;
	switch ( shape )
	{
		case 0: //ngon
			for ( int i = 0; i < n; i++ )
			{
				float ang = 2.0f * i / n * PI;
				Point p( cos( ang ), sin( ang ) );
				points.push_back( p );
			}
			break;
		case 1: //star
			for ( int i = 0; i < n; i++ )
			{
				float ang0 = 2.0f * (i * 2) / (n * 2) * PI;
				float ang1 = 2.0f * (i * 2 + 1) / (n * 2) * PI;
				Point p0( cos( ang0 ), sin( ang0 ) );
				Point p1( cos( ang1 )*rad, sin( ang1 )*rad );
				points.push_back( p0 );
				points.push_back( p1 );
			}
			break;
		case 2: //random
			//for ( int i = 0; i < n; i++ )
			//{
			//	Point p( dist( gen )*rad0, dist( gen )*rad1 );
			//	points.push_back( p );
			//}
			break;
	}
	size = points.size();
}

Point BuildShape::operator[]( size_t i )
{
	return points[i];
}