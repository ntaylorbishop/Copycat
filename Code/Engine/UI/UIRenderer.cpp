#include "Engine/UI/UIRenderer.hpp"
#include "Engine/Renderer/D3D11/Material/D3D11Material.hpp"
#include "Engine/Renderer/D3D11/Texture/Texture2D.hpp"
#include "Engine/Renderer/D3D11/Mesh/D3D11Mesh.hpp"
#include "Engine/Renderer/D3D11/Mesh/D3D11MeshRenderer.hpp"


STATIC UIRenderer* UIRenderer::s_uiRenderer = nullptr;


//---------------------------------------------------------------------------------------------------------------------------
STATIC UIRenderer* UIRenderer::Get() {

	if (!s_uiRenderer) {
		s_uiRenderer = new UIRenderer();
	}
	return s_uiRenderer;
}


//---------------------------------------------------------------------------------------------------------------------------
UIRenderer::UIRenderer() {

	m_defaultOrtho.SetAsOrthographicProjection(1600.f, 900.f, 0.1f, 100.f);
	m_defaultOrtho.Transpose();
	m_2dBlankMat = new D3D11Material("Default2D");

	Texture2D* blankPixel = new Texture2D("Data/Textures/BlankPixel.png", true, TEXTURE_BIND_SHADER_RESOURCE, (eTextureCPUAccessFlags)0);
	D3D11Resource* texID = blankPixel->GetSRVResource();
	m_2dBlankMat->AddResource(0, texID, WHICH_SHADER_FRAGMENT);

	D3D11Uniform* modelUni = new D3D11Uniform("uModel", UNIFORM_MAT4, (void*)&Matrix44::IDENTITY);
	m_2dBlankMat->AddUniform("Model2D", modelUni);


	// Create vertex buffer
	m_quadMesh = new D3D11Mesh(VERTEX_TYPE_PCT, 4);

	m_quadMesh->AddVertex(Vector3(0.f, 0.f, 1.f), RGBA::WHITE, Vector2(1.f, 0.f));
	m_quadMesh->AddVertex(Vector3(0.f, 1.f, 1.f), RGBA::WHITE, Vector2(1.f, 1.f));
	m_quadMesh->AddVertex(Vector3(1.f, 0.f, 1.f), RGBA::WHITE, Vector2(0.f, 1.f));
	m_quadMesh->AddVertex(Vector3(1.f, 1.f, 1.f), RGBA::WHITE, Vector2(0.f, 0.f));

	uint32_t indices[] = { 1,2,0, 3,2,1 };
	//uint32_t indices[] = { 0,2,1, 1,2,3 };

	m_quadMesh->InitializeMeshOnDevice(indices, ARRAYSIZE(indices));
}


//---------------------------------------------------------------------------------------------------------------------------
void UIRenderer::DrawAABB2(const Vector2& pos, const Vector2& size, const RGBA& color, float tint) {

	UNUSED(tint);
	UNUSED(color);

	Matrix44 modelMat = Matrix44::IDENTITY;

	modelMat.SetPosition(Vector3(pos.x, pos.y, 0.f));
	modelMat.Scale(Vector3(size.x, size.y, 1.f));

	D3D11Uniform* uniform = m_2dBlankMat->GetUniform("Model2D", "uModel");
	uniform->SetData((void*)&modelMat);

	m_scratchMR->RenderMeshWithMaterial(m_quadMesh, m_2dBlankMat);
}


//---------------------------------------------------------------------------------------------------------------------------
void UIRenderer::DrawTexturedAABB2(D3D11Material* mat, const Vector2& pos, const Vector2& size) {

	Matrix44 modelMat = Matrix44::IDENTITY;

	modelMat.SetPosition(Vector3(pos.x, pos.y, 0.f));
	modelMat.Scale(Vector3(size.x, size.y, 1.f));

	D3D11Uniform* uniform = mat->GetUniform("Model2D", "uModel");
	ASSERT_OR_DIE(uniform != nullptr, "ERROR: Mat should be based off the Default2D shader program.");
	uniform->SetData((void*)&modelMat);

	m_scratchMR->RenderMeshWithMaterial(m_quadMesh, m_2dBlankMat);
}