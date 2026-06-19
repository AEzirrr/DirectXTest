#pragma once

#include <Windows.h>
#include <d3d11.h>

class DeviceContext;

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	bool load(void* list_indices, UINT size_list);
	UINT getSizeIndexList();
	bool release();

private:
	UINT m_size_list;

private:
	ID3D11Buffer* m_buffer;

private:
	friend class DeviceContext;
};

