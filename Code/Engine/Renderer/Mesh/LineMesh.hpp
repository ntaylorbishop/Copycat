#pragma once

#define _USE_MATH_DEFINES

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Renderer/Vertex/Vertex.hpp"

class Mesh;
class MeshRenderer;
class Material;

class LineMesh {
public:
	//STRUCTORS
	LineMesh();
	LineMesh(Material* mat, std::vector<Vertex2D_PCT> verts, const Vector3& position);

	Vector3								m_position;
	Material*							m_material;
	std::vector<TexturedVertex_TBNBN>	m_verts;
	std::vector<uint16_t>				m_meshIndices;
};