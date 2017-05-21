#include "Engine/Math/PolarCoords.hpp"

//STRUCTORS
PolarCoords::PolarCoords() { }
PolarCoords::PolarCoords(float radius, float theta) {
	this->radius = radius;
	this->theta = theta;
}
PolarCoords::PolarCoords(const IntVector2& v) {
	radius = v.Length();
	theta = atan2f((float)v.y, (float)v.x);
}
PolarCoords::PolarCoords(const Vector2& v) {
	radius = v.Length();
	theta = atan2f(v.y, v.x);
}

Vector2 PolarCoords::ToVector2_InDegrees() {
	return Vector2(radius * CosDegrees(theta), radius * SinDegrees(theta));
}
Vector2 PolarCoords::ToVector2_InRadians() {
	return Vector2(radius * cos(theta), radius * sin(theta));
}