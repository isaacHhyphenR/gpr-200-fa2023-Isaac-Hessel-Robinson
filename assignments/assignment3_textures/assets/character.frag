#version 450
out vec4 FragColor;
in vec2 UV;
in vec2 characterPosition;

uniform sampler2D _Texture;
uniform float _Scale;


void main(){
	vec2 UV_sample = UV; //this variable is which part of the texture it actually uses
	//Maps the screen UV to the texture UV
	UV_sample -= characterPosition;
	UV_sample /= _Scale;
	UV_sample += vec2(0.5,0.5);
	//loads in the texture
	FragColor = texture(_Texture,UV_sample);
	//determines the distance between the current UV & the character's center
	float xd = distance(UV.x,characterPosition.x);
	float yd = distance(UV.y,characterPosition.y);
	float d = max(xd,yd);
	//If that distance is larger than the character size, pixels are all transparent
	d = step(_Scale/2,d);
	FragColor.a *= 1-d;
}