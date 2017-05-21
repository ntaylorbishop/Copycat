#include "Engine/Renderer/Mesh/MeshBuilder.hpp"
#include "Engine/General/Core/TheMemoryManager.hpp"
#include "Engine/Renderer/Structures/BeirusMeshCollection.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MeshBuilder::MeshBuilder() 
	: m_currMesh(nullptr)
	, m_isUsingMikkt(true)
{ 
	m_mikktContext								= new SMikkTSpaceContext();
	m_mikktInterface							= new SMikkTSpaceInterface();
	m_mikktInterface->m_getPosition				= &Mesh::GetPosition;
	m_mikktInterface->m_getNormal				= &Mesh::GetNormal;
	m_mikktInterface->m_getNumFaces				= &Mesh::GetNumFaces;
	m_mikktInterface->m_getNumVerticesOfFace	= &Mesh::GetNumVerticesOfFace;
	m_mikktInterface->m_getTexCoord				= &Mesh::GetTexCoord;
	m_mikktInterface->m_setTSpace				= &Mesh::SetTangentSpace;
	m_mikktContext->m_pInterface				= m_mikktInterface;
}


//---------------------------------------------------------------------------------------------------------------------------
MeshBuilder::~MeshBuilder() {
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BUILDING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshBuilder::AddMesh() { 
	ASSERT_OR_DIE(nullptr == m_currMesh, "ERROR: CANNOT ADD MESH BEFORE CLOSING LAST MESH");

	MeshID meshId = BeirusMeshCollection::Get()->Allocate(&VertexDefinition::TEXTUREDVERTEX_TBNBN);
	m_currMesh = BeirusMeshCollection::Get()->GetMesh(meshId);
}


//---------------------------------------------------------------------------------------------------------------------------
void MeshBuilder::AddMaterialName(const String& name) {
	m_meshMaterialNames.push_back(name);
}


//---------------------------------------------------------------------------------------------------------------------------
void MeshBuilder::AddVertex(TexturedVertex_TBNBN vert) {
	m_currMesh->AddVertex(vert);
}


//---------------------------------------------------------------------------------------------------------------------------
void MeshBuilder::AddInd(uint16_t ind) {
	m_currMesh->AddInd(ind);
}


//---------------------------------------------------------------------------------------------------------------------------
void MeshBuilder::CloseMesh() {

	if (m_isUsingMikkt) {
		m_mikktContext->m_pUserData = m_currMesh;
		genTangSpaceDefault(m_mikktContext);
	}

	std::reverse(m_currMesh->m_inds.begin(), m_currMesh->m_inds.end());
	m_currMesh->UpdateMesh();
	m_meshes.push_back(m_currMeshID);
	m_currMesh = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//READ WRITE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
STATIC void MeshBuilder::WriteMeshToFile(MeshID meshId, const String& matName, const String& path, const String& filename) {

	Mesh* mesh = BeirusMeshCollection::Get()->GetMesh(meshId);

	FileBinaryWriter fileWriter;
	fileWriter.Open(path + filename);

	/*
	VERSION 1
	Num Verts
	Positions
	Colors
	TexCoords
	Tangents
	Bitangents
	Bone Weights
	Bone Inds
	*/

	//MAT NAME
	fileWriter.WriteString(matName);

	//NUM VERTS
	fileWriter.WriteUnsignedInt((uint)mesh->m_texturedVertsTBNBN.size());
	//POSITIONS
	for (unsigned int i = 0; i < mesh->m_texturedVertsTBNBN.size(); i++) {
		TexturedVertex_TBNBN* currVert = &mesh->m_texturedVertsTBNBN[i];
		float x = currVert->m_position.x;
		float y = currVert->m_position.y;
		float z = currVert->m_position.z;

		fileWriter.WriteFloat(x);
		fileWriter.WriteFloat(y);
		fileWriter.WriteFloat(z);
	}
	//COLORS
	for (unsigned int i = 0; i < mesh->m_texturedVertsTBNBN.size(); i++) {
		TexturedVertex_TBNBN* currVert = &mesh->m_texturedVertsTBNBN[i];
		float r = currVert->m_color.r;
		float g = currVert->m_color.g;
		float b = currVert->m_color.b;
		float a = currVert->m_color.a;

		fileWriter.WriteFloat(r);
		fileWriter.WriteFloat(g);
		fileWriter.WriteFloat(b);
		fileWriter.WriteFloat(a);
	}
	//TEXCOORDS
	for (unsigned int i = 0; i < mesh->m_texturedVertsTBNBN.size(); i++) {
		TexturedVertex_TBNBN* currVert = &mesh->m_texturedVertsTBNBN[i];
		float x = currVert->m_texCoords.x;
		float y = currVert->m_texCoords.y;

		fileWriter.WriteFloat(x);
		fileWriter.WriteFloat(y);
	}
	//TANGENTS
	for (unsigned int i = 0; i < mesh->m_texturedVertsTBNBN.size(); i++) {
		TexturedVertex_TBNBN* currVert = &mesh->m_texturedVertsTBNBN[i];
		float x = currVert->m_tangent.x;
		float y = currVert->m_tangent.y;
		float z = currVert->m_tangent.z;

		fileWriter.WriteFloat(x);
		fileWriter.WriteFloat(y);
		fileWriter.WriteFloat(z);
	}
	//BITANGENTS
	for (unsigned int i = 0; i < mesh->m_texturedVertsTBNBN.size(); i++) {
		TexturedVertex_TBNBN* currVert = &mesh->m_texturedVertsTBNBN[i];
		float x = currVert->m_bitangent.x;
		float y = currVert->m_bitangent.y;
		float z = currVert->m_bitangent.z;

		fileWriter.WriteFloat(x);
		fileWriter.WriteFloat(y);
		fileWriter.WriteFloat(z);
	}
	//NORMALS
	for (unsigned int i = 0; i < mesh->m_texturedVertsTBNBN.size(); i++) {
		TexturedVertex_TBNBN* currVert = &mesh->m_texturedVertsTBNBN[i];
		float x = currVert->m_normal.x;
		float y = currVert->m_normal.y;
		float z = currVert->m_normal.z;

		fileWriter.WriteFloat(x);
		fileWriter.WriteFloat(y);
		fileWriter.WriteFloat(z);
	}
	//BONE WEIGHTS
	for (unsigned int i = 0; i < mesh->m_texturedVertsTBNBN.size(); i++) {
		TexturedVertex_TBNBN* currVert = &mesh->m_texturedVertsTBNBN[i];
		float x = currVert->m_boneWeights.x;
		float y = currVert->m_boneWeights.y;
		float z = currVert->m_boneWeights.z;
		float w = currVert->m_boneWeights.w;

		fileWriter.WriteFloat(x);
		fileWriter.WriteFloat(y);
		fileWriter.WriteFloat(z);
		fileWriter.WriteFloat(w);
	}
	//BONE INDS
	for (unsigned int i = 0; i < mesh->m_texturedVertsTBNBN.size(); i++) {
		TexturedVertex_TBNBN* currVert = &mesh->m_texturedVertsTBNBN[i];
		int x = currVert->m_boneInds.x;
		int y = currVert->m_boneInds.y;
		int z = currVert->m_boneInds.z;
		int w = currVert->m_boneInds.w;

		fileWriter.WriteInt(x);
		fileWriter.WriteInt(y);
		fileWriter.WriteInt(z);
		fileWriter.WriteInt(w);
	}

	fileWriter.Close();
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC MeshID MeshBuilder::ReadMeshFromFile(const String& path, const String& filename, String* outMatName) {

	return ReadMeshFromFile(path + filename, outMatName);
}



//---------------------------------------------------------------------------------------------------------------------------
STATIC MeshID MeshBuilder::ReadMeshFromFile(const String& pathAndFilename, String* outMatName) {

	MeshID meshId = BeirusMeshCollection::Get()->Allocate(&VertexDefinition::TEXTUREDVERTEX_TBNBN);
	Mesh* mesh = BeirusMeshCollection::Get()->GetMesh(meshId);

	FileBinaryReader fileReader;
	fileReader.Open(pathAndFilename);

	fileReader.ReadString(outMatName);

	uint numVerts = 0;
	fileReader.ReadUnsignedInt(&numVerts);

	mesh->m_texturedVertsTBNBN.reserve(numVerts + 1);

	//Add blank vertices
	for (uint i = 0; i < numVerts; i++) {
		mesh->AddVertex(TexturedVertex_TBNBN());
	}
	//Add positions
	for (uint i = 0; i < numVerts; i++) {
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_position.x);
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_position.y);
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_position.z);
	}
	//Add colors
	for (uint i = 0; i < numVerts; i++) {
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_color.r);
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_color.g);
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_color.b);
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_color.a);
	}
	//Add texcoords
	for (uint i = 0; i < numVerts; i++) {
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_texCoords.x);
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_texCoords.y);
	}
	//Add tangents
	for (uint i = 0; i < numVerts; i++) {
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_tangent.x);
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_tangent.y);
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_tangent.z);
	}
	//Add bitangents
	for (uint i = 0; i < numVerts; i++) {
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_bitangent.x);
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_bitangent.y);
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_bitangent.z);
	}
	//Add normals
	for (uint i = 0; i < numVerts; i++) {
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_normal.x);
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_normal.y);
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_normal.z);
	}
	//Add bone weights
	for (uint i = 0; i < numVerts; i++) {
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_boneWeights.x);
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_boneWeights.y);
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_boneWeights.z);
		fileReader.ReadFloat(&mesh->m_texturedVertsTBNBN[i].m_boneWeights.w);
	}
	//Add bone inds
	for (uint i = 0; i < numVerts; i++) {
		fileReader.ReadInt(&mesh->m_texturedVertsTBNBN[i].m_boneInds.x);
		fileReader.ReadInt(&mesh->m_texturedVertsTBNBN[i].m_boneInds.y);
		fileReader.ReadInt(&mesh->m_texturedVertsTBNBN[i].m_boneInds.z);
		fileReader.ReadInt(&mesh->m_texturedVertsTBNBN[i].m_boneInds.w);
	}

	mesh->m_inds.reserve(numVerts + 1);
	for (unsigned int i = 0; i < numVerts; i++) {
		mesh->AddInd((uint16_t)i);
	}
	mesh->UpdateMesh();

	fileReader.Close();
	return meshId;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GETTERS SETTERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Mesh* MeshBuilder::GetMasterMesh() {
	ASSERT_OR_DIE(nullptr != m_masterMesh, "ERROR: CANNOT GET MASTER MESH UNTIL MESHES HAVE BEEN REDUCED");
	return m_masterMesh;
}