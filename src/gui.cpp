#include "gui.h"
#include <thread>
#include <fstream>
#include "../driver/BFDrvUtils.h"

BFDrv driver;
int selectPid = 0;
HWND selectHwnd = 0;

void GUI::Draw(HWND hwnd)
{
	if (!isLoggedIn)
	{
		if (drawLogin)
		{
			DrawLoginInterface(hwnd);
		}
	}
	else
	{
		DrawInterface(hwnd);
	}
}

void GUI::DrawLoginInterface(HWND hwnd)
{
	if (!GUI::isLoggedIn)
	{
		ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
		ImGui::Begin((magiXOR("Login Interface").decrypt()), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
		{
			RECT rc;
			GetWindowRect(g_hwnd, &rc);

			if (ImGui::GetWindowPos().x != 0 || ImGui::GetWindowPos().y != 0)
			{
				MoveWindow(g_hwnd, rc.left + ImGui::GetWindowPos().x, rc.top + ImGui::GetWindowPos().y, gui->window_size.x, gui->window_size.y, TRUE);
				ImGui::SetWindowPos(ImVec2(0.f, 0.f));
			}

			float imageWidth = 80.0f;
			float imageHeight = 80.0f;
			float windowWidth = ImGui::GetWindowWidth();
			float posX = (windowWidth - imageWidth) * 0.5f;
			ImGui::SetCursorPosX(posX);
			ImGui::SetCursorPosY(30.f);
			ImGui::GetForegroundDrawList()->AddImageRounded((PVOID)logoImage, ImVec2(posX, 50.f), ImVec2(posX + imageWidth, 50.f + imageHeight), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_Text]), 0);
			ImGui::Dummy(ImVec2(0, 105.0f));

			{
				float inputWidth = windowWidth * 0.6f;

				float textWidth = ImGui::CalcTextSize(magiXOR("Key").decrypt()).x;
				ImGui::SetCursorPosX((windowWidth - textWidth) * 0.24f);
				ImGui::SetCursorPosX((windowWidth - inputWidth) * 0.5f);
				ImGui::PushItemWidth(inputWidth);
				ImGui::InputTextWithHint(magiXOR("##Key").decrypt(), "key", GUI::user_name, IM_ARRAYSIZE(GUI::user_name));
				ImGui::PopItemWidth();

				ImGui::Dummy(ImVec2(0, 20.0f));

				float buttonWidth = 140.0f;
				float buttonPosX = (windowWidth - buttonWidth) * 0.5f;
				ImGui::SetCursorPosX(buttonPosX);
				if (ImGui::Button(magiXOR("Log In").decrypt(), ImVec2(buttonWidth, 25))) {
					isLoggedIn = driver.B_InitDrv(GUI::user_name);
					//isLoggedIn = true;

					if (!isLoggedIn) {
						MessageBoxA(nullptr, driver.B_GetInitResult(), magiXOR("Error").decrypt(), MB_OK | MB_ICONERROR);
					}
				}

				ImGui::Dummy(ImVec2(0, 10.5f));

				static int currentColorScheme = 5;
				float colorTextWidth = ImGui::CalcTextSize(magiXOR("Change colors").decrypt()).x;
				ImGui::SetCursorPosX((windowWidth - colorTextWidth) * 0.49f);
				if (ImGui::Button(magiXOR("Change colors").decrypt())) {
					currentColorScheme = (currentColorScheme + 1) % 14;

					ApplyColorScheme(colorSchemes[currentColorScheme]);
				}
			}
		}

		ImGui::PopStyleVar();
		ImGui::End();
	}
}

void Common();
void KeyMouse();
void Injector();
void ProtectHide();
void Misc();
void Info();

// Declare the state variable outside the function
int state = 0;

