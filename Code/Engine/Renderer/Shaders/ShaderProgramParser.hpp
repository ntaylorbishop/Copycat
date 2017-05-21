#pragma once

class ShaderProgram;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CONSTANTS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
const char* const SPROG_PARSER_SHADERPROG			= "ShaderProgram";
const char* const SPROG_PARSER_VERTSHADER			= "VertexShader";
const char* const SPROG_PARSER_FRAGSHADER			= "FragmentShader";
const char* const SPROG_PARSER_UNIFORM				= "Uniform";


const char* const SPROG_PARSER_DEFAULT_PROJ			= "defaultProj";
const char* const SPROG_PARSER_DEFAULT_UI_PROJ		= "defaultProjUI";
const char* const SPROG_PARSER_CAMERA_POS			= "cameraPos";
const char* const SPROG_PARSER_DEFAULT_VIEW			= "defaultView";
const char* const SPROG_PARSER_SCREEN_SIZE_X		= "screenSizeX";
const char* const SPROG_PARSER_SCREEN_SIZE_Y		= "screenSizeY";
const char* const SPROG_PARSER_SCREEN_SIZE			= "screenSize";
const char* const SPROG_PARSER_DEFAULT_NEAR			= "defaultNear";
const char* const SPROG_PARSER_DEFAULT_FAR			= "defaultFar";
const char* const SPROG_PARSER_BLANK				= "blank";
const char* const SPROG_PARSER_NULL					= "null";
const char* const SPROG_PARSER_DEFAULT_DEPTH		= "defaultDepth";
const char* const SPROG_PARSER_DEFAULT_COLOR_TARGET	= "defaultColorTarget";
const char* const SPROG_PARSER_TILESIZEPIXELS		= "tileSizePixels";
const char* const SPROG_PARSER_MAXLIGHTSPERTILE		= "maxLightsPerTile";
const char* const SPROG_PARSER_DEFAULT_NORMALS		= "defaultNormals";


const char* const SPROG_PARSER_UNI_NAME				= "name";
const char* const SPROG_PARSER_UNI_TYPE				= "type";
const char* const SPROG_PARSER_UNI_BIND_POINT		= "bindPoint";
const char* const SPROG_PARSER_UNI_DEFAULT_VAL		= "default";

const char* const SPROG_PARSER_FALSE				= "false";
const char* const SPROG_PARSER_IDENTITY				= "identity";
const char* const SPROG_PARSER_WHITE				= "white";
const char* const SPROG_PARSER_TRUE					= "true";


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SHADER PROGRAM PARSER CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
class ShaderProgramParser {
public:
	static ShaderProgram** ParseShaderProgramsFromXML(const char* dir, const char* filename);
};