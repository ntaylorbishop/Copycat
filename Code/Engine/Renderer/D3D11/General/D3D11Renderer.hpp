#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/Vector2.hpp"

class D3D11BlendState;

class D3D11Renderer {
public:
	static D3D11Renderer* Get();
	void SetViewport(const Vector2& viewportSize);
	void EnableAlphaBlending();
	void EnableOpaqueBlending();

private:
	D3D11Renderer();
	~D3D11Renderer();

	void InitializeDefaultBlendStates();


	D3D11BlendState* m_defaultAlphaBlendState	= nullptr;

	static D3D11Renderer* s_d3d11Renderer;
};