void GUI::DrawInterface(HWND hwnd)
{
	ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
	ImGui::Begin((magiXOR("Interface").decrypt()), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
	{
		RECT rc;
		GetWindowRect(g_hwnd, &rc);

		if (ImGui::GetWindowPos().x != 0 || ImGui::GetWindowPos().y != 0)
		{
			MoveWindow(g_hwnd, rc.left + ImGui::GetWindowPos().x, rc.top + ImGui::GetWindowPos().y, gui->window_size.x, gui->window_size.y, TRUE);
			ImGui::SetWindowPos(ImVec2(0.f, 0.f));
		}

		float imageWidth = 60.0f;
		float imageHeight = 60.0f;
		float windowWidth = ImGui::GetWindowWidth();

		// Left-hand side child window
		ImVec2 childPos(ImGui::GetWindowPos().x + 15.0f, ImGui::GetWindowPos().y + 15.0f);
		ImVec2 childSize(imageWidth + 3.0f, imageHeight + 3.0f);

		ImGui::BeginChild(magiXOR("ChildWindow").decrypt(), childSize, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		{
			ImVec2 imagePos((childSize.x - imageWidth) * 0.5f + 18.f, (childSize.y - imageHeight) * 0.5f + 18.f);
			ImGui::SetCursorPos(imagePos);
			ImGui::GetForegroundDrawList()->AddImageRounded(logoImage, imagePos, imagePos + ImVec2(40.f, 40.f), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_Text]), 0);
		}
		ImGui::EndChild();

		// Right-hand side child window
		ImGui::SameLine();
		float rightChildWidth = ImGui::GetContentRegionAvail().x;
		float rightChildHeight = ImGui::GetContentRegionAvail().y;

		ImGui::BeginChild(magiXOR("RightChildWindow").decrypt(), ImVec2(rightChildWidth, rightChildHeight), true);
		{
			if (state == 0)
			{
				Info();
			}
			if (state == 1)
			{
				//ImGui::Text(magiXOR("Common").decrypt());
				Common();
			}
			else if (state == 2)
			{
				KeyMouse();
			}
			else if (state == 3)
			{
				Injector();
			}
			else if (state == 4) 
			{
				ProtectHide();
			}
			else if (state == 5) 
			{
				Misc();
			}
		}
		ImGui::EndChild();

		float buttonWidth = childSize.y;
		float buttonHeight = 27;

		float Interval = 35.f;
		ImGui::SetCursorPos(ImVec2(9, imageHeight += Interval));
		if (ImGui::Button("Common", ImVec2(buttonWidth, buttonHeight)))
		{
			state = 1;
		}
		ImGui::SetCursorPos(ImVec2(9, imageHeight += Interval));
		if (ImGui::Button("K / M", ImVec2(buttonWidth, buttonHeight)))
		{
			state = 2;
		}
		ImGui::SetCursorPos(ImVec2(9, imageHeight += Interval));
		if (ImGui::Button("Injector", ImVec2(buttonWidth, buttonHeight)))
		{
			state = 3;
		}
		ImGui::SetCursorPos(ImVec2(9, imageHeight += Interval));
		if (ImGui::Button("P / H", ImVec2(buttonWidth, buttonHeight)))
		{
			state = 4;
		}
		ImGui::SetCursorPos(ImVec2(9, imageHeight += Interval));
		if (ImGui::Button("Misc", ImVec2(buttonWidth, buttonHeight)))
		{
			state = 5;
		}
		ImGui::SetCursorPos(ImVec2(9, imageHeight += Interval));
		if (ImGui::Button("Info", ImVec2(buttonWidth, buttonHeight)))
		{
			state = 0;
		}
	}

	ImGui::PopStyleVar();
	ImGui::End();
}

