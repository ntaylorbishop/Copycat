#include "Engine/Math/RangeVector2.hpp"

//STRUCTORS
RangeVector2::RangeVector2()
	: m_min(Vector2::ZERO)
	, m_max(Vector2::ZERO)
{ }
RangeVector2::RangeVector2(const Vector2& min, const Vector2& max)
	: m_min(min)
	, m_max(max)
{ }

//ROLL
Vector2 RangeVector2::Roll() {
	if (m_min == m_max) {
		return m_min;
	}
	else {
		return Vector2(Randf(m_min.x, m_max.x), Randf(m_min.y, m_max.y));
	}
}