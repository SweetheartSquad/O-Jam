#version 150

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texFramebuffer;
uniform int distortionMode = 0;
uniform float time = 0;
uniform float mult = 0;

float rand1(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float rand2(vec2 p){return fract(cos(dot(p,vec2(23.14069263277926,2.665144142690225)))*123456.);}

vec2 distort(vec2 _uv){
	vec2 res = _uv;
	return res;
}

vec4 colour(vec4 _rgba){
	vec4 res = _rgba;
	if(distortionMode == 0){
		res.x = rand1(vec2(_rgba.y, _rgba.z));
		res.y = rand2(vec2(_rgba.x, _rgba.z));
		res.z = rand2(vec2(_rgba.x, _rgba.y));
	}else if(distortionMode == 1){
		res.x = rand2(vec2(_rgba.y, _rgba.z));
		res.y = rand2(vec2(_rgba.x, _rgba.z));
		res.z = rand1(vec2(_rgba.x, _rgba.y));
	}else{
		res.x = rand2(vec2(_rgba.y, _rgba.z));
		res.y = rand1(vec2(_rgba.x, _rgba.z));
		res.z = rand2(vec2(_rgba.x, _rgba.y));
	}
	return (res + _rgba)*mod(time, distortionMode);
}

void main() {
    outColor = colour(vec4(texture(texFramebuffer, distort(Texcoord)))) * mult + vec4(texture(texFramebuffer, Texcoord))*(1-mult);
    if(outColor.y > outColor.z){
    	discard;
    }
}
