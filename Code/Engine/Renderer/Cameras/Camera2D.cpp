#include "Engine/Renderer/Cameras/Camera2D.hpp"
#include "Engine/General/Logger/TheLogger.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void InitProjection(Vector2& virtualBounds, Matrix4& ortho, AABB2& virtualRect, float aspectRatio, float virtualSize) {


	if (aspectRatio < 1.f) {
		virtualBounds.y = virtualSize / aspectRatio;
		virtualBounds.x = virtualSize;
	}
	else {
		virtualBounds.x = virtualSize * aspectRatio;
		virtualBounds.y = virtualSize;
	}

	virtualRect = AABB2(Vector2(-virtualBounds.x / 2.f, -virtualBounds.y / 2.f), Vector2(virtualBounds.x / 2.f, virtualBounds.y / 2.f));

	ortho.ChangeToProjectionMatrix(virtualBounds.x, virtualBounds.y, -1.f, 1.f);

}

//---------------------------------------------------------------------------------------------------------------------------
Camera2D::Camera2D() 
	: m_view(Matrix4::IDENTITY)
	, m_rotation(0.f)
	, m_bounds(AABB2(Vector2(0.f, 0.f), Vector2(0.f, 0.f)))
	, m_hasBounds(false)
{ }


//---------------------------------------------------------------------------------------------------------------------------
Camera2D::Camera2D(const Vector2& pos, float rot, float virtualSize, float aspectRatio)
	: m_view(Matrix4::IDENTITY)
	, m_bounds(AABB2(Vector2(0.f, 0.f), Vector2(0.f, 0.f)))
	, m_hasBounds(false)
	, m_virtualSize(virtualSize)
	, m_aspectRatio(aspectRatio)
{
	InitProjection(m_virtualBounds, m_proj, m_virtualRect, m_aspectRatio, m_virtualSize);
	m_view.ChangePosition(Vector3(pos.x, 0.f, pos.y));
	m_rotation = rot;
}


//---------------------------------------------------------------------------------------------------------------------------
Camera2D::Camera2D(const Vector2& pos, float rot, const AABB2& bounds, float virtualSize, float aspectRatio)
	: m_view(Matrix4::IDENTITY)
	, m_bounds(bounds)
	, m_hasBounds(true)
	, m_virtualSize(virtualSize)
	, m_aspectRatio(aspectRatio)
{
	InitProjection(m_virtualBounds, m_proj, m_virtualRect, m_aspectRatio, m_virtualSize);
	m_view.ChangePosition(Vector3(pos.x, 0.f, pos.y));
	m_rotation = rot;

	if (pos.x < m_bounds.mins.x || pos.x > m_bounds.maxs.x || pos.y < m_bounds.mins.y || pos.y > m_bounds.maxs.y) {
		TheLogger::LogPrintf("ERROR: Camera2D starting position out of set bounds. Resetting to origin", "SpriteRenderer", LOG_DEFAULT);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GETTERS SETTERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void Camera2D::UpdatePosition(const Vector3& delta) {
	Vector3 pos = m_view.GetPosition();

	pos.x += delta.x;
	pos.y += delta.y;

	if (!m_hasBounds) {
		m_view.ChangePosition(pos);
		return;
	}

	if (pos.x < m_bounds.mins.x) {
		pos.x = m_bounds.mins.x;
	}
	else if (pos.x > m_bounds.maxs.x) {
		pos.x = m_bounds.maxs.x;
	}

	if (pos.y < m_bounds.mins.y) {
		pos.y = m_bounds.mins.y;
	}
	else if (pos.y > m_bounds.maxs.y) {
		pos.y = m_bounds.maxs.y;
	}

	m_view.ChangePosition(pos);
}


//---------------------------------------------------------------------------------------------------------------------------
void Camera2D::SetPosition(const Vector3& nPos) {

	Vector3 pos = nPos;

	if (!m_hasBounds) {
		m_view.ChangePosition(Vector3(pos.x, pos.y, 0.f));
		return;
	}

	if (pos.x < m_bounds.mins.x) {
		pos.x = m_bounds.mins.x;
	}
	else if (pos.x > m_bounds.maxs.x) {
		pos.x = m_bounds.maxs.x;
	}

	if (pos.y < m_bounds.mins.y) {
		pos.y = m_bounds.mins.y;
	}
	else if (pos.y > m_bounds.maxs.y) {
		pos.y = m_bounds.maxs.y;
	}

	m_view.ChangePosition(Vector3(pos.x, pos.y, 0.f));
}


//---------------------------------------------------------------------------------------------------------------------------
Matrix4 Camera2D::GetViewMatrix() const {
	Matrix4 viewInverse = m_view;

	viewInverse.MatrixMakeRotationEuler(m_rotation, m_rotation, 0.f);
	viewInverse.TransposeRotation();
	viewInverse.NegateTranslation(m_view.GetPosition());

	return viewInverse;
}


//---------------------------------------------------------------------------------------------------------------------------
void Camera2D::SetBounds(const AABB2& nBounds) {
	m_bounds = nBounds;
	m_hasBounds = true;
}