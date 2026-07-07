#include "Sphere.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include <Windows.h>
#include <vector>
#include <cmath>
#include "SceneCameraHandler.h"

__declspec(align(16)) struct sphere_constant {
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_projection;
	unsigned int m_time;
	Vector3D m_color;
};

struct sphere_vertex {
	Vector3D position;
	Vector3D color;
};

Sphere::Sphere(std::string name, void* shader_byte_code, size_t size_shader) : AGameObject(name)
{
	std::vector<sphere_vertex> vertices;
	std::vector<unsigned int> indices;

	int stacks = 20;
	int slices = 20;
	float radius = 0.5f;

	const float PI = 3.14159265f;

	for (int i = 0; i <= stacks; ++i) {
		float V = i / (float)stacks; 
		float phi = V * PI;     

		for (int j = 0; j <= slices; ++j) {
			float U = j / (float)slices; 
			float theta = U * (PI * 2.0f); 

			float x = radius * sin(phi) * cos(theta);
			float y = radius * cos(phi);
			float z = radius * sin(phi) * sin(theta);

			Vector3D color = Vector3D(U, V, 1.0f);

			vertices.push_back({ Vector3D(x, y, z), color });
		}
	}

	for (int i = 0; i < stacks; ++i) {
		for (int j = 0; j < slices; ++j) {

			int first = (i * (slices + 1)) + j;
			int second = first + slices + 1;

			indices.push_back(first);
			indices.push_back(first + 1);
			indices.push_back(second);

			indices.push_back(second);
			indices.push_back(first + 1);
			indices.push_back(second + 1);
		}
	}

	m_vertex_buffer = GraphicsEngine::getInstance()->createVertexBuffer();
	m_vertex_buffer->load(vertices.data(), sizeof(sphere_vertex), vertices.size(), shader_byte_code, size_shader);

	m_index_buffer = GraphicsEngine::getInstance()->createIndexBuffer();
	m_index_buffer->load(indices.data(), indices.size());

	sphere_constant cc;
	cc.m_time = 0;
	m_constant_buffer = GraphicsEngine::getInstance()->createConstantBuffer();
	m_constant_buffer->load(&cc, sizeof(sphere_constant));
}

Sphere::~Sphere()
{
	m_vertex_buffer->release();
	m_index_buffer->release();
	m_constant_buffer->release();
}

void Sphere::update(float deltaTime)
{
	Matrix4x4 rotx, roty, rotz, scale_mat, translation_mat;
	sphere_constant cc;

	m_total_time += deltaTime;

	cc.m_time = m_total_time;

	rotx.setRotationX(m_rotation.x);
	roty.setRotationY(m_rotation.y);
	rotz.setRotationZ(m_rotation.z);
	scale_mat.setScale(m_scale);
	translation_mat.setTranslation(m_position);

	m_world_matrix.setIdentity();

	m_world_matrix *= scale_mat;
	m_world_matrix *= rotx;
	m_world_matrix *= roty;
	m_world_matrix *= rotz;
	m_world_matrix *= translation_mat;
}

void Sphere::draw(Matrix4x4 projectionMatrix, VertexShader* vertexShader, PixelShader* pixelShader)
{
	sphere_constant cc;
	cc.m_world = m_world_matrix;
	cc.m_view = SceneCameraHandler::getInstance()->getSceneCameraViewMatrix();
	cc.m_projection = projectionMatrix;
	cc.m_color = Vector3D(1.0f, 1.0f, 1.0f);

	auto deviceContext = GraphicsEngine::getInstance()->getImmediateDeviceContext();

	m_constant_buffer->update(deviceContext, &cc);

	deviceContext->setConstantBuffer(vertexShader, m_constant_buffer);
	deviceContext->setConstantBuffer(pixelShader, m_constant_buffer);

	deviceContext->setVertexShader(vertexShader);
	deviceContext->setPixelShader(pixelShader);

	deviceContext->setIndexBuffer(m_index_buffer);
	deviceContext->setVertexBuffer(m_vertex_buffer);

	deviceContext->drawIndexedTriangleList(m_index_buffer->getSizeIndexList(), 0, 0);
}