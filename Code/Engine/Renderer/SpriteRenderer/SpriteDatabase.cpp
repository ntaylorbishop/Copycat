#include "Engine/Renderer/SpriteRenderer/SpriteDatabase.hpp"
#include "Engine/General/Logger/TheLogger.hpp"
#include "Engine/Utils/FileUtils/FileUtils.hpp"

STATIC SpriteDatabase* SpriteDatabase::s_spriteDatabase = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
STATIC void SpriteDatabase::RegisterSprite(const String& id, const String& texturePath) {
	
	if (!s_spriteDatabase) {
		s_spriteDatabase = new SpriteDatabase();
	}

	s_spriteDatabase->AddSpriteResource(id, texturePath);

}
STATIC void SpriteDatabase::Destroy() {
	delete s_spriteDatabase;
}

STATIC const SpriteResource* SpriteDatabase::GetSpriteResource(const String& id) {
	
	SpriteDatabaseMapIterator it = s_spriteDatabase->m_spriteResources.find(std::hash<String>()(id));

	if (it != s_spriteDatabase->m_spriteResources.end()) {
		return it->second;
	}
	else {
		TheLogger::LogPrintf("Could not get sprite resource with id %s", "SpriteRenderer", LOG_DEFAULT, id.c_str());
		return nullptr;
	}
}
STATIC SpriteResource* SpriteDatabase::EditSpriteResource(const String& id) {

	SpriteDatabaseMapIterator it = s_spriteDatabase->m_spriteResources.find(std::hash<String>()(id));

	if (it != s_spriteDatabase->m_spriteResources.end()) {
		return it->second;
	}
	else {
		TheLogger::LogPrintf("Could not get sprite resource with id %s", "SpriteRenderer", LOG_DEFAULT, id.c_str());
		return nullptr;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//SAVING
//---------------------------------------------------------------------------------------------------------------------------
STATIC void SpriteDatabase::SaveToXML() {

	if (s_spriteDatabase == nullptr) {
		TheLogger::LogPrintf("SpriteDatabase is null. Could not save out", "SpriteDatabase", LOG_DEFAULT);
	}

	FileUtils::CreateDirectoryIfDoesntExist("Data/XML");

	XMLNode rootNode = XMLNode();
	rootNode = rootNode.createXMLTopNode("SpriteDatabase");

	for (SpriteDatabaseMapIterator it = s_spriteDatabase->m_spriteResources.begin(); it != s_spriteDatabase->m_spriteResources.end(); ++it) {
		SpriteResource* currResource = it->second;
		XMLNode resourceNode = CreateSpriteResourceNode(currResource);
		rootNode.addChild(resourceNode);
	}

	if (rootNode.writeToFile("Data/XML/SpriteDatabase.xml") == 0) {
		TheLogger::LogPrintf("SpriteDatabase saved out to Data/XML/SpriteDatabase.xml succesfully!", "SpriteDatabase", LOG_DEFAULT);
	}
	else {
		TheLogger::LogPrintf("Could not save SpriteDatabase out to Data/XML/SpriteDatabase.xml!", "SpriteDatabase", LOG_DEFAULT);
	}
}
STATIC XMLNode SpriteDatabase::CreateSpriteResourceNode(SpriteResource* resource) {
	XMLNode resourceBlueprint;
	resourceBlueprint = resourceBlueprint.createXMLTopNode("NPCBlueprint");

	resourceBlueprint.addAttribute("ID", resource->m_id.c_str());
	resourceBlueprint.addAttribute("TexCoordsRect", resource->m_texCoordsRect.ToString().c_str());
	resourceBlueprint.addAttribute("Dimensions", resource->m_dimensions.ToString().c_str());
	resourceBlueprint.addAttribute("Pivot", resource->m_pivotPoint.ToString().c_str());
	resourceBlueprint.addAttribute("TextureFileName", resource->m_textureFilename.c_str());

	return resourceBlueprint;
}

//---------------------------------------------------------------------------------------------------------------------------
//LOADING
//---------------------------------------------------------------------------------------------------------------------------
STATIC void SpriteDatabase::LoadFromXML() {

	if (!s_spriteDatabase) {
		s_spriteDatabase = new SpriteDatabase();
	}

	XMLNode root = XMLNode::parseFile("Data/XML/SpriteDatabase.xml");
	root = root.getChildNode(0);

	if (root.isEmpty()) {
		TheLogger::LogPrintf("Could not load SpriteDatabase. Database is either empty or file does not exist", "SpriteDatabase", LOG_DEFAULT);
	}

	for (int i = 0; i < root.nChildNode(); i++) {

		SpriteResource* nResource = SpriteResource::CreateFromXML(root.getChildNode(i));
		s_spriteDatabase->AddSpriteResource(nResource->m_id, nResource);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SpriteDatabase::SpriteDatabase() { }
SpriteDatabase::~SpriteDatabase() {

}

void SpriteDatabase::AddSpriteResource(const String& id, const String& texturePath) {
	SpriteResource* nSpriteResource = SpriteResource::Create(id, texturePath);

	size_t hash = std::hash<String>()(id);
	SpriteDatabaseMapIterator it = m_spriteResources.find(hash);

	if (it == m_spriteResources.end()) {
		m_spriteResources.insert(SpriteDatabaseMapPair(hash, nSpriteResource));
	}
	else {
		TheLogger::LogPrintf("ERROR: Sprite resource with id %s already exists in map", "SpriteRenderer", LOG_DEFAULT, id);
	}
}
void SpriteDatabase::AddSpriteResource(const String& id, SpriteResource* nResource) {

	size_t hash = std::hash<String>()(id);
	SpriteDatabaseMapIterator it = m_spriteResources.find(hash);

	if (it == m_spriteResources.end()) {
		m_spriteResources.insert(SpriteDatabaseMapPair(hash, nResource));
	}
	else {
		TheLogger::LogPrintf("ERROR: Sprite resource with id %s already exists in map", "SpriteRenderer", LOG_DEFAULT, id);
	}
}