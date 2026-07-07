#include "UIManager.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "InspectorScreen.h"
#include "HierarchyScreen.h"
#include "ToolbarScreen.h"

UIManager* UIManager::sharedInstance = nullptr;

UIManager* UIManager::getInstance()
{
	return sharedInstance;
}

void UIManager::initialize(HWND windowHandle)
{
	if (sharedInstance == nullptr)
	{
		sharedInstance = new UIManager(windowHandle);
	}
}

void UIManager::destroy()
{
	if (sharedInstance != nullptr)
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

void UIManager::drawAllUI()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (int i = 0; i < uiList.size(); i++)
	{
		uiList[i]->drawUI();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

AUIScreen* UIManager::getScreen(String name)
{

	if (uiTable.find(name) != uiTable.end()) {
		return uiTable[name];
	}
	return nullptr;
}

UIManager::UIManager(HWND windowHandle)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(windowHandle);
	ImGui_ImplDX11_Init(GraphicsEngine::getInstance()->getDevice(), GraphicsEngine::getInstance()->getImmediateDeviceContext()->getD3DContext());

	UINames uiNames;
	InspectorScreen* inspectorScreen = new InspectorScreen(uiNames.INSPECTOR_SCREEN);
	this->uiTable[uiNames.INSPECTOR_SCREEN] = inspectorScreen;
	this->uiList.push_back(inspectorScreen);

	HierarchyScreen* hierarchyScreen = new HierarchyScreen(uiNames.HIERARCHY_SCREEN);
	this->uiTable[uiNames.HIERARCHY_SCREEN] = hierarchyScreen;
	this->uiList.push_back(hierarchyScreen);

	ToolbarScreen* toolbarScreen = new ToolbarScreen(uiNames.TOOLBAR_SCREEN);
	this->uiTable[uiNames.TOOLBAR_SCREEN] = toolbarScreen;
	this->uiList.push_back(toolbarScreen);

	/*ProfilerScreen* profilerScreen = new ProfilerScreen(uiNames.PROFILER_SCREEN);
	this->uiTable[uiNames.PROFILER_SCREEN] = profilerScreen;
	this->uiList.push_back(profilerScreen);*/
	
}

UIManager::~UIManager()
{
	for (int i = 0; i < uiList.size(); i++)
	{
		delete uiList[i];
	}
	uiList.clear();
	uiTable.clear();
}
