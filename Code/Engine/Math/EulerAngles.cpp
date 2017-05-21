#include "Engine/Math/EulerAngles.hpp"

STATIC const EulerAngles EulerAngles::ZERO(0.f, 0.f, 0.f);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UTILS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
float EulerAngles::GetShortestAngularDisplacement(float fromDegs, float toDegs) {

	float angularDisplacement = toDegs - fromDegs;

	while (angularDisplacement > 180.f) {
		angularDisplacement -= 360.f;
	}
	while (angularDisplacement < -180.f) {
		angularDisplacement += 360.f;
	}

	return angularDisplacement;
}


//---------------------------------------------------------------------------------------------------------------------------
void EulerAngles::NormalizeBackTo360Degrees() {

	//PITCH
	while (m_pitchAboutX > 360.f) {
		m_pitchAboutX -= 360.f;
	}
	while (m_pitchAboutX < -360.f) {
		m_pitchAboutX += 360.f;
	}

	//YAW
	while (m_yawAboutY > 360.f) {
		m_yawAboutY -= 360.f;
	}
	while (m_yawAboutY < -360.f) {
		m_yawAboutY += 360.f;
	}

	//ROLL
	while (m_rollAboutZ > 360.f) {
		m_rollAboutZ -= 360.f;
	}
	while (m_rollAboutZ < -360.f) {
		m_rollAboutZ += 360.f;
	}
}