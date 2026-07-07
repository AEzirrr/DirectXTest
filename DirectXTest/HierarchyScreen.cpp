#include "HierarchyScreen.h"
#include "ImGui/imgui.h"
#include "AGameObject.h"
#include "UIManager.h"
#include "InspectorScreen.h"

HierarchyScreen::HierarchyScreen(String name) : AUIScreen(name)
{}

HierarchyScreen::~HierarchyScreen()
{}

void HierarchyScreen::addObject(AGameObject* gameObject)
{
	m_objectList.push_back(gameObject);
}

void HierarchyScreen::drawUI()
{
	ImGui::Begin(getName().c_str());

	for (int i = 0; i < m_objectList.size(); i++)
	{
		String name = m_objectList[i]->getName();

		bool isSelected = (m_selectedObject == m_objectList[i]);

		if (ImGui::Selectable(name.c_str(), isSelected))
		{
			m_selectedObject = m_objectList[i];

			AUIScreen* inspector = UIManager::getInstance()->getScreen("INSPECTOR_SCREEN");

			if (inspector != nullptr)
			{
				InspectorScreen* customInspector = static_cast<InspectorScreen*>(inspector);
				customInspector->selectObject(m_objectList[i]);
			}
		}
	}

	ImGui::End();
}