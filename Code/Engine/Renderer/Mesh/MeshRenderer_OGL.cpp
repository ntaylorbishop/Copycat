#include "Engine/Config/BuildConfig.hpp"

#ifdef GRAPHICS_API_OGL
#include "Engine/Renderer/Mesh/MeshRenderer.hpp"
#include "Engine/Renderer/Shaders/ShaderProgram.hpp"
#include "Engine/General/Core/TheMemoryManager.hpp"
#include "Engine/Renderer/Structures/BeirusMeshCollection.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
MeshRenderer::MeshRenderer() {

	GL_CHECK(glGenVertexArrays(1, (GLuint*)&m_vaoID));
}

MeshRenderer::~MeshRenderer() {
	GL_CHECK(glDeleteVertexArrays(1, (GLuint*)&m_vaoID));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void MeshRenderer::RenderMeshWithMaterial(MeshID meshId, const Material* mat, const Matrix4& model, bool drawLines) const {

	Mesh* mesh = BeirusMeshCollection::Get()->GetMesh(meshId);
	Material* cMat = const_cast<Material*>(mat);

	GL_CHECK(glBindVertexArray(m_vaoID));
	mesh->BindMesh();

	UpdateModelUnifomInShaderProgram(cMat, model);

	cMat->UseMaterial(mesh->GetVertexDefinition(), &m_uniforms);

	if (mat->m_renderState.m_backfaceCulling) {
		BeirusRenderer::EnableBackfaceCulling();
	}
	else if (!mat->m_renderState.m_backfaceCulling) {
		BeirusRenderer::DisableBackfaceCulling();
	}

	mesh->DrawMesh(drawLines);

	mesh->UnbindMesh();
	cMat->StopUsingMaterial();

	GL_CHECK(glBindVertexArray(NULL));
	GL_CHECK(glActiveTexture(GL_TEXTURE0));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE MEMBER FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void MeshRenderer::UpdateModelUnifomInShaderProgram(Material* mat, const Matrix4& model) const {

	ShaderProgram* prog		= mat->GetShaderProgram();
	Uniform* uniToChange	= prog->GetUniform("gModel");

	if (uniToChange != nullptr) {
		uniToChange->SetData((void*)&model);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
Uniform* MeshRenderer::GetUniform(const char* uniformName) {

	size_t uniformNameHash = HashCString(uniformName);

	for (unsigned int i = 0; i < m_uniforms.size(); i++) {

		if (m_uniforms[i]->GetNameHash() == uniformNameHash) {
			return m_uniforms[i];
		}
	}

	return nullptr;
}

#endif