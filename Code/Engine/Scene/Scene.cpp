#include "Engine/Scene/Scene.hpp"
#include "Engine/Renderer/Mesh/Model.hpp"
#include "Engine/Renderer/Material/Material.hpp"
#include "Engine/Renderer/Mesh/MeshBuilder.hpp"
#include "Engine/Tools/FBX/SceneImport.hpp"
#include "Engine/Renderer/Effects/DepthAndNormalsPass.hpp"
#include "Engine/Renderer/General/RenderCommon.hpp"
#include "Engine/Renderer/Structures/BeirusMeshCollection.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOADING AND SAVING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
Model* Scene::AddModelFromFile(const char* filename) {

	Model* newMesh = new Model(Vector3::ZERO);
	String matName;
	newMesh->m_meshID = MeshBuilder::ReadMeshFromFile(filename, &matName);
	matName = matName.substr(0, matName.size() - 1);
	newMesh->m_materialName = matName;
	newMesh->SetMaterial(matName);

	Vector4 r1 = Vector4(1.f, 0.f, 0.f, 0.f);
	Vector4 r2 = Vector4(0.f, 0.f, 1.f, 0.f);
	Vector4 r3 = Vector4(0.f, 1.f, 0.f, 0.f);
	Vector4 r4 = Vector4(0.f, 0.f, 0.f, 1.f);

	newMesh->m_model = Matrix4(r1, r2, r3, r4);

	AddStaticMesh(newMesh);
	return newMesh;
}


