#include "Engine/Math/MatrixStack44.hpp"

MatrixStack44::MatrixStack44() {
	m_stack.push_back(Matrix4::IDENTITY);
}

bool MatrixStack44::Empty() {
	return (m_stack.size() == 1);
}

Matrix4 MatrixStack44::Top() {
	return m_stack.back();
}

void MatrixStack44::Push(const Matrix4& transform) {
	Matrix4 top = Top();
	Matrix4 newTop = transform * top;
	m_stack.push_back(newTop);
}
void MatrixStack44::Pop() {
	if (!Empty()) {
		m_stack.pop_back();
	}
}