#include "Cylinder.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include <Windows.h>
#include <vector>
#include <cmath>
#include "SceneCameraHandler.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Material.h"
#include "ShaderLibrary.h"
#include "ShaderNames.h"

__declspec(align(16)) struct TransformConstantData {
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_projection;
	float m_time;
	Vector3D m_padding;
	Vector3D m_cameraWorldPos;
};

struct cylinder_vertex {
	Vector3D position;
	Vector2D texCoord;
	Vector3D normal;
};

Cylinder::Cylinder(std::string name, void* shader_byte_code, size_t size_shader) : AGameObject(name)
{
	std::vector<cylinder_vertex> vertices;
	std::vector<unsigned int> indices;

	int slices = 36;
	float radius = 0.5f;
	float height = 1.0f;
	const float PI = 3.14159265f;

	// === SIDE WALLS ===
	int sideStartIndex = 0;
	for (int i = 0; i <= slices; i++) {
		float U = i / (float)slices;
		float theta = U * (PI * 2.0f);

		float x = radius * cos(theta);
		float z = radius * sin(theta);

		// Normal for side (pointing outward)
		Vector3D normal = Vector3D(cos(theta), 0.0f, sin(theta));

		// Top vertex of side
		vertices.push_back({ Vector3D(x, height * 0.5f, z), Vector2D(U, 1.0f), normal });
		// Bottom vertex of side
		vertices.push_back({ Vector3D(x, -height * 0.5f, z), Vector2D(U, 0.0f), normal });
	}

	// Side indices (counter-clockwise winding when viewed from outside)
	for (int i = 0; i < slices; i++) {
		int topLeft = sideStartIndex + (i * 2);
		int bottomLeft = sideStartIndex + (i * 2) + 1;
		int topRight = sideStartIndex + ((i + 1) * 2);
		int bottomRight = sideStartIndex + ((i + 1) * 2) + 1;

		// Triangle 1: topLeft -> topRight -> bottomLeft (counter-clockwise)
		indices.push_back(topLeft);
		indices.push_back(topRight);
		indices.push_back(bottomLeft);

		// Triangle 2: topRight -> bottomRight -> bottomLeft (counter-clockwise)
		indices.push_back(topRight);
		indices.push_back(bottomRight);
		indices.push_back(bottomLeft);
	}

	// === TOP CAP ===
	int topCenterIndex = vertices.size();
	vertices.push_back({ Vector3D(0.0f, height * 0.5f, 0.0f), Vector2D(0.5f, 0.5f), Vector3D(0.0f, 1.0f, 0.0f) });

	int topRingStart = vertices.size();
	for (int i = 0; i <= slices; i++) {
		float U = i / (float)slices;
		float theta = U * (PI * 2.0f);

		float x = radius * cos(theta);
		float z = radius * sin(theta);

		float u = 0.5f + 0.5f * cos(theta);
		float v = 0.5f + 0.5f * sin(theta);

		vertices.push_back({ Vector3D(x, height * 0.5f, z), Vector2D(u, v), Vector3D(0.0f, 1.0f, 0.0f) });
	}

	// Top cap indices
	for (int i = 0; i < slices; i++) {
		indices.push_back(topCenterIndex);
		indices.push_back(topRingStart + i + 1);
		indices.push_back(topRingStart + i);
	}

	// === BOTTOM CAP ===
	int bottomCenterIndex = vertices.size();
	vertices.push_back({ Vector3D(0.0f, -height * 0.5f, 0.0f), Vector2D(0.5f, 0.5f), Vector3D(0.0f, -1.0f, 0.0f) });

	int bottomRingStart = vertices.size();
	for (int i = 0; i <= slices; i++) {
		float U = i / (float)slices;
		float theta = U * (PI * 2.0f);

		float x = radius * cos(theta);
		float z = radius * sin(theta);

		float u = 0.5f + 0.5f * cos(theta);
		float v = 0.5f + 0.5f * sin(theta);

		vertices.push_back({ Vector3D(x, -height * 0.5f, z), Vector2D(u, v), Vector3D(0.0f, -1.0f, 0.0f) });
	}

	for (int i = 0; i < slices; i++) {
		indices.push_back(bottomCenterIndex);
		indices.push_back(bottomRingStart + i);
		indices.push_back(bottomRingStart + i + 1);
	}

	// Create vertex buffer
	m_vertex_buffer = GraphicsEngine::getInstance()->createVertexBuffer();
	if (m_vertex_buffer) {
		m_vertex_buffer->load(vertices.data(), sizeof(cylinder_vertex), vertices.size(), shader_byte_code, size_shader);
	}

	// Create index buffer
	m_index_buffer = GraphicsEngine::getInstance()->createIndexBuffer();
	if (m_index_buffer) {
		m_index_buffer->load(indices.data(), indices.size());
	}

	// Create constant buffer
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
		m_assignedMaterial->setAlbedoColor(Vector4D(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

Cylinder::~Cylinder()
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

void Cylinder::update(float deltaTime)
{
	Matrix4x4 rotx, roty, rotz, scale_mat, translation_mat;

	m_total_time += deltaTime;

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

void Cylinder::draw(Matrix4x4 projectionMatrix)
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