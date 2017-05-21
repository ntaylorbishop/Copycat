#pragma once

#include <functional>
#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Utils/GeneralUtils/XMLUtils.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/Material/Material.hpp"

class SpriteResource {
public:
	SpriteResource(const SpriteResource& other) = delete;
	static SpriteResource* Create(const String& id, const String& textureFilename);
	static SpriteResource* CreateFromXML(const XMLNode& node);
	static SpriteResource* Get(const String& id);
	static void DestroySpriteResources();


	String m_id;
	AABB2 m_texCoordsRect;
	Material* m_defaultMaterial;
	Vector2 m_dimensions;
	Vector2 m_pivotPoint;
	String m_textureFilename;

private:
	//STRUCTORS INIT
	SpriteResource();
	SpriteResource(const String& id, const String& textureFilename);
	void CalculateDimensions();

	static std::map<size_t, SpriteResource*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, SpriteResource*>>> s_registeredSpriteResources;
};

typedef std::map<size_t, SpriteResource*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, SpriteResource*>>> SpriteResourceMap;
typedef std::map<size_t, SpriteResource*, std::less<size_t>, UntrackedAllocator<std::pair<size_t, SpriteResource*>>>::iterator SpriteResourceMapIterator;
typedef std::pair<size_t, SpriteResource*> SpriteResourceMapPair;


/*
ID(name)
Texture
Uv rectangle
Default material
Width height dimensions
Pivot point


SpriteResourceCreate(char const* id, char const* textureFilename) {
	ASSERT(texture exists);
	ASSERT(ID DOESN’T EXIST);

	SpriteResource* src = SpriteResource();
	Sr->uid = id;
	Sr->texture = LoadTexture(filename);
	Sr->default_material = RendererDefaultMaterial;



}

SpriteResource::IDMap


*sr = SpriteResourceEdit("id");

Const * sr = SpriteGet("id");*/