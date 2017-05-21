#include "Engine/Math/IntVector3.hpp"
#include <math.h>

IntVector3::IntVector3() { }
IntVector3::IntVector3(int x, int y, int z) 
	: x(x)
	, y(y) 
	, z(z)
{}
IntVector3::IntVector3(const IntVector3& other) 
	: x(other.x)
	, y(other.y) 
	, z(other.z)
{}