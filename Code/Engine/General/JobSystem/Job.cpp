#include "Engine/General/JobSystem/Job.hpp"
#include "Engine/General/JobSystem/JobSystem.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
Job::Job() 
	: m_data()
	, m_currWritePosInBuffer(nullptr)
	, m_currReadPosInBuffer(nullptr)
	, m_category(JOB_CATEGORY_GENERIC)
	, m_workFunc(nullptr)
	, m_workState(JOB_CREATED)
{ 
	m_currWritePosInBuffer = m_data;
	m_currReadPosInBuffer = m_data;
}
Job::Job(eJobCategory category, JobFunctionPtr* workFunc)
	: m_data()
	, m_currWritePosInBuffer(nullptr)
	, m_currReadPosInBuffer(nullptr)
	, m_category(category)
	, m_workFunc(workFunc)
	, m_workState(JOB_CREATED)
{
	m_currWritePosInBuffer = m_data;
	m_currReadPosInBuffer = m_data;
}
void Job::Initialize(eJobCategory category, JobFunctionPtr* workFunc) {
	m_category = category;
	m_workFunc = workFunc;
}

void Job::AllocateToBuffer(void* data, size_t size) {
	memcpy(m_currWritePosInBuffer, data, size);
	m_currWritePosInBuffer += size;
}

void Job::Dispatch() {
	JobSystem::DispatchJobToAppropriateQueue(this);
}
void Job::Run() {
	m_workFunc(this);
}