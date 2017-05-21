#pragma once

#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Matrix4.hpp"

class Camera2D {
public:
	//STRUCTORS
	Camera2D();
	Camera2D(const Vector2& pos, float rot);
	Camera2D(const Vector2& pos, float rot, const AABB2& bounds);

	//GETTERS SETTERS
	void UpdatePosition(const Vector3& delta);
	void SetPosition(const Vector3& nPos);
	void UpdateRotation(float delta);
	void SetRotation(float nRot);
	AABB2 GetBounds() const { return m_bounds; }
	void SetBounds(const AABB2& nBounds);

	Vector3 GetPosition() const;
	float GetRotation() const;

	Matrix4 GetViewMatrix() const;

private:

	Matrix4 m_view;
	float m_rotation;

	AABB2 m_bounds;
	bool m_hasBounds;
};