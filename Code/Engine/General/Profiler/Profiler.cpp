#include "Engine/General/Profiler/Profiler.hpp"
#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Console/Console.hpp"

STATIC Profiler* Profiler::s_theProfilingSystem = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//STATICS
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
STATIC void Profiler::Startup() {
	#if IS_PROFILER_ENABLED
		ASSERT_OR_DIE(s_theProfilingSystem == nullptr, "ERROR: Cannot re-initialize the Profiler system");

		s_theProfilingSystem = new Profiler();
		s_theProfilingSystem->m_isEnabled = true;
		s_theProfilingSystem->m_desiredEnabled = true;

		Console::RegisterCommand("profiler", "Enables / disables the profiler. (profiler <enable / disable>)", ProfilerToggle);
		Console::RegisterCommand("pframe", "Prints the last frame samples taken. (pframe <listview / flatview>, <realtime>", ProfilerPrintLastFrame);
		Console::RegisterCommand("pframedisable", "Disables real-time frame profiler printing.", ProfilerDisableRealTime);
	#endif
}
STATIC void Profiler::Shutdown() {
	#if IS_PROFILER_ENABLED
		ASSERT_OR_DIE(s_theProfilingSystem != nullptr, "ERROR: Cannot shutdown Profiler before it is initialized.");
		delete s_theProfilingSystem;
	#endif
}

STATIC void Profiler::MarkFrame() {
	#if IS_PROFILER_ENABLED
		if (s_theProfilingSystem->m_isEnabled && s_theProfilingSystem->m_realTimeEnabled) {
			s_theProfilingSystem->InternalPrintLastFrame(PROFILER_LISTVIEW);
		}

		if (s_theProfilingSystem->m_isEnabled && s_theProfilingSystem->m_prevFrame) {
			s_theProfilingSystem->ClearTree(s_theProfilingSystem->m_prevFrame);
		}
		ASSERT_OR_DIE(s_theProfilingSystem->m_currentFrame == s_theProfilingSystem->m_currentSample, "ERROR: Sample was pushed without being popped.");
		s_theProfilingSystem->m_prevFrame = s_theProfilingSystem->m_currentFrame;
		s_theProfilingSystem->InternalPopFrame();

		s_theProfilingSystem->m_isEnabled = s_theProfilingSystem->m_desiredEnabled;

		if (s_theProfilingSystem->m_isEnabled) {
			s_theProfilingSystem->InternalPushFrame("frame");
		}
		s_theProfilingSystem->m_currentFrame = s_theProfilingSystem->m_currentSample;
	#endif
}

STATIC void Profiler::PushFrame(char const *tag) {
	UNUSED(tag);
	#if IS_PROFILER_ENABLED
	s_theProfilingSystem->InternalPushFrame(tag);
	#endif
}
STATIC void Profiler::PopFrame() {
	#if IS_PROFILER_ENABLED
	s_theProfilingSystem->InternalPopFrame();
	#endif
}

STATIC void Profiler::Enable() {
	#if IS_PROFILER_ENABLED
		s_theProfilingSystem->m_desiredEnabled = true;
	#endif
}
STATIC void Profiler::Disable() {
	#if IS_PROFILER_ENABLED
		s_theProfilingSystem->m_desiredEnabled = false;
	#endif
}

