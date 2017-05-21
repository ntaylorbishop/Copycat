#include "Engine/Renderer/ReflectionProbes/StaticReflectionProbe.hpp"
#include "Engine/Renderer/General/TextureCubemap.hpp"
#include "ThirdParty/Parsers/xmlParser.h"
#include "Engine/Utils/GeneralUtils/XMLUtils.hpp"
#include "Engine/Renderer/Material/Material.hpp"
#include "Engine/Renderer/Mesh/Model.hpp"
#include "Engine/Scene/Scene.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CREATE DESTROY
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC StaticReflectionProbe* StaticReflectionProbe::Load(const String& probeFile, Scene* sceneToLoadInTo) {

	XMLNode root = XMLNode::parseFile(probeFile.c_str());
	root = root.getChildNode(0);

	std::vector<XMLUtils::Attribute> rootAttribs;
	XMLUtils::GetAllAttributesOfNode(root, rootAttribs);

	ASSERT_OR_DIE(rootAttribs.size() == 3, "ERROR: Cannot parse static reflection probe XML.");

	Vector3 position = Vector3::ZERO;
	Vector3 boxMins = Vector3::ZERO;
	Vector3 boxMaxs = Vector3::ZERO;

	for (size_t i = 0; i < rootAttribs.size(); i++) {

		XMLUtils::Attribute& attrib = rootAttribs[i];
		if (attrib.name == "Position") {
			position = XMLUtils::ParseVector3(attrib.value);
		}
		else if (attrib.name == "BoxMins") {
			boxMins = XMLUtils::ParseVector3(attrib.value);
		}
		else if (attrib.name == "BoxMaxs") {
			boxMaxs = XMLUtils::ParseVector3(attrib.value);
		}
	}

	root = root.getChildNode(0);


	String xPositive;
	String xNegative;
	String yPositive;
	String yNegative;
	String zPositive;
	String zNegative;

	for (size_t i = 0; i < root.nChildNode(); i++) {

		XMLNode& child = root.getChildNode(i);

		if (strcmp(child.getName(), "Texture") != 0) {
			ERROR_AND_DIE("ERROR: Cannot parse static reflection probe XML.");
		}

		XMLAttribute faceAttrib = child.getAttribute(0);
		XMLAttribute pathAttrib = child.getAttribute(1);

		if (strcmp(faceAttrib.lpszName, "faceIdx") == 0 && strcmp(faceAttrib.lpszValue, "posX") == 0) {
			if (strcmp(pathAttrib.lpszName, "path") == 0) {
				xPositive = pathAttrib.lpszValue;
			}
		}
		else if (strcmp(faceAttrib.lpszName, "faceIdx") == 0 && strcmp(faceAttrib.lpszValue, "negX") == 0) {
			if (strcmp(pathAttrib.lpszName, "path") == 0) {
				xNegative = pathAttrib.lpszValue;
			}
		}
		else if (strcmp(faceAttrib.lpszName, "faceIdx") == 0 && strcmp(faceAttrib.lpszValue, "posY") == 0) {
			if (strcmp(pathAttrib.lpszName, "path") == 0) {
				yPositive = pathAttrib.lpszValue;
			}
		}
		else if (strcmp(faceAttrib.lpszName, "faceIdx") == 0 && strcmp(faceAttrib.lpszValue, "negY") == 0) {
			if (strcmp(pathAttrib.lpszName, "path") == 0) {
				yNegative = pathAttrib.lpszValue;
			}
		}
		else if (strcmp(faceAttrib.lpszName, "faceIdx") == 0 && strcmp(faceAttrib.lpszValue, "posZ") == 0) {
			if (strcmp(pathAttrib.lpszName, "path") == 0) {
				zPositive = pathAttrib.lpszValue;
			}
		}
		else if (strcmp(faceAttrib.lpszName, "faceIdx") == 0 && strcmp(faceAttrib.lpszValue, "negZ") == 0) {
			if (strcmp(pathAttrib.lpszName, "path") == 0) {
				zNegative = pathAttrib.lpszValue;
			}
		}
	}

	TextureCubemap* envMap = new TextureCubemap(xPositive.c_str(), xNegative.c_str(), yPositive.c_str(), 
		yNegative.c_str(), zPositive.c_str(), zNegative.c_str());

	StaticReflectionProbe* newProbe = new StaticReflectionProbe(position, boxMins, boxMaxs, sceneToLoadInTo, envMap);
	return newProbe;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void StaticReflectionProbe::Update(float deltaSeconds) {

	m_model->m_model.ChangePosition(m_position);
	ClickableObject::m_model.ChangePosition(m_position);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void StaticReflectionProbe::Render() const {

	ClickableObject::Render();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
StaticReflectionProbe::StaticReflectionProbe(const Vector3& position, Vector3 mins, Vector3 maxs, Scene* sceneToLoadInTo,
	TextureCubemap* envMap) 
	: m_envMap(envMap)
	, m_position(position)
	, m_boxMins(mins)
	, m_boxMaxs(maxs)
	, ClickableObject::ClickableObject(GEOM_TYPE_SPHERE, position)
{

	Sphere* obj = (Sphere*)m_geom;
	obj->m_pos = position;
	obj->m_radius = 1.f;

	m_model = new Model(position, PRIMITIVE_MESH_SPHERE_100);
	m_model->SetShadowCaster(false);
	m_material = new Material("DynamicReflectionProbe");
	Texture* normTex = Texture::CreateOrGetTexture("Data/Textures/Black_Normal.png");
	m_material->CreateUniform("gTexEnvironmentMap", UNIFORM_CUBEMAP, 1, 0, m_envMap);
	m_material->CreateUniform("gTexNormal", UNIFORM_TEXTURE2D, 1, 1, normTex);
	m_model->m_material = m_material;
	sceneToLoadInTo->AddStaticMesh(m_model);
}


//---------------------------------------------------------------------------------------------------------------------------
StaticReflectionProbe::~StaticReflectionProbe() {

	delete m_envMap;
	m_envMap = nullptr;
	delete m_material;
	m_material = nullptr;
}