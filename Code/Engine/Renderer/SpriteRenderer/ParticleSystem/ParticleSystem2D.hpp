#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Time/Clock.hpp"
#include "Engine/Renderer/SpriteRenderer/ParticleSystem/ParticleEmitter2D.hpp"
#include "Engine/Renderer/SpriteRenderer/ParticleSystem/ParticleSystemDefinition2D.hpp"


class ParticleSystem2D {
public:
	static ParticleSystem2D* Spawn(const String& name, const String& layerName, const Vector2& pos);
	static ParticleSystem2D* Play(const String& systemName, const String& layerName, const Vector2& pos);
	static ParticleSystem2D* Stop(ParticleSystem2D* system);

	bool IsPlaying() const { return m_isPlaying; }

private:
	ParticleSystem2D(const String& name, const String& layerName, const Vector2& pos);
	~ParticleSystem2D();

	void Update(float deltaSeconds);

	String m_name;
	String m_layerID;
	Clock m_clock;
	bool m_isPlaying;
	Vector2 m_pos;
	ParticleSystemDefinition2D* m_systemDef;
	std::vector<ParticleEmitter2D*> m_emitters;

	ParticleSystem2D* m_next;
	ParticleSystem2D* m_prev;

	friend class SpriteLayer;
};