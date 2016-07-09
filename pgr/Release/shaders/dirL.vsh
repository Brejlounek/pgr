#version 140
in vec3 position;
in vec3 vcolor;
in vec3 normal;
in vec2 uv;
out vec3 pnormal;
out vec3 onormal;
out vec3 pcolor;
out vec2 puv;
out vec3 pos;
out float fogFactor;

uniform mat4 MVP;
uniform mat4 MVPworld;
uniform mat4 MVProt;
uniform float viewDist;

void main() {
	gl_Position = MVP * vec4(position, 1.0f);
	vec4 normal4 = MVProt * vec4(normal, 1.0f);
	pnormal=normal4.xyz;
	onormal=normal;
	pcolor=vcolor;
	puv=uv;

	vec4 pos4 = MVPworld * vec4(position, 1.0f);
	pos = position;
	float dist = gl_Position.z/viewDist;
	fogFactor = 1-sqrt(dist);//max(0,exp2(-dist*dist)
	fogFactor=clamp(fogFactor,0,1);
}