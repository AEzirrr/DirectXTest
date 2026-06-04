#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RasterizerState.h"

#include "Quad.h"

class AppWindow : public Window
{
public:
	AppWindow();
	~AppWindow();

	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

private:
	RasterizerState* rasterizerState;

	SwapChain* m_swap_chain;
	VertexBuffer* m_vertex_buffer;

	VertexShader* m_vertex_shader;
	PixelShader* m_pixel_shader;

	Quad* m_rectangle1;
	Quad* m_rectangle2;
	Quad* m_rectangle3;

};

