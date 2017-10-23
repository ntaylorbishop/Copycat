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
	Matrix44(Vector4 r1, Vector4 r2, Vector4 r3, Vector4 r4)
		: m_matrix(XMVectorSet(r1.x, r1.y, r1.z, r1.w), 
					XMVectorSet(r2.x, r2.y, r2.z, r2.w),
					XMVectorSet(r3.x, r3.y, r3.z, r3.w),
					XMVectorSet(r4.x, r4.y, r4.z, r4.w))
	{ }
	Matrix44(XMMATRIX mat) 
		: m_matrix(mat)
	{ }

	void LookAt(Vector3& camPos, float yaw, float pitch, float roll);
	void SetAsPerspectiveProjection(float fovRads, float aspectRatio, float near, float farPlane);
	void SetAsOrthographicProjection(float viewWidth, float viewHeight, float zNear, float zFar);
	void SetAsIdentity();
	void Transpose();
	void Invert();
	void SetPosition(const Vector3& position);
	void Scale(const Vector3& scale);

	XMMATRIX m_matrix;

	static const Matrix44 BASIS;
	static const Matrix44 IDENTITY;
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
inline void Matrix44::SetAsOrthographicProjection(float viewWidth, float viewHeight, float zNear, float zFar) {
	m_matrix = XMMatrixOrthographicOffCenterLH(0.f, viewWidth, 0.f, viewHeight, zNear, zFar);
}


//---------------------------------------------------------------------------------------------------------------------------
inline void Matrix44::SetAsIdentity() {
	m_matrix = XMMatrixIdentity();
}


//---------------------------------------------------------------------------------------------------------------------------
inline void Matrix44::SetPosition(const Vector3& position) {

	XMVECTOR pos = XMVectorSet(position.x, position.y, position.z, 1.f);
	m_matrix.r[3] = pos;
}


//---------------------------------------------------------------------------------------------------------------------------
inline void Matrix44::Scale(const Vector3& scale) {

	XMMATRIX scalingMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);
	m_matrix = scalingMatrix * m_matrix;
}