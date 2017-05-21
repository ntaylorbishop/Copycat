#pragma once

#include "Engine/Math/RangeFloat.hpp"
#include "Engine/General/Time/Clock.hpp"
#include "Engine/Renderer/SpriteRenderer/SpriteResource.hpp"
#include "Engine/Renderer/SpriteRenderer/ParticleSystem/Particle2D.hpp"
#include "Engine/Renderer/SpriteRenderer/ParticleSystem/ParticleEmitterDefinition2D.hpp"

class ParticleSystemDefinition2D {
public:

	static ParticleSystemDefinition2D* RegisterParticleSystemDefinition(const String& name);
	static ParticleSystemDefinition2D* Get(const String& name);
	ParticleEmitterDefinition2D* AddEmitter(ParticleEmitterDefinition2D* emitterDef);

	static void DeinitializeAllDefinitions();

	bool IsLooping();

private:
	ParticleSystemDefinition2D(const String& name);

	Clock m_clock;
	String m_name;
	bool m_isLooping;
	bool m_hasCheckedForLooping;
	std::vector<ParticleEmitterDefinition2D*> m_emitterDefs;

	friend class ParticleSystem2D;


	static std::map<size_t, ParticleSystemDefinition2D*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, ParticleSystemDefinition2D*>>> s_registeredParticleSystemDefs;
};

typedef std::map<size_t, ParticleSystemDefinition2D*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, ParticleSystemDefinition2D*>>> ParticleSystemDefinitionMap;
typedef std::map<size_t, ParticleSystemDefinition2D*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, ParticleSystemDefinition2D*>>>::iterator ParticleSystemDefinitionMapIterator;
typedef std::pair<size_t, ParticleSystemDefinition2D*> ParticleSystemDefinitionMapPair;