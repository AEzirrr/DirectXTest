#include "RasterizerState.h"

RasterizerState::RasterizerState(){}
RasterizerState::~RasterizerState(){}

bool RasterizerState::InitializeWireframe(ID3D11Device* device) //initialize the wireframe state
{
    D3D11_RASTERIZER_DESC desc = {};
	desc.FillMode = D3D11_FILL_WIREFRAME; // set the rasterizer desc to wireframe mode
    desc.CullMode = D3D11_CULL_NONE;      // cull mode to none 
    //desc.AntialiasedLineEnable = true; // Enable anti-aliasing for smoother lines
    //desc.FrontCounterClockwise = false;
    //desc.DepthClipEnable = true;

    HRESULT hr = device->CreateRasterizerState(&desc, &m_state);
    return SUCCEEDED(hr);
}

void RasterizerState::SetRasterizer(ID3D11DeviceContext* context) // we bind the resterizer state to outr device context
{
    context->RSSetState(m_state);
}

void RasterizerState::RemoveRasterizer(ID3D11DeviceContext* context) // unbind if we want to reset
{
    context->RSSetState(nullptr);
}

bool RasterizerState::release()
{
    m_state->Release();
    delete this;

    return true;
}

