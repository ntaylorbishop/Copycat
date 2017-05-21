#pragma once

#include <map>

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/SpriteRenderer/SpriteResource.hpp"
#include "Engine/Utils/GeneralUtils/XMLUtils.hpp"

class SpriteDatabase {
//STATIC MEMBERS
public:
	static void SpriteDatabase::RegisterSprite(const String& id, const String& texturePath);
	static const SpriteResource* GetSpriteResource(const String& id);
	static SpriteResource* EditSpriteResource(const String& id);
	static void SpriteDatabase::Destroy();

	static void SpriteDatabase::SaveToXML();
	static void SpriteDatabase::LoadFromXML();

private:

	static XMLNode CreateSpriteResourceNode(SpriteResource* resource);

	SpriteDatabase();
	~SpriteDatabase();

	void AddSpriteResource(const String& id, const String& texturePath);
	void AddSpriteResource(const String& id, SpriteResource* nResource);

	std::map<size_t, SpriteResource*> m_spriteResources;


	static SpriteDatabase* s_spriteDatabase;
};

typedef std::map<size_t, SpriteResource*>::iterator SpriteDatabaseMapIterator;
typedef std::pair<size_t, SpriteResource*> SpriteDatabaseMapPair;
