#pragma once
#include "AUIScreen.h"

class ProfilerScreen : public AUIScreen
{
public:
	ProfilerScreen(String name);
	~ProfilerScreen();

	void drawUI() override;

private:
};

