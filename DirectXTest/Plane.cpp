#include "Plane.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include <Windows.h>
#include "SceneCameraHandler.h"

__declspec(align(16)) struct plane_constant {
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_projection;
	unsigned int m_time;
	Vector3D m_color;
};

struct plane_vertex {
	Vector3D position;
	Vector3D color;
};

Plane::Plane(std::string name, void* shader_byte_code, size_t size_shader) : AGameObject(name)
{
	plane_vertex vertex_list[] =
	{
		{Vector3D(-0.5f, 0.0f, -0.5f), Vector3D(0.18f, 0.18f, 0.18f)},
		{Vector3D(-0.5f, 0.0f,  0.5f), Vector3D(0.18f, 0.18f, 0.18f)}, 
		{Vector3D(0.5f, 0.0f,  0.5f), Vector3D(0.18f, 0.18f, 0.18f)}, 
		{Vector3D(0.5f, 0.0f, -0.5f), Vector3D(0.18f, 0.18f, 0.18f)}  
	};

	unsigned int index_list[] = {
		0, 1, 2,
		2, 3, 0 
	};

	m_vertex_buffer = GraphicsEngine::getInstance()->createVertexBuffer();
	m_vertex_buffer->load(vertex_list, sizeof(plane_vertex), ARRAYSIZE(vertex_list), shader_byte_code, size_shader);

	m_index_buffer = GraphicsEngine::getInstance()->createIndexBuffer();
	m_index_buffer->load(index_list, ARRAYSIZE(index_list));

	plane_constant cc;
	cc.m_time = 0;
	m_constant_buffer = GraphicsEngine::getInstance()->createConstantBuffer();
	m_constant_buffer->load(&cc, sizeof(plane_constant));
}

Plane::~Plane()
{
	m_vertex_buffer->release();
	m_index_buffer->release();
	m_constant_buffer->release();
}

void Plane::update(float deltaTime)
{
	Matrix4x4 rotx, roty, rotz, scale_mat, translation_mat;
	plane_constant cc;

	m_total_time += deltaTime;

	cc.m_time = m_total_time;

	rotx.setRotationX(m_rotation.x);
	roty.setRotationY(m_rotation.y);
	rotz.setRotationZ(m_rotation.z);
	scale_mat.setScale(m_scale);
	translation_mat.setTranslation(m_position);

	m_world_matrix.setIdentity();

	// CORRECT ORDER: Scale -> Rotate -> Translate
	m_world_matrix *= scale_mat;
	m_world_matrix *= rotx;
	m_world_matrix *= roty;
	m_world_matrix *= rotz;
	m_world_matrix *= translation_mat;
}

void Plane::draw(Matrix4x4 projectionMatrix, VertexShader* vertexShader, PixelShader* pixelShader)
{
	plane_constant cc;
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