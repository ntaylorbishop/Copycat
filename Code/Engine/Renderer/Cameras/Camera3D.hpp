#pragma once

#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Math/MathUtils.hpp"

class Camera3D {
public:
	Camera3D() { }
	Camera3D(const Vector3& position, float roll, float pitch, float yaw);
	~Camera3D() { }

	void Update(float deltaSeconds);

	void SetPerspectiveProjection(float fov, float aspect, float near, float farPlane);

	//GETTERS SETTERS
	Vector3 GetForwardXYZ();
	Vector3 GetForwardXZ();
	Vector3 GetLeftXZ();


	Vector3		m_position;
	float		m_pitchAboutX	= 0.f;
	float		m_yawAboutY		= 0.f;
	float		m_rollAboutZ	= 0.f;
	Matrix44	m_view;
	Matrix44	m_proj;

private:

};


//---------------------------------------------------------------------------------------------------------------------------
inline Camera3D::Camera3D(const Vector3& position, float roll, float pitch, float yaw)
	: m_position(position)
	, m_rollAboutZ(roll)
	, m_pitchAboutX(pitch)
	, m_yawAboutY(yaw)
{
	m_view.LookAt(m_position, yaw, pitch, roll);
}


//---------------------------------------------------------------------------------------------------------------------------
inline void Camera3D::SetPerspectiveProjection(float fovDegrees, float aspect, float nearPlane, float farPlane) {
	m_proj.SetAsPerspectiveProjection(ToRadians(fovDegrees), aspect, nearPlane, farPlane);
	m_proj.Transpose();
}


//---------------------------------------------------------------------------------------------------------------------------
inline Vector3 Camera3D::GetForwardXZ() {

	return Vector3(SinDegrees(m_yawAboutY), 0.f, CosDegrees(m_yawAboutY));
}


//---------------------------------------------------------------------------------------------------------------------------
inline Vector3 Camera3D::GetLeftXZ() {

	return Vector3(CosDegrees(m_yawAboutY), 0.f, -SinDegrees(m_yawAboutY));
}


//---------------------------------------------------------------------------------------------------------------------------
inline Vector3 Camera3D::GetForwardXYZ() {

	float x = CosDegrees(m_yawAboutY) * CosDegrees(m_pitchAboutX);
	float y = SinDegrees(m_yawAboutY) * CosDegrees(m_pitchAboutX);
	float z = SinDegrees(m_pitchAboutX);

	return Vector3(x, y, z);
}