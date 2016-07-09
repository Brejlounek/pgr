#include <iostream>
#include <chrono>
#include <ratio>
#include "Scene.h"
using namespace std;
using namespace chrono;

const int WIN_WIDTH = 1024;
const int WIN_HEIGHT = 640;
const char * WIN_TITLE = "PGR project";

Scene scene;
static double deltaTime = 0.0;

bool init()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	scene.init();
	return true;
}

void render()
{
	high_resolution_clock::time_point start = high_resolution_clock::now( );
	scene.render(deltaTime);
	high_resolution_clock::time_point end = high_resolution_clock::now( );

	duration<double> time_span = duration_cast<duration<double>>(end - start);
	deltaTime = time_span.count( ) ;
}

void mouseCallback( int button, int state, int x, int y ) {
	// do picking only on mouse down
	if ( state == GLUT_DOWN ) {
		scene.pick( x, y );
	}
}

void keyboardCallback( unsigned char key, int x, int y ) {
	// exit on escape
	if ( key == 27 )
		exit( 0 );
	if ( key >= 'a' && key <= 'z' )
		cout << key;
	/*if ( key >= GLUT_KEY_F1 && key <= GLUT_KEY_F12 )
		cout << "F" << key - GLUT_KEY_F1;*/
}

void specialCallback( int key, int x, int y ) {
	if ( key >= GLUT_KEY_F1 && key <= GLUT_KEY_F12 )
	cout << "F" << key - GLUT_KEY_F1+1;
}

void reshapeCallback( int newWidth, int newHeight ) {

	/*gameState.windowWidth = newWidth;
	gameState.windowHeight = newHeight;*/

	glViewport( 0, 0, (GLsizei) newWidth, (GLsizei) newHeight );
}

int main(int argc, char* argv[])
{
	cout << "Welcome to this relaxing flight over terrain simulator! Controls:" << endl;
	cout << "W\t\taccelerate" << endl;
	cout << "S\t\tbreak" << endl;
	cout << "mouse\t\tturn" << endl;
	cout << "M\t\tmanual free camera" << endl;
	cout << "WASDRF + mouse\tmanual free camera control" << endl;
	cout << "U\t\tautodrive" << endl;

	glm::vec3 automobil(0,0,0);
	automobil = glm::normalize(automobil);

	glutInit(&argc, argv);

	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL );
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow(WIN_TITLE);
	//glutFullScreen( );
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutIdleFunc( render );
	glutDisplayFunc( render );
	glutReshapeFunc( reshapeCallback );
	glutMouseFunc( mouseCallback );
	glutKeyboardFunc( keyboardCallback );
	glutSpecialFunc( specialCallback );

	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
		pgr::dieWithError("pgr init failed, required OpenGL not supported?");

	if (!init())
		pgr::dieWithError("init failed, cannot continue");

	glutMainLoop();

	return 0;
}