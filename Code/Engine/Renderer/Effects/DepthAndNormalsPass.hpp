#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

class Scene;
class Material;
class Framebuffer;

class DepthAndNormalsPass {
public:
	static DepthAndNormalsPass* Get();

	void RunPass(Scene* sceneToRender);
	void ConnectInNormalsTexture(Material* modelMat);

private:
	//STRUCTORS SHUTDOWN
	DepthAndNormalsPass();
	~DepthAndNormalsPass();
	static void Shutdown();

	
	Material*		m_passMat	= nullptr;
	Framebuffer*	m_fbo		= nullptr;

	static DepthAndNormalsPass* s_dnPass;
};