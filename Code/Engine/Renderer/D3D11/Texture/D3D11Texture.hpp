#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

class D3D11Texture {
public:
	D3D11Texture() { }
	~D3D11Texture() { }

private:


	friend class TextureDatabase;
};


//---------------------------------------------------------------------------------------------------------------------------
class TextureDatabase {
public:
	static D3D11Texture* CreateOrGetTexture(const String& filePathOrName);


private:
	TextureDatabase();
	~TextureDatabase();

	std::map<String, D3D11Texture*> m_textures;

	static TextureDatabase* s_texDB;
};
