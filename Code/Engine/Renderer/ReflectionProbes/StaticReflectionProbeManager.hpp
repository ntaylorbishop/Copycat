#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

class StaticReflectionProbe;
class Scene;

class StaticReflectionProbeManager {
public:
	//GET
	static StaticReflectionProbeManager* Get();

	//LOAD
	size_t LoadProbesInDirectory(const String& dir, Scene* sceneToLoadInTo);

	//BINDING
	void BindProbesToShader(const String& shaderName);

private:
	//STRUCTORS
	StaticReflectionProbeManager();
	~StaticReflectionProbeManager();
	static void Shutdown();


	std::vector<StaticReflectionProbe*> m_sProbes;
	uint m_numProbes = 0;

	static StaticReflectionProbeManager* s_pManager;

	friend class StaticReflectionProbe;
};