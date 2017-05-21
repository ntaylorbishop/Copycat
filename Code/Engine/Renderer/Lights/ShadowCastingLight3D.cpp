#include "Engine/Renderer/Lights/ShadowCastingLight3D.hpp"
#include "Engine/Renderer/General/Texture.hpp"
#include "Engine/Renderer/General/TextureCubemap.hpp"
#include "Engine/Renderer/Framebuffer.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/Material/MaterialBank.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Engine/Renderer/Material/Material.hpp"
#include "Engine/Renderer/General/TextureBuffer.hpp"

STATIC Material* ShadowCastingLight3D::s_depthOnlyMat = nullptr;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//VIEW ROTATION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
static Matrix4 GetYawRotation(float yawDegs) {

	Vector4 r1 = Vector4( CosDegrees(yawDegs),       0.f,    -SinDegrees(yawDegs),        0.f);
	Vector4 r2 = Vector4( 0.f,                       1.f,     0.f,                        0.f);
	Vector4 r3 = Vector4( SinDegrees(yawDegs),       0.f,     CosDegrees(yawDegs),        0.f);
	Vector4 r4 = Vector4( 0.f,                       0.f,     0.f,                        1.f);

	return Matrix4(r1, r2, r3, r4);
}


//---------------------------------------------------------------------------------------------------------------------------
static Matrix4 GetPitchRotation(float pitchDegs) {
	Vector4 r1 = Vector4(1.f,     0.f,                       0.f,                        0.f);
	Vector4 r2 = Vector4(0.f,      CosDegrees(pitchDegs),    SinDegrees(pitchDegs),      0.f);
	Vector4 r3 = Vector4(0.f,     -SinDegrees(pitchDegs),    CosDegrees(pitchDegs),      0.f);
	Vector4 r4 = Vector4(0.f,     0.f,                       0.f,                        1.f);

	return Matrix4(r1, r2, r3, r4);
}


//---------------------------------------------------------------------------------------------------------------------------
static Matrix4 GetRollRotation(float rollDegs) {
	Vector4 r1 = Vector4( CosDegrees(rollDegs),      SinDegrees(rollDegs),     0.f,       0.f);
	Vector4 r2 = Vector4(-SinDegrees(rollDegs),      CosDegrees(rollDegs),     0.f,       0.f);
	Vector4 r3 = Vector4( 0.f,                       0.f,                      1.f,       0.f);
	Vector4 r4 = Vector4( 0.f,                       0.f,                      0.f,       1.f);

	return Matrix4(r1, r2, r3, r4);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
static void ReconstructView(Matrix4& orthoToCreate, const Vector3& rot, const Vector3& pos, float zNear, float zFar) {

	orthoToCreate.ChangeToPerspectiveProjection(90.f, 1.f, zNear, zFar);

	Matrix4 yaw		= GetYawRotation(rot.y);
	Matrix4 pitch	= GetPitchRotation(rot.x);
	Matrix4 roll	= GetRollRotation(rot.z);

	Matrix4 rp = roll * pitch;
	Matrix4 rpy = rp * yaw;

	Matrix4 view = rpy;
	view.TransposeRotation();
	view.NegateTranslation(pos);

	Matrix4 newOrtho;
	newOrtho = view * orthoToCreate;

	Matrix4 model = Matrix4::IDENTITY;
	//model.ChangePosition(pos);
	//Matrix4 withModel;
	
	orthoToCreate = newOrtho;
}


//---------------------------------------------------------------------------------------------------------------------------
ShadowCastingLight3D::ShadowCastingLight3D(const Vector3& pos, const RGBA& color, float radius)
	: Light3D(pos, color, radius) 
	, m_depthMap(new TextureCubemap(IntVector2(DEPTH_BUFFER_RESOLUTION), TEXTURE_FORMAT_D16))
{ 

	if (!s_depthOnlyMat) {
		
		s_depthOnlyMat = new Material("ShadowMapping");
		s_depthOnlyMat->CreateUniform("gProj", UNIFORM_MAT4, 1, &Matrix4::IDENTITY);
	}

	ReconstructView(m_forwardOrtho,		Vector3(-720.f,  -180.f,      0.f), m_position, m_nearPlane, m_farPlane); //CHECK
	ReconstructView(m_backwardOrtho,	Vector3( 540.f,   540.f,    180.f), m_position, m_nearPlane, m_farPlane); //CHECK
	ReconstructView(m_topOrtho,			Vector3( 450.f,     0.f,    180.f), m_position, m_nearPlane, m_farPlane); //CHECK
	ReconstructView(m_bottomOrtho,		Vector3( 630.f,  -450.f,    -90.f), m_position, m_nearPlane, m_farPlane); //CHECK
	ReconstructView(m_rightOrtho,		Vector3( 180.f,   450.f,   -180.f), m_position, m_nearPlane, m_farPlane); //CHECK
	ReconstructView(m_leftOrtho,		Vector3(   0.f,   450.f,      0.f), m_position, m_nearPlane, m_farPlane); //CHECK

	GL_CHECK(glGenFramebuffers(1, &m_framebufferID));

	//TODO: Allow hook-up of shaders that use shadowmapping and iterate through them to bind the uniforms
	HookUpShadowMapToShader("BlinnPhong");
	HookUpShadowMapToShader("BlinnPhongParticleLighting");
	HookUpShadowMapToShader("BlinnPhongParticleLighting_NoOIT");
	HookUpShadowMapToShader("Water");
}


//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL ShadowCastingLight3D::~ShadowCastingLight3D() {
	delete m_depthMap;
	m_depthMap = nullptr;
}


