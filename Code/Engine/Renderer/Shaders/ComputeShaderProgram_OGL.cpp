#include "Engine/Config/BuildConfig.hpp"

#ifdef GRAPHICS_API_OGL
#include "Engine/Renderer/Shaders/ComputeShaderProgram.hpp"
#include "Engine/General/Core/TheMemoryManager.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Shaders/ShaderUtils.hpp"
#include "Engine/Renderer/Structures/SSBO.hpp"

STATIC std::map<size_t, ComputeShaderProgram*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, ComputeShaderProgram*>>> ComputeShaderProgram::s_ComputeShaderProgramRegistry;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
STATIC ComputeShaderProgram* ComputeShaderProgram::CreateOrGetComputeShaderProgram(const String& shaderProgName) {

	size_t hash = std::hash<String>()(shaderProgName);
	ComputeShaderProgramMapIter it = s_ComputeShaderProgramRegistry.find(hash);

	if (it != s_ComputeShaderProgramRegistry.end()) {
		return it->second;
	}
	else {
		ComputeShader cShader = ComputeShader("Data/Shaders/" + shaderProgName + ".glsl");
		ComputeShaderProgram* nComputeShaderProgram = new(MEMORY_RENDERING) ComputeShaderProgram(cShader);

		s_ComputeShaderProgramRegistry.insert(ComputeShaderProgramMapPair(hash, nComputeShaderProgram));
		return nComputeShaderProgram;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void ComputeShaderProgram::DeleteComputeShaderPrograms() {

	std::map<size_t, ComputeShaderProgram*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, ComputeShaderProgram*>>>::iterator it;
	for (it = s_ComputeShaderProgramRegistry.begin(); it != s_ComputeShaderProgramRegistry.end(); ++it) {
		delete it->second;
		it->second = nullptr;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DISPATCH
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void ComputeShaderProgram::Dispatch(uint xWorkGroup, uint yWorkGroup, uint zWorkGroup) {

	BindSSBOs();
	GL_CHECK(glUseProgram((GLuint)m_programID));
	BindUniforms();
	GL_CHECK(glDispatchCompute(xWorkGroup, yWorkGroup, zWorkGroup));
	GL_CHECK(glUseProgram(NULL));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADDING UNIFORMS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//---------------------------------------------------------------------------------------------------------------------------
Uniform* ComputeShaderProgram::GetUniform(const char* name) {

	size_t nameHash = HashCString(name);
	ComputeShaderProgramUniformMapIter it = m_uniforms.find(nameHash);

	if (it != m_uniforms.end()) {
		return it->second;
	}
	else {
		return nullptr;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
SSBO* ComputeShaderProgram::GetSSBOWithBindPoint(uint bindPoint) {

	for (uint i = 0; i < m_ssbos.size(); i++) {
		if (m_ssbos[i].first == bindPoint) {
			return m_ssbos[i].second;
		}
	}
	return nullptr;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
ComputeShaderProgram::ComputeShaderProgram() { }


//---------------------------------------------------------------------------------------------------------------------------
ComputeShaderProgram::ComputeShaderProgram(const ComputeShader& cShader)
	: m_computeShader(cShader)
{
	m_computeShader.Load();
	CreateAndLinkProgram();
}


//---------------------------------------------------------------------------------------------------------------------------
ComputeShaderProgram::~ComputeShaderProgram() {

	ComputeShaderProgramUniformMapIter it;

	for (it = m_uniforms.begin(); it != m_uniforms.end(); it++) {
		delete it->second;
		it->second = nullptr;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BIND AND CREATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void ComputeShaderProgram::BindSSBOs() const {
	
	for (uint i = 0; i < m_ssbos.size(); i++) {
		m_ssbos[i].second->Bind(m_ssbos[i].first);
	}
}



//---------------------------------------------------------------------------------------------------------------------------
void ComputeShaderProgram::BindUniforms() const {

	ComputeShaderProgramUniformMapConstIter uniformIt = m_uniforms.begin();

	for (uniformIt; uniformIt != m_uniforms.end(); ++uniformIt) {

		const Uniform* currUniform = uniformIt->second;
		currUniform->Bind();
	}
}


//---------------------------------------------------------------------------------------------------------------------------
int ComputeShaderProgram::CreateAndLinkProgram() {

	GL_CHECK(m_programID = glCreateProgram());

	int computeShaderID = m_computeShader.GetShaderID();
	String computeShaderPath = m_computeShader.GetFileName();

	if (m_programID == NULL) {

		SystemDialogue_Okay("Shader Problem", "Couldn't Create Program", SEVERITY_WARNING);
		return -1;
	}
	else {

		GL_CHECK(glAttachShader(m_programID, computeShaderID));
		GL_CHECK(glLinkProgram(m_programID));

		GLint status;
		GL_CHECK(glGetProgramiv(m_programID, GL_LINK_STATUS, &status));

		if (status == GL_FALSE) {
			GLint log_length;
			GL_CHECK(glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &log_length));

			char* buffer = new(MEMORY_RENDERING) char[log_length + 1];
			GL_CHECK(glGetProgramInfoLog(m_programID, log_length, &log_length, buffer));

			buffer[log_length] = NULL;
			ShaderUtils::PrintShaderCompileErrorToOutput(buffer, computeShaderPath);
			delete buffer;
			GL_CHECK(glDeleteProgram(m_programID));

			return -1;
		}
		else {
			GL_CHECK(glDetachShader(m_programID, computeShaderID));

			return m_programID;
		}
	}
}

#endif