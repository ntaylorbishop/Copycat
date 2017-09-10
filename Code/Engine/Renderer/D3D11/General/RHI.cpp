#include "Engine/Renderer/D3D11/General/RHI.hpp"
#include "Engine/Renderer/D3D11/General/RHIDeviceWindow.hpp"


//---------------------------------------------------------------------------------------------------------------------------
void RHI::ClearRenderTarget(const RGBA& clearColor) {

	float col[4] = { clearColor.r, clearColor.g, clearColor.b, clearColor.a };
	GetDeviceContext()->ClearRenderTargetView(RHIDeviceWindow::Get()->m_pRenderTargetView, col);

}
