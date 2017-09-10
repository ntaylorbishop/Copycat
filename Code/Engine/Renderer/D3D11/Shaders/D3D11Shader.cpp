#include "Engine/Renderer/D3D11/Shaders/D3D11Shader.hpp"
#include "Engine/Renderer/D3D11/Shaders/RHIShaderUtils.hpp"
#include "Engine/Renderer/D3D11/General/RHIDeviceWindow.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
static ID3DBlob* CompileShader(const char* shaderName, eD3D11ShaderType shaderType) {

	String shaderPath = String(shaderName);
	String entryPoint;

	if (shaderType == D3D11SHADERTYPE_VERTEX) {
		entryPoint = "VS";
	}
	else if (shaderType == D3D11SHADERTYPE_FRAGMENT) {
		entryPoint = "PS";
	}

	// Compile the vertex shader
	ID3DBlob* compiledBlob = nullptr;
	String entrypoint;
	String shaderModel;

	if (shaderType == D3D11SHADERTYPE_VERTEX) {
		entryPoint = "VS";
		shaderModel = "vs_4_0";
	}
	else if (shaderType == D3D11SHADERTYPE_FRAGMENT) {
		entryPoint = "PS";
		shaderModel = "ps_4_0";
	}

	compiledBlob = CompileShaderFromFile(shaderPath.c_str(), entryPoint.c_str(), shaderModel.c_str());

	return compiledBlob;
}


//---------------------------------------------------------------------------------------------------------------------------
D3D11Shader::D3D11Shader(const char* shaderName, eD3D11ShaderType shaderType)
	: m_shaderName(shaderName)
	, m_shaderType(shaderType)
{
	m_compiledBlob = CompileShader(shaderName, shaderType);
}


//---------------------------------------------------------------------------------------------------------------------------
ID3D11VertexShader* D3D11Shader::CreateVertexShader() {

	ID3D11VertexShader* handle;
	ID3D11Device* device = RHIDeviceWindow::Get()->GetD3DDevice();
	HRESULT hr = device->CreateVertexShader(m_compiledBlob->GetBufferPointer(), m_compiledBlob->GetBufferSize(), nullptr, &handle);

	if (FAILED(hr)) {
		m_compiledBlob->Release();
		ERROR_AND_DIE("ERROR: Could not create D3D11 shader.");
	}

	return handle;
}


//---------------------------------------------------------------------------------------------------------------------------
ID3D11PixelShader* D3D11Shader::CreatePixelShader() {

	ID3D11PixelShader* handle;
	ID3D11Device* device = RHIDeviceWindow::Get()->GetD3DDevice();
	HRESULT hr = device->CreatePixelShader(m_compiledBlob->GetBufferPointer(), m_compiledBlob->GetBufferSize(), nullptr, &handle);

	if (FAILED(hr)) {
		m_compiledBlob->Release();
		ERROR_AND_DIE("ERROR: Could not create D3D11 shader.");
	}

	return handle;
}


//---------------------------------------------------------------------------------------------------------------------------
D3D11Shader::~D3D11Shader() {

}