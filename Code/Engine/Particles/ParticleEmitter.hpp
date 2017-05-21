#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Time/Clock.hpp"
#include "Engine/Particles/ParticleEmitterDefinition.hpp"
#include "Engine/Structures/BeirusCompactArray.hpp"
#include "Engine/Renderer/General/RGBA.hpp"

class Material;
class Particle;
class ParticleEmitterDefinition;

constexpr RGBA	AMBIENT_COLOR	= RGBA(0.588f, 0.588f, 0.588f, 1.f);
constexpr RGBA	SPEC_COLOR		= RGBA(0.588f, 0.588f, 0.588f, 1.f);
constexpr float SPEC_POWER		= 28.f;


class ParticleEmitter {
public:
	//STRUCTORS
	ParticleEmitter(ParticleEmitterDefinition* pEmitterDef, const Vector3& position);
	~ParticleEmitter();

	//UPDATE
	void Update(float deltaSeconds);

	//GETTERS SETTERS
	const char* GetName()					{ return m_pEmitterDef->GetName();	}
	void		SetMaterial(Material* mat)	{ m_pMat = mat;						}
	Material*	GetMaterial()				{ return m_pMat;					}


	ParticleEmitterDefinition*		m_pEmitterDef	= nullptr;
	float							m_age			= 0.f;
	Vector3							m_position		= Vector3::ZERO;
	bool							m_isPlaying		= true;
	Particle*						m_head			= nullptr;
	size_t							m_numParticles	= 0;
	Material*						m_pMat			= nullptr;
	float							m_spawnTicker	= 0.f;


private:
	//MANAGE PARTICLES
	void SpawnParticle();
	void SpawnInitialParticles();
	void UpdateAndDestroyParticles(float deltaSeconds);

	//LIST UTILS
	void InsertIntoList(Particle* nParticle);
};