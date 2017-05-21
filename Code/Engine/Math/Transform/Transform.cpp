#include "Engine/Math/Transform/Transform.hpp"


STATIC const Transform Transform::IDENTITY = Transform();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UTILS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void Transform::Construct() {

	float cy = CosDegrees(m_rotation.m_yawAboutY);
	float sy = SinDegrees(m_rotation.m_yawAboutY);

	float cp = CosDegrees(m_rotation.m_pitchAboutX);
	float sp = SinDegrees(m_rotation.m_pitchAboutX);

	float cr = CosDegrees(m_rotation.m_rollAboutZ);
	float sr = SinDegrees(m_rotation.m_rollAboutZ);

	m_transform.matrix[0]	= cy * cr - sy * sp * sr;
	m_transform.matrix[1]	= cy * sr + cr * sy * sp;
	m_transform.matrix[2]	= -sy * cp;
	m_transform.matrix[3]	= 0.f;

	m_transform.matrix[4]	= -sr * cp;
	m_transform.matrix[5]	= cr * cp;
	m_transform.matrix[6]	= sp;
	m_transform.matrix[7]	= 0.f;

	m_transform.matrix[8]	= sy * cr + sr * sp * cy;
	m_transform.matrix[9]	= sr * sy - cr * sp * cy;
	m_transform.matrix[10]	= cy * sp;
	m_transform.matrix[11]	= 0.f;

	m_transform.matrix[12]	= m_position.x;
	m_transform.matrix[13]	= m_position.y;
	m_transform.matrix[14]	= m_position.z;
	m_transform.matrix[15]	= 1.f;
}