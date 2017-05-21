#include "Engine/Renderer/OpenGL/OpenGLErrorReporter.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"

STATIC OpenGLErrorReporter* OpenGLErrorReporter::s_openglErrorReporter = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INIT DESTROY
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
//INIT DESTROY
STATIC void OpenGLErrorReporter::Initialize() {

	#if defined(_DEBUG)

	ASSERT_OR_DIE(s_openglErrorReporter == nullptr, "ERROR: OGL Error Reporter already initialized.");
	s_openglErrorReporter = new OpenGLErrorReporter();

	GL_CHECK(glEnable(GL_DEBUG_OUTPUT));
	GL_CHECK(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
	GL_CHECK(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE));
	GL_CHECK(glDebugMessageCallback(&OpenGLErrorReporter::DebugCallback, nullptr));
	#endif
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void OpenGLErrorReporter::Shutdown() {

	#if defined(_DEBUG)

	ASSERT_OR_DIE(s_openglErrorReporter != nullptr, "ERROR: OGL Error Reporter already initialized.");
	delete s_openglErrorReporter;
	s_openglErrorReporter = nullptr;

	GL_CHECK(glDisable(GL_DEBUG_OUTPUT));
	GL_CHECK(glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS));

	#endif

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
/*
STATIC std::vector<String> OpenGLErrorReporter::FetchDebugMessages() {

	GLuint noIDs = 0;
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, &noIDs, GL_FALSE);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, &noIDs, GL_FALSE);

	GLint numMsgs = 0;
	GLint maxMsgLen = 0;
	GL_CHECK(glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &maxMsgLen));
	GL_CHECK(glGetIntegerv(GL_MAX_DEBUG_LOGGED_MESSAGES, &numMsgs));

	std::vector<GLchar> msgData(numMsgs * maxMsgLen);
	std::vector<GLenum> sources(numMsgs);
	std::vector<GLenum> types(numMsgs);
	std::vector<GLenum> severities(numMsgs);
	std::vector<GLuint> ids(numMsgs);
	std::vector<GLsizei> lengths(numMsgs);

	GLuint numFound;
	GL_CHECK(numFound = glGetDebugMessageLog(numMsgs, msgData.size(), &sources[0], &types[0], &ids[0], &severities[0], &lengths[0], &msgData[0]));

	sources.resize(numFound);
	types.resize(numFound);
	severities.resize(numFound);
	ids.resize(numFound);
	lengths.resize(numFound);

	std::vector<std::string> messages;
	messages.reserve(numFound);

	std::vector<GLchar>::iterator currPos = msgData.begin();
	for (size_t msg = 0; msg < lengths.size(); ++msg) {

		messages.push_back(std::string(currPos, currPos + lengths[msg] - 1));
		currPos = currPos + lengths[msg];
	}

	return messages;
}*/



STATIC void APIENTRY OpenGLErrorReporter::DebugCallback(GLenum source, GLenum type, GLuint id,
														GLenum severity, GLsizei length,
														const GLchar* message, GLvoid* userParam)
{

	return;

	(void)source; (void)type; (void)id;
	(void)severity; (void)length; (void)userParam;
	DebuggerPrintf("%s\n", message);

	if (severity == GL_DEBUG_SEVERITY_HIGH) {
		ERROR_AND_DIE("High severity OpenGL error encountered! Aborting.");
	}
}