void GUI::ApplyColorScheme(const char* colorScheme) {
	ImGuiStyle& style = ImGui::GetStyle();

	if (strcmp(colorScheme, "Standard") == 0) {
		ImGui::StyleColorsDark();
		style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.78f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.22f, 0.22f, 0.22f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.14f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.40f, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.90f, 0.90f, 0.90f, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.70f, 0.50f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.70f, 0.50f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.43f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.9f);
	}
	else if (strcmp(colorScheme, "Cinder") == 0) {
		ImGui::StyleColorsDark();
		style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
	}
	else if (strcmp(colorScheme, "Green colorway") == 0) {
		ImGui::StyleColorsClassic();
		style.Colors[ImGuiCol_Text] = ImVec4(0.76f, 0.93f, 0.78f, 0.78f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.76f, 0.93f, 0.78f, 0.28f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.78f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.18f, 0.92f, 0.29f, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.18f, 0.92f, 0.29f, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.76f, 0.93f, 0.78f, 0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.76f, 0.93f, 0.78f, 0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.18f, 0.92f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.18f, 0.92f, 0.29f, 0.43f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
	}
	else if (strcmp(colorScheme, "Blue colorway") == 0) {
		ImGui::StyleColorsDark();
		style.Colors[ImGuiCol_Text] = ImVec4(0.85f, 0.92f, 0.95f, 0.78f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.85f, 0.92f, 0.95f, 0.28f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.18f, 0.23f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.32f, 0.42f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.78f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.18f, 0.23f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.18f, 0.23f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.18f, 0.23f, 0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.18f, 0.23f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.48f, 0.69f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.31f, 0.48f, 0.69f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.63f, 0.82f, 0.14f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.35f, 0.63f, 0.82f, 0.14f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.44f, 0.60f, 0.75f, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.22f, 0.38f, 0.51f, 1.00f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.63f, 0.82f, 0.04f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.60f, 0.75f, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.85f, 0.92f, 0.95f, 0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.85f, 0.92f, 0.95f, 0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.44f, 0.60f, 0.75f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.44f, 0.60f, 0.75f, 0.43f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.18f, 0.23f, 0.9f);
	}
	else if (strcmp(colorScheme, "Purple/orange colorway") == 0) {
		ImGui::StyleColorsDark();
		style.Colors[ImGuiCol_Text] = ImVec4(0.88f, 0.84f, 0.96f, 0.78f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.88f, 0.84f, 0.96f, 0.28f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.17f, 0.12f, 0.24f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.30f, 0.20f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.30f, 0.20f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.30f, 0.20f, 0.40f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.30f, 0.20f, 0.40f, 0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.30f, 0.20f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.91f, 0.55f, 0.23f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.91f, 0.55f, 0.23f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.93f, 0.52f, 0.17f, 0.14f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.57f, 0.35f, 0.85f, 0.14f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.84f, 0.36f, 0.14f, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.45f, 0.25f, 0.58f, 1.00f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.57f, 0.35f, 0.85f, 0.04f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.88f, 0.84f, 0.96f, 0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.88f, 0.84f, 0.96f, 0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.84f, 0.36f, 0.14f, 0.43f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.22f, 0.16f, 0.28f, 0.9f);
	}
	else if (strcmp(colorScheme, "Dark mode") == 0) {
		ImGui::StyleColorsClassic();
		style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 0.28f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.22f, 0.78f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.45f, 0.45f, 0.45f, 0.14f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.29f, 0.29f, 0.29f, 0.34f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.55f, 0.55f, 0.55f, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.29f, 0.29f, 0.29f, 0.04f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.85f, 0.85f, 0.85f, 0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.85f, 0.85f, 0.85f, 0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.55f, 0.55f, 0.55f, 0.43f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.9f);
	}
	else if (strcmp(colorScheme, "Pink/green/blue colorway") == 0) {
		ImGui::StyleColorsDark();
		style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.58f, 0.70f, 0.39f, 0.78f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.58f, 0.70f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.25f, 0.25f, 0.25f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.58f, 0.70f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.25f, 0.25f, 0.25f, 0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.71f, 0.24f, 0.53f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.58f, 0.70f, 0.39f, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.58f, 0.70f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.58f, 0.70f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.77f, 0.44f, 0.90f, 0.14f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.58f, 0.70f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.12f, 0.89f, 0.91f, 0.14f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.81f, 0.28f, 0.60f, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.81f, 0.28f, 0.60f, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.81f, 0.28f, 0.60f, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.81f, 0.28f, 0.60f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.06f, 0.53f, 0.55f, 0.04f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.28f, 0.60f, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.12f, 0.89f, 0.91f, 0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.12f, 0.89f, 0.91f, 0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.81f, 0.28f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.81f, 0.28f, 0.60f, 0.43f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.9f);
	}
	else if (strcmp(colorScheme, "Blue and grey colorway") == 0) {
		ImGui::StyleColorsDark();
		style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.30f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.35f, 0.45f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.50f, 0.60f, 0.70f, 0.78f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.45f, 0.55f, 0.65f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.30f, 0.40f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.30f, 0.40f, 0.50f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.35f, 0.45f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.28f, 0.36f, 0.46f, 0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.35f, 0.45f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.50f, 0.60f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.65f, 0.75f, 0.85f, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.75f, 0.85f, 0.95f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.80f, 0.90f, 0.14f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.85f, 0.95f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.60f, 0.70f, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.75f, 0.85f, 0.95f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.50f, 0.60f, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.55f, 0.65f, 0.75f, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.77f, 0.87f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.60f, 0.70f, 0.50f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.65f, 0.75f, 0.85f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.85f, 0.95f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.90f, 1.00f, 0.20f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.90f, 0.95f, 1.00f, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.85f, 0.95f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.50f, 0.70f, 0.90f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.30f, 0.50f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.50f, 0.70f, 0.90f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.30f, 0.50f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.50f, 0.60f, 0.70f, 0.43f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.30f, 0.40f, 0.50f, 0.9f);
	}
	else if (strcmp(colorScheme, "Boring colorway") == 0) {
		ImGui::StyleColorsDark();
		style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.30f, 0.30f, 0.78f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.20f, 0.20f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.18f, 0.18f, 0.18f, 0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.80f, 0.80f, 0.80f, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.90f, 0.20f, 0.20f, 0.14f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.35f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.80f, 0.80f, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.70f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.60f, 0.90f, 0.90f, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.80f, 1.00f, 1.00f, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.90f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.80f, 0.80f, 0.50f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 1.00f, 1.00f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.90f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.90f, 0.20f, 0.20f, 0.20f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.35f, 0.35f, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.35f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.90f, 0.30f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.80f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.90f, 0.30f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.90f, 0.80f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.50f, 0.80f, 0.80f, 0.43f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.30f, 0.50f, 0.50f, 0.9f);
	}
	else if (strcmp(colorScheme, "Blue/green colorway") == 0) {
		ImGui::StyleColorsDark();
		style.Colors[ImGuiCol_Text] = ImVec4(0.85f, 0.92f, 0.88f, 0.78f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.85f, 0.92f, 0.88f, 0.28f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.11f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.24f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.73f, 0.69f, 0.78f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.73f, 0.69f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.24f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.24f, 0.27f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.44f, 0.73f, 0.69f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.24f, 0.27f, 0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.24f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.23f, 0.50f, 0.46f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.44f, 0.73f, 0.69f, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.44f, 0.73f, 0.69f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.23f, 0.50f, 0.46f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.21f, 0.47f, 0.44f, 0.14f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.44f, 0.73f, 0.69f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.32f, 0.55f, 0.51f, 0.14f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.05f, 0.38f, 0.42f, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.06f, 0.37f, 0.41f, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.05f, 0.38f, 0.42f, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.11f, 0.32f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.05f, 0.38f, 0.42f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.16f, 0.47f, 0.51f, 0.04f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.05f, 0.38f, 0.42f, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.88f, 0.96f, 0.84f, 0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.88f, 0.96f, 0.84f, 0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.05f, 0.38f, 0.42f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.05f, 0.38f, 0.42f, 0.43f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.29f, 0.32f, 0.9f);
	}
	else if (strcmp(colorScheme, "Green/yellow colorway") == 0) {
		ImGui::StyleColorsDark();
		style.Colors[ImGuiCol_Text] = ImVec4(0.88f, 0.84f, 0.96f, 0.78f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.88f, 0.84f, 0.96f, 0.28f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.18f, 0.12f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.40f, 0.30f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.96f, 0.84f, 0.29f, 0.78f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.96f, 0.84f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.40f, 0.30f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.30f, 0.50f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.96f, 0.84f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.30f, 0.50f, 0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.40f, 0.30f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.84f, 0.89f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.96f, 0.84f, 0.29f, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.96f, 0.84f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.84f, 0.89f, 0.36f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.89f, 0.93f, 0.55f, 0.14f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.96f, 0.84f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.28f, 0.41f, 0.14f, 0.14f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.84f, 0.36f, 0.14f, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.45f, 0.25f, 0.58f, 1.00f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.57f, 0.35f, 0.85f, 0.04f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.36f, 0.14f, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.88f, 0.84f, 0.96f, 0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.88f, 0.84f, 0.96f, 0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.84f, 0.36f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.84f, 0.36f, 0.14f, 0.43f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.22f, 0.16f, 0.28f, 0.9f);
	}
	else if (strcmp(colorScheme, "Orange/grey colorway") == 0) {
		ImGui::StyleColorsDark();
		style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.54f, 0.54f, 0.54f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.48f, 0.48f, 0.48f, 0.78f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.24f, 0.24f, 0.24f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.64f, 0.32f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.30f, 0.30f, 0.30f, 0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.35f, 0.50f, 0.64f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.45f, 0.65f, 0.85f, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.55f, 0.75f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.64f, 0.32f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.35f, 0.50f, 0.64f, 0.14f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.55f, 0.75f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.40f, 0.40f, 0.40f, 0.14f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.93f, 0.75f, 0.18f, 0.78f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.93f, 0.75f, 0.18f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 0.72f, 0.79f, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.93f, 0.75f, 0.18f, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.93f, 0.75f, 0.18f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.54f, 0.90f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.93f, 0.75f, 0.18f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.93f, 0.75f, 0.18f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.54f, 0.90f, 0.70f, 0.04f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.93f, 0.75f, 0.18f, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.93f, 0.75f, 0.18f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.93f, 0.75f, 0.18f, 0.43f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.23f, 0.16f, 0.42f, 0.9f);
	}
	else if (strcmp(colorScheme, "Whitey") == 0) {
		ImGui::StyleColorsDark();
		style.Colors[ImGuiCol_Text] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	}
	else if (strcmp(colorScheme, "Colorful") == 0) {
		ImGui::StyleColorsLight();
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.15f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.40f, 0.40f, 0.40f, 0.13f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.90f, 0.35f, 0.29f, 0.75f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 0.60f, 0.38f, 0.80f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 0.80f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.35f, 0.80f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.35f, 0.80f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.65f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.45f, 0.80f, 0.47f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.15f, 0.35f, 0.80f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.00f, 0.80f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 0.60f, 0.38f, 0.80f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.90f, 0.35f, 0.29f, 0.75f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.80f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.60f, 0.38f, 0.70f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.90f, 0.35f, 0.29f, 0.75f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.15f, 0.35f, 0.80f, 0.70f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.15f, 0.45f, 1.00f, 0.80f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.25f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.15f, 0.35f, 0.80f, 0.45f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.15f, 0.45f, 1.00f, 0.70f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.25f, 0.60f, 0.80f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.15f, 0.35f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.15f, 0.45f, 1.00f, 0.45f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.15f, 0.25f, 0.60f, 0.70f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.15f, 0.35f, 0.80f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.15f, 0.45f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.15f, 0.35f, 0.80f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.15f, 0.45f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.15f, 0.45f, 1.00f, 0.35f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
	}
}


