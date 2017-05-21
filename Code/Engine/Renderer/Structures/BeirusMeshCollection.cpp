#include "Engine/Renderer/Structures/BeirusMeshCollection.hpp"
#include "Engine/Renderer/Mesh/Mesh.hpp"
#include "Engine/Renderer/Mesh/PrimitiveMesh.hpp"

STATIC BeirusMeshCollection* BeirusMeshCollection::s_bMeshCollection = nullptr;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADD GET
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
MeshID BeirusMeshCollection::Allocate(VertexDefinition* vertexDef) {
	
	if (m_size == m_capacity) {
		Grow();
	}

	Mesh* newMeshSpot = m_meshes + m_size;
	new(newMeshSpot) Mesh(vertexDef);

	MeshID mID = m_size;
	m_size++;
	return mID;
}


//---------------------------------------------------------------------------------------------------------------------------
Mesh* BeirusMeshCollection::GetMesh(MeshID idx) {

	#ifdef _DEBUG
	ASSERT_OR_DIE(idx < m_size, "ERROR: BEIRUSMESHCOLLECTION INDEX OUT OF BOUNDS.");
	#endif
	return &m_meshes[idx];
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC BeirusMeshCollection* BeirusMeshCollection::Get() {

	if (s_bMeshCollection == nullptr) {
		s_bMeshCollection = new BeirusMeshCollection();
		BeirusEngine::RegisterShutdownCallback(&BeirusMeshCollection::Deinit);
	}

	return s_bMeshCollection;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
BeirusMeshCollection::BeirusMeshCollection() 
	: m_capacity((size_t)STARTING_ARRAY_SIZE)
{
	m_meshes = (Mesh*)malloc(m_capacity * sizeof(Mesh));
	InitializePrimitiveMeshes();
}


//---------------------------------------------------------------------------------------------------------------------------
BeirusMeshCollection::~BeirusMeshCollection() {

	for (size_t meshIdx = 0; meshIdx < s_bMeshCollection->m_size; meshIdx++) {
		s_bMeshCollection->m_meshes[meshIdx].~Mesh();
	}
	free(s_bMeshCollection->m_meshes);
}


//---------------------------------------------------------------------------------------------------------------------------
void BeirusMeshCollection::InitializePrimitiveMeshes() {

	MeshID id1 = Allocate(&VertexDefinition::TEXTUREDVERTEX_TBNBN);
	MeshID id2 = Allocate(&VertexDefinition::TEXTUREDVERTEX_TBNBN);
	MeshID id3 = Allocate(&VertexDefinition::TEXTUREDVERTEX_TBNBN);
	MeshID id4 = Allocate(&VertexDefinition::TEXTUREDVERTEX_TBNBN);
	MeshID id5 = Allocate(&VertexDefinition::TEXTUREDVERTEX_TBNBN);

	Mesh* cubeMesh		= GetMesh(id1);
	Mesh* sphereMesh10	= GetMesh(id2);
	Mesh* sphereMesh100 = GetMesh(id3);
	Mesh* quadMesh		= GetMesh(id4);
	Mesh* skyboxMesh	= GetMesh(id5);

	PrimitiveMesh::ConstructCubeMesh(cubeMesh);
	PrimitiveMesh::ConstructSphereMesh(sphereMesh10, 10);
	PrimitiveMesh::ConstructSphereMesh(sphereMesh100, 100);
	PrimitiveMesh::ConstructQuadMesh(quadMesh);
	PrimitiveMesh::ConstructSkybox(skyboxMesh);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE UTILS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void BeirusMeshCollection::Grow() {

	size_t newCapacity = m_capacity * GROW_SIZE;

	Mesh* newArr = (Mesh*)malloc(newCapacity * sizeof(Mesh));
	memcpy_s(newArr, newCapacity * sizeof(Mesh), m_meshes, m_capacity * sizeof(Mesh));
	free(m_meshes);
	m_meshes	= newArr;
	m_capacity	= newCapacity;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void BeirusMeshCollection::Deinit() {

	delete s_bMeshCollection;
}