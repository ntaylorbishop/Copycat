#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Utils/StringUtils.hpp"
#include "Engine/General/Profiler/Profiler.hpp"

#define STATIC

STATIC const Vector2 Vector2::UNIT_VECTOR_X(1.f, 0.f);
STATIC const Vector2 Vector2::UNIT_VECTOR_Y(0.f, 1.f);
STATIC const Vector2 Vector2::ZERO(0.f, 0.f);
STATIC const Vector2 Vector2::ONE(1.f, 1.f);

STATIC const Vector2 Vector2::TEXCOORD_TOP_LEFT(0.f, 0.f);
STATIC const Vector2 Vector2::TEXCOORD_TOP_RIGHT(1.f, 0.f);
STATIC const Vector2 Vector2::TEXCOORD_BOTTOM_RIGHT(1.f, 1.f);
STATIC const Vector2 Vector2::TEXCOORD_BOTTOM_LEFT(0.f, 1.f);


IntVector2 Vector2::ToIntVector2() {
	return IntVector2((int)x, (int)y);
}

std::string Vector2::ToString() {
	return StringUtils::Stringf("(%.2f, %.2f)", x, y); 
}

void Vector2::Normalize() {
	float length = sqrt(x * x + y * y);
	x = x / length;
	y = y / length;
}