// wchar_t* 转为 std::string
std::string WCharToString(const wchar_t* wcharStr) {
	int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wcharStr, -1, nullptr, 0, nullptr, nullptr);
	if (utf8Size == 0) {
		throw std::runtime_error("Error converting wide string to UTF-8");
	}
	std::vector<char> utf8Str(utf8Size);
	WideCharToMultiByte(CP_UTF8, 0, wcharStr, -1, utf8Str.data(), utf8Size, nullptr, nullptr);

	return std::string(utf8Str.data());
}
struct BWindow
{
	ULONG pid;
	HWND hwnd;
	std::string title;
};

// 当前选中的窗口索引
int current_window_index = 0;
// 当前选中的窗口
BWindow* current_selected_window = nullptr;
std::vector<BWindow> windows;

// 回调函数，用于处理每个枚举到的窗口
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	if (IsWindowVisible(hwnd)) { // 检查窗口是否可见
		wchar_t windowTitle[256];
		// 获取窗口标题
		int length = GetWindowTextW(hwnd, windowTitle, sizeof(windowTitle));
		if (length > 0) {
			DWORD Pid = 0;
			GetWindowThreadProcessId(hwnd, &Pid);
			windows.push_back({ Pid, hwnd, WCharToString(windowTitle) });
		}
	}
	return TRUE;
}

