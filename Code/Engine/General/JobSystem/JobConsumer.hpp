#pragma once

#include "Engine/Structures/ThreadSafe/TSQueue.hpp"
#include "Engine/General/JobSystem/Job.hpp"

const uint MAX_NUM_CONSUMER_CATEGORIES = 100;

class JobConsumer {
public:
	//STRUCTORS
	JobConsumer();
	JobConsumer(uint numCategories, ... );

	//CONSUMPTION
	void ConsumeAll();
	//bool Consume();

private:
	void PushToList(TSQueue<Job*>* jobQueue);

	TSQueue<Job*>* m_queueList[MAX_NUM_CONSUMER_CATEGORIES];
	uint m_numQueues;
};