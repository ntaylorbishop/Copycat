#include "Engine/Renderer/Shaders/ShaderProgramParser.hpp"
#include "Engine/Renderer/Shaders/ShaderProgram.hpp"
#include "Engine/Renderer/Material/Material.hpp"
#include "Engine/Renderer/Material/MaterialBank.hpp"
#include "Engine/General/Logger/TheLogger.hpp"
#include "Engine/Utils/GeneralUtils/XMLUtils.hpp"
#include "ThirdParty/Parsers/xmlParser.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PARSING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
static uint GetNumShaderProgsInFile(const XMLNode& root, const char* filename) {

	uint numShaderPrograms = (uint)root.nChildNode();

	if (numShaderPrograms == 0) {
		TheLogger::LogPrintf("No shader programs to parse in file %s", "ShaderProgram", LOG_DEFAULT, filename);
		return 0;
	}
	else {
		return numShaderPrograms;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
static void InitShaderProgramWithShaderNames(const char* dir, ShaderProgram* newProg, const XMLNode& progNode) {

	String vertShaderName = "NULL";
	String fragShaderName = "NULL";

	String shaderProgName = progNode.getAttribute(0).lpszValue;
	newProg->SetName(shaderProgName);

	for (int childIdx = 0; childIdx < progNode.nChildNode(); childIdx++) {

		XMLNode childNode = progNode.getChildNode(childIdx);

		if (strcmp(childNode.getName(), SPROG_PARSER_VERTSHADER) == 0) {
			vertShaderName = String(dir) + String(childNode.getAttribute(0).lpszValue) + String(".vert");
		}
		else if (strcmp(childNode.getName(), SPROG_PARSER_FRAGSHADER) == 0) {
			fragShaderName = String(dir) + String(childNode.getAttribute(0).lpszValue) + String(".frag");
		}
	}

	ASSERT_OR_DIE(vertShaderName != "NULL" && fragShaderName != "NULL", "ERROR: ShaderProgram XML must define vert / frag shaders.");

	newProg->Load(vertShaderName, fragShaderName);
}


//---------------------------------------------------------------------------------------------------------------------------
static void* GetDefaultValue(eUniformType uniType, const char* defaultValStr) {

	if (strcmp(defaultValStr, SPROG_PARSER_DEFAULT_PROJ) == 0) {
		return (void*)BeirusRenderer::GetProjectionMatrix();
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_DEFAULT_UI_PROJ) == 0) {
		return (void*)BeirusRenderer::GetDefaultUIProjectMatrix();
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_CAMERA_POS) == 0) {
		return (void*)&BeirusRenderer::GetPlayerCamera()->m_position;
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_DEFAULT_VIEW) == 0) {
		return (void*)BeirusRenderer::GetViewMatrix();
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_SCREEN_SIZE_X) == 0) {
		return (void*)&SCREEN_SIZE_INT.x;
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_SCREEN_SIZE_Y) == 0) {
		return (void*)&SCREEN_SIZE_INT.y;
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_SCREEN_SIZE) == 0) {
		return (void*)&SCREEN_SIZE;
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_DEFAULT_NEAR) == 0) {
		return (void*)&g_nearZ;
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_DEFAULT_FAR) == 0) {
		return (void*)&g_farZ;
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_BLANK) == 0) {
		return (void*)&g_farZ;
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_NULL) == 0) {
		return nullptr;
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_DEFAULT_DEPTH) == 0) {
		return BeirusRenderer::GetGlobalDepthMap();
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_DEFAULT_COLOR_TARGET) == 0) {
		return BeirusRenderer::GetDefaultColorTarget();
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_FALSE) == 0) {
		return (void*)&BOOL_FALSE;
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_IDENTITY) == 0) {
		return (void*)&Matrix4::IDENTITY;
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_WHITE) == 0) {
		return (void*)&RGBA::WHITE;
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_TRUE) == 0) {
		return (void*)&BOOL_TRUE;
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_TILESIZEPIXELS) == 0) {
		return (void*)&TILE_SIZE_PIXELS;
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_MAXLIGHTSPERTILE) == 0) {
		return (void*)&MAX_NUM_LIGHTS_PER_TILE;
	}
	else if (strcmp(defaultValStr, SPROG_PARSER_DEFAULT_NORMALS) == 0) {
		return BeirusRenderer::GetGlobalNormalsMap();
	}
	else {
		switch (uniType) {
		case UNIFORM_FLOAT: {
			return new float(XMLUtils::ParseFloat(defaultValStr));
		}
		default:
			return nullptr;
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseUniform(ShaderProgram* newProg, const XMLNode& uniNode) {

	const char* uniName		= "NULL";
	eUniformType uniType	= UNIFORM_NUM_UNIFORM_TYPES;
	int bindPoint			= -1;
	void* defaultVal		= nullptr;

	for (int uniAttribIdx = 0; uniAttribIdx < uniNode.nAttribute(); uniAttribIdx++) {

		XMLAttribute currAttrib	= uniNode.getAttribute(uniAttribIdx);
		const char* attribName	= currAttrib.lpszName;
		const char* attribVal	= currAttrib.lpszValue;

		if (strcmp(attribName, SPROG_PARSER_UNI_NAME) == 0) {
			uniName = attribVal;
		}
		else if (strcmp(attribName, SPROG_PARSER_UNI_TYPE) == 0) {
			uniType = Uniform::UnserializeType(attribVal);
		}
		else if (strcmp(attribName, SPROG_PARSER_UNI_BIND_POINT) == 0) {
			bindPoint = (uint)XMLUtils::ParseUint(attribVal);
		}
		else if (strcmp(attribName, SPROG_PARSER_UNI_DEFAULT_VAL) == 0) {
			defaultVal = GetDefaultValue(uniType, attribVal);
		}
	}

	char name[MAX_UNIFORM_NAME_SIZE];
	strcpy_s(name, uniName);
	newProg->CreateUniform(name, uniType, 1, bindPoint, defaultVal);
}

//---------------------------------------------------------------------------------------------------------------------------
static void ParseUniforms(ShaderProgram* newProg, const XMLNode& vertOrFragNode) {

	for (int uniNodeIdx = 0; uniNodeIdx < vertOrFragNode.nChildNode(); uniNodeIdx++) {

		XMLNode uniNode = vertOrFragNode.getChildNode(uniNodeIdx);
		ParseUniform(newProg, uniNode);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseShaderProgram(const char* dir, ShaderProgram* newProg, const XMLNode& progNode) {

	InitShaderProgramWithShaderNames(dir, newProg, progNode);

	for (int childIdx = 0; childIdx < progNode.nChildNode(); childIdx++) {

		XMLNode childNode = progNode.getChildNode(childIdx);

		if (strcmp(childNode.getName(), SPROG_PARSER_VERTSHADER) == 0) {
			ParseUniforms(newProg, childNode);
		}
		else if (strcmp(childNode.getName(), SPROG_PARSER_FRAGSHADER) == 0) {
			ParseUniforms(newProg, childNode);
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC ShaderProgram** ShaderProgramParser::ParseShaderProgramsFromXML(const char* dir, const char* filename) {

	XMLNode root	= XMLNode::parseFile(filename);
	root			= root.getChildNode(0);

	uint numShaderPrograms		= GetNumShaderProgsInFile(root, filename);
	ShaderProgram** parsedProgs	= new ShaderProgram*[numShaderPrograms];


	for (uint progIdx = 0; progIdx < numShaderPrograms; progIdx++) {

		XMLNode progNode		= root.getChildNode(progIdx);
		parsedProgs[progIdx]	= new ShaderProgram();
		ShaderProgram* newProg	= parsedProgs[progIdx];
		ParseShaderProgram(dir, newProg, progNode);
		String progName			= newProg->GetName();
		size_t hash				= std::hash<String>()(progName);
		ShaderProgram::s_shaderProgramRegistry.insert(ShaderProgramMapPair(hash, newProg));
		Material* defaultMat	= new Material(progName);
		MaterialBank::AddMaterial(defaultMat);
	}

	return nullptr;
}