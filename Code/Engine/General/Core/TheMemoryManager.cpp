#include "Engine/General/Core/TheMemoryManager.hpp"
#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/General/Core/Memory/DebugHUD.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"

DebugMemData* g_DebugMetadataCircularList = nullptr;

//---------------------------------------------------------------------------------------------------------------------------
//THE MEMORY MANAGER
//---------------------------------------------------------------------------------------------------------------------------

TheMemoryManager* g_theMemoryManager = nullptr;

TheMemoryManager::TheMemoryManager()
	: m_isEnabled(true)
	, m_age(5.f)
	, m_avgAllocBytesOverLastSecond(0)
	, m_avgFreedBytesOverLastSecond(0)
	, m_dAllocBytes(0)

{ 
	#if MEMORY_TRACKING == 2
		m_debugListConstructed = false;
		m_debugMemDatas = nullptr;
		m_numDebugMemData = 0;
		m_currFirstLineShown = 0;
	#endif
}

//---------------------------------------------------------------------------------------------------------------------------
//UPDATE
//---------------------------------------------------------------------------------------------------------------------------
void TheMemoryManager::Update(float deltaSeconds) {

	m_age += deltaSeconds;

	#if MEMORY_TRACKING == 2
	UpdateVerbose();
	#endif

	#if MEMORY_TRACKING != 0
	UpdateReg();
	#endif


}

#if MEMORY_TRACKING != 0
void TheMemoryManager::UpdateReg() {

	if (m_age >= 5.f) {
		m_avgAllocBytesOverLastSecond = s_avgAllocBytesOverLastSecond;
		m_avgFreedBytesOverLastSecond = s_avgFreedBytesOverLastSecond;
		s_avgAllocBytesOverLastSecond = 0;
		s_avgFreedBytesOverLastSecond = 0;

		m_age = 0.f;
	}
}
#endif

//---------------------------------------------------------------------------------------------------------------------------
//RENDER
//---------------------------------------------------------------------------------------------------------------------------
void TheMemoryManager::Render() const {
	if (m_isEnabled) {
		RenderWindows();
		RenderAllocations();
		RenderTags();

		#if MEMORY_TRACKING == 2
		RenderVerbose();
		#endif
	}
}

void TheMemoryManager::RenderWindows() const {

	//GENERAL WINDOW
	const Vector2* screenSize = BeirusRenderer::GetScreenSize();
	Vector2 bottomLeft = Vector2(10.f, screenSize->y - 200.f);
	Vector2 topRight = Vector2(500.f, screenSize->y - 10.f);

	BeirusRenderer::DrawAABB2(AABB2(bottomLeft, topRight), RGBA(0.f, 0.f, 0.f, 0.5f));

	//MEMORY TAG WINDOW
	bottomLeft = Vector2(10.f, screenSize->y - 400.f);
	topRight = Vector2(500.f, screenSize->y - 220.f);

	BeirusRenderer::DrawAABB2(AABB2(bottomLeft, topRight), RGBA(0.f, 0.f, 0.f, 0.5f));
}

