#include "Engine/Utils/FileUtils/FileUtils.hpp"
#include "Engine/EventSystem/GlobalEventSystem.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOAD SAVE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
bool FileUtils::LoadBinaryFileToBuffer(const String& filePath, std::vector<unsigned char>& out_buffer) {
	FILE* file;
	long size;

	int wasSuccesful = fopen_s(&file, filePath.c_str(), "rb");

	if (wasSuccesful == 0) {
		fseek(file, 0, SEEK_END);
		size = ftell(file);

		if (size == 0)
			return false;

		rewind(file);
		out_buffer.resize(size);
		fread(&out_buffer[0], sizeof(unsigned char), size, file);
		fclose(file);

		return true;
	}
	else
		return false;
}


//---------------------------------------------------------------------------------------------------------------------------
bool FileUtils::SaveBufferToBinaryFile(const String& filePath, const std::vector<unsigned char>& buffer) {

	if (buffer.size() == 0)
		return false;

	FILE* file;

	int wasSuccessful = fopen_s(&file, filePath.c_str(), "wb");

	if (wasSuccessful == 0) {
		fwrite(&buffer[0], 1, buffer.size(), file);
		fclose(file);

		return true;
	}
	else
		return false;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//EXISTS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
bool FileUtils::DoesFileExist(const String& filename) {
	FILE* file;
	if (fopen_s(&file, filename.c_str(), "r") == 0) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
bool FileUtils::DeleteFileByFileName(const char* filename) {

	if (remove(filename) == 0) {
		return true;
	}
	else {
		return false;
	}

}


//---------------------------------------------------------------------------------------------------------------------------
bool FileUtils::DoesDirectoryExist(LPCTSTR szPath) {

	DWORD dwAttrib = GetFileAttributes(szPath);
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}


//---------------------------------------------------------------------------------------------------------------------------
bool FileUtils::CreateDirectoryIfDoesntExist(const String& dirName) {
	std::wstring stemp = std::wstring(dirName.begin(), dirName.end());
	LPCWSTR sw = stemp.c_str();

	return (CreateDirectory(sw, NULL) || ERROR_ALREADY_EXISTS == GetLastError());
}


//---------------------------------------------------------------------------------------------------------------------------
bool FileUtils::CreateFileIfDoesntExist(const String& dir, const String& file) {

	if (DoesFileExist(dir + file)) {
		return true;
	}
	else {

		String filename = dir + file;
		std::wstring stemp = std::wstring(filename.begin(), filename.end());
		LPCWSTR sw = stemp.c_str();

		CreateFile(sw, GENERIC_READ, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ENUMERATE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
std::vector<String> FileUtils::GetAllFilenamesInDirectory(const String& relDir) {
	String	searchPathPattern = relDir + "*";
	std::vector<String> foundFiles;

	int error = 0;
	struct _finddata_t fileInfo;

	intptr_t searchHandle = _findfirst(searchPathPattern.c_str(), &fileInfo);

	while (searchHandle != -1 && !error) {
		String relativePathToFile = StringUtils::Stringf("%s/%s", relDir.c_str(), fileInfo.name);
		bool isDirectory = fileInfo.attrib & _A_SUBDIR ? true : false;
		bool isHidden = fileInfo.attrib & _A_HIDDEN ? true : false;

		if (!isDirectory && !isHidden) {
			foundFiles.push_back(relativePathToFile);
		}

		error = _findnext(searchHandle, &fileInfo);
	}
	_findclose(searchHandle);

	return foundFiles;
}


//---------------------------------------------------------------------------------------------------------------------------
std::vector<String> FileUtils::EnumerateFiles(const String& baseFolder, const String& filePattern, bool recurseSubfolders) {

	String	searchPathPattern = baseFolder + "*";
	std::vector<String> foundFiles;

	int error = 0;
	WIN32_FIND_DATAA fdFile;

	HANDLE searchHandle = FindFirstFileA(searchPathPattern.c_str(), &fdFile);

	while (searchHandle != INVALID_HANDLE_VALUE) {

		if (strcmp(fdFile.cFileName, ".") == 0 || strcmp(fdFile.cFileName, "..") == 0) {
			error = FindNextFileA(searchHandle, &fdFile);
			continue;
		}

		String relativePathToFile = StringUtils::Stringf("%s%s", baseFolder.c_str(), fdFile.cFileName);
		bool isDirectory = fdFile.dwFileAttributes & _A_SUBDIR ? true : false;
		bool isHidden = fdFile.dwFileAttributes & _A_HIDDEN ? true : false;

		if (!isDirectory && !isHidden) {
			if (DoesFileMatchPattern(fdFile.cFileName, filePattern)) {
				foundFiles.push_back(relativePathToFile);
			}
		}

		else if (isDirectory && recurseSubfolders) {
			std::vector<String> files = EnumerateFiles(baseFolder + fdFile.cFileName + "/", filePattern, true);
			foundFiles.insert(foundFiles.end(), files.begin(), files.end());
		}

		error = FindNextFileA(searchHandle, &fdFile);
		if (error == 0) {
			break;
		}
	}
	FindClose(searchHandle);

	return foundFiles;
}


//---------------------------------------------------------------------------------------------------------------------------
void FileUtils::FireEventForEachFileFound(const String& eventToFire, const String& baseFolder, const String& filePattern, bool recurseSubfolders) {

	String	searchPathPattern = baseFolder + "*";
	std::vector<String> foundFiles;

	int error = 0;
	WIN32_FIND_DATAA fdFile;

	HANDLE searchHandle = FindFirstFileA(searchPathPattern.c_str(), &fdFile);

	while (searchHandle != INVALID_HANDLE_VALUE) {

		if (strcmp(fdFile.cFileName, ".") == 0 || strcmp(fdFile.cFileName, "..") == 0) {
			error = FindNextFileA(searchHandle, &fdFile);
			continue;
		}

		String relativePathToFile = StringUtils::Stringf("%s%s", baseFolder.c_str(), fdFile.cFileName);
		bool isDirectory = fdFile.dwFileAttributes & _A_SUBDIR ? true : false;
		bool isHidden = fdFile.dwFileAttributes & _A_HIDDEN ? true : false;

		if (!isDirectory && !isHidden) {
			if (DoesFileMatchPattern(fdFile.cFileName, filePattern)) {
				NamedProperties fileProperties;
				fileProperties.SetProperty("FileName", String(fdFile.cFileName));
				fileProperties.SetProperty("FileExtension", GetFileExtension(fdFile.cFileName));
				fileProperties.SetProperty("FileNameWithoutExtension", GetFilenameWithoutExtension(fdFile.cFileName));
				fileProperties.SetProperty("FileRelativePath", relativePathToFile);
				fileProperties.SetProperty("FileAbsolutePath", GetAbsolutePathOfFile(fdFile.cFileName));

				EventSystem::FireEvent(eventToFire, fileProperties);
			}
		}
		else if (isDirectory && recurseSubfolders) {
			FireEventForEachFileFound(eventToFire, baseFolder + fdFile.cFileName + "/", filePattern, true);
		}

		error = FindNextFileA(searchHandle, &fdFile);
		if (error == 0) {
			break;
		}
	}
	FindClose(searchHandle);


/*
	“FileName”					“ExplosionSprites.png”
	“FileExtension”				“.png”
	“FileNameWithoutExtension”	“ExplosionSprites”
	“FileRelativePath”			“Images / ExplosionSprites.png”
	“FileAbsolutePath”			“C : / Guildhall / Projects / Tanks / Data / Images / ExplosionSprites.png”*/

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PATTERN MATCHING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
String FileUtils::GetFilenameWithoutExtension(const String& filename) {
	
	String filenameWithoutExt = "";

	for (String::const_iterator filenameIt = filename.begin(); filenameIt != filename.end(); ++filenameIt) {

		if (*filenameIt == '.') {
			break;
		}
		
		filenameWithoutExt += *filenameIt;
	}

	return filenameWithoutExt;
}



//---------------------------------------------------------------------------------------------------------------------------
String FileUtils::GetFileExtension(const String& filename) {

	String::const_reverse_iterator filenameIt = filename.rbegin();
	String ext = "";

	for (filenameIt; filenameIt != filename.rend(); ++filenameIt) {

		ext += *filenameIt;

		if (*filenameIt == '.') {
			std::reverse(ext.begin(), ext.end());
			return ext;
		}
	}

	return "NO_EXTENSION";
}


//---------------------------------------------------------------------------------------------------------------------------
bool FileUtils::DoesFileMatchPattern(const String& filename, const String& filePattern) {

	String::const_reverse_iterator patternIt = filePattern.rbegin();
	String::const_reverse_iterator filenameIt = filename.rbegin();

	for (patternIt; patternIt != filePattern.rend(); ++patternIt) {

		if (filenameIt == filename.rend()) {
			return false;
		}

		if (*patternIt != '*' && *patternIt != *filenameIt) {
			return false;
		}
		else if (*patternIt == '*') {
			return true;
		}

		++filenameIt;
	}

	if (filenameIt == filename.rend() && patternIt == filePattern.rend()) {
		return true;
	}
	else {
		return false;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
String FileUtils::GetAbsolutePathOfFile(const String& filename) {

	char szBuffer[512] = "";
	char* pszFileName = NULL;

	GetFullPathNameA(filename.c_str(), sizeof(szBuffer), szBuffer, &pszFileName);

	//Parse out the \\'s in the filepath

	String absPath = szBuffer;
	String path = "";

	for (uint i = 0; i < absPath.length(); i++) {

		if (absPath[i] == '\\') {
			path += '/';
		}
		else {
			path += absPath[i];
		}
		
	}

	return path;
}