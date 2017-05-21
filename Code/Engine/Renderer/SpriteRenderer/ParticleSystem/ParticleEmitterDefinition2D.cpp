#include "Engine/Renderer/SpriteRenderer/ParticleSystem/ParticleEmitterDefinition2D.hpp"

STATIC ParticleEmitterDefinitionMap ParticleEmitterDefinition2D::s_registeredParticleEmitterDefs;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//STATICS
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
STATIC ParticleEmitterDefinition2D* ParticleEmitterDefinition2D::Create(const String emitterName, const String& resourceName) {

	size_t hash = std::hash<String>()(emitterName);
	ParticleEmitterDefinitionMapIterator it = s_registeredParticleEmitterDefs.find(hash);
	ASSERT_OR_DIE(it == s_registeredParticleEmitterDefs.end(), "ERROR: Cannot re-initialize an already created ParticleEmitterDefinition");

	ParticleEmitterDefinition2D* nEmitterDef = new ParticleEmitterDefinition2D(emitterName, resourceName);
	s_registeredParticleEmitterDefs.insert(ParticleEmitterDefinitionMapPair(hash, nEmitterDef));
	return nEmitterDef;
}
STATIC void ParticleEmitterDefinition2D::DeinitializeAllDefinitions() {

	ParticleEmitterDefinitionMapIterator it;
	for (it = s_registeredParticleEmitterDefs.begin(); it != s_registeredParticleEmitterDefs.end(); it++) {
		delete it->second;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//MEMBERS
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ParticleEmitterDefinition2D::ParticleEmitterDefinition2D(const String& emitterName, const String& resourceName)
	: m_blendMode(BLENDMODE_ALPHABLEND)
	, m_initialSpawnCount(RangeInt(0, 0))
	, m_spawnRate(0.f)
	, m_life(RangeFloat(0.f, 0.f))
	, m_initialScale(RangeFloat(0.f, 0.f))
	, m_tint(RGBA::WHITE)
	, m_spriteResource(nullptr)
	, m_name(emitterName)
	, m_startVelocity(RangeVector2())
	, m_velocityStrength(RangeFloat(0.f, 0.f))
	, m_resourceName(resourceName)
	, m_acceleration(RangeVector2())
{
	m_spriteResource = SpriteResource::Get(resourceName);
	ASSERT_OR_DIE(m_spriteResource != nullptr, "ERROR: Cannot instantiate ParticleEmitterDefinition with null SpriteResource");
}