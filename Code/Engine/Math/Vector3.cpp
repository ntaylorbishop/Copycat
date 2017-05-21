#include "Engine/Math/Vector3.hpp"
#define STATIC //Do-nothing to indicate static

STATIC const Vector3 Vector3::ZERO(0.f, 0.f, 0.f);
STATIC const Vector3 Vector3::ONE(1.f, 1.f, 1.f);
STATIC const Vector3 Vector3::UNIT_VECTOR_X(1.f, 0.f, 0.f);
STATIC const Vector3 Vector3::UNIT_VECTOR_Y(0.f, 1.f, 0.f);
STATIC const Vector3 Vector3::UNIT_VECTOR_Z(0.f, 0.f, 1.f);

STATIC const Vector3 Vector3::RIGHT(	1.f, 0.f, 0.f);
STATIC const Vector3 Vector3::UP(		0.f, 1.f, 0.f);
STATIC const Vector3 Vector3::FORWARD(	0.f, 0.f, 1.f);