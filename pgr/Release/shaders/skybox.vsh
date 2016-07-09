#version 140
in vec3 position;
in vec2 uv;
out vec3 pos;
out vec2 puv;

uniform mat4 MVP;
uniform float zFar;

void main() {
	gl_Position = MVP * vec4(position, 1.0f);
	gl_Position.z = 0.99999*gl_Position.w;///gl_Position.w;
	pos = position;
	puv=uv;
}