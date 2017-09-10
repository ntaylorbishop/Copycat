#include "Engine/Renderer/D3D11/Resources/D3D11ConstantBuffer.hpp"


//---------------------------------------------------------------------------------------------------------------------------
D3D11ConstantBuffer::D3D11ConstantBuffer(uint byteSizeOfBuffer)
	: m_bufferSize(byteSizeOfBuffer)
{
	m_pByteBuffer = new byte[byteSizeOfBuffer];
}


//---------------------------------------------------------------------------------------------------------------------------
D3D11ConstantBuffer::~D3D11ConstantBuffer() {

	delete m_pByteBuffer;
	m_pByteBuffer = nullptr;
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11ConstantBuffer::CreateBufferOnDevice() {

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = m_bufferSize;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	HRESULT hr = RHIDeviceWindow::Get()->m_pd3dDevice->CreateBuffer(&bufferDesc, nullptr, &m_pDeviceBuffer);

	if (FAILED(hr))
		ERROR_AND_DIE("HR FAILED");
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11ConstantBuffer::ReleaseLocalBuffer() {

	delete m_pByteBuffer;
	m_pByteBuffer = nullptr;
}


//---------------------------------------------------------------------------------------------------------------------------
void D3D11ConstantBuffer::UpdateBufferOnDevice() {

	byte* pCurrSpotInBuffer = m_pByteBuffer;

	for (size_t i = 0; i < m_uniforms.size(); i++) {

		D3D11Uniform* currUniform = m_uniforms[i];
		size_t uniSize = currUniform->GetByteSize();

		memcpy_s(pCurrSpotInBuffer, m_bufferSize, currUniform->GetData(), uniSize);
		pCurrSpotInBuffer += currUniform->GetByteSize();
	}

	RHIDeviceWindow::Get()->m_pDeviceContext->UpdateSubresource(m_pDeviceBuffer, 0, nullptr, m_pByteBuffer, 0, 0);
}