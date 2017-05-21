#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Config/BuildConfig.hpp"
#include "Engine/Utils/ErrorWarningAssert.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/Renderer/Fonts/Font.hpp"
#include "Engine/General/Core/Memory/Callstack.hpp"
#include <stdlib.h>

class BeirusEngine;

//---------------------------------------------------------------------------------------------------------------------------
//ENUMS
//---------------------------------------------------------------------------------------------------------------------------
enum eMemoryTag {
	MEMORY_PHYSICS = 0,
	MEMORY_RENDERING,
	MEMORY_GENERAL,
	MEMORY_AI,
	MEMORY_GAMEPLAY,
	MEMORY_DEBUGGING
};

//---------------------------------------------------------------------------------------------------------------------------
//OPERATOR OVERLOADS
//---------------------------------------------------------------------------------------------------------------------------

void* operator new(size_t numBytes);
void* operator new[](size_t numBytes);
void* operator new(size_t numBytes, eMemoryTag memTag);
void* operator new[](size_t numBytes, eMemoryTag memTag);

#if MEMORY_TRACKING == 1
void* NewMemoryDebug(size_t numBytes, eMemoryTag memTag);
#elif MEMORY_TRACKING == 2
void* NewMemoryVerbose(size_t numBytes, eMemoryTag memTag);
#endif

void operator delete(void* ptr);
void operator delete[](void* ptr);
void operator delete(void* ptr, eMemoryTag memoryTag);
void operator delete[](void* ptr, eMemoryTag memoryTag);

void FreeMemoryDebug(void* ptr, eMemoryTag memoryTag);
void FreeMemoryVerbose(void* ptr, eMemoryTag memoryTag);

//---------------------------------------------------------------------------------------------------------------------------
//CONSTS AND STATICS
//---------------------------------------------------------------------------------------------------------------------------
class TheMemoryManager;
extern TheMemoryManager* g_theMemoryManager;

const float DEBUG_WINDOW_TEXT_SCALE = 0.4f;
const uint DEBUG_MAX_ALLOC_STACKS = 3000;

static int s_numAllocationsAtStartup = 0;
static int s_numAllocations = 0;
static unsigned int s_totalAllocatedBytes = 0;
static unsigned int s_overallAllocatedBytes = 0;

static uint s_numAllocsOverLastSecond = 0;
static uint s_avgAllocBytesOverLastSecond = 0;
static uint s_avgFreedBytesOverLastSecond = 0;
static float s_dAvgAlloc = 0;

static uint s_numCallstacks = 0;

static uint s_numBytesForGeneral = 0;
static uint s_numBytesForPhysics = 0;
static uint s_numBytesForAI = 0;
static uint s_numBytesForRendering = 0;
static uint s_numBytesForGameplay = 0;
static uint s_numBytesForDebugging = 0;

void MemoryAnalyticsStartup();
void MemoryAnalyticsShutdown();

#if MEMORY_TRACKING == 2
void PrintCallstack(Callstack* cs, uint numBytes);
#endif

//---------------------------------------------------------------------------------------------------------------------------
//Structs
//---------------------------------------------------------------------------------------------------------------------------
struct RegDebugData {
	size_t numBytes;
	size_t memTag;
};

struct DebugMemData {
	size_t numBytes;
	size_t callstackSize;
	Callstack* cs;
	char filename[128];
	uint32_t line;
	bool hasFileAndLine;
	eMemoryTag memTag;

	DebugMemData *prev, *next;
};

extern DebugMemData* g_DebugMetadataCircularList;

//---------------------------------------------------------------------------------------------------------------------------
//MEMORY MANAGER CLASS
//---------------------------------------------------------------------------------------------------------------------------

const uint MAX_VERBOSE_LINES_SHOWN = 7;

class TheMemoryManager {
public:
	TheMemoryManager();

	void Update(float deltaSeconds);

	#if MEMORY_TRACKING != 0
	void UpdateReg();
	#endif

	void Render() const;

	//GETTERS SETTERS
	void Toggle() { m_isEnabled = !m_isEnabled; }
	bool GetToggledStatus() const { return m_isEnabled; }

private:
	void RenderWindows() const;
	void RenderAllocations() const;
	void RenderTags() const;

	bool m_isEnabled;
	float m_age;

	uint m_avgAllocBytesOverLastSecond;
	uint m_avgFreedBytesOverLastSecond;
	float m_dAllocBytes;

#if MEMORY_TRACKING == 2
	void RenderVerbose() const;
	void UpdateVerbose();

	DebugMemData** m_debugMemDatas;
	uint m_numDebugMemData;
	bool m_debugListConstructed;
	uint m_currFirstLineShown;
#endif

};

#if MEMORY_TRACKING == 2
void MemFlush();

int CallstackComparator(const void* a, const void* b);
#endif

#if MEMORY_TRACKING == 1
size_t MemoryTagToSize_t(eMemoryTag tag);
eMemoryTag Size_tToMemoryTag(size_t tag);
#endif