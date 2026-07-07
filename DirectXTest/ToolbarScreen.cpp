#include "ToolbarScreen.h"
#include "ImGui/imgui.h"

ToolbarScreen::ToolbarScreen(String name) : AUIScreen(name)
{}

ToolbarScreen::~ToolbarScreen()
{}

void ToolbarScreen::setListener(ToolbarListener* listener)
{
	m_listener = listener;
}

void ToolbarScreen::drawUI()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			// We can leave this empty for now
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Game Object"))
		{
			if (ImGui::MenuItem("Create Cube")) {
				if (m_listener != nullptr) m_listener->onCreateCubeClicked();
			}
			if (ImGui::MenuItem("Create Sphere")) {
				if (m_listener != nullptr) m_listener->onCreateSphereClicked();
			}
			if (ImGui::MenuItem("Create Plane")) {
				if (m_listener != nullptr) m_listener->onCreatePlaneClicked();
			}


			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar(); 
	}
}