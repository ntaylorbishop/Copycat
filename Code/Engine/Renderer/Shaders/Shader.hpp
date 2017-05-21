#pragma once

#include <string>
#include "Engine/Config/BuildConfig.hpp"
#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Utils/FileUtils/FileUtils.hpp"

enum eShaderType;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SHADER CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Shader {
public:
	//STRUCTORS
	Shader();
	Shader(const String& fileName, eShaderType type);
	~Shader();

	//LOADING
	void Load();
	void Delete();

	//GETTERS SETTERS
	String GetFileName() { return m_fileName; }
	eShaderType GetShaderType() { return m_type; }
	uint GetShaderID() { return m_shaderID; }

private:
	int GetShaderTypeEnum();

private:
	uint m_shaderID;
	String m_fileName;
	eShaderType m_type;
};