void Common() {
	static bool first = false;
	if (!first) {
		first = true;
		windows.push_back({ 0, 0, "( 无 )" });
		::EnumWindows(EnumWindowsProc, NULL);
	}

	ImGui::Text("PID: %d", selectPid);

	static int current_window_index = 0;
	std::vector<const char*> window_names;
	for (int i = 0; i < windows.size(); ++i)
		window_names.push_back(windows[i].title.c_str());
	if (ImGui::Combo("Choose", &current_window_index, window_names.data(), window_names.size())) {
		selectPid = windows[current_window_index].pid;
		selectHwnd = windows[current_window_index].hwnd;
		driver.B_AttachProcess(selectPid);
	}

	ImGui::SameLine();

	//刷新
	if (ImGui::Button("Refresh")) {
		windows.clear();
		windows.push_back({ 0, 0, "( 无 )" });
		::EnumWindows(EnumWindowsProc, NULL);
		for (int i = 0; i < windows.size(); ++i)
		{
			if (windows[i].pid == selectPid) {
				current_window_index = i;
				break;
			}
		}
	}

	//手动输入PID
	ImGui::SetNextItemWidth(120.f);
	if (ImGui::InputInt("Input PID", (int*)&selectPid, 1, 10)) {
		driver.B_AttachProcess(selectPid);
	}

	ImGui::Dummy(ImVec2(0, 10.0f));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 10.0f));

	static bool bHex = true;
	//获取模块基址 大小
	static char moduleName[256];
	static ULONG64 moduleBase;
	static ULONG moduleSize;

	ImGui::Checkbox("Hex", &bHex);

	if (ImGui::Button("Get Module")) {
		moduleBase = driver.B_GetMoudleBaseAddress(moduleName, &moduleSize);
	}

	ImGui::SameLine();

	ImGui::SetNextItemWidth(200.f);
	ImGui::InputTextWithHint("Module Name", "notepad.exe", moduleName, sizeof moduleName);

	if (ImGui::BeginTable("ModuleTable", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders))
	{
		// 设置表头
		ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Value");
		ImGui::TableHeadersRow();

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Module Base");
		ImGui::TableNextColumn();
		ImGui::Text(bHex ? "0x%llx" : "%llu", moduleBase);

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Module Size");
		ImGui::TableNextColumn();
		ImGui::Text(bHex ? "0x%lx" : "%lu", moduleSize);

		ImGui::EndTable();
	}

	ImGui::Dummy(ImVec2(0, 10.0f));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 10.0f));

	//读写
	static ULONG64 RWAddress = 0;
	ImGui::InputULONG64("Read Address", &RWAddress);
	if (ImGui::IsItemHovered())
		ImGui::SetItemTooltip("Only decimal input is supported");

	static RWMode rwmode = RWMode::Phy;

	static ULONG64 rw_temp = 0;

	static int RWModeIndex = 2;
	static const char* RWModeNames[] =
	{
		"MmCopy",
		"Mdl",
		"Phy",
		"Ke"
	};

	if (ImGui::Button("Read")) {
		driver.B_ReadMemory(RWAddress, &rw_temp, sizeof rw_temp, rwmode);
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(120.f);
	if (ImGui::Combo("RWMode", &RWModeIndex, RWModeNames, IM_ARRAYSIZE(RWModeNames)))
		rwmode = (RWMode)RWModeIndex;

	if (ImGui::BeginTable("RWTable", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders))
	{
		ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Value");
		ImGui::TableHeadersRow();

		ImGui::TableNextRow();//byte
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Byte");
		ImGui::TableNextColumn();
		byte rw_byte = 0;
		memcpy(&rw_byte, &rw_temp, sizeof rw_byte);
		ImGui::Text(bHex ? "0x%hhx" : "%hhu", rw_byte);

		ImGui::TableNextRow();//2byte
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("2Byte");
		ImGui::TableNextColumn();
		WORD rw_2byte = 0;
		memcpy(&rw_2byte, &rw_temp, sizeof rw_2byte);
		ImGui::Text(bHex ? "0x%hx" : "%hu", rw_2byte);

		ImGui::TableNextRow();//4byte
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("4Byte");
		ImGui::TableNextColumn();
		DWORD rw_4byte = 0;
		memcpy(&rw_4byte, &rw_temp, sizeof rw_4byte);
		ImGui::Text(bHex ? "0x%lx" : "%u", rw_4byte);

		ImGui::TableNextRow();//8byte
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("8Byte");
		ImGui::TableNextColumn();
		ImGui::Text(bHex ? "0x%llx" : "%llu", rw_temp);

		ImGui::TableNextRow();//float
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Float");
		ImGui::TableNextColumn();
		float rw_float = 0;
		memcpy(&rw_float, &rw_temp, sizeof rw_float);
		ImGui::Text(bHex ? "0x%lx" : "%f", rw_float);

		ImGui::TableNextRow();//double
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("Double");
		ImGui::TableNextColumn();
		double rw_double = 0;
		memcpy(&rw_double, &rw_temp, sizeof rw_double);
		ImGui::Text(bHex ? "0x%llx" : "%lf", rw_double);

		ImGui::EndTable();
	}
}

