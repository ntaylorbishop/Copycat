#include "Engine/Renderer/Fonts/Font.hpp"
#include "Engine/General/Core/TheMemoryManager.hpp"
#include "Engine/Renderer/Material/Material.hpp"
#include "Engine/Renderer/Mesh/Mesh.hpp"
#include "Engine/Renderer/Mesh/MeshRenderer.hpp"

STATIC map<const char*, Font*, std::less<const char*>, UntrackedAllocator<std::pair<const char*, Font*>>> Font::s_fontRegistry;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STATIC FACTORY CREATION AND DESTROY
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC Font* Font::CreateOrGetFont(std::string fontName) {

	auto font = s_fontRegistry.find(fontName.c_str());

	if (font != s_fontRegistry.end())
		return font->second;
	else {
		Font* nFont = new(MEMORY_RENDERING) Font(fontName);

		s_fontRegistry.insert(pair<const char*, Font*>(fontName.c_str(), nFont));
		return nFont;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Font::DeinitFonts() {
	std::map<const char*, Font*>::iterator it;

	for (it = s_fontRegistry.begin(); it != s_fontRegistry.end(); it++) {
		delete it->second;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DRAWING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void Font::DrawText2DWithDefaultFont(const Vector2& position, const String& strToDraw, float scale, const RGBA& color) {

	Font* fontToUse = CreateOrGetFont(DEFAULT_ENGINE_FONT);
	fontToUse->DrawText2D(position, strToDraw, scale, color);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Font::DrawText2D(const String& textName, const Vector2& position, const String& strToDraw, float scale, const RGBA& color) {

	Font* fontToUse = CreateOrGetFont(textName);
	fontToUse->DrawText2D(position, strToDraw, scale, color);
}


//---------------------------------------------------------------------------------------------------------------------------
void Font::DrawText2D(const Vector2& position, const std::string& str, float scale, const RGBA& color) {

	if (str.size() == 0)
		return;

	Vector2 cursor = position + Vector2(0.f, m_lineHeight * scale);
	Glyph* prevGlyph = nullptr;

	for (unsigned int i = 0; i < str.size(); i++) {
		std::map<unsigned char, Glyph*>::iterator glyphIterator = m_glyphs.find(str[i]);

		if (glyphIterator != m_glyphs.end()) {
			Glyph* currGlyph = glyphIterator->second;

			//Adjust for kerning
			int kerning = 0;
			if (prevGlyph) {
				kerning = prevGlyph->GetFromKerningList(currGlyph->GetID());
			}

			cursor.x += kerning * scale;

			Vector2 offset = currGlyph->GetOffset() * scale;
			Vector2 size = Vector2(currGlyph->GetWidth(), currGlyph->GetHeight()) * scale;

			Vector2 mins = cursor + Vector2(offset.x + currGlyph->GetWidth() * scale, -offset.y);
			Vector2 maxs = mins - size;

			AABB2 quadToDrawOn = AABB2(mins, maxs);

			BeirusRenderer::DrawTexturedAABB2(m_material, currGlyph->GetTextureCoords(m_textureScale), color, quadToDrawOn);

			cursor.x += currGlyph->GetXAdvance() * scale;
			prevGlyph = currGlyph;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GETTERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
float Font::GetXAdvanceOfGlyph(unsigned char glyphChar, float scale) {
	std::map<unsigned char, Glyph*>::iterator glyphIterator = m_glyphs.find(glyphChar);

	if (glyphIterator != m_glyphs.end()) {
		Glyph* glyph = glyphIterator->second;

		return (glyph->GetXAdvance()) * scale;
	}
	else {
		return 0.f;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
Glyph* Font::GetGlyphForCharacter(unsigned char glyphChar) {
	std::map<unsigned char, Glyph*>::iterator glyphIterator = m_glyphs.find(glyphChar);

	if (glyphIterator != m_glyphs.end()) {
		Glyph* glyph = glyphIterator->second;

		return glyph;
	}
	else {
		return nullptr;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
float Font::GetWidthOfString(std::string str, float scale) {
	float cursor = 0.f;
	Glyph* prevGlyph = nullptr;

	for (unsigned int i = 0; i < str.size(); i++) {
		std::map<unsigned char, Glyph*>::iterator glyphIterator = m_glyphs.find(str[i]);

		if (glyphIterator != m_glyphs.end()) {
			Glyph* currGlyph = glyphIterator->second;

			//Adjust for kerning
			int kerning = 0;
			if (prevGlyph) {
				kerning = prevGlyph->GetFromKerningList(currGlyph->GetID());
			}

			cursor += kerning * scale;
			cursor += currGlyph->GetXAdvance() * scale;

			prevGlyph = currGlyph;
		}
	}

	return cursor;
}


//---------------------------------------------------------------------------------------------------------------------------
float Font::GetHeightOfString(const String& str, float scale) {

	UNUSED(scale);

	float maxHeight = 0.f;
	for (uint i = 0; i < str.size(); i++) {

		std::map<unsigned char, Glyph*>::iterator glyphIterator = m_glyphs.find(str[i]);

		if (glyphIterator != m_glyphs.end()) {
			
			Glyph* currGlyph = glyphIterator->second;
			float glyphHeight = currGlyph->GetHeight();

			if (glyphHeight > maxHeight) {
				maxHeight = glyphHeight;
			}

		}
	}

	return maxHeight;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
Font::Font(std::string fontName)
	: m_fontName(fontName)
	, m_glyphs()
	, m_textureScale(Vector2(0.f, 0.f))
	, m_lineHeight(0.f)
	, m_lineBase(0.f)
{
	std::string filePath = "Data/Fonts/" + fontName + ".png";
	Texture* texture = Texture::CreateOrGetTexture(filePath);
	texture->GenerateMipmap();
	m_material = new Material("Default2D");
	m_material->SetName(fontName);

	m_material->CreateUniform("gTexDiffuse", UNIFORM_TEXTURE2D, 1, 0, texture);

	LoadGlyphsFromFile();
}


//---------------------------------------------------------------------------------------------------------------------------
Font::~Font() {
	std::map<unsigned char, Glyph*>::iterator it;

	for (it = m_glyphs.begin(); it != m_glyphs.end(); it++) {
		delete it->second;
	}

	delete m_material;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOADING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void Font::LoadGlyphsFromFile() {
	std::string filePathToInfo = "Data/Fonts/" + m_fontName + ".fnt";
	std::vector<unsigned char> buffer;

	if (FileUtils::LoadBinaryFileToBuffer(filePathToInfo, buffer)) {
		ParseBufferIntoGlpyhs(buffer);
	}
	else {
		DebuggerPrintf("Cannot load font into memory. ");
	}

}


//---------------------------------------------------------------------------------------------------------------------------
void Font::ParseBufferIntoGlpyhs(std::vector<unsigned char> buffer) {
	std::vector<std::string> tokens;
	StringUtils::TokenizeBuffer(buffer, ' ', tokens);
	Glyph* newGlyph = nullptr;

	int kerning_first = 0;
	int kerning_second = 0;
	int kerning_amount = 0;

	for (unsigned int i = 0; i < tokens.size(); i++) {
		std::vector<std::string> elems;
		StringUtils::TokenizeBuffer(tokens[i], '=', elems);

		if (elems.size() != 0) {
			//Scale of texture
			if (elems[0] == "scaleW") {
				m_textureScale.x = (float)std::stoi(elems[1]);
			}
			else if (elems[0] == "scaleH") {
				m_textureScale.y = (float)std::stoi(elems[1]);
			}
			else if (elems[0] == "lineHeight") {
				m_lineHeight = (float)std::stoi(elems[1]);
			}
			else if (elems[0] == "base") {
				m_lineBase = (float)std::stoi(elems[1]);
			}
			else if (elems[0] == "id" && elems[1] != "0") {
				newGlyph = new(MEMORY_RENDERING) Glyph();
				newGlyph->SetID((unsigned char)std::stoul(elems[1]));
				m_glyphs.insert(std::pair<unsigned char, Glyph*>(newGlyph->GetID(), newGlyph));
			}
			else if (elems[0] == "x") {
				newGlyph->SetSpriteSheetCoordsX((float)std::stoi(elems[1]));
			}
			else if (elems[0] == "y") {
				newGlyph->SetSpriteSheetCoordsY((float)std::stoi(elems[1]));
			}
			else if (elems[0] == "width") {
				newGlyph->SetWidth((float)std::stoi(elems[1]));
			}
			else if (elems[0] == "height") {
				newGlyph->SetHeight((float)std::stoi(elems[1]));
			}
			else if (elems[0] == "xoffset") {
				newGlyph->SetOffsetX((float)std::stoi(elems[1]));
			}
			else if (elems[0] == "yoffset") {
				newGlyph->SetOffsetY((float)std::stoi(elems[1]));
			}
			else if (elems[0] == "xadvance") {
				newGlyph->SetXAdvance((float)std::stoi(elems[1]));
			}

			//Kerning
			else if (elems[0] == "first") {
				kerning_first = stoul(elems[1]);
			}
			else if (elems[0] == "second") {
				kerning_second = stoul(elems[1]);
			}
			else if (elems[0] == "amount") {
				kerning_amount = stoi(elems[1]);
				std::map<unsigned char, Glyph*>::iterator glyphIterator = m_glyphs.find((uchar)kerning_first);

				if (glyphIterator == m_glyphs.end()) {
					ERROR_RECOVERABLE("Something went wrong reading the font file");
				}
				else {
					Glyph* glyphToAddKerningTo = glyphIterator->second;

					glyphToAddKerningTo->AddToKerningList(std::pair<unsigned char, int>((unsigned char)kerning_second, kerning_amount));
				}
			}
			else {
				continue;
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STATIC GETTERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC float Font::GetLineHeight(const char* fontName, float scale) {

	Font* fontToUse = Font::CreateOrGetFont(fontName);
	return fontToUse->GetLineHeight() * scale;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC float Font::GetDefaultFontLineHeight(float scale) {

	Font* fontToUse = Font::CreateOrGetFont(DEFAULT_ENGINE_FONT);
	return fontToUse->GetLineHeight() * scale;
}