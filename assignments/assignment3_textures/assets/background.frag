#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _Texture;

void main(){
	vec2 UV_sample = UV; //this variable is which part of the texture it actually uses
	UV_sample *= 2;
	FragColor = texture(_Texture,UV_sample);
}