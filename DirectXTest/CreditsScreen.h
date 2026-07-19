#pragma once
#include "AUIScreen.h"
#include <d3d11.h>

class CreditsScreen : public AUIScreen
{
public:
	CreditsScreen(String name);
	CreditsScreen(String name, ID3D11Device* device);
	~CreditsScreen();

	bool LoadTextureFromFile(const wchar_t* filename, ID3D11Device* device, ID3D11ShaderResourceView** outSRV, int* outWidth, int* outHeight);

	void drawUI() override;

	bool isOpen = false;

private:
	ID3D11ShaderResourceView* m_logoTexture = nullptr;
	int m_logoWidth = 0;   
	int m_logoHeight = 0; 

	float m_displayWidth = 200.0f;
	float m_displayHeight = 200.0f;
};