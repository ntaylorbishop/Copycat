#include "Engine/General/Core/Memory/Callstack.hpp"

#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <Windows.h>
#include <DbgHelp.h>

//---------------------------------------------------------------------------------------------------------------------------
//DEFINES CONSTANTS
//---------------------------------------------------------------------------------------------------------------------------
#define MAX_SYMBOL_NAME_LENGTH 128
#define MAX_FILENAME_LENGTH 1024
#define MAX_DEPTH 128

//---------------------------------------------------------------------------------------------------------------------------
//FUNCTION POINTERS
//---------------------------------------------------------------------------------------------------------------------------

// SymInitialize()
typedef BOOL(__stdcall *sym_initialize_t)(IN HANDLE hProcess, IN PSTR UserSearchPath, IN BOOL fInvadeProcess);
typedef BOOL(__stdcall *sym_cleanup_t)(IN HANDLE hProcess);
typedef BOOL(__stdcall *sym_from_addr_t)(IN HANDLE hProcess, IN DWORD64 Address, OUT PDWORD64 Displacement, OUT PSYMBOL_INFO Symbol);

typedef BOOL(__stdcall *sym_get_line_t)(IN HANDLE hProcess, IN DWORD64 dwAddr, OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_LINE64 Symbol);

//---------------------------------------------------------------------------------------------------------------------------
//LOCAL VARS
//---------------------------------------------------------------------------------------------------------------------------
static HMODULE gDebugHelp;
static HANDLE gProcess;
static SYMBOL_INFO  *gSymbol;

// only called from single thread - so can use a shared buffer
static char gFileName[MAX_FILENAME_LENGTH];
static CallstackLine gCallstackBuffer[MAX_DEPTH];

static sym_initialize_t LSymInitialize;
static sym_cleanup_t LSymCleanup;
static sym_from_addr_t LSymFromAddr;
static sym_get_line_t LSymGetLineFromAddr64;

//---------------------------------------------------------------------------------------------------------------------------
//FUNCS
//---------------------------------------------------------------------------------------------------------------------------
bool CallstackSystemInit() {

	gDebugHelp = LoadLibraryA("dbghelp.dll");
	ASSERT_OR_DIE(gDebugHelp != nullptr, "ERROR: Could not load DebugHelp");
	LSymInitialize = (sym_initialize_t)GetProcAddress(gDebugHelp, "SymInitialize");
	LSymCleanup = (sym_cleanup_t)GetProcAddress(gDebugHelp, "SymCleanup");
	LSymFromAddr = (sym_from_addr_t)GetProcAddress(gDebugHelp, "SymFromAddr");
	LSymGetLineFromAddr64 = (sym_get_line_t)GetProcAddress(gDebugHelp, "SymGetLineFromAddr64");

	gProcess = GetCurrentProcess();
	LSymInitialize(gProcess, NULL, TRUE);

	gSymbol = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO) + (MAX_FILENAME_LENGTH * sizeof(char)));

	gSymbol->MaxNameLen = MAX_FILENAME_LENGTH;
	gSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
static int gCallstackCount = 0;
void CallstackSystemDeinit() {
	LSymCleanup(gProcess);

	FreeLibrary(gDebugHelp);
	gDebugHelp = NULL;

	if (gCallstackCount != 0) {
		gCallstackCount = 0;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
Callstack* CallstackFetch(uint skip_frames) {
	void *stack[MAX_DEPTH];
	ulong* hash = (ulong*)malloc(sizeof(ulong));
	uint32_t frames = CaptureStackBackTrace(skip_frames, MAX_DEPTH, stack, hash);

	gCallstackCount++;

	size_t size = sizeof(Callstack) + sizeof(void*) * frames;

	Callstack* cs = (Callstack*)malloc(size);

	byte* movedPtr = (byte*)cs;
	movedPtr += sizeof(Callstack);
	cs->hash = *hash;
	cs->frames = (void**)movedPtr;
	cs->frame_count = frames;

	memcpy(cs->frames, stack, sizeof(void*) * frames);

	free(hash);

	return cs;
}

//---------------------------------------------------------------------------------------------------------------------------
// Should only be called from the debug trace thread.  
CallstackLine* CallstackGetLines(Callstack* cs) {
	IMAGEHLP_LINE64 LineInfo;
	DWORD LineDisplacement = 0; // Displacement from the beginning of the line 
	LineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

	uint count = cs->frame_count;
	for (uint i = 0; i < count; ++i) {
		CallstackLine* line = &(gCallstackBuffer[i]);
		DWORD64 ptr = (DWORD64)(cs->frames[i]);
		LSymFromAddr(gProcess, ptr, 0, gSymbol);

		StringUtils::CStringCopy(line->function_name, gSymbol->Name, 128);


		BOOL bRet = LSymGetLineFromAddr64(
			GetCurrentProcess(), // Process handle of the current process 
			ptr, // Address 
			&LineDisplacement, // Displacement will be stored here by the function 
			&LineInfo);         // File name / line information will be stored here 

		if (bRet) {
			line->line = LineInfo.LineNumber;

			line->stackPtr = cs->frames[i];

			char const *filename = LineInfo.FileName;
			if (filename == NULL) {
				filename = LineInfo.FileName;
			}
/*
			else {
				filename += 5; // skip to the important bit - so that it can be double clicked in Output
			}*/


			StringUtils::CStringCopy(line->filename, filename, 128);
			line->offset = LineDisplacement;
		}
		else {
			line->line = 0;
			line->offset = 0;
			StringUtils::CStringCopy(line->filename, "N/A", 128);
		}
	}

	return gCallstackBuffer;
}
CallstackLine* CallstackGetFileLineAndCount(Callstack* cs) {
	uint skipFrames = 3;

	IMAGEHLP_LINE64 LineInfo;
	DWORD LineDisplacement = 0; // Displacement from the beginning of the line 
	LineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

	CallstackLine* line = &(gCallstackBuffer[0]);
	DWORD64 ptr = (DWORD64)(cs->frames[skipFrames]);
	LSymFromAddr(gProcess, ptr, 0, gSymbol);

	StringUtils::CStringCopy(line->function_name, gSymbol->Name, 128);

	BOOL bRet = LSymGetLineFromAddr64(GetCurrentProcess(), ptr, &LineDisplacement, &LineInfo);

	if (bRet) {
		line->line = LineInfo.LineNumber;

		line->stackPtr = cs->frames[skipFrames];

		char const *filename = LineInfo.FileName;
		if (filename == NULL) {
			filename = LineInfo.FileName;
		}
		filename += 45;

		StringUtils::CStringCopy(line->filename, filename, 128);
		line->offset = LineDisplacement;
	}
	else {
		line->line = 0;
		line->offset = 0;
		StringUtils::CStringCopy(line->filename, "N/A", 128);
	}

	return gCallstackBuffer;
}