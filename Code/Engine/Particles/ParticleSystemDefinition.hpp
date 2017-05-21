#pragma once

#include "Engine/Particles/ParticleEmitterDefinition.hpp"
#include "ThirdParty/Parsers/xmlParser.h"


const uint16_t MAX_PARTICLE_SYSTEM_NAME_SIZE		= 128;
const char* const PARTICLE_SYS_DEF_NODENAME			= "name";
const char* const PARTICLE_SYS_DEF_EMITTERNODE		= "ParticleEmitter";
const char* const PARTICLE_SYS_DEF_EMITTERNODENAME	= "name";



class ParticleSystemDefinition {
public:
	static ParticleSystemDefinition* LoadFromXML(const XMLNode& sysDefNode);
	static ParticleSystemDefinition* Create() { return new ParticleSystemDefinition(); }


	//ADD
	bool AddEmitterDef(ParticleEmitterDefinition* emitterDef);

	//GETTERS SETTERS
	const char*					GetName()						{ return m_name; }
	void						SetName(const char* name)		{ strcpy_s(m_name, MAX_PARTICLE_SYSTEM_NAME_SIZE, name); }
	ParticleEmitterDefinition*	GetEmitterDefAtIdx(size_t idx)	{ return m_emitterDefs[idx]; }

private:
	//STRUCTORS
	ParticleSystemDefinition()		= default;
	~ParticleSystemDefinition()		= default;


	char m_name[MAX_PARTICLE_SYSTEM_NAME_SIZE] = "NULL";
	std::vector<ParticleEmitterDefinition*> m_emitterDefs;


	friend class ParticleSystemManager;
	friend class ParticleSystem;
};