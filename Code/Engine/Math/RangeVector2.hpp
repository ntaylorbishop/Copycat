#pragma once

#include "Engine/Math/MathUtils.hpp"

class RangeVector2 {
public:
	//STRUCTORS
	RangeVector2();
	RangeVector2(const Vector2& min, const Vector2& max);

	//GETTERS SETTERS
	Vector2 GetMin() const { return m_min; }
	Vector2 GetMax() const { return m_max; }

	//ROLL
	Vector2 Roll();

	//OPERATOR OVERLOADS
	INLINE bool operator==(const RangeVector2& other);

private:
	Vector2 m_min;
	Vector2 m_max;
};

//---------------------------------------------------------------------------------------------------------------------------
//INLINE
//---------------------------------------------------------------------------------------------------------------------------
inline bool RangeVector2::operator==(const RangeVector2& other) {
	return (m_min == other.GetMin() && m_max == other.GetMax());
}