void TheMemoryManager::RenderAllocations() const {

	float lineHeight = DebugHUD::GetDefaultTextLineHeight() * DEBUG_WINDOW_TEXT_SCALE;
	const Vector2* screenSize = BeirusRenderer::GetScreenSize();
	Vector2 startPos = Vector2(15.f, screenSize->y - 40.f);

	String totalBytesFormatted = StringUtils::FormatBytesForOutput(s_totalAllocatedBytes);
	String overallBytesFormatted = StringUtils::FormatBytesForOutput(s_overallAllocatedBytes);
	String avgAllocFormatted = StringUtils::FormatBytesForOutput(m_avgAllocBytesOverLastSecond);
	String avgFreedFormatted = StringUtils::FormatBytesForOutput(m_avgFreedBytesOverLastSecond);
	String avgChangeFormatted = StringUtils::FormatBytesForOutput(m_avgAllocBytesOverLastSecond - m_avgFreedBytesOverLastSecond);

	String currNumAllocs = StringUtils::Stringf("Current number of allocations: %i", s_numAllocations);
	String currNumAllocBytes = StringUtils::Stringf("Current number of allocated bytes: %s", totalBytesFormatted.c_str());
	String currNumTotalAllocBytes = StringUtils::Stringf("Number of total allocated bytes: %s", overallBytesFormatted.c_str());
	String avgBytesAllocLastSecond = StringUtils::Stringf("Average bytes allocated every second: %s", avgAllocFormatted.c_str());
	String avgBytesFreedLastSecond = StringUtils::Stringf("Average bytes freed every second: %s", avgFreedFormatted.c_str());
	String avgMemChange = StringUtils::Stringf("Average memory rate of change: %s", avgChangeFormatted.c_str());

	DebugHUD::DrawTextAtLocation(startPos, currNumAllocs, 0.4f, RGBA::WHITE);
	DebugHUD::DrawTextAtLocation(startPos - Vector2(0.f, lineHeight), currNumAllocBytes, 0.4f, RGBA::WHITE);
	DebugHUD::DrawTextAtLocation(startPos - Vector2(0.f, 2.f * lineHeight), currNumTotalAllocBytes, 0.4f, RGBA::WHITE);
	DebugHUD::DrawTextAtLocation(startPos - Vector2(0.f, 3.f * lineHeight), avgBytesAllocLastSecond, 0.4f, RGBA::WHITE);
	DebugHUD::DrawTextAtLocation(startPos - Vector2(0.f, 4.f * lineHeight), avgBytesFreedLastSecond, 0.4f, RGBA::WHITE);
	DebugHUD::DrawTextAtLocation(startPos - Vector2(0.f, 5.f * lineHeight), avgMemChange, 0.4f, RGBA::WHITE);
}

void TheMemoryManager::RenderTags() const {
	float lineHeight = DebugHUD::GetDefaultTextLineHeight() * DEBUG_WINDOW_TEXT_SCALE;
	const Vector2* screenSize = BeirusRenderer::GetScreenSize();
	Vector2 startPos = Vector2(15.f, screenSize->y - 220.f);

	String generalFormatted = StringUtils::FormatBytesForOutput(s_numBytesForGeneral);
	String physicsFormatted = StringUtils::FormatBytesForOutput(s_numBytesForPhysics);
	String AIFormatted = StringUtils::FormatBytesForOutput(s_numBytesForAI);
	String renderingFormatted = StringUtils::FormatBytesForOutput(s_numBytesForRendering);
	String gameplayFormatted = StringUtils::FormatBytesForOutput(s_numBytesForGameplay);
	String debuggingFormatted = StringUtils::FormatBytesForOutput(s_numBytesForDebugging);

	String numBytesGeneral = StringUtils::Stringf("Using %s for General", generalFormatted.c_str());
	String numBytesPhysics = StringUtils::Stringf("Using %s for Physics", physicsFormatted.c_str());
	String numBytesAI = StringUtils::Stringf("Using %s for AI", AIFormatted.c_str());
	String numBytesRendering = StringUtils::Stringf("Using %s for Rendering", renderingFormatted.c_str());
	String numBytesGameplay = StringUtils::Stringf("Using %s for Gameplay", gameplayFormatted.c_str());
	String numBytesDebugging = StringUtils::Stringf("Using %s for Debugging", debuggingFormatted.c_str());

	DebugHUD::DrawTextAtLocation(startPos - Vector2(0.f, lineHeight)      , numBytesGeneral, 0.4f, RGBA::WHITE);
	DebugHUD::DrawTextAtLocation(startPos - Vector2(0.f, 2.f * lineHeight), numBytesPhysics, 0.4f, RGBA::WHITE);
	DebugHUD::DrawTextAtLocation(startPos - Vector2(0.f, 3.f * lineHeight), numBytesAI, 0.4f, RGBA::WHITE);
	DebugHUD::DrawTextAtLocation(startPos - Vector2(0.f, 4.f * lineHeight), numBytesRendering, 0.4f, RGBA::WHITE);
	DebugHUD::DrawTextAtLocation(startPos - Vector2(0.f, 5.f * lineHeight), numBytesGameplay, 0.4f, RGBA::WHITE);
	DebugHUD::DrawTextAtLocation(startPos - Vector2(0.f, 6.f * lineHeight), numBytesDebugging, 0.4f, RGBA::WHITE);
}

