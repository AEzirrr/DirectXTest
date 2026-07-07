#pragma once
#include "AUIScreen.h"

class ToolbarListener
{
public:
	virtual void onCreateCubeClicked() = 0;
	virtual void onCreateSphereClicked() = 0;
	virtual void onCreatePlaneClicked() = 0;
};

class ToolbarScreen : public AUIScreen
{
public:
	ToolbarScreen(String name);
	~ToolbarScreen();

	void drawUI() override;

	void setListener(ToolbarListener* listener);

private:
	ToolbarListener* m_listener = nullptr;
};