#include "AppWindow.h"
#include <Windows.h>
#include "Matrix4x4.h"
#include "InputSystem.h"

__declspec(align(16)) struct constant {
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_projection;
	unsigned int m_time;
	Vector3D m_color;
};

struct vertex {
	Vector3D position;
	Vector3D color;
};

AppWindow::AppWindow()
{}

AppWindow::~AppWindow()
{}

void AppWindow::update()
{}

void AppWindow::onCreate()
{
	InputSystem::getInstance()->addListener(this);
	InputSystem::getInstance()->showCursor(false);

	GraphicsEngine::getInstance()->init();
	m_swap_chain = GraphicsEngine::getInstance()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	rasterizerState = new RasterizerState();
	rasterizerState->InitializeWireframe(GraphicsEngine::getInstance()->getDevice());

	////////// VERTEX SHADER INITIALIZATION //////////
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::getInstance()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vertex_shader = GraphicsEngine::getInstance()->createVertexShader(shader_byte_code, size_shader);
	////////// VERTEX SHADER INITIALIZATION //////////

	m_cube1 = new Cube("MainCube", shader_byte_code, size_shader);
	m_cube2 = new Cube("SecondaryCube", shader_byte_code, size_shader);
	m_plane = new Plane("GroundPlane", shader_byte_code, size_shader);

	m_cube1->setPosition(-1.0f, 0.0f, 0.0f);
	m_cube2->setPosition(1.0f, 0.0f, 0.0f);
	m_plane->setPosition(0.0f, 0.0f, 0.0f);

	m_cube1->setScale(0.5f, 0.5f, 0.5f);
	m_cube2->setScale(0.5f, 0.5f, 0.5f);
	m_plane->setScale(5.0f, 1.0f, 5.0f);

	GraphicsEngine::getInstance()->releaseCompiledShader();

	////////// PIXEL SHADER INITIALIZATION //////////
	GraphicsEngine::getInstance()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_pixel_shader = GraphicsEngine::getInstance()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::getInstance()->releaseCompiledShader();
	///////// PIXEL SHADER INITIALIZATION //////////

	constant cc;
	cc.m_view = m_view_matrix;
	cc.m_projection = m_projection_matrix;
	cc.m_time = 0;

	m_constant_buffer = GraphicsEngine::getInstance()->createConstantBuffer();
	m_constant_buffer->load(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	InputSystem::getInstance()->update();

	constant cc;

	// Calculate Delta Time
	current_time = GetTickCount();
	deltaTime = (m_old_time > 0) ? (current_time - m_old_time) / 1000.0f : 0.0f;
	m_old_time = current_time;

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, .3f, 0.3f, 0.3f, 1.0f);

	RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setViewportSize(width, height);

	rot_x += m_key_rot_speed_x * deltaTime;
	rot_y += m_key_rot_speed_y * deltaTime;

	Matrix4x4 worldCamera;
	Matrix4x4 temp;

	worldCamera.setIdentity();

	temp.setIdentity();
	temp.setRotationX(rot_x);
	worldCamera *= temp;

	temp.setIdentity();
	temp.setRotationY(rot_y);
	worldCamera *= temp;

	Vector3D forward = worldCamera.getZDirection();
	Vector3D right = worldCamera.getXDirection();

	m_cam_pos = m_cam_pos + (forward * (m_key_move_z * deltaTime));
	m_cam_pos = m_cam_pos + (right * (m_key_move_x * deltaTime));

	temp.setIdentity();
	temp.setTranslation(m_cam_pos);
	worldCamera *= temp;

	worldCamera.inverse();
	m_view_matrix = worldCamera;
	cc.m_view = m_view_matrix;

	cc.m_projection.setPerspectiveFovLH(1.57f, (float)width / (float)height, 0.1f, 1000.0f);

	if (m_cube1)
	{
		m_cube1->update(deltaTime);
		m_cube2->update(deltaTime);

		m_cube1->draw(m_view_matrix, cc.m_projection, m_vertex_shader, m_pixel_shader);
		m_cube2->draw(m_view_matrix, cc.m_projection, m_vertex_shader, m_pixel_shader);
	}

	if (m_plane) {
		m_plane->update(deltaTime);
		m_plane->draw(m_view_matrix, cc.m_projection, m_vertex_shader, m_pixel_shader);
	}

	m_swap_chain->present(false);
}

void AppWindow::onDestroy()
{
	rasterizerState->release();

	m_swap_chain->release();

	if (m_vertex_buffer) m_vertex_buffer->release();
	if (m_index_buffer) m_index_buffer->release();

	if (m_cube1) {
		delete m_cube1;
		m_cube1 = nullptr;
	}

	if (m_cube2) {
		delete m_cube2;
		m_cube2 = nullptr;
	}

	if (m_plane) {
		delete m_plane;
		m_plane = nullptr;
	}

	m_vertex_shader->release();
	m_pixel_shader->release();
	GraphicsEngine::getInstance()->release();
}

void AppWindow::onFocus()
{
	InputSystem::getInstance()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::getInstance()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W') { m_key_move_z = 2.0f; }
	if (key == 'S') { m_key_move_z = -2.0f; }
	if (key == 'A') { m_key_move_x = -2.0f; }
	if (key == 'D') { m_key_move_x = 2.0f; }

	if (key == 'Q') {
		GraphicsEngine::getInstance()->getImmediateDeviceContext()->setRasterizerState(rasterizerState);
	}
	if (key == 'E') {
		GraphicsEngine::getInstance()->getImmediateDeviceContext()->clearRasterizerState(rasterizerState);
	}
}

void AppWindow::onKeyUp(int key)
{
	if (key == 'W' || key == 'S') { m_key_move_z = 0.0f; }
	if (key == 'A' || key == 'D') { m_key_move_x = 0.0f; }
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	int center_x = width / 2;
	int center_y = height / 2;

	POINT center_screen = { center_x, center_y };
	::ClientToScreen(this->m_hwnd, &center_screen);

	float delta_x = mouse_pos.m_x - center_screen.x;
	float delta_y = mouse_pos.m_y - center_screen.y;

	if (delta_x == 0.0f && delta_y == 0.0f) return;

	const float MOUSE_SENSITIVITY = 0.005f;

	rot_x -= delta_y * MOUSE_SENSITIVITY;
	rot_y -= delta_x * MOUSE_SENSITIVITY;

	if (rot_x > 1.57f) rot_x = 1.57f;
	if (rot_x < -1.57f) rot_x = -1.57f;

	InputSystem::getInstance()->setCursorPosition(Point(center_screen.x, center_screen.y));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	m_scale_cube += 0.1f;
	m_cube1->setScale(m_scale_cube, m_scale_cube, m_scale_cube);
	m_cube2->setScale(m_scale_cube, m_scale_cube, m_scale_cube);
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_scale_cube -= 0.1f;
	m_cube1->setScale(m_scale_cube, m_scale_cube, m_scale_cube);
	m_cube2->setScale(m_scale_cube, m_scale_cube, m_scale_cube);
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{}