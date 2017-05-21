#include "Engine/Math/RangeInt.hpp"

//STRUCTORS
RangeInt::RangeInt() 
	: m_min(0)
	, m_max(0)
{ }
RangeInt::RangeInt(int min, int max) 
	: m_min(min)
	, m_max(max)
{ }

//ROLL
int RangeInt::Roll() {
	if (m_min == m_max) {
		return m_min;
	}
	else {
		return RandInt(m_min, m_max);
	}
}