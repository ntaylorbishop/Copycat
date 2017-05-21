#include "Engine/Renderer/Sprites/SpriteAnim.hpp"
#include "Engine/Utils/GeneralUtils/XMLUtils.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOAD
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
static SpriteSheet* FindSpriteSheetPath(const XMLNode& spriteAnimNode) {

	const char* spriteSheetPath = "NULL";
	IntVector2 tileSize = IntVector2::ZERO;

	for (int attribIdx = 0; attribIdx < spriteAnimNode.nAttribute(); attribIdx++) {

		XMLAttribute attrib = spriteAnimNode.getAttribute(attribIdx);

		const char* attribName = attrib.lpszName;
		const char* attribVal = attrib.lpszValue;

		if (strcmp(attribName, SPRITE_ANIM_SPRITESHEET) == 0) {
			spriteSheetPath = attribVal;
		}
		else if (strcmp(attribName, SPRITE_ANIM_SPRITESHEET_SIZE) == 0) {
			tileSize = XMLUtils::ParseIntVector2(attribVal);
		}
	}

	if (tileSize != IntVector2::ZERO && strcmp(spriteSheetPath, "NULL") != 0) {
		return SpriteSheet::CreateOrGetSpriteSheet(tileSize, spriteSheetPath);
	}

	return nullptr;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC SpriteAnim SpriteAnim::LoadFromXML(const XMLNode& spriteAnimNode) {

	SpriteAnim newSpriteAnim = SpriteAnim();

	newSpriteAnim.m_spriteSheet = FindSpriteSheetPath(spriteAnimNode);
	ASSERT_OR_DIE(newSpriteAnim.m_spriteSheet != nullptr, "ERROR: SpriteSheet must be defined for SpriteAnim in XML.");

	for (int attribIdx = 0; attribIdx < spriteAnimNode.nAttribute(); attribIdx++) {

		XMLAttribute attrib = spriteAnimNode.getAttribute(attribIdx);

		const char* attribName = attrib.lpszName;
		const char* attribVal = attrib.lpszValue;

		if (strcmp(attribName, SPRITE_ANIM_SPRITESHEET_NUM_SPRITES) == 0) {
			newSpriteAnim.m_spriteStartIndex = 0;
			newSpriteAnim.m_spriteEndIndex = XMLUtils::ParseInt(attribVal) - 1;
		}
		else if (strcmp(attribName, SPRITE_ANIM_ANIMMODE) == 0) {
			if (strcmp(attribVal, SPRITE_ANIM_MODE_CLAMP) == 0) {
				newSpriteAnim.m_animMode = ANIM_MODE_PLAY_TO_END;
			}
			else if (strcmp(attribVal, SPRITE_ANIM_MODE_LOOPING) == 0) {
				newSpriteAnim.m_animMode = ANIM_MODE_LOOPING;
			}
			else if (strcmp(attribVal, SPRITE_ANIM_MODE_PINGPONG) == 0) {
				newSpriteAnim.m_animMode = ANIM_MODE_PING_PONG;
			}
		}
	}

	return newSpriteAnim;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void SpriteAnim::Update(float deltaSeconds) {

	if (m_isPlaying) {
		m_age += deltaSeconds;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GETTERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
AABB2 SpriteAnim::GetCurrentTexCoords() {

	int numIndexes				= m_spriteEndIndex - m_spriteStartIndex + 1;
	float fractionThroughAnim	= m_age / m_durationSeconds;
	m_currIndex					= (int)(fractionThroughAnim * numIndexes);

	return m_spriteSheet->GetTexCoordsForSpriteIndex(m_currIndex);
}


//---------------------------------------------------------------------------------------------------------------------------
uint SpriteAnim::GetCurrentSpriteIndex() {

	int numIndexes				= m_spriteEndIndex - m_spriteStartIndex + 1;
	float fractionThroughAnim	= m_age / m_durationSeconds;
	m_currIndex					= (int)(fractionThroughAnim * numIndexes);

	return m_currIndex;
}