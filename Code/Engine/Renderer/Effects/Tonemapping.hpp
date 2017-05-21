#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/General/RenderCommon.hpp"
#include "Engine/Math/Objects/AABB3.hpp"

class TextureBuffer;
class Material;
class Framebuffer;

const float MIN_EXPOSURE	= 1.6f;
const float MAX_EXPOSURE	= 6.f;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//EXPOSURE VOLUME
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
struct ExposureVolume {
	ExposureVolume();
	ExposureVolume(const AABB3& vol, float exposure) 
		: m_volume(vol)
		, m_exposureVal(exposure)
	{ }

	AABB3 m_volume		= AABB3();
	float m_exposureVal = 0.f;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TONEMAPPING PASS CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
class Tonemapping {
public:
	//GET
	static Tonemapping* Get();

	//UPDATE RENDER
	void Update(float deltaSeconds);
	void RunPass();

	//ADD VOLUMES
	void AddExposureVolume(const AABB3& volume, float exposureVal);
	void SetMinExposure(float minExposure)			{ m_minExposure			= minExposure;	}
	void SetMaxExposure(float maxExposure)			{ m_maxExposure			= maxExposure;	}
	void SetExposureChangeRate(float changeRate)	{ m_exposureChangeRate	= changeRate;	}
	void SetDefaultExposure(float defExp)			{ m_defaultExposure		= defExp;		}

	void EnableDebugVisualizer()					{ m_debugVisualizer = true;				}
	void DisableDebugVisualizer()					{ m_debugVisualizer = false;			}

	void ToggleExposureVolumes(bool enabled)		{ m_exposureVolumesEnabled = enabled;	}

private:
	//STRUCTORS
	Tonemapping();
	~Tonemapping();

	static void Shutdown();


	TextureBuffer*	m_colorTarget			= nullptr;
	Material*		m_tonemappingMat		= nullptr;
	Framebuffer*	m_tonemapFBO			= nullptr;

	float			m_minExposure			= 0.f;
	float			m_maxExposure			= 0.f;
	float			m_exposureChangeRate	= 0.f;

	float			m_exposure				= 0.f;
	float			m_targetExposure		= 1.f;
	float			m_defaultExposure		= 1.f;

	MeshID			m_fullScreenMesh		= 0;
	bool			m_debugDraw				= false;
	bool			m_debugVisualizer		= false;
	bool			m_exposureVolumesEnabled = true;
	std::vector<ExposureVolume> m_exposureVolumes;

	static Tonemapping* s_HDR;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline void Tonemapping::AddExposureVolume(const AABB3& volume, float exposureVal) {
	m_exposureVolumes.push_back(ExposureVolume(volume, exposureVal)); 
}
