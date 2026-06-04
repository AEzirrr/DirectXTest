#pragma once
#include <d3d11.h>

class SwapChain;
class VertexBuffer;
class VertexShader;
class PixelShader;
class RasterizerState;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context);

	bool clearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha);
	void setVertexBuffer(VertexBuffer* vertex_buffer);
	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void setViewportSize(UINT width, UINT height);
	
	void setVertexShader(VertexShader* vertex_shader);
	void setPixelShader(PixelShader* pixel_shader);

public:

	void setRasterizerState(RasterizerState* rasterizer_state); // method for setting the rasterizer state to the device context
	void clearRasterizerState(RasterizerState* rasterizer_state); // method for clearing the rasterizer state from the device context

	bool release();
	~DeviceContext();

private:
	ID3D11DeviceContext* m_device_context;
};

