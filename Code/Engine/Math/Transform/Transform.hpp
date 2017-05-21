#pragma once

#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/EulerAngles.hpp"

class Transform {
public:
	//STRUCTORS
	Transform();
	Transform(const Vector3& pos, const EulerAngles& rot);
	Transform(const Vector2& pos, const EulerAngles& rot);
	Transform(const Transform& other) = default;
	~Transform() = default;

	//POSITION
	void ChangePosition(const Vector2& pos);
	void ChangePosition(const Vector3& pos);
	Vector3 GetPosition() const;

	//ROTATION
	void SetRotation(const EulerAngles& rotation);
	void AddRotation(const EulerAngles& rotation);

	Matrix4 GetMatrix4();


	//OPERATORS
	Transform& operator*=(const Transform& rhs);
	const Transform Transform::operator*(const Transform& rhs) const;


	static const Transform IDENTITY;

private:
	void Construct();

	Matrix4		m_transform = Matrix4::IDENTITY;
	Vector3		m_position	= Vector3::ZERO;
	EulerAngles m_rotation	= EulerAngles::ZERO;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline Transform::Transform() 
	: m_transform(Matrix4::IDENTITY)
	, m_position(Vector3::ZERO)
	, m_rotation(EulerAngles::ZERO)
{ }


//---------------------------------------------------------------------------------------------------------------------------
inline Transform::Transform(const Vector3& pos, const EulerAngles& rot) 
	: m_transform(Matrix4::IDENTITY)
	, m_position(pos)
	, m_rotation(rot)
{ }


//---------------------------------------------------------------------------------------------------------------------------
inline Transform::Transform(const Vector2& pos, const EulerAngles& rot) 
	: m_transform(Matrix4::IDENTITY)
	, m_position(Vector3(pos, 0.f))
	, m_rotation(rot)
{ }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//OPERATORS AND OTHERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline Matrix4 Transform::GetMatrix4() {

	Construct();
	return m_transform;
}


//---------------------------------------------------------------------------------------------------------------------------
inline Transform& Transform::operator*=(const Transform& rhs) {

	this->m_transform = Matrix4::MultiplyRowMajor(this->m_transform, rhs.m_transform);
	return *this;
}


//---------------------------------------------------------------------------------------------------------------------------
inline const Transform Transform::operator*(const Transform& rhs) const {

	return Transform(*this) *= rhs;
}