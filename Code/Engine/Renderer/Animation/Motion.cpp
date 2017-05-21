#include "Engine/Renderer/Animation/Motion.hpp"
#include "Engine/General/Core/TheMemoryManager.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
Motion::Motion(String name, float timeSpan, float framerate)
	: m_name(name)
	, m_totalLengthSeconds(timeSpan)
	, m_skeleton(nullptr)
	, m_playbackType(PLAYBACK_CLAMP)
{
	m_frameCount = (uint32_t)ceil(framerate * timeSpan) + 1;
	m_frameRate = framerate;
	m_frameTime = 1.f / framerate;
	m_totalLengthSeconds = timeSpan;
}
Motion::Motion(String name, float timeSpan, float framerate, Skeleton* skeleton)
	: m_name(name)
	, m_totalLengthSeconds(timeSpan)
	, m_skeleton(skeleton)
	, m_playbackType(PLAYBACK_CLAMP)
{
	ASSERT_OR_DIE(nullptr != skeleton, "ERROR: CANNOT CONSTRUCT MOTION WITH NULL SKELETON");

	m_frameCount = (uint32_t)ceil(framerate * timeSpan) + 1;
	m_frameRate = framerate;
	m_frameTime = 1.f / framerate;
	m_totalLengthSeconds = timeSpan;

	m_keyframes.resize(m_skeleton->GetJointCount());

	for (unsigned int i = 0; i < m_keyframes.size(); i++) {
		m_keyframes[i].resize(m_frameCount);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
void Motion::GetFrameIndicesWithBlend(uint32_t& outFrameIdx0, uint32_t& outFrameIdx1, float& outBlend, float inTime) {
	uint32_t frameIdx0 = (uint32_t)floor(inTime / m_frameTime);
	uint32_t frameIdx1 = frameIdx0 + 1;

	if (frameIdx0 == (m_frameCount - 1)) {
		frameIdx1 = m_frameCount - 1;
		outBlend = 0.f;
	}
	else if (frameIdx0 == (m_frameCount - 2)) {
		float lastFrameTime = m_totalLengthSeconds - (m_frameTime * frameIdx0);
		outBlend = fmodf(inTime, m_frameTime) / lastFrameTime;

		outBlend = Clamp<float>(outBlend, 0.f, 1.f);
	}
	else {
		outBlend = fmodf(inTime, m_frameTime) / m_frameTime;
	}

	outFrameIdx0 = frameIdx0;
	outFrameIdx1 = frameIdx1;
}
//---------------------------------------------------------------------------------------------------------------------------
void Motion::ApplyMotionToSkeleton(Skeleton* skeleton, float time) {
	uint32_t frame0;
	uint32_t frame1;
	float blend;

	switch (m_playbackType) {
	case PLAYBACK_CLAMP:
		if (time > m_totalLengthSeconds) {
			time = m_totalLengthSeconds;
		}
		break;
	case PLAYBACK_LOOP:
		if (time > m_totalLengthSeconds) {
			time = fmodf(time, m_totalLengthSeconds);
		}
	case PLAYBACK_PINGPONG:
		if (time > m_totalLengthSeconds) {
			float whereInAnim = fmodf(time, m_totalLengthSeconds * 2.f) - m_totalLengthSeconds;
			if (whereInAnim > 0.f) {
				time = fmodf(time, m_totalLengthSeconds);
			}
			else if (whereInAnim < 0.f) {
				if (time > m_totalLengthSeconds) {
					time = m_totalLengthSeconds - fmodf(time, m_totalLengthSeconds);
				}
			}
		}
		break;
	}

	GetFrameIndicesWithBlend(frame0, frame1, blend, time);

	int numJoints = (int)skeleton->GetJointCount();
	for (int jointIdx = 0; jointIdx < numJoints; jointIdx++) {
		std::vector<Matrix4>* jointKeyframes = GetJointKeyframes(jointIdx);

		Matrix4 mat0 = jointKeyframes->at(frame0);
		Matrix4 mat1 = jointKeyframes->at(frame1);

		Matrix4 newModel = Matrix4::MatrixLerp(mat0, mat1, blend);
		skeleton->SetJointWorldTransform(jointIdx, newModel);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
std::vector<Matrix4>* Motion::GetJointKeyframes(int jointIdx) {
	return &m_keyframes[jointIdx];
}

//---------------------------------------------------------------------------------------------------------------------------
//IO
//---------------------------------------------------------------------------------------------------------------------------
STATIC void Motion::WriteAnimToFile(Motion* motion, const String& filename) {
	FileBinaryWriter fileWriter;
	fileWriter.Open(filename);

	//FILE LAYOUT
	//String m_name;
	//uint32_t m_frameCount;
	//float m_totalLengthSeconds;
	//float m_frameRate;
	//float m_frameTime;
	//int jointCount;
	//std::vector<std::vector<Matrix4>> m_keyframes;

	fileWriter.WriteString(motion->m_name);
	fileWriter.WriteUnsignedInt(motion->m_frameCount);
	fileWriter.WriteFloat(motion->m_totalLengthSeconds);
	fileWriter.WriteFloat(motion->m_frameRate);
	fileWriter.WriteFloat(motion->m_frameTime);
	fileWriter.WriteInt((int)motion->m_keyframes.size());

	for (unsigned int i = 0; i < motion->m_keyframes.size(); i++) {
		for (unsigned int j = 0; j < motion->m_keyframes[i].size(); j++) {
			fileWriter.WriteMatrix(motion->m_keyframes[i][j]);
		}
	}

	fileWriter.Close();
}
STATIC Motion* Motion::ReadAnimFromFile(const String& filename) {
	Motion* newMotion = nullptr;
	FileBinaryReader fileReader;
	fileReader.Open(filename);

	//FILE LAYOUT
	//String m_name;
	//uint32_t m_frameCount;
	//float m_totalLengthSeconds;
	//float m_frameRate;
	//float m_frameTime;
	//std::vector<std::vector<Matrix4>> m_keyframes;

	String name;
	uint32_t frameCount;
	float totalLengthSeconds;
	float frameRate;
	float frameTime;
	int jointCount;
	
	fileReader.ReadString(&name);
	fileReader.ReadUnsignedInt(&frameCount);
	fileReader.ReadFloat(&totalLengthSeconds);
	fileReader.ReadFloat(&frameRate);
	fileReader.ReadFloat(&frameTime);
	fileReader.ReadInt(&jointCount);

	newMotion = new(MEMORY_RENDERING) Motion(name, totalLengthSeconds, frameRate);
	newMotion->m_frameTime = 1.f / frameRate;
	newMotion->m_keyframes.reserve(jointCount);
	
	for (int i = 0; i < jointCount; i++) {

		std::vector<Matrix4> jointKeyframes;
		jointKeyframes.resize(frameCount);
		for (unsigned int j = 0; j < frameCount; j++) {
			fileReader.ReadMatrix(&jointKeyframes[j]);
		}

		newMotion->m_keyframes.push_back(jointKeyframes);
	}

	return newMotion;
}

//---------------------------------------------------------------------------------------------------------------------------
//FOR BLENDING
//---------------------------------------------------------------------------------------------------------------------------
void Motion::GetJointTransformForWeightsAtKeyframe(Skeleton* skeleton, float blendWeight, std::vector<Matrix4>& matsForJoints) {

	ASSERT_OR_DIE(blendWeight >= 0.f && blendWeight <= 1.f, "ERROR: Invalid blend weight");

	uint32_t frame0;
	uint32_t frame1;
	float blend;

	float time = blendWeight * m_totalLengthSeconds;

	GetFrameIndicesWithBlend(frame0, frame1, blend, time);

	int numJoints = (int)skeleton->GetJointCount();

	if (matsForJoints.empty()) {
		matsForJoints.resize(numJoints);
	}

	for (int jointIdx = 0; jointIdx < numJoints; jointIdx++) {
		std::vector<Matrix4>* jointKeyframes = GetJointKeyframes(jointIdx);

		Matrix4 mat0 = jointKeyframes->at(frame0);
		Matrix4 mat1 = jointKeyframes->at(frame1);

		matsForJoints[jointIdx] = Matrix4::MatrixLerp(mat0, mat1, blend);
		
	}
}