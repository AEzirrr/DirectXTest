#pragma once
#include "AUIScreen.h"

class AGameObject;

class InspectorScreen : public AUIScreen
{
public:
	InspectorScreen(String name);
	~InspectorScreen();

	void drawUI() override;

	void selectObject(AGameObject* object);
	AGameObject* getSelectedObject() const { return selectedObject; }

private:
	AGameObject* selectedObject = nullptr;
};

