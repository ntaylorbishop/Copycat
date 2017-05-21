#include "Engine/Math/Objects/Sphere.hpp"
#include "Engine/Math/Objects/AABB3.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CREATION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
GeomObject* CreateGeomType(eGeomType type) {

	switch (type) {
	case GEOM_TYPE_AABB3:
		return new AABB3();
	case GEOM_TYPE_SPHERE:
		return new Sphere();
	default:
		ERROR_AND_DIE("This shouldn't have happened.");
	}
}