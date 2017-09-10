#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/Vector2.hpp"

class D3D11Renderer {
public:
	static void Get();
	void SetViewport(const Vector2& viewportSize);
	void DrawSquare2D();

private:
	D3D11Renderer();
	~D3D11Renderer();

	static D3D11Renderer* s_d3d11Renderer;
}
