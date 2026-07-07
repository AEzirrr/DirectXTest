#include "Cube.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include <Windows.h>
#include "SceneCameraHandler.h"

__declspec(align(16)) struct cube_constant {
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_projection;
	float m_time;
	Vector3D m_color;
};

struct cube_vertex {
	Vector3D position;
	Vector3D color;
};

Cube::Cube(std::string name, void* shader_byte_code, size_t size_shader) : AGameObject(name)
{
	//Rainbow
	cube_vertex vertex_list[] =
	{
		//Front
		{Vector3D(-0.5f, -0.5f, -0.5f), Vector3D(1, 0, 0)},
		{Vector3D(-0.5f, 0.5f, -0.5f), Vector3D(0, 1, 0)},
		{Vector3D(0.5f, 0.5f, -0.5f), Vector3D(0, 0, 1)},
		{Vector3D(0.5f, -0.5f, -0.5f), Vector3D(0, 1, 1)},

		//Back
		{Vector3D(0.5f, -0.5f, 0.5f), Vector3D(1, 0, 1)},
		{Vector3D(0.5f, 0.5f, 0.5f), Vector3D(1, 1, 0)},
		{Vector3D(-0.5f, 0.5f, 0.5f), Vector3D(1, 1, 1)},
		{Vector3D(-0.5f, -0.5f, 0.5f), Vector3D(0, 1, 1)},
	};

	/* //White
	cube_vertex vertex_list[] =
	{
		//Front
		{Vector3D(-0.5f, -0.5f, -0.5f), Vector3D(1, 1, 1)},
		{Vector3D(-0.5f, 0.5f, -0.5f), Vector3D(1, 1, 1)},
		{Vector3D(0.5f, 0.5f, -0.5f), Vector3D(1, 1, 1)},
		{Vector3D(0.5f, -0.5f, -0.5f), Vector3D(1, 1, 1)},

		//Back
		{Vector3D(0.5f, -0.5f, 0.5f), Vector3D(1, 1, 1)},
		{Vector3D(0.5f, 0.5f, 0.5f), Vector3D(1, 1, 1)},
		{Vector3D(-0.5f, 0.5f, 0.5f), Vector3D(1, 1, 1)},
		{Vector3D(-0.5f, -0.5f, 0.5f), Vector3D(1, 1, 1)},
	}; */

	unsigned int index_list[] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4, 
		1, 6, 5, 5, 2, 1, 
		7, 0, 3, 3, 4, 7, 
		3, 2, 5, 5, 4, 3,
		7, 6, 1, 1, 0, 7 
	};

	// Initialize Buffers
	m_vertex_buffer = GraphicsEngine::getInstance()->createVertexBuffer();
	m_vertex_buffer->load(vertex_list, sizeof(cube_vertex), ARRAYSIZE(vertex_list), shader_byte_code, size_shader);

	m_index_buffer = GraphicsEngine::getInstance()->createIndexBuffer();
	m_index_buffer->load(index_list, ARRAYSIZE(index_list));

	cube_constant cc;
	cc.m_time = 0;
	m_constant_buffer = GraphicsEngine::getInstance()->createConstantBuffer();
	m_constant_buffer->load(&cc, sizeof(cube_constant));
}

Cube::~Cube()
{
	m_vertex_buffer->release();
	m_index_buffer->release();
	m_constant_buffer->release();
}

void Cube::update(float deltaTime)
{
	Matrix4x4 rotx, roty, rotz, scale_mat, translation_mat;
	cube_constant cc;

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

void Cube::draw(Matrix4x4 projectionMatrix, VertexShader* vertexShader, PixelShader* pixelShader)
{
	cube_constant cc;
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