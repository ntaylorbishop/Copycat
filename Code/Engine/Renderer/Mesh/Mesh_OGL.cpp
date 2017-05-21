#include "Engine/Config/BuildConfig.hpp"

#ifdef GRAPHICS_API_OGL
#include "Engine/Renderer/Mesh/Mesh.hpp"


STATIC size_t Mesh::s_meshIDs = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
Mesh::Mesh(VertexDefinition* vertexDef)
	: m_vboID(-1)
	, m_iboID(-1)
	, m_vertexDef(vertexDef)
	, m_meshID(s_meshIDs++)
{
	GL_CHECK(glGenBuffers(1, (GLuint*)&m_vboID));
	GL_CHECK(glGenBuffers(1, (GLuint*)&m_iboID));
}


//---------------------------------------------------------------------------------------------------------------------------
Mesh::~Mesh() { 
	GL_CHECK(glDeleteBuffers(1, (GLuint*)&m_vboID));
	GL_CHECK(glDeleteBuffers(1, (GLuint*)&m_iboID));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GETTERS SETTERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
void Mesh::UpdateMesh() {
	UpdateVBO(m_texturedVertsTBNBN, (uint)m_texturedVertsTBNBN.size());
	UpdateIBOData(m_inds);
}


//---------------------------------------------------------------------------------------------------------------------------
void Mesh::UpdateMeshForNormals() {
	UpdateVBO(m_texturedVertsN, (uint)m_texturedVertsN.size());
	UpdateIBOData(m_inds);
}


//---------------------------------------------------------------------------------------------------------------------------
void Mesh::UpdateMesh(const std::vector<Vertex2D_PCT>& verts) {
	m_2dVertsPCT = verts;
	UpdateVBO(verts, (uint)verts.size());
}


//---------------------------------------------------------------------------------------------------------------------------
void Mesh::UpdateMesh(const std::vector<Vertex2D_PCT>& verts, const std::vector<uint16_t>& inds) {
	m_2dVertsPCT = verts;
	m_inds = inds;
	UpdateVBO(verts, (uint)verts.size());
	UpdateIBOData(m_inds);
}


//---------------------------------------------------------------------------------------------------------------------------
void Mesh::UpdateMesh(const std::vector<TexturedVertex_TBN>& verts, const std::vector<uint16_t>& inds) {
	m_texturedVertsTBN = verts;
	m_inds = inds;
	UpdateVBO(verts, (uint)verts.size());
	UpdateIBOData(m_inds);
}


//---------------------------------------------------------------------------------------------------------------------------
void Mesh::UpdateMesh(const std::vector<TexturedVertex_TBNBN>& verts, const std::vector<uint16_t>& inds) {
	m_texturedVertsTBNBN = verts;
	m_inds = inds;
	UpdateVBO(verts, (uint)verts.size());
	UpdateIBOData(m_inds);
}


//---------------------------------------------------------------------------------------------------------------------------
void Mesh::UpdateMesh(const std::vector<TexturedVertex>& verts, const std::vector<uint16_t>& inds) {
	m_texturedVerts = verts;
	m_inds = inds;
	UpdateVBO(verts, (uint)verts.size());
	UpdateIBOData(m_inds);
}


//---------------------------------------------------------------------------------------------------------------------------
void Mesh::UpdateMesh(const std::vector<Vertex>& verts, const std::vector<uint16_t>& inds) {
	m_verts = verts;
	m_inds = inds;
	UpdateVBO(verts, (uint)verts.size());
}


//---------------------------------------------------------------------------------------------------------------------------
void Mesh::UpdateMesh(const std::vector<TexturedVertex>& verts) {
	m_texturedVerts = verts;
	UpdateVBO(verts, (uint)verts.size());
}


//---------------------------------------------------------------------------------------------------------------------------
void Mesh::UpdateMesh(const std::vector<uint16_t>& inds) {
	m_inds = inds;
	UpdateIBOData(m_inds);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DRAWING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
void Mesh::BindMesh() const {
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vboID));
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID));
}


