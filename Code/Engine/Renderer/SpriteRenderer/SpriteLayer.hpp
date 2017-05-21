#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Logger/TheLogger.hpp"
#include "Engine/Renderer/SpriteRenderer/ParticleSystem/ParticleSystem2D.hpp"

class Sprite;

class SpriteLayer {
public:
	//STATICS
	static SpriteLayer* CreateOrGetLayer(const String& layerID);
	static SpriteLayer* CreateOrGetLayer(const String& name, size_t layerID);
	static SpriteLayer* GetLayer(const String& layerID);
	static void DestroySpriteLayers();

	//PARTICLE SYSTEMS
	void Update(float deltaSeconds);
	void AddParticleSystem(ParticleSystem2D* system);

	//MEMBER FUNCS
	void AddToList(Sprite* sprite);
	void RemoveFromList(Sprite* sprite);
	void AddEffect(const String& shaderProgName);
	
	//GETTERS SETTERS
	uint GetNumSpritesInLayer() { return m_numSpritesContained; }
	size_t GetLayerID() { return m_layerID; }
	Sprite* GetHead() { return m_spriteList; }
	String GetName() { return m_name; }
	void Enable() { m_isEnabled = true; }
	void Disable() { m_isEnabled = false; }
	bool IsEnabled() { return m_isEnabled; }

	Material* m_effect;
	String m_shaderProgName;

private:
	//MEMBERS
	SpriteLayer(const String& name, int layerID);
	~SpriteLayer();

	//MEMBER PARTICLE SYSTEM FUNCS
	void RemoveParticleSystem(ParticleSystem2D* system);

	ParticleSystem2D* m_particleSystemList;
	uint m_numSystemsContained;
	Sprite* m_spriteList;
	uint m_numSpritesContained;
	size_t m_layerID;
	Texture* m_effectColorTarget;
	String m_name;
	bool m_isEnabled;



	static std::map<size_t, SpriteLayer*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, SpriteLayer*>>> s_spriteLayers;
};

typedef std::map<size_t, SpriteLayer*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, SpriteLayer*>>> SpriteLayerMap;
typedef std::pair<size_t, SpriteLayer*> SpriteLayerMapPair;
typedef std::map<size_t, SpriteLayer*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, SpriteLayer*>>>::iterator SpriteLayerMapIterator;