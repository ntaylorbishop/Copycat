#include "Engine/Renderer/OpenGL/OpenGLEnums.hpp"

int GetOpenGLEnum(eRenderEnum glEnum) {
	switch (glEnum) {
		case ENUM_FLOAT:
			return GL_FLOAT;
		case ENUM_DOUBLE:
			return GL_DOUBLE;
		case ENUM_INT:
			return GL_INT;
		case ENUM_SHORT:
			return GL_SHORT;
		case ENUM_TRUE:
			return GL_TRUE;
		case ENUM_FALSE:
			return GL_FALSE;
		case ENUM_NAME_LENGTH:
			return GL_NAME_LENGTH;
		case ENUM_TYPE:
			return GL_TYPE;
		case ENUM_ARRAY_SIZE:
			return GL_ARRAY_SIZE;
		case ENUM_PROGRAM_INPUT:
			return GL_PROGRAM_INPUT;
		case ENUM_NEAREST:
			return GL_NEAREST;
		case ENUM_REPEAT:
			return GL_REPEAT;
		case ENUM_FLOAT_VEC2:
			return GL_FLOAT_VEC2;
		case ENUM_FLOAT_VEC3:
			return GL_FLOAT_VEC3;
		case ENUM_FLOAT_VEC4:
			return GL_FLOAT_VEC4;
		case ENUM_DOUBLE_VEC2:
			return GL_DOUBLE_VEC2;
		case ENUM_DOUBLE_VEC3:
			return GL_DOUBLE_VEC3;
		case ENUM_DOUBLE_VEC4:
			return GL_DOUBLE_VEC4;
		case ENUM_INT_VEC2:
			return GL_INT_VEC2;
		case ENUM_INT_VEC3:
			return GL_INT_VEC3;
		case ENUM_INT_VEC4:
			return GL_INT_VEC4;
		case ENUM_UNSIGNED_INT:
			return GL_UNSIGNED_INT;
		case ENUM_UNSIGNED_INT_VEC2:
			return GL_UNSIGNED_INT_VEC2;
		case ENUM_UNSIGNED_INT_VEC3:
			return GL_UNSIGNED_INT_VEC3;
		case ENUM_UNSIGNED_INT_VEC4:
			return GL_UNSIGNED_INT_VEC4;
		case ENUM_BOOL:
			return GL_BOOL;
		case ENUM_BOOL_VEC2:
			return GL_BOOL_VEC2;
		case ENUM_BOOL_VEC3:
			return GL_BOOL_VEC3;
		case ENUM_BOOL_VEC4:
			return GL_BOOL_VEC4;
		case ENUM_FLOAT_MAT2:
			return GL_FLOAT_MAT2;
		case ENUM_FLOAT_MAT3:
			return GL_FLOAT_MAT3;
		case ENUM_FLOAT_MAT4:
			return GL_FLOAT_MAT4;
		case ENUM_DOUBLE_MAT2:
			return GL_DOUBLE_MAT2;
		case ENUM_DOUBLE_MAT3:
			return GL_DOUBLE_MAT3;
		case ENUM_DOUBLE_MAT4:
			return GL_DOUBLE_MAT4;
		case ENUM_SAMPLER_1D:
			return GL_SAMPLER_1D;
		case ENUM_SAMPLER_2D:
			return GL_SAMPLER_2D;
		case ENUM_SAMPLER_3D:
			return GL_SAMPLER_3D;
		case ENUM_SAMPLER_CUBE:
			return GL_SAMPLER_CUBE;
		default:
			return 0;
	}
}

eRenderEnum GetRenderEnumForOpenGLEnum(GLenum glEnum) {
	switch (glEnum) {
		case GL_FLOAT:
			return ENUM_FLOAT;
		case GL_DOUBLE:
			return ENUM_DOUBLE;
		case GL_INT:
			return ENUM_INT;
		case GL_SHORT:
			return ENUM_SHORT;
		case GL_TRUE:
			return ENUM_TRUE;
		case GL_FALSE:
			return ENUM_FALSE;
		case GL_NAME_LENGTH:
			return ENUM_NAME_LENGTH;
		case GL_TYPE:
			return ENUM_TYPE;
		case GL_ARRAY_SIZE:
			return ENUM_ARRAY_SIZE;
		case GL_PROGRAM_INPUT:
			return ENUM_PROGRAM_INPUT;
		case GL_NEAREST:
			return ENUM_NEAREST;
		case GL_REPEAT:
			return ENUM_REPEAT;
		case GL_FLOAT_VEC2:
			return ENUM_FLOAT_VEC2;
		case GL_FLOAT_VEC3:
			return ENUM_FLOAT_VEC3;
		case GL_FLOAT_VEC4:
			return ENUM_FLOAT_VEC4;
		case GL_DOUBLE_VEC2:
			return ENUM_DOUBLE_VEC2;
		case GL_DOUBLE_VEC3:
			return ENUM_DOUBLE_VEC3;
		case GL_DOUBLE_VEC4:
			return ENUM_DOUBLE_VEC4;
		case GL_INT_VEC2:
			return ENUM_INT_VEC2;
		case GL_INT_VEC3:
			return ENUM_INT_VEC3;
		case GL_INT_VEC4:
			return ENUM_INT_VEC4;
		case GL_UNSIGNED_INT:
			return ENUM_UNSIGNED_INT;
		case GL_UNSIGNED_INT_VEC2:
			return ENUM_UNSIGNED_INT_VEC2;
		case GL_UNSIGNED_INT_VEC3:
			return ENUM_UNSIGNED_INT_VEC3;
		case GL_UNSIGNED_INT_VEC4:
			return ENUM_UNSIGNED_INT_VEC4;
		case GL_BOOL:
			return ENUM_BOOL;
		case GL_BOOL_VEC2:
			return ENUM_BOOL_VEC2;
		case GL_BOOL_VEC3:
			return ENUM_BOOL_VEC3;
		case GL_BOOL_VEC4:
			return ENUM_BOOL_VEC4;
		case GL_FLOAT_MAT2:
			return ENUM_FLOAT_MAT2;
		case GL_FLOAT_MAT3:
			return ENUM_FLOAT_MAT3;
		case GL_FLOAT_MAT4:
			return ENUM_FLOAT_MAT4;
		case GL_DOUBLE_MAT2:
			return ENUM_DOUBLE_MAT2;
		case GL_DOUBLE_MAT3:
			return ENUM_DOUBLE_MAT3;
		case GL_DOUBLE_MAT4:
			return ENUM_DOUBLE_MAT4;
		case GL_SAMPLER_1D:
			return ENUM_SAMPLER_1D;
		case GL_SAMPLER_2D:
			return ENUM_SAMPLER_2D;
		case GL_SAMPLER_3D:
			return ENUM_SAMPLER_3D;
		case GL_SAMPLER_CUBE:
			return ENUM_SAMPLER_CUBE;
		default:
			return ENUM_FLOAT;
	}
}