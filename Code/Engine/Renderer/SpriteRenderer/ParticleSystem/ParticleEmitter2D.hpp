#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/SpriteRenderer/ParticleSystem/ParticleEmitterDefinition2D.hpp"

const uint MAX_NUM_EMITTER_PARTICLES = 100000;

class ParticleEmitter2D {
public:
	ParticleEmitter2D(ParticleEmitterDefinition2D* emitterDef, const String& layerName, const Vector2& position);
	~ParticleEmitter2D();

	void Update(float deltaSeconds);

	bool IsPlaying() const { return m_isPlaying; }

private:
	void SpawnParticles();
	void UpdateParticles(float deltaSeconds);
	void DestroyParticles();

	void AddParticleToArray(Particle2D* particle);

	ParticleEmitterDefinition2D* m_emitterDef;
	String m_layerName;
	bool m_isPlaying;
	uint m_currNumParticles;
	Vector2 m_position;

	float m_age;

	Particle2D* m_particles[MAX_NUM_EMITTER_PARTICLES];
};