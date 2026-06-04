#pragma once

#include <Windows.h>
#include <d3d11.h>

class DeviceContext;

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	bool load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader);

	void update(DeviceContext* context, void* vertex_list, UINT size_byte_list);

	UINT getSizeVertexList();
	bool release();


	ID3D11Buffer* getBuffer() const { return m_buffer; }

private:
	UINT m_size_vertex;
	UINT m_size_list;
	
private:
	ID3D11Buffer* m_buffer;
	ID3D11InputLayout* m_input_layout;

private:
	friend class DeviceContext;
};

