#include "Engine/Math/Vector4.hpp"
#define STATIC //Do-nothing to indicate static

STATIC const Vector4 Vector4::ZERO(0.f, 0.f, 0.f, 0.f);
STATIC const Vector4 Vector4::ONE(1.f, 1.f, 1.f, 1.f);
STATIC const Vector4 Vector4::UNIT_VECTOR_X(1.f, 0.f, 0.f, 0.f);
STATIC const Vector4 Vector4::UNIT_VECTOR_Y(0.f, 1.f, 0.f, 0.f);
STATIC const Vector4 Vector4::UNIT_VECTOR_Z(0.f, 0.f, 1.f, 0.f);
STATIC const Vector4 Vector4::UNIT_VECTOR_W(0.f, 0.f, 0.f, 1.f);

//STRUCTORS
Vector4::Vector4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
Vector4::Vector4(float x, float y, float z, float w)
	: x(x)
	, y(y)
	, z(z)
	, w(w)
{}
Vector4::Vector4(const Vector4& other)
	: x(other.x)
	, y(other.y)
	, z(other.z)
	, w(other.w)
{}
Vector4::Vector4(const Vector3& vec3, float w)
	: x(vec3.x)
	, y(vec3.y)
	, z(vec3.z)
	, w(w)
{ }