#pragma once

#include "Engine/Math/Objects/GeomObject.hpp"
#include "Engine/Math/Vector3.hpp"

class Sphere : public GeomObject {
public:
	//STRUCTORS
	Sphere() 
		: GeomObject(GEOM_TYPE_SPHERE)
		, m_pos()
		, m_radius() 
	{ }
	Sphere(const Vector3& pos, float radius) 
		: GeomObject(GEOM_TYPE_SPHERE)
		, m_pos(pos)
		, m_radius(radius) 
	{ }


	//MEMBERS
	Vector3 m_pos		= Vector3::ZERO;
	float	m_radius	= 0.f;
};