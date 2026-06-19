#pragma once
#include <D3D11.h>

class DeviceContext;
class SwapChain
{
public:
	SwapChain();
	~SwapChain();
	bool init(HWND hwnd, UINT width, UINT height);

	bool present(bool vsync);

	bool release();

private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_render_target_view;
	ID3D11DepthStencilView* m_depth_stencil_view;

private:
	friend class DeviceContext;
};

