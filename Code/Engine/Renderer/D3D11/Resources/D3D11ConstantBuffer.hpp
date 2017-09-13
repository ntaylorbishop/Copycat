#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/EventSystem/NamedProperties.hpp"
#include "Engine/Renderer/D3D11/General/RHIDeviceWindow.hpp"
#include "Engine/Renderer/D3D11/Shaders/D3D11Uniform.hpp"



class D3D11ConstantBuffer {
public:
	D3D11ConstantBuffer() { }
	D3D11ConstantBuffer(uint byteSizeOfBuffer);
	~D3D11ConstantBuffer();

	void CreateBufferOnDevice();
	void ReleaseLocalBuffer();
	void UpdateBufferOnDevice();

	void AddUniform(D3D11Uniform* uniToAdd) { m_uniforms.push_back(uniToAdd); }

	byte* GetBuffer() { return m_pByteBuffer; }
	ID3D11Buffer* GetDeviceBufferHandle() { return m_pDeviceBuffer; }

private:
	String						m_name				= "NULL";
	std::vector<D3D11Uniform*>	m_uniforms;
	byte*						m_pByteBuffer		= nullptr;
	uint						m_currSizeOfBuffer	= 0;
	uint						m_bufferSize		= 0;
	ID3D11Buffer*				m_pDeviceBuffer		= nullptr;
};



template<typename T>
void D3D11ConstantBuffer::AddElement(T elem) {

	memcpy_s(m_pByteBuffer + m_bufferSize, m_bufferSize, (void*)&elem, sizeof(T));
	m_bufferSize += sizeof(T);
}

/*
template<typename T>
void D3D11ConstantBuffer::AddElement(const T elem) {

	memcpy_s(m_pByteBuffer + m_bufferSize, m_bufferSize, (void*)&elem, sizeof(T));
	m_bufferSize += sizeof(T);
}*/