//---------------------------------------------------------------------------------------------------------------------------
void Scene::AddModelsFromDirectory(const char* dir) {

	std::vector<String> filenames = FileUtils::GetAllFilenamesInDirectory(dir);

	for (size_t i = 0; i < filenames.size(); i++) {

		Model* newMesh = new Model(Vector3::ZERO);
		String matName;
		newMesh->m_meshID = MeshBuilder::ReadMeshFromFile(filenames[i], &matName);
		matName = matName.substr(0, matName.size() - 1);
		newMesh->m_materialName = matName;
		newMesh->SetMaterial(matName);

		Vector4 r1 = Vector4(1.f, 0.f, 0.f, 0.f);
		Vector4 r2 = Vector4(0.f, 0.f, 1.f, 0.f);
		Vector4 r3 = Vector4(0.f, 1.f, 0.f, 0.f);
		Vector4 r4 = Vector4(0.f, 0.f, 0.f, 1.f);

		newMesh->m_model = Matrix4(r1, r2, r3, r4);

		AddStaticMesh(newMesh);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC Scene* Scene::LoadSceneFromDirectory(const char* sceneDir) {

	std::vector<String> filenames = FileUtils::GetAllFilenamesInDirectory(sceneDir);

	if (filenames.empty()) {
		return nullptr;
	}

	Scene* newScene = new Scene();

	for (size_t i = 0; i < filenames.size(); i++) {

		Model* newMesh = new Model(Vector3::ZERO);
		String matName;
		newMesh->m_meshID = MeshBuilder::ReadMeshFromFile(filenames[i], &matName);
		matName = matName.substr(0, matName.size() - 1);
		newMesh->m_materialName = matName;
		newMesh->SetMaterial(matName);
		newMesh->m_renderState = newMesh->m_material->m_renderState;

		Vector4 r1 = Vector4(1.f, 0.f, 0.f, 0.f);
		Vector4 r2 = Vector4(0.f, 0.f, 1.f, 0.f);
		Vector4 r3 = Vector4(0.f, 1.f, 0.f, 0.f);
		Vector4 r4 = Vector4(0.f, 0.f, 0.f, 1.f);

		newMesh->m_model = Matrix4(r1, r2, r3, r4);

		newScene->AddStaticMesh(newMesh);
	}

	return newScene;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC Scene* Scene::LoadSceneFromFBXFile(const char* filename) {

	SceneImport* import = new SceneImport(filename, MESH_LOAD_ENGINE_SCALE, ANIMATION_FPS);

	if (!import->m_importSuccessful) {
		Console::PrintOutput("ERROR: Failed to load file.", CONSOLE_WARNING);
		return nullptr;
	}
	else {

		Scene* newScene = new Scene();

		int meshSize	= import->m_meshBuilder.GetNumMeshes(); // import->m_meshes.size();
		String str		= StringUtils::Stringf("Loaded %s. Had %i meshes.", filename, meshSize);
		Console::PrintOutput(str, CONSOLE_VERIFY);

		for (uint i = 0; i < import->m_meshBuilder.GetNumMeshes(); i++) {

			Model* mesh		= new Model(Vector3::ZERO);
			mesh->m_meshID			= import->m_meshBuilder.GetMeshAtIndex(i);
			mesh->m_materialName	= import->m_meshBuilder.GetMaterialNameAtIndex(i);
			mesh->SetMaterial(mesh->m_materialName);

			Mesh* newMesh = BeirusMeshCollection::Get()->GetMesh(mesh->m_meshID);
			newMesh->UpdateMesh();
			newScene->AddStaticMesh(mesh);
		}

		return newScene;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void Scene::SaveSceneToFile(const char* filepath) {

	Model* curr = m_opaqueMeshes;

	size_t idx = 0;
	while (curr) {
		String filenameFormatted = filepath + std::to_string(idx);
		MeshBuilder::WriteMeshToFile(curr->m_meshID, curr->m_materialName, "Data/Mesh/Sponza/", filenameFormatted + ".mesh");
		curr = curr->m_next;
		idx++;
	}

	curr = m_translucentMeshesHead;

	idx = 0;
	while (curr) {
		String filenameFormatted = filepath + std::to_string(idx);
		MeshBuilder::WriteMeshToFile(curr->m_meshID, curr->m_materialName, "Data/Mesh/Sponza/", filenameFormatted + ".mesh");
		curr = curr->m_next;
		idx++;
	}

	Console::PrintOutput("Meshes saved successfully.", CONSOLE_VERIFY);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void Scene::RenderForShadows(Material* shadowMat) const {

	Model* curr = m_opaqueMeshes;

	static size_t idx = 0;

	while (curr != nullptr) {
		if (curr->IsActive()) {
			curr->Render(shadowMat);
		}
		curr = curr->m_next;
		idx++;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
void Scene::RenderWithMaterial(Material* mat) const {

	Model* curr = m_opaqueMeshes;

	while (curr != nullptr) {
		if (curr->IsActive()) {

			DepthAndNormalsPass::Get()->ConnectInNormalsTexture(curr->m_material);
			curr->Render(mat);
		}
		curr = curr->m_next;
	}
}



bool TransparentModelComparator(Model* i, Model* j) { 
	return (i->CalcDistFromCamera() > j->CalcDistFromCamera()); 
}

//---------------------------------------------------------------------------------------------------------------------------
void Scene::Render(bool isRenderingTransparent, MeshRenderer* customMeshRenderer) const {

	if (!isRenderingTransparent) {

		Model* curr = m_opaqueMeshes;
		
		while (curr != nullptr) {
			if (curr->IsActive()) {
				curr->Render(customMeshRenderer);
			}
			curr = curr->m_next;
		}
	}
	else if (isRenderingTransparent) {

		std::vector<Model*> tModels;

		Model* curr = m_translucentMeshesHead;
		int idx = 0;
		while (curr != nullptr) {
			if (curr->IsActive()) {
				tModels.push_back(curr);
			}
			curr = curr->m_next;
		}

		if (m_debugShouldSort) {
			std::sort(tModels.begin(), tModels.end(), TransparentModelComparator);
		}

		for (size_t i = 0; i < tModels.size(); i++) {
			tModels[i]->Render(customMeshRenderer);
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void Scene::RenderSkybox(MeshRenderer* customMeshRenderer) const {

	m_skybox->Render(customMeshRenderer);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADDING REMOVING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void Scene::AddStaticMesh(Model* newMesh) {
	
	if (newMesh->m_renderState.m_blendMode == BLEND_MODE_OPAQUE || 
		newMesh->m_renderState.m_blendMode == BLEND_MODE_DEFAULT) {
		AddStaticMeshToOpaquePass(newMesh);
	}
	else if (newMesh->m_renderState.m_blendMode == BLEND_MODE_TRANSPARENT_DEFAULT) {
		AddStaticMeshToTranslucentPass(newMesh);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void Scene::RemoveStaticMesh(Model* mesh) {

	if (mesh->m_renderState.m_blendMode == BLEND_MODE_OPAQUE ||
		mesh->m_renderState.m_blendMode == BLEND_MODE_DEFAULT) {
		RemoveStaticMeshFromOpaquePass(mesh);
	}
	else if (mesh->m_renderState.m_blendMode == BLEND_MODE_TRANSPARENT_DEFAULT) {
		RemoveStaticMeshFromTranslucentPass(mesh);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE ADD REMOVE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void Scene::AddStaticMeshToOpaquePass(Model* newMesh) {

	if (!m_opaqueMeshes) {
		m_opaqueMeshes = newMesh;
		newMesh->m_next = nullptr;
		newMesh->m_prev = nullptr;
	}
	else {
		Model* head = m_opaqueMeshes;
		m_opaqueMeshes = newMesh;
		m_opaqueMeshes->m_next = head;
		m_opaqueMeshes->m_prev = nullptr;
		m_opaqueMeshes->m_next->m_prev = m_opaqueMeshes;
	}
	m_numMeshes++;
}


//---------------------------------------------------------------------------------------------------------------------------
void Scene::RemoveStaticMeshFromOpaquePass(Model* mesh) {

	Model* toRemove = mesh;

	//WARNING: BROKEN. IMPLEMENT THE TRANSLUCENT VERSION OF THIS - IT IS CORRECT

	if (toRemove->m_prev && toRemove->m_next) {	//MIDDLE

		Model* prev = toRemove->m_prev;
		Model* next = toRemove->m_next;

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


//---------------------------------------------------------------------------------------------------------------------------
void Scene::AddStaticMeshToTranslucentPass(Model* newMesh) {

	if (!m_translucentMeshesHead) {
		m_translucentMeshesHead = newMesh;
		m_translucentMeshesTail = newMesh;
		newMesh->m_next = nullptr;
		newMesh->m_prev = nullptr;
	}
	else {
		Model* head = m_translucentMeshesHead;
		m_translucentMeshesHead = newMesh;
		m_translucentMeshesHead->m_next = head;
		m_translucentMeshesHead->m_prev = nullptr;
		m_translucentMeshesHead->m_next->m_prev = m_translucentMeshesHead;
	}
	m_numMeshes++;
}


//---------------------------------------------------------------------------------------------------------------------------
void Scene::RemoveStaticMeshFromTranslucentPass(Model* mesh) {

	Model* toRemove = mesh;

	if (m_translucentMeshesHead == nullptr || toRemove == nullptr)
		return;

	if (m_translucentMeshesHead == toRemove) {
		m_translucentMeshesHead = toRemove->m_next;
	}

	if (toRemove->m_next != nullptr) {
		toRemove->m_next->m_prev = toRemove->m_prev;
	}

	if (toRemove->m_prev != nullptr) {
		toRemove->m_prev->m_next = toRemove->m_next;
	}

	m_numMeshes--;
}