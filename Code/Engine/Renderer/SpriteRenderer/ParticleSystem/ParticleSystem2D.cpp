#include "Engine/Renderer/SpriteRenderer/ParticleSystem/ParticleSystem2D.hpp"
#include "Engine/Renderer/SpriteRenderer/ParticleSystem/ParticleSystemDefinition2D.hpp"
#include "Engine/Renderer/SpriteRenderer/SpriteLayer.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//STATICS
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
STATIC ParticleSystem2D* ParticleSystem2D::Spawn(const String& name, const String& layerName, const Vector2& pos) {
	
	
	ParticleSystemDefinition2D* def = ParticleSystemDefinition2D::Get(name);
	ASSERT_OR_DIE(def->IsLooping(), "ERROR: Cannot create a non-looping particle system");
	return new ParticleSystem2D(name, layerName, pos);
}
STATIC ParticleSystem2D* ParticleSystem2D::Play(const String& systemName, const String& layerName, const Vector2& pos) {

	ParticleSystemDefinition2D* def = ParticleSystemDefinition2D::Get(systemName);
	ASSERT_OR_DIE(!def->IsLooping(), "ERROR: Cannot play a looping particle system");
	return new ParticleSystem2D(systemName, layerName, pos);
}
STATIC ParticleSystem2D* ParticleSystem2D::Stop(ParticleSystem2D* system) {
	system->m_isPlaying = false;
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//MEMBERS
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ParticleSystem2D::ParticleSystem2D(const String& name, const String& layerName, const Vector2& pos) 
	: m_name(name)
	, m_pos(pos)
	, m_isPlaying(true)
	, m_layerID(layerName)
	, m_prev(nullptr)
	, m_next(nullptr)
{
	m_systemDef = ParticleSystemDefinition2D::Get(name);

	for (uint i = 0; i < m_systemDef->m_emitterDefs.size(); i++) {
		m_emitters.push_back(new ParticleEmitter2D(m_systemDef->m_emitterDefs[i], layerName, pos));
	}

	SpriteLayer* spriteLayer = SpriteLayer::CreateOrGetLayer(m_layerID);

	spriteLayer->AddParticleSystem(this);
}
ParticleSystem2D::~ParticleSystem2D() {
	for (uint i = 0; i < m_emitters.size(); i++) {
		delete m_emitters[i];
		m_emitters[i] = nullptr;
	}
}

void ParticleSystem2D::Update(float deltaSeconds) {

	bool isStillPlaying = false;
	for (uint i = 0; i < m_emitters.size(); i++) {
		if (m_emitters[i]->IsPlaying()) {
			m_emitters[i]->Update(deltaSeconds);
			isStillPlaying = true;
		}
	}

	m_isPlaying = isStillPlaying;
}