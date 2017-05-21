/*
 * Time.cpp
 * 	A simple high-precision time utility function for Windows
 * 	based on code by Squirrel Eiserloh
*/

#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/General/Time/Time.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Utils/ErrorWarningAssert.hpp"
#include "Engine/Utils/StringUtils.hpp"

double InitializeTime(LARGE_INTEGER& out_initialTime) {
	LARGE_INTEGER countsPerSecond;
	QueryPerformanceFrequency(&countsPerSecond);
	QueryPerformanceCounter(&out_initialTime);
	return(1.0 / static_cast<double>(countsPerSecond.QuadPart));
}

uint64_t Time::CyclesToMilliseconds(uint64_t cycles) {
	static LARGE_INTEGER s_frequency;
	BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);

	ASSERT_OR_DIE(s_use_qpc, "QPF Failed!");

	return (1000ULL * cycles) / s_frequency.QuadPart;
}
uint64_t Time::ConvertPerformanceCountToTimeSeconds(uint64_t startTime, uint64_t endTime) {
	static LARGE_INTEGER s_frequency;
	BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);

	ASSERT_OR_DIE(s_use_qpc, "QPF Failed!");

	uint64_t startMilliseconds = (1000ULL * startTime) / s_frequency.QuadPart;
	uint64_t endMilliseconds = (1000ULL * endTime) / s_frequency.QuadPart;
	return (endMilliseconds - startMilliseconds);
}
uint64_t Time::GetCurrentPerformanceCount() {
	LARGE_INTEGER countsPerSecond;
	QueryPerformanceCounter(&countsPerSecond);
	return (uint64_t)countsPerSecond.QuadPart;
}
double Time::GetCurrentTimeSeconds() {
	static LARGE_INTEGER initialTime;
	static double secondsPerCount = InitializeTime(initialTime);
	LARGE_INTEGER currentCount;
	QueryPerformanceCounter(&currentCount);
	LONGLONG elapsedCountsSinceInitialTime = currentCount.QuadPart - initialTime.QuadPart;

	double currentSeconds = static_cast<double>(elapsedCountsSinceInitialTime) * secondsPerCount;
	return currentSeconds;
}

double Time::GetPerformanceFrequency() {
	LARGE_INTEGER countsPerSecond;
	QueryPerformanceFrequency(&countsPerSecond);
	return(1.0 / static_cast<double>(countsPerSecond.QuadPart));
}

LocalTime Time::GetCurrentLocalTime() {
	LocalTime lTime;

	time_t t = time(0);   // get time now
	tm now;
	localtime_s(&now, &t);

	lTime.year = now.tm_year + 1900;
	lTime.month = now.tm_mon;
	lTime.day = now.tm_mday;
	lTime.hour = now.tm_hour;
	lTime.minute = now.tm_min;
	lTime.second = now.tm_sec;

	return lTime;
}

String Time::FormatTimeForFilenames(LocalTime time) {
	String year = std::to_string(time.year);
	String month = std::to_string(time.month);
	String day = std::to_string(time.day);
	String hour = std::to_string(time.hour);
	String minute = std::to_string(time.minute);
	String second = std::to_string(time.second);

	String fMonth;
	String fDay;
	String fHour;
	String fMinute;
	String fSecond;

	//MONTH
	if (month.length() == 1)
		fMonth = "0" + month;
	else
		fMonth = month;

	//DAY
	if (day.length() == 1)
		fDay = "0" + day;
	else
		fDay = day;

	//HOUR
	if (hour.length() == 1)
		fHour = "0" + hour;
	else
		fHour = hour;

	//MINUTE
	if (minute.length() == 1)
		fMinute = "0" + minute;
	else
		fMinute = minute;

	//SECOND
	if (second.length() == 1)
		fSecond = "0" + second;
	else
		fSecond = second;

	return StringUtils::Stringf("%s%s%s_%s%s%s", year.c_str(), fMonth.c_str(), fDay.c_str(), fHour.c_str(), fMinute.c_str(), fSecond.c_str());
}