void KeyMouse() {
	ImGui::Text("Mouse move");
	ImGui::Dummy({ 0, 10.f });

	static int movementX = 50;
	static int movementY = 50;

	ImGui::InputInt("X", &movementX);
	ImGui::InputInt("Y", &movementY);

	static const char* moveTypeNames[] = { "relative", "absolute" };
	static int currentMoveTypeIndex = 0;
	ImGui::Combo("MoveType", &currentMoveTypeIndex, moveTypeNames, IM_ARRAYSIZE(moveTypeNames));

	if (ImGui::Button("Move")) {
		driver.B_MouseMove(movementX, movementY, (MoveType)currentMoveTypeIndex);
	}

	ImGui::Dummy(ImVec2(0, 10.0f));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 10.0f));

	ImGui::Text("Mouse Control");
	ImGui::Dummy({ 0, 10.f });

	static const char* mouseKeyNames[] = { "LEFT", "RIGHT", "MIDDLE", "X1", "X2" };
	static int currentMouseKeyIndex = 0;
	ImGui::Combo("MouseKey", &currentMouseKeyIndex, mouseKeyNames, IM_ARRAYSIZE(mouseKeyNames));

	static const char* mouseStateNames[] = { "UP", "DOWN" };
	static int currentMouseStateIndex = 0;
	ImGui::Combo("KeyState", &currentMouseStateIndex, mouseStateNames, IM_ARRAYSIZE(mouseStateNames));

	if (ImGui::Button("Control"))
		driver.B_MouseCtl((MouseKey)currentMouseKeyIndex, (MouseStatus)currentMouseStateIndex);

	ImGui::Dummy(ImVec2(0, 10.0f));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 10.0f));

	ImGui::Text("Keyboard");
	ImGui::Dummy({ 0, 10.f });

	if (ImGui::Button("Click \'a\'"))
		driver.B_KeyClick('a');
}

#include <shlobj.h>
#include "../driver/TestDLL.c"

