#version 410

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

uniform vec2 translation;
uniform float pointSize;

out vec4 fragColor;

void main() {
  gl_PointSize = pointSize;
  gl_Position = vec4(inPosition.xy + translation, 0, 1);
  if(gl_Position.y > 1.0f - 2.0f/30 || gl_Position.y < -1.0f + 2.0f/30){
    fragColor = vec4(0,0,0,0);
  }else{
    fragColor = vec4(inColor, 1);
  }
}