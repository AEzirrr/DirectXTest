#pragma once
#include "GraphicsEngine.h"
#include "VertexBuffer.h"
#include "Vector3D.h"
#include "Matrix4x4.h"

struct QuadVertex {
    Vector3D pos;
    Vector3D color;
};

class Quad
{
public:
    Quad(void* shader_byte_code, UINT size_shader);
    ~Quad();

    void draw();
    bool release();

    void setPosition(float x, float y, float z);

    void setScale(float x, float y, float z);
    void setScale(float amt);

    Vector3D getPosition() const { return m_position; }
    Vector3D getScale() const { return m_scale; }

    void setColor(float r, float g, float b);

    void setColor(Vector3D c1, Vector3D c2, Vector3D c3, Vector3D c4);

	Vector3D getColor() const { return m_color; } 

    Matrix4x4 getTransform() const { return m_world; }

private:
    void updateWorldMatrix();

private:
    VertexBuffer* m_vertex_buffer = nullptr;
    UINT m_vertex_count = 4;

    QuadVertex m_vertices[4];

    Vector3D m_position;
    Vector3D m_scale;
    Vector3D m_color;

    Matrix4x4 m_world;
};