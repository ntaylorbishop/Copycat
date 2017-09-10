#include "Engine/Renderer/D3D11/Shaders/RHIShaderUtils.hpp"
#include "Engine/Utils/FileUtils/FileUtils.hpp"
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include "Engine/Renderer/Shaders/ShaderUtils.hpp"


//---------------------------------------------------------------------------------------------------------------------------
ID3DBlob* CompileShaderFromFile(const char* fileName, const char* entryPoint, const char* shaderModel) {

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* blobOut = nullptr;

	String workingPath = "Data/Shaders/" + String(fileName);
	WideString wFileName = StringUtils::ConvertStringToWideString(fileName);

	HRESULT compileResult = D3DCompileFromFile(wFileName.c_str(), nullptr, nullptr, entryPoint, shaderModel, 
		shaderFlags, 0, &blobOut, &errorBlob);

	if (FAILED(compileResult)) {
		if (errorBlob) {
			const char* errCStr = reinterpret_cast<const char*>(errorBlob->GetBufferPointer());
			ShaderUtils::PrintShaderCompileErrorToOutput(String(errCStr), String(fileName));
			errorBlob->Release();
		}
	}
	if (errorBlob) {
		errorBlob->Release();
	}

	return blobOut;
}