#version 410 core

uniform mat4 gModel;
uniform mat4 gView;
uniform mat4 gProj;
uniform float gAge;

in vec3 inPosition;
in vec4 inColor;
in vec2 inUV0;
in vec3 inNormal;
in vec3 inTangent;
in vec3 inBitangent;
in vec4 inBoneWeights;
in ivec4 inBoneInds;

out vec4 passColor;
out vec2 passUV0;
out vec3 passPosition;
out vec3 passNormal;
out vec3 passTangent;
out vec3 passBitangent;

void main ( void ) {
	passColor = inColor;
	passUV0 = inUV0;

	vec4 pos = vec4(inPosition, 1.0f);
	
	passPosition = (vec4(inPosition, 1.f) * gModel).xyz;
	passNormal = (vec4(inNormal, 0.f) * gModel).xyz;
	passTangent = (vec4(inTangent, 0.f) * gModel).xyz;
	passBitangent = (vec4(inBitangent, 0.f) * gModel).xyz;
	
	passColor = vec4(vec3(1.f), 1.f);
	//passColor = inColor;
	//pos = pos * modelToWorld * gView * gProj;

	gl_Position = vec4(inPosition, 1.0f) * gModel * gView * gProj;
}