#include "Engine/Config/BuildConfig.hpp"

#ifdef GRAPHICS_API_OGL
#include "Engine/Renderer/Shaders/ComputeShader.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/Shaders/ShaderUtils.hpp"
#include "Engine/Renderer/General/Texture.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
ComputeShader::ComputeShader() 
	: m_filename("NULL")
	, m_shaderID(0)
{ }
//---------------------------------------------------------------------------------------------------------------------------
ComputeShader::ComputeShader(const String& filename) 
	: m_filename(filename)
	, m_shaderID(0)
{ }


//---------------------------------------------------------------------------------------------------------------------------
void ComputeShader::Load() {
	ShaderUtils::CreateAndLoadShader(m_shaderID, SHADER_COMPUTE, m_filename);
}

#endif