#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Math/Matrix4.hpp"
#include "Engine/Utils/FileUtils/FileBinaryReader.hpp"
#include "Engine/Utils/FileUtils/FileBinaryWriter.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//JOINT STRUCT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
struct Joint {
	Joint(String name, int parentIdx, Matrix4 transform)
		: m_name(name)
		, m_parentIdx(parentIdx)
		, m_transform(transform)
	{ }
	
	String	m_name		= "NULL";
	int		m_parentIdx = 0;
	Matrix4 m_transform = Matrix4::IDENTITY;
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SKELETON CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
class Skeleton {
public:
	//STRUCTORS
	Skeleton();

	int		GetLastAddedJointIndex() const;
	void	AddJoint(const char* str, int parentJointIdx, Matrix4 initialBoneToModelMatrix);

	Vector3 GetJointPosition(int idx) const;
	bool	GetBonePositions(int idx, Vector3* out_p0, Vector3* out_p1) const;

	int		FindJointIndexByName(const String& name);
	Joint*	GetJointByIndex(int idx);
	size_t	GetJointCount() { return m_names.size(); }

	void	SetJointWorldTransform(int jointIdx, const Matrix4& newModel);


	void Visualize() const;

	//READ WRITE
	static void			WriteSkeletonToFile(Skeleton* skeleton, const String& filename);
	static Skeleton*	ReadSkeletonFromFile(const String& filename);

	std::vector<Matrix4> m_boneToModelSpace;
	std::vector<Matrix4> m_modelToBoneSpace;

private:
	std::vector<String> m_names;
	std::vector<int> m_parentIndices;
};