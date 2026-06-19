#include "Quad.h"
#include "DeviceContext.h"
#include <cmath>

Quad::Quad(void* shader_byte_code, UINT size_shader) : m_position(0.0f, 0.0f, 0.0f), m_scale(1.0f, 1.0f, 1.0f), m_color(1.0f, 1.0f, 1.0f)
{

	m_vertices[0] = { {-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f} }; // BL
	m_vertices[1] = { {-1.0f,  1.0f, 0.0f}, {1.0f, 1.0f, 1.0f} }; // TL
	m_vertices[2] = { { 1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f} }; // BR
	m_vertices[3] = { { 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f, 1.0f} }; // TR

    m_vertex_buffer = GraphicsEngine::getInstance()->createVertexBuffer();
    m_vertex_buffer->load(m_vertices, sizeof(QuadVertex), 4, shader_byte_code, size_shader);
}

Quad::~Quad()
{}

void Quad::draw()
{
    GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexBuffer(m_vertex_buffer);
    GraphicsEngine::getInstance()->getImmediateDeviceContext()->drawTriangleStrip(m_vertex_buffer->getSizeVertexList(), 0);
}

bool Quad::release()
{
	if (m_vertex_buffer) {
		m_vertex_buffer->release();
		m_vertex_buffer = nullptr;
	}
	delete this;
	return true;
}

void Quad::setPosition(float x, float y, float z)
{
	m_position = Vector3D(x, y, z);
	updateWorldMatrix();
}

void Quad::setScale(float x, float y, float z)
{
	m_scale = Vector3D(x, y, z);
	updateWorldMatrix();
}

void Quad::setScale(float amt)
{
	float x = getScale().x * amt;
	float y = getScale().y * amt;
	float z = getScale().z * amt;
	m_scale = Vector3D(x, y, z);
	updateWorldMatrix();
}

void Quad::setColor(float r, float g, float b) // Solid color
{
	m_color = Vector3D(r, g, b);
}

void Quad::setColor(Vector3D c1, Vector3D c2, Vector3D c3, Vector3D c4) // Per vertex color
{
	m_vertices[0].color = c1;
	m_vertices[1].color = c2;
	m_vertices[2].color = c3;
	m_vertices[3].color = c4;

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->getD3DContext()->UpdateSubresource(
		m_vertex_buffer->getBuffer(),
		0, nullptr, m_vertices, 0, 0
	);
}

void Quad::updateWorldMatrix()
{
	Matrix4x4 scaleMatrix, transMatrix;

	scaleMatrix.setScale(m_scale);
	transMatrix.setTranslation(m_position);

	m_world = scaleMatrix;
	m_world *= transMatrix;
}
