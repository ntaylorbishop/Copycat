#include "Engine/Math/RangeFloat.hpp"

//STRUCTORS
RangeFloat::RangeFloat()
	: m_min(0)
	, m_max(0)
{ }
RangeFloat::RangeFloat(float min, float max)
	: m_min(min)
	, m_max(max)
{ }

//ROLL
float RangeFloat::Roll() {
	if (m_min == m_max) {
		return m_min;
	}
	else {
		return Randf(m_min, m_max);
	}
}