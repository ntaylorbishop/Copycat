#pragma once

#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Engine/Math/Matrix44.hpp"

class D3D11Material;
class D3D11Mesh;
class D3D11MeshRenderer;

class UIRenderer {
public:
	static UIRenderer* Get();

	void DrawAABB2(const Vector2& pos, const Vector2& size, const RGBA& color, float tint);
	void DrawTexturedAABB2(D3D11Material* mat, const Vector2& pos, const Vector2& size);
	Matrix44* GetDefaultOrtho() { return &m_defaultOrtho; }

private:
	UIRenderer();
	~UIRenderer() = default;

	Matrix44			m_defaultOrtho;
	D3D11Material*		m_2dBlankMat	= nullptr;
	D3D11Mesh*			m_quadMesh		= nullptr;
	D3D11MeshRenderer*	m_scratchMR		= nullptr;

	static UIRenderer*	s_uiRenderer;
};