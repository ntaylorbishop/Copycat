#include "Engine/Renderer/ReflectionProbes/DynamicReflectionProbe.hpp"
#include "Engine/Renderer/ReflectionProbes/DynamicReflectionProbeManager.hpp"
#include "Engine/Renderer/General/TextureCubemap.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Engine/Renderer/General/Texture.hpp"
#include "Engine/Renderer/General/TextureBuffer.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/Material/Material.hpp"
#include "Engine/Renderer/Material/MaterialBank.hpp"
#include "Engine/Renderer/Structures/BeirusMeshCollection.hpp"
#include "Engine/Renderer/Mesh/Model.hpp"
#include "Engine/Renderer/General/Texture.hpp"
#include "Engine/Renderer/Effects/OIT/OIT.hpp"
#include "Engine/Renderer/Structures/BeirusMeshCollection.hpp"
#include "ThirdParty/Parsers/xmlParser.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
static Matrix4 GetYawRotation(float yawDegs) {

	Vector4 r1 = Vector4(CosDegrees(yawDegs), 0.f, -SinDegrees(yawDegs), 0.f);
	Vector4 r2 = Vector4(0.f,                 1.f, 0.f,                  0.f);
	Vector4 r3 = Vector4(SinDegrees(yawDegs), 0.f, CosDegrees(yawDegs),  0.f);
	Vector4 r4 = Vector4(0.f,                 0.f, 0.f,                  1.f);

	return Matrix4(r1, r2, r3, r4);
}


//---------------------------------------------------------------------------------------------------------------------------
static Matrix4 GetPitchRotation(float pitchDegs) {
	Vector4 r1 = Vector4(1.f,  0.f,                   0.f,                   0.f);
	Vector4 r2 = Vector4(0.f,  CosDegrees(pitchDegs), SinDegrees(pitchDegs), 0.f);
	Vector4 r3 = Vector4(0.f, -SinDegrees(pitchDegs), CosDegrees(pitchDegs), 0.f);
	Vector4 r4 = Vector4(0.f, 0.f,                    0.f,                   1.f);

	return Matrix4(r1, r2, r3, r4);
}


//---------------------------------------------------------------------------------------------------------------------------
static Matrix4 GetRollRotation(float rollDegs) {
	Vector4 r1 = Vector4( CosDegrees(rollDegs), SinDegrees(rollDegs), 0.f, 0.f);
	Vector4 r2 = Vector4(-SinDegrees(rollDegs), CosDegrees(rollDegs), 0.f, 0.f);
	Vector4 r3 = Vector4(0.f,                   0.f,                  1.f, 0.f);
	Vector4 r4 = Vector4(0.f,                   0.f,                  0.f, 1.f);

	return Matrix4(r1, r2, r3, r4);
}


//---------------------------------------------------------------------------------------------------------------------------
static void ReconstructView(Matrix4& viewToCreate, const Vector3& rot, const Vector3& pos) {

	Matrix4 yaw		= GetYawRotation(rot.y);
	Matrix4 pitch	= GetPitchRotation(rot.x);
	Matrix4 roll	= GetRollRotation(rot.z);
	Matrix4 rp		= roll * pitch;
	Matrix4 rpy		= rp * yaw;
	viewToCreate	= rpy;
	viewToCreate.TransposeRotation();
	viewToCreate.NegateTranslation(pos);

}


//---------------------------------------------------------------------------------------------------------------------------
DynamicReflectionProbe::DynamicReflectionProbe(const Vector3& position, Scene* sceneToAddTo, uint texSize)
	: m_position(position)
	, ClickableObject::ClickableObject(GEOM_TYPE_SPHERE, position)
{

	Sphere* obj = (Sphere*)m_geom;
	obj->m_pos = position;
	obj->m_radius = 1.f;

	m_proj.ChangeToPerspectiveProjection(90.f, 1.f, m_nearPlane, m_farPlane);
	m_colorTarget = new TextureCubemap(IntVector2(texSize), TEXTURE_FORMAT_RGBA8);
	m_depthTarget = new TextureCubemap(IntVector2(texSize), TEXTURE_FORMAT_D24S8);


	GL_CHECK(glGenFramebuffers(1, &m_framebufferID));

	ReconstructView(m_view, m_rightRot,		m_position);

	m_meshRendererWithCustomMats.CreateUniform("gProj",				UNIFORM_MAT4, 1, &m_proj);
	m_meshRendererWithCustomMats.CreateUniform("gView",				UNIFORM_MAT4, 1, &m_view);
	m_meshRendererWithCustomMats.CreateUniform("gIsUsingSSAO",		UNIFORM_BOOL, 1, &BOOL_FALSE);
	m_meshRendererWithCustomMats.CreateUniform("gWriteToOITBuffers",UNIFORM_BOOL, 1, &BOOL_FALSE);

	m_model		= new Model(position, PRIMITIVE_MESH_SPHERE_100);
	m_model->SetShadowCaster(false);
	m_material	= new Material("DynamicReflectionProbe");
	Texture* normTex = Texture::CreateOrGetTexture("Data/Textures/Black_Normal.png");
	m_material->CreateUniform("gTexEnvironmentMap", UNIFORM_CUBEMAP,	1, 0, m_colorTarget);
	m_material->CreateUniform("gTexNormal",			UNIFORM_TEXTURE2D,	1, 1, normTex);
	m_model->m_material = m_material;
	sceneToAddTo->AddStaticMesh(m_model);
}


