#include "Engine/Renderer/Vertex/Vertex.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//3D VERTS
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TEXTURED VERTEX
TexturedVertex::TexturedVertex()
	: m_position(Vector3(0.f, 0.f, 0.f))
	, m_color(RGBA(0.f, 0.f, 0.f, 0.f))
	, m_texCoords(0.f, 0.f)
{}
TexturedVertex::TexturedVertex(const Vector3& pos, const RGBA& col, const Vector2& tex)
	: m_position(pos)
	, m_color(col)
	, m_texCoords(tex)
{}

//VERTEX
Vertex::Vertex()
	: m_position()
	, m_color()
{}
Vertex::Vertex(const Vector3& pos, const RGBA& col)
	: m_position(pos)
	, m_color(col)
{}

//TexturedVertex_TBN 
TexturedVertex_TBN::TexturedVertex_TBN() {

}
TexturedVertex_TBN::TexturedVertex_TBN(const Vector3& pos, const RGBA& col, const Vector2& tex, const Vector3& tangent, const Vector3& bitangent) 
	: m_position(pos)
	, m_color(col)
	, m_texCoords(tex)
	, m_tangent(tangent)
	, m_bitangent(bitangent)
{ }

//TexturedVertex_N
TexturedVertex_N::TexturedVertex_N() {

}
TexturedVertex_N::TexturedVertex_N(const Vector3& pos, const RGBA& col, const Vector2& tex, const Vector3& normal)
	: m_position(pos)
	, m_color(col)
	, m_texCoord(tex)
	, m_normal(normal)
{ }

//TexturedVertex_TBNBN
TexturedVertex_TBNBN::TexturedVertex_TBNBN() { }
TexturedVertex_TBNBN::TexturedVertex_TBNBN(const Vector3& pos, 
	const RGBA& col,
	const Vector2& tex, 
	const Vector3& tangent, 
	const Vector3& bitangent, 
	const Vector3& normal,
	const Vector4& boneWeights, 
	const IntVector4& boneInds
)
	: m_position(pos)
	, m_color(col)
	, m_texCoords(tex)
	, m_tangent(tangent)
	, m_bitangent(bitangent)
	, m_normal(normal)
	, m_boneWeights(boneWeights)
	, m_boneInds(boneInds)
{ }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//2D VERTS
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Vertex2D_PCT::Vertex2D_PCT()
	: m_position(Vector2(0.f, 0.f))
	, m_color(RGBA::WHITE)
	, m_texCoords(Vector2(0.f, 0.f))
{ }
Vertex2D_PCT::Vertex2D_PCT(const Vector2& pos, const RGBA& col, const Vector2& tex)
	: m_position(pos)
	, m_color(col)
	, m_texCoords(tex)
{ }