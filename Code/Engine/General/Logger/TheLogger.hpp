#pragma once

#include <fstream>
#include <iostream>

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Utils/StringUtils.hpp"
#include "Engine/Structures/ThreadSafe/Thread.hpp"
#include "Engine/Structures/ThreadSafe/TSQueue.hpp"
#include "Engine/Structures/ThreadSafe/MutexLock.hpp"


//---------------------------------------------------------------------------------------------------------------------------
//ENUMS, TYPEDEFS, STRUCTS, CONSTS
//---------------------------------------------------------------------------------------------------------------------------
typedef void(WriteFunctionCallback)(const String&);

enum eLogLevel {
	LOG_NONE = 0,
	LOG_SEVERE,
	LOG_RECOVERABLE,
	LOG_DEFAULT,
	LOG_ALL
};

enum eListMode {
	LOG_WHITELIST = 0,
	LOG_BLACKLIST
};

struct LogMessage {
	std::string msg;
	std::string tag;
	eLogLevel logLvl;
	bool printCallstack;
};

const char LOG_FILE_PATH[] = "Data/Logs/";
const char LOG_FILE_NAME[] = "debug";

//---------------------------------------------------------------------------------------------------------------------------
//LOGGER CLASS
//---------------------------------------------------------------------------------------------------------------------------
class TheLogger {
public:
	 //INTERFACE
	static void LogPrintf(const char* messageFormat, ...);
	static void LogPrintf(const char* messageFormat, const char* tag, eLogLevel logLevel, ...);
	static void LogPrintfWithCallstack(const char* messageFormat, const char* tag, eLogLevel logLevel, ...);
	 
	static void LogEnable(const char* tag);
	static void LogDisable(const char* tag);
	static void LogDisableAll();

	//MESSAGE HANDLING
	static void HandleMessage(LogMessage msgToProcess);
	static void FlushRemainingMessages();

private:
	TheLogger();
	~TheLogger();

	//UTILS
	static void MakeCopyOfLog();
	static String GetLogFileNameToCreate();
	static void OpenDebugFile();

	//RUNNING
	static void Run();

	//OUTPUT WRITING
	static void WriteToOutputWindow(const String& output);
	static void WriteToDevConsole(const String& output);
	static void WriteToFile(const String& output);



	Thread m_thread;
	std::fstream file;
	std::vector<WriteFunctionCallback*> m_writeCallbacks;

	eLogLevel m_logLevel;

	TSQueue<LogMessage> m_messageBuffer;
	MutexLock m_listedTagsLock;
	std::vector<std::string> m_listedTags;
	eListMode m_logListMode;
	std::fstream m_file;
	String m_fileName;


	static TheLogger* s_theLogger;
};