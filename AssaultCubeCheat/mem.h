#pragma once

#include <vector>
#include <windows.h>
#include <TlHelp32.h>
#include <cstdint>

namespace mem {
	void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
	void NopEx(BYTE* dst, unsigned int size, HANDLE hProcess);
}