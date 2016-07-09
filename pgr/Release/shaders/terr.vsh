#version 140
in vec3 position;
in vec3 vcolor;
in vec3 normal;
out vec3 pnormal;
out vec3 pcolor;
out vec3 pos;
out float fogFactor;

uniform mat4 MVP;
uniform mat4 MVPworld;
uniform vec3 normalS;
uniform vec3 patchCoord;
uniform float viewDist;

void main() {
	gl_Position = MVP * vec4(position, 1.0f);
	pnormal=normal*normalS;
	pcolor=vcolor;

	pos = position*normalS + patchCoord;
	float dist = gl_Position.z/viewDist;
	fogFactor = 1-sqrt(dist);//max(0,exp2(-dist*dist)
	fogFactor=clamp(fogFactor,0,1);
}