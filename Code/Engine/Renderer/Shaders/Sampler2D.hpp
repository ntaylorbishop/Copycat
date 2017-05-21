/*
#pragma once

#include <map>

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/General/Texture.hpp"
#include "Engine/Renderer/OpenGL/OpenGLEnums.hpp"

class ShaderProgram;
class TextureBuffer;

enum eSamplerOptions : uint8_t {
	SAMPLER_OPTION_IS_MIPMAPPED = 1 << 0,
	SAMPLER_OPTIONS_IS_ANISOTROPIC_FILTERED = 1 << 1
};

class Sampler2D {
public:
	//STRUCTORS
	Sampler2D();
	Sampler2D(eSamplerOptions options);
	~Sampler2D();
	void Destroy();

	void GenerateSamplerIDWithDefaultOptions();

	//INTERFACE
	inline void AddTexture(const std::string& nameInShader, Texture* texture);
	inline void AddTexture(const std::string& nameInShader, TextureBuffer* texture);
	uint BindTextures(uint programID) const;
	void ClearTextures() { m_texturesToBind.clear(); }

	//GETTERS SETTERS
	Texture* GetTextureByName(const std::string& textureNameInShader);
	int GetSamplerID() const { return m_samplerID; }
	void SetTextureByName(const std::string& textureNameInShader, Texture* newTexture);

private:

	void BindToShaderProgram(int programID, const String& name, int val) const;
	void BindTextureToSampler(Texture* texture, unsigned int bindPoint) const;
	void BindTextureToSampler(TextureBuffer* texture, unsigned int bindPoint) const;


	int m_samplerID;

	std::vector < std::pair<std::string, Texture*>> m_texturesToBind;
	std::vector < std::pair<std::string, TextureBuffer*>> m_textureBuffersToBind;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void Sampler2D::AddTexture(const std::string& nameInShader, Texture* texture) { 
	m_texturesToBind.push_back(std::pair<std::string, Texture*>(nameInShader.c_str(), texture)); 
}


//---------------------------------------------------------------------------------------------------------------------------
void Sampler2D::AddTexture(const std::string& nameInShader, TextureBuffer* texture) { 
	m_textureBuffersToBind.push_back(std::pair<std::string, TextureBuffer*>(nameInShader.c_str(), texture)); 
}*/