#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
layout(location = 2) in vec4 vColor;
out vec2 UV;
out vec4 Color;
void main(){
	gl_Position = vec4(vPos,1.0);
	UV = vUV;
	Color = vColor;
}