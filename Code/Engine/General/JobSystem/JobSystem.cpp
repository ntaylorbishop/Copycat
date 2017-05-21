#include "Engine/General/JobSystem/JobSystem.hpp"
#include "Engine/General/Logger/TheLogger.hpp"
#include "Engine/General/JobSystem/JobConsumer.hpp"

STATIC JobSystem* JobSystem::s_theJobSystem = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//STATICS
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
STATIC void JobSystem::Startup(uint numCategories, int numThreads) {
	
	ASSERT_OR_DIE(s_theJobSystem == nullptr, "ERROR: Job system already initialized");
	ASSERT_OR_DIE(numCategories <= MAX_NUM_CATEGORIES, "ERROR: Cannot create more than 1000 categories");
	s_theJobSystem = new JobSystem(numCategories, numThreads);
	s_theJobSystem->Initialize();
}
STATIC void JobSystem::Shutdown() {
	
	ASSERT_OR_DIE(s_theJobSystem != nullptr, "ERROR: Job system has not been initialized before trying to shut down.");
	delete s_theJobSystem;
	s_theJobSystem = nullptr;
}


STATIC void JobSystem::AddWorker(uint numCategories, va_list categories) {
	ASSERT_OR_DIE(s_theJobSystem->m_hasStarted == false, "ERROR: Cannot create job consumer when job system has started");
	s_theJobSystem->PushJobConsumerToList(new JobConsumer(numCategories, categories));
}

STATIC Job* JobSystem::CreateJob(uint16_t category, JobFunctionPtr* jobWorkFunc) {

	Job* job = s_theJobSystem->m_jobPool.Create();
	job->Initialize((eJobCategory)category, jobWorkFunc);
	return job;
}
STATIC void JobSystem::WaitForJobs(const std::vector<Job*> jobs) {
	
	bool hasFinished = false;
	while (hasFinished) {
		
		bool allJobsFinished = true;
		for (uint i = 0; i < jobs.size(); i++) {
			if (jobs[i]->GetState() != JOB_FINISHED) {
				allJobsFinished = false;
			}
		}

		hasFinished = allJobsFinished;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//MEMBERS
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
JobSystem::JobSystem(uint numCategories, int numThreads)
	: m_numThreads(0)
	, m_numCategories(numCategories)
	, m_jobThreadList(nullptr)
	, m_jobQueues()
	, m_prioritySwitch()
	, m_numConsumers(0)
	, m_hasStarted(false)
	, m_isActive(true)
{
	m_jobPool = ObjectPool<Job>(100000);
	uint numCores = SystemGetCoreCount();

	if (numThreads < 0) {
		m_numThreads = numCores + numThreads;
	}
	else if (numThreads > 0) {
		m_numThreads = numThreads;
		
		if (numThreads > (int)numCores) {
			TheLogger::LogPrintf("WARNING: Spawning number of threads higher than the core count can be ill-performant.", "JobSystem", LOG_SEVERE);
		}
	}

	for (uint i = 0; i < MAX_NUM_CATEGORIES; i++) {
		m_jobQueues[i] = nullptr;
	}
	for (uint i = 0; i < MAX_NUM_CATEGORIES; i++) {
		m_jobConsumers[i] = nullptr;
	}
	for (uint i = 0; i < m_numCategories; i++) {
		m_jobQueues[i] = new TSQueue<Job*>();
	}
}
JobSystem::~JobSystem() {
	m_isActive = false;
	for (uint i = 0; i < m_numCategories; i++) {
		delete m_jobQueues[i];
		m_jobQueues[i] = nullptr;
	}
	for (uint i = 0; i < m_numConsumers; i++) {
		delete m_jobConsumers[i];
		m_jobConsumers[i] = nullptr;
	}

	JobThread* curr = m_jobThreadList;
	for (uint i = 0; i < m_numThreads; i++) {
		JobThread* tmp = curr->m_next;
		curr->m_thread.Join();
		delete curr;
		curr = tmp;
	}
}
void JobSystem::Initialize() {
	InitializeJobThreadList();
	m_hasStarted = true;
}

void JobSystem::InitializeJobThreadList() {

	m_jobThreadList = new JobThread();
	m_jobThreadList->m_thread = Thread(WorkerJobThread, (void*)m_prioritySwitch[0]);
	JobThread* head = m_jobThreadList;

	for (uint i = 1; i < m_numThreads; i++) {
		m_prioritySwitch[i] = (i % 2 == 0) ? (0) : (1);
		m_jobThreadList->m_next = new JobThread();
		m_jobThreadList->m_next->m_thread = Thread(WorkerJobThread, (void*)m_prioritySwitch[i]);
		m_jobThreadList = m_jobThreadList->m_next;
	}
	m_jobThreadList = head;
}



//---------------------------------------------------------------------------------------------------------------------------
//INTERNALS
//---------------------------------------------------------------------------------------------------------------------------
STATIC void JobSystem::WorkerJobThread(void* userData) {

	size_t which = (size_t)userData;

	JobConsumer consumer;

	if (!(which == 0 || which == 1)) {
		TheLogger::LogPrintf("ERROR: Cannot prioritize thread. Aborting", "JobSystem", LOG_SEVERE);
	}

	consumer = (which == 0) 
		? (JobConsumer(2, JOB_CATEGORY_GENERIC, JOB_CATEGORY_GENERIC_SLOW)) 
		: (JobConsumer(2, JOB_CATEGORY_GENERIC_SLOW, JOB_CATEGORY_GENERIC));

	while (IsJobSystemRunning()) {
		consumer.ConsumeAll();
		std::this_thread::yield();
	}

	consumer.ConsumeAll();
}

TSQueue<Job*>* JobSystem::GetQueueForCategory(uint16_t category) {
	return m_jobQueues[category];
}
void JobSystem::PushJobConsumerToList(JobConsumer* consumer) {
	//Not using these yet.
	m_jobConsumers[m_numConsumers++] = consumer;
}

STATIC void JobSystem::DispatchJobToAppropriateQueue(Job* job) {
	uint16_t category = job->GetCategory();
	job->SetState(JOB_ENQUEUED);
	s_theJobSystem->m_jobQueues[category]->Enqueue(job);
}

//---------------------------------------------------------------------------------------------------------------------------
//OUTSIDE OF CLASS
//---------------------------------------------------------------------------------------------------------------------------
uint SystemGetCoreCount() {
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	return (uint)sysinfo.dwNumberOfProcessors;
}