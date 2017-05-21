#pragma once

#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Renderer/General/RGBA.hpp"

//Only supporting point lights for now

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ENUMS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
enum LightType {
	LIGHT_POINT = 0,
	LIGHT_DIRECTIONAL,
	LIGHT_CONE,
	NUM_LIGHTS
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LIGHT3D CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
class Light3D {
public:
	//STRUCTORS
	inline Light3D();
	inline Light3D(const Vector3& pos, const RGBA& color, float radius, float intensity = 1.f);
	virtual ~Light3D() { }

	//UPDATE
	virtual void RenderShadowMaps(float deltaSeconds) { UNUSED(deltaSeconds); }

	//GETTERS SETTERS
	Vector3 GetPosition() const						{ return m_position;			}
	RGBA    GetColor() const						{ return m_color;				}
	float   GetMinLightDistance() const				{ return m_minLightDistance;	}
	float   GetMaxLightDistance() const				{ return m_maxLightDistance;	}
	float   GetPowerAtMin() const					{ return m_powerAtMin;			}
	float   GetPowerAtMax() const					{ return m_powerAtMax;			}

	void    SetPosition(const Vector3& position)	{ m_position = position;										}
	void    SetColor(const RGBA& color)				{ m_color = color;												}
	void    SetMinLightDistance(float minLightDist) { m_minLightDistance = minLightDist;							}
	void    SetMaxLightDistance(float maxLightDist) { m_maxLightDistance = maxLightDist;							}
	void    SetPowerAtMin(float minPower)			{ m_powerAtMin = minPower;										}
	void    SetPowerAtMax(float maxPower)			{ m_powerAtMax = maxPower;										}
	void    SetRadius(float radius)					{ m_minLightDistance = -radius; m_maxLightDistance = radius;	}

	virtual bool IsShadowCasting()					{ return false; }



	Vector3	m_position			= Vector3::ZERO;
	RGBA	m_color				= RGBA::WHITE;
	float	m_minLightDistance	= 0.f;
	float	m_maxLightDistance	= 0.f;
	float	m_powerAtMin		= 0.f;
	float	m_powerAtMax		= 0.f;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
INLINE Light3D::Light3D()
	: m_position(Vector3(0.f, 0.f, 0.f))
	, m_color(RGBA::WHITE)
	, m_minLightDistance(0.f)
	, m_maxLightDistance(0.f)
	, m_powerAtMin(0.f)
	, m_powerAtMax(0.f)
{ }


//---------------------------------------------------------------------------------------------------------------------------
INLINE Light3D::Light3D(const Vector3& pos, const RGBA& color, float radius, float intensity)
	: m_position(pos)
	, m_color(color)
	, m_minLightDistance(radius - 1.f)
	, m_maxLightDistance(radius + 1.f)
	, m_powerAtMin(intensity)
	, m_powerAtMax(0.f)
{ }