#pragma once

#include "Engine/Renderer/D3D11/Shaders/D3D11Shader.hpp"

class D3D11PixelShader : public D3D11Shader {
public:
	D3D11PixelShader(const char* shaderName, eD3D11ShaderType shaderType) 
		: D3D11Shader(shaderName, shaderType) 
		, m_shaderHandle(CreatePixelShader())
	{ }
	~D3D11PixelShader() = default;


	ID3D11PixelShader* GetShaderHandle() { return m_shaderHandle; }


private:
	ID3D11PixelShader*		m_shaderHandle = nullptr;
};