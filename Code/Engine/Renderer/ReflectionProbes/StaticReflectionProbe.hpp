#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Scene/ClickableObject.hpp"

class TextureCubemap;
class Model;
class Material;
class Scene;

class StaticReflectionProbe : public ClickableObject {
public:
	//CREATE DESTROY
	static StaticReflectionProbe* Load(const String& probeFile, Scene* sceneToLoadInTo);

	//UPDATE
	void Update(float deltaSeconds);
	
	//RENDER
	void Render() const;

	//GETTERS
	TextureCubemap* GetEnvironmentMap() { return m_envMap; }


	Vector3			m_position	= Vector3::ZERO;
	Vector3			m_boxMins	= Vector3::ZERO;
	Vector3			m_boxMaxs	= Vector3::ZERO;

private:
	StaticReflectionProbe(const Vector3& position, Vector3 mins, Vector3 maxs, Scene* sceneToLoadInTo, TextureCubemap* envMap);
	~StaticReflectionProbe();


	TextureCubemap* m_envMap	= nullptr;

	Model*			m_model = nullptr;
	Material*		m_material = nullptr;


	friend class StaticReflectionProbeManager;
};