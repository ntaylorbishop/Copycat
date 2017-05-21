#include "Engine/Renderer/Effects/DepthAndNormalsPass.hpp"
#include "Engine/Renderer/Material/Material.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/Framebuffer.hpp"
#include "Engine/Renderer/Shaders/Uniform.hpp"
#include "Engine/Renderer/Material/MaterialBank.hpp"
#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/Scene/Scene.hpp"

STATIC DepthAndNormalsPass* DepthAndNormalsPass::s_dnPass = nullptr;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RUN
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void DepthAndNormalsPass::RunPass(Scene* sceneToRender) {

	m_fbo->Bind();
	BeirusRenderer::EnableDepthWriting();
	BeirusRenderer::SetDepthTestingMode(GL_LESS);
	sceneToRender->RenderWithMaterial(m_passMat);
}


//---------------------------------------------------------------------------------------------------------------------------
void DepthAndNormalsPass::ConnectInNormalsTexture(Material* modelMat) {

	Uniform* normalMap = modelMat->GetUniform("gTexNormal");

	if (normalMap != nullptr) {
		Texture* normalTex = (Texture*)normalMap->GetData();
		Uniform* uniInMat = m_passMat->GetUniform("gTexNormal");
		uniInMat->SetData(normalTex);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS GET
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
DepthAndNormalsPass::DepthAndNormalsPass() {

	m_passMat = MaterialBank::GetMaterial("DepthAndNormalsPass");

	std::vector<TextureBuffer*> colorTargets;
	colorTargets.push_back(BeirusRenderer::GetGlobalNormalsMap());
	m_fbo = new Framebuffer(colorTargets, BeirusRenderer::GetGlobalDepthMap());
}


//---------------------------------------------------------------------------------------------------------------------------
DepthAndNormalsPass::~DepthAndNormalsPass() {

}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void DepthAndNormalsPass::Shutdown() {
	delete s_dnPass;
	s_dnPass = nullptr;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC DepthAndNormalsPass* DepthAndNormalsPass::Get() {

	if (s_dnPass == nullptr) {
		s_dnPass = new DepthAndNormalsPass();
		BeirusEngine::RegisterShutdownCallback(&DepthAndNormalsPass::Shutdown);
	}
	return s_dnPass;
}