#pragma once
#include <D3D11.h>

class DeviceContext;
class SwapChain
{
public:
	SwapChain();
	~SwapChain();
	// Initializes the swap chain and render target view
	bool init(HWND hwnd, UINT width, UINT height);

	bool present(bool vsync);

	// Releases all the resources loaded
	bool release();

private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_render_target_view;

private:
	friend class DeviceContext;
};

