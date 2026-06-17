#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"

#include <d3dcompiler.h>

GraphicsEngine::GraphicsEngine()
{}

GraphicsEngine::~GraphicsEngine()
{}

bool GraphicsEngine::init()
{
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,   // hardware acceleration
		D3D_DRIVER_TYPE_WARP,       // WARP rasterizer
		D3D_DRIVER_TYPE_REFERENCE,  // reference rasterizer
	};

	UINT num_driver_type = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0    // DirectX 11.0
	};

	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT res = 0;

	m_imm_context = nullptr;

	for (UINT driver_type_index = 0; driver_type_index < num_driver_type;) {

		res = D3D11CreateDevice(
			NULL,                    // Use default graphics adapter
			driver_types[driver_type_index],   // Use hardware acceleration
			NULL,                    // No software rasterizer
			NULL,                       // No creation flags
			feature_levels,
			num_feature_levels,         // Default feature levels
			D3D11_SDK_VERSION,          // SDK version
			&m_d3d_device,                  // Output device pointer
			&m_feature_level,                    // Don't care about feature level
			&m_imm_context                     // Don't care about immediate context
		);

		if (SUCCEEDED(res))
			break;
		++driver_type_index;
	}

	if (FAILED(res)) {
		return false;
	}

	m_imm_device_context = new DeviceContext(m_imm_context);


	//
	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device); // Get the DXGI device from the D3D device
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter); // Get the adapter from the device
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory); // Get the factory from the adapter



	return true;
}

bool GraphicsEngine::release()
{
	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();

	m_imm_device_context->release();
	m_d3d_device->Release();

	return true;
}

SwapChain* GraphicsEngine::createSwapChain()
{
	return new SwapChain();
}

DeviceContext* GraphicsEngine::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBuffer* GraphicsEngine::createVertexBuffer()
{
	return new VertexBuffer();
}

IndexBuffer* GraphicsEngine::createIndexBuffer()
{
	return new IndexBuffer();
}

VertexShader* GraphicsEngine::createVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShader* vertex_shader = new VertexShader();

	if (!vertex_shader->init(shader_byte_code, byte_code_size)) {

		vertex_shader->release();
		return nullptr;
	}

	return vertex_shader;
}

PixelShader* GraphicsEngine::createPixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShader* pixel_shader = new PixelShader();

	if (!pixel_shader->init(shader_byte_code, byte_code_size)) {

		pixel_shader->release();
		return nullptr;
	}

	return pixel_shader;
}

ConstantBuffer* GraphicsEngine::createConstantBuffer()
{
	return new ConstantBuffer();
}

bool GraphicsEngine::compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;

	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &error_blob)))
	{
		if (error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();

	return true;
}

bool GraphicsEngine::compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;

	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_blob, &error_blob)))
	{
		if (error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();

	return true;
}

void GraphicsEngine::releaseCompiledShader()
{
	if (m_blob) m_blob->Release();

}

GraphicsEngine* GraphicsEngine::getInstance()
{
	static GraphicsEngine instance;
	return &instance;
}