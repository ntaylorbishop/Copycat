#include "Engine/Config/BuildConfig.hpp"

#ifdef GRAPHICS_API_OGL
#include "Texture.hpp"
#include "Engine/General/Core/TheMemoryManager.hpp"
#include "Engine/General/Logger/TheLogger.hpp"

#define STBI_HEADER_FILE_ONLY

#pragma warning(push)
#include "ThirdParty/stb_image.c"
#pragma warning(pop)

STATIC TextureMap Texture::s_textureRegistry;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CREATE AND DEINIT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC Texture* Texture::CreateOrGetTexture(const String& imageFilePath, bool readInLinearSpace) {

	auto textureIt = s_textureRegistry.find(StringHash()(imageFilePath));

	if (textureIt != s_textureRegistry.end())
		return textureIt->second;
	else {
		Texture* texture = new(MEMORY_RENDERING) Texture(imageFilePath, readInLinearSpace);
		s_textureRegistry.insert(TextureMapPair(StringHash()(imageFilePath), texture));
		return texture;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Texture::DeinitTextures() {
	TextureMapIterator regIt;

	for (regIt = s_textureRegistry.begin(); regIt != s_textureRegistry.end(); ++regIt) {
		delete regIt->second;
		regIt->second = nullptr;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MODIFIERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void Texture::GenerateMipmap() {

	GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_openglTextureID));
	GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GL_CHECK(glSamplerParameteri((GLuint)m_samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BINDING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void Texture::BindToShaderProgram(int locInShader, int bindPoint) {

	GL_CHECK(glActiveTexture(GL_TEXTURE0 + (GLuint)bindPoint));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, (GLuint)m_openglTextureID));
	GL_CHECK(glBindSampler((GLuint)bindPoint, (GLuint)m_samplerID));
	GL_CHECK(glUniform1i((GLuint)locInShader, bindPoint));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INTERNAL STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void GenerateTexture(const String& imageFilePath, const IntVector2& texelSize, bool needsGammaCorrection) {

	IntVector2 textureSize = texelSize;
	int numComponents = 0;
	int numComponentsRequested = 0;

	unsigned char* imageData = stbi_load(imageFilePath.c_str(), &textureSize.x, &textureSize.y, &numComponents, numComponentsRequested);

	GLenum bufferFormat;
	GLenum internalFormat;
	if (numComponents == 4) {
		bufferFormat	= (needsGammaCorrection) ? (GL_RGBA) : (GL_RGBA);
		internalFormat	= (needsGammaCorrection) ? (GL_SRGB_ALPHA) : (GL_RGBA);
	}
	else if (numComponents == 3) {
		bufferFormat	= (needsGammaCorrection) ? (GL_RGB) : (GL_RGB);
		internalFormat	= (needsGammaCorrection) ? (GL_SRGB) : (GL_RGB);
	}
	else if (numComponents == 2) {
		bufferFormat = GL_RG;
		internalFormat = GL_RG;
		if (needsGammaCorrection) {
			DebuggerPrintf("WARNING: Trying to read in 2-channel texture in linear space.");
		}
	}
	else if (numComponents == 1) {
		bufferFormat = GL_RED;
		internalFormat = GL_RED;
		if (needsGammaCorrection) {
			DebuggerPrintf("WARNING: Trying to read in 1-channel texture in linear space.");
		}
	}
	else if (numComponents == 0) { //Invalid texture - spit out default values
		bufferFormat = GL_RGBA;
		internalFormat = GL_RGBA;
	}
	else {
		bufferFormat = 0;
		internalFormat = 0;
	}

	//GIVING ERROR - IGNORE
	//TODO: FIX
	GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
	GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, textureSize.x, textureSize.y, 0, bufferFormat, GL_UNSIGNED_BYTE, imageData));

	stbi_image_free(imageData);
}


//---------------------------------------------------------------------------------------------------------------------------
void GenerateDefaultSampler(GLuint* samplerID) {

	glGenSamplers(1, samplerID);
	GL_CHECK(glSamplerParameteri(*samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CHECK(glSamplerParameteri(*samplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CHECK(glSamplerParameteri(*samplerID, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GL_CHECK(glSamplerParameteri(*samplerID, GL_TEXTURE_WRAP_T, GL_REPEAT));
}


//---------------------------------------------------------------------------------------------------------------------------
Texture::Texture(const String& imageFilePath, bool readInLinearSpace)
	: m_openglTextureID(99)
	, m_texelSize(99, 99)
	, m_needsGammaCorrection(readInLinearSpace)
	, m_name(imageFilePath)
{
	GL_CHECK(glGenTextures(1, (GLuint*)&m_openglTextureID));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_openglTextureID));

	GenerateTexture(imageFilePath, m_texelSize, m_needsGammaCorrection);
	GenerateDefaultSampler((GLuint*)&m_samplerID);
}


//---------------------------------------------------------------------------------------------------------------------------
Texture::~Texture() {
	GL_CHECK(glDeleteTextures(1, &m_openglTextureID));
	GL_CHECK(glDeleteSamplers(1, (GLuint*)&m_samplerID));
}

#endif