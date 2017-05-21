#include "Engine/Particles/Particle.hpp"
#include "Engine/Particles/ParticleSystemManager.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Engine/Renderer/Material/Material.hpp"
#include "Engine/Particles/ParticleDefinition.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void ParticleRenderSetupCallback(void* userData) {

	Particle* particle	= (Particle*)userData;
	Model* sMesh		= particle->m_staticMesh;
	
	Uniform* model		= sMesh->m_material->GetUniform("gModel");
	Uniform* isParticle = sMesh->m_material->GetUniform("gIsParticle");
	Uniform* spriteIdx	= sMesh->m_material->GetUniform("gSpriteIndex");
	Uniform* tileSize	= sMesh->m_material->GetUniform("gTileSize");
	Uniform* color		= sMesh->m_material->GetUniform("gVertColor");

	particle->m_spriteAnim.GetCurrentSpriteIndex();

	isParticle->SetData((void*)&BOOL_TRUE);
	spriteIdx->SetData((void*)&particle->m_spriteIndex);
	tileSize->SetData(&particle->m_spritesheetTileSize);
	color->SetData(&particle->m_color);

	model->SetData(&sMesh->m_model);
}


//---------------------------------------------------------------------------------------------------------------------------
Particle::Particle(Material* pMat, ParticleDefinition* pDef, const Vector3& startPos, const Vector3& startingVel, 
	const Vector3& accel, float lifetime, const Vector3& scale, const RGBA& tint) 
	: m_velocity(startingVel)
	, m_acceleration(accel)
	, m_lifetime(lifetime)
	, m_position(startPos + Vector3(Randf(-1.f, 1.f), Randf(-1.f, 1.f), Randf(-1.f, 1.f)))
	, m_pMat(pMat)
	, m_pDef(pDef)
	, m_scale(scale)
	, m_staticMesh(new Model(Vector3::ZERO, PRIMITIVE_MESH_QUAD))
{
	UNUSED(tint);
	m_spriteAnim = pDef->m_spriteAnim;
	m_spriteAnim.SetDuration(lifetime);
	m_staticMesh->m_model.ChangePosition(m_position);
	m_staticMesh->m_model.Scale(m_scale);
	m_staticMesh->m_material = pMat;
	m_staticMesh->m_renderState = pDef->m_renderState;
	m_spritesheetTileSize = pDef->m_spriteAnim.GetSpriteSheet()->GetSize().ToVector2();

	ParticleSystemManager::GetScene()->AddStaticMesh(m_staticMesh);
	m_staticMesh->SetRenderSetupCallback(&ParticleRenderSetupCallback, this);
	UpdateBillboard();
}


//---------------------------------------------------------------------------------------------------------------------------
Particle::~Particle() {
	ParticleSystemManager::GetScene()->RemoveStaticMesh(m_staticMesh);
	delete m_staticMesh;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UPDATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void Particle::Update(float deltaSeconds) {

	m_age			+= deltaSeconds;

	if (m_age >= m_lifetime) {
		m_isAlive = false;
		return;
	}

	m_color.a = 1.f - (m_age / m_lifetime);

	
	m_velocity += m_acceleration * deltaSeconds;
	m_position += m_velocity	 * deltaSeconds;

	m_spriteAnim.Update(deltaSeconds);
	m_spriteIndex = m_spriteAnim.GetCurrentSpriteIndex();

	UpdateBillboard();
	m_staticMesh->CalcDistFromCamera();
}


//---------------------------------------------------------------------------------------------------------------------------
void Particle::UpdateBillboard() {

	Matrix4 view = *BeirusRenderer::GetPlayerCamera()->GetViewNotInvMatrix();

	m_staticMesh->m_model.SetFirstRow(m_scale.x * view.GetFirstRow());
	m_staticMesh->m_model.SetSecondRow(m_scale.y * view.GetSecondRow());
	m_staticMesh->m_model.SetThirdRow(m_scale.z * view.GetThirdRow());
	m_staticMesh->m_model.SetFourthRow(Vector4(m_position, 1.f));
}