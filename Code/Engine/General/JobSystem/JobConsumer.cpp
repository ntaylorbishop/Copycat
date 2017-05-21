#include "Engine/General/JobSystem/JobConsumer.hpp"
#include "Engine/General/JobSystem/JobSystem.hpp"

//---------------------------------------------------------------------------------------------------------------------------
//STRUCTORS
//---------------------------------------------------------------------------------------------------------------------------
JobConsumer::JobConsumer() 
	: m_queueList()
	, m_numQueues(0)
{ }
JobConsumer::JobConsumer(uint numCategories, ... ) 
	: m_queueList()
	, m_numQueues(0)
{

	for (uint i = 0; i < MAX_NUM_CONSUMER_CATEGORIES; i++) {
		m_queueList[i] = nullptr;
	}

	va_list arguments;
	va_start(arguments, numCategories);

	for (uint i = 0; i < numCategories; i++) {
		TSQueue<Job*>* queueToAdd = JobSystem::s_theJobSystem->GetQueueForCategory(va_arg(arguments, uint16_t));
		PushToList(queueToAdd);
	}
	va_end(arguments);
}

//---------------------------------------------------------------------------------------------------------------------------
//CONSUMPTION
//---------------------------------------------------------------------------------------------------------------------------
void JobConsumer::ConsumeAll() {

	bool b = true;
	while(b) {
		for (uint i = 0; i < m_numQueues; i++) {
			Job* jobToConsume = nullptr;
			m_queueList[i]->Dequeue(jobToConsume);
			if (jobToConsume) {
				jobToConsume->SetState(JOB_INPROGRESS);
				jobToConsume->Run();
				jobToConsume->SetState(JOB_FINISHED);

			}
			else {
				return;
			}
		}
	}
}
/* Unreachable code wtf?
bool JobConsumer::Consume() {

	for (uint i = 0; i < m_numQueues; i++) {
		Job* jobToConsume = nullptr;
		m_queueList[i]->Dequeue(jobToConsume);
		if (jobToConsume) {
			jobToConsume->SetState(JOB_INPROGRESS);
			jobToConsume->Run();
			jobToConsume->SetState(JOB_FINISHED);

		}
		else {
			return false;
		}
		return true;
	}

	return false;
}*/

//---------------------------------------------------------------------------------------------------------------------------
//PRIVATE
//---------------------------------------------------------------------------------------------------------------------------
void JobConsumer::PushToList(TSQueue<Job*>* jobQueue) {
	m_numQueues++;
	m_queueList[m_numQueues++] = jobQueue;
}