void Injector()
{
	ImGui::Text("Inject Dll");
	ImGui::Dummy({ 0, 10.f });

	ImGui::Text("Process ID: %d", selectPid);

	static char dllPath[MAX_PATH] = { 0 };
	if (ImGui::Button("Choose dll")) {
		TCHAR szBuffer[MAX_PATH] = { 0 };
		OPENFILENAME ofn = { 0 };
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = 0;
		ofn.lpstrFilter = L"dll文件(*.dll)\0*.dll\0所有文件(*.*)\0*.*\0";//要选择的文件后缀   
		ofn.lpstrInitialDir = L"D:\\Program Files";//默认的文件路径   
		ofn.lpstrFile = szBuffer;//存放文件的缓冲区   
		ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
		ofn.nFilterIndex = 0;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//标志如果是多选要加上OFN_ALLOWMULTISELECT  
		BOOL bSel = GetOpenFileName(&ofn);

		strcpy(dllPath, WCharToString(szBuffer).c_str());
	}

	ImGui::Text("dll: %s", dllPath);

	ImGui::Dummy({ 0,10.f });

	static const char* injectTypeNames[] =
	{
		"V1 mapping (just support dx11, support ACE)",
		"V2 Normal mapping (Most of the game's anti-cheats can be blocked)",
		"V3 APC Maping (support EAC,BE,ACE)",
		"V4 RIP Maping (The dll must be compiled in MD/MT)",
		"V5 RIPV2 Maping (It depends on your luck)"
	};

	static int currentInjectTypeIndex = 0;

	ImGui::Combo("InjectType", &currentInjectTypeIndex, injectTypeNames, IM_ARRAYSIZE(injectTypeNames));

	if (ImGui::Button("Inject by Path")) {
		bool result = false;
		if ( selectPid && dllPath[0] != '\0') {
			if (currentInjectTypeIndex == 0) {
				result = driver.B_MapDll(dllPath);
			}
			else if (currentInjectTypeIndex == 1) {
				result = driver.B_MapDLLV2(dllPath);
			}
			else if (currentInjectTypeIndex == 2) {
				result = driver.B_MapDLLV3(dllPath);
			}
			else if (currentInjectTypeIndex == 3) {
				result = driver.B_MapDLLV4(dllPath);
			}
			else if (currentInjectTypeIndex == 4) {
				result = driver.B_MapDLLV5(dllPath);
			}
		}
		MessageBoxA(0, result ? "Inject successful!" : "Inject failed!", "Inject result", MB_OK);
	}

	if (ImGui::Button("Inject by memory (\"TestDLL.c\")")) {
		bool result = false;
		if (selectPid) {
			if (currentInjectTypeIndex == 0) {
				result = driver.B_MapDll(TestDLL, sizeof TestDLL);
			}
			else if (currentInjectTypeIndex == 1) {
				result = driver.B_MapDLLV2(TestDLL, sizeof TestDLL);
			}
			else if (currentInjectTypeIndex == 2) {
				result = driver.B_MapDLLV3(TestDLL, sizeof TestDLL);
			}
			else if (currentInjectTypeIndex == 3) {
				result = driver.B_MapDLLV4(TestDLL, sizeof TestDLL);
			}
			else if (currentInjectTypeIndex == 4) {
				result = driver.B_MapDLLV5(TestDLL, sizeof TestDLL);
			}
		}
		MessageBoxA(0, result ? "Inject successful!" : "Inject failed!", "Inject result", MB_OK);
	}
}

void ProtectHide()
{
	ImGui::Text("Protect");
	ImGui::Dummy({ 0, 10.f });

	ImGui::Text("Process ID: %d", selectPid);

	static bool protectProcess = false;
	if (ImGui::Checkbox("Protect Process", &protectProcess))
		driver.B_ProtectProcess(protectProcess, selectPid);
	if (ImGui::IsItemHovered())
		ImGui::SetItemTooltip("Prevent debugging of software such as CE and OD attach");

	static bool protectWindow = false;
	static bool protectWindowGlobal = false;
	ImGui::Checkbox("protect window (Global)", &protectWindowGlobal);
	if (ImGui::IsItemHovered())
		ImGui::SetItemTooltip("Blocks all functions that get window handles, Use carefully!");

	if (ImGui::Checkbox("Protect Window", &protectWindow))
		driver.B_ProtectWindow(protectWindow, selectPid, protectWindowGlobal);
	if (ImGui::IsItemHovered())
		ImGui::SetItemTooltip("Prevent you from being searched by any means of searching for window handles");

	ImGui::Dummy(ImVec2(0, 10.0f));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 10.0f));
	ImGui::Text("Hide");
	ImGui::Dummy({ 0, 10.f });

	ImGui::Text("Window Hwnd: %p", selectHwnd);

	static bool hideProcess = false;
	if (ImGui::Checkbox("Hide Process", &hideProcess))
		driver.B_HideProcess(hideProcess, selectPid);

	static const char* AntiScreenshotNames[] =
	{
		"None",
		"Transparent",
		"Black",
	};

	static int currentAntiScreenshotIndex = 0;

	ImGui::Combo("AntiScreenshot Type", &currentAntiScreenshotIndex, AntiScreenshotNames, IM_ARRAYSIZE(AntiScreenshotNames));

	if (ImGui::Button("Apply anti screenshot"))
		driver.B_HideWindow((ULONG64)selectHwnd, (HideWindowType)currentAntiScreenshotIndex);


}

