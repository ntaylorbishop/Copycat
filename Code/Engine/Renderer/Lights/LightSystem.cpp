#include "Engine/Renderer/Lights/LightSystem.hpp"
#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/Renderer/Shaders/ShaderProgram.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/Cameras/PlayerCamera3D.hpp"
#include "Engine/Renderer/Structures/SSBO.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Engine/Renderer/Lights/ShadowCastingLight3D.hpp"
#include "Engine/General/Time/Time.hpp"
#include "Engine/Console/Command.hpp"
#include "Engine/Console/Console.hpp"

STATIC LightSystem* LightSystem::s_theLightSystem = nullptr;

RGBA LIGHTS_AMBIENT_LIGHT = RGBA(1.f, 1.f, 1.f, 0.5f);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STATIC INIT SHUTDOWN
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void LightSystem::Initialize() {

	ASSERT_OR_DIE(s_theLightSystem == nullptr, "ERROR: LightSystem already initialized.");
	s_theLightSystem = new LightSystem();
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void LightSystem::Shutdown() {

	ASSERT_OR_DIE(s_theLightSystem != nullptr, "ERROR: LightSystem already shutdown.");
	delete s_theLightSystem;
	s_theLightSystem = nullptr;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STATIC UPDATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ADD REMOVE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC uint LightSystem::AddLight(Light3D* light) {

	s_theLightSystem->m_lights.push_back(light);
	s_theLightSystem->m_numLights++;
	return (uint)s_theLightSystem->m_lights.size() - 1;
}

//---------------------------------------------------------------------------------------------------------------------------
STATIC void LightSystem::ClearLights() {

	s_theLightSystem->m_lights.clear();
	s_theLightSystem->m_numLights = 0;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PRIVATE STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC LightSystem::LightSystem() 
	: m_lightDataBuffer(new SSBO())
	, m_numLights(0)
{
	m_lightDataBuffer->Generate(MAX_NUM_LIGHTS * sizeof(Light3D));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INTERNAL UPDATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void LightSystem::InternalUpdateLightsOnMultilightShader() {

	std::vector<Light3D*>& lights = s_theLightSystem->m_lights;

	if (lights.empty()) {
		return;
	}

	ShaderProgram* multiLightProg = ShaderProgram::CreateOrGetShaderProgram("BlinnPhong");

	//WRITE OUT GENERAL INFO
	Uniform* gCameraPosition	= multiLightProg->GetUniform("gCameraPosition");
	Uniform* gView				= multiLightProg->GetUniform("gView");
	Uniform* gProj				= multiLightProg->GetUniform("gProj");
	Uniform* gAmbientLight		= multiLightProg->GetUniform("gAmbientLight");

	gCameraPosition	->SetData((void*)&BeirusRenderer::GetPlayerCamera()->m_position);
	gView			->SetData((void*)BeirusRenderer::GetViewMatrix());
	gProj			->SetData((void*)BeirusRenderer::GetProjectionMatrix());
	gAmbientLight	->SetData((void*)&LIGHTS_AMBIENT_LIGHT);

	//WRITE OUT LIGHT DATA
	for (uint i = 0; i < m_numLights; i++) {
		m_lightDataBuffer->Write<Vector3>(lights[i]->GetPosition());
		m_lightDataBuffer->Write<RGBA>(lights[i]->GetColor());
		m_lightDataBuffer->Write<float>(lights[i]->GetMinLightDistance());
		m_lightDataBuffer->Write<float>(lights[i]->GetMaxLightDistance());
		m_lightDataBuffer->Write<float>(lights[i]->GetPowerAtMin());
		m_lightDataBuffer->Write<float>(lights[i]->GetPowerAtMax());
	}

	//BIND
	m_lightDataBuffer->BindDataToGPU();
	m_lightDataBuffer->ResetHead();
}


//---------------------------------------------------------------------------------------------------------------------------
void LightSystem::InternalUpdateShadowCastingLights(Scene* scene) {

	for (uint i = 0; i < m_numLights; i++) {
		
		if (m_lights[i]->IsShadowCasting()) {
			ShadowCastingLight3D* lightToCastShadow = (ShadowCastingLight3D*)m_lights[i];
			lightToCastShadow->RenderShadowMaps(scene);
		}
	}
}