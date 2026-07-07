#include "ProfilerScreen.h"
#include "ImGui/imgui.h"

ProfilerScreen::ProfilerScreen(String name) : AUIScreen(name)
{}

ProfilerScreen::~ProfilerScreen()
{}

void ProfilerScreen::drawUI()
{
	ImGui::Begin(getName().c_str());

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
}

