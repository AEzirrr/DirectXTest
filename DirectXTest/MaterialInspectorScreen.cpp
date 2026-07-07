#include "MaterialInspectorScreen.h"
#include "ImGui/imgui.h"
#include "AGameObject.h"

MaterialInspectorScreen::MaterialInspectorScreen(String name) : AUIScreen(name)
{}

MaterialInspectorScreen::~MaterialInspectorScreen()
{}

void MaterialInspectorScreen::drawUI()
{
	ImGui::Begin(getName().c_str());

	ImGui::Text("Material Inspector");

	ImGui::End();
}