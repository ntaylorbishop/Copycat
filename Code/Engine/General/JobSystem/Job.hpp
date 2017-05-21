#pragma once

#include "Engine/General/Core/EngineCommon.hpp"

const size_t JOB_BUFFER_SIZE = 72 * 8; //72B

class Job;
typedef void(JobFunctionPtr)(Job* job);

enum eJobCategory : uint16_t {
	JOB_CATEGORY_GENERIC = 0,
	JOB_CATEGORY_GENERIC_SLOW,
	JOB_CATEGORY_NUM_CATEGORIES
};

enum eJobState {
	JOB_CREATED = 0,
	JOB_ENQUEUED,
	JOB_INPROGRESS,
	JOB_FINISHED
};

class Job {
public:
	//STRUCTORS
	Job();
	Job(eJobCategory category, JobFunctionPtr* workFunc);
	void Initialize(eJobCategory category, JobFunctionPtr* workFunc);

	//READ WRITE
	template<typename T> 
	T Read();
	template<typename T>
	void Write(T data);

	void Dispatch();
	void Run();

	eJobCategory GetCategory() const { return m_category; }
	eJobState GetState() const { return m_workState; }
	void SetState(eJobState nWorkState) { m_workState = nWorkState; }

private:

	void AllocateToBuffer(void* data, size_t size);

	byte m_data[JOB_BUFFER_SIZE];
	byte* m_currWritePosInBuffer;
	byte* m_currReadPosInBuffer;
	JobFunctionPtr* m_workFunc;
	eJobCategory m_category;
	eJobState m_workState;

	friend class JobSystem;
};

//---------------------------------------------------------------------------------------------------------------------------
//TEMPLATE FUNCTIONS
//---------------------------------------------------------------------------------------------------------------------------
template <typename T>
T Job::Read() {
	T* nObject = (T*)m_currReadPosInBuffer;
	m_currReadPosInBuffer += sizeof(T);
	return *nObject;
}

template <typename T>
void Job::Write(T data) {
	AllocateToBuffer((void*)&data, sizeof(T));
}