STATIC void Profiler::PrintLastFrame(eProfilerView view) {
	UNUSED(view);
	#if IS_PROFILER_ENABLED
	s_theProfilingSystem->InternalPrintLastFrame(view);
	#endif
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
//MEMBERS
//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Profiler::Profiler() 
	: m_currentSample(nullptr)
	, m_currentFrame(nullptr)
	, m_prevFrame(nullptr)
	, m_isEnabled(false)
	, m_desiredEnabled(false)
	, m_realTimeEnabled(false)
{
	m_samplePool = ObjectPool<Sample>(MAX_NUM_SAMPLES);
}
Profiler::~Profiler() { 
	m_samplePool.Deconstruct(); 
}

void Profiler::InternalPushFrame(char const *tag) {
	Sample* nSample = m_samplePool.Create();
	nSample->Initialize(tag);

	if (m_currentSample) {
		nSample->parent = m_currentSample;
		m_currentSample = nSample;
		InsertAsChild(nSample);
	}
	else {
		m_currentSample = nSample;
		m_currentFrame = nSample;
	}
}
void Profiler::InternalPopFrame() {

	if (m_currentSample && m_currentSample->parent) {
		m_currentSample->end = Time::GetCurrentPerformanceCount();
		m_currentSample = m_currentSample->parent;
	}
	else if (m_currentSample && !m_currentSample->parent) {
		m_currentSample->end = Time::GetCurrentPerformanceCount();
		m_currentSample = nullptr;
	}
}

void Profiler::InsertAsChild(Sample* nSample) {
	Sample* parent = nSample->parent;

	if (!parent)
		return;
	
	if (!parent->children) {
		nSample->parent->children = nSample;
	}
	else {

		Sample* child = parent->children;

		if (child->next) {
			Sample* temp = child->next;
			child->next = nSample;
			nSample->prev = child;
			nSample->next = temp;

		}
		else {
			child->next = nSample;
			nSample->prev = child;
		}
	}
}

void Profiler::ClearTree(Sample* curr) {

	if (curr->children) {
		ClearTree(curr->children);
	}

	if (curr->next) {
		ClearTree(curr->next);
	}


	Sample* last = curr->prev;
	m_samplePool.Free(curr);

	if(last)
		last->next = nullptr;
}

void Profiler::InternalPrintLastFrame(eProfilerView view) {
	Console::PrintOutput("PRINTING LIST VIEW", CONSOLE_VERIFY);

	std::vector<ConstructedSample> listToCollapse;
	ConstructList(0, listToCollapse, m_prevFrame);

	std::vector<ProfilerPrintedLine> collapsedList;
	if (view == PROFILER_LISTVIEW) {
		CollapseListToListView(listToCollapse, collapsedList);
	}
	else {
		CollapseListToFlatView(listToCollapse, collapsedList);
	}

	char output[512];
	sprintf_s(output, 512, "%-40s %-10s %-35s %-35s %-20s", "TAG", "CALLS", "TIME", "SELF TIME", "PERCENT FRAME TIME");
	Console::PrintOutput(output, CONSOLE_NORMAL);

	PrintList(collapsedList, view);
}

void Profiler::ConstructList(uint currDepth, std::vector<ConstructedSample>& lines, Sample* sample) {
	if (sample->children) {
		ConstructList(currDepth + 1, lines, sample->children);
	}

	if (sample->next) {
		ConstructList(currDepth, lines, sample->next);
	}

	ConstructedSample cSample;
	cSample.alreadyWorkedOn = false;
	cSample.depth = currDepth;
	cSample.sample = sample;
	lines.push_back(cSample);
}
void Profiler::CollapseListToFlatView(std::vector<ConstructedSample>& listToCollapse, std::vector<ProfilerPrintedLine>& outCollapsedList) {

	qsort(listToCollapse.data(), listToCollapse.size(), sizeof(ConstructedSample), SampleComparatorListView);

	for (uint i = 0; i < listToCollapse.size(); ) {

		if (i == listToCollapse.size())
			break;

		Sample* currSample = listToCollapse[i].sample;

		ProfilerPrintedLine nLine = ProfilerPrintedLine();
		nLine.tag = currSample->tag;
		nLine.numCalls = 1;
		nLine.totalCycles = currSample->end - currSample->start;
		nLine.totalTime = Time::ConvertPerformanceCountToTimeSeconds(currSample->start, currSample->end);
		nLine.selfTimeCycles = nLine.totalCycles;
		nLine.selfTime = nLine.totalTime;
		nLine.depth = listToCollapse[i].depth;
		nLine.percentFrameTime = 0.f;

		uint idxOfNextToCheck = 1;
		while ((i + idxOfNextToCheck) < listToCollapse.size() 
			&& listToCollapse[i].depth - listToCollapse[i + idxOfNextToCheck].depth == -1) 
		{
			Sample* nextSample = listToCollapse[i + idxOfNextToCheck].sample;
			nLine.selfTimeCycles -= nextSample->end - nextSample->start;
			nLine.selfTime -= Time::ConvertPerformanceCountToTimeSeconds(nextSample->start, nextSample->end);
			idxOfNextToCheck++;
		}

		idxOfNextToCheck = 1;
		while (i < (listToCollapse.size() - 1)
			&& listToCollapse[i].sample->tag == listToCollapse[i + idxOfNextToCheck].sample->tag 
			&& listToCollapse[i].depth == listToCollapse[i + idxOfNextToCheck].depth)
		{
			Sample* nextSample = listToCollapse[i + idxOfNextToCheck].sample;
			nLine.selfTimeCycles += (nextSample->end - nextSample->start);
			nLine.selfTime += Time::ConvertPerformanceCountToTimeSeconds(nextSample->start, nextSample->end);
			nLine.numCalls++;
			idxOfNextToCheck++;
		}

		outCollapsedList.push_back(nLine);
		i += idxOfNextToCheck;
	}

	qsort(outCollapsedList.data(), outCollapsedList.size(), sizeof(ProfilerPrintedLine), SampleComparatorFlatView);

	std::vector<ProfilerPrintedLine> outLines;
	for (uint i = 0; i < outCollapsedList.size(); ) {

		ProfilerPrintedLine& currLine = outCollapsedList[i];
		outLines.push_back(currLine);

		if (i == outCollapsedList.size() - 1)
			break;

		for (uint j = i + 1; j < outCollapsedList.size(); j++) {

			ProfilerPrintedLine& nextLine = outCollapsedList[j];

			if (currLine.tag == nextLine.tag) {
				outLines[i].totalCycles += nextLine.selfTimeCycles;
				outLines[i].totalTime += nextLine.selfTime;
				outLines[i].numCalls++;
			}
			else {
				i = j;
				break;
			}
		}
	}

	outCollapsedList = outLines;

	//UPDATE PERCENT FRAME TIMES
	uint64_t totalCycles = 0;
	for (uint i = 0; i < outCollapsedList.size(); i++) {
		totalCycles += outCollapsedList[i].selfTimeCycles;
	}

	for (uint i = 0; i < outCollapsedList.size(); i++) {
		outCollapsedList[i].percentFrameTime = (double)outCollapsedList[i].selfTimeCycles / (double)totalCycles;
	}

	qsort(outCollapsedList.data(), outCollapsedList.size(), sizeof(ProfilerPrintedLine), ProfilerPrintedLineComparator);
}
void Profiler::CollapseListToListView(std::vector<ConstructedSample>& listToCollapse, std::vector<ProfilerPrintedLine>& outCollapsedList) {

	qsort(listToCollapse.data(), listToCollapse.size(), sizeof(ConstructedSample), SampleComparatorListView);

	for (uint i = 0; i < listToCollapse.size(); ) {

		if (i == listToCollapse.size())
			break;

		Sample* currSample = listToCollapse[i].sample;

		ProfilerPrintedLine nLine = ProfilerPrintedLine();
		nLine.tag = currSample->tag;
		nLine.numCalls = 1;
		nLine.totalCycles = currSample->end - currSample->start;
		nLine.totalTime = Time::ConvertPerformanceCountToTimeSeconds(currSample->start, currSample->end);
		nLine.selfTimeCycles = nLine.totalCycles;
		nLine.selfTime = nLine.totalTime;
		nLine.depth = listToCollapse[i].depth;
		nLine.percentFrameTime = 0.f;

		uint idxOfNextToCheck = 1;
		while ((i + idxOfNextToCheck) < listToCollapse.size()
			&& listToCollapse[i].depth - listToCollapse[i + idxOfNextToCheck].depth == -1)
		{
			Sample* nextSample = listToCollapse[i + idxOfNextToCheck].sample;
			nLine.selfTimeCycles -= nextSample->end - nextSample->start;
			nLine.selfTime -= Time::ConvertPerformanceCountToTimeSeconds(nextSample->start, nextSample->end);
			idxOfNextToCheck++;
		}

		idxOfNextToCheck = 1;
		while (i < (listToCollapse.size() - 1)
			&& listToCollapse[i].sample->tag == listToCollapse[i + idxOfNextToCheck].sample->tag
			&& listToCollapse[i].depth == listToCollapse[i + idxOfNextToCheck].depth)
		{
			Sample* nextSample = listToCollapse[i + idxOfNextToCheck].sample;
			nLine.totalCycles += (nextSample->end - nextSample->start);
			nLine.totalTime += Time::ConvertPerformanceCountToTimeSeconds(nextSample->start, nextSample->end);
			nLine.selfTimeCycles += (nextSample->end - nextSample->start);
			nLine.selfTime += Time::ConvertPerformanceCountToTimeSeconds(nextSample->start, nextSample->end);
			nLine.numCalls++;
			idxOfNextToCheck++;
		}

		outCollapsedList.push_back(nLine);
		i += idxOfNextToCheck;
	}

	//UPDATE PERCENT FRAME TIMES
	uint64_t totalCycles = 0;
	for (uint i = 0; i < outCollapsedList.size(); i++) {
		totalCycles += outCollapsedList[i].selfTimeCycles;
	}

	for (uint i = 0; i < outCollapsedList.size(); i++) {
		outCollapsedList[i].percentFrameTime = (double)outCollapsedList[i].selfTimeCycles / (double)totalCycles;
	}

	qsort(outCollapsedList.data(), outCollapsedList.size(), sizeof(ProfilerPrintedLine), ProfilerPrintedLineComparator);
}
void Profiler::PrintList(const std::vector<ProfilerPrintedLine>& collapsedList, eProfilerView view) {
	for (uint i = 0; i < collapsedList.size(); i++) {
		InternalPrintLine(collapsedList[i], view);
	}
}

void Profiler::InternalPrintLine(const ProfilerPrintedLine& lineToPrint, eProfilerView view) {

	//TAG FORMATTING
	char tag[256];
	if (view == PROFILER_LISTVIEW) {
		sprintf_s(tag, 256, "%*s%s", lineToPrint.depth * 2, " ", lineToPrint.tag);
	}
	
	//CALLS FORMATTING
	char numCalls[6];
	sprintf_s(numCalls, 6, "%u", lineToPrint.numCalls);

	char totalTimeStr[50];
	sprintf_s(totalTimeStr, 50, "%llu Cycles (%llums)", lineToPrint.totalCycles, lineToPrint.totalTime);

	char selfTimeStr[50];
	sprintf_s(selfTimeStr, 50, "%llu Cycles (%llums)", lineToPrint.selfTimeCycles, lineToPrint.selfTime);

	char finalOutput[1024];
	if (view == PROFILER_LISTVIEW) {
		sprintf_s(finalOutput, 1024, "%-40s %-10s %-35s %-35s %3.2f", tag, numCalls, totalTimeStr, selfTimeStr, lineToPrint.percentFrameTime);
	}
	else {
		sprintf_s(finalOutput, 1024, "%-40s %-10s %-35s %-35s %3.2f", lineToPrint.tag, numCalls, totalTimeStr, selfTimeStr, lineToPrint.percentFrameTime);
	}
	Console::PrintOutput(finalOutput, CONSOLE_NORMAL);
}

int SampleComparatorListView(void const *a, void const *b) {
	ConstructedSample *line1 = (ConstructedSample*)a;
	ConstructedSample *line2 = (ConstructedSample*)b;

	return COMPARE(line2->depth, line1->depth) && COMPARE(line2->sample->tag, line1->sample->tag);
}
int SampleComparatorFlatView(void const *a, void const *b) {
	ProfilerPrintedLine *line1 = (ProfilerPrintedLine*)a;
	ProfilerPrintedLine *line2 = (ProfilerPrintedLine*)b;

	return COMPARE(line2->tag, line1->tag);
}
int ProfilerPrintedLineComparator(void const *a, void const *b) {
	ProfilerPrintedLine *line1 = (ProfilerPrintedLine*)a;
	ProfilerPrintedLine *line2 = (ProfilerPrintedLine*)b;

	return COMPARE(line2->percentFrameTime, line1->percentFrameTime);
}

//---------------------------------------------------------------------------------------------------------------------------
//CONSOLE COMMAND
//---------------------------------------------------------------------------------------------------------------------------
void ProfilerToggle(Command& args) {
	String toggle;
	args.GetNextString(toggle);

	if (toggle == "enable") {
		Profiler::Enable();
		Console::PrintOutput("Profiler has been enabled.", CONSOLE_VERIFY);
	}
	else if (toggle == "disable") {
		Profiler::Disable();
		Console::PrintOutput("Profiler has been disabled.", CONSOLE_VERIFY);
	}
	else {
		Console::PrintOutput("Unrecognized input. Use <enable / disable>.", CONSOLE_VERIFY);
	}
}
void ProfilerPrintLastFrame(Command& args) {
	
	String view;
	String realTime;

	args.GetNextString(view);

	if (view == "listview") {
		Profiler::PrintLastFrame(PROFILER_LISTVIEW);
	}
	else if (view == "flatview") {
		Profiler::PrintLastFrame(PROFILER_FLATVIEW);
	}
	else {
		Profiler::PrintLastFrame(PROFILER_LISTVIEW);
	}

	if (args.GetNextString(realTime) && realTime == "realtime") {
		Profiler::EnableRealTime();
	}
}
void ProfilerDisableRealTime(Command& args) {
	UNUSED(args);
	Profiler::DisableRealTime();
	Console::PrintOutput("Profiler real-time frame printing disabled.", CONSOLE_VERIFY);
}