#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <math.h>
#include <vector>

#include "Engine/Config/BuildConfig.hpp"
#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Utils/ErrorWarningAssert.hpp"
#include "Engine/Utils/FileUtils/FileUtils.hpp"
#include "Engine/General/Logger/TheLogger.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/IntVector3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Objects/AABB3.hpp"
#include "Engine/Math.hpp"
#include "Engine/Renderer/OpenGL/OpenGLExtensions.hpp"
#include "Engine/Renderer/Vertex/Vertex.hpp"
#include "Engine/Renderer/Sprites/Spritesheet.hpp"
#include "Engine/Renderer/General/Texture.hpp"
#include "Engine/Renderer/General/RGBA.hpp"
#include "Engine/Renderer/Vertex/VertexDefinition.hpp"
#include "Engine/Renderer/OpenGL/OpenGLEnums.hpp"
#include "Engine/Renderer/Mesh/StaticMesh2D.hpp"
#include "Engine/Renderer/Cameras/PlayerCamera3D.hpp"
#include "Engine/Renderer/OpenGL/OpenGLUtils.hpp"
#include "Engine/Renderer/OpenGL/OpenGLErrorReporter.hpp"
#include "Engine/Renderer/General/RenderCommon.hpp"
#include "Engine/Renderer/Mesh/LineMesh.hpp"

#define MAX(a,b)				\
   ({ __typeof__ (a) _a = (a);	\
	   __typeof__ (b) _b = (b); \
	 _a > _b ? _a : _b; })

#define MIN(a,b)				\
   ({ __typeof__ (a) _a = (a);	\
	   __typeof__ (b) _b = (b); \
	 _a < _b ? _a : _b; })


const float g_nearZ				= 0.1f;
const float g_farZ				= 1000.f;
const int	g_defaultViewMode	= 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FORWARD DECLARATIONS AND DEFINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class PlayerCamera3D;
class Framebuffer;
class Model;
class Camera2D;
class MeshRenderer;
class TextureBuffer;
class GPUPerformanceTimer;
class SSBO;
class Scene;

