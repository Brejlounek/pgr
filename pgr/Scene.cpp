#include "pgr.h"
#include "Scene.h"
#include "frameinfo.h"
#include <glm/glm.hpp>
//#include <glfw3.h>
#include <random>
#include <chrono>
using namespace glm;

#include "Meshes.h"

#define DEBUG

void Scene::init()
{
	ois.init( );
	buf.init( BUF_POS | BUF_NORMAL | BUF_UV | BUF_COLOR );
	renderer.init( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );

	uint seed = chrono::system_clock::now( ).time_since_epoch( ).count( );
	//seed = 3246853454;
	default_random_engine gen( seed );
	uniform_real_distribution<double> dist( 0.0, 1.0 );
	

	cout << "creating models..." << endl;
	vector<glm::vec2> points;
	for (int i = 0; i < 0; i++)
	{
		GLfloat wi = 14;
		glm::vec2 p(dist(gen) * wi - wi / 2, dist(gen) * wi - wi / 2);
		points.push_back(p);
	}

	vector<OglMesh> meshes;
	for ( uint i = 0; i < points.size(); i++ )
	{
		glm::mat4 transl = glm::translate(glm::vec3(points[i].x, 0, points[i].y));

		float minDist = INFINITY;
		for (uint q = 0; q < points.size(); q++)
		{
			if (q != i)
			{
				float dist = glm::distance(points[i], points[q]);
				if (dist < minDist)
					minDist = dist;
			}
		}

		//minDist = minDist*0.5;
		minDist = minDist*0.8;

		MeshBuilding building;
		building.init(BUF_POS | BUF_COLOR | BUF_NORMAL | BUF_UV,minDist);
		building.transform(transl);
		meshes.push_back( building );
	}

	plane.init( BUF_POS | BUF_NORMAL | BUF_UV | BUF_COLOR );
	plane.color( glm::vec3( .9 + dist( gen )*.1, .9 + dist( gen )*.1, .9 + dist( gen )*.1 ) );
	plane.toBuffs( &buf );
	//plane.transform( glm::scale( glm::mat4( 1 ), glm::vec3( 1000, 1, 1000 ) ) );
	meshes.push_back( plane );

	/*for (uint i = 0; i < 25; i++)
		meshes.push_back(hmaps[i]);*/

	for (int i = 1; i < meshes.size(); i++)
		meshes[0] = meshes[0] + meshes[i];
	env = meshes[0];
	env.toBuffs(&buf);

	HeightMap* hmap = terrain.init( 1, terrN );
	terrain.toBuffs( &buf );

	ship.init(&renderer);
	ship.toBuffs( buf );
	cam = ship.getCam( );
	ship.hmap = hmap;

	skybox.fromFile("meshes/skybox.3ds", &renderer, FLIPZ);
	skybox.toBuffs(buf);
	skybox.scale = glm::vec3(500);


	for ( uint i = 0; i < fireN; i++ )
	{
		Fire fire;
		fire.init( &buf, i );
		fires.push_back( fire );
	}
	wasser.init(&buf);
	//water.init(0);
	//water.toBuffs(&buf);

	buf.postInit( );

	lDir.color = glm::vec3( 0.8, 0.8, 0.8 );
	lDir.pos = glm::vec3( -0.45, 0.32, 0.83 );
	camView1();
	freeCam = false;
}

int cnt = 0;

void Scene::camView1()
{
	cam->x = staticCam1.x;
	cam->y = staticCam1.y;
	cam->z = staticCam1.z;
	cam->rotx = staticCam1Rot;
	cam->roty = 0;
}
void Scene::camView2()
{
	cam->x = staticCam2.x;
	cam->y = staticCam2.y;
	cam->z = staticCam2.z;
	cam->rotx = staticCam2Rot;
	cam->roty = 0;
}

bool mWasDown = false;
bool uWasDown = false;

