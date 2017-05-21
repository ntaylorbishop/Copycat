#include "Engine/Math/IntVector4.hpp"
#define STATIC //Do-nothing to indicate static

STATIC const IntVector4 IntVector4::ZERO(0, 0, 0, 0);
STATIC const IntVector4 IntVector4::ONE(1, 1, 1, 1);
STATIC const IntVector4 IntVector4::UNIT_VECTOR_X(1, 0, 0, 0);
STATIC const IntVector4 IntVector4::UNIT_VECTOR_Y(0, 1, 0, 0);
STATIC const IntVector4 IntVector4::UNIT_VECTOR_Z(0, 0, 1, 0);
STATIC const IntVector4 IntVector4::UNIT_VECTOR_W(0, 0, 0, 1);

//STRUCTORS
IntVector4::IntVector4() {}
IntVector4::IntVector4(int x, int y, int z, int w)
	: x(x)
	, y(y)
	, z(z)
	, w(w)
{}
IntVector4::IntVector4(const IntVector4& other)
	: x(other.x)
	, y(other.y)
	, z(other.z)
	, w(other.w)
{}