#version 450
out vec4 FragColor;
in vec3 Color;
in vec2 UV;
uniform float _Time;
uniform vec2 _Resolution;

uniform vec3 _SkyColorTop;
uniform vec3 _SkyColorBottom;
uniform vec3 _SunRiseColor;
uniform vec3 _SunSetColor;
uniform float _SunSpeed;
uniform vec3 _HillColor;


void main(){
    float sunStartY = 0.5f; //higher makes the sun be further up
    float sunRange = 0.5; //higher number makes the sun go further off screen
    float sunX = 0.5; //the horitzontal position of the sun
    float sunSize = 0.3;

    float backgroundLight = 0.5; //larger numbers make the sky turn black when the sun is futher down
    
    float hillFrequency = 12.0; //higher means more hills
    float hillHeight = 0.2; //how far up the screen the hills start
    float hillGrade = 0.05; //higher means steeper hills
    
    float smoothing = 0.01; //larger means longer gradients
    vec2 _UV = UV;

    //makes things not get horizontally stretched
    float aspectRatio = _Resolution.x/_Resolution.y;
    _UV.x *= aspectRatio;
    
    //////////////////Sun
    float sunY = sin(_Time / (1- (_SunSpeed +0.1))) * sunRange;
    sunY += sunStartY;
    vec2 sunPos = vec2(sunX * aspectRatio, sunY);
    
    float s = distance(_UV, sunPos);
    s = smoothstep(sunSize-smoothing, sunSize+smoothing, s);
    
    vec3 sunColor = mix(_SunSetColor, _SunRiseColor, sunY);
    
    //////////////////Hills
    float h = hillHeight + sin(_UV.x * hillFrequency) * hillGrade;
    h = smoothstep (h-smoothing, h + smoothing, _UV.y);
    
    vec3 foregroundColor = _HillColor * sunY;
    
    //////////////////Sky
    vec3 skyColor = mix(_SkyColorBottom, _SkyColorTop, _UV.y);
    skyColor *= sunY + backgroundLight; //gets darker as the sun sets
    
    
    //////////////////Final Color
    vec3 color = mix(sunColor, skyColor, s);
    color = mix(foregroundColor, color, h);
    FragColor = vec4(color,1.0);
}