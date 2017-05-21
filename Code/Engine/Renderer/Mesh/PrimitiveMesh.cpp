#include "Engine/Renderer/Mesh/PrimitiveMesh.hpp"
#include "Engine/Renderer/Mesh/Mesh.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CONSTRUCT PRIMITIVES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void PrimitiveMesh::ConstructCubeMesh(Mesh* mesh) {

	const float scale = 1.f;
	std::vector<TexturedVertex_TBNBN> cubeVerts;

	//FRONT FACE
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, scale, scale),
		RGBA::WHITE, Vector2(1.f, 0.f), Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tl
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, scale, -scale),
		RGBA::WHITE, Vector2(1.f, 1.f), Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//bl
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, scale, -scale),
		RGBA::WHITE, Vector2(0.f, 1.f), Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//br
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, scale, scale),
		RGBA::WHITE, Vector2(0.f, 0.f), Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tr

	//BACK FACE
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, -scale, scale),
		RGBA::WHITE, Vector2(0.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tr
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, -scale, -scale),
		RGBA::WHITE, Vector2(0.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//br
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, -scale, -scale),
		RGBA::WHITE, Vector2(1.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//bl
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, -scale, scale),
		RGBA::WHITE, Vector2(1.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tl

	//BOTTOM FACE																					
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, scale, -scale),
		RGBA::WHITE, Vector2(0.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, -1.f, 0.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, -1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	); //tl
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, scale, -scale),
		RGBA::WHITE, Vector2(1.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, -1.f, 0.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, -1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	); //tr
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, -scale, -scale),
		RGBA::WHITE, Vector2(1.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, -1.f, 0.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, -1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	); //br
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, -scale, -scale),
		RGBA::WHITE, Vector2(0.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, -1.f, 0.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, -1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	); //bl

	//TOP FACE																						
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, scale, scale),
		RGBA::WHITE, Vector2(0.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tl
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, -scale, scale),
		RGBA::WHITE, Vector2(0.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//bl
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, -scale, scale),
		RGBA::WHITE, Vector2(1.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//br
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, scale, scale),
		RGBA::WHITE, Vector2(1.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tr

			//RIGHT FACE																					
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, scale, -scale),
		RGBA::WHITE, Vector2(1.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tl
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, scale, scale),
		RGBA::WHITE, Vector2(1.f, 0.f), Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tr
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, -scale, scale),
		RGBA::WHITE, Vector2(0.f, 0.f), Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//br
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, -scale, -scale),
		RGBA::WHITE, Vector2(0.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//bl

	//LEFT FACE																						
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, scale, -scale),
		RGBA::WHITE, Vector2(0.f, 1.f), Vector3(0.f, -1.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(0.f, -1.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tl
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, -scale, -scale),
		RGBA::WHITE, Vector2(1.f, 1.f), Vector3(0.f, -1.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(0.f, -1.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//bl
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, -scale, scale),
		RGBA::WHITE, Vector2(1.f, 0.f), Vector3(0.f, -1.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(0.f, -1.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//br
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, scale, scale),
		RGBA::WHITE, Vector2(0.f, 0.f), Vector3(0.f, -1.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(0.f, -1.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tr

	std::vector<uint16_t> cubeIndices = {
		2, 1, 0, 3, 2, 0,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	std::reverse(cubeIndices.begin(), cubeIndices.end());

	mesh->m_texturedVertsTBNBN = cubeVerts;
	mesh->m_inds = cubeIndices;
	mesh->UpdateMesh();
}


//---------------------------------------------------------------------------------------------------------------------------
void PrimitiveMesh::ConstructSphereMesh(Mesh* mesh, int fidelity) {

	std::vector<TexturedVertex_TBNBN> sphereVerts;
	std::vector<uint16_t> sphereIndices;
	float radius	= 1.f;
	int rings		= fidelity;
	int sectors		= fidelity;
	int curRow		= 0;
	int nextRow		= 0;
	int nextS		= 0;

	const float R = 1.f / (float)(rings - 1);
	const float S = 1.f / (float)(sectors - 1);

	for (int r = 0; r < rings; ++r) {

		for (int s = 0; s < sectors; ++s) {

			const float theta = PI_F * s * S;
			const float phi = PI_F * r * R;
			const float phiPlusHalfPi = PI_HALF_F + phi;

			const float x = sin(2 * theta) * sin(phi);
			const float y = cos(2 * theta) * sin(phi);
			const float z = -sin(PI_HALF_F + phi);

			Vector3 normal = Normalize(Vector3(x, y, z));
			Vector3 tangent = Normalize(CrossProduct(Vector3::UP, normal));
			Vector3 bitangent = Normalize(CrossProduct(normal, tangent));

			sphereVerts.push_back(TexturedVertex_TBNBN(Vector3(x, y, z) * radius, RGBA::WHITE, Vector2(1 - s*S, r*R), tangent, bitangent, normal, Vector4::UNIT_VECTOR_X, IntVector4::ZERO));

			curRow = r			* sectors;
			nextRow = (r + 1)	* sectors;
			nextS = (s + 1) % sectors;

			if (r < rings - 1) {

				sphereIndices.push_back((ushort)nextRow + (ushort)nextS);
				sphereIndices.push_back((ushort)nextRow + (ushort)s);
				sphereIndices.push_back((ushort)curRow + (ushort)s);

				sphereIndices.push_back((ushort)curRow + (ushort)nextS);
				sphereIndices.push_back((ushort)nextRow + (ushort)nextS);
				sphereIndices.push_back((ushort)curRow + (ushort)s);
			}
		}
	}

	std::reverse(sphereIndices.begin(), sphereIndices.end());

	mesh->m_texturedVertsTBNBN = sphereVerts;
	mesh->m_inds = sphereIndices;
	mesh->UpdateMesh();
}


//---------------------------------------------------------------------------------------------------------------------------
void PrimitiveMesh::ConstructQuadMesh(Mesh* mesh) {

	std::vector<TexturedVertex_TBNBN> quadVerts;

	const float SCALE = 1.f;

	//TOP LEFT
	quadVerts.push_back(TexturedVertex_TBNBN(Vector3(-SCALE, SCALE, 0.f),
		RGBA::WHITE, Vector2(1.f, 0.f), Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f),
		CrossProduct(Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);
	//BOTTOM LEFT
	quadVerts.push_back(TexturedVertex_TBNBN(Vector3(-SCALE, -SCALE, 0.f),
		RGBA::WHITE, Vector2(1.f, 1.f), Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f),
		CrossProduct(Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);
	//BOTTOM RIGHT
	quadVerts.push_back(TexturedVertex_TBNBN(Vector3(SCALE, -SCALE, 0.f),
		RGBA::WHITE, Vector2(0.f, 1.f), Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f),
		CrossProduct(Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);
	//TOP RIGHT
	quadVerts.push_back(TexturedVertex_TBNBN(Vector3(SCALE, SCALE, 0.f),
		RGBA::WHITE, Vector2(0.f, 0.f), Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f),
		CrossProduct(Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);

	std::vector<uint16_t> cubeIndices = { 2, 1, 0, 3, 2, 0 };

	mesh->m_texturedVertsTBNBN = quadVerts;
	mesh->m_inds = cubeIndices;
	mesh->UpdateMesh();
}


//---------------------------------------------------------------------------------------------------------------------------
void PrimitiveMesh::ConstructSkybox(Mesh* mesh) {


	const float scale = 1.f;
	std::vector<TexturedVertex_TBNBN> cubeVerts;

	//FRONT FACE
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, scale, scale),
		RGBA::WHITE, Vector2(1.f, 0.f), Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tl

	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, scale, -scale),
		RGBA::WHITE, Vector2(1.f, 1.f), Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//bl
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, scale, -scale),
		RGBA::WHITE, Vector2(0.f, 1.f), Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
		);	//br
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, scale, scale),
		RGBA::WHITE, Vector2(0.f, 0.f), Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(-1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tr

	//BACK FACE
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, -scale, scale),
		RGBA::WHITE, Vector2(0.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tr
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, -scale, -scale),
		RGBA::WHITE, Vector2(0.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//br
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, -scale, -scale),
		RGBA::WHITE, Vector2(1.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//bl
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, -scale, scale),
		RGBA::WHITE, Vector2(1.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tl

	//BOTTOM FACE																					
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, scale, -scale),
		RGBA::WHITE, Vector2(0.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, -1.f, 0.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, -1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
		); //tl
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, scale, -scale),
		RGBA::WHITE, Vector2(1.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, -1.f, 0.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, -1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	); //tr
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, -scale, -scale),
		RGBA::WHITE, Vector2(1.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, -1.f, 0.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, -1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	); //br
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, -scale, -scale),
		RGBA::WHITE, Vector2(0.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, -1.f, 0.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, -1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	); //bl

		   //TOP FACE																						
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, scale, scale),
		RGBA::WHITE, Vector2(0.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tl
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, -scale, scale),
		RGBA::WHITE, Vector2(0.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//bl
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, -scale, scale),
		RGBA::WHITE, Vector2(1.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//br
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, scale, scale),
		RGBA::WHITE, Vector2(1.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f),
		CrossProduct(Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tr

	//RIGHT FACE																					
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, scale, -scale),
		RGBA::WHITE, Vector2(1.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tl
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, scale, scale),
		RGBA::WHITE, Vector2(1.f, 0.f), Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tr
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, -scale, scale),
		RGBA::WHITE, Vector2(0.f, 0.f), Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//br
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(scale, -scale, -scale),
		RGBA::WHITE, Vector2(0.f, 1.f), Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(0.f, 1.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//bl

	//LEFT FACE																						
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, scale, -scale),
		RGBA::WHITE, Vector2(0.f, 1.f), Vector3(0.f, -1.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(0.f, -1.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tl
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, -scale, -scale),
		RGBA::WHITE, Vector2(1.f, 1.f), Vector3(0.f, -1.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(0.f, -1.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//bl
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, -scale, scale),
		RGBA::WHITE, Vector2(1.f, 0.f), Vector3(0.f, -1.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(0.f, -1.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//br
	cubeVerts.push_back(TexturedVertex_TBNBN(Vector3(-scale, scale, scale),
		RGBA::WHITE, Vector2(0.f, 0.f), Vector3(0.f, -1.f, 0.f), Vector3(0.f, 0.f, 1.f),
		CrossProduct(Vector3(0.f, -1.f, 0.f), Vector3(0.f, 0.f, 1.f)), Vector4::UNIT_VECTOR_X, IntVector4::UNIT_VECTOR_X)
	);	//tr

	std::vector<uint16_t> cubeIndices = {
		2, 1, 0, 3, 2, 0,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	mesh->m_texturedVertsTBNBN = cubeVerts;
	mesh->m_inds = cubeIndices;
	mesh->UpdateMesh();
}