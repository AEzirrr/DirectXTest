#pragma once
#include "AUIScreen.h"
#include <vector>

class AGameObject;

class HierarchyScreen : public AUIScreen
{
public:
	HierarchyScreen(String name);
	~HierarchyScreen();

	void drawUI() override;

	void addObject(AGameObject* gameObject);

private:
	std::vector<AGameObject*> m_objectList;

	AGameObject* m_selectedObject = nullptr;
};