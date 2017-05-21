#pragma once

#include <wtypes.h>

inline size_t HashCString(const char* str) { return std::hash<std::string>()((LPCSTR)str); }