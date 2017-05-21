#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/Matrix4.hpp"
#include "Engine/Math/Objects/GeomObject.hpp"

class Model;
class Material;

class ClickableObject {
public:
	//STRUCTORS
	ClickableObject(eGeomType geomType, const Vector3& pos);
	~ClickableObject();

	//UPDATE RENDER
	virtual void Update(float deltaSeconds);
	virtual void Render() const;

	float CalcDistFromCamera() const;

	static void SortByDistanceToCamera();

	bool operator==(const ClickableObject& other) { return m_id == other.m_id; }
	bool operator!=(const ClickableObject& other) { return m_id != other.m_id; }



	bool		m_isSelected	= false;
	uint32_t	m_id			= 0;

	Material*	m_outlineMat	= nullptr;
	Vector3		m_viewDir		= Vector3::ZERO;
	Matrix4		m_model			= Matrix4::IDENTITY;
	Vector3		m_pos			= Vector3::ZERO;

	GeomObject* m_geom			= nullptr;


	static std::vector<ClickableObject*> s_sceneClickableObjs;
	static uint32_t s_id;
};