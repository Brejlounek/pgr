#version 140
in vec3 pnormal;
in vec3 pcolor;
in vec3 pos;
in vec3 posW;
in vec3 glPos;
out vec4 color;

uniform vec3 ambient;
uniform vec3 view;

uniform vec3 dirL;
uniform vec3 dirLcol;
uniform vec3 specCol;
uniform vec3 dirLspec;
uniform float shiny;

uniform vec3 mCol;
uniform vec3 skyCol;
uniform vec3 skySunCol;
uniform vec3 camPos;
uniform vec2 off1;
uniform vec2 off2;
uniform float viewDist;

uniform sampler2D tex;

void main() {
	vec3 light = clamp( dot(pnormal,dirL), 0,1) * dirLcol;
	vec3 color3 = ambient+light;
	float spec = pow( max(0,dot(view,reflect(dirL,pnormal))),shiny);
	if (dot(pnormal,dirL)<0)
		spec=0;

	//color3 *= 1-fogFactor*fogFactor;
	//color3 *= fogFactor;

	vec3 texColor;
	texColor = texture( tex, posW.xz*0.02 + off1 ).rgb + texture( tex, posW.xz*0.005+0.875154 + off2 ).rgb;
	texColor *= 0.5f;
	//vec3 texColor=vec3(1);

	float fLight = dot(normalize(posW-camPos),dirL)*0.5f+0.5f;
	vec3 fogColor = skySunCol*fLight + skyCol*(1-fLight);

	float dist = glPos.z/viewDist;
	float fogFactor = 1-sqrt(dist);
	fogFactor = clamp(fogFactor,0,1);
	vec3 finColor = (texColor*color3 + spec*specCol)*fogFactor + fogColor*(1-fogFactor);

	finColor = (finColor-0.13f)*1.14943f;
	color = vec4(finColor,1);
	//color = vec4(color3,1);
	//color = vec4(fogFactor,0,0,1);
	//color = vec4(normalize(pnormal),1);
}