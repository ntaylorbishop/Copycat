#pragma once

#include <windows.h>

#define _CRT_SECURE_NO_DEPRECATE
#include <vector>
#include <string>
#include "io.h"
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <algorithm>

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Utils/StringUtils.hpp"

namespace FileUtils {
	//LOAD SAVE
	bool LoadBinaryFileToBuffer(const String& filePath, std::vector<unsigned char>& out_buffer);
	bool SaveBufferToBinaryFile(const String& filePath, const std::vector<unsigned char>& buffer);


	//EXISTS
	bool DoesFileExist(const String& filename);
	bool DoesDirectoryExist(LPCTSTR szPath);
	bool CreateDirectoryIfDoesntExist(const String& dirName);
	bool CreateFileIfDoesntExist(const String& dir, const String& file);

	bool DeleteFileByFileName(const char* filename);

	//ENUMERATE
	std::vector<String> GetAllFilenamesInDirectory(const String& relDir);
	std::vector<String> EnumerateFiles(const String& basefolder, const String& filePattern, bool recurseSubfolders);
	void FireEventForEachFileFound(const String& eventToFire, const String& baseFolder, const String& filePattern, bool recurseSubfolders);

	//PATTERN MATCHING
	String GetFilenameWithoutExtension(const String& filename);
	String GetFileExtension(const String& filename);
	bool DoesFileMatchPattern(const String& filename, const String& filePattern);
	String GetAbsolutePathOfFile(const String& filename);

/*
	String GetPathOfExecutable();
	/*
	String FileUtils::GetPathOfExecutable() {
	WCHAR buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	PathRemoveFileSpec(buffer);
	std::wstring ws(buffer);
	String path(ws.begin(), ws.end());
	return path;
	}*/
}