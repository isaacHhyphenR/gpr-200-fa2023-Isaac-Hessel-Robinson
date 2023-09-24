#version 450
out vec4 FragColor;
in vec3 Color;
in vec2 UV;
uniform vec3 _Color;
uniform float _Brightness;
uniform float _Time;
uniform vec2 _Resolution;

void main(){
    
    float sunSpeed = 0.5;
    float sunStartY = 0.5; //higher number makes the sun be higher up
    float sunX = 0.5; //the horitzontal position of the sun
    float sunSize = 0.3;
    vec3 sunStartColor = vec3(0.95,0.6,0.05); //the sun's color at the top of the screen
    vec3 sunEndColor = vec3(1.0,0.0,0.2); //the sun's color at the bottom of the screen
    
    vec3 skyColorTop = vec3(0.9, 0.4, 0.7); //The base background color when the sun is fully there
    vec3 skyColorBottom = vec3(0.6, 0.15, 0.2); //The base background color when the sun is fully there
    float backgroundLight = 0.5; //larger numbers make the sky turn black when the sun is futher down
    
    vec3 hillColor = vec3(0.1, 0.6, 0.25);
    float hillFrequency = 12.0; //higher means more hills
    float hillHeight = 0.2; //how far up the screen the hills start
    float hillGrade = 0.05; //higher means steeper hills
    
    float smoothing = 0.01; //larger means longer gradients
    vec2 _UV = UV;

    //makes things not get horizontally stretched
    float aspectRatio = _Resolution.x/_Resolution.y;
    _UV.x *= aspectRatio;
    
    //////////////////Sun
    float sunY = sin(_Time - sunStartY) * sunSpeed; //the minus is to start it in a better position on frame 1
    sunY += sunStartY;
    vec2 sunPos = vec2(sunX * aspectRatio, sunY);
    
    float s = distance(_UV, sunPos);
    s = smoothstep(sunSize-smoothing, sunSize+smoothing, s);
    
    vec3 sunColor = mix(sunEndColor, sunStartColor, sunY);
    
    //////////////////Hills
    float h = hillHeight + sin(_UV.x * hillFrequency) * hillGrade;
    h = smoothstep (h-smoothing, h + smoothing, _UV.y);
    
    vec3 foregroundColor = hillColor * sunY;
    
    //////////////////Sky
    vec3 skyColor = mix(skyColorBottom, skyColorTop, _UV.y);
    skyColor *= sunY + backgroundLight; //gets darker as the sun sets
    
    
    //////////////////Final Color
    vec3 color = mix(sunColor, skyColor, s);
    color = mix(foregroundColor, color, h);
    FragColor = vec4(color,1.0);
}