#include "gui.h"
#include "hotkey.h"
#include "options.h"
#include <chrono>

bool demoOpen{ true };
static int activeTab{};

bool test{ false };

void drawMenu() {
	ImGui::SetNextWindowSize(ImVec2(500, 500));
	ImGui::Begin("Menu", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	{
		static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
		
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AC MENU");
		ImGui::SameLine();
		ImGui::TextColored(ImColor(255, 0 , 0 , 255), "by IWAN");
		if (ImGui::Button("Misc"))
		{
			activeTab = 0;
		}
		ImGui::SameLine(0, -1.0f);
		if (ImGui::Button("CM SOON"))
		{
			activeTab = 1;
		}
		ImGui::SameLine(0, -1.0f);
		if (ImGui::Button("CM SOON"))
		{
			activeTab = 2;
		}

		switch (activeTab) {
		case 0:
			misc();
			break;
		}
	}
	ImGui::End();
}



void misc() {
	

	ImGui::Checkbox("Unlimited Ammo", &menu::unlimitedammo);

	ImGui::Checkbox("No Recoil", &menu::nr);

	ImGui::Checkbox("Rapidfire", &menu::rapidfire);

	ImGui::TextColored(ImColor(109, 7, 186, 200), "Weapon: %s", menu::curWeapon);
	
}



void hotkey(int* k, const ImVec2& size_arg)
{
	static bool waitingforkey = false;
	if (waitingforkey == false) {
		if (ImGui::Button(KeyNames[*(int*)k], size_arg))
			waitingforkey = true;
	}
	else if (waitingforkey == true) {
		ImGui::Button("....", size_arg);
		for (auto& Key : KeyCodes)
		{
			if (GetAsyncKeyState(Key)) {
				if (Key == VK_LBUTTON) {
					break;
				}
				else {
					*(int*)k = Key;
					waitingforkey = false;
				}
			}
		}
	}
}