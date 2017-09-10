#pragma once

#include "stdint.h"
#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/IntVector4.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Engine/Renderer/D3D11/General/RHIDeviceWindow.hpp"


enum eVertexType : uint8_t {
	VERTEX_TYPE_PC = 0,
	VERTEX_TYPE_PCT,
	VERTEX_TYPE_PCTN,
	VERTEX_TYPE_PCTTBN,
	VERTEX_TYPE_PCTTBNBN,
	VERTEX_TYPE_NUM_TYPES
};



size_t CreateVertex(byte* buffer, size_t bufferSize, const Vector3& pos, const RGBA& col);
size_t CreateVertex(byte* buffer, size_t bufferSize, const Vector3& pos, const RGBA& col, const Vector2& tex);
size_t CreateVertex(byte* buffer, size_t bufferSize, const Vector3& pos, const RGBA& col, const Vector2& tex, const Vector4& normal);
size_t CreateVertex(byte* buffer, size_t bufferSize, const Vector3& pos, const RGBA& col, const Vector2& tex, const Vector4& tangent, const Vector4& bitangent, const Vector4& normal);
size_t CreateVertex(byte* buffer, size_t bufferSize, const Vector3& pos, const RGBA& col, const Vector2& tex, const Vector4& tangent, const Vector4& bitangent, const Vector4& normal, const Vector4& boneWeights, const IntVector4& boneInds);

uint16_t GetVertexByteSize(eVertexType vertType);

void SetInputLayout(eVertexType vertType, D3D11_INPUT_ELEMENT_DESC*& layout, UINT& numElements);