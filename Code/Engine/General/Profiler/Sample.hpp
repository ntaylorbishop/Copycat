#pragma once

#include "Engine/General/Core/GameEngine.hpp"

class Sample {
public:
	Sample();
	Sample(const char* tag, double startTime);


	const char* m_tag;
	double m_startTime;
	double m_endTime;
	Sample* m_parent;

	std::vector<Sample*> m_children;
};