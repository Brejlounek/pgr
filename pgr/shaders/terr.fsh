#version 140
in vec3 pnormal;
in vec3 pcolor;
in vec3 pos;
in float fogFactor;
out vec4 color;

uniform vec3 ambient;
uniform vec3 dirL;
uniform vec3 camPos;
uniform vec3 dirLcol;
uniform vec3 skyCol;
uniform vec3 skySunCol;
uniform float viewDist;

uniform sampler2D texRock;
uniform sampler2D texGrass;

void main() {
	vec3 light = clamp( dot(pnormal,dirL), 0,1) * dirLcol;
	vec3 color3 = pcolor * (ambient+light);

	//color3 *= 1-fogFactor*fogFactor;
	vec2 UV = pos.xz*0.05;

	float grassFactor = dot(pnormal,vec3(0,1,0));
	grassFactor*=grassFactor*grassFactor;
	float rockFactor = 1 - grassFactor;
	vec3 texColor = texture( texGrass, UV ).rgb*grassFactor + texture( texRock, UV ).rgb*rockFactor;
	//texColor=vec3(1);

	float fLight = dot(normalize(pos-camPos),dirL)*0.5f+0.5f;
	vec3 fogColor = skySunCol*fLight + skyCol*(1-fLight);

	vec3 finColor = texColor*color3*fogFactor + fogColor*(1-fogFactor);

	finColor = (finColor-0.13f)*1.14943f;
	color = vec4(finColor,1);
	//color = vec4(vec3(abs(fogFactor)),1);
	//color = vec4(abs(camPos),1);
}