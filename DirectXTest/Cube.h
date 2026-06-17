#pragma once
#include "AGameObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

class Cube : public AGameObject
{
public:
	Cube(std::string name, void* shader_byte_code, size_t size_shader);
	~Cube();

	virtual void update(float deltaTime) override;
	virtual void draw(Matrix4x4 viewMatrix, Matrix4x4 projectionMatrix, VertexShader* vertexShader, PixelShader* pixelShader) override;

private:
	VertexBuffer* m_vertex_buffer;
	IndexBuffer* m_index_buffer;
	ConstantBuffer* m_constant_buffer;
};