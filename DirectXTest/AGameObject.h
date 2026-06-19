#pragma once
#include <string>
#include "Vector3D.h"
#include "Matrix4x4.h"

class VertexShader;
class PixelShader;

class AGameObject
{
public:
	AGameObject(std::string name);
	virtual ~AGameObject();

	virtual void update(float deltaTime) = 0;
	virtual void draw(Matrix4x4 viewMatrix, Matrix4x4 projectionMatrix, VertexShader* vertexShader, PixelShader* pixelShader) = 0;

	void setPosition(float x, float y, float z);
	void setPosition(Vector3D pos);
	Vector3D getLocalPosition();

	void setScale(float x, float y, float z);
	void setScale(Vector3D scale);
	Vector3D getLocalScale();

	void setRotation(float x, float y, float z);
	void setRotation(Vector3D rot);
	Vector3D getLocalRotation();

protected:
	std::string m_name;
	Vector3D m_position = Vector3D(0, 0, 0);
	Vector3D m_scale = Vector3D(1, 1, 1);
	Vector3D m_rotation = Vector3D(0, 0, 0);
	Matrix4x4 m_world_matrix;
};