#version 450
out vec4 FragColor;
in vec3 Color;
in vec2 UV;
uniform vec3 _Color;
uniform float _Brightness;
void main(){
	FragColor = vec4(UV.x, UV.y, 0.0, 1.0);
}