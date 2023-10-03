#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _Clouds;
uniform sampler2D _Noise;
uniform sampler2D _Tree;
uniform float _Scale;
uniform float _Time;
uniform float _ForegroundOpacity;


void main(){
	vec2 UV_sample = UV; //this variable is which part of the texture it actually uses
	UV_sample /= _Scale;

	//Adds distortion
	float noise = texture(_Noise,UV_sample).r;
	UV_sample += noise * sin(_Time) * 0.1f;
	
	vec4 cloudColor = texture(_Clouds,UV_sample);
	vec4 treeColor = texture(_Tree,UV_sample);

	FragColor = vec4(mix(cloudColor.rgb,treeColor.rgb,treeColor.a * _ForegroundOpacity),1);
}