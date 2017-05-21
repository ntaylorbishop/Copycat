#pragma once

#include <cstdlib>
#include <string>
#include <map>
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/Sprites/SpriteSheet.hpp"
#include "Engine/Renderer/General/RGBA.hpp"

using namespace std;

class BitmapFont {
public:
	static BitmapFont* CreateOrGetFont(const std::string& bitmapFontName);
	static void DeinitBitmapFonts();

	AABB2 GetTexCoordsForGlyph(int glyphUnicode) const;

	void DrawText2D(const Vector2& startBottomLeft, const std::string& asciiText, float cellHeight, const RGBA& tint = RGBA(1.f, 1.f, 1.f, 1.f), const BitmapFont* font = nullptr);

	SpriteSheet* GetSpriteSheet() const;


private:
	char* m_squirrelFixedFont = "Data/Fonts/SquirrelFixedFont.png";

	BitmapFont(const string& bitmapFontName);


	static map< const char*, BitmapFont*, std::less<const char*>, UntrackedAllocator<std::pair<const char*, BitmapFont*>>> s_fontRegistry;
	SpriteSheet* m_spriteSheet;
};