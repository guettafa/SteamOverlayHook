#include "pattern.h"

MODULEINFO Pattern::GetModuleInfo(const wchar_t* moduleName)
{
	MODULEINFO moduleInfo;

	HMODULE hModule = GetModuleHandle(moduleName);
	HANDLE  hProcess = GetCurrentProcess();

	GetModuleInformation(hProcess, hModule, &moduleInfo, sizeof(MODULEINFO));

	return moduleInfo;
}

uintptr_t Pattern::FindPattern(const wchar_t* moduleName, const char* pattern, const char* mask)
{
	MODULEINFO moduleInfo = GetModuleInfo(moduleName);
	DWORD      imageSize = moduleInfo.SizeOfImage;

	uintptr_t moduleBaseAddrs = (uintptr_t)moduleInfo.lpBaseOfDll;

	int patternLength = strlen(mask);

	for (size_t i = 0; i < imageSize; i++)
	{
		bool isPatternFound = true;

		// Go through each byte in instruction
		for (size_t j = 0; j < patternLength; j++)
		{
			if (pattern[j] != *(char*)(moduleBaseAddrs + i + j) && mask[j] != '?')
			{
				isPatternFound = false;
			}
		}
		if (isPatternFound) // return adress of the instruction
			return moduleBaseAddrs + i;
	}
	return std::string::npos;
}