#include "Engine/Config/BuildConfig.hpp"

#ifdef GRAPHICS_API_OGL
#include "Engine/Renderer/Material/Material.hpp"
#include "Engine/Renderer/Shaders/ShaderProgram.hpp"
#include "Engine/General/Core/TheMemoryManager.hpp"
#include "Engine/Renderer/General/TextureBuffer.hpp"
#include "Engine/General/Logger/TheLogger.hpp"

#define STATIC

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
Material::Material()
	: m_shaderProgram(nullptr)
	, m_name("NOSHADERPROGRAM")
{ }


//---------------------------------------------------------------------------------------------------------------------------
Material::Material(const String& shaderName)
	: m_shaderProgram(nullptr)
{
	m_shaderProgram = ShaderProgram::CreateOrGetShaderProgram(shaderName);
	m_name = shaderName;
}


//---------------------------------------------------------------------------------------------------------------------------
Material::~Material() {

	for (unsigned int i = 0; i < m_uniforms.size(); i++) {
		delete m_uniforms[i];
		m_uniforms[i] = nullptr;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//USING MATERIALS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
void Material::UseMaterial(VertexDefinition* vertexDef, std::vector<Uniform*>* mrUniforms) {

	//Hook up MeshRenderer uniform locs in shader
	for (size_t i = 0; i < mrUniforms->size(); i++) {

		const char* name	= mrUniforms->at(i)->GetName();
		GLint shaderLoc		= INVALID_SHADER_LOC;
		GL_CHECK(shaderLoc	= glGetUniformLocation((GLuint)m_shaderProgram->GetProgramID(), name));
		mrUniforms->at(i)->SetLocInShader(shaderLoc);
		//TheLogger::LogPrintf("WARNING: MeshRenderer Uniform not found in shader", "MATERIAL BINDING", LOG_DEFAULT);
	}


	std::vector<Uniform*> uniformsToPass;

	for (size_t i = 0; i < m_uniforms.size(); i++) {

		bool foundInMRUniforms = false;
		for (size_t j = 0; j < mrUniforms->size(); j++) {
			if (m_uniforms[i]->GetNameHash() == mrUniforms->at(j)->GetNameHash()) {
				foundInMRUniforms = true;
			}
		}

		if (!foundInMRUniforms) {
			uniformsToPass.push_back(m_uniforms[i]);
		}
	}

	for (size_t i = 0; i < mrUniforms->size(); i++) {
		uniformsToPass.push_back(mrUniforms->at(i));
	}

	m_shaderProgram->UseProgram();
	m_shaderProgram->BindAttributes(vertexDef);
	m_shaderProgram->BindUniforms(&uniformsToPass);
	m_shaderProgram->BindSSBOs();
	OverrideUniforms(uniformsToPass);
}


//---------------------------------------------------------------------------------------------------------------------------
void Material::StopUsingMaterial() {
	GL_CHECK(glUseProgram(NULL));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UNIFORMS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
Uniform* Material::GetUniform(const char* uniformName) {

	size_t uniformNameHash = HashCString(uniformName);

	for (unsigned int i = 0; i < m_uniforms.size(); i++) {

		if (m_uniforms[i]->GetNameHash() == uniformNameHash) {
			return m_uniforms[i];
		}
	}

	return nullptr;
}


//---------------------------------------------------------------------------------------------------------------------------
void Material::OverrideUniforms(std::vector<Uniform*>& uniformsToBind) {

	for (uint i = 0; i < uniformsToBind.size(); i++) {

		Uniform*& currUniform = uniformsToBind[i];
		currUniform->Bind();
	}
}


#endif