//---------------------------------------------------------------------------------------------------------------------------
void MemoryAnalyticsStartup() {
	DebuggerPrintf("Num Allocations on Startup: %i\n", s_numAllocations);
	DebuggerPrintf("Allocated Bytes on Startup: %i\n", s_totalAllocatedBytes);

	s_numAllocationsAtStartup = s_numAllocations;

	CallstackSystemInit();

	g_theMemoryManager = new(MEMORY_DEBUGGING) TheMemoryManager();
}

void MemoryAnalyticsShutdown() {

	delete g_theMemoryManager;
	g_theMemoryManager = nullptr;

	DebuggerPrintf("Num Allocations on Shutdown: %i\n", s_numAllocations);
	DebuggerPrintf("Allocated Bytes on Shutdown: %i\n", s_totalAllocatedBytes);

	#if MEMORY_TRACKING == 2
	MemFlush();
	#endif

	if (s_numAllocations != s_numAllocationsAtStartup) {

		char errStr[100];
		sprintf_s(errStr, "ERROR: Memory leaks detected. \n Number of leaks: %u", s_numAllocations);
		ERROR_AND_DIE(errStr);
	}

	CallstackSystemDeinit();
}

#if MEMORY_TRACKING == 2
void PrintCallstack(Callstack* cs, uint numBytes) {

	//DebugMemData* head = g_DebugMetadataCircularList;
	CallstackLine* pCallstack = CallstackGetLines(cs);

	DebuggerPrintf("CALLSTACK %lu:\n%s(%u): %p Leaked %u bytes.\n", cs->hash, pCallstack->filename, pCallstack->line, pCallstack->stackPtr, numBytes);
	for (uint i = 0; i < cs->frame_count; i++) {
		DebuggerPrintf("\t%s(%u): %p %s\n", pCallstack->filename, pCallstack->line, pCallstack->stackPtr, pCallstack->function_name);
		pCallstack++;
	}
}
#endif

