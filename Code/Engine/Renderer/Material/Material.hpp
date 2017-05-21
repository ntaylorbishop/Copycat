#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/Shaders/Uniform.hpp"
#include "Engine/Renderer/Lights/LightSystem.hpp"
#include "Engine/Renderer/Vertex/VertexDefinition.hpp"
#include "Engine/Renderer/Shaders/ShaderProgram.hpp"
#include "Engine/Renderer/Material/RenderState.hpp"

class ShaderProgram;
class TextureBuffer;

class Material {
public:
	//STRUCTORS
	Material();
	Material(const String& shaderName);
	~Material();

	void InitializeDefaultRenderState();

	//NAME
	String GetName() const								{ return m_name; }
	void SetName(const String& name)					{ m_name = name; }
	void SetShaderProgram(const char* shaderProgName)	{ m_shaderProgram = ShaderProgram::CreateOrGetShaderProgram(shaderProgName); }

	//USING MATERIAL
	void UseMaterial(VertexDefinition* vertexDef, std::vector<Uniform*>* mrUniforms);
	void StopUsingMaterial();

	//UNIFORMS
	void						CreateUniform(const char* name, eUniformType type, size_t count);
	void						CreateUniform(const char* name, eUniformType type, size_t count, int bindPoint);
	template <typename T> void	CreateUniform(const char* name, eUniformType type, size_t count, int bindPoint, T* dataArr);
	template <typename T> void	CreateUniform(const char* name,	eUniformType type, size_t count, T* dataArr);
	Uniform*					GetUniform(const char* uniformName);
	ShaderProgram*				GetShaderProgram() const { return m_shaderProgram; }



	std::vector<Uniform*>	m_uniforms;
	String					m_name			= "NULL";
	ShaderProgram*			m_shaderProgram	= nullptr;
	RenderState				m_renderState	= RenderState::RENDER_STATE_DEFAULT;
	
private:

	void OverrideUniforms(std::vector<Uniform*>& uniformsToBind);

	friend class MaterialBank;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UNIFORM TEMPLATE DEFS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template <typename T> 
void Material::CreateUniform(const char* name, eUniformType type, size_t count, int bindPoint, T* dataArr) {

	ASSERT_OR_DIE(Uniform::IsCorrectType(type, dataArr), "ERROR: Can't create uniform. Wrong type.");

	if (Uniform::NeedsBindPoint(type)) {

		Uniform* uni = m_shaderProgram->GetUniform(name);
		ASSERT_OR_DIE(uni != nullptr, "ERROR: Uniform not found in ShaderProgram for this material.");
		bindPoint = uni->GetBindPoint();
	}

	GLint shaderLoc = INVALID_SHADER_LOC;
	GL_CHECK(shaderLoc = glGetUniformLocation((GLuint)m_shaderProgram->GetProgramID(), name));
	Uniform* nUniform = new(MEMORY_RENDERING) Uniform(name, count, type, shaderLoc, bindPoint, (void*)dataArr);
	m_uniforms.push_back(nUniform);
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T> 
void Material::CreateUniform(const char* name, eUniformType type, size_t count, T* dataArr) {

	ASSERT_OR_DIE(!Uniform::NeedsBindPoint(type), "ERROR: Texture uniforms must have bind point specified.");
	CreateUniform(name, type, count, -1, dataArr);
}


//---------------------------------------------------------------------------------------------------------------------------
inline void Material::CreateUniform(const char* name, eUniformType type, size_t count) {

	CreateUniform(name, type, count, -1);
}


//---------------------------------------------------------------------------------------------------------------------------
inline void Material::CreateUniform(const char* name, eUniformType type, size_t count, int bindPoint) {

	GLint shaderLoc = INVALID_SHADER_LOC;
	GL_CHECK(shaderLoc = glGetUniformLocation((GLuint)m_shaderProgram->GetProgramID(), name));
	Uniform* nUniform = new(MEMORY_RENDERING) Uniform(name, count, type, shaderLoc, bindPoint, nullptr);
	m_uniforms.push_back(nUniform);
}