#pragma once

#include "Engine/Math/Vector3.hpp"

struct Ray {
	Ray(const Vector3& origin, const Vector3& dir) 
		: m_origin(origin)
		, m_dir(dir) 
	{ }

	Vector3 m_origin	= Vector3::ZERO;
	Vector3 m_dir		= Vector3::ZERO;
};