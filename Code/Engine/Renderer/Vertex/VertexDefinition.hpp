#pragma once

#include <vector>

#include "Engine/Renderer/Vertex/Vertex.hpp"
#include "Engine/Renderer/OpenGL/OpenGLEnums.hpp"
#include "Engine/General/Core/EngineCommon.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ENUMS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum VertexType {
	VTYPE_VERTEX = 0,
	VTYPE_TEXTUREDVERTEX,
	VTYPE_TEXTUREDVERTEX_TBN,
	VTYPE_TEXTUREDVERTEX_N,
	VTYPE_TEXTUREDVERTEX_TBNBN,
	VTYPE_VERTEX2D_PCT,
	VTYPE_NUM_VTYPES
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//VERTEX ATTRIBUTE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct VertexAttribute {
	const char* name;
	unsigned int count;
	eRenderEnum type;
	eRenderEnum normalize;
	size_t stride;
	size_t offset;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//VERTEX DEFINITION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class VertexDefinition {
public:
	//INIT
	static void InitializeVertexDefinitions();

	//GETTERS SETTERS
	size_t					GetVertexSize() const		{ return m_attribs[0].stride;	}
	size_t					GetStride() const			{ return m_attribs[0].stride;	}
	const VertexAttribute*	GetAllAttributes() const	{ return m_attribs;				}
	uint8_t					GetNumAttribs() const		{ return m_numAttribs;			}
	VertexType				GetVertexType() const		{ return m_vertexType;			}

	static VertexDefinition VERTEX;
	static VertexDefinition TEXTUREDVERTEX;
	static VertexDefinition TEXTUREDVERTEX_TBN;
	static VertexDefinition TEXTUREDVERTEX_N;
	static VertexDefinition TEXTUREDVERTEX_TBNBN;
	static VertexDefinition VERTEX2D_PCT;

private:
	//INIT
	VertexDefinition(uint8_t numAttribs);

	void SetAttribute(const char* name, unsigned int numElements, eRenderEnum primitiveType, eRenderEnum isNormalized, size_t size, size_t offset);

private:
	VertexAttribute* m_attribs;
	uint8_t m_numAttribs;
	//std::vector<VertexAttribute, UntrackedAllocator<VertexAttribute>> m_attribs;
	VertexType m_vertexType;
};