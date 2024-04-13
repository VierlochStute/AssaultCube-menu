#pragma once
#include <Windows.h>
#include<TlHelp32.h>
#include <iostream>
#include <tchar.h> 
#include <vector>





uintptr_t GetModuleBaseAddress(TCHAR* lpszModuleName, uintptr_t pID) {
	uintptr_t dwModuleBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
	MODULEENTRY32 ModuleEntry32 = { 0 };
	ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapshot, &ModuleEntry32))
	{
		do {
			if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0)
			{
				dwModuleBaseAddress = (uintptr_t)ModuleEntry32.modBaseAddr;
				break;
			}
		} while (Module32Next(hSnapshot, &ModuleEntry32));


	}
	CloseHandle(hSnapshot);
	return dwModuleBaseAddress;
}

HWND GetGameWindow(LPCSTR gameName) {
	HWND hGameWindow = FindWindow(NULL, gameName);
	if (hGameWindow == NULL) {
		return 0;
	}
	else {
		return hGameWindow;
	}
}

DWORD GetPID(HWND gameWindow) {
	DWORD pID = NULL;
	GetWindowThreadProcessId(gameWindow, &pID);
	if (pID == NULL) {
		return 0;
	}
	else {
		return pID;
	}
}

HANDLE GetProcessHandle(DWORD pID) {
	HANDLE processHandle = NULL;
	processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
	if (processHandle == INVALID_HANDLE_VALUE || processHandle == NULL) {
		return 0;
	}
	else {
		return processHandle;
	}
}


uintptr_t RPM(HANDLE pHandle, uintptr_t gameAddress, uintptr_t gameBaseAddress, std::vector<DWORD> pointsOffsets) {

	uintptr_t baseAddress = NULL;

	ReadProcessMemory(pHandle, (LPVOID)(gameBaseAddress + gameAddress), &baseAddress, sizeof(baseAddress), NULL);
	uintptr_t pointsAddress = baseAddress;
	for (int i = 0; i < pointsOffsets.size() - 1; i++)
	{
		ReadProcessMemory(pHandle, (LPVOID)(pointsAddress + pointsOffsets.at(i)), &pointsAddress, sizeof(pointsAddress), NULL);
	}
	pointsAddress += pointsOffsets.at(pointsOffsets.size() - 1);

	return pointsAddress;

}











