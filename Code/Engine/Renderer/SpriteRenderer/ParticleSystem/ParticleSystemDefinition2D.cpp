#include "Engine/Renderer/SpriteRenderer/ParticleSystem/ParticleSystemDefinition2D.hpp"

STATIC ParticleSystemDefinitionMap ParticleSystemDefinition2D::s_registeredParticleSystemDefs;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//STATICS
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
STATIC ParticleSystemDefinition2D* ParticleSystemDefinition2D::RegisterParticleSystemDefinition(const String& name) {

	size_t hash = std::hash<String>()(name);
	ParticleSystemDefinitionMapIterator it = s_registeredParticleSystemDefs.find(hash);
	ASSERT_OR_DIE(it == s_registeredParticleSystemDefs.end(), "ERROR: Cannot re-register a Particle System Definition");

	ParticleSystemDefinition2D* nPSysDef = new ParticleSystemDefinition2D(name);
	s_registeredParticleSystemDefs.insert(ParticleSystemDefinitionMapPair(hash, nPSysDef));
	return nPSysDef;
}
STATIC ParticleSystemDefinition2D* ParticleSystemDefinition2D::Get(const String& name) {

	size_t hash = std::hash<String>()(name);
	ParticleSystemDefinitionMapIterator it = s_registeredParticleSystemDefs.find(hash);
	ASSERT_OR_DIE(it != s_registeredParticleSystemDefs.end(), "ERROR: No ParticleSystemDefinition exists by that name");
	return it->second;
}
ParticleEmitterDefinition2D* ParticleSystemDefinition2D::AddEmitter(ParticleEmitterDefinition2D* emitterDef) {

	m_emitterDefs.push_back(emitterDef); 
	return emitterDef;
}

STATIC void ParticleSystemDefinition2D::DeinitializeAllDefinitions() {

	ParticleSystemDefinitionMapIterator it;
	for (it = s_registeredParticleSystemDefs.begin(); it != s_registeredParticleSystemDefs.end(); it++) {
		delete it->second;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//MEMBERS
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ParticleSystemDefinition2D::ParticleSystemDefinition2D(const String& name) 
	: m_name(name)
	, m_hasCheckedForLooping(false)
	, m_isLooping(false)
{ }

bool ParticleSystemDefinition2D::IsLooping() { 

	if (m_hasCheckedForLooping) {
		return m_isLooping;
	}
	else {
		for (uint i = 0; i < m_emitterDefs.size(); i++) {
			if (m_emitterDefs[i]->m_spawnRate != 0.f) {
				m_isLooping = true;
				break;
			}
		}

		return m_isLooping; 
	}
}