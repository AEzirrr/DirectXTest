#include "InspectorScreen.h"
#include "ImGui/imgui.h"
#include "AGameObject.h"

InspectorScreen::InspectorScreen(String name) : AUIScreen(name)
{}

InspectorScreen::~InspectorScreen()
{}

void InspectorScreen::drawUI()
{
	ImGui::Begin(getName().c_str());

	if (selectedObject == nullptr)
	{
		ImGui::Text("No object selected.");
		ImGui::End();
		return;
	}

	ImGui::Text("Object Properties");
	ImGui::Separator();

	Vector3D currentPos = selectedObject->getLocalPosition();
	Vector3D currentRot = selectedObject->getLocalRotation();
	Vector3D currentScale = selectedObject->getLocalScale();
	float posArray[3] = { currentPos.x, currentPos.y, currentPos.z };
	float rotArray[3] = { currentRot.x, currentRot.y, currentRot.z };
	float scaleArray[3] = { currentScale.x, currentScale.y, currentScale.z };

	char nameBuffer[256];
	strncpy_s(nameBuffer, selectedObject->getName().c_str(), sizeof(nameBuffer));
	if (ImGui::InputText("Object Name", nameBuffer, sizeof(nameBuffer)))
	{
		selectedObject->setName(std::string(nameBuffer));
	}

	if (ImGui::DragFloat3("Position", posArray, 0.1f))
	{
		selectedObject->setPosition(posArray[0], posArray[1], posArray[2]);
	}

	if (ImGui::DragFloat3("Rotation", rotArray, 0.1f))
	{
		selectedObject->setRotation(rotArray[0], rotArray[1], rotArray[2]);
	}

	if (ImGui::DragFloat3("Scale", scaleArray, 0.1f))
	{
		selectedObject->setScale(scaleArray[0], scaleArray[1], scaleArray[2]);
	}

	if (ImGui::Button("Reset Position")) {
		selectedObject->setPosition(0.0f, 0.5f, 0.0f);
	}

	if (ImGui::Button("Reset Rotation")) {
		selectedObject->setRotation(0.0f, 0.0f, 0.0f);
	}

	if (ImGui::Button("Reset Scale")) {
		selectedObject->setScale(1.0f, 1.0f, 1.0f);
	}

	ImGui::End();
}

void InspectorScreen::selectObject(AGameObject* object)
{
	selectedObject = object;
}

