#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Cameras/Camera2D.hpp"
#include "Engine/Renderer/Mesh/MeshRenderer.hpp"
#include "Engine/Renderer/SpriteRenderer/SpriteLayer.hpp"
#include "Engine/Renderer/SpriteRenderer/SpriteResource.hpp"

class TextureBuffer;

class TheSpriteRenderer {
public:
	//INIT
	static void Initialize(float virtualSize, float aspectRatio);
	static void RegisterSpriteRendererCommands();
	static void Destroy();

	static void RegisterSprite(Sprite* sprite);
	static void UnregisterSprite(Sprite* sprite);

	static void CreateLayer(const String& layer);

	//UPDATE RENDER
	static void Update(float deltaSeconds)		{ s_theSpriteRenderer->Update(deltaSeconds);		}
	static void Render(const Camera2D& cam)		{ s_theSpriteRenderer->UpdateViewAndRender(cam);	}
	static void Render()						{ s_theSpriteRenderer->InternalRender();			}


	//GETTERS SETTERS
	static AABB2		GetRect()						{ return s_theSpriteRenderer->m_virtualRect;				}
	static void			SetImportSize(float importSize) { s_theSpriteRenderer->InternalSetImportSize(importSize);	}
	static Vector2		GetBounds()						{ return s_theSpriteRenderer->m_virtualBounds;				}
	static void			SetClearColor(const RGBA& col)	{ BeirusRenderer::ClearScreen(col);							}
	static Material*	GetDefaultMaterial()			{ return s_theSpriteRenderer->m_defaultMaterial;			}
	static float		GetImportSize()					{ return s_theSpriteRenderer->m_importSize;					}
	static float		GetVirtualSize()				{ return s_theSpriteRenderer->m_virtualSize;				}

	//LAYER EFFECTS
	static void AddLayerEffect(const String& layerName, const String& shaderProgName) { s_theSpriteRenderer->InternalAddLayerEffect(layerName, shaderProgName); }
	static void EnableLayer(const String& layerName);
	static void DisableLayer(const String& layerName);

	//CAMERA
	static void UpdateCamera(const Camera2D& cam);

private:
	//STRUCTORS
	TheSpriteRenderer(float virtualSize, float aspectRatio);
	~TheSpriteRenderer();

	void InitializeDefaultShaders();
	void InternalSetImportSize(float importSize);

	//UPDATE
	void InternalUpdate(float deltaSeconds);

	//RENDER
	void UpdateViewAndRender(const Camera2D& cam);
	void InternalRender();
	void RenderLayer(SpriteLayer* currLayer);
	void DrawSprite(Sprite* sprite);

	Vector2 InternalGetBounds() const { return m_virtualBounds; }
	Material* InternalGetDefaultMaterial() { return m_defaultMaterial; }
	float InternalGetImportSize() { return m_importSize; }

	static void CopyToMesh(Sprite* sprite);
	bool Cull(Sprite* s) const;

	//LAYER EFFECTS
	void InternalAddLayerEffect(const String& layerName, const String& shaderProgName);
	void RenderFullScreenQuad(Material* mat);
	SpriteLayer* GetLayer(const String& name);

	//CAMERA
	void UpdateViewMatrix(const Camera2D& cam);
	void UpdateViewInMaterial(Material* mat) const;
	
	
	////////////////////////////////////////////////////
	//------------------------------------------
	//MEMBERS VARS
	MeshRenderer				m_meshRenderer;
	Material*					m_defaultMaterial		= nullptr;
	float						m_virtualSize			= 0.f;
	Vector2						m_virtualBounds			= Vector2::ZERO;
	float						m_aspectRatio			= 0.f;
	Matrix4						m_ortho					= Matrix4::IDENTITY;
	float						m_importSize			= 0.f;
	AABB2						m_virtualRect			= AABB2();
	Matrix4						m_view					= Matrix4::IDENTITY;
	Framebuffer*				m_currentSurface		= nullptr;
	Framebuffer*				m_effectSurface			= nullptr;
	TextureBuffer*				m_currentColorTarget	= nullptr;
	TextureBuffer*				m_effectColorTarget		= nullptr;
	std::vector<Sprite*>		m_sprites;
	std::vector<SpriteLayer*>	m_layers;

	////////////////////////////////////////////////////
	//------------------------------------------
	//STATIC MEMBER VARS
	static TheSpriteRenderer*	s_theSpriteRenderer;
	static MeshID				s_mesh;
	static MeshID				s_fullScreenMesh;
};

//CONSOLE COMMANDS
void LayerToggle(Command& args);