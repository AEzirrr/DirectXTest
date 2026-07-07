#pragma once
#include "AGameObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

class Plane : public AGameObject
{
public:
	Plane(std::string name, void* shader_byte_code, size_t size_shader);
	~Plane();

	virtual void update(float deltaTime) override;
	virtual void draw(Matrix4x4 projectionMatrix, VertexShader* vertexShader, PixelShader* pixelShader) override;

private:
	VertexBuffer* m_vertex_buffer;
	IndexBuffer* m_index_buffer;
	ConstantBuffer* m_constant_buffer;

	float m_total_time = 0.0f;

};