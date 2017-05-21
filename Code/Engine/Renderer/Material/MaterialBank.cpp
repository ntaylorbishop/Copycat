#include "Engine/Renderer/Material/MaterialBank.hpp"
#include "Engine/General/Logger/TheLogger.hpp"
#include "Engine/Renderer/Shaders/ShaderProgram.hpp"
#include "Engine/Renderer/General/TextureCubemap.hpp"
#include "Engine/Renderer/Lights/ShadowCastingLight3D.hpp"
#include "Engine/Renderer/Effects/SSAO.hpp"
#include "Engine/Utils/GeneralUtils/XMLUtils.hpp"

STATIC MaterialBank* MaterialBank::s_materialBank = nullptr;
float MATERIAL_SPEC_POWER = 20.f;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INIT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------
STATIC void MaterialBank::Initialize() {

	ASSERT_OR_DIE(s_materialBank == nullptr, "ERROR: Material bank already initialized.");
	s_materialBank = new MaterialBank();

}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void MaterialBank::Destroy() {

	ASSERT_OR_DIE(s_materialBank != nullptr, "ERROR: Material bank already destroyed.");
	delete s_materialBank;
	s_materialBank = nullptr;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool MaterialBank::AddMaterialsFromMTLFile(const char* filenameAndPath) {

	std::fstream fin(filenameAndPath, std::ios::in);

	if (!fin.is_open()) {
		TheLogger::LogPrintf("ERROR: Could not open .mtl file.", "MaterialBank", LOG_SEVERE);
		return false;
	}

	while (!fin.eof()) {
		String fLine;
		std::getline(fin, fLine);
		s_materialBank->ParseMTLLine(fLine);
	}

	return true;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool MaterialBank::AddMaterialsFromXMLFile(const char* dir, const char* filename) {

	String filenameAndPath = String(dir) + String(filename);
	XMLNode root = XMLNode::parseFile(filenameAndPath.c_str());
	root = root.getChildNode(0);

	for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++) {
		
		XMLNode matNode = root.getChildNode(childIdx);
		s_materialBank->ParseMaterialXML(dir, matNode);
	}

	return true;
}



//---------------------------------------------------------------------------------------------------------------------------
STATIC bool MaterialBank::AddGeneratedMaterial(Material* mat) {

	size_t id = std::hash<String>()(mat->GetName());
	MaterialMapIterator matIt = s_materialBank->m_materials.find(id);

	if (matIt != s_materialBank->m_materials.end()) {
		return false;
	}
	else {
		s_materialBank->m_materials.insert(MaterialMapPair(id, mat));
		return true;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC Material* MaterialBank::GetMaterial(const String& name) {

	size_t hash = std::hash<String>()(name);
	MaterialMapIterator mat = s_materialBank->m_materials.find(hash);

	if (mat == s_materialBank->m_materials.end()) {
		return nullptr;
	}
	else {
		return mat->second;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC bool MaterialBank::InsertMaterial(Material* mat) {

	size_t hash = std::hash<String>()(mat->GetName());
	MaterialMapIterator matIt = s_materialBank->m_materials.find(hash);

	if (matIt == s_materialBank->m_materials.end()) {
		s_materialBank->m_materials.insert(MaterialMapPair(hash, mat));
		return true;
	}

	return false;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------------------------------
MaterialBank::~MaterialBank() {

	for (MaterialMapIterator it = m_materials.begin(); it != m_materials.end(); ++it) {
		delete it->second;
	}

	m_materials.clear();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INIT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void MaterialBank::AddMaterial(Material* newMat) {

	String matName = newMat->GetName();
	size_t nameHash = std::hash<String>()(matName);

	MaterialMapIterator mapIt = s_materialBank->m_materials.find(nameHash);
	if (mapIt != s_materialBank->m_materials.end()) {
		TheLogger::LogPrintf("Material %s already exists in MaterialBank.", matName.c_str());
	}

	s_materialBank->m_materials.insert(MaterialMapPair(nameHash, newMat));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MTL PARSING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void MaterialBank::ParseMTLLine(const String& str) {

	//String relDir = "Data/Models/Sponza/";

	std::vector<String> lineToks;
	StringUtils::TokenizeBuffer(str, ' ', lineToks);

	if (lineToks.size() == 0) {
		return;
	}

	//ADD NEW MATERIAL
	if (lineToks[0] == "newmtl") {
		AddCurrWorkingMaterialToBankAndReset();
		m_currMaterial = new Material("BlinnPhong");
		m_currMaterial->SetName(lineToks[1]);
	}

	//SPECULAR EXPONENT
	else if (lineToks[0] == "\tNs") {
		float* specExponent = new float((float)atof(lineToks[1].c_str()));
		m_currMaterial->CreateUniform("gSpecularExponent", UNIFORM_FLOAT, 1, specExponent);
	}

	//DIFFUSE MAP
	else if (lineToks[0] == "\tmap_Ka") {

		Texture* texture = Texture::CreateOrGetTexture("Data/Models/Sponza/" + lineToks[1], true);
		texture->GenerateMipmap();
		m_currMaterial->CreateUniform("gTexDiffuse", UNIFORM_TEXTURE2D, 1, texture);
	}

	//BUMP MAP
	else if (lineToks[0] == "\tmap_bump") {

		Texture* texture = Texture::CreateOrGetTexture("Data/Models/Sponza/" + lineToks[1]);
		texture->GenerateMipmap();
		m_currMaterial->CreateUniform("gTexBump", UNIFORM_TEXTURE2D, 1, texture);
	}

	//NORMAL MAP
	else if (lineToks[0] == "\tmap_normal") {

		Texture* texture = Texture::CreateOrGetTexture("Data/Models/Sponza/" + lineToks[1]);
		texture->GenerateMipmap();
		m_currMaterial->CreateUniform("gTexNormal", UNIFORM_TEXTURE2D, 1, texture);
	}

	//SPECULAR MAP
	else if (lineToks[0] == "\tmap_Ns") {

		Texture* texture = Texture::CreateOrGetTexture("Data/Models/Sponza/" + lineToks[1]);
		texture->GenerateMipmap();
		m_currMaterial->CreateUniform("gTexSpecular", UNIFORM_TEXTURE2D, 1, texture);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void MaterialBank::AddCurrWorkingMaterialToBankAndReset() {

	if (!m_currMaterial) {
		return;
	}

	size_t hash = std::hash<String>()(m_currMaterial->GetName());
	m_materials.insert(MaterialMapPair(hash, m_currMaterial));

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MTL PARSING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
static void ParseColorProperties(const char* dir, const XMLNode& colorPropertiesNode, Material* currMat) {

	XMLNode colNode = colorPropertiesNode;

	for (int colPropertiesIdx = 0; colPropertiesIdx < colNode.nAttribute(); colPropertiesIdx++) {

		XMLAttribute colAttrib = colNode.getAttribute(colPropertiesIdx);

		//AMBIENT REFLECTIVITY
		if (strcmp(colAttrib.lpszName, "AmbientReflectivity") == 0) {

			Vector4 specExponent = XMLUtils::ParseVector4(colAttrib.lpszValue);
			currMat->CreateUniform("gAmbientLight", UNIFORM_VECTOR4, 1, new Vector4(specExponent));
		}

		//SPECULAR COLOR
		else if (strcmp(colAttrib.lpszName, "SpecularColor") == 0) {

			//TODO: IMPLEMENT SPECULAR COLOR
		}

		//SPECULAR EXPONENT
		else if (strcmp(colAttrib.lpszName, "SpecularExponent") == 0) {

			float* specExponent = new float(XMLUtils::ParseFloat(colAttrib.lpszValue));
			currMat->CreateUniform("gSpecularExponent", UNIFORM_FLOAT, 1, specExponent);
		}

		//DIFFUSE TEXTURE
		else if (strcmp(colAttrib.lpszName, "DiffuseTexture") == 0) {

			String diffuseTexPath = String(dir) + String(colAttrib.lpszValue);
			Texture* diffuseTex = nullptr;
			if (IS_HDR_ENABLED) {
				diffuseTex = Texture::CreateOrGetTexture(diffuseTexPath, true);
			}
			else {
				diffuseTex = Texture::CreateOrGetTexture(diffuseTexPath, false);
			}
			diffuseTex->GenerateMipmap();
			currMat->CreateUniform("gTexDiffuse", UNIFORM_TEXTURE2D, 1, 0, diffuseTex);
		}

		//NORMAL MAP
		else if (strcmp(colAttrib.lpszName, "NormalMap") == 0) {

			String normalTexPath = String(dir) + String(colAttrib.lpszValue);
			Texture* normalTex = Texture::CreateOrGetTexture(normalTexPath);

			normalTex->GenerateMipmap();
			currMat->CreateUniform("gTexNormal", UNIFORM_TEXTURE2D, 1, 1, normalTex);
		}

		//MASK TEXTURE
		else if (strcmp(colAttrib.lpszName, "MaskTexture") == 0) {

			String maskTexPath = String(dir) + String(colAttrib.lpszValue);
			Texture* maskTex = Texture::CreateOrGetTexture(maskTexPath);
			maskTex->GenerateMipmap();
			currMat->CreateUniform("gTexSpecular", UNIFORM_TEXTURE2D, 1, 2, maskTex);
		}

		//MASK TEXTURE
		else if (strcmp(colAttrib.lpszName, "RoughnessMap") == 0) {

			String maskTexPath = String(dir) + String(colAttrib.lpszValue);
			Texture* rTex = Texture::CreateOrGetTexture(maskTexPath);
			rTex->GenerateMipmap();
			currMat->CreateUniform("gTexRoughness", UNIFORM_TEXTURE2D, 1, 3, rTex);
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
static void ParseStateProperties(const XMLNode& statePropertiesNode, Material* currMat) {

	XMLNode stateNode = statePropertiesNode;

	for (int statePropertiesIdx = 0; statePropertiesIdx < stateNode.nAttribute(); statePropertiesIdx++) {

		XMLAttribute stateAttrib = stateNode.getAttribute(statePropertiesIdx);

		//DEPTH TEST
		if (strcmp(stateAttrib.lpszName, "DepthTest") == 0) {

			const char* depthTestMode = stateAttrib.lpszValue;

			if (strcmp(depthTestMode, DEPTH_TEST_PARAM_ON) == 0) {
				currMat->m_renderState.m_depthMode = DEPTH_MODE_ON;
			}
			else if (strcmp(depthTestMode, DEPTH_TEST_PARAM_OFF) == 0) {
				currMat->m_renderState.m_depthMode = DEPTH_MODE_OFF;
			}
			else if (strcmp(depthTestMode, DEPTH_TEST_PARAM_DUAL) == 0) {
				currMat->m_renderState.m_depthMode = DEPTH_MODE_DUAL;
			}
			else {
				ERROR_AND_DIE("Invalid depth test mode defined in XML.");
			}
		}

		//BLEND MODE
		else if (strcmp(stateAttrib.lpszName, "BlendMode") == 0) {
			
			const char* isOpaque = stateAttrib.lpszValue;

			if (strcmp(isOpaque, BLEND_MODE_PARAM_OPAQUE) == 0) {
				currMat->m_renderState.m_blendMode = BLEND_MODE_OPAQUE;
			}
			else if (strcmp(isOpaque, BLEND_MODE_PARAM_TRANSLUCENT) == 0) {
				currMat->m_renderState.m_blendMode = BLEND_MODE_TRANSPARENT_DEFAULT;
			}
			else if (strcmp(isOpaque, BLEND_MODE_PARAM_ADDITIVE) == 0) {
				currMat->m_renderState.m_blendMode = BLEND_MODE_TRANSPARENT_ADDITIVE;
			}
			else {
				ERROR_AND_DIE("Invalid depth test mode defined in XML.");
			}
		}

		//BACKFACE CULLING
		else if (strcmp(stateAttrib.lpszName, "BackFace") == 0) {

			const char* isOpaque = stateAttrib.lpszValue;

			if (strcmp(isOpaque, BACKFACE_NOCULL) == 0) {
				currMat->m_renderState.m_backfaceCulling = false;
			}
			else if (strcmp(isOpaque, BACKFACE_CULL) == 0) {
				currMat->m_renderState.m_backfaceCulling = true;
			}
			else {
				ERROR_AND_DIE("Invalid depth test mode defined in XML.");
			}
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
static String GetShaderProgramNameFromLightingModelAttrib(const char* lightingModelAttrib) {

	//LIGHTING MODEL
	if (strcmp(lightingModelAttrib, LIGHTING_MODEL_PARAM_PHONG) == 0) {
		return "BlinnPhong";
	}
	else if (strcmp(lightingModelAttrib, LIGHTING_MODEL_PARAM_BRDF) == 0) {
		return "CookTorranceBRDF";
	}
	else if (strcmp(lightingModelAttrib, LIGHTING_MODEL_PARAM_UNLIT) == 0) {
		return "Unlit";
	}
	else {
		ERROR_AND_DIE("Invalid depth test mode defined in XML.");
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void MaterialBank::ParseMaterialXML(const char* dir, XMLNode& matNode) {

	XMLAttribute nameAttrib = matNode.getAttribute(0);
	XMLAttribute lightingModelAttrib = matNode.getAttribute(1);

	bool isWellFormed =
		strcmp(nameAttrib.lpszName, "name") == 0					&&
		strcmp(lightingModelAttrib.lpszName, "lightingModel") == 0	&&
		matNode.nChildNode() == 2									&&
		matNode.nAttribute() == 2;

	ASSERT_OR_DIE(isWellFormed, "ERROR: Material data not well formed XML.");

	String matName = nameAttrib.lpszValue;
	String shaderProg = GetShaderProgramNameFromLightingModelAttrib(lightingModelAttrib.lpszValue);
	Material* newMat = new Material(shaderProg);
	newMat->SetName(matName);

	for (int propertiesIdx = 0; propertiesIdx < matNode.nChildNode(); propertiesIdx++) {

		XMLNode child = matNode.getChildNode(propertiesIdx);

		if (strcmp(child.getName(), "ColorProperties") == 0) {
			ParseColorProperties(dir, child, newMat);
		}
		else if (strcmp(child.getName(), "StateProperties") == 0) {
			ParseStateProperties(child, newMat);
		}
	}

	size_t id = std::hash<String>()(matName);
	s_materialBank->m_materials.insert(MaterialMapPair(id, newMat));
}