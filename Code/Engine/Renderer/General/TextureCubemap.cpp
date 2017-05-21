#include "Engine/Renderer/General/TextureCubemap.hpp"
#include "Engine/Renderer/General/Texture.hpp"
#include "Engine/Renderer/OpenGL/OpenGLUtils.hpp"
#include "Engine/Renderer/General/TextureBuffer.hpp"

#define STBI_HEADER_FILE_ONLY
#define STB_IMAGE_WRITE_IMPLEMENTATION

#pragma warning(push)
#include "ThirdParty/stb_image.c"
#include "ThirdParty/stb_image_write.h"
#pragma warning(pop)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
static void InitializeSampler(uint* samplerID) {

	GL_CHECK(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));


	GL_CHECK(glGenSamplers(1, samplerID));
	GL_CHECK(glSamplerParameteri(*samplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CHECK(glSamplerParameteri(*samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CHECK(glSamplerParameteri(*samplerID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_CHECK(glSamplerParameteri(*samplerID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GL_CHECK(glSamplerParameteri(*samplerID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

}


//---------------------------------------------------------------------------------------------------------------------------
static IntVector2 LoadTextureFromFile(GLenum whichSide, const char* texturePath) {

	IntVector2 textureSize;
	int numComponents = 0;
	unsigned char* imageData = stbi_load(texturePath, &textureSize.x, &textureSize.y, &numComponents, 0);

	//ASSERT_OR_DIE(numComponents == 4, "ERROR: Cubemap must be in RGBA format.");

	if (numComponents == 3) {
		GL_CHECK(glTexImage2D(whichSide, 0, GL_RGB, textureSize.x, textureSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData));
	}
	else if (numComponents == 4) {
		GL_CHECK(glTexImage2D(whichSide, 0, GL_RGBA, textureSize.x, textureSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData));
	}
	else {
		ERROR_AND_DIE("SKYBOX NOT IN CORRECT FORMAT");
	}
	stbi_image_free(imageData);

	return textureSize;
}


//---------------------------------------------------------------------------------------------------------------------------
TextureCubemap::TextureCubemap(const IntVector2& size, eTextureFormat format)
	: m_textureSize(size)
	#if defined(_DEBUG)
	, m_format(format)
	#endif
{

	GL_CHECK(glGenTextures(1, &m_textureID));
	GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID));

	GLenum bufferChannels = GL_RGBA;
	GLenum bufferFormat = GL_UNSIGNED_INT_8_8_8_8;
	GLenum internalFormat = GL_RGBA8;

	switch (format) {
	case TEXTURE_FORMAT_D24S8: {
		bufferChannels = GL_DEPTH_STENCIL;
		bufferFormat = GL_UNSIGNED_INT_24_8;
		internalFormat = GL_DEPTH24_STENCIL8;
		break;
	}
	case TEXTURE_FORMAT_D16: {
		bufferChannels = GL_DEPTH_COMPONENT;
		bufferFormat = GL_FLOAT;
		internalFormat = GL_DEPTH_COMPONENT32;

		break;
	}
	case TEXTURE_FORMAT_R32F:
		bufferChannels = GL_RED;
		bufferFormat = GL_FLOAT;
		internalFormat = GL_R32F;
		break;
	}

	GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalFormat, size.x, size.y, 0, bufferChannels, bufferFormat, NULL));
	GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalFormat, size.x, size.y, 0, bufferChannels, bufferFormat, NULL));
	GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalFormat, size.x, size.y, 0, bufferChannels, bufferFormat, NULL));
	GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalFormat, size.x, size.y, 0, bufferChannels, bufferFormat, NULL));
	GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalFormat, size.x, size.y, 0, bufferChannels, bufferFormat, NULL));
	GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalFormat, size.x, size.y, 0, bufferChannels, bufferFormat, NULL));


	//GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_LINEAR));
	//GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_LINEAR));
	//GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_LINEAR));
	//GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	//GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

	InitializeSampler((GLuint*)&m_samplerID);

	if (format == TEXTURE_FORMAT_D16) {
		GL_CHECK(glSamplerParameteri(m_samplerID, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE));
		GL_CHECK(glSamplerParameteri(m_samplerID, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
	}
}


//---------------------------------------------------------------------------------------------------------------------------
TextureCubemap::TextureCubemap(	const char* xPositive, const char* xNegative,
								const char* yPositive, const char* yNegative,
								const char* zPositive, const char* zNegative)
{

	GL_CHECK(glGenTextures(1, &m_textureID));
	GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID));
	GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

	m_textureSize = LoadTextureFromFile(GL_TEXTURE_CUBE_MAP_POSITIVE_X, xPositive); //Only care about the size of the first one we loaded
	LoadTextureFromFile(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, xNegative);
	LoadTextureFromFile(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, yPositive);
	LoadTextureFromFile(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, yNegative);
	LoadTextureFromFile(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, zPositive);
	LoadTextureFromFile(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, zNegative);

	GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
