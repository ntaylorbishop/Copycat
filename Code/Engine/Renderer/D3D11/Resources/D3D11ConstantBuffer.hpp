#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/EventSystem/NamedProperties.hpp"
#include "Engine/Renderer/D3D11/General/RHIDeviceWindow.hpp"
#include "Engine/Renderer/D3D11/Shaders/D3D11Uniform.hpp"


//---------------------------------------------------------------------------------------------------------------------------
class D3D11ConstantBuffer {
public:
	static D3D11ConstantBuffer* GetConstantBuffer(const String& cBufferName);
	static D3D11ConstantBuffer* CreateOrGetConstantBuffer(const String& cBufferName, uint byteSizeOfBuffer);

	void CreateBufferOnDevice();
	void ReleaseLocalBuffer();
	void UpdateBufferOnDevice();
	void UpdateBufferOnDevice(const std::vector<D3D11Uniform*>& overrideUniforms);

	void AddUniform(D3D11Uniform* uniToAdd) { m_uniforms.push_back(uniToAdd); }

	byte* GetBuffer()						{ return m_pByteBuffer;		}
	ID3D11Buffer* GetDeviceBufferHandle()	{ return m_pDeviceBuffer;	}
	String GetName() const					{ return m_name;			}

	D3D11Uniform* GetUniform(const String& name);

private:
	D3D11ConstantBuffer() { }
	D3D11ConstantBuffer(const String& cBufferName, uint byteSizeOfBuffer);
	~D3D11ConstantBuffer();

	String						m_name				= "NULL";
	std::vector<D3D11Uniform*>	m_uniforms;
	byte*						m_pByteBuffer		= nullptr;
	uint						m_currSizeOfBuffer	= 0;
	uint						m_bufferSize		= 0;
	ID3D11Buffer*				m_pDeviceBuffer		= nullptr;

	static std::map<size_t, D3D11ConstantBuffer*> s_cBufferRegistry;

	friend class D3D11ShaderProgram;
	friend class D3D11Material;
};

typedef std::map<size_t, D3D11ConstantBuffer*>				D3D11CBufferMap;
typedef std::map<size_t, D3D11ConstantBuffer*>::iterator	D3D11CBufferMapIter;
typedef std::pair<size_t, D3D11ConstantBuffer*>				D3D11CBufferMapPair;