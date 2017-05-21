#define STATIC

#include "Engine/Renderer/Sprites/SpriteSheet.hpp"
#include "Engine/General/Core/TheMemoryManager.hpp"
#include "Engine/Utils/GeneralUtils.hpp"

STATIC map<size_t, SpriteSheet*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, SpriteSheet*>>> SpriteSheet::s_spriteSheetRegistry;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CREATE DESTROY
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC SpriteSheet* SpriteSheet::CreateOrGetSpriteSheet(const IntVector2& tileSize, const char* imageFilePath) {

	size_t nameHash = HashCString(imageFilePath);

	auto spriteSheetIt = s_spriteSheetRegistry.find(nameHash);

	if (spriteSheetIt != s_spriteSheetRegistry.end())
		return spriteSheetIt->second;
	else {
		SpriteSheet* spriteSheet = new(MEMORY_RENDERING) SpriteSheet(tileSize, imageFilePath);
		s_spriteSheetRegistry.insert(std::pair<size_t, SpriteSheet*>(nameHash, spriteSheet));
		return spriteSheet;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void SpriteSheet::DeinitSpriteSheets() {

	std::map<size_t, SpriteSheet*>::iterator it;

	for (it = s_spriteSheetRegistry.begin(); it != s_spriteSheetRegistry.end(); ++it) {
		delete it->second;
		it->second = nullptr;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GETTERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
AABB2 SpriteSheet::GetTexCoordsForSpriteCoords(const IntVector2& spriteCoords) const {

	AABB2 texCoords;
	texCoords.mins.x = m_texCoordsPerTile.x * (float)spriteCoords.x;
	texCoords.maxs.x = m_texCoordsPerTile.x * (float)spriteCoords.x + m_texCoordsPerTile.x;
	texCoords.mins.y = m_texCoordsPerTile.y * (float)spriteCoords.y;
	texCoords.maxs.y = m_texCoordsPerTile.y * (float)spriteCoords.y + m_texCoordsPerTile.y;

	return texCoords;
}


//---------------------------------------------------------------------------------------------------------------------------
AABB2 SpriteSheet::GetTexCoordsForSpriteIndex(int spriteIndex) const {

	int x = spriteIndex % m_tileSize.x;
	int y = spriteIndex / m_tileSize.y;

	AABB2 texCoords;
	texCoords.mins.x = m_texCoordsPerTile.x * (float)x;
	texCoords.maxs.x = m_texCoordsPerTile.x * (float)x + m_texCoordsPerTile.x;
	texCoords.mins.y = m_texCoordsPerTile.y * (float)y;
	texCoords.maxs.y = m_texCoordsPerTile.y * (float)y + m_texCoordsPerTile.y;

	return texCoords;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
SpriteSheet::SpriteSheet(const IntVector2& tileSize, const char* imageFilePath)
	: m_tileSize(0, 0)
	, m_spriteSheetTexture(nullptr)
	, m_texCoordsPerTile(0.f, 0.f)
{
	m_tileSize				= tileSize;
	m_spriteSheetTexture	= Texture::CreateOrGetTexture(imageFilePath);
	m_texCoordsPerTile		= Vector2(1.f / (float)m_tileSize.x, 1.f / (float)m_tileSize.y);
}