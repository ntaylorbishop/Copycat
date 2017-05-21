#pragma once

#include "Engine/Math/MathUtils.hpp"

class RangeFloat {
public:
	//STRUCTORS
	RangeFloat();
	RangeFloat(float min, float max);

	//GETTERS SETTERS
	float GetMin() const { return m_min; }
	float GetMax() const { return m_max; }
	String ToString() const { return std::to_string(m_min) + "," + std::to_string(m_max); }

	//ROLL
	float Roll();

	//OPERATOR OVERLOADS
	INLINE bool operator==(const RangeFloat& other);

private:
	float m_min;
	float m_max;
};

//---------------------------------------------------------------------------------------------------------------------------
//INLINE
//---------------------------------------------------------------------------------------------------------------------------
inline bool RangeFloat::operator==(const RangeFloat& other) {
	return (m_min == other.GetMin() && m_max == other.GetMax());
}