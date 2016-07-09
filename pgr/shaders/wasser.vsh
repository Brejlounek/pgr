#version 140
in vec3 position;
in vec3 vcolor;
in vec3 normal;
out vec3 pnormal;
out vec3 pos;
out vec3 posW;
out vec3 glPos;

uniform mat4 MVP;
uniform mat4 MVPworld;
uniform float viewDist;

void main() {
	gl_Position = MVP * vec4(position, 1.0f);
	pnormal=normal;

	vec4 pos4 = MVPworld * vec4(position, 1.0f);
	posW = pos4.xyz;
	pos = position;
	glPos = gl_Position.xyz;
}