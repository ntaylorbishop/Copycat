#include "Engine/Renderer/Cameras/PlayerCamera3D.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"

STATIC PlayerCamera3D* PlayerCamera3D::s_thePlayerCamera3D = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INTERNAL INITIALIZE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void PlayerCamera3D::Initialize(const Vector3& position, float roll, float pitch, float yaw) {

	s_thePlayerCamera3D->m_position		= position;
	s_thePlayerCamera3D->m_rollAboutZ	= roll;
	s_thePlayerCamera3D->m_pitchAboutX	= pitch;
	s_thePlayerCamera3D->m_yawAboutY	= yaw;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void PlayerCamera3D::Destroy() {

	if (s_thePlayerCamera3D == nullptr) {
		return;
	}

	delete s_thePlayerCamera3D;
	s_thePlayerCamera3D = nullptr;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GETTERS SETTERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC Vector3 PlayerCamera3D::GetForwardXYZ() { 

	if (s_thePlayerCamera3D == nullptr) {
		SystemInitialize();
	}

	float x = CosDegrees(s_thePlayerCamera3D->m_yawAboutY) * CosDegrees(s_thePlayerCamera3D->m_pitchAboutX);
	float y = SinDegrees(s_thePlayerCamera3D->m_yawAboutY) * CosDegrees(s_thePlayerCamera3D->m_pitchAboutX);
	float z = SinDegrees(s_thePlayerCamera3D->m_pitchAboutX);

	return Vector3(x, y, z); 
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC Vector3 PlayerCamera3D::GetForwardXZ() { 

	if (s_thePlayerCamera3D == nullptr) {
		SystemInitialize();
	}

	return Vector3(SinDegrees(s_thePlayerCamera3D->m_yawAboutY), 0.f, CosDegrees(s_thePlayerCamera3D->m_yawAboutY));
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC Vector3 PlayerCamera3D::GetLeftXZ() { 

	if (s_thePlayerCamera3D == nullptr) {
		SystemInitialize();
	}

	return Vector3(CosDegrees(s_thePlayerCamera3D->m_yawAboutY), 0.f, -SinDegrees(s_thePlayerCamera3D->m_yawAboutY));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void PlayerCamera3D::Update(float deltaSeconds) {

	UNREFERENCED_PARAMETER(deltaSeconds);

	if (s_thePlayerCamera3D == nullptr) {
		SystemInitialize();
	}

	float pitch = s_thePlayerCamera3D->m_pitchAboutX;
	float yaw	= s_thePlayerCamera3D->m_yawAboutY;

	s_thePlayerCamera3D->m_invView.MatrixMakeRotationEuler(0.f, pitch, yaw);

	s_thePlayerCamera3D->m_view = s_thePlayerCamera3D->m_invView;

	s_thePlayerCamera3D->m_invView.TransposeRotation();
	s_thePlayerCamera3D->m_invView.NegateTranslation(s_thePlayerCamera3D->m_position);

// 	s_thePlayerCamera3D->m_view.ChangeToLookAtMatrix(lookAtPos, s_thePlayerCamera3D->m_position);
// 	s_thePlayerCamera3D->m_view.TransposeRotation();
// 	s_thePlayerCamera3D->m_view.NegateTranslation(s_thePlayerCamera3D->m_position);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INTERNAL STRUCTORS AND INIT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//---------------------------------------------------------------------------------------------------------------------------
STATIC void PlayerCamera3D::SystemInitialize() {

	ASSERT_OR_DIE(s_thePlayerCamera3D == nullptr, "ERROR: PlayerCamera already initialized.");
	s_thePlayerCamera3D = new PlayerCamera3D(Vector3::ZERO, 0.f, 0.f, 0.f);
	BeirusRenderer::AttachPlayerCamera3D(s_thePlayerCamera3D);
}