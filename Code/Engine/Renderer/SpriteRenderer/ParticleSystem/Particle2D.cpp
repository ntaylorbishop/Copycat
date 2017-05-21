#include "Engine/Renderer/SpriteRenderer/ParticleSystem/Particle2D.hpp"
#include "Engine/Renderer/SpriteRenderer/Sprite.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
Particle2D::Particle2D() 
	: m_position(Vector2::ZERO)
	, m_velocity(Vector2::ZERO)
	, m_acceleration(Vector2::ZERO)
	, m_age(0.f)
	, m_maxAge(0.f)
	, m_scale(Vector2::ZERO)
	, m_tint(RGBA::WHITE)
	, m_isAlive(false)
	, m_sprite(nullptr)
{ }
Particle2D::Particle2D(const String& spriteID, const String& layerID, const Vector2& pos, const Vector2& vel, const Vector2& accel, float maxAge, const Vector2& scale, const RGBA& tint)
	: m_position(pos)
	, m_velocity(vel)
	, m_acceleration(accel)
	, m_age(0.f)
	, m_maxAge(maxAge)
	, m_scale(scale)
	, m_tint(tint)
	, m_isAlive(true)
	, m_sprite(nullptr)
{ 
	m_sprite = Sprite::CreateSprite(spriteID, layerID);
	m_sprite->Enable();
	m_sprite->m_scale = m_scale;
	m_sprite->m_tint = m_tint;
}
Particle2D::~Particle2D() {
	Sprite::Delete(m_sprite);
}

void Particle2D::Update(float deltaSeconds) {
	m_age += deltaSeconds;
	m_velocity = m_velocity + m_acceleration * deltaSeconds;
	m_position = m_position + m_velocity * deltaSeconds;

	if (m_age >= m_maxAge) {
		m_isAlive = false;
	}
	m_sprite->m_position = m_position;

	Cull();
}

void Particle2D::Cull() {
	AABB2 rect = TheSpriteRenderer::GetRect();
	rect.mins = rect.mins + Vector2(-1.f, -1.f);
	rect.maxs = rect.maxs + Vector2(1.f, 1.f);

	if (!IsDiskInSquare(m_position, m_scale.x, rect)) {
		m_isAlive = false;
	}
}