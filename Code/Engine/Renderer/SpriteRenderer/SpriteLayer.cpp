#include "Engine/Renderer/SpriteRenderer/SpriteLayer.hpp"
#include "Engine/Renderer/SpriteRenderer/Sprite.hpp"

STATIC SpriteLayerMap SpriteLayer::s_spriteLayers;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//STATICS
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
STATIC SpriteLayer* SpriteLayer::CreateOrGetLayer(const String& layerID) {

	return CreateOrGetLayer(layerID, std::hash<String>()(layerID));
}
STATIC SpriteLayer* SpriteLayer::CreateOrGetLayer(const String& name, size_t layerID) {

	SpriteLayerMapIterator it = s_spriteLayers.find(layerID);

	if (it != s_spriteLayers.end()) {
		return it->second;
	}
	else {
		SpriteLayer* nLayer = new SpriteLayer(name, (int)layerID);
		s_spriteLayers.insert(SpriteLayerMapPair(layerID, nLayer));
		return nLayer;
	}
}
STATIC SpriteLayer* SpriteLayer::GetLayer(const String& layerID) {

	SpriteLayerMapIterator it = s_spriteLayers.find(std::hash<String>()(layerID));
	return (it != s_spriteLayers.end()) ? (it->second) : (nullptr);
}
STATIC void SpriteLayer::DestroySpriteLayers() {
	for (SpriteLayerMapIterator it = s_spriteLayers.begin(); it != s_spriteLayers.end(); it++) {
		delete it->second;
		it->second = nullptr;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//PARTICLE SYSTEMS
//---------------------------------------------------------------------------------------------------------------------------
void SpriteLayer::Update(float deltaSeconds) {

	ParticleSystem2D* head = m_particleSystemList;

	while (head) {

		if (!head->IsPlaying()) {
			RemoveParticleSystem(head);
			ParticleSystem2D* toDelete = head;
			head = head->m_next;
			delete toDelete;
		}
		else {
			head->Update(deltaSeconds);
			head = head->m_next;
		}
	}
}
void SpriteLayer::AddParticleSystem(ParticleSystem2D* system) {

	if (m_particleSystemList != nullptr) {
		ParticleSystem2D* tmp = m_particleSystemList;

		while (tmp->m_next != nullptr) {
			tmp = tmp->m_next;
		}

		tmp->m_next = system;
		system->m_prev = tmp;

		m_numSystemsContained++;
	}
	else {
		m_particleSystemList = system;
		system->m_prev = nullptr;
		system->m_next = nullptr;
		m_numSystemsContained++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//MEMBERS
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
SpriteLayer::SpriteLayer(const String& name, int layerID)
	: m_layerID(layerID)
	, m_numSpritesContained(0)
	, m_spriteList(nullptr)
	, m_effect(nullptr)
	, m_name(name)
	, m_isEnabled(true)
	, m_particleSystemList(nullptr)
	, m_numSystemsContained(0)
{ 
	m_shaderProgName = "DefaultFullScreen";
	m_effect = new Material("DefaultFullScreen");
}

SpriteLayer::~SpriteLayer() { 

	ParticleSystem2D* head = m_particleSystemList;
	while (head) {

		ParticleSystem2D* toDelete = head;
		head = head->m_next;
		delete toDelete;
	}
	delete m_effect;
}

//---------------------------------------------------------------------------------------------------------------------------
//MEMBER PARTICLE SYSTEM FUNCS
//---------------------------------------------------------------------------------------------------------------------------
void SpriteLayer::RemoveParticleSystem(ParticleSystem2D* system) {

	if (m_particleSystemList == nullptr || system == nullptr)
		return;

	if (m_particleSystemList == system)
		m_particleSystemList = system->m_next;

	if (system->m_next != NULL)
		system->m_next->m_prev = system->m_prev;

	if (system->m_prev != NULL)
		system->m_prev->m_next = system->m_next;

	m_numSystemsContained--;
}

//---------------------------------------------------------------------------------------------------------------------------
//ADD REMOVE
//---------------------------------------------------------------------------------------------------------------------------
void SpriteLayer::AddToList(Sprite* sprite) {
	if (m_spriteList != nullptr) {
		Sprite* tmp = m_spriteList;

		while (tmp->m_next != nullptr) {
			tmp = tmp->m_next;
		}

		tmp->m_next = sprite;
		sprite->m_prev = tmp;
		
		m_numSpritesContained++;
	}
	else {
		m_spriteList = sprite;
		sprite->m_prev = nullptr;
		sprite->m_next = nullptr;
		m_numSpritesContained++;
	}
}
void SpriteLayer::RemoveFromList(Sprite* sprite) {

	if (m_spriteList == nullptr || sprite == nullptr)
		return;

	if (m_spriteList == sprite)
		m_spriteList = sprite->m_next;

	if (sprite->m_next != NULL)
		sprite->m_next->m_prev = sprite->m_prev;

	if (sprite->m_prev != NULL)
		sprite->m_prev->m_next = sprite->m_next;

	m_numSpritesContained--;
}

//---------------------------------------------------------------------------------------------------------------------------
//LAYER EFFECTS
//---------------------------------------------------------------------------------------------------------------------------
void SpriteLayer::AddEffect(const String& shaderProgName) {
	delete m_effect;
	m_effect = new Material(shaderProgName);

	m_shaderProgName = shaderProgName;
}