#if MEMORY_TRACKING == 2
void TheMemoryManager::RenderVerbose() const {
	//RENDER WINDOW
	Vector2 screenSize = BeirusRenderer::GetScreenSize();
	Vector2 bottomLeft = Vector2(550.f, screenSize.y - 200.f);
	Vector2 topRight = Vector2(1550.f, screenSize.y - 10.f);

	BeirusRenderer::DrawAABB2(AABB2(bottomLeft, topRight), RGBA(0.f, 0.f, 0.f, 0.5f), PRIMITIVE_QUADS);

	//RENDER OUTPUT
	int idx = 0;
	float lineHeight = BeirusEngine::GetDefaultTextLineHeight() * DEBUG_WINDOW_TEXT_SCALE;
	Vector2 startPos = Vector2(550.f, screenSize.y - 40.f);

	for (unsigned int i = m_currFirstLineShown; i < m_currFirstLineShown + MAX_VERBOSE_LINES_SHOWN; i++) {

		String allocData = StringUtils::Stringf("Allocation %i File: %s Line: %u NumBytes: %u", i + 1, m_debugMemDatas[i]->filename, m_debugMemDatas[i]->line, m_debugMemDatas[i]->numBytes);
		DebugHUD::DrawTextAtLocation(startPos - Vector2(0.f, (float)idx * lineHeight), allocData, 0.4f, RGBA::WHITE);

		idx++;
	}
}
void TheMemoryManager::UpdateVerbose() {
	if (m_age >= 5.f) {

		if (m_debugListConstructed) {
			free(m_debugMemDatas);
			m_numDebugMemData = 0;
		}

		//Copy metadata list to an array and sort it
		DebugMemData** callstackArr = (DebugMemData**)malloc(sizeof(DebugMemData*) * s_numCallstacks);
		DebugMemData** callstackArrFront = callstackArr;

		DebugMemData* headOfGlobal = g_DebugMetadataCircularList;
		DebugMemData* currOfGlobal = headOfGlobal;
		for (uint i = 0; i < s_numCallstacks; i++) {
			callstackArr[i] = currOfGlobal;

			CallstackLine* callstackLines = CallstackGetFileLineAndCount(callstackArr[i]->cs);
			strcpy_s(callstackArr[i]->filename, callstackLines->filename);
			callstackArr[i]->line = callstackLines->line;
			callstackArr[i]->hasFileAndLine = true;

			currOfGlobal = currOfGlobal->next;
		}

		qsort(callstackArrFront, s_numCallstacks, sizeof(callstackArrFront[0]), CallstackComparator);

		m_numDebugMemData = s_numCallstacks;
		m_debugMemDatas = callstackArrFront;
	}

	//UPDATE CURR FIRST LINE SHOWN
	if (InputSystem::GetKeyDown('V')) { //UP
		if (m_currFirstLineShown > 0) {
			m_currFirstLineShown--;
		}
	}
	else if (InputSystem::GetKeyDown('Z')) { //DOWN
		if (m_currFirstLineShown <= m_numDebugMemData - MAX_VERBOSE_LINES_SHOWN) {
			m_currFirstLineShown++;
		}
	}
}

void MemFlush() {

	if (s_numCallstacks != 0 && s_numCallstacks != 1) {
		//Copy metadata list to an array and sort it
		DebugMemData** callstackArr = (DebugMemData**)malloc(sizeof(DebugMemData*) * s_numCallstacks);
		DebugMemData** callstackArrFront = callstackArr;

		DebugMemData* headOfGlobal = g_DebugMetadataCircularList;
		DebugMemData* currOfGlobal = headOfGlobal->next;
		*callstackArr = currOfGlobal;
		callstackArr++;
		while (currOfGlobal != headOfGlobal) {
			*callstackArr = currOfGlobal;
			callstackArr++;
			currOfGlobal = currOfGlobal->next;
		}

		qsort(callstackArrFront, s_numCallstacks, sizeof(callstackArrFront[0]), CallstackComparator);


		DebugMemData* prev = callstackArrFront[0];
		uint idx = 0;

		while (idx < s_numCallstacks - 1) {

			uint thisStackBytesAllocated = 0;
			uint numSimilar = 0;

			while (callstackArrFront[idx]->cs->hash == prev->cs->hash && idx < s_numCallstacks - 1) {

				thisStackBytesAllocated += callstackArrFront[idx]->numBytes;
				idx++;
				numSimilar++;
			}

			DebuggerPrintf("Num Similar: %u", numSimilar);
			PrintCallstack(prev->cs, thisStackBytesAllocated);
			prev = callstackArrFront[idx];
		}

		free(callstackArrFront);
	}
	else if (s_numCallstacks == 1) {
		DebuggerPrintf("Num Similar: 1");
		PrintCallstack(g_DebugMetadataCircularList->cs, g_DebugMetadataCircularList->numBytes);
	}
}
int CallstackComparator(const void* a, const void* b) {

	const DebugMemData* aDMB = *(DebugMemData**)a;
	const DebugMemData* bDMB = *(DebugMemData**)b;

	const Callstack* aCS = aDMB->cs;
	const Callstack* bCS = bDMB->cs;

	ulong val = aCS->hash - bCS->hash;

	return (int)val;
}
#endif



