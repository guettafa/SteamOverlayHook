#pragma once

#include <Windows.h>
#include <psapi.h>
#include <string>

namespace Pattern
{
	constexpr const char* createHookSig = "\x55\x8B\xEC\x51\x8B\x45\x10\xC7";
	constexpr const char* hookPresentSig = "\x55\x8B\xEC\x64\xA1\x00\x00\x00\x00\x6A\xFF\x68\xCC\xCC\xCC\xCC\x50\x64\x89\x25\x00\x00\x00\x00\x53\x8B\x5D\x10\x56\x57\xF6\xC3\x01\x74\x21";

	MODULEINFO GetModuleInfo(const wchar_t* moduleName);
	uintptr_t  FindPattern(const wchar_t* moduleName, const char* pattern, const char* mask);
}