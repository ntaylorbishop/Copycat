#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/gl.h>
#include <gl/gl.h>
#include "ThirdParty/glext.h"
#include "ThirdParty/wglext.h"

extern PFNGLGENBUFFERSPROC					glGenBuffers;
extern PFNGLBINDBUFFERPROC					glBindBuffer;
extern PFNGLBUFFERDATAPROC					glBufferData;
extern PFNGLGENERATEMIPMAPPROC				glGenerateMipmap;
extern PFNGLDELETEBUFFERSPROC				glDeleteBuffers;
extern PFNGLCREATESHADERPROC				glCreateShader;
extern PFNGLSHADERSOURCEPROC				glShaderSource;
extern PFNGLCOMPILESHADERPROC				glCompileShader;
extern PFNGLGETSHADERIVPROC					glGetShaderiv;
extern PFNGLDELETESHADERPROC				glDeleteShader;
extern PFNGLCREATEPROGRAMPROC				glCreateProgram;
extern PFNGLATTACHSHADERPROC				glAttachShader;
extern PFNGLLINKPROGRAMPROC					glLinkProgram;
extern PFNGLGETPROGRAMIVPROC				glGetProgramiv;
extern PFNGLDETACHSHADERPROC				glDetachShader;
extern PFNGLDELETEPROGRAMPROC				glDeleteProgram;
extern PFNGLACTIVETEXTUREPROC				glActiveTexture;
extern PFNGLGETSHADERINFOLOGPROC			glGetShaderInfoLog;
extern PFNGLGETPROGRAMINFOLOGPROC			glGetProgramInfoLog;
extern PFNGLGENVERTEXARRAYSPROC				glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC			glDeleteVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC				glBindVertexArray;
extern PFNGLGETATTRIBLOCATIONPROC			glGetAttribLocation;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC		glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC			glVertexAttribPointer;
extern PFNGLVERTEXATTRIBIPOINTERPROC		glVertexAttribIPointer;
extern PFNGLUSEPROGRAMPROC					glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation;
extern PFNGLUNIFORM1FVPROC					glUniform1fv;
extern PFNGLUNIFORM2FVPROC					glUniform2fv;
extern PFNGLUNIFORM3FVPROC					glUniform3fv;
extern PFNGLUNIFORM4FVPROC					glUniform4fv;
extern PFNGLUNIFORM1IPROC					glUniform1i;
extern PFNGLUNIFORM1IVPROC					glUniform1iv;
extern PFNGLUNIFORM2IVPROC					glUniform2iv;
extern PFNGLUNIFORM3IVPROC					glUniform3iv;
extern PFNGLUNIFORM4IVPROC					glUniform4iv;
extern PFNGLUNIFORM1UIPROC					glUniform1ui;
extern PFNGLUNIFORM1UIVPROC					glUniform1uiv;
extern PFNGLUNIFORM2UIVPROC					glUniform2uiv;
extern PFNGLUNIFORM3UIVPROC					glUniform3uiv;
extern PFNGLUNIFORM4UIVPROC					glUniform4uiv;
extern PFNGLBINDSAMPLERPROC					glBindSampler;
extern PFNGLGENSAMPLERSPROC					glGenSamplers;
extern PFNGLSAMPLERPARAMETERIPROC			glSamplerParameteri;
extern PFNGLDELETESAMPLERSPROC				glDeleteSamplers;
extern PFNGLUNIFORMMATRIX4FVPROC			glUniformMatrix4fv;
extern PFNGLGENFRAMEBUFFERSPROC				glGenFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC				glBindFramebuffer;
extern PFNGLFRAMEBUFFERTEXTUREPROC			glFramebufferTexture;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC		glCheckFramebufferStatus;
extern PFNGLDRAWBUFFERSPROC					glDrawBuffers;
extern PFNGLDELETEFRAMEBUFFERSPROC			glDeleteFramebuffers;
extern PFNGLBLITFRAMEBUFFERPROC				glBlitFramebuffer;								
extern PFNGLGETPROGRAMRESOURCEIVPROC		glGetProgramResourceiv;
extern PFNGLGETPROGRAMRESOURCENAMEPROC		glGetProgramResourceName;
extern PFNGLGETACTIVEUNIFORMPROC			glGetActiveUniform;							
extern PFNGLTEXSTORAGE2DPROC				glTexStorage2D;
extern PFNGLTEXIMAGE2DMULTISAMPLEPROC		glTexImage2DMultisample;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC		glFramebufferTexture2D;
extern PFNGLSAMPLERPARAMETERFPROC			glSamplerParameterf;
extern PFNGLDISPATCHCOMPUTEPROC				glDispatchCompute;
extern PFNGLDISPATCHCOMPUTEINDIRECTPROC		glDispatchComputeIndirect;
extern PFNGLBINDIMAGETEXTUREPROC			glBindImageTexture;
extern PFNGLMEMORYBARRIERPROC				glMemoryBarrier;
extern PFNGLMAPBUFFERPROC					glMapBuffer;
extern PFNGLUNMAPBUFFERPROC					glUnmapBuffer;
extern PFNGLBINDBUFFERBASEPROC				glBindBufferBase;
extern PFNGLGETINTEGER64VPROC				glGetInteger64v;
extern PFNGLGENQUERIESPROC					glGenQueries;
extern PFNGLQUERYCOUNTERPROC				glQueryCounter;
extern PFNGLGETQUERYOBJECTIVPROC			glGetQueryObjectiv;
extern PFNGLGETQUERYOBJECTUI64VPROC			glGetQueryObjectui64v;
extern PFNGLBUFFERSUBDATAPROC				glBufferSubData;
extern PFNGLDEBUGMESSAGECONTROLPROC			glDebugMessageControl;
extern PFNGLGETDEBUGMESSAGELOGPROC			glGetDebugMessageLog;
extern PFNGLPUSHDEBUGGROUPPROC				glPushDebugGroup;
extern PFNGLDEBUGMESSAGECALLBACKPROC		glDebugMessageCallback;
extern PFNGLENABLEIPROC						glEnablei;
extern PFNGLDISABLEIPROC					glDisablei;
extern PFNGLBLENDFUNCIPROC					glBlendFunci;
extern PFNGLCLEARBUFFERFVPROC				glClearBufferfv;
