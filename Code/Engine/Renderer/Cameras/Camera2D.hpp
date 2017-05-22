#pragma once

#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Matrix4.hpp"

class Camera2D {
public:
	//STRUCTORS
	Camera2D();
	Camera2D(const Vector2& pos, float rot, float virtualSize, float aspectRatio);
	Camera2D(const Vector2& pos, float rot, const AABB2& bounds, float virtualSize, float aspectRatio);

	//GETTERS SETTERS
	void	UpdatePosition(const Vector3& delta);
	void	SetPosition(const Vector3& nPos);
	void	UpdateRotation(float delta) { m_rotation += delta;			}
	void	SetRotation(float nRot)		{ m_rotation = nRot;			}
	Vector3 GetPosition() const			{ return m_view.GetPosition();	}
	float	GetRotation() const			{ return m_rotation;			}
	AABB2	GetBounds() const			{ return m_bounds;				}
	void	SetBounds(const AABB2& nBounds);
	Matrix4 GetViewMatrix() const;

	Matrix4		m_proj = Matrix4::IDENTITY;
	Matrix4		m_view = Matrix4::IDENTITY;

private:
	float		m_rotation		= 0.f;
	AABB2		m_bounds		= AABB2(Vector2::ZERO, Vector2::ZERO);
	bool		m_hasBounds		= false;
	float		m_virtualSize	= 0.f;
	float		m_aspectRatio	= 0.f;
	AABB2		m_virtualRect	= AABB2(Vector2::ZERO, Vector2::ZERO);
	Vector2		m_virtualBounds = Vector2::ZERO;
};