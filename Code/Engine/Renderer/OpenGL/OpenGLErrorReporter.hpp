#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/OpenGL/OpenGLUtils.hpp"
#include "Engine/Renderer/OpenGL/OpenGLExtensions.hpp"


class OpenGLErrorReporter {
public:
	//INIT DESTROY
	static void Initialize();
	static void Shutdown();

	//static std::vector<String> FetchDebugMessages();

	static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length,
		const GLchar* message, GLvoid* userParam);

private:



	static OpenGLErrorReporter* s_openglErrorReporter;
};