#pragma once

#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/General/RGBA.hpp"

class UIRenderer {
public:
	static UIRenderer* Get();

	void DrawSquare(const AABB2& quad, const RGBA& color, float tint);
	void DrawFontGlyph(Material* mat, const Vector2& texCoords, const RGBA& color, float tint);


private:


};