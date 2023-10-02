#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;

uniform float _Xpos;
uniform float _Ypos;
uniform float _Time;
uniform float _Speed;
uniform float _Scale;

out vec2 UV;
out vec2 characterPosition;


float characterRange = 0.5; //larger number makes the character move further
vec2 vCharacterPosition;

void main(){
	gl_Position = vec4(vPos,1.0);
	UV = vUV;
	
	//animates the character in a little bouncy bounce
	vCharacterPosition = vec2(_Xpos,_Ypos);
	vCharacterPosition.x += sin(_Time / (1 - _Speed + 0.01) *_Speed) * _Scale * characterRange;
	vCharacterPosition.y -= abs(sin(_Time / (1 - _Speed + 0.01) *_Speed) * _Scale * characterRange);

	characterPosition = vCharacterPosition;
}