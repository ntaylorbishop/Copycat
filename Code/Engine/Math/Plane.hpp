#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/VectorUtils.hpp"

//n * (P - P0) = 0

class Plane {
public:
	Plane(const Vector3& p0, const Vector3& normal) : m_p0(p0), m_normal(normal) { }

	static inline bool IsSphereInFrustrum(const Vector3& spherePos,    float radius,
		                                  const Plane& leftFrustrum,   const Plane& rightFrustrum, const Plane& topFrustrum,
		                                  const Plane& bottomFrustrum, const Plane& backFrustrum,  const Plane& frontFrustrum);

	inline bool IsPointAbovePlane(const Vector3& point) const;
	inline bool DoesSphereIntersectOrLieBelow(const Vector3& pos, float radius) const;

private:
	Vector3 m_p0;
	Vector3 m_normal;
};


////////////////////////////////////////////////////
//------------------------------------------
//INLINES

//---------------------------------------------------------------------------------------------------------------------------
STATIC INLINE bool Plane::IsSphereInFrustrum(const Vector3& spherePos,    float radius,
	                                         const Plane& leftFrustrum,   const Plane& rightFrustrum, const Plane& topFrustrum,
	                                         const Plane& bottomFrustrum, const Plane& backFrustrum,  const Plane& frontFrustrum)
{
	
	bool aboveLeftFrustrum		= leftFrustrum.DoesSphereIntersectOrLieBelow(spherePos, radius);
	bool aboveRightFrustrum		= rightFrustrum.DoesSphereIntersectOrLieBelow(spherePos, radius);
	bool aboveTopFrustrum		= topFrustrum.DoesSphereIntersectOrLieBelow(spherePos, radius);
	bool aboveBottomFrustrum	= bottomFrustrum.DoesSphereIntersectOrLieBelow(spherePos, radius);
	bool aboveBackFrustrum		= backFrustrum.DoesSphereIntersectOrLieBelow(spherePos, radius);
	bool aboveFrontFrustrum		= frontFrustrum.DoesSphereIntersectOrLieBelow(spherePos, radius);

	return (aboveLeftFrustrum	 &&
			aboveRightFrustrum	 &&
			aboveTopFrustrum	 &&
			aboveBottomFrustrum	 &&
			aboveBackFrustrum	 &&
			aboveFrontFrustrum);
}


//---------------------------------------------------------------------------------------------------------------------------
INLINE bool Plane::IsPointAbovePlane(const Vector3& point) const  {

	Vector3 pointToP0 = point - m_p0;
	pointToP0.Normalize();

	if (DotProduct(pointToP0, m_normal) > 0.f) {
		return true;
	}
	else {
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
INLINE bool Plane::DoesSphereIntersectOrLieBelow(const Vector3& pos, float radius) const {

	Vector3 eVec = pos - m_p0;	
	float distOfCenterToPlane = DotProduct(m_normal, eVec);

	if (abs(distOfCenterToPlane) <= radius || (abs(distOfCenterToPlane) >= radius && distOfCenterToPlane < 0.f)) {
		return true;
	}
	else {
		return false;
	}
}