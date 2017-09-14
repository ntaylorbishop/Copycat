#include "Engine/Math/Matrix44.hpp"

STATIC const Matrix44 Matrix44::BASIS(XMVectorSet(1.f,0.f,0.f,0.f),XMVectorSet(0.f,1.f,0.f,0.f),XMVectorSet(0.f,0.f,1.f,0.f),XMVectorSet(0.f,0.f,0.f,1.f));
STATIC const Matrix44 Matrix44::IDENTITY(XMVectorSet(1.f,0.f,0.f,0.f),XMVectorSet(0.f,1.f,0.f,0.f),XMVectorSet(0.f,0.f,1.f,0.f),XMVectorSet(0.f,0.f,0.f,1.f));