//---------------------------------------------------------------------------------------------------------------------------
DynamicReflectionProbe::~DynamicReflectionProbe() { 

	DynamicReflectionProbeManager::Get()->DetachReflectionProbe(this);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC DynamicReflectionProbe* DynamicReflectionProbe::Create(const Vector3& position, Scene* sceneToAddTo, uint texSize) {

	DynamicReflectionProbe* newProbe = new DynamicReflectionProbe(position, sceneToAddTo, texSize);
	DynamicReflectionProbeManager::Get()->AttachReflectionProbe(newProbe);

	return newProbe;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void DynamicReflectionProbe::RenderSide(Scene* scene, GLenum whichSide) {

	Uniform* proj = m_meshRendererWithCustomMats.GetUniform("gProj");
	Uniform* view = m_meshRendererWithCustomMats.GetUniform("gView");

	proj->SetData((void*)&m_proj);
	view->SetData((void*)&m_view);

	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID));
	GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, whichSide, m_colorTarget->GetTextureID(), 0));
	GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  whichSide, m_depthTarget->GetTextureID(), 0));
	GL_CHECK(glDrawBuffer(GL_COLOR_ATTACHMENT0));
	GL_CHECK(glViewport(0, 0, (GLsizei)REFLECTION_TEX_SIZE, (GLsizei)REFLECTION_TEX_SIZE));
	GL_CHECK(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

	scene->RenderSkybox(&m_meshRendererWithCustomMats);
	scene->Render(false,	&m_meshRendererWithCustomMats);
}


