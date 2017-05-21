#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/Vector3.hpp"

class ParticleEmitter;
class ParticleSystemDefinition;

class ParticleSystem {
public:
	//CREATE DESTROY
	static ParticleSystem* Create(const char* name, const Vector3& pos);
	static bool Destroy(ParticleSystem* pSystem);

	//UPDATE
	void Update(float deltaSeconds);

	//OPERATORS
	bool operator==(const ParticleSystem& other);
	bool operator!=(const ParticleSystem& other);

private:
	//STRUCTORS
	ParticleSystem(ParticleSystemDefinition* pSysDef, const Vector3& pos);
	~ParticleSystem();


	std::vector<ParticleEmitter*>	m_emitters;
	ParticleSystemDefinition*		m_pSysDef		= nullptr;
	size_t							m_id			= 0;
	Vector3							m_position		= Vector3::ZERO;


	static size_t s_particleSystemIDs;


	friend class ParticleSystemManager;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline bool ParticleSystem::operator==(const ParticleSystem& other) {
	return (m_id == other.m_id);
}


//---------------------------------------------------------------------------------------------------------------------------
inline bool ParticleSystem::operator!=(const ParticleSystem& other) {
	return (m_id != other.m_id);
}