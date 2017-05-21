#version 410 core

uniform sampler2D gTexDiffuse;

in vec2 passUV0;
in vec4 passColor;

out vec4 outColor;

void main ( void ) {
	vec4 diffuse = texture(gTexDiffuse, passUV0);

	outColor = passColor * diffuse;
}
