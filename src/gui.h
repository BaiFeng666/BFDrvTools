#ifndef GUI_H
#define GUI_H
#pragma once
#include <d3d11.h>
#include <memory>
#include <chrono>

#include "ImGui/imgui.h"
#include "magiXOR.h"
#include "ImGui/imgui_internal.h"

inline ID3D11ShaderResourceView* logoImage = nullptr;

inline HWND g_hwnd;

class GUI final
{
public:
	void Draw(HWND hwnd);
	void DrawLoginInterface(HWND hwnd);
	void DrawInterface(HWND hwnd);
	void ApplyColorScheme(const char* colorScheme);

	enum class ColorScheme {
		Dark,
		Light,
		Classic
	};

	const ImVec2 window_pos{ 0, 0 };
	const ImVec2 window_size{ 600, 350 };

	bool loginFailed = false;
	bool drawLogin = true;
	bool drawInterface = false;

private:
	bool active = true;
	char user_name[255] = "";

	bool isLoggedIn = false;
};

inline const char* colorSchemes[] = { "Standard", "Cinder", "Green colorway", "Blue colorway", "Purple/orange colorway", "Dark mode", "Pink/green/blue colorway", "Blue and grey colorway", "Boring colorway", "Blue/green colorway", "Green/yellow colorway", "Orange/grey colorway", "Whitey", "Colorful" };

inline std::unique_ptr<GUI> gui = std::make_unique<GUI>();

#endif
