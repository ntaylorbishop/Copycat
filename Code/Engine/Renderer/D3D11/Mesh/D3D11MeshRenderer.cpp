#include "Engine/Renderer/D3D11/Mesh/D3D11MeshRenderer.hpp"
#include "Engine/Renderer/D3D11/Mesh/D3D11Mesh.hpp"
#include "Engine/Renderer/D3D11/Material/D3D11Material.hpp"
#include "Engine/Renderer/D3D11/General/RHI.hpp"


//---------------------------------------------------------------------------------------------------------------------------
void D3D11MeshRenderer::RenderMeshWithMaterial(D3D11Mesh* mesh, D3D11Material* mat) {

	//TODO: Need to add overrides for mesh renderer
	mesh->BindVertBufferToDeviceWindow();
	mesh->BindIndBufferToDeviceWindow();

	mat->Use();
	RHIDeviceWindow::Get()->m_pDeviceContext->DrawIndexed(mesh->GetNumInds(), 0, 0);
}