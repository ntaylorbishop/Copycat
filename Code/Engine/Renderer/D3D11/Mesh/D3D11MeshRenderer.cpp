#include "Engine/Renderer/D3D11/Mesh/D3D11MeshRenderer.hpp"
#include "Engine/Renderer/D3D11/Mesh/D3D11Mesh.hpp"
#include "Engine/Renderer/D3D11/Material/D3D11Material.hpp"
#include "Engine/Renderer/D3D11/General/RHI.hpp"


//---------------------------------------------------------------------------------------------------------------------------
void D3D11MeshRenderer::RenderMeshWithMaterial(D3D11Mesh* mesh, D3D11Material* mat) {

	//TODO: Need to add overrides for mesh renderer
	RHI::ClearRenderTarget(RGBA(0.1f, 0.1f, 0.1f, 1.f));

	mesh->BindIndBufferToDeviceWindow();

	mat->Use();
	RHIDeviceWindow::Get()->m_pDeviceContext->DrawIndexed(mesh->GetNumInds(), 0, 0);
	RHIDeviceWindow::Get()->m_pSwapChain->Present(0, 0);
}