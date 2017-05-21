#pragma once

#include "Engine/Renderer/SpriteRenderer/Sprite.hpp"

enum eSpriteLoopMode {
	SPRITE_ANIMMODE_ONESHOT = 0,
	SPRITE_ANIMMODE_LOOP
};

struct SpriteFrame {
	float m_start;
	SpriteResource* m_spriteResource;

};

class AnimatedSprite : Sprite {
public:
	AnimatedSprite();

private:

};