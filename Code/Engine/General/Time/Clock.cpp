#include "Engine/General/Time/Clock.hpp"
#include "Engine/General/Core/BeirusEngine.hpp"

STATIC ulong Clock::s_clockId = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Clock::Clock() //Specifically for the system clock
	: m_parent(nullptr)
	, m_scale(1.f)
	, m_paused(false)
	, m_currentTime(0.f)
	, m_deltaTime(0.f)
	, m_id(s_clockId++)
{ 

	if (BeirusEngine::HasBeenInitialized()) {
		m_parent = BeirusEngine::GetEngineClock();
		m_parent->AttachChild(this);
	}


}


//---------------------------------------------------------------------------------------------------------------------------
Clock::Clock(Clock* parent) 
	: m_parent(parent)
	, m_scale(1.f)
	, m_paused(false)
	, m_currentTime(0.f)
	, m_deltaTime(0.f)
	, m_id(s_clockId++)
{ 

	if (m_parent) {
		m_parent->AttachChild(this);
	}
	else {
		m_parent = BeirusEngine::GetEngineClock();
	}
}


//---------------------------------------------------------------------------------------------------------------------------
Clock::Clock(Clock* parent, float scale)
	: m_parent(parent)
	, m_scale(scale)
	, m_paused(false)
	, m_currentTime(0.f)
	, m_deltaTime(0.f)
	, m_id(s_clockId++)
{

	if (m_parent) {
		m_parent->AttachChild(this);
	}
	else {
		m_parent = BeirusEngine::GetEngineClock();
	}
}


//---------------------------------------------------------------------------------------------------------------------------
Clock::~Clock() {

	if (m_parent) {
		m_parent->DetachChild(this);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void Clock::Update(float deltaSeconds) {

	if (m_paused) {
		m_deltaTime = 0.f;
	}
	else {
		m_deltaTime = deltaSeconds;
		m_deltaTime *= m_scale;
	}

	m_currentTime += m_deltaTime;

	for (uint i = 0; i < m_children.size(); i++) {
		m_children[i]->Update(deltaSeconds);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void Clock::AttachChild(Clock* clock) {
	m_children.push_back(clock); 
}


//---------------------------------------------------------------------------------------------------------------------------
void Clock::DetachChild(Clock* clock) {

	uint idx = 0;
	for (idx; idx < m_children.size(); idx++) {
		if (*m_children[idx] == *clock) {
			break;
		}
	}

	//ASSERT_OR_DIE();
	m_children.erase(m_children.begin() + idx);
}