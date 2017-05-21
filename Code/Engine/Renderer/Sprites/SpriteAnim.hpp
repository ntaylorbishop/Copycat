#pragma once

#include "Engine/Renderer/Sprites/Spritesheet.hpp"
#include "Engine/Renderer/General/Texture.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Vector2.hpp"
#include "ThirdParty/Parsers/xmlParser.h"
#include "Engine/General/Time/Clock.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CONSTANTS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
const char* const SPRITE_ANIM_MODE_CLAMP				= "clamp";
const char* const SPRITE_ANIM_MODE_LOOPING				= "loop";
const char* const SPRITE_ANIM_MODE_PINGPONG				= "ping-pong";
const char* const SPRITE_ANIM_SPRITESHEET				= "spriteSheet";
const char* const SPRITE_ANIM_SPRITESHEET_SIZE			= "spriteSheetSize";
const char* const SPRITE_ANIM_SPRITESHEET_NUM_SPRITES	= "spriteSheetNumSprites";
const char* const SPRITE_ANIM_ANIMMODE					= "animMode";
const char* const SPRITE_ANIM_XML_NAME					= "SpriteAnim";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ENUMS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
enum eAnimMode {
	ANIM_MODE_PLAY_TO_END = 0,
	ANIM_MODE_LOOPING,
	ANIM_MODE_PING_PONG,
	NUM_ANIM_SPRITE_MODES
};

class SpriteAnim {
public:
	//LOAD
	static SpriteAnim LoadFromXML(const XMLNode& spriteAnimNode);

	//STRUCTORS
	SpriteAnim();
	SpriteAnim(SpriteSheet* spriteSheet, float durationSeconds, 
			eAnimMode spriteAnimMode, int spriteStartIndex, int spriteEndIndex);


	//UPDATE
	void				Update(float deltaSeconds);

	//MODIFY
	void				Pause()										{ m_isPlaying = false;										}
	void				Resume()									{ m_isPlaying = true;										}
	void				Reset()										{ m_age = 0.f;												}
	
	//GETTERS
	bool				IsFinished()			const				{ return (m_age >= m_durationSeconds);						}
	bool				IsPlaying()				const				{ return m_isPlaying;										}
	const Texture*		GetTexture()								{ return m_spriteSheet->GetTexture();						}
	AABB2				GetCurrentTexCoords();
	SpriteSheet*		GetSpriteSheet()		const				{ return m_spriteSheet;										}
	float				GetDurationSeconds()	const				{ return m_durationSeconds;									}
	float				GetSecondsElapsed()		const				{ return m_age;												}
	float				GetSecondsRemaining()	const				{ return m_durationSeconds - m_age;							}
	float				GetFractionElapsed()	const				{ return m_age / m_durationSeconds;							}
	float				GetFractionRemaining()	const				{ return (m_durationSeconds - m_age) / m_durationSeconds;	}
	uint				GetCurrentSpriteIndex();

	//SETTERS
	void				SetSecondsElapsed(float secondsElapsed)		{ m_age = secondsElapsed;									}
	void				SetFractionElapsed(float fractionElapsed)	{ m_age = fractionElapsed * m_durationSeconds;				}
	void				SetDuration(float dur)						{ m_durationSeconds = dur;									}


	SpriteSheet*	m_spriteSheet				= nullptr;
	float			m_durationSeconds			= 0.f;
	eAnimMode		m_animMode					= NUM_ANIM_SPRITE_MODES;
	int				m_spriteStartIndex			= 0;
	int				m_spriteEndIndex			= 0;
	uint			m_currIndex					= 0;
	bool			m_isPlaying					= false;
	float			m_age						= 0.f;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline SpriteAnim::SpriteAnim()
	: m_spriteSheet(nullptr)
	, m_durationSeconds(0.f)
	, m_animMode(ANIM_MODE_PLAY_TO_END)
	, m_spriteStartIndex(0)
	, m_spriteEndIndex(0)
	, m_isPlaying(true)
	, m_age(0.f)
	, m_currIndex(0)
{ }


//---------------------------------------------------------------------------------------------------------------------------
inline SpriteAnim::SpriteAnim(SpriteSheet* spriteSheet, float durationSeconds, eAnimMode spriteAnimMode,
		int spriteStartIndex, int spriteEndIndex)
	: m_spriteSheet(spriteSheet)
	, m_durationSeconds(durationSeconds)
	, m_animMode(spriteAnimMode)
	, m_spriteStartIndex(spriteStartIndex)
	, m_spriteEndIndex(spriteEndIndex)
	, m_isPlaying(true)
	, m_age(0.f)
	, m_currIndex(spriteStartIndex)
{ }