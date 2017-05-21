#pragma once

#include "Engine/Math/MathUtils.hpp"

float CalcShortestAngularDisplacement(float fromDegs, float toDegs);


class EulerAngles {
public:
	//STRUCTORS
	EulerAngles();
	EulerAngles(float pitchAboutX, float yawAboutY, float rollAboutZ);
	~EulerAngles() = default;

	friend EulerAngles operator+(const EulerAngles& lhs, const EulerAngles& rhs);
	friend EulerAngles operator-(const EulerAngles& lhs, const EulerAngles& rhs);
	EulerAngles& operator+=(const EulerAngles& rhs);
	EulerAngles& operator-=(const EulerAngles& rhs);

	void NormalizeBackTo360Degrees();
	static float GetShortestAngularDisplacement(float fromDegs, float toDegs);



	static const EulerAngles ZERO;
	float m_pitchAboutX = 0.f;
	float m_yawAboutY	= 0.f;
	float m_rollAboutZ	= 0.f;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline EulerAngles::EulerAngles()
	: m_pitchAboutX(0.f)
	, m_yawAboutY(0.f)
	, m_rollAboutZ(0.f)
{ }


//---------------------------------------------------------------------------------------------------------------------------
inline EulerAngles::EulerAngles(float pitchAboutX, float yawAboutY, float rollAboutZ)
	: m_pitchAboutX(pitchAboutX)
	, m_yawAboutY(yawAboutY)
	, m_rollAboutZ(rollAboutZ)
{ }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//OPERATORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline EulerAngles operator+(const EulerAngles& lhs, const EulerAngles& rhs) {

	EulerAngles combined;

	float xDiff = CalcShortestAngularDisplacement(lhs.m_pitchAboutX,	rhs.m_pitchAboutX);
	float yDiff = CalcShortestAngularDisplacement(lhs.m_yawAboutY,		rhs.m_yawAboutY);
	float zDiff = CalcShortestAngularDisplacement(lhs.m_rollAboutZ,		rhs.m_rollAboutZ);

	combined.m_pitchAboutX	= lhs.m_pitchAboutX + xDiff;
	combined.m_yawAboutY	= lhs.m_yawAboutY	+ yDiff;
	combined.m_rollAboutZ	= lhs.m_rollAboutZ	+ zDiff;

	combined.NormalizeBackTo360Degrees();

	return combined;
}


//---------------------------------------------------------------------------------------------------------------------------
inline EulerAngles operator-(const EulerAngles& lhs, const EulerAngles& rhs) {

	EulerAngles combined;

	float xDiff = CalcShortestAngularDisplacement(lhs.m_pitchAboutX,	rhs.m_pitchAboutX);
	float yDiff = CalcShortestAngularDisplacement(lhs.m_yawAboutY,		rhs.m_yawAboutY);
	float zDiff = CalcShortestAngularDisplacement(lhs.m_rollAboutZ,		rhs.m_rollAboutZ);
	 
	combined.m_pitchAboutX	= lhs.m_pitchAboutX - xDiff;
	combined.m_yawAboutY	= lhs.m_yawAboutY	- yDiff;
	combined.m_rollAboutZ	= lhs.m_rollAboutZ	- zDiff;

	combined.NormalizeBackTo360Degrees();

	return combined;
}


//---------------------------------------------------------------------------------------------------------------------------
inline EulerAngles& EulerAngles::operator+=(const EulerAngles& rhs) {

	float xDiff = CalcShortestAngularDisplacement(m_pitchAboutX,	rhs.m_pitchAboutX);
	float yDiff = CalcShortestAngularDisplacement(m_yawAboutY,		rhs.m_yawAboutY);
	float zDiff = CalcShortestAngularDisplacement(m_rollAboutZ,		rhs.m_rollAboutZ);

	m_pitchAboutX	+= xDiff;
	m_yawAboutY		+= yDiff;
	m_rollAboutZ	+= zDiff;

	NormalizeBackTo360Degrees();

	return *this;
}


//---------------------------------------------------------------------------------------------------------------------------
inline EulerAngles& EulerAngles::operator-=(const EulerAngles& rhs) {

	float xDiff = CalcShortestAngularDisplacement(m_pitchAboutX,	rhs.m_pitchAboutX);
	float yDiff = CalcShortestAngularDisplacement(m_yawAboutY,		rhs.m_yawAboutY);
	float zDiff = CalcShortestAngularDisplacement(m_rollAboutZ,		rhs.m_rollAboutZ);

	m_pitchAboutX	-= xDiff;
	m_yawAboutY		-= yDiff;
	m_rollAboutZ	-= zDiff;

	NormalizeBackTo360Degrees();

	return *this;
}