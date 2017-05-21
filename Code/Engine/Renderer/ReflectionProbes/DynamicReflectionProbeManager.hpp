#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

const uint8_t MAX_NUM_DYNAMIC_REFLECTION_PROBES = 128;

class Scene;
class DynamicReflectionProbe;

class DynamicReflectionProbeManager {
public:
	//GET
	static DynamicReflectionProbeManager* Get();

	//UPDATE
	void UpdateProbes(Scene* sceneToRender);
	void RenderOutlines();

private:
	//STRUCTORS DESTROY
	DynamicReflectionProbeManager();
	~DynamicReflectionProbeManager();

	void AttachReflectionProbe(DynamicReflectionProbe* newProbe);
	void DetachReflectionProbe(DynamicReflectionProbe* probe);

	static void Destroy();

	std::vector<DynamicReflectionProbe*> m_probes;
	size_t m_currRendering = 0;

	static DynamicReflectionProbeManager* s_DRPManager;

	friend class DynamicReflectionProbe;
};