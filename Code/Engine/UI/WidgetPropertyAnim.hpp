#pragma once

#include "Engine/Math/MathUtils.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ANIM PROPERTY MODE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
enum eAnimPropertyMode {
	ANIM_PROPERTY_MODE_CLAMP = 0,
	ANIM_PROPERTY_MODE_LOOP,
	ANIM_PROPERTY_MODE_PINGPONG
};


//---------------------------------------------------------------------------------------------------------------------------
inline eAnimPropertyMode GetAnimPropertyModeFromString(const String& animPropertyMode) {

	if (animPropertyMode == g_keyInData_animModeClamp) {
		return ANIM_PROPERTY_MODE_CLAMP;
	}
	else if (animPropertyMode == g_keyInData_animModePingPong) {
		return ANIM_PROPERTY_MODE_PINGPONG;
	}
	else if (animPropertyMode == g_keyInData_animModeLoop) {
		return ANIM_PROPERTY_MODE_LOOP;
	}
	
	return ANIM_PROPERTY_MODE_CLAMP;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//KEYFRAME
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
struct Keyframe {
	float m_parametric;
	T m_value;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//KEYFRAMEPROPERTY
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
class KeyFrameProperty {
public:
	void AddKeyframe(float parametricKey, const T& value);
	T EvaluateAtParametric(float parametricKey); //This depends on clamping or wrapping based on what the KeyFramedAnim says

	std::vector<Keyframe<T>> m_keyframes;
};


//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
void KeyFrameProperty<T>::AddKeyframe(float parametricKey, const T& value) {

	for (uint i = 0; i < m_keyframes.size(); i++) {

		if (parametricKey <= m_keyframes[i].m_parametric) {

			Keyframe<T> newKeyframe;
			newKeyframe.m_parametric = parametricKey;
			newKeyframe.m_value = value;

			if (i == 0) {
				m_keyframes.insert(m_keyframes.begin() + 0, 1, newKeyframe);
			}
			else {
				m_keyframes.insert(m_keyframes.begin() + i, 1, newKeyframe);
			}
			return;
		}
	}

	Keyframe<T> newKeyframe;
	newKeyframe.m_parametric = parametricKey;
	newKeyframe.m_value = value;

	m_keyframes.push_back(newKeyframe);
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
T KeyFrameProperty<T>::EvaluateAtParametric(float parametricKey) {

	ASSERT_OR_DIE(!m_keyframes.empty(), "ERROR: Cannot evaluate with no keyframes.");

	uint beginIdx = 0;
	uint endIdx = 0;

	for (uint i = 0; i < m_keyframes.size(); i++) {

		if (parametricKey == m_keyframes[i].m_parametric) {
			beginIdx = i;
			endIdx = i;
			break;
		}
		else if (parametricKey < m_keyframes[i].m_parametric) {
			beginIdx = i - 1;
			endIdx = i;
			break;
		}
	}

	if (beginIdx == m_keyframes.size() - 1) {
		endIdx = beginIdx;
		beginIdx--;
	}

	return Lerp(m_keyframes[beginIdx].m_value, m_keyframes[endIdx].m_value, parametricKey);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//KEYFRAMEDANIM
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
class WidgetPropertyAnim {
public:
	//STRUCTORS
	WidgetPropertyAnim()
		: m_durationSeconds(0.f)
		, m_currentSeconds(0.f)
		, m_timeInAnimSeconds(0.f)
		, m_animMode(ANIM_PROPERTY_MODE_CLAMP)
		, m_propertyName("NULL")
	{ }
	WidgetPropertyAnim(float durationSeconds, eAnimPropertyMode animMode, const String& propertyName, const String& animName)
		: m_durationSeconds(durationSeconds)
		, m_currentSeconds(0.f)
		, m_timeInAnimSeconds(0.f)
		, m_animMode(animMode)
		, m_propertyName(propertyName)
		, m_animName(animName)
	{ }

	//UPDATE
	T Evaluate();
	void Update(float deltaSeconds);

	//MEMBERS
	float m_durationSeconds;
	float m_currentSeconds;
	float m_timeInAnimSeconds;
	String m_propertyName;
	eAnimPropertyMode m_animMode;
	String m_animName;
	KeyFrameProperty<T> m_keyframedProperty;
};


//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
T WidgetPropertyAnim<T>::Evaluate() {

	return m_keyframedProperty.EvaluateAtParametric(m_timeInAnimSeconds / m_durationSeconds);
}


//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
void WidgetPropertyAnim<T>::Update(float deltaSeconds) {

	m_currentSeconds += deltaSeconds;

	switch (m_animMode) {
	case ANIM_PROPERTY_MODE_CLAMP:
		if (m_currentSeconds > m_durationSeconds) {
			m_timeInAnimSeconds = m_durationSeconds;
		}
		break;
	case ANIM_PROPERTY_MODE_LOOP:
		if (m_currentSeconds > m_durationSeconds) {
			m_timeInAnimSeconds = fmodf(m_currentSeconds, m_durationSeconds);
		}
		break;
	case ANIM_PROPERTY_MODE_PINGPONG:
		if (m_currentSeconds > m_durationSeconds) {
			float whereInAnim = fmodf(m_currentSeconds, m_durationSeconds * 2.f) - m_durationSeconds;
			if (whereInAnim > 0.f) {
				m_timeInAnimSeconds = fmodf(m_currentSeconds, m_durationSeconds);
			}
			else if (whereInAnim < 0.f) {
				if (m_currentSeconds > m_durationSeconds) {
					m_timeInAnimSeconds = m_durationSeconds - fmodf(m_currentSeconds, m_durationSeconds);
				}
			}
		}
		break;
	}
}
