#include "Engine/Renderer/Fonts/BitmapFont.hpp"
#include "Engine/General/Core/TheMemoryManager.hpp"

#define STATIC //do-nothing static indicator

STATIC map< const char*, BitmapFont*, std::less<const char*>, UntrackedAllocator<std::pair<const char*, BitmapFont*>>> BitmapFont::s_fontRegistry;

STATIC BitmapFont* BitmapFont::CreateOrGetFont(const std::string& bitmapFontName) {
	auto font = s_fontRegistry.find(bitmapFontName.c_str());

	if (font != s_fontRegistry.end())
		return font->second;
	else {
		BitmapFont* nFont = new(MEMORY_RENDERING) BitmapFont(bitmapFontName.c_str());

		s_fontRegistry.insert(pair<const char*, BitmapFont*>(bitmapFontName.c_str(), nFont));
		return nFont;
	}
}

STATIC void BitmapFont::DeinitBitmapFonts() {
	std::map<const char*, BitmapFont*>::iterator it;

	for (it = s_fontRegistry.begin(); it != s_fontRegistry.end(); it++) {
		delete it->second;
		it->second = nullptr;
	}
}

AABB2 BitmapFont::GetTexCoordsForGlyph(int glyphUnicode) const {
	return m_spriteSheet->GetTexCoordsForSpriteIndex(glyphUnicode);
}

void BitmapFont::DrawText2D(const Vector2& startBottomLeft, const std::string& asciiText, float cellHeight, const RGBA& tint, const BitmapFont* font) {
	
	UNREFERENCED_PARAMETER(tint);
	UNREFERENCED_PARAMETER(font);
	
	for (unsigned int i = 0; i < asciiText.size(); i++) {
		char charToDraw = asciiText[i];

		AABB2 quadToDrawOn = AABB2(startBottomLeft + Vector2((float)(i * cellHeight), 0.f), startBottomLeft + Vector2((float)(i * cellHeight + cellHeight), cellHeight));
		AABB2 texCoords = m_spriteSheet->GetTexCoordsForSpriteIndex((int)charToDraw);

		//BeirusRenderer::DrawTexturedAABB2FromSpriteSheet(m_material, tint, quadToDrawOn, texCoords);
		//FIXME: PUSH THESE BACK TO A NEW VERT LIST AND RENDER ALL AT ONCE
	}
}

BitmapFont::BitmapFont(const string& bitmapFontName) {
	UNREFERENCED_PARAMETER(bitmapFontName);
	m_spriteSheet = SpriteSheet::CreateOrGetSpriteSheet(IntVector2(16, 16), m_squirrelFixedFont);
}

SpriteSheet* BitmapFont::GetSpriteSheet() const {
	return m_spriteSheet;
}