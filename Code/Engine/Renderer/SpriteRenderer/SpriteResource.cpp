#include "Engine/Renderer/SpriteRenderer/SpriteResource.hpp"
#include "Engine/Renderer/SpriteRenderer/TheSpriteRenderer.hpp"

STATIC SpriteResourceMap SpriteResource::s_registeredSpriteResources;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//STATICS
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
STATIC SpriteResource* SpriteResource::Create(const String& id, const String& textureFilename) {

	size_t hash = std::hash<String>()(id);

	SpriteResourceMapIterator it = s_registeredSpriteResources.find(hash);

	if (it == s_registeredSpriteResources.end()) {
		SpriteResource* nSpriteResource = new SpriteResource(id, textureFilename);
		s_registeredSpriteResources.insert(SpriteResourceMapPair(hash, nSpriteResource));
		return nSpriteResource;
	}
	else {
		TheLogger::LogPrintf("Sprite resource with id %s already exists", "SpriteRenderer", LOG_DEFAULT, id);
		ERROR_AND_DIE("Sprite resource already exists");
	}
}
STATIC SpriteResource* SpriteResource::CreateFromXML(const XMLNode& node) {
	SpriteResource* nResource = new SpriteResource();

	std::vector<XMLUtils::Attribute> resourceAttribs;
	XMLUtils::GetAllAttributesOfNode(node, resourceAttribs);

	for (unsigned int i = 0; i < resourceAttribs.size(); i++) {

		XMLUtils::Attribute& currAttrib = resourceAttribs[i];
		String currAttribName = currAttrib.name;
		String currAttribVal = currAttrib.value;

		if (currAttribName == "ID") {
			nResource->m_id = currAttribVal;
		}
		else if (currAttribName == "TexCoordsRect") {
			nResource->m_texCoordsRect = XMLUtils::ParseAABB2(currAttribVal);
		}
		else if (currAttribName == "Dimensions") {
			nResource->m_dimensions = XMLUtils::ParseVector2(currAttribVal);
		}
		else if (currAttribName == "Pivot") {
			nResource->m_pivotPoint = XMLUtils::ParseVector2(currAttribVal);
		}
		else if (currAttribName == "TextureFileName") {
			nResource->m_textureFilename = currAttribVal;
		}
	}

	nResource->m_pivotPoint = Vector2(nResource->m_dimensions.x / 2.f, nResource->m_dimensions.y / 2.f);

	////////////////////////////////////////////////////
	//------------------------------------------
	//ADD TO REGISTRY
	size_t hash = std::hash<String>()(nResource->m_id);
	SpriteResourceMapIterator it = s_registeredSpriteResources.find(hash);

	if (it == s_registeredSpriteResources.end()) {
		s_registeredSpriteResources.insert(SpriteResourceMapPair(hash, nResource));
		return nResource;
	}
	else {
		TheLogger::LogPrintf("Sprite resource with id %s already found!", "SpriteRenderer", LOG_DEFAULT, nResource->m_id.c_str());
		ERROR_AND_DIE("Sprite resource already exists");
		return nullptr;
	}
}
STATIC SpriteResource* SpriteResource::Get(const String& id) {
	size_t hash = std::hash<String>()(id);

	SpriteResourceMapIterator it = s_registeredSpriteResources.find(hash);

	if (it != s_registeredSpriteResources.end()) {
		return it->second;
	}
	else {
		TheLogger::LogPrintf("ERROR: Could sprite resource with id %s does not exist.", "SpriteRenderer", LOG_DEFAULT, id);
		return nullptr;
	}
}

STATIC void SpriteResource::DestroySpriteResources() {
	for (SpriteResourceMapIterator it = s_registeredSpriteResources.begin(); it != s_registeredSpriteResources.end(); it++) {
		delete it->second;
		it->second = nullptr;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//MEMBERS
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS INIT
//---------------------------------------------------------------------------------------------------------------------------
SpriteResource::SpriteResource()
	: m_id("")
	, m_texCoordsRect()
	, m_defaultMaterial(nullptr)
	, m_dimensions(Vector2::ZERO)
	, m_pivotPoint(Vector2::ZERO)
	, m_textureFilename("")
{ }
SpriteResource::SpriteResource(const String& id, const String& textureFilename) 
	: m_id(id)
	, m_texCoordsRect()
	, m_defaultMaterial(nullptr)
	, m_dimensions(Vector2::ZERO)
	, m_pivotPoint(Vector2::ZERO)
	, m_textureFilename(textureFilename)
{
	m_texCoordsRect = AABB2(Vector2(0.f, 0.f), Vector2(1.f, 1.f));
	CalculateDimensions();
	m_pivotPoint = Vector2(m_dimensions.x / 2.f, m_dimensions.y / 2.f);
}

void SpriteResource::CalculateDimensions() {
	const Vector2* screenSize = BeirusRenderer::GetScreenSize();
	Vector2 virtualBounds = TheSpriteRenderer::GetBounds();
	float importSize = TheSpriteRenderer::GetImportSize();

	m_dimensions = Vector2((importSize / screenSize->x) * virtualBounds.x, (importSize / screenSize->y) * virtualBounds.y);
}
