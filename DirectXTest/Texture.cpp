#include "Texture.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const wchar_t* full_path) {
    std::wstring ws(full_path);
    std::string path(ws.begin(), ws.end());

    int width = 0;
    int height = 0;
    int channels = 0;

    unsigned char* image_data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (!image_data) {
        std::cerr << "Failed to load texture file path: " << path << std::endl;
        return;
    }

    D3D11_TEXTURE2D_DESC texture_desc = {};
    texture_desc.Width = width;
    texture_desc.Height = height;
    texture_desc.MipLevels = 1;
    texture_desc.ArraySize = 1;
    texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
    texture_desc.SampleDesc.Count = 1;
    texture_desc.SampleDesc.Quality = 0;
    texture_desc.Usage = D3D11_USAGE_DEFAULT;
    texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texture_desc.CPUAccessFlags = 0;
    texture_desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA init_data = {};
    init_data.pSysMem = image_data;
    init_data.SysMemPitch = width * 4; // Width dimensions scaled by 4 channels (RGBA bytes)
    init_data.SysMemSlicePitch = width * height * 4;

    HRESULT hr = GraphicsEngine::getInstance()->getDevice()->CreateTexture2D(&texture_desc, &init_data, (ID3D11Texture2D**)&m_texture);

    if (SUCCEEDED(hr)) {
        // 3. Create the Shader Resource View (SRV) for binding into shader asset slots
        D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
        srv_desc.Format = texture_desc.Format;
        srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srv_desc.Texture2D.MostDetailedMip = 0;
        srv_desc.Texture2D.MipLevels = 1;

        hr = GraphicsEngine::getInstance()->getDevice()->CreateShaderResourceView(m_texture, &srv_desc, &m_shader_res_view);
    }

    stbi_image_free(image_data);
}

Texture::~Texture() {
    if (m_shader_res_view) m_shader_res_view->Release();
    if (m_texture) m_texture->Release();
}

void Texture::bind(DeviceContext* device_context, unsigned int slot) {
    if (m_shader_res_view) {
        device_context->getD3DContext()->PSSetShaderResources(slot, 1, &m_shader_res_view);
    }
}