//---------------------------------------------------------------------------------------------------------------------------
void Mesh::UnbindMesh() const {
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


//---------------------------------------------------------------------------------------------------------------------------
void Mesh::DrawMesh(bool drawLines) const {

	if (drawLines) {
		GL_CHECK(glDrawElements(GL_LINES, (GLsizei)m_inds.size(), GL_UNSIGNED_SHORT, (GLvoid*)0));
	}
	else {
		GL_CHECK(glDrawElements(GL_TRIANGLES,	(GLsizei)m_inds.size(), GL_UNSIGNED_SHORT, (GLvoid*)0));
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MIKKT CALCULATIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
STATIC int Mesh::GetNumFaces(const SMikkTSpaceContext* mikktContext) {

	Mesh* meshContext = (Mesh*)mikktContext->m_pUserData;

	return (int)(meshContext->m_texturedVertsTBNBN.size() / 3);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC int Mesh::GetNumVerticesOfFace(const SMikkTSpaceContext* mikktContext, const int iFace) {

	UNREFERENCED_PARAMETER(mikktContext);
	UNREFERENCED_PARAMETER(iFace);

	return 3;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Mesh::GetPosition(const SMikkTSpaceContext* pContext, float fvPosOut[], const int iFace, const int iVert) {

	Mesh* meshContext = (Mesh*)pContext->m_pUserData;

	int vertIndex = iFace * 3 + iVert;

	fvPosOut[0] = meshContext->m_texturedVertsTBNBN[vertIndex].m_position.x;
	fvPosOut[1] = meshContext->m_texturedVertsTBNBN[vertIndex].m_position.y;
	fvPosOut[2] = meshContext->m_texturedVertsTBNBN[vertIndex].m_position.z;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Mesh::GetNormal(const SMikkTSpaceContext* pContext, float fvNormOut[], const int iFace, const int iVert) {

	Mesh* meshContext = (Mesh*)pContext->m_pUserData;

	int vertIndex = iFace * 3 + iVert;

	fvNormOut[0] = meshContext->m_texturedVertsTBNBN[vertIndex].m_normal.x;
	fvNormOut[1] = meshContext->m_texturedVertsTBNBN[vertIndex].m_normal.y;
	fvNormOut[2] = meshContext->m_texturedVertsTBNBN[vertIndex].m_normal.z;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Mesh::GetTexCoord(const SMikkTSpaceContext* pContext, float fvTexcOut[], const int iFace, const int iVert) {

	Mesh* meshContext = (Mesh*)pContext->m_pUserData;

	int vertIndex = iFace * 3 + iVert;

	fvTexcOut[0] = meshContext->m_texturedVertsTBNBN[vertIndex].m_texCoords.x;
	fvTexcOut[1] = meshContext->m_texturedVertsTBNBN[vertIndex].m_texCoords.y;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Mesh::SetTangentSpaceBasic(const SMikkTSpaceContext * pContext, const float fvTangent[], const float fSign,
	const int iFace, const int iVert) {

	UNREFERENCED_PARAMETER(fSign);

	Mesh* meshContext = (Mesh*)pContext->m_pUserData;

	int vertIndex = iFace * 3 + iVert;

	Vector3 tan = Vector3(fvTangent[0], fvTangent[1], fvTangent[2]);
	Vector3 norm = meshContext->m_texturedVertsTBNBN[vertIndex].m_normal;

	tan.Normalize();
	norm.Normalize();

	meshContext->m_texturedVertsTBNBN[vertIndex].m_normal = CrossProduct(norm, tan);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void Mesh::SetTangentSpace(const SMikkTSpaceContext * pContext, const float fvTangent[], const float fvBiTangent[],
	const float fMagS, const float fMagT,
	const tbool bIsOrientationPreserving, const int iFace, const int iVert) {

	UNREFERENCED_PARAMETER(fMagS);
	UNREFERENCED_PARAMETER(fMagT);
	UNREFERENCED_PARAMETER(fvBiTangent);

	Mesh* meshContext = (Mesh*)pContext->m_pUserData;

	int vertIndex = iFace * 3 + iVert;

	meshContext->m_texturedVertsTBNBN[vertIndex].m_tangent.x = fvTangent[0];
	meshContext->m_texturedVertsTBNBN[vertIndex].m_tangent.y = fvTangent[1];
	meshContext->m_texturedVertsTBNBN[vertIndex].m_tangent.z = fvTangent[2];

	Vector3 tan = meshContext->m_texturedVertsTBNBN[vertIndex].m_tangent;

	float fSign = 1.f;
	if (bIsOrientationPreserving) {
		fSign = -1.f;
	}

	Vector3 normal = meshContext->m_texturedVertsTBNBN[vertIndex].m_normal;
	meshContext->m_texturedVertsTBNBN[vertIndex].m_bitangent = fSign * CrossProduct(normal, tan);
}


//---------------------------------------------------------------------------------------------------------------------------
void Mesh::UpdateIBOData(const std::vector<uint16_t>& data) {

	size_t vertexArrayNumBytes = sizeof(uint16_t) * data.size();

	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_iboID));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vertexArrayNumBytes, data.data(), GL_STATIC_DRAW));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

#endif