#include "Engine/Renderer/Vertex/VertexDefinition.hpp"

STATIC VertexDefinition VertexDefinition::VERTEX(2);
STATIC VertexDefinition VertexDefinition::TEXTUREDVERTEX(3);
STATIC VertexDefinition VertexDefinition::TEXTUREDVERTEX_TBN(5);
STATIC VertexDefinition VertexDefinition::TEXTUREDVERTEX_N(4);
STATIC VertexDefinition VertexDefinition::TEXTUREDVERTEX_TBNBN(8);
STATIC VertexDefinition VertexDefinition::VERTEX2D_PCT(3);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INIT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
STATIC void VertexDefinition::InitializeVertexDefinitions() {
	VERTEX.SetAttribute("inPosition", 3, ENUM_FLOAT, ENUM_TRUE, sizeof(Vertex), offsetof(Vertex, m_position));
	VERTEX.SetAttribute("inColor", 4, ENUM_FLOAT, ENUM_TRUE, sizeof(Vertex), offsetof(Vertex, m_color));
	VERTEX.m_vertexType = VTYPE_VERTEX;

	TEXTUREDVERTEX.SetAttribute("inPosition", 3, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex), offsetof(TexturedVertex, m_position));
	TEXTUREDVERTEX.SetAttribute("inColor", 4, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex), offsetof(TexturedVertex, m_color));
	TEXTUREDVERTEX.SetAttribute("inUV0", 2, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex), offsetof(TexturedVertex, m_texCoords));
	TEXTUREDVERTEX.m_vertexType = VTYPE_TEXTUREDVERTEX;

	TEXTUREDVERTEX_TBN.SetAttribute("inPosition", 3, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex_TBN), offsetof(TexturedVertex_TBN, m_position));
	TEXTUREDVERTEX_TBN.SetAttribute("inColor", 4, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex_TBN), offsetof(TexturedVertex_TBN, m_color));
	TEXTUREDVERTEX_TBN.SetAttribute("inUV0", 2, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex_TBN), offsetof(TexturedVertex_TBN, m_texCoords));
	TEXTUREDVERTEX_TBN.SetAttribute("inTangent", 3, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex_TBN), offsetof(TexturedVertex_TBN, m_tangent));
	TEXTUREDVERTEX_TBN.SetAttribute("inBitangent", 3, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex_TBN), offsetof(TexturedVertex_TBN, m_bitangent));
	TEXTUREDVERTEX_TBN.m_vertexType = VTYPE_TEXTUREDVERTEX_TBN;

	TEXTUREDVERTEX_N.SetAttribute("inPosition", 3, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex_N), offsetof(TexturedVertex_N, m_position));
	TEXTUREDVERTEX_N.SetAttribute("inColor", 4, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex_N), offsetof(TexturedVertex_N, m_color));
	TEXTUREDVERTEX_N.SetAttribute("inUV0", 2, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex_N), offsetof(TexturedVertex_N, m_texCoord));
	TEXTUREDVERTEX_N.SetAttribute("inNormal", 3, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex_N), offsetof(TexturedVertex_N, m_normal));
	TEXTUREDVERTEX_N.m_vertexType = VTYPE_TEXTUREDVERTEX_N;

	TEXTUREDVERTEX_TBNBN.SetAttribute("inPosition", 3, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex_TBNBN), offsetof(TexturedVertex_TBNBN, m_position));
	TEXTUREDVERTEX_TBNBN.SetAttribute("inColor", 4, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex_TBNBN), offsetof(TexturedVertex_TBNBN, m_color));
	TEXTUREDVERTEX_TBNBN.SetAttribute("inUV0", 2, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex_TBNBN), offsetof(TexturedVertex_TBNBN, m_texCoords));
	TEXTUREDVERTEX_TBNBN.SetAttribute("inNormal", 3, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex_TBNBN), offsetof(TexturedVertex_TBNBN, m_normal));
	TEXTUREDVERTEX_TBNBN.SetAttribute("inTangent", 3, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex_TBNBN), offsetof(TexturedVertex_TBNBN, m_tangent));
	TEXTUREDVERTEX_TBNBN.SetAttribute("inBitangent", 3, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex_TBNBN), offsetof(TexturedVertex_TBNBN, m_bitangent));
	TEXTUREDVERTEX_TBNBN.SetAttribute("inBoneWeights", 4, ENUM_FLOAT, ENUM_TRUE, sizeof(TexturedVertex_TBNBN), offsetof(TexturedVertex_TBNBN, m_boneWeights));
	TEXTUREDVERTEX_TBNBN.SetAttribute("inBoneInds", 4, ENUM_INT, ENUM_TRUE, sizeof(TexturedVertex_TBNBN), offsetof(TexturedVertex_TBNBN, m_boneInds));
	TEXTUREDVERTEX_TBNBN.m_vertexType = VTYPE_TEXTUREDVERTEX_TBNBN;

	VERTEX2D_PCT.SetAttribute("inPosition", 2, ENUM_FLOAT, ENUM_TRUE, sizeof(Vertex2D_PCT), offsetof(Vertex2D_PCT, m_position));
	VERTEX2D_PCT.SetAttribute("inColor", 4, ENUM_FLOAT, ENUM_TRUE, sizeof(Vertex2D_PCT), offsetof(Vertex2D_PCT, m_color));
	VERTEX2D_PCT.SetAttribute("inUV0", 2, ENUM_FLOAT, ENUM_TRUE, sizeof(Vertex2D_PCT), offsetof(Vertex2D_PCT, m_texCoords));
	VERTEX2D_PCT.m_vertexType = VTYPE_VERTEX2D_PCT;

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE MEMBER FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
VertexDefinition::VertexDefinition(uint8_t numAttribs) 
	: m_vertexType(VTYPE_NUM_VTYPES) 
{ 
	m_attribs = new VertexAttribute[numAttribs];
}


//---------------------------------------------------------------------------------------------------------------------------
void VertexDefinition::SetAttribute(const char* name, unsigned int numElements, eRenderEnum primitiveType, eRenderEnum isNormalized, size_t size, size_t offset) {

	m_attribs[m_numAttribs].name = name;
	m_attribs[m_numAttribs].count = numElements;
	m_attribs[m_numAttribs].type = primitiveType;
	m_attribs[m_numAttribs].normalize = isNormalized;
	m_attribs[m_numAttribs].stride = size;
	m_attribs[m_numAttribs].offset = offset;

	m_numAttribs++;
}