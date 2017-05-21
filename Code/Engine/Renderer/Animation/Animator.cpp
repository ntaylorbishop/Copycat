#include "Engine/Renderer/Animation/Animator.hpp"

void Animator::AddMotionForBlending(Motion* motion, float evalWeight) {

	motion->SetPlaybackType(PLAYBACK_LOOP);
	m_motions.push_back(MotionBlend(motion, evalWeight));
}

void Animator::ApplyMotionToSkeleton(Skeleton* skeleton, float deltaSeconds, float evalWeight) {

	MotionBlend* motionToBlendFrom = nullptr;
	MotionBlend* motionToBlendTo = nullptr;

	if (evalWeight >= m_motions[0].m_weight && evalWeight <= m_motions[1].m_weight) {
		motionToBlendFrom = &m_motions[0];
		motionToBlendTo = &m_motions[1];
	}
	else if (evalWeight >= m_motions[1].m_weight && evalWeight <= m_motions[2].m_weight) {
		motionToBlendFrom = &m_motions[1];
		motionToBlendTo = &m_motions[2];
	}

	float s = 0.f;
	float e = 0.f;

	s = motionToBlendFrom->m_weight;
	e = motionToBlendTo->m_weight;
	float blend = (evalWeight - s) / (e - s);
	motionToBlendFrom->m_normalizedWeight = 1.f - blend;
	motionToBlendTo->m_normalizedWeight = blend;

	CalculatePercentThrough(deltaSeconds, motionToBlendFrom, motionToBlendTo);

	std::vector<Matrix4> jointTransformsAtTime_ForFirst;
	std::vector<Matrix4> jointTransformsAtTime_ForSecond;
	motionToBlendFrom->m_motion->GetJointTransformForWeightsAtKeyframe(skeleton, m_percentThrough, jointTransformsAtTime_ForFirst);
	motionToBlendTo->m_motion->GetJointTransformForWeightsAtKeyframe(skeleton, m_percentThrough, jointTransformsAtTime_ForSecond);

	float w1 = motionToBlendFrom->m_normalizedWeight;
	float w2 = motionToBlendTo->m_normalizedWeight;
	float lerpingBlendWeight = w2 / (w1 + w2);

	BlendAnims(skeleton, jointTransformsAtTime_ForFirst, jointTransformsAtTime_ForSecond, lerpingBlendWeight);
}

//---------------------------------------------------------------------------------------------------------------------------
//UTILITY FUNCS
//---------------------------------------------------------------------------------------------------------------------------
void Animator::CalculatePercentThrough(float deltaSeconds, MotionBlend* from, MotionBlend* to) {

	//deltaSeconds = deltaSeconds / 20.f;

	float fromTime = (float)from->m_motion->m_totalLengthSeconds;
	float toTime = (float)to->m_motion->m_totalLengthSeconds;

	float ndt1 = deltaSeconds / fromTime;
	float ndt2 = deltaSeconds / toTime;

	float percentToAdd = (ndt1 * from->m_normalizedWeight) + (ndt2 * to->m_normalizedWeight);

	m_percentThrough += percentToAdd;

	m_percentThrough = fmodf(m_percentThrough, 1.f);
}

void Animator::BlendAnims(Skeleton* skeleton, const std::vector<Matrix4>& from, const std::vector<Matrix4>& to, float blendWeight) {

	int numJoints = (int)skeleton->GetJointCount();
	for (int jointIdx = 0; jointIdx < numJoints; jointIdx++) {

		Matrix4 mat0 = from[jointIdx];
		Matrix4 mat1 = to[jointIdx];

		Matrix4 newModel = Matrix4::MatrixLerp(mat0, mat1, blendWeight);
		skeleton->SetJointWorldTransform(jointIdx, newModel);
	}
}