#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Engine/Math/Range.hpp"
#include "Engine/Math/Vector3.hpp"
#include "ThirdParty/Parsers/xmlParser.h"

class ParticleDefinition;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CONSTANTS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
const char* const PEMITTERDEF_NODENAME				= "ParticleEmitterDefinition";
const char* const PEMITTERDEF_ATTRIB_NAME			= "name";
const char* const PEMITTERDEF_ATTRIB_INITSPAWNCOUNT = "initialSpawnCount";
const char* const PEMITTERDEF_ATTRIB_SPAWNRATE		= "spawnRate";
const char* const PEMITTERDEF_ATTRIB_LIFETIME		= "lifeTime";
const char* const PEMITTERDEF_ATTRIB_INITSCALE		= "initialScale";
const char* const PEMITTERDEF_ATTRIB_COLOR			= "tint";
const char* const PEMITTERDEF_ATTRIB_STARTVEL		= "startVelocity";
const char* const PEMITTERDEF_ATTRIB_VELSTR			= "velocityStrength";
const char* const PEMITTERDEF_ATTRIB_ACCEL			= "acceleration";
const char* const PEMITTERDEF_ATTRIB_SHADERTYPE		= "shaderType";
const char* const PEMITTERDEF_ATTRIB_PDEF			= "particleDefinition";

const uint16_t MAX_PARTICLE_EMITTER_NAME_SIZE = 128;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PARTICLE EMMITER DEFINITION CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
class ParticleEmitterDefinition {
public:
	//LOAD CREATE
	static ParticleEmitterDefinition* LoadFromXML(const XMLNode& emitterDefNode);
	static ParticleEmitterDefinition* Create()	{ return new ParticleEmitterDefinition(); }

	//GETTERS SETTERS
	const char* GetName()					{ return m_name; }
	const char* GetShaderName()				{ return m_shaderName; }
	void SetName(const char* name)			{ strcpy_s(m_name, MAX_PARTICLE_EMITTER_NAME_SIZE, name); }
	void SetShaderName(const char* name)	{ strcpy_s(m_shaderName, MAX_PARTICLE_EMITTER_NAME_SIZE, name); }



	char				m_shaderName[MAX_PARTICLE_EMITTER_NAME_SIZE]	= "NULL";
	char				m_name[MAX_PARTICLE_EMITTER_NAME_SIZE]			= "NULL";
	Range<int>			m_initialSpawnCount								= Range<int>(0, 0);
	float				m_spawnRate										= 1.f;
	Range<float>		m_lifetime										= Range<float>(0.f, 0.f);
	Range<float>		m_initialScale									= Range<float>(0.f, 0.f);
	RGBA				m_tint											= RGBA::WHITE;
	Range<Vector3>		m_startVelocity									= Vector3::ZERO;
	Range<float>		m_velocityStrength								= Range<float>(0.f, 0.f);
	Range<Vector3>		m_acceleration									= Range<Vector3>(Vector3::ZERO, Vector3::ZERO);
	ParticleDefinition* m_particleDef									= nullptr;


private:
	//STRUCTORS
	ParticleEmitterDefinition()		= default;
	~ParticleEmitterDefinition()	= default;




	friend class ParticleSystemManager;
	friend class ParticleEmitter;
};