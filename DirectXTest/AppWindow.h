#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "RasterizerState.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "Matrix4x4.h"

#include "Cube.h"
#include "Plane.h"

class AppWindow : public Window, public InputListener
{
public:
	AppWindow();
	~AppWindow();

	void update();

	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;

	void onKeyDown(int key) override;
	void onKeyUp(int key) override;
	void onMouseMove(const Point& mouse_pos) override;
	void onLeftMouseDown(const Point& mouse_pos) override;
	void onLeftMouseUp(const Point& mouse_pos) override;
	void onRightMouseDown(const Point& mouse_pos) override;
	void onRightMouseUp(const Point& mouse_pos) override;

private:
	RasterizerState* rasterizerState;

	SwapChain* m_swap_chain;
	VertexBuffer* m_vertex_buffer;
	IndexBuffer* m_index_buffer;

	VertexShader* m_vertex_shader;
	PixelShader* m_pixel_shader;

	ConstantBuffer* m_constant_buffer;

	Cube* m_cube1;
	Cube* m_cube2;
	Plane* m_plane;

	unsigned long m_old_time = 0;
	unsigned long current_time = 0;
	float deltaTime = 0.0f;

	float rot_x = 0.0f;
	float rot_y = 0.0f;

	float m_key_move_x = 0.0f;
	float m_key_move_y = 0.0f;
	float m_key_move_z = 0.0f;

	float m_key_rot_speed_x = 0.0f;
	float m_key_rot_speed_y = 0.0f;

	float m_scale_cube = 1.0f;

private:
	Matrix4x4 m_view_matrix;
	Matrix4x4 m_projection_matrix;

	Vector3D m_cam_pos;
};