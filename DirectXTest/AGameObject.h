#pragma once
#include <string>
#include "Vector3D.h"
#include "Matrix4x4.h"
class Material;
class VertexShader;
class PixelShader;

class AGameObject
{
public:
	AGameObject(std::string name);
	virtual ~AGameObject();

	virtual void update(float deltaTime) = 0;
	virtual void draw(Matrix4x4 projectionMatrix) = 0;

	std::string setName(std::string name);
	std::string getName() const;

	void setPosition(float x, float y, float z);
	void setPosition(Vector3D pos);
	Vector3D getLocalPosition();

	void setScale(float x, float y, float z);
	void setScale(Vector3D scale);
	Vector3D getLocalScale();

	void setRotation(float x, float y, float z);
	void setRotation(Vector3D rot);
	Vector3D getLocalRotation();

	void assignMaterial(Material* material) { m_assignedMaterial = material; }
	Material* getAssignedMaterial();

protected:
	Material* m_assignedMaterial;

	std::string m_name;
	Vector3D m_position = Vector3D(0, 0, 0);
	Vector3D m_scale = Vector3D(1, 1, 1);
	Vector3D m_rotation = Vector3D(0, 0, 0);
	Matrix4x4 m_world_matrix;
};