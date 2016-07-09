#version 140
in vec3 pos;
in vec2 puv;
out vec4 color;

uniform sampler2D tex;
uniform vec3 skyCol;
uniform vec3 skySunCol;
uniform vec3 dirL;

void main() {
	float texColor = texture( tex, vec2(puv.x,1-puv.y) ).r;
	float light = dot(normalize(pos),dirL)*0.5f+0.5f;
	vec3 sColor = skySunCol*light + skyCol*(1-light);
	sColor = sColor + (1-sColor)*texColor;
	
	sColor = (sColor-0.13f)*1.14943f;
	color = vec4(sColor,1);
	//color = vec4(texColor,1);
}