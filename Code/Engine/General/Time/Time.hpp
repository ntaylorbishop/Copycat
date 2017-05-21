/*
* Time.hpp
* 	A simple high-precision time utility function for Windows
* 	based on code by Squirrel Eiserloh
*/
#pragma once

#include "time.h"
#include <string>

struct LocalTime {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
};

namespace Time {

	uint64_t CyclesToMilliseconds(uint64_t cycles);
	uint64_t ConvertPerformanceCountToTimeSeconds(uint64_t startTime, uint64_t endTime);
	uint64_t GetCurrentPerformanceCount();
	double GetCurrentTimeSeconds();

	double GetPerformanceFrequency();

	LocalTime GetCurrentLocalTime();
	std::string FormatTimeForFilenames(LocalTime time);

};