void Scene::render(double delta)
{
	uint seed = chrono::system_clock::now().time_since_epoch().count();
	//seed = 3246853454;
	default_random_engine gen(seed);
	uniform_real_distribution<double> dist(0.0, 1.0);

	MouseInfo mi = ois.getmouse( );
	ois.keyb->capture( );
	if (ois.keyb->isKeyDown(KC_M))
	{
		if (!mWasDown)
		{
			freeCam = !freeCam;
			if (freeCam)
				cam = &camWASD;
			else
				cam = ship.getCam();
			mWasDown = true;
		}
	}
	else
		mWasDown = false;
	if ( ois.keyb->isKeyDown( KC_U ) )
	{
		if ( !uWasDown )
		{
			ship.autodrive = !ship.autodrive;
			uWasDown = true;
		}
	}
	else
		uWasDown = false;
	if (ois.keyb->isKeyDown(KC_C))
		camView1();
	if (ois.keyb->isKeyDown(KC_V))
	{
		camView2();
	}

	frameinfo fr;
	fr.rwidth = glutGet( GLUT_WINDOW_WIDTH );
	fr.rheight = glutGet( GLUT_WINDOW_HEIGHT );
	fr.ratio = fr.rwidth / fr.rheight;
	if (freeCam)
	{
		camWASD.moveUp = true;
		camWASD.doinput(mi, ois.keyb, delta);
	}
	else
		ship.doInput(mi, ois.keyb, delta, fr);

	vector<Bufferism*> bufv( 1 ); bufv[0] = { &buf };

	attrMap attr=renderer.preRenderDir( cam, ambient, &lDir, viewDist, fr, "terr" );
//	renderer.activeShader->importValue( "fogColor", skyCol );
	renderer.activeShader->importValue( "skyCol", skyCol );
	renderer.activeShader->importValue( "skySunCol", skySunCol );
	terrain.render( attr, glm::vec3(cam->x,cam->y,cam->z), viewDist, &renderer );

	glStencilFunc( GL_ALWAYS, 2, -1 );

	attr = renderer.preRenderDir(cam, glm::vec3(0.2), &lDir, viewDist, fr, "dirL");
	renderer.activeShader->importValue( "MVP", glm::mat4(1) );
	renderer.transformUpdate();

	renderer.activeShader->importValue( "fogColor", skyCol );

	glm::vec3 posRandom(dist(gen)-0.5, dist(gen)-0.5, dist(gen)-0.5);
	posRandom *= 10;
	glm::vec3 posL = glm::vec3(0, 0, -30) + posRandom;
	renderer.activeShader->importValue("pointL", posL);
	renderer.activeShader->importValue("pointCol", pointCol);
	renderer.activeShader->importValue("specCol", specCol);
	renderer.activeShader->importValue("pointAtt", pointAtt);

	renderer.activeShader->importValue("dirLspec", dirLspec);
	renderer.activeShader->importValue("shiny", shiny);

	glm::vec3 camView = glm::vec3(cam->xt - cam->x, cam->yt - cam->y, cam->zt - cam->z);
	camView = glm::normalize(camView);
	renderer.activeShader->importValue("view", camView);

	ship.render( attr );

	glStencilFunc(GL_ALWAYS, 3, -1);

	attr = renderer.preRenderWasser( cam, ambient, &lDir, viewDist, fr, "wasser" );
	renderer.activeShader->importValue( "skyCol", skyCol );
	renderer.activeShader->importValue( "skySunCol", skySunCol );
	renderer.activeShader->importValue("specCol", specCol);
	renderer.activeShader->importValue("dirLspec", dirLspec);
	renderer.activeShader->importValue( "shiny", shiny );

	wasser.renderer = &renderer;
	wasser.step( glm::vec2(cam->x,cam->z), delta*waterSpeed );
	renderer.transformUpdate();
	wasser.render(attr);

	glStencilFunc(GL_ALWAYS, 0, -1);

	//glDisable(GL_DEPTH_TEST);
	attr = renderer.preRenderSkybox(fr, &lDir, "skybox");
	renderer.activeShader->importValue("zFar", cam->farz);
	renderer.activeShader->importValue("MVP",cam->noTranslMatrix);
	renderer.activeShader->importValue( "skyCol", skyCol );
	renderer.activeShader->importValue( "skySunCol", skySunCol );
	skybox.skyboxRender(attr);
	//glEnable(GL_DEPTH_TEST);

	glStencilFunc(GL_ALWAYS, 4, -1);

	attr = renderer.preRenderFire( cam, fr, "fire" );

	for ( uint i = 0; i < fireN; i++ )
	{
		fires[i].step( delta, &ship );
		fires[i].renderer = &renderer;
	}

	renderer.transformUpdate( );
	for ( uint i = 0; i < fireN; i++ )
	{
		fires[i].render( attr );
	}
	renderer.postRenderFire();

	renderer.postRender();
}

void Scene::pick( int x, int y )
{
	glEnable( GL_STENCIL_TEST );
	glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
	glStencilFunc( GL_ALWAYS, 1, -1 );

	render( 0.00001 );

	glFinish( );

	unsigned char pixelID;
	glReadPixels(x, glutGet(GLUT_WINDOW_HEIGHT) - y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &pixelID);

	switch ( pixelID )
	{
		case 0:
			cout << "sky! ";
			break;
		case 1:
			cout << "terrain! ";
			break;
		case 2:
			cout << "plane! ";
			break;
		case 3:
			cout << "water! ";
			break;
		case 4:
			cout << "fire! ";
			break;
	}
	cout << x << " " << y << endl;

	glDisable( GL_STENCIL_TEST );


}