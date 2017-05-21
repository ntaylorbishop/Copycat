#version 410 core

uniform sampler2D gTexDiffuse;

in vec2 passUV0;

out vec4 outColor;

float vx_offset = 1.f;

void main() { 
  vec2 uv = passUV0;
  vec4 c = texture2D(gTexDiffuse, uv);
  
  outColor = c;
}