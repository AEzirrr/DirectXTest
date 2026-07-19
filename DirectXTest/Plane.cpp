#include "Plane.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include <Windows.h>
#include "SceneCameraHandler.h"
#include "Material.h"
#include "Vector2D.h"
#include "ShaderNames.h"
#include "ShaderLibrary.h"

__declspec(align(16)) struct TransformConstantData {
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_projection;
	float m_time;
	Vector3D m_padding;
	Vector3D m_cameraWorldPos;
};

struct plane_vertex {
	Vector3D position;
	Vector2D texCoord;
	Vector3D normal;
};

Plane::Plane(std::string name, void* shader_byte_code, size_t size_shader) : AGameObject(name)
{
	plane_vertex vertex_list[] =
	{
		// Position, TexCoord, Normal (pointing up)
		{ Vector3D(-0.5f, 0.0f, -0.5f), Vector2D(0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f) },
		{ Vector3D(-0.5f, 0.0f,  0.5f), Vector2D(0.0f, 1.0f), Vector3D(0.0f, 1.0f, 0.0f) },
		{ Vector3D(0.5f, 0.0f,  0.5f), Vector2D(1.0f, 1.0f), Vector3D(0.0f, 1.0f, 0.0f) },
		{ Vector3D(0.5f, 0.0f, -0.5f), Vector2D(1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f) }
	};

	unsigned int index_list[] = {
		0, 1, 2,
		2, 3, 0
	};

	m_vertex_buffer = GraphicsEngine::getInstance()->createVertexBuffer();
	if (m_vertex_buffer) {
		m_vertex_buffer->load(vertex_list, sizeof(plane_vertex), ARRAYSIZE(vertex_list), shader_byte_code, size_shader);
	}

	m_index_buffer = GraphicsEngine::getInstance()->createIndexBuffer();
	if (m_index_buffer) {
		m_index_buffer->load(index_list, ARRAYSIZE(index_list));
	}

	TransformConstantData tcd;
	tcd.m_time = 0;
	tcd.m_padding = Vector3D(0, 0, 0);
	tcd.m_cameraWorldPos = Vector3D(0, 0, 0);
	m_constant_buffer = GraphicsEngine::getInstance()->createConstantBuffer();
	if (m_constant_buffer) {
		m_constant_buffer->load(&tcd, sizeof(TransformConstantData));
	}

	// Create default material
	m_assignedMaterial = new Material(name + "_Material");
	if (m_assignedMaterial) {
		m_assignedMaterial->setAlbedoColor(Vector4D(0.5f, 0.5f, 0.5f, 1.0f)); // Gray default
	}
}

Plane::~Plane()
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

void Plane::update(float deltaTime)
{
	Matrix4x4 rotx, roty, rotz, scale_mat, translation_mat;

	m_total_time += deltaTime;

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

void Plane::draw(Matrix4x4 projectionMatrix)
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