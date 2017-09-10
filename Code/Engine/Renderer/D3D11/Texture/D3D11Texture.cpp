#include "Engine/Renderer/D3D11/Texture/D3D11Texture.hpp"

STATIC TextureDatabase* TextureDatabase::s_texDB = nullptr;


//---------------------------------------------------------------------------------------------------------------------------
STATIC D3D11Texture* TextureDatabase::CreateOrGetTexture(const String& filePath) {

	std::map<String, D3D11Texture*>::iterator texIt = s_texDB->m_textures.find(filePath);

	if (texIt != s_texDB->m_textures.end()) {
		return texIt->second;
	}
	else {
		return nullptr;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
TextureDatabase::TextureDatabase() {

}


//---------------------------------------------------------------------------------------------------------------------------
TextureDatabase::~TextureDatabase() {

	std::map<String, D3D11Texture*>::iterator texIt = s_texDB->m_textures.begin();

	for (texIt; texIt != s_texDB->m_textures.end(); ++texIt) {

		D3D11Texture* texToDelete = texIt->second;
		delete texToDelete;
	}
}