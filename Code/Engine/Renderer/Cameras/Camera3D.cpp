#include "Engine/Renderer/Cameras/Camera3D.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void Camera3D::Update(float deltaSeconds) {

	UNREFERENCED_PARAMETER(deltaSeconds);

	m_view.LookAt(m_position, m_yawAboutY, m_pitchAboutX, m_rollAboutZ);
	m_view.Transpose();
	m_view.Invert();
}