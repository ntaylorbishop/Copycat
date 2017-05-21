#pragma once

#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/VectorUtils.hpp"
#include "Engine/Math/EulerAngles.hpp"

class Matrix4 {
public:
	//STRUCTORS
	Matrix4();
	Matrix4(Vector4 r1, Vector4 r2, Vector4 r3, Vector4 r4);
	Matrix4(float arr[16]);

	//SCALING
	static Matrix4 MakeScaleMatrix(float scale);
	static Matrix4 MakeScaleMatrix(const Vector3& scale);

	//MATH
	void Transpose();
	void ChangeToProjectionMatrix(float xNearClip, float xFarClip, float yNearClip, float yFarClip, float zNearClip, float zFarClip);
	void ChangeToProjectionMatrix(float screenWidth, float screenHeight, float nearClip, float farClip);

	void ChangeToPerspectiveProjection(float yFovDegrees, float aspect, float nearClippingPLane, float farClippingPlane);
	void ChangeToTranslationMatrix(Vector3 position);
	void ChangeToLookAtMatrix(const Vector3& posToLookAt, const Vector3& currPos, const Vector3& scale);

	void TransposeRotation();

	void MatrixMakeRotationEuler(float yaw, float pitch, float roll);

	void SetBasis(Vector3 right, Vector3 up, Vector3 forward);

	void ChangePosition(Vector3 pos);

	void Scale(const Vector3& scale);



	//GETTERS SETTERS
	Vector4 GetFirstRow() const;
	Vector4 GetSecondRow() const;
	Vector4 GetThirdRow() const;
	Vector4 GetFourthRow() const;
	Vector3 GetPosition() const;

	Vector3 GetForward() const;
	void SetForward(const Vector3& forward);

	void SetFirstRow(const Vector4& row);
	void SetSecondRow(const Vector4& row);
	void SetThirdRow(const Vector4& row);
	void SetFourthRow(const Vector4& row);


	void SetFirstRow( float r0, float r1, float r2, float r3);
	void SetSecondRow(float r0, float r1, float r2, float r3);
	void SetThirdRow( float r0, float r1, float r2, float r3);
	void SetFourthRow(float r0, float r1, float r2, float r3);

	void NegateTranslation(Vector3 position);

	void GetBasis(Vector3* r, Vector3* u, Vector3* f, Vector3* t);

	static Matrix4 MatrixLerp(Matrix4 a, Matrix4 b, float time);

	
	
	
	//NEW STUFF
	static Matrix4 MultiplyRowMajor(const Matrix4& left, const Matrix4& right);





	//STATIC
	Matrix4 Inverse();

	//OPERATOR OVERLOADS
	Matrix4& operator=(const Matrix4& other);
	bool operator==(const Matrix4& other);
	bool operator!=(const Matrix4& other);