//---------------------------------------------------------------------------------------------------------------------------
//OPERATOR OVERLOADS: NEW
//---------------------------------------------------------------------------------------------------------------------------
void* operator new(size_t numBytes) {

	#if MEMORY_TRACKING == 2 
		return NewMemoryVerbose(numBytes, MEMORY_GENERAL);
	#elif MEMORY_TRACKING == 1 
		return NewMemoryDebug(numBytes, MEMORY_GENERAL);
	#else 
		void* ptr = malloc(numBytes);
		return ptr;
	#endif
}
void* operator new[](size_t numBytes) {

	#if MEMORY_TRACKING == 2 
		return NewMemoryVerbose(numBytes, MEMORY_GENERAL);
	#elif MEMORY_TRACKING == 1 
		return NewMemoryDebug(numBytes, MEMORY_GENERAL);
	#else 
		void* ptr = malloc(numBytes);
		return ptr;
	#endif
}
void* operator new(size_t numBytes, eMemoryTag memTag) {

	memTag;

	#if MEMORY_TRACKING == 2 
		return NewMemoryVerbose(numBytes, memTag);
	#elif MEMORY_TRACKING == 1 
		return NewMemoryDebug(numBytes, memTag);
	#else 
		void* ptr = malloc(numBytes);
		return ptr;
	#endif
}
void* operator new[](size_t numBytes, eMemoryTag memTag) {

	memTag;

	#if MEMORY_TRACKING == 2 
		return NewMemoryVerbose(numBytes, memTag);
	#elif MEMORY_TRACKING == 1 
		return NewMemoryDebug(numBytes, memTag);
	#else 
		void* ptr = malloc(numBytes);
		return ptr;
	#endif
}

//---------------------------------------------------------------------------------------------------------------------------
#if MEMORY_TRACKING == 1
void* NewMemoryDebug(size_t numBytes, eMemoryTag memTag) {
	RegDebugData* ptr = (RegDebugData*)malloc(sizeof(RegDebugData) + numBytes);

	ptr->numBytes = numBytes;
	ptr->memTag = MemoryTagToSize_t(memTag);

	ptr++;

	switch (memTag) {
	case MEMORY_GENERAL:
		s_numBytesForGeneral += numBytes;
		break;
	case MEMORY_PHYSICS:
		s_numBytesForPhysics += numBytes;
		break;
	case MEMORY_AI:
		s_numBytesForAI += numBytes;
		break;
	case MEMORY_RENDERING:
		s_numBytesForRendering += numBytes;
		break;
	case MEMORY_GAMEPLAY:
		s_numBytesForGameplay += numBytes;
		break;
	case MEMORY_DEBUGGING:
		s_numBytesForDebugging += numBytes;
		break;
	}

	s_numAllocations++;
	s_totalAllocatedBytes += numBytes;
	s_overallAllocatedBytes += numBytes;
	s_avgAllocBytesOverLastSecond += numBytes;

	return ptr;
}
#elif MEMORY_TRACKING == 2
void* NewMemoryVerbose(size_t numBytes, eMemoryTag memTag) {
	//Allocate memory needed for the new and the debug metadata
	Callstack* nCallstack = CallstackFetch(0);

	DebugMemData* nMetadata = (DebugMemData*)malloc(sizeof(DebugMemData) + numBytes);

	//Set the other debug metadata stuff
	nMetadata->numBytes = numBytes;
	nMetadata->callstackSize = sizeof(Callstack) + nCallstack->frame_count * sizeof(void*);
	nMetadata->cs = nCallstack;
	nMetadata->hasFileAndLine = false;
	nMetadata->memTag = memTag;

	switch (memTag) {
	case MEMORY_GENERAL:
		s_numBytesForGeneral += numBytes;
		break;
	case MEMORY_PHYSICS:
		s_numBytesForPhysics += numBytes;
		break;
	case MEMORY_AI:
		s_numBytesForAI += numBytes;
		break;
	case MEMORY_RENDERING:
		s_numBytesForRendering += numBytes;
		break;
	case MEMORY_GAMEPLAY:
		s_numBytesForGameplay += numBytes;
		break;
	case MEMORY_DEBUGGING:
		s_numBytesForDebugging += numBytes;
		break;
	}

	//Insert it into the circular linked list
	if (s_numCallstacks == 0) {
		g_DebugMetadataCircularList = nMetadata;
		g_DebugMetadataCircularList->prev = g_DebugMetadataCircularList;
		g_DebugMetadataCircularList->next = g_DebugMetadataCircularList;
	}
	else {
		nMetadata->next = g_DebugMetadataCircularList->next;
		nMetadata->next->prev = nMetadata;
		nMetadata->prev = g_DebugMetadataCircularList;
		g_DebugMetadataCircularList->next = nMetadata;
		g_DebugMetadataCircularList = nMetadata;
	}

	//Increment global mem debug stuff
	s_numCallstacks++;
	s_numAllocations++;
	s_totalAllocatedBytes += numBytes;
	s_overallAllocatedBytes += numBytes;
	s_avgAllocBytesOverLastSecond += numBytes;

	//Move ptr forward and return
	nMetadata++;
	return nMetadata;
}
#endif

