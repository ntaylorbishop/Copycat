#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Time/Time.hpp"
#include <vector>

class Clock {
public:
	//STRUCTORS
	Clock();
	Clock(Clock* parent);
	Clock(Clock* parent, float scale);
	~Clock();

	//UPDATE
	void Update(float deltaSeconds);

	//GETTERS
	double GetCurrent() const			{ return m_currentTime; }
	double GetDelta()	const			{ return m_deltaTime; }

	void SetScale(float scale)			{ m_scale = scale; }
	void Reset()						{ m_currentTime = 0.f; }

	void Pause()						{ m_paused = true; }
	void Unpause()						{ m_paused = false; }

	bool operator==(const Clock& other) const { return (m_id == other.m_id); }
	bool operator!=(const Clock& other) const { return (m_id != other.m_id); }

private:
	size_t m_id						= 0;
	bool m_paused					= false;
	double m_scale					= 0.0;
	double m_currentTime			= 0.0;
	double m_deltaTime				= 0.0;
	Clock* m_parent					= nullptr;
	std::vector<Clock*> m_children;

	void AttachChild(Clock* clock);
	void DetachChild(Clock* clock);

	static ulong s_clockId;
};