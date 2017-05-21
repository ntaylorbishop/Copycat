#include "Engine/Math/VectorUtils.hpp"

float VectorToAngle(const IntVector2& point) {
	return atan2f((float)point.y, (float)point.x);
}
float VectorToAngle(const Vector2& point) {
	return atan2f(point.y, point.x);
}
float VectorToAngle(const IntVector3& point) {
	return atan2f((float)point.y, (float)point.x);
}
float VectorToAngle(const Vector3& point) {
	return atan2f(point.y, point.x);
}



float DotProduct(const IntVector2& v1, const IntVector2& v2) {
	return (float)((v1.x * v2.x) + (v1.y * v2.y));
}
float DotProduct(const Vector2& v1, const Vector2& v2) {
	return (v1.x * v2.x) + (v1.y * v2.y);
}
float DotProduct(const IntVector3& v1, const IntVector3& v2) {
	return (float)((v1.x * v2.x) + (v1.y * v2.y));
}
float DotProduct(const Vector3& v1, const Vector3& v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}
float DotProduct(const Vector4& v1, const Vector4& v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.z * v2.z);
}

Vector3 CrossProduct(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

Vector2 IncreaseVectorByLength(const Vector2& v, float length) {
	return Vector2(v.x * length, v.y * length);
}
Vector3 IncreaseVectorByLength(const Vector3& v, float length) {
	return Vector3(v.x * length, v.y * length, v.z * length);
}


IntVector2 FindNormalToLine(const IntVector2& p1, const IntVector2& p2, bool positiveHalfSpace) {
	IntVector2 d = IntVector2(p2.x - p1.x, p2.y - p1.y);

	//Positive: (dy, -dx) and Negative: (-dy, dx)

	if (positiveHalfSpace)
		return IntVector2(d.y, -d.x);
	else
		return IntVector2(-d.y, d.x);
}
Vector2 FindNormalToLine(const Vector2& p1, const Vector2& p2, bool positiveHalfSpace) {
	Vector2 d = Vector2(p2.x - p1.x, p2.y - p1.y);

	//Positive: (dy, -dx) and Negative: (-dy, dx)

	if (positiveHalfSpace)
		return Vector2(d.y, -d.x);
	else
		return Vector2(-d.y, d.x);
}

std::string ToString(const IntVector2& v) {
	return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
}
std::string ToString(const Vector2& v) {
	return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
}
std::string ToString(const IntVector3& v) {
	return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
}
std::string ToString(const Vector3& v) {
	return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
}