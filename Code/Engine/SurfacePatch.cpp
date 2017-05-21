#include "Engine/SurfacePatch.hpp"
#include "Engine/General/Core/TheMemoryManager.hpp"

Vector3 SinSquared(float x, float y) {
	return Vector3(x, y, sin(x * y));
}
Vector3 MyFunc1(float x, float y) {
	return Vector3(x, y, sin(x + y));
}

Vector3 SurfacePatch::PlaneFunc(void const *data, float x, float y) {
	Plane* plane = (Plane*)data;
	return plane->initialPosition + x * plane->right + y * plane->up;
}

Mesh* SurfacePatch::MakePlane(Vector3 initialPosition, Vector3 right, Vector3 up, Vector2 start, Vector2 end, IntVector2 sectionCount) {
	ASSERT_OR_DIE(sectionCount.x > 0, "Section Count must be greater than 0");
	ASSERT_OR_DIE(sectionCount.y > 0, "Section Count must be greater than 0");

	Mesh* mesh = new(MEMORY_RENDERING) Mesh(VertexDefinition::TEXTUREDVERTEX_TBN);
	std::vector<TexturedVertex_TBN> texturedVertsTBN;
	std::vector<uint16_t> inds;

	IntVector2 vertexCount = sectionCount + 1;

	float xRange = end.x - start.x;
	float yRange = end.y - start.y;
	float xStep = xRange / (float)sectionCount.x;
	float yStep = yRange / (float)sectionCount.y;

	float uStep = 1.f / (float)sectionCount.x;
	float vStep = 1.f / (float)sectionCount.y;

	Vector3 tangent = right;
	Vector3 bitangent = up;
	Vector3 normal = CrossProduct(up, right);
	tangent.Normalize();
	bitangent.Normalize();
	normal.Normalize();

	float x, y;
	float u, v;

	y = start.y;
	v = 0.f;

	//ADD VERTS
	for (int iy = 0; iy < vertexCount.y; iy++) {
		x = start.x;
		u = 0.f;

		for (int ix = 0; ix < vertexCount.x; ix++) {
			TexturedVertex_TBN vert = TexturedVertex_TBN();

			vert.m_texCoords = Vector2(u, v);
			vert.m_position = initialPosition + x * right + y * up;
			vert.m_tangent = tangent;
			vert.m_bitangent = bitangent;

			texturedVertsTBN.push_back(vert);

			x += xStep;
			u += uStep;
		}

		y += yStep;
		v += vStep;
	}

	//ADD INDS
	for (int iy = 0; iy < sectionCount.y; iy++) {
		for (int ix = 0; ix < sectionCount.x; ix++) {

			uint16_t idx_bl = ((uint16_t)iy * (uint16_t)vertexCount.x) + (uint16_t)ix;
			uint16_t idx_br = idx_bl + 1;
			uint16_t idx_tl = idx_bl + (uint16_t)vertexCount.x;
			uint16_t idx_tr = idx_tl + 1;

			inds.push_back((uint16_t)idx_bl);
			inds.push_back((uint16_t)idx_br);
			inds.push_back((uint16_t)idx_tl);
						 
			inds.push_back((uint16_t)idx_tr);
			inds.push_back((uint16_t)idx_tl);
			inds.push_back((uint16_t)idx_br);
		}
	}

	mesh->UpdateMesh(texturedVertsTBN, inds);
	return mesh;
}
Mesh* SurfacePatch::MakePatch(PatchFunction* patchFunctionPtr, Vector3 initialPosition, Vector3 right, Vector3 up, Vector2 start, Vector2 end, IntVector2 sectionCount) {
	ASSERT_OR_DIE(sectionCount.x > 0, "Section Count must be greater than 0");
	ASSERT_OR_DIE(sectionCount.y > 0, "Section Count must be greater than 0");

	UNREFERENCED_PARAMETER(initialPosition);

	Mesh* mesh = new(MEMORY_RENDERING) Mesh(VertexDefinition::TEXTUREDVERTEX_TBN);
	std::vector<TexturedVertex_TBN> texturedVertsTBN;
	std::vector<uint16_t> inds;

	IntVector2 vertexCount = sectionCount + 1;

	float xRange = end.x - start.x;
	float yRange = end.y - start.y;
	float xStep = xRange / (float)sectionCount.x;
	float yStep = yRange / (float)sectionCount.y;

	float uStep = 1.f / (float)sectionCount.x;
	float vStep = 1.f / (float)sectionCount.y;

	Vector3 tangent = right;
	Vector3 bitangent = up;
	Vector3 normal = CrossProduct(up, right);
	tangent.Normalize();
	bitangent.Normalize();
	normal.Normalize();

	float x, y;
	float u, v;

	y = start.y;
	v = 0.f;

	for (int iy = 0; iy < vertexCount.y; iy++) {
		x = start.y;
		u = 0.f;

		for (int ix = 0; ix < vertexCount.x; ix++) {
			TexturedVertex_TBN vert = TexturedVertex_TBN();

			vert.m_texCoords = Vector2(u, v);

			float const delta = 0.001f;

			vert.m_tangent = patchFunctionPtr(x + delta, y) - patchFunctionPtr(x - delta, y);
			vert.m_bitangent = patchFunctionPtr(x, y + delta) - patchFunctionPtr(x, y - delta);
			vert.m_tangent.Normalize();
			vert.m_bitangent.Normalize();
			vert.m_position = patchFunctionPtr(x, y);

			texturedVertsTBN.push_back(vert);
			x += xStep;
			u += uStep;
		}

		y += yStep;
		v += vStep;
	}

	//Add indices
	for (int iy = 0; iy < sectionCount.y; iy++) {
		for (int ix = 0; ix < sectionCount.x; ix++) {

			uint16_t bl_idx = ((uint16_t)iy * (uint16_t)vertexCount.x) + (uint16_t)ix;
			uint16_t br_idx = bl_idx + 1;
			uint16_t tl_idx = bl_idx + (uint16_t)vertexCount.x;
			uint16_t tr_idx = tl_idx + 1;

			inds.push_back(bl_idx);
			inds.push_back(br_idx);
			inds.push_back(tl_idx);

			inds.push_back(tr_idx);
			inds.push_back(tl_idx);
			inds.push_back(br_idx);
		}
	}

	mesh->UpdateMesh(texturedVertsTBN, inds);
	return mesh;
}