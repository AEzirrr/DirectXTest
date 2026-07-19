#include "CreditsScreen.h"
#include "ImGui/imgui.h"
#include <wincodec.h>
#include <vector>
#include <iostream>
#pragma comment(lib, "windowscodecs.lib")

CreditsScreen::CreditsScreen(String name, ID3D11Device* device) : AUIScreen(name)
{
    const bool loaded = LoadTextureFromFile(L"Images/DLSU LOGO.png", device, &m_logoTexture, &m_logoWidth, &m_logoHeight);
    if (!loaded) {
        std::cout << "Failed to load texture from file." << std::endl;
    }
}

CreditsScreen::~CreditsScreen()
{
}

bool CreditsScreen::LoadTextureFromFile(const wchar_t* filename, ID3D11Device* device, ID3D11ShaderResourceView** outSRV, int* outWidth, int* outHeight)
{
    IWICImagingFactory* wicFactory = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
    if (FAILED(hr)) return false;

    IWICBitmapDecoder* decoder = nullptr;
    hr = wicFactory->CreateDecoderFromFilename(filename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
    if (FAILED(hr)) return false;

    IWICBitmapFrameDecode* frame = nullptr;
    decoder->GetFrame(0, &frame);

    IWICFormatConverter* converter = nullptr;
    wicFactory->CreateFormatConverter(&converter);
    converter->Initialize(frame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);

    UINT width, height;
    converter->GetSize(&width, &height);

    std::vector<unsigned char> pixels(width * height * 4);
    converter->CopyPixels(nullptr, width * 4, (UINT)pixels.size(), pixels.data());

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA subResource = {};
    subResource.pSysMem = pixels.data();
    subResource.SysMemPitch = width * 4;

    ID3D11Texture2D* texture = nullptr;
    device->CreateTexture2D(&desc, &subResource, &texture);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    device->CreateShaderResourceView(texture, &srvDesc, outSRV);
    texture->Release();

    *outWidth = width;
    *outHeight = height;

    converter->Release();
    frame->Release();
    decoder->Release();
    wicFactory->Release();

    return true;
}

void CreditsScreen::drawUI()
{
    if (isOpen == false) return;

    ImGui::Begin(getName().c_str());

    if (m_logoTexture)
    {
        ImGui::Image((ImTextureID)m_logoTexture, ImVec2(m_displayWidth, m_displayHeight));
    }

    ImGui::Text("DirectX Engine v1.5");
    ImGui::Text("Developed by: Marlou Vincent Ruiz");
    ImGui::Text("July 15, 2026");
    ImGui::Text("GDENG03 X21");
    ImGui::Text("");
    ImGui::Text("Acknowledgements:");
    ImGui::Text("Pardcode DirextX Engine Tutorial");
    ImGui::Text("https://www.pardcode.com");
    ImGui::Text("");
    ImGui::Text("GDENG03 Instructors:");
    ImGui::Text("Martin Laureta");
    ImGui::Text("Neil Patrick Del Gallego");
    ImGui::Text("");
    ImGui::Text("My Bias:");
    ImGui::Text("Choi Jisu (Lia from ITZY)");

    ImGui::End();
}

