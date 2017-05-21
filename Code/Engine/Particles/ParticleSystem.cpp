#include "Engine/Particles/ParticleSystem.hpp"
#include "Engine/Particles/ParticleSystemManager.hpp" 
#include "Engine/Particles/ParticleSystemDefinition.hpp"
#include "Engine/Particles/ParticleEmitter.hpp"


STATIC size_t ParticleSystem::s_particleSystemIDs = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CREATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC ParticleSystem* ParticleSystem::Create(const char* name, const Vector3& pos) {

	ParticleSystemDefinition* sysDef = ParticleSystemManager::GetParticleSystemDefinition(name);

	ParticleSystem* newSystem = new ParticleSystem(sysDef, pos);
	ASSERT_OR_DIE(newSystem != nullptr, "ERROR: That particle system does not have a system definition.");

	ParticleSystemManager::RegisterParticleSystem(newSystem);
	return newSystem;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool ParticleSystem::Destroy(ParticleSystem* pSystem) {

	bool wasRegistered = ParticleSystemManager::UnregisterParticleSystem(pSystem);
	ASSERT_OR_DIE(wasRegistered == true, "ERROR: ParticleSystem trying to destroy wasn't registered in the manager. This shouldn't have happened.");

	delete pSystem;
	pSystem = nullptr;
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void ParticleSystem::Update(float deltaSeconds) {

	for (uint eDefIdx = 0; eDefIdx < m_emitters.size(); eDefIdx++) {
		m_emitters[eDefIdx]->Update(deltaSeconds);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
ParticleSystem::ParticleSystem(ParticleSystemDefinition* pSysDef, const Vector3& pos) 
	: m_pSysDef(pSysDef) 
	, m_id(s_particleSystemIDs++)
	, m_position(pos)
{
	
	for (uint sysDefIdx = 0; sysDefIdx < m_pSysDef->m_emitterDefs.size(); sysDefIdx++) {
		ParticleEmitter* nEmitter = new ParticleEmitter(m_pSysDef->GetEmitterDefAtIdx(sysDefIdx), m_position);
		m_emitters.push_back(nEmitter);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
ParticleSystem::~ParticleSystem() {

	for (uint emitterIdx = 0; emitterIdx < m_emitters.size(); emitterIdx++) {
		delete m_emitters[emitterIdx];
		m_emitters[emitterIdx] = nullptr;
	}
}