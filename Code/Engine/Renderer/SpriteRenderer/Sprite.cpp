#include "Engine/Renderer/SpriteRenderer/Sprite.hpp"

STATIC uint Sprite::s_spriteIDs = 0;

//---------------------------------------------------------------------------------------------------------------------------
//STATICS
//---------------------------------------------------------------------------------------------------------------------------
STATIC Sprite* Sprite::CreateSprite(const String& ID, const String& layer) {
	Sprite* s = new Sprite(ID);
	s->SetLayer(layer);
	return s;
}
STATIC void Sprite::Delete(Sprite* s) {
	TheSpriteRenderer::UnregisterSprite(s);
	delete s;
}

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
Sprite::Sprite(const String& ID)
	: m_instanceID(s_spriteIDs++)
	, m_position(Vector2(0.f, 0.f))
	, m_rotation(0.f)
	, m_scale(Vector2(1.f, 1.f))
	, m_enabled(false)
	, m_layerID()
	, m_spriteResource(nullptr)
	, m_material(nullptr)
	, m_prev(nullptr)
	, m_next(nullptr)
	, m_pivot(Vector2(0.f, 0.f))
	, m_tint(RGBA::WHITE)
{
	m_spriteResource = SpriteResource::Get(ID.c_str());
	Vector2 dim = m_spriteResource->m_dimensions;
	m_pivot = Vector2(dim.x / 2.f, dim.y / 2.f);

	m_material = new Material("DefaultSprite");

	Texture* tex = Texture::CreateOrGetTexture(m_spriteResource->m_textureFilename);
	m_material->CreateUniform("gTexDiffuse",	UNIFORM_TEXTURE2D,	1, tex);
	m_material->CreateUniform("gView",			UNIFORM_MAT4,		1, &Matrix4::IDENTITY);
}
Sprite::~Sprite() {
	delete m_material;
}

void Sprite::SetLayer(const String& layer) {
	if (m_enabled) {
		TheSpriteRenderer::UnregisterSprite(this);
		m_layerID = layer;
		TheSpriteRenderer::RegisterSprite(this);
	}
	else {
		m_layerID = layer;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//ENABLE DISABLE
//---------------------------------------------------------------------------------------------------------------------------
void Sprite::Enable() {
	if (!m_enabled) {
		m_enabled = true;
		TheSpriteRenderer::RegisterSprite(this);
	}
}
void Sprite::Disable() {
	if (m_enabled) {
		m_enabled = false;
		TheSpriteRenderer::UnregisterSprite(this);
	}
}
