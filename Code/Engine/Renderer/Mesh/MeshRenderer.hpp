#pragma once

#include "Engine/Renderer/Material/Material.hpp"
#include "Engine/Renderer/Mesh/Mesh.hpp"
#include "Engine/Renderer/General/RenderCommon.hpp"


class MeshRenderer {
public:
	//STRUCTORS
	MeshRenderer();
	~MeshRenderer();

	MeshRenderer& operator=(const MeshRenderer& other);

	//UPDATE RENDER
	void RenderMeshWithMaterial(MeshID meshId, const Material* mat, const Matrix4& model = Matrix4::IDENTITY, bool drawLines = false) const;

	template <typename T>
	void CreateUniform(const char* name, eUniformType type, size_t count, int bindPoint, T* dataArr);
	template <typename T>
	void CreateUniform(const char* name, eUniformType type, size_t count, T* dataArr);
	Uniform* GetUniform(const char* uniformName);

private:
	void UpdateModelUnifomInShaderProgram(Material* mat, const Matrix4& model) const;
	
private:
	int						m_vaoID		= 0;
	mutable std::vector<Uniform*>	m_uniforms;

	friend class Model;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline MeshRenderer& MeshRenderer::operator=(const MeshRenderer& other) {
	m_vaoID = other.m_vaoID;
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
void MeshRenderer::CreateUniform(const char* name, eUniformType type, size_t count, int bindPoint, T* dataArr) {

	ASSERT_OR_DIE(Uniform::IsCorrectType(type, dataArr), "ERROR: Can't create uniform. Wrong type.");
	//ASSERT_OR_DIE(!Uniform::NeedsBindPoint(type) && bindPoint == -1, "ERROR: Bind point uniform must have bind point.");

	for (uint8_t i = 0; i < m_uniforms.size(); i++) {
		ASSERT_OR_DIE(m_uniforms[i]->GetNameHash() != HashCString(name), "ERROR: Uniform already exists.");
	}

	Uniform* nUniform = new(MEMORY_RENDERING) Uniform(name, count, type, -1, bindPoint, (void*)dataArr);
	m_uniforms.push_back(nUniform);
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
void MeshRenderer::CreateUniform(const char* name, eUniformType type, size_t count, T* dataArr) {
	CreateUniform(name, type, count, -1, dataArr);
}