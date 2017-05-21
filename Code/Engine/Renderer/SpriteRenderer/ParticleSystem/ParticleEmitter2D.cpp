#include "Engine/Renderer/SpriteRenderer/ParticleSystem/ParticleEmitter2D.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
ParticleEmitter2D::ParticleEmitter2D(ParticleEmitterDefinition2D* emitterDef, const String& layerName, const Vector2& position)
	: m_emitterDef(emitterDef)
	, m_layerName(layerName)
	, m_isPlaying(true)
	, m_currNumParticles(0)
	, m_age(0.f)
	, m_position(position)
{ 

	for (uint i = 0; i < MAX_NUM_EMITTER_PARTICLES; i++) {
		m_particles[i] = nullptr;
	}

	SpawnParticles();
}
ParticleEmitter2D::~ParticleEmitter2D() {
	for (uint i = 0; i < MAX_NUM_EMITTER_PARTICLES; i++) {
		if (m_particles[i]) {
			delete m_particles[i];
			m_particles[i] = nullptr;
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
//UPDATE
//---------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter2D::Update(float deltaSeconds) {

	m_age += deltaSeconds;

	if (m_emitterDef->m_spawnRate == 0.f && m_currNumParticles == 0) {
		m_isPlaying = false;
	}

	UpdateParticles(deltaSeconds);
	DestroyParticles();

	if (m_isPlaying) {
		SpawnParticles();
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE UPDATES
//---------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter2D::SpawnParticles() {

	if (m_age != 0.f && m_emitterDef->m_spawnRate == 0.f)
		return;

	uint numToSpawn = m_emitterDef->m_initialSpawnCount.Roll();

	for (uint i = 0; i < numToSpawn; i++) {
		
		Vector2 startingVel = m_emitterDef->m_startVelocity.Roll() * m_emitterDef->m_velocityStrength.Roll();
		Vector2 accel = m_emitterDef->m_acceleration.Roll();
		float life = m_emitterDef->m_life.Roll();
		float scale = m_emitterDef->m_initialScale.Roll();

		Particle2D* particle = new Particle2D(m_emitterDef->m_resourceName, m_layerName, m_position, startingVel, accel, life, Vector2(scale, scale), m_emitterDef->m_tint);
		AddParticleToArray(particle);
	}
}
void ParticleEmitter2D::UpdateParticles(float deltaSeconds) {
	for (uint i = 0; i < MAX_NUM_EMITTER_PARTICLES; i++) {
		if(m_particles[i])
			m_particles[i]->Update(deltaSeconds);
	}
}
void ParticleEmitter2D::DestroyParticles() {
	for (uint i = 0; i < MAX_NUM_EMITTER_PARTICLES; i++) {
		if (m_particles[i] && !m_particles[i]->m_isAlive) {
			delete m_particles[i];
			m_particles[i] = nullptr;
			m_currNumParticles--;
		}
	}
}

void ParticleEmitter2D::AddParticleToArray(Particle2D* particle) {

	bool foundASpot = false;
	for (uint i = 0; i < MAX_NUM_EMITTER_PARTICLES; i++) {
		if (!m_particles[i]) {
			m_particles[i] = particle;
			m_currNumParticles++;
			foundASpot = true;
			break;
		}
	}

	ASSERT_OR_DIE(foundASpot, "ERROR: Particle buffer too small to add particle");
}