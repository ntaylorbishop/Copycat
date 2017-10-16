#pragma once

#include <fstream>
#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Utils/GeneralUtils.hpp"
#include "ThirdParty/Parsers/xmlParser.h"
#include "Engine/Renderer/General/RGBA.hpp"

class D3D11Material;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INITIAL DEFINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const RGBA	MATERIAL_AMBIENT_LIGHT = RGBA(1.f, 1.f, 1.f, 1.f);
extern float MATERIAL_SPEC_POWER;
const int	DEFAULT_VIEW_MODE = 0;

const char* const DEPTH_TEST_PARAM_ON = "true";
const char* const DEPTH_TEST_PARAM_OFF = "false";
const char* const DEPTH_TEST_PARAM_DUAL = "dual";

const char* const BLEND_MODE_PARAM_OPAQUE = "opaque";
const char* const BLEND_MODE_PARAM_TRANSLUCENT = "translucent";
const char* const BLEND_MODE_PARAM_ADDITIVE = "additive";

const char* const BACKFACE_NOCULL = "noCull";
const char* const BACKFACE_CULL = "cull";

const char* const LIGHTING_MODEL_PARAM_PHONG = "phong";
const char* const LIGHTING_MODEL_PARAM_BRDF = "brdf";
const char* const LIGHTING_MODEL_PARAM_UNLIT = "unlit";


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MATERIAL BANK
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class D3D11MaterialBank {
public:

	static D3D11MaterialBank* Get();

	//INTERFACE
	bool AddMaterialsFromMTLFile(const char* filenameAndPath);
	bool AddMaterialsFromXMLFile(const char* dir, const char* filename);
	bool AddGeneratedMaterial(D3D11Material* mat);
	D3D11Material* GetMaterial(const String& name);
	bool InsertMaterial(D3D11Material* mat);
	void AddMaterial(D3D11Material* newMat);

private:
	//STRUCTORS
	D3D11MaterialBank() : m_currMaterial(nullptr) { }
	~D3D11MaterialBank();

	static void Destroy();

	//MTL PARSING
	void ParseMTLLine(const String& str);
	void AddCurrWorkingMaterialToBankAndReset();

	//XML PARSING
	void ParseMaterialXML(const char* dir, XMLNode& matNode);


	static D3D11MaterialBank* s_materialBank;
	std::map<size_t, D3D11Material*> m_materials;
	D3D11Material* m_currMaterial = nullptr;
};

typedef std::map<size_t, D3D11Material*>::iterator MaterialMapIterator;
typedef std::pair<size_t, D3D11Material*> MaterialMapPair;