#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/RangeFloat.hpp"
#include "Engine/Math/RangeVector2.hpp"
#include "Engine/Renderer/SpriteRenderer/SpriteResource.hpp"
#include "Engine/Renderer/SpriteRenderer/ParticleSystem/Particle2D.hpp"

enum eEmitterBlendMode {
	BLENDMODE_ADDITIVE = 0,
	BLENDMODE_ALPHABLEND
};

class ParticleEmitterDefinition2D {
public:
	static ParticleEmitterDefinition2D* Create(const String emitterName, const String& resourceName);
	static void DeinitializeAllDefinitions();

	eEmitterBlendMode m_blendMode;
	RangeInt m_initialSpawnCount;
	float m_spawnRate;
	RangeFloat m_life;
	RangeFloat m_initialScale;
	RangeVector2 m_startVelocity;
	RangeFloat m_velocityStrength;
	RangeVector2 m_acceleration;
	RGBA m_tint;

private:
	ParticleEmitterDefinition2D(const String& emitterName, const String& resourceName);
	SpriteResource* m_spriteResource;
	String m_resourceName;
	String m_name;

	friend class ParticleEmitter2D;

	static std::map<size_t, ParticleEmitterDefinition2D*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, ParticleEmitterDefinition2D*>>> s_registeredParticleEmitterDefs;
};

typedef std::map<size_t, ParticleEmitterDefinition2D*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, ParticleEmitterDefinition2D*>>> ParticleEmitterDefinitionMap;
typedef std::map<size_t, ParticleEmitterDefinition2D*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, ParticleEmitterDefinition2D*>>>::iterator ParticleEmitterDefinitionMapIterator;
typedef std::pair<size_t, ParticleEmitterDefinition2D*> ParticleEmitterDefinitionMapPair;