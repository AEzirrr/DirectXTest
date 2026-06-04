#include "AppWindow.h"

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	//Window::onCreate();
	GraphicsEngine::getInstance()->init();
	m_swap_chain = GraphicsEngine::getInstance()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);


	rasterizerState = new RasterizerState();
	rasterizerState->InitializeWireframe(GraphicsEngine::getInstance()->getDevice()); // wireframe intialization

	//GraphicsEngine::getInstance()->createShaders();
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::getInstance()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	m_vertex_shader = GraphicsEngine::getInstance()->createVertexShader(shader_byte_code, size_shader);

	// Quad 1: Left Side (Red/Yellow theme)
	QuadVertex q1_v1 = { {-0.8f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f} }; // Bottom-Left
	QuadVertex q1_v2 = { {-0.8f,  0.5f, 0.0f}, {1.0f, 1.0f, 0.0f} }; // Top-Left
	QuadVertex q1_v3 = { {-0.3f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f} }; // Bottom-Right
	QuadVertex q1_v4 = { {-0.3f,  0.5f, 0.0f}, {1.0f, 1.0f, 0.0f} }; // Top-Right
	m_rectangle1 = new Quad(q1_v1, q1_v2, q1_v3, q1_v4, shader_byte_code, size_shader);

	// Quad 2: Center (Green/Cyan theme)
	QuadVertex q2_v1 = { {-0.2f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f} };
	QuadVertex q2_v2 = { {-0.2f,  0.5f, 0.0f}, {0.0f, 1.0f, 1.0f} };
	QuadVertex q2_v3 = { { 0.3f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f} };
	QuadVertex q2_v4 = { { 0.3f,  0.5f, 0.0f}, {0.0f, 1.0f, 1.0f} };
	m_rectangle2 = new Quad(q2_v1, q2_v2, q2_v3, q2_v4, shader_byte_code, size_shader);

	// Quad 3: Right Side (Blue/Magenta theme)
	QuadVertex q3_v1 = { { 0.4f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f} };
	QuadVertex q3_v2 = { { 0.4f,  0.5f, 0.0f}, {1.0f, 0.0f, 1.0f} };
	QuadVertex q3_v3 = { { 0.9f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f} };
	QuadVertex q3_v4 = { { 0.9f,  0.5f, 0.0f}, {1.0f, 0.0f, 1.0f} };
	m_rectangle3 = new Quad(q3_v1, q3_v2, q3_v3, q3_v4, shader_byte_code, size_shader);

	GraphicsEngine::getInstance()->releaseCompiledShader();


	GraphicsEngine::getInstance()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_pixel_shader = GraphicsEngine::getInstance()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::getInstance()->releaseCompiledShader();

}

void AppWindow::onUpdate()
{
	//Window::onUpdate();
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, .3f, 0.3f, 0.3f, 1.0f);


	////////// WIREFRAME TEST CONTROLS //////////
	if (GetAsyncKeyState('W') & 0x8000) //Wireframe mode
	{
		GraphicsEngine::getInstance()->getImmediateDeviceContext()->setRasterizerState(rasterizerState);
	}
	else if (GetAsyncKeyState('S') & 0x8000) //solid mode
	{
		GraphicsEngine::getInstance()->getImmediateDeviceContext()->clearRasterizerState(rasterizerState);
	}
	////////// WIREFRAME TEST CONTROLS ///////////

	
	RECT rc = this->getClientWindowRect();

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	//GraphicsEngine::getInstance()->setShaders();
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexShader(m_vertex_shader);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setPixelShader(m_pixel_shader);

	if (m_rectangle1) m_rectangle1->draw();
	if (m_rectangle2) m_rectangle2->draw();
	if (m_rectangle3) m_rectangle3->draw();



	// Triangle List method
	// GraphicsEngine::getInstance()->getImmediateDeviceContext()->drawTriangleStrip(m_vertex_buffer->getSizeVertexList(), 0);


	m_swap_chain->present(false);
}

void AppWindow::onDestroy()
{
	//Window::onDestroy();

	rasterizerState->release();

	m_swap_chain->release();
	m_vertex_buffer->release();

	m_rectangle1->release();;
	m_rectangle2->release();;
	m_rectangle3->release();;

	m_vertex_shader->release();
	m_pixel_shader->release();
	GraphicsEngine::getInstance()->release();
}
