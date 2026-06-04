#pragma once
#include <d3d11.h>

class RasterizerState
{
public:
    RasterizerState();
    ~RasterizerState();

    bool InitializeWireframe(ID3D11Device* device); //initialize the wireframe state

    void SetRasterizer(ID3D11DeviceContext* context); // we bind the resterizer state to outr device context

    void RemoveRasterizer(ID3D11DeviceContext* context); // unbind if we want to reset

	bool release();

private:
    ID3D11RasterizerState* m_state; // rasterizer state reference
};

