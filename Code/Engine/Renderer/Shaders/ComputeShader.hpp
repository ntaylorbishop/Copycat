#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

class Texture;

class ComputeShader {
public:
	ComputeShader();
	ComputeShader(const String& name);
	~ComputeShader() { }

	void Load();

	uint GetShaderID() const { return m_shaderID; }
	String GetFileName() const { return m_filename; }
private:
	String m_filename;
	uint m_shaderID;
};