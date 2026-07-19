#include "Cube.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include <Windows.h>
#include "SceneCameraHandler.h"
#include "Material.h"
#include "ShaderLibrary.h"
#include "ShaderNames.h"

//__declspec(align(16)) struct cube_constant {
//	Matrix4x4 m_world;
//	Matrix4x4 m_view;
//	Matrix4x4 m_projection;
//	float m_time;
//	Vector3D m_color;
//};

__declspec(align(16)) struct TransformConstantData {
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_projection;
	float m_time;
	Vector3D m_padding;
	Vector3D m_cameraWorldPos;
};

//struct MaterialConstantData {
//	Vector4D albedoColor; 
//};

struct cube_vertex {
	Vector3D position;
	Vector2D texCoord;
	Vector3D normal;
};

Cube::Cube(std::string name, void* shader_byte_code, size_t size_shader) : AGameObject(name)
{
	cube_vertex vertex_list[] =
	{
		// === FRONT FACE (Normal: 0, 0, -1) ===
		{ Vector3D(-0.5f, -0.5f, -0.5f), Vector2D(0.0f, 1.0f), Vector3D(0.0f, 0.0f, -1.0f) },
		{ Vector3D(-0.5f,  0.5f, -0.5f), Vector2D(0.0f, 0.0f), Vector3D(0.0f, 0.0f, -1.0f) },
		{ Vector3D(0.5f,  0.5f, -0.5f), Vector2D(1.0f, 0.0f), Vector3D(0.0f, 0.0f, -1.0f) },
		{ Vector3D(0.5f, -0.5f, -0.5f), Vector2D(1.0f, 1.0f), Vector3D(0.0f, 0.0f, -1.0f) },

		// === BACK FACE (Normal: 0, 0, 1) ===
		{ Vector3D(0.5f, -0.5f,  0.5f), Vector2D(0.0f, 1.0f), Vector3D(0.0f, 0.0f, 1.0f) },
		{ Vector3D(0.5f,  0.5f,  0.5f), Vector2D(0.0f, 0.0f), Vector3D(0.0f, 0.0f, 1.0f) },
		{ Vector3D(-0.5f,  0.5f,  0.5f), Vector2D(1.0f, 0.0f), Vector3D(0.0f, 0.0f, 1.0f) },
		{ Vector3D(-0.5f, -0.5f,  0.5f), Vector2D(1.0f, 1.0f), Vector3D(0.0f, 0.0f, 1.0f) },

		// === TOP FACE (Normal: 0, 1, 0) ===
		{ Vector3D(-0.5f,  0.5f, -0.5f), Vector2D(0.0f, 1.0f), Vector3D(0.0f, 1.0f, 0.0f) },
		{ Vector3D(-0.5f,  0.5f,  0.5f), Vector2D(0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f) },
		{ Vector3D(0.5f,  0.5f,  0.5f), Vector2D(1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f) },
		{ Vector3D(0.5f,  0.5f, -0.5f), Vector2D(1.0f, 1.0f), Vector3D(0.0f, 1.0f, 0.0f) },

		// === BOTTOM FACE (Normal: 0, -1, 0) ===
		{ Vector3D(-0.5f, -0.5f,  0.5f), Vector2D(0.0f, 1.0f), Vector3D(0.0f, -1.0f, 0.0f) },
		{ Vector3D(-0.5f, -0.5f, -0.5f), Vector2D(0.0f, 0.0f), Vector3D(0.0f, -1.0f, 0.0f) },
		{ Vector3D(0.5f, -0.5f, -0.5f), Vector2D(1.0f, 0.0f), Vector3D(0.0f, -1.0f, 0.0f) },
		{ Vector3D(0.5f, -0.5f,  0.5f), Vector2D(1.0f, 1.0f), Vector3D(0.0f, -1.0f, 0.0f) },

		// === RIGHT FACE (Normal: 1, 0, 0) ===
		{ Vector3D(0.5f, -0.5f, -0.5f), Vector2D(0.0f, 1.0f), Vector3D(1.0f, 0.0f, 0.0f) },
		{ Vector3D(0.5f,  0.5f, -0.5f), Vector2D(0.0f, 0.0f), Vector3D(1.0f, 0.0f, 0.0f) },
		{ Vector3D(0.5f,  0.5f,  0.5f), Vector2D(1.0f, 0.0f), Vector3D(1.0f, 0.0f, 0.0f) },
		{ Vector3D(0.5f, -0.5f,  0.5f), Vector2D(1.0f, 1.0f), Vector3D(1.0f, 0.0f, 0.0f) },

		// === LEFT FACE (Normal: -1, 0, 0) ===
		{ Vector3D(-0.5f, -0.5f,  0.5f), Vector2D(0.0f, 1.0f), Vector3D(-1.0f, 0.0f, 0.0f) },
		{ Vector3D(-0.5f,  0.5f,  0.5f), Vector2D(0.0f, 0.0f), Vector3D(-1.0f, 0.0f, 0.0f) },
		{ Vector3D(-0.5f,  0.5f, -0.5f), Vector2D(1.0f, 0.0f), Vector3D(-1.0f, 0.0f, 0.0f) },
		{ Vector3D(-0.5f, -0.5f, -0.5f), Vector2D(1.0f, 1.0f), Vector3D(-1.0f, 0.0f, 0.0f) }
	};



	unsigned int index_list[] = {
		// Front
		0, 1, 2,  2, 3, 0,
		// Back
		4, 5, 6,  6, 7, 4,
		// Top
		8, 9, 10, 10, 11, 8,
		// Bottom
		12, 13, 14, 14, 15, 12,
		// Right
		16, 17, 18, 18, 19, 16,
		// Left
		20, 21, 22, 22, 23, 20
	};

	// Initialize Buffers
	m_vertex_buffer = GraphicsEngine::getInstance()->createVertexBuffer();
	m_vertex_buffer->load(vertex_list, sizeof(cube_vertex), ARRAYSIZE(vertex_list), shader_byte_code, size_shader);

	m_index_buffer = GraphicsEngine::getInstance()->createIndexBuffer();
	m_index_buffer->load(index_list, ARRAYSIZE(index_list));

	TransformConstantData tcd;
	tcd.m_time = 0;
	tcd.m_padding = Vector3D(0, 0, 0);
	tcd.m_cameraWorldPos = Vector3D(0, 0, 0);
	m_constant_buffer = GraphicsEngine::getInstance()->createConstantBuffer();
	m_constant_buffer->load(&tcd, sizeof(TransformConstantData));

	// Create default material
	m_assignedMaterial = new Material(name + "_Material");
	if (m_assignedMaterial) {
		m_assignedMaterial->setAlbedoColor(Vector4D(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

Cube::~Cube()
{
	if (m_vertex_buffer) {
		m_vertex_buffer->release();
		m_vertex_buffer = nullptr;
	}
	if (m_index_buffer) {
		m_index_buffer->release();
		m_index_buffer = nullptr;
	}
	if (m_constant_buffer) {
		m_constant_buffer->release();
		m_constant_buffer = nullptr;
	}
}

void Cube::update(float deltaTime)
{
	Matrix4x4 rotx, roty, rotz, scale_mat, translation_mat;
	TransformConstantData tcd;

	m_total_time += deltaTime;
	// tcd.m_time = m_total_time;

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

void Cube::draw(Matrix4x4 projectionMatrix)
{
	auto deviceContext = GraphicsEngine::getInstance()->getImmediateDeviceContext();
	if (!deviceContext) return;

	ShaderNames shaderNames;
	VertexShader* vertexShader = ShaderLibrary::getInstance()->getVertexShader(shaderNames.BASE_VERTEX_SHADER_NAME);
	PixelShader* pixelShader = ShaderLibrary::getInstance()->getPixelShader(shaderNames.BASE_PIXEL_SHADER_NAME);

	TransformConstantData tcd;
	tcd.m_world = m_world_matrix;
	tcd.m_view = SceneCameraHandler::getInstance()->getSceneCameraViewMatrix();
	tcd.m_projection = projectionMatrix;
	tcd.m_time = m_total_time;
	tcd.m_padding = Vector3D(0, 0, 0);

	auto camera = SceneCameraHandler::getInstance()->getCamera();
	tcd.m_cameraWorldPos = camera ? camera->getLocalPosition() : Vector3D(0, 0, 0);

	m_constant_buffer->update(deviceContext, &tcd);
	deviceContext->setConstantBuffer(vertexShader, m_constant_buffer);
	deviceContext->setConstantBuffer(pixelShader, m_constant_buffer);

	if (m_assignedMaterial) {
		m_assignedMaterial->bind(deviceContext, pixelShader);
	}

	deviceContext->setVertexShader(vertexShader);
	deviceContext->setPixelShader(pixelShader);
	deviceContext->setIndexBuffer(m_index_buffer);
	deviceContext->setVertexBuffer(m_vertex_buffer);

	deviceContext->drawIndexedTriangleList(m_index_buffer->getSizeIndexList(), 0, 0);
}