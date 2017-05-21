#pragma once

#include "Engine/Renderer/Sprites/SpriteAnim.hpp"
#include "Engine/Renderer/Material/RenderState.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Engine/Math/Transform/Transform.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Renderer/Mesh/Model.hpp"

class Material;
class ParticleDefinition;

//const float MOVEMENT_SCALE = 30.f;

class Particle {
public:
	//STRUCTORS
	Particle() = default;
	Particle(Material* pMat, ParticleDefinition* pDef, const Vector3& startPos, const Vector3& startingVel, 
		const Vector3& accel, float lifetime, const Vector3& scale, const RGBA& tint);
	~Particle();

	//UPDATE
	void Update(float deltaSeconds);
	void UpdateBillboard();

	//GETTERS SETTERS
	bool IsAlive() const { return m_isAlive; }


	bool				m_isAlive				= true;
	SpriteAnim			m_spriteAnim			= SpriteAnim();
	float				m_lifetime				= 0.f;
	float				m_age					= 0.f;
	Transform			m_transform				= Transform::IDENTITY;
	RenderState			m_renderState			= RenderState();
	Vector3				m_velocity				= Vector3::ZERO;
	Vector3				m_acceleration			= Vector3::ZERO;
	Vector3				m_position				= Vector3::ZERO;
	ParticleDefinition* m_pDef					= nullptr;
	Material*			m_pMat					= nullptr;
	Particle*			m_next					= nullptr;
	Vector3				m_scale					= Vector3::ZERO;
	Vector2				m_spritesheetTileSize	= Vector2::ZERO;
	uint				m_spriteIndex			= 0;
	RGBA				m_color					= RGBA::WHITE;

	//DEBUG DRAW
	Model*				m_staticMesh			= nullptr;

	friend class ParticleEmitter;
};


void ParticleRenderSetupCallback(void* userData);