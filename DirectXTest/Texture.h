#pragma once
#include <string>
#include <d3d11.h>

class DeviceContext;

class Texture {
public:
    Texture(const wchar_t* full_path);
    ~Texture();

    void bind(DeviceContext* device_context, unsigned int slot);

    bool isValid() const { return m_shader_res_view != nullptr; }

private:
    ID3D11Resource* m_texture = nullptr;
    ID3D11ShaderResourceView* m_shader_res_view = nullptr;
};