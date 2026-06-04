#pragma once
#include "GraphicsEngine.h"
#include "VertexBuffer.h"

struct vec3 {
    float x, y, z;
};

struct QuadVertex {
    vec3 pos;
    vec3 color;
};

class Quad
{
public:
    Quad(QuadVertex v1, QuadVertex v2, QuadVertex v3, QuadVertex v4, void* shader_byte_code, UINT size_shader);
    ~Quad();

    void draw();

    bool release();

private:
    VertexBuffer* m_vertex_buffer = nullptr;
    UINT m_vertex_count = 4;

};