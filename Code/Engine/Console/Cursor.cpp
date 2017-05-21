#include "Engine/Console/Cursor.hpp"

//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
Cursor::Cursor(float height, float thickness, float blinkSpeed, Vector2 startingPos)
	: m_height(height)
	, m_thickness(thickness)
	, m_blinkSpeed(blinkSpeed)
	, m_age(0.f)
	, m_position(startingPos)
	, m_startingPosition(startingPos)
	, m_tickingDown(false)
{

}
Cursor::~Cursor() {

}

//UPDATE RENDER
//---------------------------------------------------------------------------------------------------------------------------
void Cursor::Update(float deltaSeconds) {
	
	if (m_age >= m_blinkSpeed) {
		m_tickingDown = true;
	}
	else if (m_age <= -m_blinkSpeed) {
		m_tickingDown = false;
	}

	if (m_tickingDown)
		m_age -= deltaSeconds;
	else
		m_age += deltaSeconds;
}
void Cursor::BlinkCursor() const {
	if (m_age > 0.f) {
		AABB2 cursorPipe = AABB2(m_position, m_position + Vector2(m_thickness, m_height));
		BeirusRenderer::DrawAABB2(cursorPipe, RGBA::WHITE);
	}
}

void Cursor::MoveCursor(float amount) {
	m_position.x += amount;
}
void Cursor::ResetCursorPosition() {
	m_position.x = m_startingPosition.x;
}

//GETTERS SETTERS
//---------------------------------------------------------------------------------------------------------------------------
Vector2 Cursor::GetStartingPosition() {
	return m_startingPosition;
}
float Cursor::GetHeight() {
	return m_height;
}