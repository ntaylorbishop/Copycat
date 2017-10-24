#include "Engine/Scene/D3D11Scene.hpp"
#include "Engine/Renderer/D3D11/Mesh/D3D11Model.hpp"
#include "Engine/Renderer/D3D11/Material/D3D11Material.hpp"
#include "Engine/Renderer/Mesh/MeshBuilder.hpp"
#include "Engine/Tools/FBX/SceneImport.hpp"
#include "Engine/Renderer/Effects/DepthAndNormalsPass.hpp"
#include "Engine/Renderer/General/RenderCommon.hpp"
#include "Engine/Renderer/Structures/BeirusMeshCollection.hpp"
#include "Engine/Renderer/D3D11/Mesh/D3D11MeshRenderer.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOADING AND SAVING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void D3D11Scene::AddModelsFromDirectory(const char* dir) {

	std::vector<String> filenames = FileUtils::GetAllFilenamesInDirectory(dir);

	for (size_t i = 0; i < filenames.size(); i++) {

		D3D11Model* newMesh = new D3D11Model(Vector3::ZERO);
		String matName;
		newMesh->m_mesh = MeshBuilder::ReadD3D11MeshFromFile(filenames[i], &matName);
		matName = matName.substr(0, matName.size() - 1);
		newMesh->m_materialName = matName;
		newMesh->SetMaterial(matName);

		Vector4 r1 = Vector4(1.f, 0.f, 0.f, 0.f);
		Vector4 r2 = Vector4(0.f, 0.f, 1.f, 0.f);
		Vector4 r3 = Vector4(0.f, 1.f, 0.f, 0.f);
		Vector4 r4 = Vector4(0.f, 0.f, 0.f, 1.f);

		newMesh->m_model = Matrix44(r1, r2, r3, r4);

		AddModel(newMesh);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC D3D11Scene* D3D11Scene::LoadSceneFromDirectory(const char* sceneDir) {

	std::vector<String> filenames = FileUtils::GetAllFilenamesInDirectory(sceneDir);

	if (filenames.empty()) {
		return nullptr;
	}

	D3D11Scene* newScene = new D3D11Scene();

	for (size_t i = 0; i < filenames.size(); i++) {

		D3D11Model* newMesh = new D3D11Model(Vector3::ZERO);
		String matName;
		newMesh->m_mesh = MeshBuilder::ReadD3D11MeshFromFile(filenames[i], &matName);
		matName = matName.substr(0, matName.size() - 1);
		newMesh->m_materialName = matName;
		newMesh->SetMaterial(matName);

		Vector4 r1 = Vector4(1.f, 0.f, 0.f, 0.f);
		Vector4 r2 = Vector4(0.f, 0.f, 1.f, 0.f);
		Vector4 r3 = Vector4(0.f, 1.f, 0.f, 0.f);
		Vector4 r4 = Vector4(0.f, 0.f, 0.f, 1.f);

		newMesh->m_model = Matrix44(r1, r2, r3, r4);

		newScene->AddModel(newMesh);
	}

	return newScene;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void D3D11Scene::RenderForShadows(D3D11Material* shadowMat) const {

	D3D11Model* curr = m_opaqueMeshes;

	static size_t idx = 0;

	while (curr != nullptr) {
		curr->Render(shadowMat);
		curr = curr->m_next;
		idx++;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
void D3D11Scene::RenderWithMaterial(D3D11Material* mat) const {

	D3D11Model* curr = m_opaqueMeshes;

	while (curr != nullptr) {
		curr->Render(mat);
		curr = curr->m_next;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Scene::Render(bool isRenderingTransparent, D3D11MeshRenderer* customMeshRenderer) const {

	D3D11Model* curr = m_opaqueMeshes;

	while (curr != nullptr) {
		curr->Render(customMeshRenderer);
		curr = curr->m_next;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Scene::RenderSkybox(D3D11MeshRenderer* customMeshRenderer) const {

	//m_skybox->Render(customMeshRenderer);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADDING REMOVING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void D3D11Scene::AddModel(D3D11Model* newMesh) {

	AddStaticMeshToOpaquePass(newMesh);
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Scene::RemoveModel(D3D11Model* mesh) {

	RemoveStaticMeshFromOpaquePass(mesh);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE ADD REMOVE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void D3D11Scene::AddStaticMeshToOpaquePass(D3D11Model* newMesh) {

	if (!m_opaqueMeshes) {
		m_opaqueMeshes = newMesh;
		newMesh->m_next = nullptr;
		newMesh->m_prev = nullptr;
	}
	else {
		D3D11Model* head = m_opaqueMeshes;
		m_opaqueMeshes = newMesh;
		m_opaqueMeshes->m_next = head;
		m_opaqueMeshes->m_prev = nullptr;
		m_opaqueMeshes->m_next->m_prev = m_opaqueMeshes;
	}
	m_numMeshes++;
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11Scene::RemoveStaticMeshFromOpaquePass(D3D11Model* mesh) {

	D3D11Model* toRemove = mesh;

	//WARNING: BROKEN. IMPLEMENT THE TRANSLUCENT VERSION OF THIS - IT IS CORRECT

	if (toRemove->m_prev && toRemove->m_next) {	//MIDDLE

		D3D11Model* prev = toRemove->m_prev;
		D3D11Model* next = toRemove->m_next;

		prev->m_next = next;
		next->m_prev = prev;
	}
	else if (!toRemove->m_prev && toRemove->m_next) { //HEAD

		m_opaqueMeshes = toRemove->m_next;
		toRemove->m_next->m_prev = m_opaqueMeshes;
	}
	else if (toRemove->m_prev && !toRemove->m_next) { //TAIL

		toRemove->m_prev->m_next = nullptr;
	}
	else if (!toRemove->m_prev && !toRemove->m_next) { //HEAD, ONLY ELEM
		m_opaqueMeshes = nullptr;
	}

	m_numMeshes--;
}