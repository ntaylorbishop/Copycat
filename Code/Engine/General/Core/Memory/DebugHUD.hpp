#pragma once

#include "Engine/Renderer/Fonts/Font.hpp"

class DebugHUD {
public:
	//INIT DESTROY
	static void Initialize();
	static void Destroy();

	//INTERFACE
	static void DrawTextAtLocation(const Vector2& location, const String& str, float scale, const RGBA& color) { s_theDebugHUD->m_font->DrawText2D(location, str, scale, color); }
	static float GetDefaultTextLineHeight() { return s_theDebugHUD->m_font->GetLineHeight(); }

private:
	//INTERNAL STRUCTORS INIT
	DebugHUD();
	~DebugHUD() { }

	//UPDATE RENDER
	void Update(float deltaSeconds);
	void Render() const;

	Font* m_font;

	static DebugHUD* s_theDebugHUD;
};