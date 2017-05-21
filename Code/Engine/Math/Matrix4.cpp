#include "Engine/Math/Matrix4.hpp"
#include "Engine/General/Profiler/Profiler.hpp"

#define STATIC

//right
//up
//forward

STATIC const Matrix4 Matrix4::BASIS(Vector4(1.f, 0.f, 0.f, 0.f), Vector4(0.f, 0.f, 1.f, 0.f), Vector4(0.f, 1.f, 0.f, 0.f), Vector4(0.f, 0.f, 0.f, 1.f));
STATIC const Matrix4 Matrix4::IDENTITY(Vector4(1.f, 0.f, 0.f, 0.f), Vector4(0.f, 1.f, 0.f, 0.f), Vector4(0.f, 0.f, 1.f, 0.f), Vector4(0.f, 0.f, 0.f, 1.f));


//-----------------------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//-----------------------------------------------------------------------------------------------------------------------------------------
Matrix4::Matrix4() {
	matrix[0]  = 1.f;   matrix[1]  = 0.f;   matrix[2]  = 0.f;   matrix[3]  = 0.f;
	matrix[4]  = 0.f;   matrix[5]  = 1.f;   matrix[6]  = 0.f;   matrix[7]  = 0.f;
	matrix[8]  = 0.f;   matrix[9]  = 0.f;   matrix[10] = 1.f;   matrix[11] = 0.f;
	matrix[12] = 0.f;   matrix[13] = 0.f;   matrix[14] = 0.f;   matrix[15] = 1.f;
}
Matrix4::Matrix4(Vector4 r1, Vector4 r2, Vector4 r3, Vector4 r4) {
	matrix[0]  = r1.x;   matrix[1]  = r1.y;   matrix[2]  = r1.z;  matrix[3]  = r1.w;
	matrix[4]  = r2.x;   matrix[5]  = r2.y;   matrix[6]  = r2.z;  matrix[7]  = r2.w;
	matrix[8]  = r3.x;   matrix[9]  = r3.y;   matrix[10] = r3.z;  matrix[11] = r3.w;
	matrix[12] = r4.x;   matrix[13] = r4.y;   matrix[14] = r4.z;  matrix[15] = r4.w;
}
Matrix4::Matrix4(float arr[16]) { 
	matrix[0]  = arr[0];   matrix[1]  = arr[1];   matrix[2]  = arr[2];   matrix[3]  = arr[3];
	matrix[4]  = arr[4];   matrix[5]  = arr[5];   matrix[6]  = arr[6];   matrix[7]  = arr[7];
	matrix[8]  = arr[8];   matrix[9]  = arr[9];   matrix[10] = arr[10];  matrix[11] = arr[11];
	matrix[12] = arr[12];  matrix[13] = arr[13];  matrix[14] = arr[14];  matrix[15] = arr[15];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SCALING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC Matrix4 Matrix4::MakeScaleMatrix(float scale) {
	Matrix4 scaleMatrix;

	scaleMatrix.matrix[0] = scale;    scaleMatrix.matrix[1] = 0.f;      scaleMatrix.matrix[2] = 0.f;      scaleMatrix.matrix[3] = 0.f;
	scaleMatrix.matrix[4] = 0.f;      scaleMatrix.matrix[5] = scale;    scaleMatrix.matrix[6] = 0.f;      scaleMatrix.matrix[7] = 0.f;
	scaleMatrix.matrix[8] = 0.f;      scaleMatrix.matrix[9] = 0.f;      scaleMatrix.matrix[10] = scale;   scaleMatrix.matrix[11] = 0.f;

	return scaleMatrix;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC Matrix4 Matrix4::MakeScaleMatrix(const Vector3& scale) {
	Matrix4 scaleMatrix;

	scaleMatrix.matrix[0] = scale.x;    scaleMatrix.matrix[1] = 0.f;      scaleMatrix.matrix[2] = 0.f;      scaleMatrix.matrix[3] = 0.f;
	scaleMatrix.matrix[4] = 0.f;      scaleMatrix.matrix[5] = scale.y;    scaleMatrix.matrix[6] = 0.f;      scaleMatrix.matrix[7] = 0.f;
	scaleMatrix.matrix[8] = 0.f;      scaleMatrix.matrix[9] = 0.f;      scaleMatrix.matrix[10] = scale.z;   scaleMatrix.matrix[11] = 0.f;

	return scaleMatrix;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//MATH
//-----------------------------------------------------------------------------------------------------------------------------------------
void Matrix4::Transpose() {

	Profiler::PushFrame("MatrixTranspose");

	std::swap(matrix[1],  matrix[4]);
	std::swap(matrix[2],  matrix[8]);
	std::swap(matrix[3],  matrix[12]);
	std::swap(matrix[6],  matrix[9]);
	std::swap(matrix[7],  matrix[13]);
	std::swap(matrix[11], matrix[14]);

	Profiler::PopFrame();
}
void Matrix4::ChangeToProjectionMatrix(float xNearClip, float xFarClip, float yNearClip, float yFarClip, float zNearClip, float zFarClip) {
	
	float sx = 1.0f / (xFarClip - xNearClip);
	float sy = 1.0f / (yFarClip - yNearClip);
	float sz = 1.0f / (zFarClip - zNearClip);

	matrix[0]  = 2.0f * sx;  matrix[1] = 0.f;       matrix[2]  = 0.f;        matrix[3]  = -(xFarClip + xNearClip) * sx;
	matrix[4]  = 0.f;        matrix[5] = 2.0f * sy; matrix[6]  = 0.f;        matrix[7]  = -(yFarClip + yNearClip) * sy;
	matrix[8]  = 0.f;        matrix[9] = 0.f;       matrix[10] = 2.0f * sz;  matrix[11] = -(zFarClip + zNearClip) * sz;
	matrix[12] = 0.f;        matrix[13] = 0.f;      matrix[14] = 0.f;        matrix[15] = 1.f;

	Transpose();
}
void Matrix4::ChangeToProjectionMatrix(float screenWidth, float screenHeight, float nearClip, float farClip) {

	float sz = 1.0f / (farClip - nearClip);

	matrix[0] = 2.f / screenWidth;  matrix[1] = 0.f;                matrix[2] = 0.f;        matrix[3] = 0.f;
	matrix[4] = 0.f;                matrix[5] = 2.f / screenHeight; matrix[6] = 0.f;        matrix[7] = 0.f;
	matrix[8] = 0.f;                matrix[9] = 0.f;                matrix[10] = 2.f * sz;  matrix[11] = -(farClip + nearClip) * sz;
	matrix[12] = 0.f;               matrix[13] = 0.f;               matrix[14] = 0.f;       matrix[15] = 1.f;
}




void Matrix4::ChangeToPerspectiveProjection(float yFovDegrees, float aspect, float nearClippingPlane, float farClippingPlane) {
	float rads = ToRadians(yFovDegrees);
	float size = 1.f / (tan(rads / 2.f));
	//float size = atan(rads / 2.0f);

	float w = size;
	float h = size;

	if (aspect > 1.0f) {
		w *= aspect;
	}
	else {
		h /= aspect;
	}

	float q					=  1.f / (farClippingPlane - nearClippingPlane);
	float bottomLeftVal1	=        (farClippingPlane + nearClippingPlane) * q;
	float bottomLeftVal2	= -2.f * nearClippingPlane * farClippingPlane	* q;

	matrix[0]	= 1.f / w;  matrix[1]	= 0.f;		matrix[2]	= 0.f;              matrix[3] = 0.f;
	matrix[4]	= 0.f;      matrix[5]	= 1.f / h;  matrix[6]	= 0.f;              matrix[7] = 0.f;
	matrix[8]	= 0.f;		matrix[9]	= 0.f;      matrix[10]	= bottomLeftVal1;	matrix[11] = 1.f; //Flipped this with bottomLeftVal2 with this for row memory layout
	matrix[12]	= 0.f;      matrix[13]	= 0.f;      matrix[14]	= bottomLeftVal2;	matrix[15] = 0.f;
}
void Matrix4::ChangeToTranslationMatrix(Vector3 position) {
	matrix[12] = position.x; 
	matrix[13] = position.y;
	matrix[14] = position.z;
}
void Matrix4::ChangeToLookAtMatrix(const Vector3& posToLookAt, const Vector3& currPos, const Vector3& scale) {

	Vector3 dir = posToLookAt - currPos; //3
	dir.Normalize(); //

	Vector3 right = CrossProduct(dir, Vector3::UP);
	Vector3 up = CrossProduct(right, dir);
	Vector3 forward = dir;

	right.Normalize();
	up.Normalize();
	forward.Normalize();

	Vector3 sRight		= scale.x * right;
	Vector3 sUp			= scale.y * up;
	Vector3 sForward	= scale.z * forward;

	matrix[0]	 = sRight.x;	matrix[1]  = sRight.y;		matrix[2]   = sRight.z;		matrix[3]  = 0.f;
	matrix[4]	 = sUp.x;		matrix[5]  = sUp.y;			matrix[6]   = sUp.z;		matrix[7]  = 0.f;
	matrix[8]	 = sForward.x;	matrix[9]  = sForward.y;	matrix[10]  = sForward.z;	matrix[11] = 0.f;
	matrix[12]	 = currPos.x;	matrix[13] = currPos.y;		matrix[14]  = currPos.z;	matrix[15] = 1.f;

	//Transpose();
}

void Matrix4::TransposeRotation() {
	std::swap(matrix[1], matrix[4]);
	std::swap(matrix[2], matrix[8]);
	std::swap(matrix[6], matrix[9]);
}

void Matrix4::MatrixMakeRotationEuler(float yaw, float pitch, float roll) {

	float sx = SinDegrees(pitch);
	float cx = CosDegrees(pitch);

	float sy = SinDegrees(roll);
	float cy = CosDegrees(roll);

	float sz = SinDegrees(yaw);
	float cz = CosDegrees(yaw);


	matrix[0] = cy * cz; 
	matrix[1] = cz * sx * sy - cx * sz;
	matrix[2] = cx * cz * sy + sx * sz;
	matrix[3] = matrix[12];

	matrix[4] = cy * sz;
	matrix[5] = cx * cz + sx * sy * sz;
	matrix[6] = -cz * sx + cx * sy * sz;
	matrix[7] = matrix[13];

	matrix[8] = -sy;
	matrix[9] = cy * sx;
	matrix[10] = cx * cy;
	matrix[11] = matrix[14];

	matrix[12] = 0.f;
	matrix[13] = 0.f;
	matrix[14] = 0.f;
	matrix[15] = 1.f;

	//TODO: Transpose this by hand
	Transpose();
}

void Matrix4::SetBasis(Vector3 right, Vector3 up, Vector3 forward) {
	matrix[0] = right.x;     matrix[1] = right.y;     matrix[2] = right.z;      matrix[3] = 0.f;
	matrix[4] = up.x;        matrix[5] = up.y;        matrix[6] = up.z;         matrix[7] = 0.f;
	matrix[8] = forward.x;   matrix[9] = forward.y;   matrix[10] = forward.z;   matrix[11] = 0.f;
}

void Matrix4::ChangePosition(Vector3 pos) {
	matrix[12] = pos.x;
	matrix[13] = pos.y;
	matrix[14] = pos.z;
}


void Matrix4::NegateTranslation(Vector3 position) {

	Vector3 nPos = -1.f * position;

	Vector3 col1 = Vector3(matrix[0], matrix[4], matrix[8]);
	Vector3 col2 = Vector3(matrix[1], matrix[5], matrix[9]);
	Vector3 col3 = Vector3(matrix[2], matrix[6], matrix[10]);
	Vector3 col4 = Vector3(matrix[3], matrix[7], matrix[11]);

	
	Vector3 translatedPos = Vector3(DotProduct(nPos, col1), DotProduct(nPos, col2), DotProduct(nPos, col3));

	matrix[12] = translatedPos.x;
	matrix[13] = translatedPos.y;
	matrix[14] = translatedPos.z;
}

Vector3 Matrix4::GetForward() const {
	return Vector3(matrix[8], matrix[9], matrix[10]);
}
void Matrix4::SetForward(const Vector3& forward) {
	matrix[8] = forward.x;
	matrix[9] = forward.y;
	matrix[10] = forward.z;
}

// Courtesy of Chris Forseth
Matrix4 Matrix4::Inverse() {

	Matrix4 inv;
	double det;
	int i;

	inv.matrix[0] = matrix[5] * matrix[10] * matrix[15] -
		matrix[5] * matrix[11] * matrix[14] -
		matrix[9] * matrix[6] * matrix[15] +
		matrix[9] * matrix[7] * matrix[14] +
		matrix[13] * matrix[6] * matrix[11] -
		matrix[13] * matrix[7] * matrix[10];

	inv.matrix[4] = -matrix[4] * matrix[10] * matrix[15] +
		matrix[4] * matrix[11] * matrix[14] +
		matrix[8] * matrix[6] * matrix[15] -
		matrix[8] * matrix[7] * matrix[14] -
		matrix[12] * matrix[6] * matrix[11] +
		matrix[12] * matrix[7] * matrix[10];

	inv.matrix[8] = matrix[4] * matrix[9] * matrix[15] -
		matrix[4] * matrix[11] * matrix[13] -
		matrix[8] * matrix[5] * matrix[15] +
		matrix[8] * matrix[7] * matrix[13] +
		matrix[12] * matrix[5] * matrix[11] -
		matrix[12] * matrix[7] * matrix[9];

	inv.matrix[12] = -matrix[4] * matrix[9] * matrix[14] +
		matrix[4] * matrix[10] * matrix[13] +
		matrix[8] * matrix[5] * matrix[14] -
		matrix[8] * matrix[6] * matrix[13] -
		matrix[12] * matrix[5] * matrix[10] +
		matrix[12] * matrix[6] * matrix[9];

	inv.matrix[1] = -matrix[1] * matrix[10] * matrix[15] +
		matrix[1] * matrix[11] * matrix[14] +
		matrix[9] * matrix[2] * matrix[15] -
		matrix[9] * matrix[3] * matrix[14] -
		matrix[13] * matrix[2] * matrix[11] +
		matrix[13] * matrix[3] * matrix[10];

	inv.matrix[5] = matrix[0] * matrix[10] * matrix[15] -
		matrix[0] * matrix[11] * matrix[14] -
		matrix[8] * matrix[2] * matrix[15] +
		matrix[8] * matrix[3] * matrix[14] +
		matrix[12] * matrix[2] * matrix[11] -
		matrix[12] * matrix[3] * matrix[10];

	inv.matrix[9] = -matrix[0] * matrix[9] * matrix[15] +
		matrix[0] * matrix[11] * matrix[13] +
		matrix[8] * matrix[1] * matrix[15] -
		matrix[8] * matrix[3] * matrix[13] -
		matrix[12] * matrix[1] * matrix[11] +
		matrix[12] * matrix[3] * matrix[9];

	inv.matrix[13] = matrix[0] * matrix[9] * matrix[14] -
		matrix[0] * matrix[10] * matrix[13] -
		matrix[8] * matrix[1] * matrix[14] +
		matrix[8] * matrix[2] * matrix[13] +
		matrix[12] * matrix[1] * matrix[10] -
		matrix[12] * matrix[2] * matrix[9];

	inv.matrix[2] = matrix[1] * matrix[6] * matrix[15] -
		matrix[1] * matrix[7] * matrix[14] -
		matrix[5] * matrix[2] * matrix[15] +
		matrix[5] * matrix[3] * matrix[14] +
		matrix[13] * matrix[2] * matrix[7] -
		matrix[13] * matrix[3] * matrix[6];

	inv.matrix[6] = -matrix[0] * matrix[6] * matrix[15] +
		matrix[0] * matrix[7] * matrix[14] +
		matrix[4] * matrix[2] * matrix[15] -
		matrix[4] * matrix[3] * matrix[14] -
		matrix[12] * matrix[2] * matrix[7] +
		matrix[12] * matrix[3] * matrix[6];

	inv.matrix[10] = matrix[0] * matrix[5] * matrix[15] -
		matrix[0] * matrix[7] * matrix[13] -
		matrix[4] * matrix[1] * matrix[15] +
		matrix[4] * matrix[3] * matrix[13] +
		matrix[12] * matrix[1] * matrix[7] -
		matrix[12] * matrix[3] * matrix[5];

	inv.matrix[14] = -matrix[0] * matrix[5] * matrix[14] +
		matrix[0] * matrix[6] * matrix[13] +
		matrix[4] * matrix[1] * matrix[14] -
		matrix[4] * matrix[2] * matrix[13] -
		matrix[12] * matrix[1] * matrix[6] +
		matrix[12] * matrix[2] * matrix[5];

	inv.matrix[3] = -matrix[1] * matrix[6] * matrix[11] +
		matrix[1] * matrix[7] * matrix[10] +
		matrix[5] * matrix[2] * matrix[11] -
		matrix[5] * matrix[3] * matrix[10] -
		matrix[9] * matrix[2] * matrix[7] +
		matrix[9] * matrix[3] * matrix[6];

	inv.matrix[7] = matrix[0] * matrix[6] * matrix[11] -
		matrix[0] * matrix[7] * matrix[10] -
		matrix[4] * matrix[2] * matrix[11] +
		matrix[4] * matrix[3] * matrix[10] +
		matrix[8] * matrix[2] * matrix[7] -
		matrix[8] * matrix[3] * matrix[6];

	inv.matrix[11] = -matrix[0] * matrix[5] * matrix[11] +
		matrix[0] * matrix[7] * matrix[9] +
		matrix[4] * matrix[1] * matrix[11] -
		matrix[4] * matrix[3] * matrix[9] -
		matrix[8] * matrix[1] * matrix[7] +
		matrix[8] * matrix[3] * matrix[5];

	inv.matrix[15] = matrix[0] * matrix[5] * matrix[10] -
		matrix[0] * matrix[6] * matrix[9] -
		matrix[4] * matrix[1] * matrix[10] +
		matrix[4] * matrix[2] * matrix[9] +
		matrix[8] * matrix[1] * matrix[6] -
		matrix[8] * matrix[2] * matrix[5];

	det = matrix[0] * inv.matrix[0] + matrix[1] * inv.matrix[4] + matrix[2] * inv.matrix[8] + matrix[3] * inv.matrix[12];

	if (det == 0)
		return false;

	det = 1.0 / det;

	Matrix4 invOut;

	for (i = 0; i < 16; i++) {
		invOut.matrix[i] = inv.matrix[i] * (float)det;
	}

	return invOut;
}

void Matrix4::Scale(const Vector3& scale) {
	matrix[0] *= scale.x;
	matrix[5] *= scale.y;
	matrix[10] *= scale.z;
}

void Matrix4::GetBasis(Vector3* r, Vector3* u, Vector3* f, Vector3* t) {
	*r = Vector3(matrix[0], matrix[1], matrix[2]);
	*u = Vector3(matrix[4], matrix[5], matrix[6]);
	*f = Vector3(matrix[8], matrix[9], matrix[10]);
	*t = Vector3(matrix[12], matrix[13], matrix[14]);
}

STATIC Matrix4 Matrix4::MatrixLerp(Matrix4 a, Matrix4 b, float time) {
	Vector3 r0, u0, f0, t0;
	a.GetBasis(&r0, &u0, &f0, &t0);

	Vector3 r1, u1, f1, t1;
	b.GetBasis(&r1, &u1, &f1, &t1);

	Vector3 r, u, f, t;
	r = Slerp(r0, r1, time);
	u = Slerp(u0, u1, time);
	f = Slerp(f0, f1, time);
	t = Lerp(t0, t1, time);

	Vector4 r4 = Vector4(r.x, r.y, r.z, 0.f);
	Vector4 u4 = Vector4(u.x, u.y, u.z, 0.f);
	Vector4 f4 = Vector4(f.x, f.y, f.z, 0.f);
	Vector4 t4 = Vector4(t.x, t.y, t.z, 1.f);

	return Matrix4(r4, u4, f4, t4);
}




Matrix4 operator*(Matrix4 left, const Matrix4& right) {
	Matrix4 temp;

	temp.matrix[0]	= left.matrix[0] * right.matrix[0] + left.matrix[1] * right.matrix[4] + left.matrix[2] * right.matrix[8] + left.matrix[3] * right.matrix[12];
	temp.matrix[1]	= left.matrix[0] * right.matrix[1] + left.matrix[1] * right.matrix[5] + left.matrix[2] * right.matrix[9] + left.matrix[3] * right.matrix[13];
	temp.matrix[2]	= left.matrix[0] * right.matrix[2] + left.matrix[1] * right.matrix[6] + left.matrix[2] * right.matrix[10] + left.matrix[3] * right.matrix[14];
	temp.matrix[3]	= left.matrix[0] * right.matrix[3] + left.matrix[1] * right.matrix[7] + left.matrix[2] * right.matrix[11] + left.matrix[3] * right.matrix[15];

	temp.matrix[4]	= left.matrix[4] * right.matrix[0] + left.matrix[5] * right.matrix[4] + left.matrix[6] * right.matrix[8] + left.matrix[7] * right.matrix[12];
	temp.matrix[5]	= left.matrix[4] * right.matrix[1] + left.matrix[5] * right.matrix[5] + left.matrix[6] * right.matrix[9] + left.matrix[7] * right.matrix[13];
	temp.matrix[6]	= left.matrix[4] * right.matrix[2] + left.matrix[5] * right.matrix[6] + left.matrix[6] * right.matrix[10] + left.matrix[7] * right.matrix[14];
	temp.matrix[7]	= left.matrix[4] * right.matrix[3] + left.matrix[5] * right.matrix[7] + left.matrix[6] * right.matrix[11] + left.matrix[7] * right.matrix[15];

	temp.matrix[8]	= left.matrix[8] * right.matrix[0] + left.matrix[9] * right.matrix[4] + left.matrix[10] * right.matrix[8] + left.matrix[11] * right.matrix[12];
	temp.matrix[9]	= left.matrix[8] * right.matrix[1] + left.matrix[9] * right.matrix[5] + left.matrix[10] * right.matrix[9] + left.matrix[11] * right.matrix[13];
	temp.matrix[10] = left.matrix[8] * right.matrix[2] + left.matrix[9] * right.matrix[6] + left.matrix[10] * right.matrix[10] + left.matrix[11] * right.matrix[14];
	temp.matrix[11] = left.matrix[8] * right.matrix[3] + left.matrix[9] * right.matrix[7] + left.matrix[10] * right.matrix[11] + left.matrix[11] * right.matrix[15];

	temp.matrix[12] = left.matrix[12] * right.matrix[0] + left.matrix[13] * right.matrix[4] + left.matrix[14] * right.matrix[8] + left.matrix[15] * right.matrix[12];
	temp.matrix[13] = left.matrix[12] * right.matrix[1] + left.matrix[13] * right.matrix[5] + left.matrix[14] * right.matrix[9] + left.matrix[15] * right.matrix[13];
	temp.matrix[14] = left.matrix[12] * right.matrix[2] + left.matrix[13] * right.matrix[6] + left.matrix[14] * right.matrix[10] + left.matrix[15] * right.matrix[14];
	temp.matrix[15] = left.matrix[12] * right.matrix[3] + left.matrix[13] * right.matrix[7] + left.matrix[14] * right.matrix[11] + left.matrix[15] * right.matrix[15];

	left = temp;

	return left;
}


STATIC Matrix4 Matrix4::MultiplyRowMajor(const Matrix4& left, const Matrix4& right) {


	Vector4 l_r1		= Vector4(left.matrix[0],	left.matrix[1],		left.matrix[2],		left.matrix[3]);
	Vector4 l_r2		= Vector4(left.matrix[4],	left.matrix[5],		left.matrix[6],		left.matrix[7]);
	Vector4 l_r3		= Vector4(left.matrix[8],	left.matrix[9],		left.matrix[10],	left.matrix[11]);
	Vector4 l_r4		= Vector4(left.matrix[12],	left.matrix[13],	left.matrix[14],	left.matrix[15]);

	Vector4 r_c1		= Vector4(right.matrix[0],	right.matrix[4],	right.matrix[8],	right.matrix[12]);
	Vector4 r_c2		= Vector4(right.matrix[1],	right.matrix[5],	right.matrix[9],	right.matrix[13]);
	Vector4 r_c3		= Vector4(right.matrix[2],	right.matrix[6],	right.matrix[10],	right.matrix[14]);
	Vector4 r_c4		= Vector4(right.matrix[3],	right.matrix[7],	right.matrix[11],	right.matrix[15]);

	Matrix4 mul;

	mul.SetFirstRow(	Dot(l_r1, r_c1), Dot(l_r1, r_c2), Dot(l_r1, r_c3), Dot(l_r1, r_c4));
	mul.SetSecondRow(	Dot(l_r2, r_c1), Dot(l_r2, r_c2), Dot(l_r2, r_c3), Dot(l_r2, r_c4));
	mul.SetThirdRow(	Dot(l_r3, r_c1), Dot(l_r3, r_c2), Dot(l_r3, r_c3), Dot(l_r3, r_c4));
	mul.SetFourthRow(	Dot(l_r4, r_c1), Dot(l_r4, r_c2), Dot(l_r4, r_c3), Dot(l_r4, r_c4));

	return mul;
}