//---------------------------------------------------------------------------------------------------------------------------
//OPERATOR OVERLOADS: DELETE
//---------------------------------------------------------------------------------------------------------------------------
void operator delete(void* ptr) {

	#if MEMORY_TRACKING == 2
		FreeMemoryVerbose(ptr, MEMORY_GENERAL);
	#elif MEMORY_TRACKING == 1
		FreeMemoryDebug(ptr, MEMORY_GENERAL);
	#elif MEMORY_TRACKING == 0
		free(ptr);
	#endif
}
void operator delete[](void* ptr) {

	#if MEMORY_TRACKING == 2
		FreeMemoryVerbose(ptr, MEMORY_GENERAL);
	#elif MEMORY_TRACKING == 1
		FreeMemoryDebug(ptr, MEMORY_GENERAL);
	#elif MEMORY_TRACKING == 0
		free(ptr);
	#endif
}
void operator delete(void* ptr, eMemoryTag memoryTag) {

	#if MEMORY_TRACKING == 2
		FreeMemoryVerbose(ptr, memoryTag);
	#elif MEMORY_TRACKING == 1
		FreeMemoryDebug(ptr, memoryTag);
	#elif MEMORY_TRACKING == 0
		memoryTag;
		free(ptr);
	#endif
}
void operator delete[](void* ptr, eMemoryTag memoryTag) {

	#if MEMORY_TRACKING == 2
		FreeMemoryVerbose(ptr, memoryTag);
	#elif MEMORY_TRACKING == 1
		FreeMemoryDebug(ptr, memoryTag);
	#elif MEMORY_TRACKING == 0
		memoryTag;
		free(ptr);
	#endif
}

