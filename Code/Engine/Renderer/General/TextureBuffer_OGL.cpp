#include "Engine/Config/BuildConfig.hpp"

#ifdef GRAPHICS_API_OGL
#include "Engine/Renderer/General/TextureBuffer.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/General/Texture.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void GenerateTextureBuffer(int* textureID, int width, int height, eTextureFormat format) {

	GL_CHECK(glGenTextures(1, (GLuint*)textureID));

	GLenum bufferChannels	= GL_RGBA;
	GLenum bufferFormat		= GL_UNSIGNED_INT_8_8_8_8;
	GLenum internalFormat	= GL_RGBA8;

	switch (format) {
	case TEXTURE_FORMAT_D24S8: {
		bufferChannels	= GL_DEPTH_STENCIL;
		bufferFormat	= GL_UNSIGNED_INT_24_8;
		internalFormat	= GL_DEPTH24_STENCIL8;
		break;
	}
	case TEXTURE_FORMAT_D16: {
		bufferChannels	= GL_DEPTH_COMPONENT;
		bufferFormat	= GL_FLOAT;
		internalFormat	= GL_DEPTH_COMPONENT16;
	}
	case TEXTURE_FORMAT_R32F: {
		bufferChannels	= GL_RED;
		bufferFormat	= GL_FLOAT;
		internalFormat	= GL_R32F;
	}
	case TEXTURE_FORMAT_RGBA16F:
		bufferChannels	= GL_RGBA;
		bufferFormat	= GL_FLOAT;
		internalFormat	= GL_RGBA16F;
	}

	GL_CHECK(glActiveTexture(GL_TEXTURE0));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, *textureID));
	GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, bufferChannels, bufferFormat, NULL));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
}


//---------------------------------------------------------------------------------------------------------------------------
void GenerateSampler(int* samplerID) {

	glGenSamplers(1, (GLuint*)samplerID);
	GL_CHECK(glSamplerParameteri(*samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CHECK(glSamplerParameteri(*samplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CHECK(glSamplerParameteri(*samplerID, GL_TEXTURE_WRAP_S,		GL_CLAMP_TO_EDGE));
	GL_CHECK(glSamplerParameteri(*samplerID, GL_TEXTURE_WRAP_T,		GL_CLAMP_TO_EDGE));
}

//---------------------------------------------------------------------------------------------------------------------------
TextureBuffer::TextureBuffer(int width, int height, eTextureFormat format)
	: m_textureID(-1)
	, m_texelSize(width, height)
	, m_format(format)
{
	GenerateTextureBuffer(&m_textureID, width, height, format);
	GenerateSampler(&m_samplerID);
}


//---------------------------------------------------------------------------------------------------------------------------
TextureBuffer::~TextureBuffer() {
	GL_CHECK(glDeleteTextures(1, (GLuint*)&m_textureID));
	GL_CHECK(glDeleteSamplers(1, (GLuint*)&m_samplerID));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BINDING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void TextureBuffer::BindToShaderProgram(int locInShader, int bindPoint) {

	GL_CHECK(glActiveTexture(GL_TEXTURE0 + (GLuint)bindPoint));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, (GLuint)m_textureID));
	GL_CHECK(glBindSampler((GLuint)bindPoint, (GLuint)m_samplerID));
	GL_CHECK(glUniform1i((GLuint)locInShader, bindPoint));
}


//---------------------------------------------------------------------------------------------------------------------------
void TextureBuffer::GenerateMipmap() const {

	GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_textureID));
	GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
}


//---------------------------------------------------------------------------------------------------------------------------
void TextureBuffer::SetToMaxLOD() {

	GL_CHECK(glSamplerParameteri(m_samplerID, GL_TEXTURE_MIN_LOD, GL_TEXTURE_MAX_LEVEL));
	GL_CHECK(glSamplerParameteri(m_samplerID, GL_TEXTURE_MAX_LOD, GL_TEXTURE_MAX_LEVEL));
}


//---------------------------------------------------------------------------------------------------------------------------
void TextureBuffer::SetToMinLOD() {

	GL_CHECK(glSamplerParameteri(m_samplerID, GL_TEXTURE_MIN_LOD, GL_TEXTURE_BASE_LEVEL));
	GL_CHECK(glSamplerParameteri(m_samplerID, GL_TEXTURE_MAX_LOD, GL_TEXTURE_BASE_LEVEL));
}


//---------------------------------------------------------------------------------------------------------------------------
Texture* TextureBuffer::AsTexture() {

	Texture* conTex = new Texture();
	conTex->m_openglTextureID = m_textureID;
	conTex->m_texelSize = m_texelSize;
	conTex->m_samplerID = m_samplerID;
	conTex->m_name = "TEXTUREBUFFER";
	return conTex;
}

#endif