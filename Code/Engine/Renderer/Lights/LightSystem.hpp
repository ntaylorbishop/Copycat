#pragma once

#define _USE_MATH_DEFINES

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/Lights/Light3D.hpp"
#include "Engine/Math/Vector3.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FORWARD DECLARES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class LightSystem;
class Light3D;
class SSBO;
class Scene;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CONSTS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_NUM_LIGHTS 4096
extern RGBA LIGHTS_AMBIENT_LIGHT;

class LightSystem {
public:
	//INIT SHUTDOWN
	static void Initialize();
	static void Shutdown();

	//UPDATE RENDER
	static void UpdateLightsOnMultilightShader()		{ s_theLightSystem->InternalUpdateLightsOnMultilightShader();	}
	static void UpdateShadowCastingLights(Scene* scene)	{ s_theLightSystem->InternalUpdateShadowCastingLights(scene);	}

	static void UpdateLightListOnGPU() { }

	//ADDING REMOVING
	static uint AddLight(Light3D* light);
	static void ClearLights();

	//GETTERS SETTERS
	static std::vector<Light3D*>* GetLights() { return &s_theLightSystem->m_lights; }
	static SSBO* GetLightDataSSBO() { return s_theLightSystem->m_lightDataBuffer; }
	static uint* GetNumLightsInScene();

private:
	//STRUCTORS
	LightSystem();

	//INTERNAL UPDATE
	void InternalUpdateLightsOnMultilightShader();
	void InternalUpdateShadowCastingLights(Scene* scene);
	
	std::vector<Light3D*> m_lights;
	uint m_numLights;

	SSBO* m_lightDataBuffer;

	static LightSystem* s_theLightSystem;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC inline uint* LightSystem::GetNumLightsInScene() {

	s_theLightSystem->m_numLights = (uint)s_theLightSystem->m_lights.size();
	return (uint*)&s_theLightSystem->m_numLights;
}