//---------------------------------------------------------------------------------------------------------------------------
#if MEMORY_TRACKING == 1
void FreeMemoryDebug(void* ptr, eMemoryTag memoryTag) {
	UNREFERENCED_PARAMETER(memoryTag);
	byte* movePtr = (byte*)ptr;
	movePtr -= sizeof(RegDebugData);
	RegDebugData* metadata = (RegDebugData*)movePtr;

	size_t numBytes = metadata->numBytes;
	eMemoryTag tag = Size_tToMemoryTag(metadata->memTag);

	switch (tag) {
	case MEMORY_GENERAL:
		s_numBytesForGeneral -= numBytes;
		break;
	case MEMORY_PHYSICS:
		s_numBytesForPhysics -= numBytes;
		break;
	case MEMORY_AI:
		s_numBytesForAI -= numBytes;
		break;
	case MEMORY_RENDERING:
		s_numBytesForRendering -= numBytes;
		break;
	case MEMORY_GAMEPLAY:
		s_numBytesForGameplay -= numBytes;
		break;
	case MEMORY_DEBUGGING:
		s_numBytesForDebugging -= numBytes;
		break;
	}

	s_totalAllocatedBytes -= numBytes;
	s_avgFreedBytesOverLastSecond += numBytes;
	s_numAllocations--;

	free(movePtr);
}
#elif MEMORY_TRACKING == 2
void FreeMemoryVerbose(void* ptr, eMemoryTag memoryTag) {
	UNREFERENCED_PARAMETER(memoryTag);
	byte* movePtr = (byte*)ptr;
	movePtr -= sizeof(DebugMemData);
	DebugMemData* metadata = (DebugMemData*)movePtr;

	//Clear from the list
/*
	DebugMemData* curr = g_DebugMetadataCircularList;
	while (curr != metadata) {
		curr = curr->next;
	}*/

	/* If node to be deleted is head node */
	if (g_DebugMetadataCircularList == metadata)
		g_DebugMetadataCircularList = metadata->next;

	/* Change next only if node to be deleted is NOT the last node */
	if (metadata->next != NULL)
		metadata->next->prev = metadata->prev;

	/* Change prev only if node to be deleted is NOT the first node */
	if (metadata->prev != NULL)
		metadata->prev->next = metadata->next;

/*
	if (curr == g_DebugMetadataCircularList) {
		g_DebugMetadataCircularList = curr->next;
	}*/

	//Update the linked list
/*
	curr->prev->next = curr->next;
	curr->next->prev = curr->prev;*/

	eMemoryTag memTag = metadata->memTag;
	uint numBytes = metadata->numBytes;

	switch (memTag) {
	case MEMORY_GENERAL:
		s_numBytesForGeneral -= numBytes;
		break;
	case MEMORY_PHYSICS:
		s_numBytesForPhysics -= numBytes;
		break;
	case MEMORY_AI:
		s_numBytesForAI -= numBytes;
		break;
	case MEMORY_RENDERING:
		s_numBytesForRendering -= numBytes;
		break;
	case MEMORY_GAMEPLAY:
		s_numBytesForGameplay -= numBytes;
		break;
	case MEMORY_DEBUGGING:
		s_numBytesForDebugging -= numBytes;
		break;
	}

	//Free the callstack
	Callstack* cs = metadata->cs;
	free(cs);

	s_totalAllocatedBytes -= metadata->numBytes;
	s_avgFreedBytesOverLastSecond += metadata->numBytes;
	s_numAllocations--;
	s_numCallstacks--;

	free(movePtr);
}
#endif

//---------------------------------------------------------------------------------------------------------------------------
//MEM TAG ENUM SWITCHES
//---------------------------------------------------------------------------------------------------------------------------
#if MEMORY_TRACKING == 1
size_t MemoryTagToSize_t(eMemoryTag tag) {

	switch (tag) {
	case MEMORY_PHYSICS:
		return 0;
	case MEMORY_RENDERING:
		return 1;
	case MEMORY_GENERAL:
		return 2;
	case MEMORY_AI:
		return 3;
	case MEMORY_GAMEPLAY:
		return 4;
	case MEMORY_DEBUGGING:
		return 5;
	default:
		return 99;
	}

}
eMemoryTag Size_tToMemoryTag(size_t tag) {

	switch (tag) {
	case 0:
		return MEMORY_PHYSICS;
	case 1:
		return MEMORY_RENDERING;
	case 2:
		return MEMORY_GENERAL;
	case 3:
		return MEMORY_AI;
	case 4:
		return MEMORY_GAMEPLAY;
	case 5:
		return MEMORY_DEBUGGING;
	default:
		return MEMORY_GENERAL;
	}
}
#endif