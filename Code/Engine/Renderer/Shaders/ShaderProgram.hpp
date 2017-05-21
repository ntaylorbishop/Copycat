#pragma once

#include <functional>

#include "Engine/General/Core/TheMemoryManager.hpp"
#include "Engine/Renderer/Shaders/Uniform.hpp"
#include "Engine/Renderer/Shaders/Shader.hpp"
#include "Engine/Renderer/Vertex/VertexDefinition.hpp"

class SSBO;

class ShaderProgram {
public:
	//CREATION DELETION USE
	static ShaderProgram* CreateOrGetShaderProgram(const String& shaderProgName);
	static void DeleteShaderPrograms();

	void UseProgram() const;

	//UNIFORMS
	template <typename T> void CreateUniform( const char* name, eUniformType type, size_t count, T* dataArr);
	template <typename T> void CreateUniform( const char* name, eUniformType type, size_t count, const T* dataArr);
	template <typename T> void CreateUniform( const char* name, eUniformType type, size_t count, int bindPoint, const T* dataArr);
	template <typename T> void CreateUniform( const char* name, eUniformType type, size_t count, int bindPoint, T* dataArr);
	void CreateUniform(const char* name, eUniformType type, size_t count, void* dataArr);
	void CreateUniform(const char* name, eUniformType type, size_t count, int bindPoint, void* dataArr);

	Uniform* GetUniform(const char* name);

	void	SetName(const String& progName) { m_name = progName;	}
	String	GetName() const					{ return m_name;		}

	//ADD SSBOS
	void AddSSBO(uint bindPoint, SSBO* ssbo) { m_ssbos.push_back(std::pair<uint, SSBO*>(bindPoint, ssbo)); }


	//BINDING
	void BindAttributes(VertexDefinition* vertexDef) const;
	uint BindUniforms(const std::vector<Uniform*>* matUniforms) const;
	void BindSSBOs() const;

	int GetProgramID() const { return m_programID; }
	void Load(const String& vertShaderName, const String& fragShaderName);

private:
	//STRUCTORS
	ShaderProgram();
	ShaderProgram(const Shader& vShader, const Shader& fShader);
	~ShaderProgram();


	//BINDING AND CREATION
	int CreateAndLinkProgram();
	void BindVertexAttributes(VertexDefinition* vertexDef) const;
	void BindProperty(const String& name, int32_t count, int32_t type, int normalize, int stride, size_t offset) const;


	static std::map<size_t, ShaderProgram*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, ShaderProgram*>>> s_shaderProgramRegistry;

	int		m_programID			= 0;
	String	m_name				= "NULL";
	Shader	m_vertexShader;
	Shader	m_fragmentShader;

	std::vector<std::pair<uint, SSBO*>> m_ssbos;
	std::map<size_t, Uniform*> m_uniforms;

	friend class ShaderProgramParser;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TYPEDEFS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef std::map<size_t, ShaderProgram*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, ShaderProgram*>>>				ShaderProgramMap;
typedef std::map<size_t, ShaderProgram*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, ShaderProgram*>>>::iterator	ShaderProgramMapIter;
typedef std::pair<size_t, ShaderProgram*>																						ShaderProgramMapPair;
typedef std::pair<size_t, Uniform*>																								ShaderProgramUniformMapPair;
typedef std::map<size_t, Uniform*>::iterator																					ShaderProgramUniformMapIter;
typedef std::map<size_t, Uniform*>::const_iterator																				ShaderProgramUniformMapConstIter;
typedef std::pair<uint, SSBO*>																									BindPointSSBOPair;
typedef std::vector<std::pair<uint, SSBO*>>::iterator																			BindPointSSBOIterator;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TEMPLATE DEFINITIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADD AND SET UNIFORMS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
void ShaderProgram::CreateUniform(const char* name, eUniformType type, size_t count, int bindPoint, T* data) {

	ASSERT_OR_DIE(Uniform::IsCorrectType(type, data), "ERROR: Can't create uniform. Wrong type.");

	GLint shaderLoc = INVALID_SHADER_LOC;

	if (count == 1) {
		GL_CHECK(shaderLoc = glGetUniformLocation((GLuint)m_programID, name));
	}
	else {
		String arrName = StringUtils::Stringf("%s[0]", name);
		GL_CHECK(shaderLoc = glGetUniformLocation((GLuint)m_programID, arrName.c_str()));
	}

	Uniform* nUniform = new(MEMORY_RENDERING) Uniform(name, count, type, shaderLoc, bindPoint, (void*)data);
	m_uniforms.insert(ShaderProgramUniformMapPair(nUniform->GetNameHash(), nUniform));
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T> 
void ShaderProgram::CreateUniform(const char* name, eUniformType type, size_t count, const T* dataArr) {

	ASSERT_OR_DIE(!Uniform::NeedsBindPoint(type), "ERROR: Texture uniforms must explicitly specify bind points.");
	T* dataArrNonConst = const_cast<T*>(dataArr);
	CreateUniform(name, type, count, -1, dataArrNonConst);
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T> 
void ShaderProgram::CreateUniform(const char* name, eUniformType type, size_t count, T* dataArr) {

	ASSERT_OR_DIE(!Uniform::NeedsBindPoint(type), "ERROR: Texture uniforms must explicitly specify bind points.");
	CreateUniform(name, type, count, -1, dataArr);
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
void ShaderProgram::CreateUniform(const char* name, eUniformType type, size_t count, int bindPoint, const T* dataArr) {

	T* dataArrNonConst = const_cast<T*>(dataArr);
	CreateUniform(name, type, count, bindPoint, dataArrNonConst);
}