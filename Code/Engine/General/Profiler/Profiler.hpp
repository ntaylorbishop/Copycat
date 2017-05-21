#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/General/Core/Allocators/ObjectPool.hpp"
#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/General/Time/Time.hpp"

const uint MAX_NUM_SAMPLES = 10000;
const uint MAX_NUM_PRINTEDLINES = 1000;

enum eProfilerView {
	PROFILER_LISTVIEW = 0,
	PROFILER_FLATVIEW
};

//---------------------------------------------------------------------------------------------------------------------------
//SAMPLE
//---------------------------------------------------------------------------------------------------------------------------
struct ProfilerPrintedLine {
	ProfilerPrintedLine() {}
	char const *tag;
	uint numCalls;
	uint64_t totalCycles;
	uint64_t totalTime;
	uint64_t selfTimeCycles;
	uint64_t selfTime;
	double percentFrameTime;
	uint depth;
};

struct Sample {
	void Initialize(char const *sampleTag) {
		tag = sampleTag;
		parent = nullptr;
		next = nullptr;
		children = nullptr;
		start = Time::GetCurrentPerformanceCount();
		end = 0;
	}

	char const *tag;
	Sample *prev;
	Sample *next;
	Sample *children;
	Sample *parent;
	ProfilerPrintedLine *sampleInfo;
	uint64_t start;
	uint64_t end;
};

struct ConstructedSample {
	uint depth;
	Sample* sample;
	bool alreadyWorkedOn;
};

//---------------------------------------------------------------------------------------------------------------------------
//PROFILER
//---------------------------------------------------------------------------------------------------------------------------
class Profiler {
public:
	static void Startup();
	static void Shutdown();

	static void MarkFrame();

	static void PushFrame(char const *tag);
	static void PopFrame();

	static void Enable();
	static void Disable();

	static void EnableRealTime() { s_theProfilingSystem->m_realTimeEnabled = true; }
	static void DisableRealTime() { s_theProfilingSystem->m_realTimeEnabled = false; }

	static void PrintLastFrame(eProfilerView view);

	Sample* m_currentSample;
	Sample* m_currentFrame;
	Sample* m_prevFrame;
	bool m_isEnabled;
	bool m_desiredEnabled;
	ObjectPool<Sample> m_samplePool;
	bool m_realTimeEnabled;

private:
	Profiler();
	~Profiler();
	
	void InternalPushFrame(char const *tag);
	void InternalPopFrame();

	void InsertAsChild(Sample* nSample);

	void ClearTree(Sample* root);

	Sample* GetLastFrame() const { return m_prevFrame; }

	void InternalPrintLastFrame(eProfilerView view);

	void ConstructList(uint currDepth, std::vector<ConstructedSample>& lines, Sample* sample);
	void CollapseListToListView(std::vector<ConstructedSample>& listToCollapse, std::vector<ProfilerPrintedLine>& outCollapsedList);
	void CollapseListToFlatView(std::vector<ConstructedSample>& listToCollapse, std::vector<ProfilerPrintedLine>& outCollapsedList);
	void PrintList(const std::vector<ProfilerPrintedLine>& collapsedList, eProfilerView view);

	void InternalPrintLine(const ProfilerPrintedLine& lineToPrint, eProfilerView view);

	ProfilerPrintedLine m_printedLines[MAX_NUM_PRINTEDLINES];

	static Profiler* s_theProfilingSystem;
};

int SampleComparatorListView(void const *a, void const *b);
int SampleComparatorFlatView(void const *a, void const *b);
int ProfilerPrintedLineComparator(void const *a, void const *b);

void ProfilerToggle(Command& args);
void ProfilerPrintLastFrame(Command& args);
void ProfilerDisableRealTime(Command& args);