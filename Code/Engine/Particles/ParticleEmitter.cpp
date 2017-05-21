#include "Engine/Particles/ParticleEmitter.hpp"
#include "Engine/Particles/Particle.hpp"
#include "Engine/Particles/ParticleDefinition.hpp"
#include "Engine/Renderer/Material/Material.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
static void InitializeMat(ParticleEmitter* pEmitter) {

	ParticleEmitterDefinition*	pEmitterDef	= pEmitter->m_pEmitterDef;
	ParticleDefinition*			pDef		= pEmitter->m_pEmitterDef->m_particleDef;

	ASSERT_OR_DIE(pDef != nullptr, "ERROR: Particle emitter definition must have at least one particle definition attached.");

	const char* pShader = pEmitterDef->GetShaderName();
	Material*	newMat	= new Material(pShader);
	newMat->SetName("ParticleMat");

	Texture* diffSpritesheet	= pDef->GetSpriteAnim()->GetSpriteSheet()->GetTexture();
	Texture* normalMap			= pDef->GetNormalMap();
	
	newMat->CreateUniform("gTexDiffuse",		UNIFORM_TEXTURE2D,	1, 0, diffSpritesheet);
	newMat->CreateUniform("gModel",				UNIFORM_MAT4,		1, &Matrix4::IDENTITY);
	newMat->CreateUniform("gVertColor",			UNIFORM_RGBA,		1, &RGBA::WHITE);
	newMat->CreateUniform("gDiffuseColor",		UNIFORM_RGBA,		1, &RGBA::WHITE);
	newMat->CreateUniform("gUsesDiffuseTex",	UNIFORM_UINT,		1, &UINT_TRUE);
	newMat->CreateUniform("gAmbientLight",		UNIFORM_RGBA,		1, &AMBIENT_COLOR);
	newMat->CreateUniform("gSpecularColor",		UNIFORM_RGBA,		1, &SPEC_COLOR);
	newMat->CreateUniform("gIsParticle",		UNIFORM_BOOL,		1, &BOOL_FALSE);
	newMat->CreateUniform("gSpriteIndex",		UNIFORM_UINT,		1, &UINT_FALSE);
	newMat->CreateUniform("gTileSize",			UNIFORM_VECTOR2,	1, &Vector2::ZERO);
	newMat->CreateUniform("gSpecularPower",		UNIFORM_FLOAT,		1, &SPEC_POWER);

	if (normalMap != nullptr) {
		newMat->CreateUniform("gTexNormal",	UNIFORM_TEXTURE2D,	1, 1, normalMap);
	}

	pEmitter->m_pMat = newMat;
}


//---------------------------------------------------------------------------------------------------------------------------
ParticleEmitter::ParticleEmitter(ParticleEmitterDefinition* pEmitterDef, const Vector3& position)
	: m_pEmitterDef(pEmitterDef)
	, m_position(position)
{
	
	InitializeMat(this);
	SpawnInitialParticles();
}


//---------------------------------------------------------------------------------------------------------------------------
ParticleEmitter::~ParticleEmitter() {

	Particle* curr = m_head;

	while (curr) {
		Particle* next = curr->m_next;
		delete curr;
		curr = next;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::Update(float deltaSeconds) {
	
	m_age			+= deltaSeconds;
	m_spawnTicker	+= deltaSeconds;

	if (m_pEmitterDef->m_spawnRate == 0.f && m_numParticles == 0) {
		m_isPlaying = false;
	}

	UpdateAndDestroyParticles(deltaSeconds);

	if (m_isPlaying && m_spawnTicker >= m_pEmitterDef->m_spawnRate) {
		SpawnInitialParticles();
		m_spawnTicker = 0.f;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MANAGE PARTICLES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::SpawnParticle() {

	Vector3 startingVel = m_pEmitterDef->m_startVelocity.Roll() * m_pEmitterDef->m_velocityStrength.Roll();
	Vector3 accel		= m_pEmitterDef->m_acceleration.Roll();
	float life			= m_pEmitterDef->m_lifetime.Roll();
	float scale			= m_pEmitterDef->m_initialScale.Roll();

	Particle* nParticle = new Particle(m_pMat, m_pEmitterDef->m_particleDef, m_position,
		startingVel, accel, life, Vector3(scale), m_pEmitterDef->m_tint);
	InsertIntoList(nParticle);
}


//---------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::SpawnInitialParticles() {

	if (m_age != 0.f && m_pEmitterDef->m_spawnRate == 0.f) {
		return;
	}

	uint numToSpawn = m_pEmitterDef->m_initialSpawnCount.Roll();

	for (uint i = 0; i < numToSpawn; i++) {

		SpawnParticle();
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::UpdateAndDestroyParticles(float deltaSeconds) {

	Particle* curr = m_head;

	if (curr) {
		curr->Update(deltaSeconds);

		if (!curr->m_isAlive) {
			Particle* tmp = curr->m_next;
			delete curr;
			m_numParticles--;
			m_head = tmp;
			curr = m_head;
		}
	}
	else {
		return;
	}

	while (curr && curr->m_next) {
		if (!curr->m_next->m_isAlive) {
			Particle* next = curr->m_next->m_next;
			delete curr->m_next;
			m_numParticles--;
			curr->m_next = next;
			curr = curr->m_next;
		}
		else {
			curr->m_next->Update(deltaSeconds);
			curr = curr->m_next;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LIST UTILS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::InsertIntoList(Particle* nParticle) {

	if (!m_head) {
		m_head = nParticle;
		nParticle->m_next = nullptr;
	}
	else {
		Particle* tmp = m_head;
		m_head = nParticle;
		m_head->m_next = tmp;
	}
	m_numParticles++;
}