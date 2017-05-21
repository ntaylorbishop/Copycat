#include "Engine/Config/BuildConfig.hpp"

#ifdef GRAPHICS_API_OGL
#include "Engine/GPUPerformanceTimer.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
GPUPerformanceTimer::GPUPerformanceTimer() 
	: m_startTime(0)
	, m_stopTime(0)
{
	GL_CHECK(glGenQueries(2, m_queryID));
}


//---------------------------------------------------------------------------------------------------------------------------
GPUPerformanceTimer::~GPUPerformanceTimer() { }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//START STOP
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void GPUPerformanceTimer::Start() {
	GL_CHECK(glQueryCounter(m_queryID[0], GL_TIMESTAMP));
}


//---------------------------------------------------------------------------------------------------------------------------
void GPUPerformanceTimer::Stop() {
	GL_CHECK(glQueryCounter(m_queryID[1], GL_TIMESTAMP));

	// wait until the results are available
	unsigned int stopTimerAvailable = 0;
	while (!stopTimerAvailable) {
		GL_CHECK(glGetQueryObjectiv(m_queryID[1], GL_QUERY_RESULT_AVAILABLE, (GLint*)&stopTimerAvailable));
	}

	// get query results
	GL_CHECK(glGetQueryObjectui64v(m_queryID[0], GL_QUERY_RESULT, (GLuint64*)&m_startTime));
	GL_CHECK(glGetQueryObjectui64v(m_queryID[1], GL_QUERY_RESULT, (GLuint64*)&m_stopTime));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GETTERS SETTERS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
double GPUPerformanceTimer::GetDifference() const {
	return ((m_stopTime - m_startTime) / 1000000.0);
}

#endif
