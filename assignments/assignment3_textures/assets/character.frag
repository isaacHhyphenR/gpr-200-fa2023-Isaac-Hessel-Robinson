#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _Texture;
uniform float _Scale;
uniform float _Xpos;
uniform float _Ypos;


void main(){
	vec2 UV_sample = UV; //this variable is which part of the texture it actually uses
	UV_sample /= _Scale;
	vec2 characterPos = vec2(_Xpos,_Ypos);
	float d = distance(UV,characterPos);
	d = step(_Scale,d);
	FragColor = texture(_Texture,UV_sample);
	FragColor.a = d;
	FragColor = vec4(d,d,d,1.0);
}