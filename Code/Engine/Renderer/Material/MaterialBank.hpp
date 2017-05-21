#pragma once

#include <fstream>
#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Utils/GeneralUtils.hpp"
#include "Engine/Renderer/Material/Material.hpp"
#include "ThirdParty/Parsers/xmlParser.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INITIAL DEFINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const RGBA	MATERIAL_AMBIENT_LIGHT				= RGBA(1.f, 1.f, 1.f, 1.f);
extern float MATERIAL_SPEC_POWER;
const int	DEFAULT_VIEW_MODE					= 0;

const char* const DEPTH_TEST_PARAM_ON			= "true";
const char* const DEPTH_TEST_PARAM_OFF			= "false";
const char* const DEPTH_TEST_PARAM_DUAL			= "dual";

const char* const BLEND_MODE_PARAM_OPAQUE		= "opaque";
const char* const BLEND_MODE_PARAM_TRANSLUCENT	= "translucent";
const char* const BLEND_MODE_PARAM_ADDITIVE		= "additive";

const char* const BACKFACE_NOCULL				= "noCull";
const char* const BACKFACE_CULL					= "cull";

const char* const LIGHTING_MODEL_PARAM_PHONG	= "phong";
const char* const LIGHTING_MODEL_PARAM_BRDF		= "brdf";
const char* const LIGHTING_MODEL_PARAM_UNLIT	= "unlit";


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MATERIAL BANK
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MaterialBank {
public:
	static void Initialize();
	static void Destroy();

	//INTERFACE
	static bool AddMaterialsFromMTLFile(const char* filenameAndPath);
	static bool AddMaterialsFromXMLFile(const char* dir, const char* filename);
	static bool AddGeneratedMaterial(Material* mat);
	static Material* GetMaterial(const String& name);
	static bool InsertMaterial(Material* mat);
	static Material* InitializeFXAAPostProgram(TextureBuffer* colorTarget, const Vector2* frameBufSize, const Matrix4* projMat);
	static void AddMaterial(Material* newMat);

private:
	//STRUCTORS
	MaterialBank() : m_currMaterial(nullptr) { }
	~MaterialBank();

	//INIT MATERIALS AND DEFAULT PROGRAMS
	void InitializeMultiLightProgramAndMaterial();
	void InitializeDefault2DProgramAndMaterial();
	void InitializePreDepthPassProgramAndMaterial();
	void InitializeShadowCastingShaderProgramAndMaterials();
	void InitializeUnlitShaderProgram();

	//MTL PARSING
	void ParseMTLLine(const String& str);
	void AddCurrWorkingMaterialToBankAndReset();
	
	//XML PARSING
	void ParseMaterialXML(const char* dir, XMLNode& matNode);




	static MaterialBank* s_materialBank;
	std::map<size_t, Material*> m_materials;
	Material* m_currMaterial					= nullptr;
};

typedef std::map<size_t, Material*>::iterator MaterialMapIterator;
typedef std::pair<size_t, Material*> MaterialMapPair;