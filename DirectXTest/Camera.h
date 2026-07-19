#pragma once
#include "AGameObject.h"
#include "InputListener.h"
#include "Matrix4x4.h"
#include <string>

class Camera : public AGameObject, public InputListener
{
public:
	Camera(std::string name);
	~Camera();

	void update(float deltaTime) override;
	virtual void draw(Matrix4x4 projectionMatrix) override;
	void updateViewMatrix();
	Matrix4x4 getViewMatrix();

	Vector3D getForwardVector();
	Vector3D getRightVector();

	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;
	virtual void onMouseWheel(int delta) override;

private:
	Matrix4x4 m_view_matrix;

	bool isRightMouseDown = false;

	Point m_last_mouse_pos;

	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	float m_upward = 0.0f;
	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;

	float camera_speed = 2.0f;
	float move_speed_multiplier = 1.0f;
	float zoom_speed = 1.0f;
};