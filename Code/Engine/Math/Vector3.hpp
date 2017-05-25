#pragma once

#include <math.h>
#include "Engine/Utils/ErrorWarningAssert.hpp"
#include "Engine/Math/Vector2.hpp"

class Vector3 {
public:
	//STRUCTORS
	constexpr Vector3()									: x(),			y(),		z()		{ }
	constexpr Vector3(float val)						: x(val),		y(val),		z(val)	{ }
	constexpr Vector3(float x, float y, float z)		: x(x),			y(y),		z(z)	{ }
	constexpr Vector3(const Vector2& other, float z)	: x(other.x),	y(other.y), z(z)	{ }
	Vector3(const Vector3& other) = default;


	//COMPS
	float Length() const;
	void Normalize();
	Vector3 GetNormal();
	Vector2 xy() { return Vector2(x, y); }
	Vector2 xz() { return Vector2(x, z); }
	Vector2 yz() { return Vector2(y, z); }

	//OPERATOR OVERLOADS
	Vector3 operator-();

	friend Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
	friend Vector3 operator+(const Vector3& lhs, const float rhs);
	friend Vector3 operator+(const float lhs, const Vector3& rhs);

	friend Vector3 operator-(const Vector3& lhs, const Vector3& rhs);
	friend Vector3 operator-(const Vector3& lhs, const float rhs);
	friend Vector3 operator-(const float lhs, const Vector3& rhs);

	friend Vector3 operator*(const Vector3& lhs, const Vector3& rhs);
	friend Vector3 operator*(const Vector3& lhs, const float rhs);
	friend Vector3 operator*(const float lhs, const Vector3& rhs);

	Vector3& operator+=(const Vector3& rhs);
	Vector3& operator+=(const float scalar);
	Vector3& operator-=(const Vector3& rhs);
	Vector3& operator-=(const float scalar);
	Vector3& operator*=(const Vector3& rhs);
	Vector3& operator*=(const float rhs);

	friend Vector3 operator/(const Vector3& lhs, const float rhs);
	friend Vector3 operator/(const Vector3& lhs, const Vector3& rhs);
	Vector3& operator/=(const float rhs);

	//CONDITIONAL OPERATORS
	bool operator<(const Vector3 &other) const;
	bool operator>(const Vector3 &other) const;
	bool operator<=(const Vector3 &other) const;
	bool operator>=(const Vector3 &other) const;
	bool operator==(const Vector3 &other) const;
	bool operator!=(const Vector3 &other) const;

public:
	static const Vector3 ZERO;
	static const Vector3 ONE;
	static const Vector3 UNIT_VECTOR_X;
	static const Vector3 UNIT_VECTOR_Y;
	static const Vector3 UNIT_VECTOR_Z;
	static const Vector3 RIGHT;
	static const Vector3 UP;
	static const Vector3 FORWARD;


	float x;
	float y;
	float z;
};


//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINE FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//COMPS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline float Vector3::Length() const {
	return sqrt(x * x + y * y + z * z);
}


//---------------------------------------------------------------------------------------------------------------------------
inline void Vector3::Normalize() {
	float length = sqrt(x * x + y * y + z * z);

	if (length != 0.f) {
		x = x / length;
		y = y / length;
		z = z / length;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
inline Vector3 Vector3::GetNormal() {

	float length = sqrt(x * x + y * y + z * z);

	if (length != 0.f) {
		x = x / length;
		y = y / length;
		z = z / length;
	}

	return *this;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//OPERATOR OVERLOADS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
inline Vector3 Vector3::operator-() {
	return Vector3(-x, -y, -z);
}


//---------------------------------------------------------------------------------------------------------------------------
inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs) {  
	return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); 
}
inline Vector3 operator+(const Vector3& lhs, const float rhs) {
	return Vector3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
}
inline Vector3 operator+(const float lhs, const Vector3& rhs) {
	return Vector3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
}


//---------------------------------------------------------------------------------------------------------------------------
inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs) {
	return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
inline Vector3 operator-(const Vector3& lhs, const float rhs) { 
	return Vector3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs); 
}
inline Vector3 operator-(const float lhs, const Vector3& rhs) {
	return Vector3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
}


//---------------------------------------------------------------------------------------------------------------------------
inline Vector3 operator*(const Vector3& lhs, const Vector3& rhs) {
	return Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z); 
}
inline Vector3 operator*(const Vector3& lhs, const float rhs) {
	return Vector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}
inline Vector3 operator*(const float lhs, const Vector3& rhs) {
	return Vector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}


//---------------------------------------------------------------------------------------------------------------------------
inline Vector3& Vector3::operator+=(const Vector3& rhs) {
	x += rhs.x;	
	y += rhs.y;		
	z += rhs.z;		
	return *this; 
}
inline Vector3& Vector3::operator+=(const float scalar) {
	x -= scalar;	
	y -= scalar;	
	z -= scalar;	
	return *this; 
}
inline Vector3& Vector3::operator-=(const Vector3& rhs) {
	x -= rhs.x;	
	y -= rhs.y;		
	z -= rhs.z;		
	return *this; 
}
inline Vector3& Vector3::operator-=(const float scalar) {
	x -= scalar;	
	y -= scalar;	
	z -= scalar;	
	return *this; 
}
inline Vector3& Vector3::operator*=(const Vector3& rhs) {
	x *= rhs.x;	
	y *= rhs.y;		
	z *= rhs.z;		
	return *this; 
}
inline Vector3& Vector3::operator*=(const float rhs) {
	x *= rhs;		
	y *= rhs;		
	z *= rhs;		
	return *this; 
}


//---------------------------------------------------------------------------------------------------------------------------
inline Vector3 operator/(const Vector3& lhs, const float rhs) {
	return Vector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}
inline Vector3 operator/(const Vector3& lhs, const Vector3& rhs) {
	return Vector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
inline Vector3& Vector3::operator/=(const float rhs) {
	x /= rhs;
	y /= rhs;
	z /= rhs;
	return *this;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CONDITIONAL OPERATORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline bool Vector3::operator<(const Vector3 &other) const {
	if (x < other.x) {
		return true;
	}
	else if (x > other.x) {
		return false;
	}
	else if (x == other.x) {
		if (y < other.y) {
			return true;
		}
		else if (y > other.y) {
			return false;
		}
		else if (y == other.y) {
			if (z < other.z) {
				return true;
			}
			else {
				return false;
			}
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
inline bool Vector3::operator>(const Vector3 &other) const {
	return !(*this < other);
}


//---------------------------------------------------------------------------------------------------------------------------
inline bool Vector3::operator<=(const Vector3 &other) const {
	if (x <= other.x) {
		return true;
	}
	else {
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
inline bool Vector3::operator>=(const Vector3 &other) const {
	if (x >= other.x) {
		return true;
	}
	else {
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
inline bool Vector3::operator==(const Vector3 &other) const {
	if (x == other.x && y == other.y && z == other.z) {
		return true;
	}
	else {
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
inline bool Vector3::operator!=(const Vector3 &other) const {
	return !(*this == other);
}