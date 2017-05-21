#pragma once

struct Pixel {
	Pixel(float r, float g, float b, float a, float d)
		: m_redChannel(r)
		, m_greenChannel(g)
		, m_blueChannel(b)
		, m_alphaChannel(a)
		, m_depthFromCamera(d)
	{ }


	float m_redChannel;
	float m_greenChannel;
	float m_blueChannel;
	float m_alphaChannel;
	float m_depthFromCamera;
};