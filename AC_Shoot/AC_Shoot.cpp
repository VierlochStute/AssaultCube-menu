#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <string>
#include <thread>

#include "functions.h"
#include "gui.h"
#include "graphics.h"
#include "offsets.h"
#include "options.h"

#include <dwmapi.h>
#include <d3d11.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

using namespace std;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int wWidth{};
int wHeight{};
RECT windowPos{};

void setWindowSize() {
	OutputDebugStringW(L"My output string.");
	HWND gameWindow = FindWindow(nullptr, "AssaultCube");
	if (gameWindow != nullptr) {
		RECT targetWindow{};
		GetWindowRect(gameWindow, &targetWindow);
		wWidth = targetWindow.right - targetWindow.left;
		wHeight = targetWindow.bottom - targetWindow.top;

		GetWindowRect(gameWindow, &windowPos);
	}
}

void updateWindow(const HWND& window) {
	HWND gameWindow = FindWindow(nullptr, "AssaultCube");
	RECT targetWindow{};
	GetWindowRect(gameWindow, &targetWindow);
	wWidth = targetWindow.right - targetWindow.left; // FIX 
	wHeight = targetWindow.bottom - targetWindow.top;
	MoveWindow(window, targetWindow.left, targetWindow.top, wWidth, wHeight, true);
}

void openConsole() {
	AllocConsole();

	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
}



void features(DWORD pID, HANDLE pHandle, char gameName[], uintptr_t gameBaseAdress)
	{
	uintptr_t adr_norecoil1 = RPM(pHandle, localPlayer, gameBaseAdress, NoRecoil1);
	uintptr_t adr_norecoil2 = RPM(pHandle, localPlayer, gameBaseAdress, NoRecoil2);
	uintptr_t adr_rapidfire = RPM(pHandle, localPlayer, gameBaseAdress, Rapidfire);
	uintptr_t adr_weapon = RPM(pHandle, localPlayer, gameBaseAdress, Weapon);
	uintptr_t adr_curammo = RPM(pHandle, localPlayer, gameBaseAdress, CurAmmo);
	
	int var3;
	int var4;

	ReadProcessMemory(pHandle, (LPVOID)(adr_weapon), &var3, sizeof(var3), NULL);
	switch (var3) {
	case 6: //Rifle
		menu::curWeapon = "Assault Rifle";
		break;
	case 1: //Pistol
		menu::curWeapon = "Pistol";
		break;
	case 0:
		menu::curWeapon = "Knife";
		break;
	default:
		menu::curWeapon = "Unknown";
		break;
	}

	if (menu::nr)
	{
		WriteProcessMemory(pHandle, (LPVOID)(adr_norecoil1), &val::nr_set, sizeof(val::nr_set), 0);
		WriteProcessMemory(pHandle, (LPVOID)(adr_norecoil2), &val::nr_set, sizeof(val::nr_set), 0);
	}
	else if(!menu::nr){
		ReadProcessMemory(pHandle, (LPVOID)(adr_weapon), &var3, sizeof(var3), NULL);
		switch (var3) {
		case 6:
			WriteProcessMemory(pHandle, (LPVOID)(adr_norecoil1), &old::nr_rifle1, sizeof(old::nr_rifle1), 0);
			WriteProcessMemory(pHandle, (LPVOID)(adr_norecoil2), &old::nr_rifle2, sizeof(old::nr_rifle2), 0);
			break;
		case 1: 
			WriteProcessMemory(pHandle, (LPVOID)(adr_norecoil1), &old::nr_pistol1, sizeof(old::nr_pistol1), 0);
			WriteProcessMemory(pHandle, (LPVOID)(adr_norecoil2), &old::nr_pistol2, sizeof(old::nr_pistol2), 0);
			break;
		case 0:
			break;
		default:
			WriteProcessMemory(pHandle, (LPVOID)(adr_norecoil1), &old::nr_rifle1, sizeof(&old::nr_rifle1), 0);
			WriteProcessMemory(pHandle, (LPVOID)(adr_norecoil2), &old::nr_rifle2, sizeof(old::nr_rifle2), 0);
			break;
		}
	}
	if (menu::unlimitedammo) {
		int var;
		ReadProcessMemory(pHandle, (LPVOID)(adr_curammo), &var, sizeof(var), 0);
		if (var <= 0) {WriteProcessMemory(pHandle, (LPVOID)(adr_curammo), &val::ammo_set, sizeof(val::ammo_set), 0);}
	}
	if (menu::rapidfire) {
		WriteProcessMemory(pHandle, (LPVOID)(adr_rapidfire), &val::rf_set, sizeof(val::rf_set), 0);
	}
	else if (!menu::rapidfire) {
		ReadProcessMemory(pHandle, (LPVOID)(adr_weapon), &var3, sizeof(var3), NULL);
		switch (var3) {
		case 6:
			WriteProcessMemory(pHandle, (LPVOID)(adr_rapidfire), &old::rf_rifle, sizeof(old::rf_rifle), 0);
			break;
		case 1:
			WriteProcessMemory(pHandle, (LPVOID)(adr_rapidfire), &old::rf_pistol, sizeof(old::rf_pistol), 0);
			break;
		case 0:
			break;
		default:
			WriteProcessMemory(pHandle, (LPVOID)(adr_rapidfire), &old::rf_rifle, sizeof(&old::rf_rifle), 0);
			break;
		}
	}
}

