#include "Engine/Scene/PickRay.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Math/Ray.hpp"


//---------------------------------------------------------------------------------------------------------------------------
static Vector3 GetScreenSpacePositionInWorldSpace(const Vector2& screenSpacePos) {

	Vector2 screenSize = *BeirusRenderer::GetScreenSize();
	Vector2 mousePosNorm = Vector2(screenSpacePos.x / screenSize.x, screenSpacePos.y / screenSize.y);

	Vector3 mouse_NDC = Vector3(mousePosNorm * Vector2(2.f) - Vector2(1.f), 0.f);

	Matrix4 view = *BeirusRenderer::GetViewMatrix();
	Matrix4 proj = *BeirusRenderer::GetProjectionMatrix();

	Matrix4 viewProj = view * proj;

	Matrix4 viewProjInv = viewProj.Inverse();

	Vector4 mouse_Clip = Vector4(mouse_NDC, 1.f) * viewProjInv;
	Vector3 mouse_WS = Vector3(mouse_Clip.x / mouse_Clip.w, mouse_Clip.y / mouse_Clip.w, mouse_Clip.z / mouse_Clip.w);

	return mouse_WS;
}


//---------------------------------------------------------------------------------------------------------------------------
void CollectAllIntersectedObjects(const Ray& ray, std::vector<ClickableObject*>& intersectedObjects) {

	ClickableObject::SortByDistanceToCamera();

	std::vector<ClickableObject*>& allObjs = ClickableObject::s_sceneClickableObjs;

	for (uint32_t i = 0; i < allObjs.size(); i++) {

		if (allObjs[i]->m_geom->GetType() == GEOM_TYPE_SPHERE) {
			if (GetRayIntersectionWithSphere(ray, *(Sphere*)allObjs[i]->m_geom)) {
				intersectedObjects.push_back(allObjs[i]);
			}
		}
		else if (allObjs[i]->m_geom->GetType() == GEOM_TYPE_AABB3) {
			if (GetRayIntersectionWithAABB3(ray, *(AABB3*)allObjs[i]->m_geom)) {
				intersectedObjects.push_back(allObjs[i]);
			}
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void CreatePickRay(std::vector<ClickableObject*>& intersectedObjects, Vector3& start, Vector3& end) {

	Vector2 mousePos = InputSystem::GetRelativeCursorPosition().ToVector2();
	Vector3 mouseWS = GetScreenSpacePositionInWorldSpace(mousePos);

	Vector3 dir = mouseWS - BeirusRenderer::GetPlayerCamera()->m_position;
	dir.Normalize();

	start = mouseWS;	
	end = mouseWS + 10.f * dir;

	Ray r(start, dir);

	CollectAllIntersectedObjects(r, intersectedObjects);
}