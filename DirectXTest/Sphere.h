#pragma once
#include "AGameObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

class Sphere : public AGameObject
{
public:
	Sphere(std::string name, void* shader_byte_code, size_t size_shader);
	~Sphere();

	void update(float deltaTime) override;
	void draw(Matrix4x4 projectionMatrix, VertexShader* vertexShader, PixelShader* pixelShader) override;

	void setDirection(const Vector3D& dir) { m_sphere_direction = dir; }
	Vector3D getDirection() const { return m_sphere_direction; }

private:
	VertexBuffer* m_vertex_buffer;
	IndexBuffer* m_index_buffer;
	ConstantBuffer* m_constant_buffer;

	Vector3D m_sphere_direction;

	float m_total_time = 0.0f;
};