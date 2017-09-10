#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include <d3d11_1.h>

enum eD3D11ShaderType {
	D3D11SHADERTYPE_VERTEX = 0,
	D3D11SHADERTYPE_FRAGMENT,
	D3D11SHADERTYPE_NUM_SHADERTYPES
};

class D3D11Shader {
public:
	~D3D11Shader();

	ID3DBlob* GetCompiledBlob() { return m_compiledBlob; }

protected:
	D3D11Shader(const char* shaderName, eD3D11ShaderType shaderType);

	ID3D11VertexShader* CreateVertexShader();
	ID3D11PixelShader* CreatePixelShader();

private:
	String				m_shaderName;
	eD3D11ShaderType	m_shaderType;
	ID3DBlob*			m_compiledBlob;
};