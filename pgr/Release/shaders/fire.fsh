#version 140
in vec2 puv;
out vec4 color;

uniform sampler2D tex;

void main() {
	vec3 tCol = texture( tex, vec2(puv.x,puv.y) ).rgb;

	color = vec4(tCol,1);
	//color = vec4(abs(puv),0,1);
	//color = vec4(normalize(pnormal),1);
}