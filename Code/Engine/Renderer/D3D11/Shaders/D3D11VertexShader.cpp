#include "Engine/Renderer/D3D11/Shaders/D3D11VertexShader.hpp"


//---------------------------------------------------------------------------------------------------------------------------
void D3D11VertexShader::CreateVertexLayout(eVertexType vertType) {

	UINT numElements = 0;
	SetInputLayout(vertType, m_pInputDesc, numElements);

	HRESULT hr = RHIDeviceWindow::Get()->m_pd3dDevice->CreateInputLayout(m_pInputDesc, numElements,
		GetCompiledBlob()->GetBufferPointer(),
		GetCompiledBlob()->GetBufferSize(), &m_pVertLayout);

	GetCompiledBlob()->Release();

	if (FAILED(hr)) {
		ERROR_AND_DIE("HR FAILED");
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11VertexShader::BindVertexLayout() {

	RHIDeviceWindow::Get()->m_pDeviceContext->IASetInputLayout(m_pVertLayout);
}