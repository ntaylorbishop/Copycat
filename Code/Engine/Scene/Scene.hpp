#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

class Model;
class Material;
class MeshRenderer;

const float ANIMATION_FPS = 30.f;
const float MESH_LOAD_ENGINE_SCALE = 0.4f;

class Scene {
public:
	//LOADING SAVING
	Model*			AddModelFromFile(const char* filename);
	void			AddModelsFromDirectory(const char* dir);
	static Scene*	LoadSceneFromDirectory(const char* sceneDir);
	static Scene*	LoadSceneFromFBXFile(const char* filename);
	void			SaveSceneToFile(const char* filepath);

	//STRUCTORS
	Scene()		{ }
	~Scene()	{ }
	
	//RENDER
	void RenderForShadows(Material* shadowMat) const;
	void RenderWithMaterial(Material* mat) const;
	void Render(bool isRenderingTransparent = false, MeshRenderer* customMeshRenderer = nullptr) const;
	void RenderSkybox(MeshRenderer* customMeshRenderer = nullptr) const;

	//ADDING REMOVING
	void AddStaticMesh(Model* newMesh);
	void RemoveStaticMesh(Model* mesh);
	void SetSkybox(Model* skybox) { m_skybox = skybox; }

	void EnableDebugSorting() { m_debugShouldSort = true; }
	void DisableDebugSorting() { m_debugShouldSort = false; }

private:
	//PRIVATE ADD REMOVE
	void AddStaticMeshToOpaquePass(Model* newMesh);
	void RemoveStaticMeshFromOpaquePass(Model* mesh);
	void AddStaticMeshToTranslucentPass(Model* newMesh);
	void RemoveStaticMeshFromTranslucentPass(Model* mesh);


	Model*			m_opaqueMeshes			= nullptr;
	Model*			m_translucentMeshesHead	= nullptr;
	mutable Model*	m_translucentMeshesTail	= nullptr;
	size_t			m_numMeshes				= 0;
	Model*			m_skybox				= nullptr;

	bool			m_debugShouldSort		= false;
};