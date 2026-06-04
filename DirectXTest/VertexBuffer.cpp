#include "VertexBuffer.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"

VertexBuffer::VertexBuffer() :m_input_layout(0), m_buffer(0)
{}

VertexBuffer::~VertexBuffer()
{}

bool VertexBuffer::load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader)
{
	if (m_buffer)m_buffer->Release();
	if (m_input_layout)m_input_layout->Release();

	D3D11_BUFFER_DESC buffer_desc = {};
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.ByteWidth = size_vertex * size_list;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_vertices;

	m_size_vertex = size_vertex;
	m_size_list = size_list;

	if (FAILED(GraphicsEngine::getInstance()->m_d3d_device->CreateBuffer(&buffer_desc, &init_data, &m_buffer))) {
		
		return false;
	}


	D3D11_INPUT_ELEMENT_DESC layout[] = {


		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT size_layout = ARRAYSIZE(layout);

	if (FAILED(GraphicsEngine::getInstance()->m_d3d_device->CreateInputLayout(layout, size_layout, shader_byte_code, size_byte_shader, &m_input_layout))) {
		
		return false;
	}

    return true;
}

void VertexBuffer::update(DeviceContext* context, void* vertex_list, UINT size_byte_list)
{
	context->getD3DContext()->UpdateSubresource(
		this->m_buffer,
		0,
		nullptr,
		vertex_list,
		0,
		0
	);
}

UINT VertexBuffer::getSizeVertexList()
{
	return m_size_list;
}

bool VertexBuffer::release()
{
	m_input_layout->Release();
	m_buffer->Release();
	delete this;

    return true;
}
