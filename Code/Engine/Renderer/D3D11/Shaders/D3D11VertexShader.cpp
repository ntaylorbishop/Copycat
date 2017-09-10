#include "Engine/Renderer/D3D11/Shaders/D3D11VertexShader.hpp"


//---------------------------------------------------------------------------------------------------------------------------
void D3D11VertexShader::BindVertexLayoutToDeviceWindow(eVertexType vertType) {

	UINT numElements = 0;
	SetInputLayout(vertType, m_pInputDesc, numElements);

	HRESULT hr = RHIDeviceWindow::Get()->m_pd3dDevice->CreateInputLayout(m_pInputDesc, numElements,
		GetCompiledBlob()->GetBufferPointer(),
		GetCompiledBlob()->GetBufferSize(), &m_pVertLayout);

	GetCompiledBlob()->Release();

	if (FAILED(hr))
		ERROR_AND_DIE("HR FAILED");

	// Set the input layout
	RHIDeviceWindow::Get()->m_pDeviceContext->IASetInputLayout(m_pVertLayout);
}