#pragma once

#include <Windows.h>
#include <vector>




uintptr_t localPlayer = 0x17E0A8;

std::vector<DWORD> NoReload{ 0x368, 0x14, 0x0 };
std::vector<DWORD> NoRecoil1{ 0x364, 0xC, 0x5E };
std::vector<DWORD> NoRecoil2{ 0x364, 0xC, 0x60 };
std::vector<DWORD> Rapidfire{ 0x364, 0xC, 0x48 };
std::vector<DWORD> Weapon{ 0x364, 0x4 };
std::vector<DWORD> CurAmmo{ 0x368, 0x14, 0x0 };


namespace old {
	int nr_rifle1 = 50;
	int nr_rifle2 = 25;
	int nr_pistol1 = 22;
	int nr_pistol2 = 22;
	int rf_rifle = 120;
	int rf_pistol = 160;
}
namespace val {
	int nr_set = 0;
	int ammo_set = 20;
	int rf_set = 60;
}


bool n = false;

const char* curWeapon = "Test";

int nr_old1 = 0;
int nr_old2 = 0;
int rf_old = 0;

int noreload_new = 100;

int norecoil_new = 0;

