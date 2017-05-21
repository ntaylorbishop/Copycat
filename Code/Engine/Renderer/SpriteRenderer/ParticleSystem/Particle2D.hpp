#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/General/RGBA.hpp"

class Sprite;

class Particle2D {
public:
	Particle2D();
	Particle2D(const String& spriteID, const String& layerID, const Vector2& pos, const Vector2& vel, const Vector2& accel, float maxAge, const Vector2& scale, const RGBA& tint);
	~Particle2D();

	void Update(float deltaSeconds);
	
	Vector2 m_position;
	Vector2 m_velocity;
	Vector2 m_acceleration;
	bool m_isAlive;

private:

	void Cull();

	float m_age;
	float m_maxAge;
	Vector2 m_scale;
	RGBA m_tint;
	Sprite* m_sprite;
};

