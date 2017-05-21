#pragma once

#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Objects/GeomObject.hpp"

class AABB3 : public GeomObject {
public:
	//STRUCTORS
	AABB3() 
		: GeomObject(GEOM_TYPE_AABB3)
		, mins()
		, maxs() 
	{ }
	AABB3(const Vector3& mins, const Vector3& maxs)
		: GeomObject(GEOM_TYPE_AABB3)
		, mins(mins)
		, maxs(maxs)
	{ }

	//MEMBERS
	Vector3 mins;
	Vector3 maxs;
};