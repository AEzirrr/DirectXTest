#include "Sphere.h"
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

struct sphere_vertex {
    Vector3D position;
    Vector2D texCoord;
    Vector3D normal;
};

Sphere::Sphere(std::string name, void* shader_byte_code, size_t size_shader) : AGameObject(name)
{
    std::vector<sphere_vertex> vertices;
    std::vector<unsigned int> indices;

    int stacks = 30;
    int slices = 30;
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

            Vector3D normal = Vector3D(x, y, z);
            float length = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
            if (length > 0.0f) {
                normal.x /= length;
                normal.y /= length;
                normal.z /= length;
            }

            Vector2D texCoord = Vector2D(U, V);

            vertices.push_back({ Vector3D(x, y, z), texCoord, normal });
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
    if (m_vertex_buffer) {
        m_vertex_buffer->load(vertices.data(), sizeof(sphere_vertex), vertices.size(), shader_byte_code, size_shader);
    }

    m_index_buffer = GraphicsEngine::getInstance()->createIndexBuffer();
    if (m_index_buffer) {
        m_index_buffer->load(indices.data(), indices.size());
    }

    TransformConstantData tcd;
    tcd.m_time = 0;
    tcd.m_padding = Vector3D(0, 0, 0);
    tcd.m_cameraWorldPos = Vector3D(0, 0, 0);

    m_constant_buffer = GraphicsEngine::getInstance()->createConstantBuffer();
    if (m_constant_buffer) {
        if (!m_constant_buffer->load(&tcd, sizeof(TransformConstantData))) {
            OutputDebugStringA("Failed to load constant buffer for Sphere\n");
        }
    }
    else {
        OutputDebugStringA("Failed to create constant buffer for Sphere\n");
    }

    m_assignedMaterial = new Material(name + "_Material");
    if (m_assignedMaterial) {
        m_assignedMaterial->setAlbedoColor(Vector4D(1.0f, 1.0f, 1.0f, 1.0f));
    }
}

Sphere::~Sphere()
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

void Sphere::update(float deltaTime)
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

void Sphere::draw(Matrix4x4 projectionMatrix)
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