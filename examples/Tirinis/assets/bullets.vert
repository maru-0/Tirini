#version 410

layout(location = 0) in vec2 inPosition;

uniform vec4 color;
uniform float pointSize;
uniform vec2 translation;

out vec4 fragColor;

void main() {
    vec2 newPosition = translation + (vec2(inPosition.x*(3.0/4), inPosition.y) );
    gl_Position = vec4(newPosition, 0, 1);
    if(gl_Position.y > 1.0f - 2.0f/30 || gl_Position.y < -1.0f + 2.0f/30 || gl_Position.x > 0.3f || gl_Position.x < -0.9f){
        fragColor = vec4(0,0,0,0);
    }else{
        fragColor = color;
    }
    
}