#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

#include "Engine/Utils/FileUtils/FileBinaryWriter.hpp"
#include "Engine/Utils/FileUtils/FileBinaryReader.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/General/RenderCommon.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Engine/Renderer/Mesh/Mesh.hpp"
#include "ThirdParty/MikkT/mikktspace.h"

class MeshBuilder {
public:
	//STRUCTORS
	MeshBuilder();
	~MeshBuilder();

	//BUILDING
	void AddMesh();
	void AddMaterialName(const String& name);
	void AddPosition(const Vector3& pos) { m_meshPositions.push_back(pos); }
	void AddVertex(TexturedVertex_TBNBN vert);
	void AddInd(uint16_t ind);
	void CloseMesh();

	//READ WRITE
	static void WriteMeshToFile(MeshID meshId, const String& matName, const String& path, const String& filename);
	static MeshID ReadMeshFromFile(const String& path, const String& filename, String* outMatName);
	static MeshID ReadMeshFromFile(const String& pathAndFilename, String* outMatName);

	//GETTERS SETTERS
	Mesh*	GetMasterMesh();
	MeshID	GetMeshAtIndex(uint idx)			{ return m_meshes[idx];				}
	String	GetMaterialNameAtIndex(uint idx)	{ return m_meshMaterialNames[idx];	}
	Vector3 GetPositionOfMesh(uint idx)			{ return m_meshPositions[idx];		}
	uint	GetNumMeshes() const				{ return (uint)m_meshes.size();		}


	void DontBuildMikkt() { m_isUsingMikkt = false; }

private:
	Mesh* m_currMesh;
	MeshID m_currMeshID = 0;

	std::vector<MeshID> m_meshes;
	std::vector<String> m_meshMaterialNames;
	std::vector<Vector3> m_meshPositions;
	Mesh* m_masterMesh;

	bool m_isUsingMikkt;

	FileBinaryWriter m_fileWriter;
	FileBinaryReader m_fileReader;

	//MIKKT
	SMikkTSpaceInterface* m_mikktInterface;
	SMikkTSpaceContext* m_mikktContext;
};