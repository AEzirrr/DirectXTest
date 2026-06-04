#include "AppWindow.h"
#include <Windows.h>
#include "Matrix4x4.h"

__declspec(align(16)) struct constant {
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_projection;
	unsigned int m_time;
	Vector3D m_color;
};

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

	////////// VERTEX SHADER INITIALIZATION //////////
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::getInstance()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vertex_shader = GraphicsEngine::getInstance()->createVertexShader(shader_byte_code, size_shader);
	////////// VERTEX SHADER INITIALIZATION //////////

	////////// QUAD INITIALIZATION //////////
	m_quad1 = new Quad(shader_byte_code, size_shader);
	m_quad2 = new Quad(shader_byte_code, size_shader);
	m_quad3 = new Quad(shader_byte_code, size_shader);

	m_quad1->setPosition(-1.0f, 0.0f, 0.0f);
	m_quad1->setScale(0.1f); // uniform scale 
	m_quad1->setColor(1.0f, 0.0f, 0.0f);

	m_quad2->setPosition(0.0f, 0.0f, 0.0f);
	m_quad2->setScale(0.3f, 0.3f, 0.3f); // non uniform scale
	m_quad2->setColor(Vector3D(0.0f, 1.0f, 0.0f), Vector3D(1.0f, 0.0f, 0.0f), Vector3D(0.0f, 0.0f, 1.0f), Vector3D(0.0f, 1.0f, 1.0f));

	m_quad3->setPosition(1.0f, 0.0f, 0.0f);
	m_quad3->setScale(0.4f, 0.4f, 0.4f);
	m_quad3->setColor(0.0f, 0.0f, 1.0f);
	////////// QUAD INITIALIZATION //////////

	GraphicsEngine::getInstance()->releaseCompiledShader();


	////////// PIXEL SHADER INITIALIZATION //////////
	GraphicsEngine::getInstance()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_pixel_shader = GraphicsEngine::getInstance()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::getInstance()->releaseCompiledShader();
	///////// PIXEL SHADER INITIALIZATION //////////

	constant cc;
	cc.m_time = 0;

	m_constant_buffer = GraphicsEngine::getInstance()->createConstantBuffer();
	m_constant_buffer->load(&cc, sizeof(constant));

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

	constant cc;
	cc.m_time = GetTickCount();
	cc.m_view.setIdentity();
	cc.m_projection.setOrthoLH(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 400.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 400.0f,
		-4.0f,
		4.0f
	);


	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setConstantBuffer(m_vertex_shader, m_constant_buffer);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setConstantBuffer(m_pixel_shader, m_constant_buffer);

	//GraphicsEngine::getInstance()->setShaders();
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexShader(m_vertex_shader);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setPixelShader(m_pixel_shader);


	// Quad drawing
	if (m_quad1) {
		cc.m_world = m_quad1->getTransform();
		cc.m_color = m_quad1->getColor();
		m_constant_buffer->update(GraphicsEngine::getInstance()->getImmediateDeviceContext(), &cc);
		m_quad1->draw();
	}

	if (m_quad2) {
		cc.m_world = m_quad2->getTransform();
		cc.m_color = m_quad2->getColor();
		m_constant_buffer->update(GraphicsEngine::getInstance()->getImmediateDeviceContext(), &cc);
		m_quad2->draw();
	}

	if (m_quad3) {
		cc.m_world = m_quad3->getTransform();
		cc.m_color = m_quad3->getColor();
		m_constant_buffer->update(GraphicsEngine::getInstance()->getImmediateDeviceContext(), &cc);
		m_quad3->draw();
	}

	

	m_swap_chain->present(false);
}

void AppWindow::onDestroy()
{
	//Window::onDestroy();

	rasterizerState->release();

	m_swap_chain->release();
	m_vertex_buffer->release();

	m_quad2->release();;
	m_quad2->release();;
	m_quad3->release();;

	m_vertex_shader->release();
	m_pixel_shader->release();
	GraphicsEngine::getInstance()->release();
}
