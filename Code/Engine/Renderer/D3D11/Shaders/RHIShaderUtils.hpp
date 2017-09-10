#pragma once

#include "Engine/Renderer/D3D11/Shaders/D3D11Shader.hpp"


ID3DBlob* CompileShaderFromFile(const char* fileName, const char* entryPoint, const char* shaderModel);