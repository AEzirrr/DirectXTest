#include "Camera.h"
#include "InputSystem.h"
#include <Windows.h>
#include <iostream>

Camera::Camera(std::string name) : AGameObject(name)
{
	this->setPosition(Vector3D(0.0f, 1.0f, -5.0f));
	m_rot_x = 0.0f;
	m_rot_y = 0.0f;
	this->setRotation(Vector3D(m_rot_x, m_rot_y, 0.0f));

	m_view_matrix.setIdentity();
}

Camera::~Camera()
{}

void Camera::update(float deltaTime)
{
	Vector3D currentPos = this->getLocalPosition();

	if (isRightMouseDown) {
		currentPos = currentPos + (getForwardVector() * (m_forward * deltaTime));
		currentPos = currentPos + (getRightVector() * (m_rightward * deltaTime));
		currentPos.y += (m_upward * deltaTime);
	}

	this->setPosition(currentPos);
	this->setRotation(Vector3D(m_rot_x, m_rot_y, 0.0f));

	updateViewMatrix();
}

void Camera::draw(Matrix4x4 projectionMatrix)
{
	
}

void Camera::updateViewMatrix()
{
	Matrix4x4 worldCam;
	worldCam.setIdentity();

	Matrix4x4 temp;
	temp.setIdentity();

	Vector3D localRot = this->getLocalRotation();

	temp.setRotationX(localRot.x);
	worldCam *= temp;

	temp.setIdentity();
	temp.setRotationY(localRot.y);
	worldCam *= temp;

	temp.setIdentity();
	temp.setTranslation(this->getLocalPosition());
	worldCam *= temp;

	worldCam.inverse();
	m_view_matrix = worldCam;
}

Matrix4x4 Camera::getViewMatrix()
{
	return m_view_matrix;
}

Vector3D Camera::getForwardVector()
{
	Matrix4x4 worldCam;
	worldCam.setIdentity();

	Matrix4x4 temp;
	temp.setIdentity();

	temp.setRotationX(m_rot_x);
	worldCam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	worldCam *= temp;

	Vector3D forward = worldCam.getZDirection();

	return forward;
}

Vector3D Camera::getRightVector()
{
	Matrix4x4 worldCam;
	worldCam.setIdentity();

	Matrix4x4 temp;
	temp.setIdentity();

	temp.setRotationX(m_rot_x);
	worldCam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	worldCam *= temp;

	Vector3D right = worldCam.getXDirection();

	return right;
}

void Camera::onKeyDown(int key)
{

	if (key == 'W') { m_forward = camera_speed * move_speed_multiplier; }
	if (key == 'S') { m_forward = -camera_speed * move_speed_multiplier; }
	if (key == 'A') { m_rightward = -camera_speed * move_speed_multiplier; }
	if (key == 'D') { m_rightward = camera_speed * move_speed_multiplier; }
	if (key == 'Q') { m_upward = -camera_speed * move_speed_multiplier; }
	if (key == 'E') { m_upward = camera_speed * move_speed_multiplier; }

	if (key ==VK_SHIFT) {
		move_speed_multiplier = 2.0f;
		zoom_speed = 2.0f;
	}

}

void Camera::onKeyUp(int key)
{
	if (key == 'W' || key == 'S') { m_forward = 0.0f; }
	if (key == 'A' || key == 'D') { m_rightward = 0.0f; }
	if (key == 'Q' || key == 'E') { m_upward = 0.0f; }

	if (key == VK_SHIFT) {
		move_speed_multiplier = 1.0f;
		zoom_speed = 1.0f;
	}
}

void Camera::onMouseMove(const Point& mouse_pos)
{
	if (isRightMouseDown) {
		float delta_x = mouse_pos.m_x - m_last_mouse_pos.m_x;
		float delta_y = mouse_pos.m_y - m_last_mouse_pos.m_y;

		if (delta_x == 0.0f && delta_y == 0.0f) return;

		const float MOUSE_SENSITIVITY = 0.005f;

		m_rot_x -= delta_y * MOUSE_SENSITIVITY;
		m_rot_y -= delta_x * MOUSE_SENSITIVITY;

		if (m_rot_x > 1.57f) m_rot_x = 1.57f;
		if (m_rot_x < -1.57f) m_rot_x = -1.57f;

		m_last_mouse_pos = mouse_pos;

	}
}

void Camera::onLeftMouseDown(const Point& mouse_pos) {}
void Camera::onLeftMouseUp(const Point& mouse_pos) {}
void Camera::onRightMouseDown(const Point& mouse_pos) {
	isRightMouseDown = true;

	m_last_mouse_pos = mouse_pos;
}
void Camera::onRightMouseUp(const Point& mouse_pos) {
	isRightMouseDown = false;
}

void Camera::onMouseWheel(int delta) {

	Vector3D currentPos = this->getLocalPosition();

	if (isRightMouseDown) {
		if (delta > 0) {
			camera_speed += 0.1f;
		}
		else if (delta < 0) {
			camera_speed -= 0.1f;
		}

		std::cout << "Camera speed: " << camera_speed << std::endl;
	}
	else {
		if (delta > 0) {
			currentPos = currentPos + (getForwardVector() * zoom_speed);
		}
		else if (delta < 0) {
			currentPos = currentPos - (getForwardVector() * zoom_speed);
		}

		this->setPosition(currentPos);
	}

}

