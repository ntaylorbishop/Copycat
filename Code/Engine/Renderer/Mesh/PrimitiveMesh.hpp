#pragma once

class Mesh;

class PrimitiveMesh {
private:
	//CONSTRUCT PRIMITIVES
	static void ConstructCubeMesh(Mesh* mesh);
	static void ConstructSphereMesh(Mesh* mesh, int fidelity);
	static void ConstructQuadMesh(Mesh* mesh);
	static void ConstructSkybox(Mesh* mesh);

	friend class BeirusMeshCollection;
};