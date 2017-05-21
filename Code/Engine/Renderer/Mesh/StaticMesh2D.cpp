#include "Engine/Renderer/Mesh/StaticMesh2D.hpp"
#include "Engine/Renderer/Material/MaterialBank.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/Material/Material.hpp"
#include "Engine/Renderer/Mesh/Mesh.hpp"
#include "Engine/Renderer/Mesh/MeshRenderer.hpp"



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StaticMesh2D::StaticMesh2D() {
	m_material = MaterialBank::GetMaterial("Default2D");
	m_meshIndices = { 0, 1, 2, 0, 2, 3 };
	std::reverse(m_meshIndices.begin(), m_meshIndices.end());
}


//---------------------------------------------------------------------------------------------------------------------------
StaticMesh2D::StaticMesh2D(Material* mat, std::vector<Vertex2D_PCT> verts, const Vector3& position) {
	m_material = mat;
	m_position = position;
	m_meshIndices = { 0, 1, 2, 0, 2, 3 };
	std::reverse(m_meshIndices.begin(), m_meshIndices.end());
}