#version 140
in vec3 pnormal;
in vec3 onormal;
in vec3 pcolor;
in vec2 puv;
in vec3 pos;
in float fogFactor;
out vec4 color;

uniform vec3 ambient;
uniform vec3 view;

uniform vec3 dirL;
uniform vec3 dirLcol;
uniform vec3 specCol;
uniform vec3 dirLspec;
uniform vec3 pointL;
uniform vec3 pointCol;
uniform float pointAtt;
uniform float shiny;

uniform vec3 mCol;
uniform vec3 fogColor;
uniform vec3 camPos;
uniform float viewDist;
uniform int hasTexture;

uniform sampler2D tex;

void main() {
	vec3 light = clamp( dot(pnormal,dirL), 0,1) * dirLcol;
	vec3 color3 = pcolor * (ambient+light);
	float spec = pow( max(0,dot(view,reflect(dirL,pnormal))),shiny);
	if (dot(pnormal,dirL)<0)
		spec=0;

	vec3 lightDir = pointL-pos;
	float lightDist = length(lightDir);
	float NdotL = max(dot(onormal,normalize(lightDir)),0.0);
	color3 += pointCol*NdotL/(pointAtt*lightDist*lightDist);

	//color3 *= 1-fogFactor*fogFactor;
	//color3 *= fogFactor;

	vec3 texColor;
	if (hasTexture==1)
		texColor = texture( tex, vec2(puv.x,1-puv.y) ).rgb;
	else
		texColor = mCol;
	//vec3 texColor=vec3(1);

	vec3 finColor = (texColor*color3 + spec*specCol)*fogFactor + fogColor*(1-fogFactor);

	color = vec4(finColor,1);
	//color = vec4(spec,0,0,1);
	//color = vec4(normalize(pnormal),1);
}