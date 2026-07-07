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
#include "Sphere.h"
#include "Plane.h"
#include <chrono>
#include <vector>
#include "Camera.h"

#include "ToolbarScreen.h"

class AppWindow : public Window, public InputListener, public ToolbarListener
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
	void onMouseWheel(int delta) override;

	void onCreateCubeClicked() override;
	void onCreateSphereClicked() override;
	void onCreatePlaneClicked() override;

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
	Cube* m_cube3;

	float cube_rotx;
	float cube_roty;

	std::vector<Cube*> m_cubes;

	std::vector<AGameObject*> m_game_objects;

	//std::vector<Sphere*> m_spheres;

	void* m_shader_byte_code = nullptr;
	size_t m_size_shader = 0;

	Vector3D m_boundsMin;
	Vector3D m_boundsMax;

	Plane* m_plane;

	std::chrono::time_point<std::chrono::steady_clock> m_old_time;
	float deltaTime = 0.0f;

	float m_target_fps = 60.0f;
	float m_target_frame_time = 1.0f / m_target_fps;


	float m_scale_cube = 1.0f;



private:
	Matrix4x4 m_projection_matrix;
};