#pragma once

#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Utils/StringUtils.hpp"
#include "Engine/Utils/FileUtils/FileUtils.hpp"
#include "Engine/Renderer/General/Texture.hpp"
#include "Engine/Renderer/Fonts/Glyph.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"

const char* const DEFAULT_ENGINE_FONT = "Tahoma";

class Font {
public:
	static Font* CreateOrGetFont(std::string fontName);
	static void DeinitFonts();

	//DRAW
	static void DrawText2DWithDefaultFont(const Vector2& position, const String& strToDraw, float scale, const RGBA& color = RGBA::WHITE);
	static void DrawText2D(const String& textName, const Vector2& position, const String& strToDraw, float scale, const RGBA& color = RGBA::WHITE);
	void DrawText2D(const Vector2& position, const std::string& str, float scale, const RGBA& color = RGBA::WHITE);


	//GETTERS
	float GetXAdvanceOfGlyph(unsigned char glyphChar, float scale);
	Glyph* GetGlyphForCharacter(unsigned char glyphChar);
	float GetLineHeight() { return m_lineHeight; }
	float GetLineBase() const { return m_lineBase; }
	float GetLineBaseToLineHeight() const { return m_lineHeight - m_lineBase; }
	float GetWidthOfString(std::string str, float scale);
	float GetHeightOfString(const String& str, float scale);

	static float GetLineHeight(const char* fontName, float scale);
	static float GetDefaultFontLineHeight(float scale);

private:
	//STRUCTORS
	Font(std::string fontName);
	~Font();

	//LOADING
	void LoadGlyphsFromFile();
	void ParseBufferIntoGlpyhs(std::vector<unsigned char> buffer);


private:
	static map<const char*, Font*, std::less<const char*>, UntrackedAllocator<std::pair<const char*, Font*>>> s_fontRegistry;
	
	std::string m_fontName;
	Material* m_material;
	std::map<uchar, Glyph*> m_glyphs;
	Vector2 m_textureScale;
	float m_lineHeight;
	float m_lineBase;
};