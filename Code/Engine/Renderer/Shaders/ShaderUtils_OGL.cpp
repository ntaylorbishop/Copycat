#include "Engine/Config/BuildConfig.hpp"

#ifdef GRAPHICS_API_OGL
#include "Engine/Renderer/Shaders/ShaderUtils.hpp"
#include "Engine/Utils/StringUtils.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/General/Core/TheMemoryManager.hpp"


void ShaderUtils::PrintShaderCompileErrorToOutput(const String& error, const String& fileNameOfShader) {

	//TODO: FIX ABS PATH
	String solDir = "d:\\guildhall\\p4\\c25\\students\\nbishop\\projects\\thesis\\artifact\\run_x64\\" + fileNameOfShader;
	std::transform(solDir.begin(), solDir.end(), solDir.begin(), ::tolower);

	String glVersion = BeirusRenderer::GetCurrentVersionOfGLSL();
	String glslVersion = BeirusRenderer::GetCurrentVersionOfOpenGL();

	String err = error;

	for (unsigned int i = 0; i < solDir.size(); i++) {
		if (solDir[i] == '/')
			solDir[i] = '\\';
	}
	StringUtils::ReplaceAll(err, "0(", solDir + "("); // replace all 'x' to 'y'

	String lineNumber = "";
	String firstError;
	for (unsigned int i = 0; i < err.size(); i++) {
		if (err[i] == '(') {
			int idxOfNum = i + 1;
			while (err[idxOfNum] != ')') {
				lineNumber.push_back(err[idxOfNum]);
				idxOfNum++;
			}

			while (err[idxOfNum] != '\n') {
				firstError.push_back(err[idxOfNum]);
				idxOfNum++;
			}

			break;
		}
	}

	SystemDialogue_Okay("COMPILE SHADER ERROR",
		"OpenGL Version: " + glVersion + "\n" +
		"GLSL Version: " + glslVersion + "\n\n" +
		"ERROR AT LINE NUMBER (" + lineNumber + ")" + " IN FILE (" + fileNameOfShader + firstError + "\n\n" +
		err, SEVERITY_FATAL);

	DebuggerPrintf(glVersion.c_str());
	DebuggerPrintf("\n");
	DebuggerPrintf(glslVersion.c_str());
	DebuggerPrintf("\n");
	DebuggerPrintf(err.c_str());
	InputSystem::Quitting(true);
}


//---------------------------------------------------------------------------------------------------------------------------
void ShaderUtils::CreateAndLoadShader(uint& outShaderID, eShaderType shaderType, const String& filename) {

	std::vector<unsigned char> charBuffer;

	if (FileUtils::LoadBinaryFileToBuffer(filename, charBuffer)) {
		charBuffer.push_back('\0'); //Make sure garbage doesn't get pushed in to the buffer after reading the file

		GLuint shaderID = 0;
		GL_CHECK(shaderID = glCreateShader(GetShaderTypeEnum(shaderType)));
		GLint src_length = (GLint)charBuffer.size();
		GLchar* arr = (GLchar*)&charBuffer[0];

		GL_CHECK(glShaderSource(shaderID, 1, &arr, &src_length));
		GL_CHECK(glCompileShader(shaderID));

		//Check for errors
		GLint status;
		GL_CHECK(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status));
		if (status == GL_FALSE) {

			GLint length;
			GL_CHECK(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length));

			char* buffer = new(MEMORY_RENDERING) char[length + 1];
			GL_CHECK(glGetShaderInfoLog(shaderID, length, &length, buffer));

			buffer[length] = NULL;
			ShaderUtils::PrintShaderCompileErrorToOutput(buffer, filename);
			delete buffer;

			GL_CHECK(glDeleteShader(shaderID));

			outShaderID = 0;
		}
		else {
			outShaderID = shaderID;
		}

	}
	else {
		String output = StringUtils::Stringf("Can't load shader %s", filename.c_str());
		SystemDialogue_Okay("Shader Problem", output, SEVERITY_WARNING);

		outShaderID = 0;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
int ShaderUtils::GetShaderTypeEnum(eShaderType type) {
	if (type == SHADER_VERTEX) {
		return GL_VERTEX_SHADER;
	}
	else if (type == SHADER_FRAGMENT) {
		return GL_FRAGMENT_SHADER;
	}
	else if (type == SHADER_COMPUTE) {
		return GL_COMPUTE_SHADER;
	}
	else {
		return GL_VERTEX_SHADER;
	}
}
#endif