#include <vector>
#include <map>
using namespace std;

#include "pgr.h"
#include "OglBuff.h"
#include "OglMesh.h"
#include "OglModel.h"
#include "OglRenderer.h"
#include "OglOIS.h"

#include "Meshes.h"
#include "MeshesCity.h"
#include "Ship.h"
#include "Terrain.h"
#include "Fire.h"
#include "Wasser.h"

#include "parametry.h"

#define attrMap map<string,GLint>

/**
custom scene
*/
class Scene {
public:
	void init();
	void render(double delta);

	void camView1();
	void camView2();
	void pick( int x, int y );

private:
	Bufferism buf;
	OglRenderer renderer;
	OglOIS ois;

	CamWASDRF camWASD;
	OglLightDir lDir;

	OglMesh env;
	Ship ship;
	Terrain terrain;
	OglModel skybox;
	Wasser wasser;
	MeshPlane plane;
	vector<Fire> fires;

	OglCam *cam;
	bool freeCam;
};