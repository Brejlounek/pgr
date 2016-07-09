#version 140
in vec3 position;
in vec2 uv;
out vec2 puv;

uniform mat4 MVP;
uniform vec2 size;
uniform float frame;

void main() {
	gl_Position = MVP * vec4(position, 1.0f);

	float flFrame = mod( floor(frame), size.x*size.y );
	puv.x = mod(flFrame,size.x) / size.x;
	puv.y = 1-(flFrame - mod(flFrame,size.x))/(size.x * size.y);
	puv += uv/size;
	//puv = uv;
}