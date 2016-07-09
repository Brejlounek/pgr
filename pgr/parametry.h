#ifndef PARAMETRY_KLZUO5785EXSUT4IUZTUD
#define PARAMETRY_KLZUO5785EXSUT4IUZTUD

#define uint unsigned int

//scene
const glm::vec3 skySunCol(201 / 255.0, 237 / 255.0, 255 / 255.0); //! color of the sky around the sun
const glm::vec3 skyCol(90 / 255.0, 194 / 255.0, 247 / 255.0); //! color of the sky
const glm::vec3 pointCol(255 / 16.0, 116 / 16.0, 0); //! color of the point light in nozzle
const glm::vec3 specCol(1, 1, 1); //! plane's specular color
const glm::vec3 dirLspec(0.8f, 0.8f, 0.8f); //! specular intensity of sun
const float pointAtt = 0.01f; //! attribution of point light in nozzle
const float shiny = 20.0f; //! shininess of the plane

const glm::vec3 staticCam1(225, 50, 348);
const glm::vec3 staticCam2(675, 50, 453);
const float staticCam1Rot = 100 * DEGTORAD;
const float staticCam2Rot = 200 * DEGTORAD;

const uint fireN = 8; //! number of fire planes
const float viewDist = 1000; //! view distance
const glm::vec3 ambient(0.2);
const float waterSpeed = 0.1f;

//terrain
const uint terrN = 161; //! number of terrain vertices
const float sizeBl = 500.0f; //! size of one block of terrain (-sizeBl/terrN in reality)
const float terrHeight = 1000; //! terrain height, much smaller in reality (hierarchy)

//ship
const float stiffness = 0.2; //! reaction to rotation
const float dist = 20; //! camera dostance from the plane
const float heightRatio = 0.3; //! camera height above the plane
const float brakeFactor = 3; //! how much plane breaks
const uint points = 10; //! random points in autodrive
const float rWi = 2000; //! how far to drive in autodrive
const float timeAuto = 40; //! how many seconds does the autodrive looplast
const float shipOff = 10; //! minimum distance between plane and terrain (cca)
const float sizeH = 500.0f; //! same as sizeBl for some reason
const float camDist = 0.001f; //! how much further will the camera be with acceleration
const float autodriveHeight = 150.0f; //! how high to autodrive
const float shipSpeed = -100.0;
const glm::vec3 firstPos(0,160,0);

//wasser
const float wSc = 2000; //! how far from camera water renders (viewDist would be enough, huh)
const float wasserMinus = -30; //! water level

//fire
const float fireScale = 15;
const glm::vec3 fireOffset(0, 0, -20); //! fire start position
const glm::vec3 fireOffset2(0, 0, -70); //! fire end position
const uint randN = 200; //! number of generated random numbers
const float frames = 32; //! number of frames in texture
const float rSize = 30; //! how far to randomize fire
const float fireSpeed = 40;

#endif