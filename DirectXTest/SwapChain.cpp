#include "SwapChain.h"
#include "GraphicsEngine.h"

SwapChain::SwapChain()
{
}

SwapChain::~SwapChain()
{
}

bool SwapChain::init(HWND hwnd, UINT width, UINT height)
{
	ID3D11Device* device = GraphicsEngine::getInstance()->m_d3d_device;

	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
	swap_chain_desc.BufferCount = 1; // Double buffering
	swap_chain_desc.BufferDesc.Width = width;
	swap_chain_desc.BufferDesc.Height = height;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32-bit color
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 144; // Refresh rate
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Use the back buffer as a render target
	swap_chain_desc.OutputWindow = hwnd; // Handle to the window
	swap_chain_desc.SampleDesc.Count = 1; // No multi-sampling
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.Windowed = TRUE; // Windowed mode


	// Create the swap chain for the window
	HRESULT hr = GraphicsEngine::getInstance()->m_dxgi_factory->CreateSwapChain(device, &swap_chain_desc, &m_swap_chain);

	if (FAILED(hr))
		return false;

	ID3D11Texture2D* buffer=NULL;
	hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(hr))
		return false;

	hr = device->CreateRenderTargetView(buffer, NULL, &m_render_target_view);
	buffer->Release();

	if (FAILED(hr))
		return false;

	return true;
}

bool SwapChain::present(bool vsync)
{
	m_swap_chain->Present(vsync, NULL);
	return true;
}

bool SwapChain::release()
{
	m_swap_chain->Release();
	delete this;
	return true;
}
