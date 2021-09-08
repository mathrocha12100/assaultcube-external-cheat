#include "mem.h"
#include <iostream>
#include <stdint.h>

DWORD mem::GetValueOfAdress(uintptr_t addr, HANDLE hProcess, bool isFloat) {
	DWORD value;
	ReadProcessMemory(hProcess, (BYTE*)addr, &value, sizeof(addr), 0);

	if (isFloat) {
		return *reinterpret_cast<float const*>(&value);
	}

	return value;
}

void mem::PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess) {
	DWORD oldprotected;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotected);

	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	VirtualProtectEx(hProcess, dst, size, oldprotected, &oldprotected);

}

void mem::NopEx(BYTE* dst, unsigned int size, HANDLE hProcess) {
	BYTE* nopArray = new BYTE[size];
	memset(nopArray, 0x90, size);

	PatchEx(dst, nopArray, size, hProcess);
	delete[] nopArray;
}