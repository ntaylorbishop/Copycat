#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

class D3D11Model;
class D3D11Material;
class D3D11MeshRenderer;

const float ANIMATION_FPS = 30.f;
const float MESH_LOAD_ENGINE_SCALE = 0.4f;

class D3D11Scene {
public:
	//LOADING SAVING
	void				AddModelsFromDirectory(const char* dir);
	static D3D11Scene*	LoadSceneFromDirectory(const char* sceneDir);
	static D3D11Scene*	LoadSceneFromFBXFile(const char* filename);
	void				SaveSceneToFile(const char* filepath);

	//STRUCTORS
	D3D11Scene()	{ }
	~D3D11Scene()	{ }

	//RENDER
	void RenderForShadows(D3D11Material* shadowMat) const;
	void RenderWithMaterial(D3D11Material* mat) const;
	void Render(bool isRenderingTransparent = false, D3D11MeshRenderer* customMeshRenderer = nullptr) const;
	void RenderSkybox(D3D11MeshRenderer* customMeshRenderer = nullptr) const;

	//ADDING REMOVING
	void AddModel(D3D11Model* newMesh);
	void RemoveModel(D3D11Model* mesh);
	void SetSkybox(D3D11Model* skybox) { m_skybox = skybox; }

	void EnableDebugSorting() { m_debugShouldSort = true; }
	void DisableDebugSorting() { m_debugShouldSort = false; }

private:
	//PRIVATE ADD REMOVE
	void AddStaticMeshToOpaquePass(D3D11Model* newMesh);
	void RemoveStaticMeshFromOpaquePass(D3D11Model* mesh);


	D3D11Model*			m_opaqueMeshes			= nullptr;
	size_t				m_numMeshes				= 0;
	D3D11Model*			m_skybox				= nullptr;

	bool				m_debugShouldSort		= false;
};