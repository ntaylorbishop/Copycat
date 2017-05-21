#version 410 core

uniform mat4 gProj;
uniform mat4 gView;

in vec2 inPosition;
in vec4 inColor;
in vec2 inUV0;

out vec2 passUV0;
out vec4 passColor;

void main( void ) {
	passUV0 = vec2(inUV0.x, 1.f - inUV0.y);
	passColor = inColor;

	vec4 pos = vec4(inPosition, 0.0f, 1.0f);
	pos = pos * gView * gProj;

	gl_Position = pos;
}
