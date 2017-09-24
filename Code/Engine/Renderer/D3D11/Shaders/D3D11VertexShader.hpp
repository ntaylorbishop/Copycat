#pragma once

#include "Engine/Renderer/D3D11/Shaders/D3D11Shader.hpp"
#include "Engine/Renderer/D3D11/Mesh/D3D11Vertex.hpp"

class D3D11VertexShader : public D3D11Shader {
public:
	D3D11VertexShader(const char* shaderName, eD3D11ShaderType shaderType) 
		: D3D11Shader(shaderName, shaderType) 
		, m_pShaderHandle(CreateVertexShader())
	{ }
	~D3D11VertexShader() = default;


	ID3D11VertexShader* GetShaderHandle() { return m_pShaderHandle; }

	void CreateVertexLayout(eVertexType vertType);
	void BindVertexLayout();


private:
	ID3D11VertexShader*			m_pShaderHandle	= nullptr;
	ID3D11InputLayout*			m_pVertLayout	= nullptr;
	D3D11_INPUT_ELEMENT_DESC*	m_pInputDesc	= nullptr;
};