#include "Engine/Renderer/D3D11/Mesh/D3D11Vertex.hpp"

//---------------------------------------------------------------------------------------------------------------------------
size_t CreateVertex(byte* buffer, size_t bufferSize, const Vector3& pos, const RGBA& col) {
	memcpy_s(buffer, bufferSize, (void*)&pos, sizeof(Vector3));
	buffer += sizeof(Vector3);
	memcpy_s(buffer, bufferSize, (void*)&col, sizeof(RGBA));
	return sizeof(Vector3) + sizeof(RGBA);
}


//---------------------------------------------------------------------------------------------------------------------------
size_t CreateVertex(byte* buffer, size_t bufferSize, const Vector3& pos, const RGBA& col, const Vector2& tex) {
	memcpy_s(buffer, bufferSize, (void*)&pos, sizeof(Vector3));
	buffer += sizeof(Vector3);
	memcpy_s(buffer, bufferSize, (void*)&col, sizeof(RGBA));
	buffer += sizeof(RGBA);
	memcpy_s(buffer, bufferSize, (void*)&tex, sizeof(Vector2));
	return sizeof(Vector3) + sizeof(RGBA) + sizeof(Vector2);
}


//---------------------------------------------------------------------------------------------------------------------------
size_t CreateVertex(byte* buffer, size_t bufferSize, const Vector3& pos, const RGBA& col, const Vector2& tex, const Vector4& normal) {
	memcpy_s(buffer, bufferSize, (void*)&pos, sizeof(Vector3));
	buffer += sizeof(Vector3);
	memcpy_s(buffer, bufferSize, (void*)&col, sizeof(RGBA));
	buffer += sizeof(RGBA);
	memcpy_s(buffer, bufferSize, (void*)&tex, sizeof(Vector2));
	buffer += sizeof(Vector2);
	memcpy_s(buffer, bufferSize, (void*)&normal, sizeof(Vector4));
	return sizeof(Vector3) + sizeof(RGBA) + sizeof(Vector2) + sizeof(Vector4);
}


//---------------------------------------------------------------------------------------------------------------------------
size_t CreateVertex(byte* buffer, size_t bufferSize, const Vector3& pos, const RGBA& col, const Vector2& tex, const Vector4& tangent, const Vector4& bitangent, const Vector4& normal) {
	memcpy_s(buffer, bufferSize, (void*)&pos, sizeof(Vector3));
	buffer += sizeof(Vector3);
	memcpy_s(buffer, bufferSize, (void*)&col, sizeof(RGBA));
	buffer += sizeof(RGBA);
	memcpy_s(buffer, bufferSize, (void*)&tex, sizeof(Vector2));
	buffer += sizeof(Vector2);
	memcpy_s(buffer, bufferSize, (void*)&tangent, sizeof(Vector4));
	buffer += sizeof(Vector4);
	memcpy_s(buffer, bufferSize, (void*)&bitangent, sizeof(Vector4));
	buffer += sizeof(Vector4);
	memcpy_s(buffer, bufferSize, (void*)&normal, sizeof(Vector4));
	return sizeof(Vector3) + sizeof(RGBA) + sizeof(Vector2) + sizeof(Vector4) + sizeof(Vector4) + sizeof(Vector4);
}


//---------------------------------------------------------------------------------------------------------------------------
size_t CreateVertex(byte* buffer, size_t bufferSize, const Vector3& pos, const RGBA& col, const Vector2& tex, const Vector4& tangent, const Vector4& bitangent, const Vector4& normal, const Vector4& boneWeights, const IntVector4& boneInds) {
	memcpy_s(buffer, bufferSize, (void*)&pos, sizeof(Vector3));
	buffer += sizeof(Vector3);
	memcpy_s(buffer, bufferSize, (void*)&col, sizeof(RGBA));
	buffer += sizeof(RGBA);
	memcpy_s(buffer, bufferSize, (void*)&tex, sizeof(Vector2));
	buffer += sizeof(Vector2);
	memcpy_s(buffer, bufferSize, (void*)&tangent, sizeof(Vector4));
	buffer += sizeof(Vector4);
	memcpy_s(buffer, bufferSize, (void*)&bitangent, sizeof(Vector4));
	buffer += sizeof(Vector4);
	memcpy_s(buffer, bufferSize, (void*)&normal, sizeof(Vector4));
	buffer += sizeof(Vector4);
	memcpy_s(buffer, bufferSize, (void*)&boneWeights, sizeof(Vector4));
	buffer += sizeof(Vector4);
	memcpy_s(buffer, bufferSize, (void*)&boneInds, sizeof(IntVector4));
	return sizeof(Vector3) + sizeof(RGBA) + sizeof(Vector2) + sizeof(Vector4) + sizeof(Vector4) + sizeof(Vector4) + sizeof(IntVector4);
}


//---------------------------------------------------------------------------------------------------------------------------
uint16_t GetVertexByteSize(eVertexType vertType) {
	switch (vertType) {
	case VERTEX_TYPE_PC: {
		return sizeof(Vector3) + sizeof(RGBA);
	}
	case VERTEX_TYPE_PCT: {
		return sizeof(Vector3) + sizeof(RGBA) + sizeof(Vector2);
	}
	case VERTEX_TYPE_PCTN: {
		return sizeof(Vector3) + sizeof(RGBA) + sizeof(Vector2) + sizeof(Vector4);
	}
	case VERTEX_TYPE_PCTTBN: {
		return sizeof(Vector3) + sizeof(RGBA) + sizeof(Vector2) + 3 * sizeof(Vector4);
	}
	case VERTEX_TYPE_PCTTBNBN: {
		return sizeof(Vector3) + sizeof(RGBA) + sizeof(Vector2) + 3 * sizeof(Vector4) + sizeof(Vector4) + sizeof(IntVector4);
	}
	case VERTEX_TYPE_NUM_TYPES: {
		return 0;
	}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void SetInputLayout(eVertexType vertType, D3D11_INPUT_ELEMENT_DESC*& layout, UINT& numElements) {
	
	switch (vertType) {
	case VERTEX_TYPE_PC: {
		layout = new D3D11_INPUT_ELEMENT_DESC[2];
		layout[0] = { "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layout[1] = { "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 };
		numElements = 2;
		break;
	}
	case VERTEX_TYPE_PCT: {
		layout = new D3D11_INPUT_ELEMENT_DESC[3];
		layout[0] = { "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layout[1] = { "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layout[2] = { "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 28,	D3D11_INPUT_PER_VERTEX_DATA, 0 };
		numElements = 3;
		break;
	}
	case VERTEX_TYPE_PCTN: {
		layout = new D3D11_INPUT_ELEMENT_DESC[4];
		layout[0] = { "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layout[1] = { "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layout[2] = { "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 28,	D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layout[3] = { "NORMAL",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 36,	D3D11_INPUT_PER_VERTEX_DATA, 0 };
		numElements = 4;
		break;
	}
	case VERTEX_TYPE_PCTTBN: {
		layout = new D3D11_INPUT_ELEMENT_DESC[6];
		layout[0] = { "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layout[1] = { "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layout[2] = { "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 28,	D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layout[3] = { "TANGENT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 36,	D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layout[4] = { "BINORMAL",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 52,	D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layout[5] = { "NORMAL",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 68, 	D3D11_INPUT_PER_VERTEX_DATA, 0 };
		numElements = 6;
		break;
	}
	case VERTEX_TYPE_PCTTBNBN: {
		break;
	}
	}
}