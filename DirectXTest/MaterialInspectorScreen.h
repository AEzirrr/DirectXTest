#pragma once
#include "AUIScreen.h"

class MaterialInspectorScreen : public AUIScreen
{
public:
	MaterialInspectorScreen(String name);
	~MaterialInspectorScreen();

	void drawUI() override;

private:
};

