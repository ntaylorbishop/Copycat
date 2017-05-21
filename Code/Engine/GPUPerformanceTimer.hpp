#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

class GPUPerformanceTimer {
public:
	//STRUCTORS
	GPUPerformanceTimer();
	~GPUPerformanceTimer();

	//START STOP
	void Start();
	void Stop();

	//GETTERS SETTERS
	double GetDifference() const;

private:
	uint64_t m_startTime;
	uint64_t m_stopTime;
	uint m_queryID[2];
};