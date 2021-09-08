#pragma once

#include <vector>
#include <windows.h>
#include <TlHelp32.h>

namespace mem {
	DWORD GetValueOfAdress(uintptr_t addr, HANDLE hProcess, bool isFloat = false);
	void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
	void NopEx(BYTE* dst, unsigned int size, HANDLE hProcess);
}