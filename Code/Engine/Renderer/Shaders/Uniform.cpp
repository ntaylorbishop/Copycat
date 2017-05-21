#include "Engine/Renderer/Shaders/Uniform.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/General/Texture.hpp"
#include "Engine/Renderer/General/TextureBuffer.hpp"
#include "Engine/Renderer/General/TextureCubemap.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BINDING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void BindTextureToShader(int locInShader, void* data, uint currBindPoint) {

	if (data == nullptr) {
		return;
	}

	Texture* textureToBind = (Texture*)data;
	textureToBind->BindToShaderProgram(locInShader, currBindPoint);
}


//---------------------------------------------------------------------------------------------------------------------------
void BindTextureBufferToShader(int locInShader, void* data, uint currBindPoint) {

	if (data == nullptr) {
		return;
	}

	TextureBuffer* textureToBind = (TextureBuffer*)data;
	textureToBind->BindToShaderProgram(locInShader, currBindPoint);
}


//---------------------------------------------------------------------------------------------------------------------------
void BindTextureCubemapToShader(int locInShader, void* data, uint currBindPoint) {

	if (data == nullptr) {
		return;
	}

	TextureCubemap* textureToBind = (TextureCubemap*)data;
	textureToBind->BindToShaderProgram(locInShader, currBindPoint);
}


//---------------------------------------------------------------------------------------------------------------------------
void Uniform::Bind() const {

	if (!m_isDirty) {
		//return;
	}

	if (m_data == nullptr) {
		return;
	}

	switch (m_type) {
	case UNIFORM_FLOAT: {
		GL_CHECK(glUniform1fv((GLuint)m_locInShader, (GLsizei)m_count, (GLfloat*)m_data));
		break;
	}
	case UNIFORM_VECTOR3: {
		GL_CHECK(glUniform3fv((GLuint)m_locInShader, (GLsizei)m_count, (GLfloat*)m_data));
		break;
	}
	case UNIFORM_VECTOR2: {
		GL_CHECK(glUniform2fv((GLuint)m_locInShader, (GLsizei)m_count, (GLfloat*)m_data));
		break;
	}
	case UNIFORM_VECTOR4: {
		GL_CHECK(glUniform4fv((GLuint)m_locInShader, (GLsizei)m_count, (GLfloat*)m_data));
		break;
	}
	case UNIFORM_MAT4: {
		GL_CHECK(glUniformMatrix4fv((GLuint)m_locInShader, (GLsizei)m_count, GL_TRUE, (GLfloat*)m_data));
		break;
	}
	case UNIFORM_INT: {
		GL_CHECK(glUniform1iv((GLuint)m_locInShader, (GLsizei)m_count, (GLint*)m_data));
		break;
	}
	case UNIFORM_UINT: {
		GL_CHECK(glUniform1uiv((GLuint)m_locInShader, (GLsizei)m_count, (GLuint*)m_data));
		break;
	}
	case UNIFORM_RGBA: {
		GL_CHECK(glUniform4fv((GLuint)m_locInShader, (GLsizei)m_count, (GLfloat*)m_data));
		break;
	}
	case UNIFORM_BOOL: {

		bool* valPtr = (bool*)m_data;
		bool val = *valPtr;
		GLuint valCorrect = (GLuint)val;

		GL_CHECK(glUniform1ui((GLuint)m_locInShader, valCorrect));
		break;
	}
	case UNIFORM_TEXTURE2D: {
		BindTextureToShader(m_locInShader, m_data, m_bindPoint);
		break;
	}
	case UNIFORM_TEXTUREBUFFER2D: {
		BindTextureBufferToShader(m_locInShader, m_data, m_bindPoint);
		break;
	}
	case UNIFORM_CUBEMAP: {
		BindTextureCubemapToShader(m_locInShader, m_data, m_bindPoint);
		break;
	}
	}

	m_isDirty = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UTILS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC INLINE size_t Uniform::GetSizeOfType(eUniformType uniType) {

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
	case UNIFORM_TEXTURE2D:			return sizeof(Texture*);
	case UNIFORM_TEXTUREBUFFER2D:	return sizeof(TextureBuffer*);
	case UNIFORM_CUBEMAP:			return sizeof(TextureCubemap*);
	case UNIFORM_BOOL:				return sizeof(bool);
	default:						return 0;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC String Uniform::SerializeType(eUniformType type) {

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
STATIC eUniformType Uniform::UnserializeType(const String& typeStr) {


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