#pragma once

#include <functional>

#include "Engine/Renderer/Shaders/Uniform.hpp"
#include "Engine/Renderer/Shaders/Shader.hpp"
#include "Engine/Renderer/Vertex/VertexDefinition.hpp"
#include "Engine/Renderer/Shaders/ComputeShader.hpp"

class SSBO;

class ComputeShaderProgram {
public:
	//INIT
	static ComputeShaderProgram* CreateOrGetComputeShaderProgram(const String& shaderProgName);
	static void DeleteComputeShaderPrograms();

	//DISPATCH
	void Dispatch(uint xWorkGroup, uint yWorkGroup, uint zWorkGroup);

	//ADDING UNIFORMS
	template <typename T> void CreateUniform(const char* name, eUniformType type, size_t count, T* dataArr);
	template <typename T> void CreateUniform(const char* name, eUniformType type, size_t count, const T* dataArr);
	template <typename T> void CreateUniform(const char* name, eUniformType type, size_t count, int bindPoint, const T* dataArr);
	template <typename T> void CreateUniform(const char* name, eUniformType type, size_t count, int bindPoint, T* dataArr);

	Uniform* GetUniform(const char* name);

	void AddSSBO(uint bindPoint, SSBO* ssbo) { m_ssbos.push_back(std::pair<uint, SSBO*>(bindPoint, ssbo)); }


	//BIND
	void BindSSBOs() const;
	SSBO* GetSSBOWithBindPoint(uint bindPoint);

private:
	//STRUCTORS INIT
	ComputeShaderProgram();
	ComputeShaderProgram(const ComputeShader& cShader);
	~ComputeShaderProgram();

	//CREATE AND BIND
	void BindUniforms() const;


	int CreateAndLinkProgram();

	int m_programID;
	ComputeShader m_computeShader;
	std::map<size_t, Uniform*> m_uniforms;
	std::vector<std::pair<uint, SSBO*>> m_ssbos;

	static std::map<size_t, ComputeShaderProgram*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, ComputeShaderProgram*>>> s_ComputeShaderProgramRegistry;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TYPEDEFS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef std::map<size_t, ComputeShaderProgram*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, ComputeShaderProgram*>>>			ComputeShaderProgramMap;
typedef std::map<size_t, ComputeShaderProgram*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, ComputeShaderProgram*>>>::iterator	ComputeShaderProgramMapIter;
typedef std::pair<size_t, ComputeShaderProgram*>																							ComputeShaderProgramMapPair;
typedef std::map<size_t, Uniform*>::iterator																								ComputeShaderProgramUniformMapIter;
typedef std::map<size_t, Uniform*>::const_iterator																							ComputeShaderProgramUniformMapConstIter;
typedef std::pair<size_t, Uniform*>																											ComputeShaderProgramUniformMapPair;
typedef std::vector<std::pair<uint, SSBO*>>																									SSBOArray;
typedef std::vector<std::pair<uint, SSBO*>>::iterator																						SSBOArrayIterator;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TEMPLATE DEFINITIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADD AND SET UNIFORMS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
void ComputeShaderProgram::CreateUniform(const char* name, eUniformType type, size_t count, T* data) {

	ASSERT_OR_DIE(!Uniform::NeedsBindPoint(type), "ERROR: Texture uniform must have bind point specified.");
	CreateUniform(name, type, count, -1, data);
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T> 
void ComputeShaderProgram::CreateUniform(const char* name, eUniformType type, size_t count, const T* dataArr) {

	ASSERT_OR_DIE(!Uniform::NeedsBindPoint(type), "ERROR: Texture uniform must have bind point specified.");
	T* dataArrNonConst = const_cast<T*>(dataArr);
	CreateUniform(name, type, count, -1, dataArrNonConst);
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T> 
void ComputeShaderProgram::CreateUniform(const char* name, eUniformType type, size_t count, int bindPoint, const T* dataArr) {

	T* dataArrNonConst = const_cast<T*>(dataArr);
	CreateUniform(name, type, count, bindPoint, dataArrNonConst);
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T> 
void ComputeShaderProgram::CreateUniform(const char* name, eUniformType type, size_t count, int bindPoint, T* dataArr) {

	ASSERT_OR_DIE(Uniform::IsCorrectType(type, dataArr), "ERROR: Can't create uniform. Wrong type.");

	GLint shaderLoc = INVALID_SHADER_LOC;
	GL_CHECK(shaderLoc = glGetUniformLocation((GLuint)m_programID, name));
	Uniform* nUniform = new(MEMORY_RENDERING) Uniform(name, count, type, shaderLoc, bindPoint, (void*)dataArr);
	m_uniforms.insert(ComputeShaderProgramUniformMapPair(nUniform->GetNameHash(), nUniform));
}