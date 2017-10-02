#pragma once

#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Engine/Math/Matrix44.hpp"

class D3D11Material;
class D3D11Mesh;
class D3D11MeshRenderer;

const Vector2 SCREEN_SIZE_UI = Vector2(1600.f, 900.f);

class UIRenderer {
public:
	static UIRenderer* Get();

	void DrawAABB2(const Vector2& pos, const Vector2& size, const RGBA& color);
	void DrawTexturedAABB2(D3D11Material* mat, const Vector2& pos, const Vector2& size);
	Matrix44* GetDefaultOrtho()			{ return &m_defaultOrtho;	}
	Vector2 GetScreenSize()				{ return SCREEN_SIZE_UI;	}

private:
	UIRenderer() = default;
	~UIRenderer() = default;
	void Initialize();

	bool				m_hasInitialized	= false;
	Matrix44			m_defaultOrtho;
	D3D11Material*		m_2dBlankMat		= nullptr;
	D3D11Mesh*			m_quadMesh			= nullptr;
	D3D11MeshRenderer*	m_scratchMR			= nullptr;
	RGBA				m_blankMatTint		= RGBA::WHITE;

	static UIRenderer*	s_uiRenderer;
};