#pragma once
#pragma warning( disable : 4091)

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Utils/StringUtils.hpp"


//---------------------------------------------------------------------------------------------------------------------------
//STRUCTS
//---------------------------------------------------------------------------------------------------------------------------
struct Callstack {
	ulong hash;
	uint frame_count;
	void** frames;
};

struct CallstackLine {
	char filename[128];
	char function_name[128];
	uint32_t line;
	uint32_t offset;
	void* stackPtr;
};

//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION PROTOTYPES
//---------------------------------------------------------------------------------------------------------------------------

bool CallstackSystemInit();
void CallstackSystemDeinit();
Callstack* CallstackFetch(uint skip_frames);

CallstackLine* CallstackGetLines(Callstack* cs);
CallstackLine* CallstackGetFileLineAndCount(Callstack* cs);