#include "Quad.h"
#include "DeviceContext.h"
#include <cmath>

Quad::Quad(QuadVertex v1, QuadVertex v2, QuadVertex v3, QuadVertex v4, void* shader_byte_code, UINT size_shader)
{
    QuadVertex vertex_list[] = { v1, v2, v3, v4 };

    m_vertex_buffer = GraphicsEngine::getInstance()->createVertexBuffer();
    m_vertex_buffer->load(vertex_list, sizeof(QuadVertex), 4, shader_byte_code, size_shader);
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