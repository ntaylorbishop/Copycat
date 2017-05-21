#pragma once

#include <map>
#include "Engine/General/Core/EngineCommon.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CONSTANTS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
const char* const ROOT_OF_PARTICLE_SYSTEMS_DEF	= "ParticleSystems";
const char* const PARTICLE_DEFINITION			= "ParticleDefinition";
const char* const PARTICLE_SYSTEM_DEFINITION	= "ParticleSystemDefinition";
const char* const PARTICLE_EMITTER_DEFINITION	= "ParticleEmitterDefinition";

class ParticleEmitterDefinition;
class ParticleSystemDefinition;
class ParticleDefinition;
class ParticleSystem;
class Scene;



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PARTICLE SYSTEM MANAGER CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
class ParticleSystemManager {
public:
	//LOAD
	static bool LoadFile(const char* filename);
	static bool LoadFiles(const char* dir);

	//ADD
	static bool AddEmitterDefinition(ParticleEmitterDefinition* emitterDef);
	static bool AddSystemDefinition(ParticleSystemDefinition* systemDef);
	static bool AddParticleDefinition(ParticleDefinition* particleDef);

	//GETTERS SETTERS
	static ParticleDefinition*			GetParticleDefinition(const char* name);
	static ParticleEmitterDefinition*	GetParticleEmitterDefinition(const char* name);
	static ParticleSystemDefinition*	GetParticleSystemDefinition(const char* name);
	static size_t						GetNumberOfActiveParticleSystem();

	static void		SetScene(Scene* scene)	{ s_particleSystemManager->m_currentScene = scene; }
	static Scene*	GetScene()				{ return GetInstance()->m_currentScene; }

	//SHUTDOWN
	static void Shutdown();

private:
	//STRUCTORS
	ParticleSystemManager()		{ }
	~ParticleSystemManager();

	static ParticleSystemManager* GetInstance();

	//UPDATE
	void Update(float deltaSeconds);
	void Render() const;

	//REGISTER UNREGISTER
	static bool RegisterParticleSystem(ParticleSystem* pSystem);
	static bool UnregisterParticleSystem(ParticleSystem* pSystem);


	std::vector<ParticleSystem*> m_pSystems;

	std::map<size_t, ParticleDefinition*>			m_particleDefs;
	std::map<size_t, ParticleEmitterDefinition*>	m_emitterDefs;
	std::map<size_t, ParticleSystemDefinition*>		m_systemDefs;
	Scene*											m_currentScene	= nullptr;


	friend class BeirusRenderer;
	friend class ParticleSystem;


	static ParticleSystemManager* s_particleSystemManager;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TYPEDEFS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
typedef std::pair<size_t, ParticleEmitterDefinition*>			ParticleEmitterDefMapPair;
typedef std::map<size_t, ParticleEmitterDefinition*>::iterator	ParticleEmitterDefMapIterator;
typedef std::pair<size_t, ParticleSystemDefinition*>			ParticleSystemDefPair;
typedef std::map<size_t, ParticleSystemDefinition*>::iterator	ParticleSystemDefIterator;
typedef std::pair<size_t, ParticleDefinition*>					ParticleDefPair;
typedef std::map<size_t, ParticleDefinition*>::iterator			ParticleDefIterator;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC inline size_t ParticleSystemManager::GetNumberOfActiveParticleSystem() { 
	return s_particleSystemManager->m_pSystems.size(); 
}
