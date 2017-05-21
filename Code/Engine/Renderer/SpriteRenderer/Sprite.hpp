#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/SpriteRenderer/TheSpriteRenderer.hpp"

class Sprite {
public:
	//STRUCTORS
	static Sprite* CreateSprite(const String& ID, const String& layer);
	static void Delete(Sprite* s);

	void SetLayer(const String& layer);
	String GetLayer() { return m_layerID; }

	//ENABLE DISABLE
	void Enable();
	void Disable();

	//GETTERS SETTERS
	uint GetSpriteID() { return m_instanceID; }
	Material* GetMaterial() { return m_material; }
	SpriteResource* GetSpriteResource() { return m_spriteResource; }

	bool operator==(const Sprite& other);


	Vector2 m_position;
	float m_rotation;
	Vector2 m_scale;
	Vector2 m_pivot;
	RGBA m_tint;
	
	Sprite* m_prev;
	Sprite* m_next;

private:
	Sprite(const String& ID);
	~Sprite();
	
	bool m_enabled;
	String m_layerID;
	uint m_instanceID;
	SpriteResource* m_spriteResource;

	Material* m_material;


	static uint s_spriteIDs;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//INLINE
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool Sprite::operator==(const Sprite& other) {
	return m_instanceID == other.m_instanceID;
}

