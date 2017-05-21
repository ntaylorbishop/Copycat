#pragma once

#include <map>
#include <string>
#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Core/Memory/UntrackedAllocator.hpp"
#include "Engine/Renderer/General/Texture.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/AABB2.hpp"

typedef AABB2 SpriteCoords;
typedef Vector2 TexCoords;


using namespace std;
class SpriteSheet {
public:
	//CREATE DESTROY
	static SpriteSheet* CreateOrGetSpriteSheet(const IntVector2& tileSize, const char* imageFilePath);
	static void			DeinitSpriteSheets();

	//GETTERS
	AABB2		GetTexCoordsForSpriteCoords(const IntVector2& spriteCoords) const;
	AABB2		GetTexCoordsForSpriteIndex(int spriteIndex) const;
	Texture*	GetTexture()	const	{ return m_spriteSheetTexture;			}
	int			GetNumSprites() const	{ return m_tileSize.x * m_tileSize.y;	}
	IntVector2	GetSize() const			{ return m_tileSize;					}

private:
	SpriteSheet(const IntVector2& tileSize, const char* imageFilePath);

	static map<size_t, SpriteSheet*, less<size_t>, UntrackedAllocator<pair<size_t, SpriteSheet*>>> s_spriteSheetRegistry;

	Vector2		m_texCoordsPerTile		= Vector2::ZERO;
	IntVector2	m_tileSize				= IntVector2::ZERO;
	Texture*	m_spriteSheetTexture	= nullptr;
};