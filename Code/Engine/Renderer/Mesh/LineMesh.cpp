#include "Engine/Renderer/Mesh/LineMesh.hpp"
#include "Engine/Renderer/Mesh/MeshRenderer.hpp"
#include "Engine/Renderer/Mesh/Mesh.hpp"
#include "Engine/Renderer/Material/MaterialBank.hpp"
#include "Engine/Renderer/Material/Material.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LineMesh::LineMesh() {

	m_material = MaterialBank::GetMaterial("LineDrawing");
	m_meshIndices = { 0, 1 };
}


//---------------------------------------------------------------------------------------------------------------------------
LineMesh::LineMesh(Material* mat, std::vector<Vertex2D_PCT> verts, const Vector3& position) {

	m_material		= mat;
	m_position		= position;
	m_meshIndices	= { 0, 1 };
}