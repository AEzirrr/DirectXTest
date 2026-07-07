#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_dx11.h"
#include "ImGui/backends/imgui_impl_win32.h"
#include "Window.h"
#include "AUIScreen.h"

class UINames {
public:
	const String PROFILER_SCREEN = "PROFILER_SCREEN";
	const String TOOLBAR_SCREEN = "TOOLBAR_SCREEN";
	const String INSPECTOR_SCREEN = "INSPECTOR_SCREEN";
	const String HIERARCHY_SCREEN = "HIERARCHY_SCREEN";
};


class UIManager
{
public:

	typedef std::string String;
	typedef std::vector<AUIScreen*> UIList;
	typedef std::unordered_map<String, AUIScreen*> UITable;

	static UIManager* getInstance();
	static void initialize(HWND windowHandle);
	static void destroy();

	void drawAllUI();

	AUIScreen* getScreen(String name);

	static const int WINDOW_WIDTH = 1440;
	static const int WINDOW_HEIGHT = 900;

private:
    UIManager(HWND windowHandle);
    ~UIManager();
    UIManager(UIManager const&) {};
	UIManager& operator=(UIManager const&) {};
	static UIManager* sharedInstance;

    UIList uiList;
	UITable uiTable;
};
