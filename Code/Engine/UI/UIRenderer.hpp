#pragma once

#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/General/RGBA.hpp"

class D3D11Material;

class UIRenderer {
public:
	static UIRenderer* Get();

	void DrawAABB2(const AABB2& quad, const RGBA& color, float tint);
	void DrawFontGlyph(D3D11Material* mat, const Vector2& texCoords, const AABB2& quad, const RGBA& color, float tint);


private:


};