//---------------------------------------------------------------------------------------------------------------------------
void DynamicReflectionProbe::Update(Scene* sceneToRender) {

	m_model->m_model.ChangePosition(m_position);
	ClickableObject::m_model.ChangePosition(m_position);

	if (m_sideToRenderThisFrame == 6) {
		m_sideToRenderThisFrame = 0;
	}

	switch (m_sideToRenderThisFrame) {
	case 0: {
		m_proj.ChangeToPerspectiveProjection(90.f, 1.f, m_nearPlane, m_farPlane);
		ReconstructView(m_view, m_rightRot, m_position);
		RenderSide(sceneToRender, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
		break;
	}
	case 1: {
		m_proj.ChangeToPerspectiveProjection(90.f, 1.f, m_nearPlane, m_farPlane);
		ReconstructView(m_view,	m_leftRot, m_position);
		RenderSide(sceneToRender, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
		break;
	}
	case 2: {
		m_proj.ChangeToPerspectiveProjection(90.f, 1.f, m_nearPlane, m_farPlane);
		ReconstructView(m_view, m_topRot, m_position);
		RenderSide(sceneToRender, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
		break;
	}
	case 3: {
		m_proj.ChangeToPerspectiveProjection(90.f, 1.f, m_nearPlane, m_farPlane);
		ReconstructView(m_view, m_bottomRot, m_position);
		RenderSide(sceneToRender, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
		break;
	}
	case 4: {
		m_proj.ChangeToPerspectiveProjection(90.f, 1.f, m_nearPlane, m_farPlane);
		ReconstructView(m_view, m_forwardRot, m_position);
		RenderSide(sceneToRender, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
		break;
	}
	case 5: {
		m_proj.ChangeToPerspectiveProjection(90.f, 1.f, m_nearPlane, m_farPlane);
		ReconstructView(m_view, m_backwardRot, m_position);
		RenderSide(sceneToRender, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
		break;
	}
	}

	m_sideToRenderThisFrame++;


	UpdateAxisBoundingBoxes();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CLICKABLES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void DynamicReflectionProbe::UpdateAxisBoundingBoxes() {

	//m_xAxis.
}


//---------------------------------------------------------------------------------------------------------------------------
void DynamicReflectionProbe::RenderAxes() const {

	MeshID meshID = PRIMITIVE_MESH_SKYBOX;
	Material* mat = MaterialBank::GetMaterial("UnlitOpaque");
	mat->CreateUniform("gDiffuseColor", UNIFORM_RGBA, 1);
	Matrix4 model = Matrix4::IDENTITY;
	MeshRenderer* mr = BeirusRenderer::GetGlobalMeshRenderer();

	float smallScale = 0.04f;
	float largeScale = 1.f;

	BeirusRenderer::DisableDepthTesting();

	//X AXIS
	mat->GetUniform("gDiffuseColor")->SetData((void*)&RGBA::RED);
	model = Matrix4::IDENTITY;
	model.ChangePosition(m_position + Vector3(largeScale, 0.f, 0.f));
	model.Scale(Vector3(largeScale, smallScale, smallScale));
	mr->RenderMeshWithMaterial(meshID, mat, model);

	//Y AXIS
	mat->GetUniform("gDiffuseColor")->SetData((void*)&RGBA::GREEN);
	model = Matrix4::IDENTITY;
	model.ChangePosition(m_position + Vector3(0.f, largeScale, 0.f));
	model.Scale(Vector3(smallScale, largeScale, smallScale));
	mr->RenderMeshWithMaterial(meshID, mat, model);

	//Z AXIS
	mat->GetUniform("gDiffuseColor")->SetData((void*)&RGBA::BLUE);
	model = Matrix4::IDENTITY;
	model.ChangePosition(m_position + Vector3(0.f, 0.f, largeScale));
	model.Scale(Vector3(smallScale, smallScale, largeScale));
	mr->RenderMeshWithMaterial(meshID, mat, model);

	BeirusRenderer::EnableDepthTesting();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RENDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void DynamicReflectionProbe::Render() const {

	ClickableObject::Render();

	if (m_isSelected) {
		RenderAxes();
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//WRITING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void DynamicReflectionProbe::WriteToFile(const String& path, const String& namePrefix) {

	m_colorTarget->WriteToFile(path, namePrefix); 

	//WRITE PROBE FILE
	XMLNode rootNode = rootNode.createXMLTopNode("StaticReflectionProbe");

	String posStr = StringUtils::Stringf("%.2f,%.2f,%.2f", m_position.x, m_position.y, m_position.z);
	String minStr = StringUtils::Stringf("%.2f,%.2f,%.2f", m_boxMins.x, m_boxMins.y, m_boxMins.z);
	String maxStr = StringUtils::Stringf("%.2f,%.2f,%.2f", m_boxMaxs.x, m_boxMaxs.y, m_boxMaxs.z);
	rootNode.addAttribute("Position", posStr.c_str());
	rootNode.addAttribute("BoxMins", minStr.c_str());
	rootNode.addAttribute("BoxMaxs", maxStr.c_str());

	XMLNode textureNode = textureNode.createXMLTopNode("Textures");
	
	XMLNode posX = posX.createXMLTopNode("Texture");
	posX.addAttribute("faceIdx", "posX");
	posX.addAttribute("path", (path + namePrefix + "_posX.png").c_str());
	textureNode.addChild(posX);

	XMLNode negX = negX.createXMLTopNode("Texture");
	negX.addAttribute("faceIdx", "negX");
	negX.addAttribute("path", (path + namePrefix + "_negX.png").c_str());
	textureNode.addChild(negX);

	XMLNode posY = posY.createXMLTopNode("Texture");
	posY.addAttribute("faceIdx", "posY");
	posY.addAttribute("path", (path + namePrefix + "_posY.png").c_str());
	textureNode.addChild(posY);

	XMLNode negY = negY.createXMLTopNode("Texture");
	negY.addAttribute("faceIdx", "negY");
	negY.addAttribute("path", (path + namePrefix + "_negY.png").c_str());
	textureNode.addChild(negY);

	XMLNode posZ = posZ.createXMLTopNode("Texture");
	posZ.addAttribute("faceIdx", "posZ");
	posZ.addAttribute("path", (path + namePrefix + "_posZ.png").c_str());
	textureNode.addChild(posZ);

	XMLNode negZ = negZ.createXMLTopNode("Texture");
	negZ.addAttribute("faceIdx", "negZ");
	negZ.addAttribute("path", (path + namePrefix + "_negZ.png").c_str());
	textureNode.addChild(negZ);

	rootNode.addChild(textureNode);

	String xmlPath = path + namePrefix + ".xml";

	if (rootNode.writeToFile(xmlPath.c_str()) != 0) {
		ERROR_AND_DIE("COULD NOT SAVE PROBE XML FILE.");
	}
}