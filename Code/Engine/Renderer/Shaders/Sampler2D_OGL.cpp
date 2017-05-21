/*
#include "Engine/Config/BuildConfig.hpp"

#ifdef GRAPHICS_API_OGL
#include "Engine/Renderer/Shaders/Sampler2D.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/General/TextureBuffer.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
Sampler2D::Sampler2D()
	: m_samplerID(-1)
{ }


//---------------------------------------------------------------------------------------------------------------------------
Sampler2D::Sampler2D(eSamplerOptions options) {

	GL_CHECK(glGenSamplers(1, (GLuint*)&m_samplerID));

	GL_CHECK(glSamplerParameteri((GLuint)m_samplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CHECK(glSamplerParameteri((GLuint)m_samplerID, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GL_CHECK(glSamplerParameteri((GLuint)m_samplerID, GL_TEXTURE_WRAP_T, GL_REPEAT));

	if ((SAMPLER_OPTION_IS_MIPMAPPED & options) != 0) {
		GL_CHECK(glSamplerParameteri((GLuint)m_samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	}

	if ((SAMPLER_OPTIONS_IS_ANISOTROPIC_FILTERED & options) != 0) {
		//GLfloat fLargest;
		//GL_CHECK(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest));
		//GL_CHECK(glSamplerParameterf(m_samplerID, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest));
	}
}


//---------------------------------------------------------------------------------------------------------------------------
Sampler2D::~Sampler2D() { }


//---------------------------------------------------------------------------------------------------------------------------
void Sampler2D::Destroy() {
	glDeleteSamplers(1, (GLuint*)&m_samplerID);
}



//---------------------------------------------------------------------------------------------------------------------------
void Sampler2D::GenerateSamplerIDWithDefaultOptions() {

	GL_CHECK(glGenSamplers(1, (GLuint*)&m_samplerID));

	GL_CHECK(glSamplerParameteri((GLuint)m_samplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CHECK(glSamplerParameteri((GLuint)m_samplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CHECK(glSamplerParameteri((GLuint)m_samplerID, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GL_CHECK(glSamplerParameteri((GLuint)m_samplerID, GL_TEXTURE_WRAP_T, GL_REPEAT));
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INTERFACE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
uint Sampler2D::BindTextures(uint programID) const {

	uint currTextureBindPoint = 0;

	for (unsigned int i = 0; i < m_texturesToBind.size(); i++) {

		BindTextureToSampler(m_texturesToBind[i].second, currTextureBindPoint);
		BindToShaderProgram(programID, m_texturesToBind[i].first, currTextureBindPoint);
		currTextureBindPoint++;
	}

	for (unsigned int i = 0; i < m_textureBuffersToBind.size(); i++) {

		BindTextureToSampler(m_textureBuffersToBind[i].second, currTextureBindPoint);
		BindToShaderProgram(programID, m_textureBuffersToBind[i].first, currTextureBindPoint);
		currTextureBindPoint++;
	}

	return currTextureBindPoint;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GETTERS SETTERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
Texture* Sampler2D::GetTextureByName(const std::string& textureNameInShader) {

	for (unsigned int i = 0; i < m_texturesToBind.size(); i++) {
		if (textureNameInShader == m_texturesToBind[i].first) {
			return m_texturesToBind[i].second;
		}
	}

	ERROR_AND_DIE("ERROR: COULDN'T FIND TEXTURE IN SAMPLER2D WITH NAME");
}


//---------------------------------------------------------------------------------------------------------------------------
void Sampler2D::SetTextureByName(const std::string& textureNameInShader, Texture* newTexture) {


	for (unsigned int i = 0; i < m_texturesToBind.size(); i++) {
		if (textureNameInShader == m_texturesToBind[i].first) {
			m_texturesToBind[i].second = newTexture;
			return;
		}
	}

	ERROR_AND_DIE("ERROR: COULDN'T FIND TEXTURE IN SAMPLER2D WITH NAME");

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE MEMBER FUNCS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void Sampler2D::BindToShaderProgram(int programID, const String& name, int bindPoint) const {

	uint32_t loc = 0;
	GL_CHECK(loc = glGetUniformLocation((GLuint)programID, name.c_str()));
	GL_CHECK(glUniform1i((GLuint)loc, bindPoint));
}


//---------------------------------------------------------------------------------------------------------------------------
void Sampler2D::BindTextureToSampler(Texture* texture, unsigned int bindPoint) const {

	GL_CHECK(glActiveTexture(GL_TEXTURE0 + (GLuint)bindPoint));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, (GLuint)texture->GetTextureID()));
	GL_CHECK(glBindSampler((GLuint)bindPoint, (GLuint)m_samplerID));
}


//---------------------------------------------------------------------------------------------------------------------------
void Sampler2D::BindTextureToSampler(TextureBuffer* texture, unsigned int bindPoint) const {

	GL_CHECK(glActiveTexture(GL_TEXTURE0 + (GLuint)bindPoint));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, (GLuint)texture->GetTextureID()));
	GL_CHECK(glBindSampler((GLuint)bindPoint, (GLuint)m_samplerID));
}

#endif*/