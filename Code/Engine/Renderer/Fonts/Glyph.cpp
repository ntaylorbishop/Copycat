#include "Engine/Renderer/Fonts/Glyph.hpp"

//STRUCTORS------------------------------------------------------------------------------------------------------------------
Glyph::Glyph() 
	: m_ID(0)
	, m_pixelCoords(Vector2(0.f, 0.f))
	, m_height(0.f)
	, m_offset(Vector2(0.f, 0.f))
	, m_xAdvance(0.f)
{ }
Glyph::Glyph(unsigned char id, Vector2 spriteSheetCoords, float height, Vector2 offset, float xAdvance) 
	: m_ID(id)
	, m_pixelCoords(spriteSheetCoords)
	, m_height(height)
	, m_offset(offset)
	, m_xAdvance(xAdvance)
{ }

//GETTERS------------------------------------------------------------------------------------------------------------------
char Glyph::GetID() const {
	return m_ID;
}
AABB2 Glyph::GetTextureCoords(const Vector2& sizeOfImage) const {
	Vector2 mins = Vector2(m_pixelCoords.x / sizeOfImage.x, m_pixelCoords.y / sizeOfImage.y);
	Vector2 maxs = Vector2((m_pixelCoords.x + m_width) / sizeOfImage.x, (m_pixelCoords.y + m_height) / sizeOfImage.y);
	return AABB2(maxs, mins);
}
float Glyph::GetHeight() const {
	return m_height;
}
float Glyph::GetWidth() const {
	return m_width;
}
Vector2 Glyph::GetOffset() const {
	return m_offset;
}
float Glyph::GetXAdvance() const {
	return m_xAdvance;
}
int Glyph::GetFromKerningList(unsigned char id) {
	std::map<unsigned char, int>::iterator idAndAmount = m_kernings.find(id);

	if (idAndAmount != m_kernings.end()) {
		return idAndAmount->second;
	}
	else {
		return 0;
	}
}

//SETTERS------------------------------------------------------------------------------------------------------------------
void Glyph::SetID(unsigned char id) {
	m_ID = id;
}
void Glyph::SetSpriteSheetCoordsX(float x) {
	m_pixelCoords.x = x;
}
void Glyph::SetSpriteSheetCoordsY(float y) {
	m_pixelCoords.y = y;
}
void Glyph::SetWidth(float width) {
	m_width = width;
}
void Glyph::SetHeight(float height) {
	m_height = height;
}
void Glyph::SetOffsetX(float xOffset) {
	m_offset.x = xOffset;
}
void Glyph::SetOffsetY(float yOffset) {
	m_offset.y = yOffset;
}
void Glyph::SetXAdvance(float xAdvance) {
	m_xAdvance = xAdvance;
}
void Glyph::AddToKerningList(std::pair<unsigned char, int> idAndAmount) {
	m_kernings.insert(idAndAmount);
}