//---------------------------------------------------------------------------------------------------------------------------
void ShadowCastingLight3D::HookUpShadowMapToShader(const char* shaderName) {

	ShaderProgram* multiLightProg = ShaderProgram::CreateOrGetShaderProgram(shaderName);

	Uniform* shadowMapUni		= multiLightProg->GetUniform("gShadowMap");
	Uniform* hasShadowMapUni	= multiLightProg->GetUniform("gHasShadowLight");
	Uniform* shadowLightPos		= multiLightProg->GetUniform("gShadowLightPos");
	Uniform* shadowNearPlane	= multiLightProg->GetUniform("gNearPlane");
	Uniform* shadowFarPlane		= multiLightProg->GetUniform("gFarPlane");

	shadowMapUni	->SetData((void*)m_depthMap);
	hasShadowMapUni	->SetData((void*)&UINT_TRUE);
	shadowLightPos	->SetData((void*)&m_position);
	shadowNearPlane	->SetData((void*)&m_nearPlane);
	shadowFarPlane	->SetData((void*)&m_farPlane);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void ShadowCastingLight3D::RenderShadowMap(Scene* scene, GLenum whichSide, const Matrix4& currProj) {

	Uniform* proj = s_depthOnlyMat->GetUniform("gProj");
	proj->SetData((void*)&currProj);

	GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, whichSide, m_depthMap->GetTextureID(), 0));
	GL_CHECK(glDrawBuffer(GL_NONE));
	GL_CHECK(glViewport(0, 0, (GLsizei)DEPTH_BUFFER_RESOLUTION, (GLsizei)DEPTH_BUFFER_RESOLUTION));
	GL_CHECK(glClear(GL_DEPTH_BUFFER_BIT));

	scene->RenderForShadows(s_depthOnlyMat);
}

//---------------------------------------------------------------------------------------------------------------------------
VIRTUAL void ShadowCastingLight3D::RenderShadowMaps(Scene* scene) {

	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID));
	BeirusRenderer::EnableDepthWriting();
	BeirusRenderer::EnableDepthTesting();
	BeirusRenderer::DisableColorWriting();
	BeirusRenderer::SetDepthTestingMode(GL_LESS);
	
	if (m_sideToRenderThisFrame == 6) {
		m_sideToRenderThisFrame = 0;
	}

	if (!m_isMoving) {

		switch (m_sideToRenderThisFrame) {
		case 0: {
			ReconstructView(m_rightOrtho, Vector3(180.f, 450.f, -180.f), m_position, m_nearPlane, m_farPlane); //CHECK
			RenderShadowMap(scene, GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_rightOrtho);
			break;
		}
		case 1: {
			ReconstructView(m_leftOrtho, Vector3(0.f, 450.f, 0.f), m_position, m_nearPlane, m_farPlane); //CHECK
			RenderShadowMap(scene, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, m_leftOrtho);
			break;
		}
		case 2: {
			ReconstructView(m_topOrtho, Vector3(450.f, 0.f, 180.f), m_position, m_nearPlane, m_farPlane); //CHECK
			RenderShadowMap(scene, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, m_topOrtho);
			break;
		}
		case 3: {
			ReconstructView(m_bottomOrtho, Vector3(630.f, -450.f, -90.f), m_position, m_nearPlane, m_farPlane); //CHECK
			RenderShadowMap(scene, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, m_bottomOrtho);
			break;
		}
		case 4: {
			ReconstructView(m_forwardOrtho, Vector3(-720.f, -180.f, 0.f), m_position, m_nearPlane, m_farPlane); //CHECK
			RenderShadowMap(scene, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, m_forwardOrtho);
			break;
		}
		case 5: {
			ReconstructView(m_backwardOrtho, Vector3(540.f, 540.f, 180.f), m_position, m_nearPlane, m_farPlane); //CHECK
			RenderShadowMap(scene, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, m_backwardOrtho);
			break;
		}
		}
	}
	else if (m_isMoving) {

		ReconstructView(m_rightOrtho, Vector3(180.f, 450.f, -180.f), m_position, m_nearPlane, m_farPlane); //CHECK
		RenderShadowMap(scene, GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_rightOrtho);
		
		ReconstructView(m_leftOrtho, Vector3(0.f, 450.f, 0.f), m_position, m_nearPlane, m_farPlane); //CHECK
		RenderShadowMap(scene, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, m_leftOrtho);
		
		ReconstructView(m_topOrtho, Vector3(450.f, 0.f, 180.f), m_position, m_nearPlane, m_farPlane); //CHECK
		RenderShadowMap(scene, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, m_topOrtho);
		
		ReconstructView(m_bottomOrtho, Vector3(630.f, -450.f, -90.f), m_position, m_nearPlane, m_farPlane); //CHECK
		RenderShadowMap(scene, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, m_bottomOrtho);
		
		ReconstructView(m_forwardOrtho, Vector3(-720.f, -180.f, 0.f), m_position, m_nearPlane, m_farPlane); //CHECK
		RenderShadowMap(scene, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, m_forwardOrtho);
		
		ReconstructView(m_backwardOrtho, Vector3(540.f, 540.f, 180.f), m_position, m_nearPlane, m_farPlane); //CHECK
		RenderShadowMap(scene, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, m_backwardOrtho);
	}

	m_sideToRenderThisFrame++;
}