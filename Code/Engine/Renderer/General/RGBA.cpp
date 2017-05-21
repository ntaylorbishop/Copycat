#include "Engine/Renderer/General/RGBA.hpp"

#define STATIC

STATIC const RGBA RGBA::WHITE(1.f, 1.f, 1.f, 1.f);
STATIC const RGBA RGBA::BLACK(0.f, 0.f, 0.f, 1.f);
STATIC const RGBA RGBA::RED(1.f, 0.f, 0.f, 1.f);
STATIC const RGBA RGBA::GREEN(0.f, 1.f, 0.f, 1.f);
STATIC const RGBA RGBA::BLUE(0.f, 0.f, 1.f, 1.f);
STATIC const RGBA RGBA::YELLOW(1.f, 1.f, 0.f, 1.f);
STATIC const RGBA RGBA::MAGENTA(1.f, 0.f, 1.f, 1.f);
STATIC const RGBA RGBA::CYAN(0.f, 1.f, 1.f, 1.f);
STATIC const RGBA RGBA::GREY(0.5f, 0.5f, 0.5f, 1.f);
STATIC const RGBA RGBA::ORANGE(0xdd, 0x92, 0x00, 0xFF);

void RGBA::ConvertHex(int rVal, int gVal, int bVal, int aVal) {
	r = ((float)rVal / 255.f);
	g = ((float)gVal / 255.f);
	b = ((float)bVal / 255.f);
	a = ((float)aVal / 255.f);
}
void RGBA::ConvertHex(int rVal, int gVal, int bVal) {
	r = ((float)rVal / 255.f);
	g = ((float)gVal / 255.f);
	b = ((float)bVal / 255.f);
}
