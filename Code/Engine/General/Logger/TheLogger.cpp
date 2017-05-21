#include "Engine/General/Logger/TheLogger.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Utils/FileUtils/FileUtils.hpp"
#include "Engine/Config/BuildConfig.hpp"
#include "Engine/General/Time/Time.hpp"
#include "Engine/General/Core/Memory/Callstack.hpp"
#include "Engine/General/Core/BeirusEngine.hpp"

STATIC TheLogger* TheLogger::s_theLogger = nullptr;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STATIC INTERFACE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void TheLogger::LogPrintf(const char* messageFormat, ...) {

	if (!s_theLogger) {
		s_theLogger				= new TheLogger();
		s_theLogger->m_thread	= Thread(&Run);
	}

	const int MESSAGE_MAX_LENGTH = 2048;
	char messageLiteral[MESSAGE_MAX_LENGTH];
	va_list variableArgumentList;
	va_start(variableArgumentList, messageFormat);
	vsnprintf_s(messageLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, messageFormat, variableArgumentList);
	va_end(variableArgumentList);
	messageLiteral[MESSAGE_MAX_LENGTH - 1] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	LogMessage msgToAdd;
	msgToAdd.msg			= StringUtils::Stringf("%s\n", messageLiteral);
	msgToAdd.tag			= "DEFAULT";
	msgToAdd.logLvl			= LOG_DEFAULT;
	msgToAdd.printCallstack = false;
	s_theLogger->m_messageBuffer.Enqueue(msgToAdd);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void TheLogger::LogPrintf(const char* messageFormat, const char* tag, eLogLevel logLevel, ...) {

	if (!s_theLogger) {
		s_theLogger				= new TheLogger();
		s_theLogger->m_thread	= Thread(&Run);
	}

	const int MESSAGE_MAX_LENGTH = 2048;
	char messageLiteral[MESSAGE_MAX_LENGTH];
	va_list variableArgumentList;
	va_start(variableArgumentList, messageFormat);
	vsnprintf_s(messageLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, messageFormat, variableArgumentList);
	va_end(variableArgumentList);
	messageLiteral[MESSAGE_MAX_LENGTH - 1] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	LogMessage msgToAdd;
	msgToAdd.msg				= StringUtils::Stringf("%s\n", messageLiteral);
	msgToAdd.tag				= tag;
	msgToAdd.logLvl				= logLevel;
	msgToAdd.printCallstack		= false;
	s_theLogger->m_messageBuffer.Enqueue(msgToAdd);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void TheLogger::LogPrintfWithCallstack(const char* messageFormat, const char* tag, eLogLevel logLevel, ...) {

	if (!s_theLogger) {
		s_theLogger				= new TheLogger();
		s_theLogger->m_thread	= Thread(&Run);
	}

	const int MESSAGE_MAX_LENGTH = 2048;
	char messageLiteral[MESSAGE_MAX_LENGTH];
	va_list variableArgumentList;
	va_start(variableArgumentList, messageFormat);
	vsnprintf_s(messageLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, messageFormat, variableArgumentList);
	va_end(variableArgumentList);
	messageLiteral[MESSAGE_MAX_LENGTH - 1] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	LogMessage msgToAdd;
	msgToAdd.msg				= StringUtils::Stringf("%s\n", messageLiteral);
	msgToAdd.tag				= tag;
	msgToAdd.logLvl				= logLevel;
	msgToAdd.printCallstack		= true;
	s_theLogger->m_messageBuffer.Enqueue(msgToAdd);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ENABLE DISABLE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void TheLogger::LogEnable(const char* tag) {

	if (!s_theLogger) {
		s_theLogger				= new TheLogger();
		s_theLogger->m_thread	= Thread(&Run);
	}

	if (s_theLogger->m_logListMode == LOG_WHITELIST) {
		s_theLogger->m_listedTags.push_back(tag);
	}
	else {
		for (uint i = 0; i < s_theLogger->m_listedTags.size(); i++) {
			if (s_theLogger->m_listedTags[i] == tag) {
				s_theLogger->m_listedTags.erase(s_theLogger->m_listedTags.begin() + i);
				break;
			}
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void TheLogger::LogDisable(const char* tag) {

	if (!s_theLogger) {
		s_theLogger				= new TheLogger();
		s_theLogger->m_thread	= Thread(&Run);
	}

	if (s_theLogger->m_logListMode == LOG_BLACKLIST) {
		s_theLogger->m_listedTags.push_back(tag);
	}
	else {
		for (uint i = 0; i < s_theLogger->m_listedTags.size(); i++) {
			if (s_theLogger->m_listedTags[i] == tag) {
				s_theLogger->m_listedTags.erase(s_theLogger->m_listedTags.begin() + i);
				break;
			}
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void TheLogger::LogDisableAll() {

	if (!s_theLogger) {
		s_theLogger				= new TheLogger();
		s_theLogger->m_thread	= Thread(&Run);
	}

	s_theLogger->m_listedTags.clear();
	s_theLogger->m_logListMode = LOG_WHITELIST;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INTERNAL STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
TheLogger::TheLogger() {
	m_writeCallbacks.push_back(&WriteToFile);

	#if PRINT_TO_OUTPUT_PANE == true
	m_writeCallbacks.push_back(&WriteToOutputWindow);
	#endif

	#if PRINT_TO_DEV_CONSOLE == true
	m_writeCallbacks.push_back(&WriteToDevConsole);
	#endif

	int level = LOG_LEVEL;

	switch (level) {
	case 0:
		m_logLevel = LOG_NONE;
		break;
	case 1:
		m_logLevel = LOG_SEVERE;
		break;
	case 2:
		m_logLevel = LOG_RECOVERABLE;
		break;
	case 3:
		m_logLevel = LOG_DEFAULT;
		break;
	case 4:
		m_logLevel = LOG_ALL;
		break;
	}

	m_logListMode = LOG_BLACKLIST;
}


//---------------------------------------------------------------------------------------------------------------------------
TheLogger::~TheLogger() {
	m_thread.Join();
	m_file.close();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MESSAGE HANDLING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void TheLogger::HandleMessage(LogMessage msgToProcess) {

	////////////////////////////////////////////////////
	//------------------------------------------
	//CHECK LOG LEVEL
	if (msgToProcess.logLvl > s_theLogger->m_logLevel) {
		return;
	}

	////////////////////////////////////////////////////
	//------------------------------------------
	//CHECK LISTING MODE
	bool shouldProcess = false;
	if (s_theLogger->m_logListMode == LOG_WHITELIST) {
		for (uint i = 0; i < s_theLogger->m_listedTags.size(); i++) {
			if (s_theLogger->m_listedTags[i] == msgToProcess.tag) {
				shouldProcess = true;
				break;
			}
		}
	}
	else {
		bool foundBlacklistedTag = false;

		for (uint i = 0; i < s_theLogger->m_listedTags.size(); i++) {
			if (s_theLogger->m_listedTags[i] == msgToProcess.tag) {
				foundBlacklistedTag = true;
				break;
			}
		}

		shouldProcess = !foundBlacklistedTag;
	}

	if (!shouldProcess)
		return;

	////////////////////////////////////////////////////
	//------------------------------------------
	//PROCESS MESSAGE
	LocalTime time = Time::GetCurrentLocalTime();

	String timestamp = StringUtils::Stringf("%i/%i/%i-%ih/%im/%is", (time.month), (time.day), (time.year), (time.hour), (time.minute), (time.second));
	String output = StringUtils::Stringf("%s (%s): %s", timestamp.c_str(), msgToProcess.tag.c_str(), msgToProcess.msg.c_str());

	for (uint i = 0; i < s_theLogger->m_writeCallbacks.size(); i++) {
		if (output.size() == 0)
			break;
		s_theLogger->m_writeCallbacks[i](output);
	}

	//CALlSTACK
	if (msgToProcess.printCallstack) {
		//CALLSTACK
		Callstack* cs = CallstackFetch(0);
		CallstackLine* lines = CallstackGetLines(cs);

		for (uint i = 0; i < cs->frame_count; i++) {
			LogMessage msg;
			String callstackLineOutput = StringUtils::Stringf("\t%s(%u): %p %s\n", lines->filename, lines->line, lines->stackPtr, lines->function_name);
			for (uint j = 0; j < s_theLogger->m_writeCallbacks.size(); j++) {

				if (callstackLineOutput.size() == 0)
					break;
				s_theLogger->m_writeCallbacks[j](callstackLineOutput);
			}
			lines++;
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void TheLogger::FlushRemainingMessages() {

	LogMessage msg;
	while (s_theLogger->m_messageBuffer.Dequeue(msg)) {

		s_theLogger->HandleMessage(msg);
	}

	s_theLogger->m_file.close();
	MakeCopyOfLog();

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UTILS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void TheLogger::MakeCopyOfLog() {
	std::vector<uchar> buffer;
	FileUtils::LoadBinaryFileToBuffer(s_theLogger->m_fileName, buffer);
	String fName = StringUtils::Stringf("%s%s.txt", LOG_FILE_PATH, LOG_FILE_NAME);
	FileUtils::SaveBufferToBinaryFile(fName, buffer);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC String TheLogger::GetLogFileNameToCreate() {

	LocalTime time = Time::GetCurrentLocalTime();
	String fTime = Time::FormatTimeForFilenames(time);
	return StringUtils::Stringf("%s%s_%s.txt", LOG_FILE_PATH, LOG_FILE_NAME, fTime.c_str());
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void TheLogger::OpenDebugFile() {

	std::vector<std::string> logFiles = FileUtils::GetAllFilenamesInDirectory(LOG_FILE_PATH);

	if (logFiles.size() >= (MAX_LOG_FILE_HISTORY - 1)) {

		uint numFilesToDelete = ((uint)logFiles.size() - MAX_LOG_FILE_HISTORY);

		int idx = 0;
		for (std::vector<std::string>::reverse_iterator it = logFiles.rbegin(); it != logFiles.rend(); it++) {

			if (idx == (int)numFilesToDelete)
				break;

			FileUtils::DeleteFileByFileName((*it).c_str());

			idx++;
		}
	}

	s_theLogger->m_fileName = GetLogFileNameToCreate();
	s_theLogger->m_file.open(s_theLogger->m_fileName.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RUNNING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
STATIC void TheLogger::Run() {

	OpenDebugFile();

	while (!InputSystem::HasQuit()) {

		if (s_theLogger->m_messageBuffer.size() > 0) {
			LogMessage msg;
			if (s_theLogger->m_messageBuffer.Dequeue(msg)) {
				s_theLogger->HandleMessage(msg);
			}
		}
	}

	s_theLogger->FlushRemainingMessages();
	s_theLogger->m_file.close();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//WRITE FUNCS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void TheLogger::WriteToOutputWindow(const String& output) {
	#if defined( PLATFORM_WINDOWS )
	if (IsDebuggerAvailable()) {
		OutputDebugStringA(output.c_str());
	}
	#endif

	std::cout << output;
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void TheLogger::WriteToDevConsole(const String& output) {
	Console::PrintDebugOutput(output, CONSOLE_DEBUG);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void TheLogger::WriteToFile(const String& output) {
	s_theLogger->m_file << output;
}