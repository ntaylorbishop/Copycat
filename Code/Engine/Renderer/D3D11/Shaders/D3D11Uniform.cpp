#include "Engine/Renderer/D3D11/Shaders/D3D11Uniform.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BINDING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void D3D11Uniform::BindTextureToShader(int locInShader, void* data, uint currBindPoint) {

	UNREFERENCED_PARAMETER(locInShader);
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(currBindPoint);

	if (data == nullptr) {
		return;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Uniform::BindTextureBufferToShader(int locInShader, void* data, uint currBindPoint) {

	UNREFERENCED_PARAMETER(locInShader);
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(currBindPoint);

	if (data == nullptr) {
		return;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Uniform::BindTextureCubemapToShader(int locInShader, void* data, uint currBindPoint) {

	UNREFERENCED_PARAMETER(locInShader);
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(currBindPoint);

	if (data == nullptr) {
		return;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Uniform::Bind() const {

	if (!m_isDirty) {
		//return;
	}

	if (m_data == nullptr) {
		return;
	}

	switch (m_type) {
	case UNIFORM_FLOAT: {
		break;
	}
	case UNIFORM_VECTOR3: {
		break;
	}
	case UNIFORM_VECTOR2: {
		break;
	}
	case UNIFORM_VECTOR4: {
		break;
	}
	case UNIFORM_MAT4: {
		break;
	}
	case UNIFORM_INT: {
		break;
	}
	case UNIFORM_UINT: {
		break;
	}
	case UNIFORM_RGBA: {
		break;
	}
	case UNIFORM_BOOL: {

		break;
	}
	case UNIFORM_TEXTURE2D: {
		break;
	}
	case UNIFORM_TEXTUREBUFFER2D: {
		break;
	}
	case UNIFORM_CUBEMAP: {
		break;
	}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UTILS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC INLINE size_t D3D11Uniform::GetSizeOfType(eUniformType uniType) {

	switch (uniType) {
	case UNIFORM_FLOAT:				return sizeof(float);
	case UNIFORM_DOUBLE:			return sizeof(double);
	case UNIFORM_VECTOR3:			return sizeof(Vector3);
	case UNIFORM_VECTOR2:			return sizeof(Vector2);
	case UNIFORM_VECTOR4:			return sizeof(Vector4);
	case UNIFORM_MAT4:				return sizeof(Matrix4);
	case UNIFORM_INT:				return sizeof(int);
	case UNIFORM_UINT:				return sizeof(uint);
	case UNIFORM_SHORT:				return sizeof(short);
	case UNIFORM_RGBA:				return sizeof(RGBA);
	case UNIFORM_TEXTURE2D:			return sizeof(D3D11Texture*);
	case UNIFORM_TEXTUREBUFFER2D:	return sizeof(TextureBuffer*);
	case UNIFORM_CUBEMAP:			return sizeof(TextureCubemap*);
	case UNIFORM_BOOL:				return sizeof(bool);
	default:						return 0;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC String D3D11Uniform::SerializeType(eUniformType type) {

	switch (type) {
	case UNIFORM_FLOAT:				return "float";
	case UNIFORM_DOUBLE:			return "double";
	case UNIFORM_VECTOR3:			return "vec3";
	case UNIFORM_VECTOR2:			return "vec2";
	case UNIFORM_VECTOR4:			return "vec4";
	case UNIFORM_MAT4:				return "mat4";
	case UNIFORM_INT:				return "int";
	case UNIFORM_UINT:				return "uint";
	case UNIFORM_SHORT:				return "short";
	case UNIFORM_RGBA:				return "vec4";
	case UNIFORM_TEXTURE2D:			return "texture";
	case UNIFORM_TEXTUREBUFFER2D:	return "textureBuffer";
	case UNIFORM_CUBEMAP:			return "samplerCube";
	case UNIFORM_BOOL:				return "bool";
	default:						return "INVALID_UNIFORM_TYPE";
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC eUniformType D3D11Uniform::UnserializeType(const String& typeStr) {


	if (typeStr == "mat4") {
		return UNIFORM_MAT4;
	}
	else if (typeStr == "vec4") {
		return UNIFORM_VECTOR4;
	}
	else if (typeStr == "bool") {
		return UNIFORM_BOOL;
	}
	else if (typeStr == "uint") {
		return UNIFORM_UINT;
	}
	else if (typeStr == "vec2") {
		return UNIFORM_VECTOR2;
	}
	else if (typeStr == "texture2D") {
		return UNIFORM_TEXTURE2D;
	}
	else if (typeStr == "textureBuffer2D") {
		return UNIFORM_TEXTUREBUFFER2D;
	}
	else if (typeStr == "vec3") {
		return UNIFORM_VECTOR3;
	}
	else if (typeStr == "float") {
		return UNIFORM_FLOAT;
	}
	else if (typeStr == "int") {
		return UNIFORM_INT;
	}
	else if (typeStr == "cubemap") {
		return UNIFORM_CUBEMAP;
	}

	return UNIFORM_NUM_UNIFORM_TYPES;
}