/*
	GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0));
	GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0));*/

	InitializeSampler(&m_samplerID);
}


//---------------------------------------------------------------------------------------------------------------------------
TextureCubemap::~TextureCubemap() {
	GL_CHECK(glDeleteTextures(1, &m_textureID));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MODIFIERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void TextureCubemap::GenerateMipmap() const {

/*
	GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID));
	GL_CHECK(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
	GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GL_CHECK(glSamplerParameteri((GLuint)m_samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));*/
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BINDING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void TextureCubemap::BindToShaderProgram(int locInShader, int bindPoint) {

	GL_CHECK(glActiveTexture(GL_TEXTURE0 + (GLuint)bindPoint));
	GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint)m_textureID));
	GL_CHECK(glBindSampler((GLuint)bindPoint, (GLuint)m_samplerID));
	GL_CHECK(glUniform1i((GLuint)locInShader, bindPoint));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//WRITING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void TextureCubemap::WriteToFile(const String& filepath, const String& namePrefix) {

	int comp = 4;
	int stride = 0;

	GLuint* imgData = new GLuint[m_textureSize.x * m_textureSize.y * sizeof(byte) * 4];
	glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint)m_textureID);
	glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	stbi_write_png((filepath + namePrefix + "_posX" + ".png").c_str(), m_textureSize.x, m_textureSize.y, comp, (void*)imgData, stride);
	delete imgData;

	imgData = new GLuint[m_textureSize.x * m_textureSize.y * sizeof(byte) * 4];
	glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint)m_textureID);
	glGetTexImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	stbi_write_png((filepath + namePrefix + "_negX" + ".png").c_str(), m_textureSize.x, m_textureSize.y, comp, (void*)imgData, stride);
	delete imgData;

	imgData = new GLuint[m_textureSize.x * m_textureSize.y * sizeof(byte) * 4];
	glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint)m_textureID);
	glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	stbi_write_png((filepath + namePrefix + "_posY" + ".png").c_str(), m_textureSize.x, m_textureSize.y, comp, (void*)imgData, stride);
	delete imgData;

	imgData = new GLuint[m_textureSize.x * m_textureSize.y * sizeof(byte) * 4];
	glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint)m_textureID);
	glGetTexImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	stbi_write_png((filepath + namePrefix + "_negY" + ".png").c_str(), m_textureSize.x, m_textureSize.y, comp, (void*)imgData, stride);
	delete imgData;

	imgData = new GLuint[m_textureSize.x * m_textureSize.y * sizeof(byte) * 4];
	glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint)m_textureID);
	glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	stbi_write_png((filepath + namePrefix + "_posZ" + ".png").c_str(), m_textureSize.x, m_textureSize.y, comp, (void*)imgData, stride);
	delete imgData;

	imgData = new GLuint[m_textureSize.x * m_textureSize.y * sizeof(byte) * 4];
	glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint)m_textureID);
	glGetTexImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	stbi_write_png((filepath + namePrefix + "_negZ" + ".png").c_str(), m_textureSize.x, m_textureSize.y, comp, (void*)imgData, stride);
	delete imgData;
}