LRESULT CALLBACK WindowProc(HWND window, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(window, uMsg, wParam, lParam)) {
		return 0L;
	}

	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0L;
	}
	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0L;
	}
	return DefWindowProc(window, uMsg, wParam, lParam);
}



int WINAPI WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmdShow) {
	openConsole();
	DWORD pID = GetPID(GetGameWindow("AssaultCube"));
	HANDLE pHandle = GetProcessHandle(pID);
	char gameName[] = "ac_client.exe";
	uintptr_t gameBaseAdress = GetModuleBaseAddress(_T(gameName), pID);

	

	if (!pID) {
		cout << "Error: ProcessID not found! Please start the game! \n";
		Sleep(3000);
		return 0;
	}
	else if (!gameBaseAdress) {
		cout << "Error: GameAdress not found! Please start the game! \n";
		Sleep(3000);
		return 0;
	}
	else {
		FreeConsole();
	}


	WNDCLASSEXW wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = L"External Overlay";

	RegisterClassExW(&wc);
	setWindowSize();

	const HWND window = CreateWindowExW(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
		wc.lpszClassName,
		L"Overlay",
		WS_POPUP,
		0,
		0,
		wWidth,
		wHeight,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);

	SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);
	{
		RECT clientArea{};
		GetClientRect(window, &clientArea);

		RECT windowArea{};
		GetWindowRect(window, &windowArea);

		POINT diff{};
		ClientToScreen(window, &diff);

		const MARGINS margins{
			windowArea.left + (diff.x - windowArea.left),
			windowArea.top + (diff.y - windowArea.top),
			clientArea.right,
			clientArea.bottom
		};

		DwmExtendFrameIntoClientArea(window, &margins);
	}

	Graphics render(window);
	ShowWindow(window, cmdShow);
	UpdateWindow(window);
	render.initImGui(window);

	bool running = true;
	while (running) {
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				running = false;
			}

		}

		if (!running) {
			break;
		}

		updateWindow(window);
		render.createNewFrame();

		if (menu::menu) {
			drawMenu();
		}
		if (GetAsyncKeyState(VK_INSERT) & 1) {
			menu::menu = !menu::menu;

			long style = GetWindowLongPtr(window, GWL_EXSTYLE);
			style = menu::menu ? (style & ~WS_EX_LAYERED) : (style | WS_EX_LAYERED);
			SetWindowLongPtr(window, GWL_EXSTYLE, style);

			SetForegroundWindow(menu::menu ? window : FindWindow(nullptr, "AssaultCube"));
		}
		features(pID, pHandle, gameName, gameBaseAdress);
		render.RenderTarget();
		render.swapChain->Present(0U, 0U);
	}

	DestroyWindow(window);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);

	return 0;
}



