#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/gl.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DEBUG
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CheckOpenGLError(const char* stmt, const char* fname, int line, bool breakOnOGLErrors);

#if defined(_DEBUG) && defined(DEBUG_OPENGL)
#define GL_CHECK(stmt) do { \
			stmt;																	\
			__pragma(warning(push))													\
			__pragma(warning(disable:4127))											\
			CheckOpenGLError(#stmt, __FILE__, __LINE__, BREAK_ON_OPENGL_ERRORS);	\
		} while (0)																	\
		__pragma(warning(pop))	
#else
#define GL_CHECK(stmt) stmt
#endif