#pragma once

#include "stdint.h"
#include "Engine/General/Core/Allocators/ObjectPool.hpp"
#include "Engine/Structures/ThreadSafe/TSQueue.hpp"
#include "Engine/General/JobSystem/Job.hpp"
#include "Engine/Structures/ThreadSafe/Thread.hpp"
#include "Engine/General/JobSystem/JobConsumer.hpp"

const uint MAX_NUM_THREADS		= 64;
const uint MAX_NUM_CATEGORIES	= 1000;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//JOBTHREAD
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
struct JobThread {
	Thread m_thread;
	JobThread* m_next;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//JOBSYSTEM CLASS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
class JobSystem {
public:
	//STARTUP SHUTDOWN
	static void Startup(uint numCategories, int numThreads);
	static void Shutdown();

	//GET
	static bool IsJobSystemRunning() { return (s_theJobSystem->m_isActive); }

	//INTERFACE
	static void WaitForJobs(const std::vector<Job*> jobs);
	static void AddWorker(uint numCategories, va_list categories );
	static Job* CreateJob(uint16_t category, JobFunctionPtr* jobWorkFunc);

private:
	//STRUCTORS INIT
	JobSystem(uint numCategories, int numThreads);
	~JobSystem();
	void Initialize();
	void InitializeJobThreadList();
	static void WorkerJobThread(void* userData);

	//INTERNALS
	TSQueue<Job*>* GetQueueForCategory(uint16_t category);
	void PushJobConsumerToList(JobConsumer* consumer);
	static void DispatchJobToAppropriateQueue(Job* job);
	

	TSQueue<Job*>*	m_jobQueues[MAX_NUM_CATEGORIES];
	size_t			m_prioritySwitch[MAX_NUM_THREADS];
	JobConsumer*	m_jobConsumers[MAX_NUM_CATEGORIES];
	ObjectPool<Job> m_jobPool;
	uint			m_numThreads	= 0;
	uint			m_numCategories = 0;
	JobThread*		m_jobThreadList = nullptr;
	uint			m_numConsumers	= 0;
	bool			m_hasStarted	= false;
	bool			m_isActive		= false;

	static JobSystem* s_theJobSystem;

	friend class JobConsumer;
	friend class Job;
};

uint SystemGetCoreCount();