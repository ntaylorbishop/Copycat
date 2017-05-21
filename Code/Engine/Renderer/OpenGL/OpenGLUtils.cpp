#include "Engine/Renderer/OpenGL/OpenGLUtils.hpp"

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/OpenGL/OpenGLExtensions.hpp"
#include "Engine/Utils/StringUtils.hpp"
#include "Engine/Renderer/OpenGL/OpenGLErrorReporter.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DEBUG
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void CheckOpenGLError(const char* stmt, const char* fname, int line, bool breakOnOPGLErrors) {

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		String errorCode = "UNKNOWN";
		switch (err) {
		case GL_NO_ERROR:
			errorCode = "GL_NO_ERROR";
			break;
		case GL_INVALID_ENUM:
			errorCode = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			errorCode = "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			errorCode = "GL_INVALID_OPERATION";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errorCode = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		case GL_OUT_OF_MEMORY:
			errorCode = "GL_OUT_OF_MEMORY";
			break;
		case GL_STACK_UNDERFLOW:
			errorCode = "GL_STACK_UNDERFLOW";
			break;
		case GL_STACK_OVERFLOW:
			errorCode = "GL_STACK_OVERFLOW";
			break;
		}

		String output = StringUtils::Stringf("OpenGL error %s:\n %s(%i)\n %s\n", errorCode.c_str(), fname, line, stmt);


		if (breakOnOPGLErrors) {

			//std::vector<String> debugMsgLog = OpenGLErrorReporter::FetchDebugMessages();
			//ERROR_AND_DIE(output);
		}
		else {
			DebuggerPrintf(output.c_str());
		}
	}
}