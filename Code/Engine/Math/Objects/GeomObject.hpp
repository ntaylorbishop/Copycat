#pragma once

enum eGeomType {
	GEOM_TYPE_SPHERE = 0,
	GEOM_TYPE_AABB3
};

class GeomObject {
public:
	GeomObject(eGeomType type) : m_type(type) { }

	eGeomType GetType() const { return m_type; }

private:
	eGeomType m_type;
};

GeomObject* CreateGeomType(eGeomType type);