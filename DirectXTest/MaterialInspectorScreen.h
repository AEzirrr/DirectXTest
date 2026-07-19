#pragma once
#include "AUIScreen.h"
#include <vector>  
#include <string>

class MaterialInspectorScreen : public AUIScreen
{
public:
	MaterialInspectorScreen(String name);
	~MaterialInspectorScreen();

	void drawUI() override;

private:
    void refreshAssetCache();

private:
    std::vector<std::string> m_availableTextures;
    char m_searchBuffer[128] = "";
    float m_cacheRefreshTimer = 0.0f;
};

