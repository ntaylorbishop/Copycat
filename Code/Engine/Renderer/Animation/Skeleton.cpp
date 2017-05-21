#include "Engine/Renderer/Animation/Skeleton.hpp"
#include "Engine/General/Core/TheMemoryManager.hpp"

Skeleton::Skeleton()
	: m_names()
	, m_parentIndices()
	, m_modelToBoneSpace()
	, m_boneToModelSpace()
{ }


int Skeleton::GetLastAddedJointIndex() const {
	return (int)m_names.size() - 1;
}
void Skeleton::AddJoint(const char* str, int parentJointIdx, Matrix4 initialBoneToModelMatrix) {
	m_names.push_back(str);
	m_parentIndices.push_back(parentJointIdx);

	//FIX THIS
	m_modelToBoneSpace.push_back(initialBoneToModelMatrix.Inverse());
	m_boneToModelSpace.push_back(initialBoneToModelMatrix);
}

Vector3 Skeleton::GetJointPosition(int idx) const {
	return m_boneToModelSpace[idx].GetPosition();
}

bool Skeleton::GetBonePositions(int idx, Vector3* out_p0, Vector3* out_p1) const {
	int parentIdx = m_parentIndices[idx];

	if (parentIdx == -1) {
		*out_p0 = GetJointPosition(idx);
		*out_p1 = GetJointPosition(idx);
	}
	else {
		*out_p0 = GetJointPosition(parentIdx);
		*out_p1 = GetJointPosition(idx);
	}

	return true;
}

void Skeleton::Visualize() const {
	for (unsigned int i = 0; i < m_boneToModelSpace.size(); i++) {
		if (m_parentIndices[i] == -1) {
			//g_theDebugRenderer->DrawDebugSphere(GetJointPosition(i), 0.1f, RGBA::BLUE, DEBUG_DRAW_DEPTH_TEST_DUAL);
		}
		else {
			//g_theDebugRenderer->DrawDebugSphere(GetJointPosition(i), 0.02f, RGBA::YELLOW, DEBUG_DRAW_DEPTH_TEST_DUAL);
		}
	}

	std::vector<Vector3> jointPositions;
	for (unsigned int i = 0; i < m_boneToModelSpace.size(); i++) {
		jointPositions.push_back(GetJointPosition(i));
	}

	std::vector<std::pair<Vector3, Vector3>> bones;
	Vector3 start;
	Vector3 end;
	for (unsigned int i = 0; i < m_names.size() - 1; i++) {
		GetBonePositions(i, &start, &end);
		bones.push_back(std::pair<Vector3, Vector3>(start, end));
	}

	for (unsigned int i = 0; i < bones.size(); i++) {
		//g_theDebugRenderer->DrawDebugLine(bones[i].first, bones[i].second, RGBA::BLUE, DEBUG_DRAW_DEPTH_TEST_DUAL);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
//GET JOINT
//---------------------------------------------------------------------------------------------------------------------------
int Skeleton::FindJointIndexByName(const String& name) {
	int idx = -1;

	for (unsigned int i = 0; i < m_names.size(); i++) {
		if (m_names[i] == name) {
			idx = i;
		}
	}

	return idx;
}
Joint* Skeleton::GetJointByIndex(int idx) {
	ASSERT_OR_DIE(idx >= 0 && (uint)idx < m_names.size(), "ERROR: JOINT IDX OUT OF BOUNDS");

	return new(MEMORY_RENDERING) Joint(m_names[idx], m_parentIndices[idx], m_boneToModelSpace[idx]);
}

//---------------------------------------------------------------------------------------------------------------------------
//READ WRITE
//---------------------------------------------------------------------------------------------------------------------------
STATIC void Skeleton::WriteSkeletonToFile(Skeleton* skeleton, const String& filename) {
	FileBinaryWriter fileWriter;
	fileWriter.Open(filename);

	//FILE LAYOUT
	//Num joints
	//Joint names
	//Parent indices
	//Initial model space

	fileWriter.WriteInt((int)skeleton->m_names.size());

	//Joint names
	for (unsigned int i = 0; i < skeleton->m_names.size(); i++) {
		fileWriter.WriteString(skeleton->m_names[i].c_str());
	}

	//Parent indices
	for (unsigned int i = 0; i < skeleton->m_parentIndices.size(); i++) {
		fileWriter.WriteInt(skeleton->m_parentIndices[i]);
	}

	//Transforms
	for (unsigned int i = 0; i < skeleton->m_boneToModelSpace.size(); i++) {
		fileWriter.WriteMatrix(skeleton->m_boneToModelSpace[i]);
	}

	fileWriter.Close();
}
STATIC Skeleton* Skeleton::ReadSkeletonFromFile(const String& filename) {
	Skeleton* newSkeleton = nullptr;
	FileBinaryReader fileReader;
	fileReader.Open(filename);

	//Num joints
	int numJoints = 0;
	fileReader.ReadInt(&numJoints);

	std::vector<String> names;
	std::vector<int> parentIndices;
	std::vector<Matrix4> boneToModelMatrices;
	std::vector<Matrix4> modelToBoneMatrices;
	names.reserve(numJoints + 1);
	parentIndices.reserve(numJoints + 1);
	boneToModelMatrices.reserve(numJoints + 1);
	modelToBoneMatrices.reserve(numJoints + 1);


	//Joint names
	for (int i = 0; i < numJoints; i++) {
		String str = "";
		fileReader.ReadString(&str);
		names.push_back(str);
	}

	//Parent indices
	for (int i = 0; i < numJoints; i++) {
		int ind = 0;
		fileReader.ReadInt(&ind);
		parentIndices.push_back(ind);
	}

	//Transforms
	for (int i = 0; i < numJoints; i++) {
		Matrix4 mat = Matrix4::IDENTITY;
		fileReader.ReadMatrix(&mat);
		boneToModelMatrices.push_back(mat);
	}
	
	//Invert the transforms
	for (int i = 0; i < numJoints; i++) {
		modelToBoneMatrices.push_back(boneToModelMatrices[i].Inverse());
	}

	newSkeleton = new(MEMORY_RENDERING) Skeleton();
	newSkeleton->m_names = names;
	newSkeleton->m_boneToModelSpace = boneToModelMatrices;
	newSkeleton->m_parentIndices = parentIndices;
	newSkeleton->m_modelToBoneSpace = modelToBoneMatrices;

	return newSkeleton;
}

void Skeleton::SetJointWorldTransform(int jointIdx, const Matrix4& newModel) {
	m_boneToModelSpace[jointIdx] = newModel;
}