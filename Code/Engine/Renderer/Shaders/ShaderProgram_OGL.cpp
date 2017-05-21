#include "Engine/Config/BuildConfig.hpp"

#ifdef GRAPHICS_API_OGL
#include "Engine/Renderer/Shaders/ShaderProgram.hpp"
#include "Engine/General/Core/TheMemoryManager.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Shaders/ShaderUtils.hpp"
#include "Engine/Renderer/Structures/SSBO.hpp"

STATIC std::map<size_t, ShaderProgram*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, ShaderProgram*>>> ShaderProgram::s_shaderProgramRegistry;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
STATIC ShaderProgram* ShaderProgram::CreateOrGetShaderProgram(const String& shaderProgName) {

	size_t hash = std::hash<String>()(shaderProgName);
	ShaderProgramMapIter it = s_shaderProgramRegistry.find(hash);

	if (it != s_shaderProgramRegistry.end()) {
		return it->second;
	}
	else {
		Shader vShader = Shader("Data/Shaders/" + shaderProgName + ".vert", SHADER_VERTEX);
		Shader fShader = Shader("Data/Shaders/" + shaderProgName + ".frag", SHADER_FRAGMENT);
		ShaderProgram* nShaderProgram = new(MEMORY_RENDERING) ShaderProgram(vShader, fShader);

		s_shaderProgramRegistry.insert(ShaderProgramMapPair(hash, nShaderProgram));
		return nShaderProgram;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void ShaderProgram::DeleteShaderPrograms() {
	std::map<size_t, ShaderProgram*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, ShaderProgram*>>>::iterator it;

	for (it = s_shaderProgramRegistry.begin(); it != s_shaderProgramRegistry.end(); ++it) {
		delete it->second;
		it->second = nullptr;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void ShaderProgram::UseProgram()  const {
	GL_CHECK(glUseProgram((GLuint)m_programID));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UNIFORMS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
Uniform* ShaderProgram::GetUniform(const char* name) {

	size_t nameHash = HashCString(name);
	ShaderProgramUniformMapIter it = m_uniforms.find(nameHash);

	if (it != m_uniforms.end()) {
		return it->second;
	}
	else {
		return nullptr;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BINDING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void ShaderProgram::BindAttributes(VertexDefinition* vertexDef) const {
	BindVertexAttributes(vertexDef);
}


//---------------------------------------------------------------------------------------------------------------------------
uint ShaderProgram::BindUniforms(const std::vector<Uniform*>* matUniforms) const {

	ShaderProgramUniformMapConstIter uniformIt = m_uniforms.begin();
	uint currBindPoint = 0;

	for (uniformIt; uniformIt != m_uniforms.end(); ++uniformIt) {

		const Uniform* currUniform = uniformIt->second;
		bool shouldBind = true;

		for (uint i = 0; i < matUniforms->size(); i++) {

			if (currUniform->GetNameHash() == matUniforms->at(i)->GetNameHash()) {
				shouldBind = false;
				break;
			}
		}

		if (shouldBind) {
			currUniform->Bind();
		}
	}

	return currBindPoint;
}


//---------------------------------------------------------------------------------------------------------------------------
void ShaderProgram::BindSSBOs() const {

	for (uint i = 0; i < m_ssbos.size(); i++) {
		m_ssbos[i].second->Bind(m_ssbos[i].first);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
ShaderProgram::ShaderProgram() { }


//---------------------------------------------------------------------------------------------------------------------------
ShaderProgram::ShaderProgram(const Shader& vShader, const Shader& fShader)
	: m_vertexShader(vShader)
	, m_fragmentShader(fShader)
{
	m_vertexShader.Load();
	m_fragmentShader.Load();
	CreateAndLinkProgram();
}


//---------------------------------------------------------------------------------------------------------------------------
ShaderProgram::~ShaderProgram() {
	
	ShaderProgramUniformMapIter it = m_uniforms.begin();

	for (it; it != m_uniforms.end(); it++) {
		delete it->second;
		it->second = nullptr;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void ShaderProgram::Load(const String& vertShaderName, const String& fragShaderName) {

	m_vertexShader		= Shader(vertShaderName, SHADER_VERTEX);
	m_fragmentShader	= Shader(fragShaderName, SHADER_FRAGMENT);

	m_vertexShader.Load();
	m_fragmentShader.Load();
	CreateAndLinkProgram();
}


//---------------------------------------------------------------------------------------------------------------------------
void ShaderProgram::BindVertexAttributes(VertexDefinition* vertexDef) const {

	const VertexAttribute* attribs;
	attribs = vertexDef->GetAllAttributes();

	for (uint8_t i = 0; i < vertexDef->GetNumAttribs(); i++) {

		VertexAttribute attrib = attribs[i];
		BindProperty(attrib.name, attrib.count, GetOpenGLEnum(attrib.type), GetOpenGLEnum(attrib.normalize), (int)attrib.stride, (size_t)attrib.offset);
	}
	
}


//---------------------------------------------------------------------------------------------------------------------------
void ShaderProgram::BindProperty(const String& name, int32_t count, int32_t type, int normalize, int stride, size_t offset) const {

	GLint posBindPoint;
	GL_CHECK(posBindPoint = glGetAttribLocation(m_programID, name.c_str()));

	if (posBindPoint >= 0) {
		if (type != GL_INT) {
			GL_CHECK(glEnableVertexAttribArray(posBindPoint));
			GL_CHECK(glVertexAttribPointer(posBindPoint, count, type, (GLboolean)normalize, stride, (GLvoid*)offset));
		}
		else {
			GL_CHECK(glEnableVertexAttribArray(posBindPoint));
			GL_CHECK(glVertexAttribIPointer(posBindPoint, count, type, stride, (GLvoid*)offset));
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
int ShaderProgram::CreateAndLinkProgram() {

	GL_CHECK(m_programID = glCreateProgram());

	int vertexShaderID = m_vertexShader.GetShaderID();
	int fragmentVertexShaderID = m_fragmentShader.GetShaderID();
	String vertFilePath = m_vertexShader.GetFileName();
	String fragFilePath = m_fragmentShader.GetFileName();

	if (m_programID == NULL) {
		SystemDialogue_Okay("Shader Problem", "Couldn't Create Program", SEVERITY_WARNING);
		return -1;
	}
	else {
		GL_CHECK(glAttachShader(m_programID, vertexShaderID));
		GL_CHECK(glAttachShader(m_programID, fragmentVertexShaderID));

		GL_CHECK(glLinkProgram(m_programID));

		GLint status;
		GL_CHECK(glGetProgramiv(m_programID, GL_LINK_STATUS, &status));

		if (status == GL_FALSE) {
			GLint log_length;
			GL_CHECK(glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &log_length));

			char* buffer = new(MEMORY_RENDERING) char[log_length + 1];
			GL_CHECK(glGetProgramInfoLog(m_programID, log_length, &log_length, buffer));

			buffer[log_length] = NULL;
			ShaderUtils::PrintShaderCompileErrorToOutput(buffer, vertFilePath);
			delete buffer;
			GL_CHECK(glDeleteProgram(m_programID));

			return -1;
		}
		else {
			GL_CHECK(glDetachShader(m_programID, vertexShaderID));
			GL_CHECK(glDetachShader(m_programID, fragmentVertexShaderID));

			return m_programID;
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void ShaderProgram::CreateUniform(const char* name, eUniformType type, size_t count, void* dataArr) {

	ASSERT_OR_DIE(!Uniform::NeedsBindPoint(type), "ERROR: Texture uniform must specify bind point.");
	CreateUniform(name, type, count, -1, dataArr);
}


//---------------------------------------------------------------------------------------------------------------------------
void ShaderProgram::CreateUniform(const char* name, eUniformType type, size_t count, int bindPoint, void* dataArr) {

	GLint shaderLoc = INVALID_SHADER_LOC;

	if (count == 1) {
		GL_CHECK(shaderLoc = glGetUniformLocation((GLuint)m_programID, name));
	}
	else {
		String arrName = StringUtils::Stringf("%s[0]", name);
		GL_CHECK(shaderLoc = glGetUniformLocation((GLuint)m_programID, arrName.c_str()));
	}

	Uniform* nUniform = new(MEMORY_RENDERING) Uniform(name, count, type, shaderLoc, bindPoint, dataArr);
	m_uniforms.insert(ShaderProgramUniformMapPair(nUniform->GetNameHash(), nUniform));
}

#endif