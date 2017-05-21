#pragma once

#include <stdint.h>
#include <vector>
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/Vertex/Vertex.hpp"
#include "Engine/Renderer/Material/Material.hpp"
#include "Engine/Renderer/Framebuffer.hpp"
#include "Engine/Renderer/Vertex/VertexDefinition.hpp"
#include "ThirdParty/MikkT/mikktspace.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
struct DrawInstruction {
	unsigned int primitiveType;
	unsigned int startIndex;
	unsigned int count;
	bool useIndexBuffer;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MESH CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
class Mesh {
public:

	//ADD
	void AddVertex(const Vertex& vert)					{ m_verts.push_back(vert);					}
	void AddVertex(const TexturedVertex& vert)			{ m_texturedVerts.push_back(vert);			}
	void AddVertex(const TexturedVertex_TBN& vert)		{ m_texturedVertsTBN.push_back(vert);		}
	void AddVertex(const TexturedVertex_N& vert)		{ m_texturedVertsN.push_back(vert);			}
	void AddVertex(const TexturedVertex_TBNBN& vert)	{ m_texturedVertsTBNBN.push_back(vert);		}
	void AddInd(uint16_t ind)							{ m_inds.push_back(ind);					}

	//UPDATE
	void UpdateMesh();
	void UpdateMeshForNormals();
	void UpdateMesh(const std::vector<Vertex2D_PCT>& verts);
	void UpdateMesh(const std::vector<Vertex2D_PCT>& verts, const std::vector<uint16_t>& inds);
	void UpdateMesh(const std::vector<TexturedVertex_TBN>& verts, const std::vector<uint16_t>& inds);
	void UpdateMesh(const std::vector<TexturedVertex_TBNBN>& verts, const std::vector<uint16_t>& inds);
	void UpdateMesh(const std::vector<TexturedVertex>& verts, const std::vector<uint16_t>& inds);
	void UpdateMesh(const std::vector<Vertex>& verts, const std::vector<uint16_t>& inds);
	void UpdateMesh(const std::vector<TexturedVertex_TBN>& verts);
	void UpdateMesh(const std::vector<TexturedVertex>& verts);
	void UpdateMesh(const std::vector<uint16_t>& inds);
	void UpdateMeshInds(const std::vector<uint16_t>& inds) { m_inds = inds; }

	//GETTERS
	int					GetVboID() const { return m_vboID; }
	int					GetIboID() const { return m_iboID; }
	VertexDefinition*	GetVertexDefinition()		{ return m_vertexDef;	}

	//DRAWING
	void BindMesh() const;
	void UnbindMesh() const;
	void DrawMesh(bool drawLines = false) const;

	//MIKKT CALCULATIONS
	static int GetNumFaces(const SMikkTSpaceContext* mikktContext);
	static int GetNumVerticesOfFace(const SMikkTSpaceContext* mikktContext, const int iFace);
	static void GetPosition(const SMikkTSpaceContext * pContext, float fvPosOut[], const int iFace, const int iVert);
	static void GetNormal(const SMikkTSpaceContext * pContext, float fvNormOut[], const int iFace, const int iVert);
	static void GetTexCoord(const SMikkTSpaceContext * pContext, float fvTexcOut[], const int iFace, const int iVert);

	static void SetTangentSpaceBasic(const SMikkTSpaceContext * pContext, const float fvTangent[], 
		const float fSign, const int iFace, const int iVert);
	static void SetTangentSpace(const SMikkTSpaceContext * pContext, const float fvTangent[], 
		const float fvBiTangent[], const float fMagS, const float fMagT, const tbool bIsOrientationPreserving, 
		const int iFace, const int iVert);

	//OPERATORS
	bool operator==(const Mesh& other) const { return (m_meshID == other.m_meshID); }
	bool operator!=(const Mesh& other) const { return (m_meshID != other.m_meshID); }

public:


	VertexDefinition*					m_vertexDef;
	std::vector<DrawInstruction>		m_drawInstructions;

	std::vector<TexturedVertex_TBNBN>	m_texturedVertsTBNBN;
	std::vector<TexturedVertex_N>		m_texturedVertsN;
	std::vector<TexturedVertex_TBN>		m_texturedVertsTBN;
	std::vector<TexturedVertex>			m_texturedVerts;
	std::vector<Vertex2D_PCT>			m_2dVertsPCT;
	std::vector<Vertex>					m_verts;
	std::vector<uint16_t>				m_inds;
	int									m_vboID			= -1;
	int									m_iboID			= -1;
	bool								m_usesInds		= true;
	size_t								m_meshID		= 0;

	static size_t						s_meshIDs;

private:
	//STRUCTORS
	Mesh(VertexDefinition* vertexDef);
	~Mesh();

	template <typename T>
	void UpdateVBO(const std::vector<T>& verts, int numVerts);

	template <typename T>
	void UpdateVBO(const T* verts, int numVerts);

	void UpdateIBOData(const std::vector<uint16_t>& data);

	friend class BeirusMeshCollection;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TEMPLATES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
void Mesh::UpdateVBO(const std::vector<T>& verts, int numVerts) {

	size_t vertexArrayNumBytes = sizeof(T) * numVerts;
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vboID));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vertexArrayNumBytes, verts.data(), GL_STATIC_DRAW));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
void Mesh::UpdateVBO(const T* verts, int numVerts) {

	size_t vertexArrayNumBytes = sizeof(T) * numVerts;
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vboID));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vertexArrayNumBytes, verts, GL_STATIC_DRAW));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}