	friend Matrix4 operator*(Matrix4 left, const Matrix4& right);
	friend Vector4 operator*(Vector4 left, const Matrix4& right);
	friend Vector4 operator*(const Matrix4& right, Vector4 left);

public:
	float matrix[16];

public:
	static const Matrix4 BASIS;
	static const Matrix4 IDENTITY;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINE FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline Vector4 Matrix4::GetFirstRow() const {
	return Vector4(matrix[0], matrix[1], matrix[2], matrix[3]);
}


//---------------------------------------------------------------------------------------------------------------------------
inline Vector4 Matrix4::GetSecondRow() const {
	return Vector4(matrix[4], matrix[5], matrix[6], matrix[7]);
};


//---------------------------------------------------------------------------------------------------------------------------
inline Vector4 Matrix4::GetThirdRow() const {
	return Vector4(matrix[8], matrix[9], matrix[10], matrix[11]);
}


//---------------------------------------------------------------------------------------------------------------------------
inline Vector4 Matrix4::GetFourthRow() const {
	return Vector4(matrix[12], matrix[13], matrix[14], matrix[15]);
}


//---------------------------------------------------------------------------------------------------------------------------
inline Vector3 Matrix4::GetPosition() const {
	return Vector3(matrix[12], matrix[13], matrix[14]);
}


//---------------------------------------------------------------------------------------------------------------------------
inline void Matrix4::SetFirstRow(const Vector4& row) {
	matrix[0] = row.x;
	matrix[1] = row.y;
	matrix[2] = row.z;
	matrix[3] = row.w;
}
inline void Matrix4::SetSecondRow(const Vector4& row) {
	matrix[4] = row.x;
	matrix[5] = row.y;
	matrix[6] = row.z;
	matrix[7] = row.w;
}
inline void Matrix4::SetThirdRow(const Vector4& row) {
	matrix[8] = row.x;
	matrix[9] = row.y;
	matrix[10] = row.z;
	matrix[11] = row.w;
}
inline void Matrix4::SetFourthRow(const Vector4& row) {
	matrix[12] = row.x;
	matrix[13] = row.y;
	matrix[14] = row.z;
	matrix[15] = row.w;
}

inline void Matrix4::SetFirstRow(float r0, float r1, float r2, float r3) {
	matrix[0] = r0;
	matrix[1] = r1;
	matrix[2] = r2;
	matrix[3] = r3;
}
inline void Matrix4::SetSecondRow(float r0, float r1, float r2, float r3) {
	matrix[4] = r0;
	matrix[5] = r1;
	matrix[6] = r2;
	matrix[7] = r3;
}
inline void Matrix4::SetThirdRow(float r0, float r1, float r2, float r3) {
	matrix[8]	= r0;
	matrix[9]	= r1;
	matrix[10]	= r2;
	matrix[11]	= r3;
}
inline void Matrix4::SetFourthRow(float r0, float r1, float r2, float r3) {
	matrix[12] = r0;
	matrix[13] = r1;
	matrix[14] = r2;
	matrix[15] = r3;
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//OPERATOR OVERLOADS
//-----------------------------------------------------------------------------------------------------------------------------------------
inline Matrix4& Matrix4::operator=(const Matrix4& other) {
	matrix[0] = other.matrix[0];   matrix[1] = other.matrix[1];   matrix[2] = other.matrix[2];   matrix[3] = other.matrix[3];
	matrix[4] = other.matrix[4];   matrix[5] = other.matrix[5];   matrix[6] = other.matrix[6];   matrix[7] = other.matrix[7];
	matrix[8] = other.matrix[8];   matrix[9] = other.matrix[9];   matrix[10] = other.matrix[10]; matrix[11] = other.matrix[11];
	matrix[12] = other.matrix[12]; matrix[13] = other.matrix[13]; matrix[14] = other.matrix[14]; matrix[15] = other.matrix[15];

	return *this;
}
inline bool Matrix4::operator==(const Matrix4& other) {
	return matrix == other.matrix;
}
inline bool Matrix4::operator!=(const Matrix4& other) {
	return matrix != other.matrix;
}

inline Vector4 operator*(Vector4 left, const Matrix4& right) {
	Vector4 temp;

	temp.x = left.x * right.matrix[0] + left.y * right.matrix[4] + left.z * right.matrix[8] + left.w * right.matrix[12];
	temp.y = left.x * right.matrix[1] + left.y * right.matrix[5] + left.z * right.matrix[9] + left.w * right.matrix[13];
	temp.z = left.x * right.matrix[2] + left.y * right.matrix[6] + left.z * right.matrix[10] + left.w * right.matrix[14];
	temp.w = left.x * right.matrix[3] + left.y * right.matrix[7] + left.z * right.matrix[11] + left.w * right.matrix[15];

	left = temp;

	return left;
}

inline Vector4 operator*(const Matrix4& right, Vector4 left) {
	Vector4 temp;

	temp.x = left.x * right.matrix[0] + left.y * right.matrix[4] + left.z * right.matrix[8] + left.w * right.matrix[12];
	temp.y = left.x * right.matrix[1] + left.y * right.matrix[5] + left.z * right.matrix[9] + left.w * right.matrix[13];
	temp.z = left.x * right.matrix[2] + left.y * right.matrix[6] + left.z * right.matrix[10] + left.w * right.matrix[14];
	temp.w = left.x * right.matrix[3] + left.y * right.matrix[7] + left.z * right.matrix[11] + left.w * right.matrix[15];

	left = temp;

	return left;
}