void Misc()
{
	ImGui::Text("Dump module");
	ImGui::Dummy({ 0, 10.f });

	static char dumpModule[MAX_PATH] = { 0 };
	static char dumpFile[MAX_PATH] = { 0 };

	ImGui::InputTextWithHint("need dump module", "notepad.exe", dumpModule, sizeof dumpModule);
	ImGui::InputTextWithHint("dump file", "C:\\BFDrv_Dump.bin", dumpFile, sizeof dumpFile);

	if (ImGui::Button("Dump to file"))
	{
		ULONG size = 0;
		auto base = driver.B_GetMoudleBaseAddress(dumpModule, &size);
		auto result = driver.B_DumpToFile(base, size, dumpFile, RWMode::MmCopy);

		MessageBoxA(0, result ? "Dump successful!" : "Dump failed!", "Dump result", MB_OK);
	}

	ImGui::Dummy(ImVec2(0, 10.0f));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 10.0f));
	ImGui::Text("Get Module Export Function");
	ImGui::Dummy({ 0, 10.f });

	static char ExportModule[MAX_PATH] = { 0 };
	static char ExportFuncName[MAX_PATH] = { 0 };

	ImGui::InputTextWithHint("Module", "Kernel32.dll", ExportModule, sizeof ExportModule);
	ImGui::InputTextWithHint("Export function", "IsBadReadPtr", ExportFuncName, sizeof ExportFuncName);

	static char exportModuleName[100] = { 0 };
	static ULONG64 exportFuncAddress = 0;
	if (ImGui::Button("Get export function address")) {
		ULONG size = 0;
		auto base = driver.B_GetMoudleBaseAddress(ExportModule, &size);
		exportFuncAddress = driver.B_GetMoudleExportFuncAddress(base, ExportFuncName);
		if (exportFuncAddress != 0)
			strcpy(exportModuleName, ExportFuncName);
	}

	if (ImGui::BeginTable("ExportFunc", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders))
	{
		ImGui::TableSetupColumn("Func Name", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Address");
		ImGui::TableHeadersRow();

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text(exportModuleName);
		ImGui::TableNextColumn();
		ImGui::Text("0x%llx", exportFuncAddress);

		ImGui::EndTable();
	}

	ImGui::Dummy(ImVec2(0, 10.0f));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 10.0f));
	ImGui::Text("Pattern Scanning");
	ImGui::Dummy({ 0, 10.f });

	static char signatue[256] = { 0 };
	static char scanModule[256] = { 0 };
	static std::vector<ULONG64> result;

	ImGui::InputTextWithHint("Scanning module", "notepad.exe", scanModule, sizeof scanModule);
	ImGui::InputTextWithHint("Signature", "CC ? 83 EC 28", signatue, sizeof signatue);

	if (ImGui::Button("Scan")) {
		ULONG size = 0;
		auto base = driver.B_GetMoudleBaseAddress(scanModule, &size);
		if (base) {
			result = driver.B_AOBScanV2(signatue, base, size, RWMode::MmCopy);
		}

	}
	if (!result.empty())
	{
		ImGui::BeginTable("Scan result", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders);
		ImGui::TableSetupColumn("Count", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn(" - Scan result - ", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();

		int count = 1;
		for(auto i : result)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%d", count);
			ImGui::TableSetColumnIndex(1);
			ImGui::Text("0x%llx", i);
			count++;
		}

		ImGui::EndTable();
	}

}

void Info()
{
	ImGui::Text("Hi, Welcome use BFDrv Tools");
	ImGui::Dummy({ 0, 5.f });
	ImGui::Text("Expiration: %s", driver.B_GetExpiration());
	ImGui::Dummy({ 0, 5.f });
	ImGui::Text("System version: %d", driver.B_GetWindowsBuildNumber());
	ImGui::Dummy({ 0, 5.f });
	ImGui::Text("Driver last build time: %s", driver.B_GetDriverBuildTime().c_str());
}
