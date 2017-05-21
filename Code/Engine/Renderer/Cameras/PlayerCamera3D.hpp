#pragma once

#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/Math.hpp"

class PlayerCamera3D {
public:
	static void Initialize(const Vector3& position, float roll, float pitch, float yaw);
	static void Destroy();

	//UPDATE RENDER
	static void Update(float deltaSeconds);
	 
	//GETTERS SETTERS
	static Vector3 GetForwardXYZ();
	static Vector3 GetForwardXZ();
	static Vector3 GetLeftXZ();
	
	static const Matrix4* GetViewNotInvMatrix();
	static const Matrix4* GetViewMatrix();
	static const Vector3* GetPosition();


	Vector3 m_position	= Vector3::ZERO;
	float m_pitchAboutX = 0.f;
	float m_yawAboutY	= 0.f;
	float m_rollAboutZ	= 0.f;
	Matrix4 m_view		= Matrix4::IDENTITY;
	Matrix4 m_invView	= Matrix4::IDENTITY;

private:
	//STRUCTORS
	PlayerCamera3D();
	PlayerCamera3D(const Vector3& position, float roll, float pitch, float yaw);
	static void SystemInitialize();


	static PlayerCamera3D* s_thePlayerCamera3D;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline PlayerCamera3D::PlayerCamera3D()
	: m_position()
	, m_pitchAboutX(0.f)
	, m_yawAboutY(0.f)
	, m_rollAboutZ(0.f)
{ }


//---------------------------------------------------------------------------------------------------------------------------
inline PlayerCamera3D::PlayerCamera3D(const Vector3& position, float roll, float pitch, float yaw)
	: m_position(position)
	, m_pitchAboutX(pitch)
	, m_yawAboutY(yaw)
	, m_rollAboutZ(roll)
{ }


//---------------------------------------------------------------------------------------------------------------------------
STATIC inline const Matrix4* PlayerCamera3D::GetViewNotInvMatrix() { 

	if (s_thePlayerCamera3D == nullptr) {
		SystemInitialize();
	}
	return &s_thePlayerCamera3D->m_view; 
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC inline const Matrix4* PlayerCamera3D::GetViewMatrix() { 

	if (s_thePlayerCamera3D == nullptr) {
		SystemInitialize();
	}
	return &s_thePlayerCamera3D->m_invView; 
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC inline const Vector3* PlayerCamera3D::GetPosition() { 

	if (s_thePlayerCamera3D == nullptr) {
		SystemInitialize();
	}
	return &s_thePlayerCamera3D->m_position; 
}