#pragma once

#include "DirectXMath.h"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/MathUtils.hpp"

using namespace DirectX;

class Matrix44 {
public:
	Matrix44() { }
	Matrix44(XMVECTOR r1, XMVECTOR r2, XMVECTOR r3, XMVECTOR r4) 
		: m_matrix(r1, r2, r3, r4)
	{ }
	Matrix44(XMMATRIX mat) 
		: m_matrix(mat)
	{ }

	void LookAt(Vector3& camPos, float yaw, float pitch, float roll);
	void SetAsPerspectiveProjection(float fovRads, float aspectRatio, float near, float farPlane);
	void SetAsIdentity();
	void Transpose();
	void Invert();


	XMMATRIX m_matrix;
};


//---------------------------------------------------------------------------------------------------------------------------
inline Matrix44 Inverse(XMMATRIX mat) {
	return Matrix44(XMMatrixInverse(nullptr, mat));
}


//---------------------------------------------------------------------------------------------------------------------------
inline Matrix44 TransposeMatrix(XMMATRIX mat) {

	return Matrix44(XMMatrixTranspose(mat));
}


//---------------------------------------------------------------------------------------------------------------------------
inline void Matrix44::Invert() {
	m_matrix = XMMatrixInverse(nullptr, m_matrix);
}


//---------------------------------------------------------------------------------------------------------------------------
inline void Matrix44::Transpose() {

	m_matrix = XMMatrixTranspose(m_matrix);
}


//---------------------------------------------------------------------------------------------------------------------------
inline void Matrix44::LookAt(Vector3& camPos, float yaw, float pitch, float roll) {

	XMVECTOR eye = XMVectorSet(camPos.x, camPos.y, camPos.z, 1.f);
	m_matrix = XMMatrixRotationRollPitchYaw(ToRadians(pitch), ToRadians(yaw), ToRadians(roll));

	m_matrix.r[3] = eye;
}


//---------------------------------------------------------------------------------------------------------------------------
inline void Matrix44::SetAsPerspectiveProjection(float fovRads, float aspectRatio, float nearPlane, float farPlane) {
	m_matrix = XMMatrixPerspectiveFovLH(fovRads, aspectRatio, nearPlane, farPlane);
}


//---------------------------------------------------------------------------------------------------------------------------
inline void Matrix44::SetAsIdentity() {
	m_matrix = XMMatrixIdentity();
}