#define MAX_NUM_RENDERABLE_QUADS 1000

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ENUMS 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
enum ePrimitiveType {
	PRIMITIVE_POINTS = 0,
	PRIMITIVE_LINES,
	PRIMITIVE_LINE_LOOP,
	PRIMITIVE_TRIANGLES,
	PRIMITIVE_QUADS,
	NUM_PRIMITIVE_TYPES
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CLUSTERED FORWARD CONSTANTS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const uint MAX_NUM_LIGHTS_PER_TILE	= 256;
const uint TILE_SIZE_PIXELS			= 16;

const float DEFAULT_PERSPECTIVE_FOV = 120.f;
const float DEFAULT_ASPECT_RATIO	= 16.f / 9.f;
const float DEFAULT_NEAR_PLANE		= 0.1f;
const float DEFAULT_FAR_PLANE		= 1000.f;

extern Matrix4 g_defaultPerspectiveProjectionMatrix;
extern Matrix4 g_defaultUIMatrix;

#if IS_WINDOWED
const Vector2		SCREEN_SIZE		= Vector2(1600.f, 900.f);
const IntVector2	SCREEN_SIZE_INT	= IntVector2(1600, 900);
#else
const Vector2		SCREEN_SIZE		= Vector2(3840.f, 2160.f);
const IntVector2	SCREEN_SIZE_INT	= IntVector2(3840, 2160);
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BEIRUSRENDERER 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BeirusRenderer {
public:
	//INIT SHUTDOWN
	static void Initialize(const RGBA& clearColor);
	static void InitDefaultProjectionMats();
	static void Shutdown();


	static const Vector2* GetScreenSize() { return &SCREEN_SIZE; }
	

	//UPDATE RENDER
	static void Render() { }

	static void RenderScene(Scene* sceneToRender) { s_theBeirusRenderer->InternalRenderScene(sceneToRender); }
	 
	 
	//SETUP / UTILITY FUNCS
	static void ClearColorTarget(GLenum targetType, int idx, float* val);
	static void EnableBlendingForColorTarget(uint colorTargetIdx);
	static void DisableBlendingForColorTarget(uint colorTargetIdx);
	static void SetBlendModeForColorTarget(GLenum src, GLenum dest, uint colorTargetIdx);

	static void EnableBlending();
	static void EnableBackfaceCulling();
	static void EnableDepthTesting();
	static void EnableLineSmoothing();
	static void EnableDepthWriting();
	static void EnableColorWriting();
	static void DisableColorWriting();
	static void DisableDepthWriting();
	static void DisableBackfaceCulling();
	static void DisableBlending();
	static void DisableDepthTesting();
	static void DisableLineSmoothing();
	static void SetDepthTestingMode(GLenum depthTestMode);
	static void SetCullFace(bool isFront);

	static void ToggleFXAA();
	static uint GetFXAAToggle() { return s_theBeirusRenderer->m_usingFXAA; }

	static void BlendMode(GLenum sfactor, GLenum dfactor);
	static void PointSize(float size);
	static void LineWidth(float width);
	static void ClearBuffer(GLbitfield mask);
	static void ClearScreen(const RGBA& col);
	static void ClearScreen();
	static void ClearDepth();
	static void ClearColor();
	static String GetCurrentVersionOfGLSL();
	static String GetCurrentVersionOfOpenGL();
	 
	//2D DRAW OBJECTS
	static Material* GetOrGenerateMaterial(Texture* tex);
	static Material* GetOrGenerateOneChannelMaterial(Texture* tex);
	static void DrawAABB2(const AABB2& quad, const RGBA& tint);
	static void DrawTexturedAABB2(Material* mat, const RGBA& tint, const AABB2& quad);
	static void DrawTexturedAABB2(Material* mat, const AABB2& texCoords, const RGBA& tint, const AABB2& quad);
	static void DrawTexturedAABB2(Texture* tex, const RGBA& tint, const AABB2& quad);
	static void DrawTexturedAABB2(Texture* tex, const AABB2& texCoords, const RGBA& tint, const AABB2& quad);



	static void DrawCircle(const Camera2D& cam, const RGBA& color, const Vector3& position, const float radius);

	//LINE DRAWING
	static void DrawLine(const Vector3& start, const Vector3& end, const RGBA& tint);
	static void DrawBoxLineMesh(const AABB3& box, const RGBA& tint);

	//MODEL VIEW TRANSFORM
	static const Matrix4* GetViewMatrix()						{ return PlayerCamera3D::GetViewMatrix();			}
	static const Matrix4* GetProjectionMatrix()					{ return &g_defaultPerspectiveProjectionMatrix;		}
	static const Matrix4* GetDefaultUIProjectMatrix()			{ return &g_defaultUIMatrix;						}

	//GETTERS SETTERS
	static Framebuffer*		GetGlobalFBO()						{ return s_theBeirusRenderer->m_globalFBO;			}
	static PlayerCamera3D*	GetPlayerCamera()					{ return s_theBeirusRenderer->m_playerCamera;		}
	static SSBO*			GetLightIndexSSBO()					{ return s_theBeirusRenderer->m_lightIndexBuffer;	}
	static MeshID			GetGlobalFullscreenMesh()			{ return s_fullScreenMesh;							}
	static MeshRenderer*	GetGlobalMeshRenderer()				{ return s_theBeirusRenderer->m_meshRenderer;		}
	static TextureBuffer*	GetGlobalDepthMap()					{ return s_theBeirusRenderer->m_depthTarget;		}
	static TextureBuffer*	GetGlobalNormalsMap()				{ return s_theBeirusRenderer->m_normalsTarget;		}
	static float			GetHDRExposureValue()				{ return s_theBeirusRenderer->m_hdrExposureVal;		}
	static float			GetWhitePoint()						{ return s_theBeirusRenderer->m_whitePoint;			}
	static TextureBuffer*	GetDefaultColorTarget()				{ return s_theBeirusRenderer->m_colorTarget;		}

	static void				SetHDRExposureValue(float exposure)	{ s_theBeirusRenderer->m_hdrExposureVal = exposure; }
	static void				SetWhitePoint(float whitePoint)		{ s_theBeirusRenderer->m_whitePoint = whitePoint;	}

	static double GetGPUTime();

	static GPUPerformanceTimer* GetGPUPerfTimer()				{ return s_theBeirusRenderer->m_gpuTimer;			}
	static MeshID				GetScratch2DMeshID()			{ return s_scratchMesh2D;							}

private:
	//STRUCTORS AND INIT
	BeirusRenderer(const RGBA& clearColor);
	~BeirusRenderer();

	void InitializeAllShaders();
	void InitializeComputeShaderAndDataForIt();
	void InitializeFBO();
	static void InitializeOpenGLDefinitions();

	//UPDATE
	void Update(float deltaSeconds);

	//FORWARD+
	void RunLightCullingComputeShader();


	//RENDER
	void InternalRenderScene(Scene* sceneToRender);
	void RenderDepthAndNormals(Scene* sceneToRender);
	void Render3DElements(Scene* sceneToRender);
	void RenderLines();
	void RenderTransparentElements(Scene* sceneToRender);
	void Render2DElements();
	void RenderFullScreenQuad();
	void RenderFXAA();

	//INTERFACE
	static void AttachPlayerCamera3D(PlayerCamera3D* pCam3D);


	//LINE DRAWING
	std::vector<LineMesh>		m_linesToRender;
	static MeshID				s_scratchLineMesh;

	std::vector<StaticMesh2D>	m_rQuadsToRender;

	PlayerCamera3D* m_playerCamera			= nullptr;
	Camera2D*		m_hudCamera				= nullptr;
	Vector2			m_screenSize			= Vector2::ZERO;

	//CLUSTERED FORWARD
	SSBO*			m_lightIndexBuffer		= nullptr;

	//GLOBAL FBO
	static MeshID	s_fullScreenMesh;
	static MeshID	s_scratchMesh2D;
	TextureBuffer*	m_colorTarget			= nullptr;
	TextureBuffer*	m_averageLuminosity		= nullptr;
	TextureBuffer*	m_normalsTarget			= nullptr;
	TextureBuffer*	m_depthTarget			= nullptr;
	Framebuffer*	m_globalFBO				= nullptr;
	MeshRenderer*	m_meshRenderer			= nullptr;
	Material*		m_defaultFBOMaterial	= nullptr;
	Scene*			m_currentScene			= nullptr;

	//TONEMAPPING
	float			m_hdrExposureVal		= 1.f;
	float			m_whitePoint			= 0.20f;

	GPUPerformanceTimer* m_gpuTimer			= nullptr;

	//FXAA
	Material*		m_fxaaMat				= nullptr;
	bool			m_usingFXAA				= true;

	static BeirusRenderer* s_theBeirusRenderer;

	friend class PlayerCamera3D;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INLINES
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
inline STATIC void BeirusRenderer::ToggleFXAA() { 

	s_theBeirusRenderer->m_usingFXAA = !s_theBeirusRenderer->m_usingFXAA;
}


