#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/Vertex/VertexDefinition.hpp"
#include "Engine/Renderer/General/RenderCommon.hpp"

const size_t STARTING_ARRAY_SIZE = 10;
const uint8_t GROW_SIZE = 3;

class Mesh;


class BeirusMeshCollection {
public:
	//ADD GET
	MeshID Allocate(VertexDefinition* vertexDef);
	Mesh* GetMesh(MeshID idx);
	static BeirusMeshCollection* Get();

private:
	//STRUCTORS
	BeirusMeshCollection();
	~BeirusMeshCollection();

	void InitializePrimitiveMeshes();

	//PRIVATE UTILS
	void Grow();
	static void Deinit();

	Mesh*	m_meshes	= nullptr;
	size_t	m_capacity	= 0;
	size_t	m_size		= 0;

	static BeirusMeshCollection* s_bMeshCollection;
};