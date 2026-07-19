#include "Material.h"
#include "ConstantBuffer.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "Texture.h"

Material::Material(std::string name) : m_name(name) {
    m_bufferData.albedoColor = Vector4D(1.0f, 1.0f, 1.0f, 1.0f);
    m_bufferData.tiling = Vector2D::ones();  
    m_bufferData.offset = Vector2D::zeros();
    m_bufferData.metallic = 0.0f;            
    m_bufferData.smoothness = 0.5f;

    m_bufferData.hasAlbedoMap = 0;
    m_bufferData.hasNormalMap = 0;
    m_bufferData.hasMetallicSmoothnessMap = 0;

    m_materialConstantBuffer = GraphicsEngine::getInstance()->createConstantBuffer();
    m_materialConstantBuffer->load(&m_bufferData, sizeof(MaterialConstantData));
}

Material::~Material() {
    if (m_materialConstantBuffer) {
        m_materialConstantBuffer->release();
    }
    delete m_baseTexture;
    delete m_normalTexture;
    delete m_metSmoothTexture;
}

void Material::setAlbedoColor(const Vector4D& color) {
    m_bufferData.albedoColor = color;
}

Vector4D Material::getAlbedoColor() const {
    return m_bufferData.albedoColor;
}

std::string Material::getName() const {
    return m_name;
}

void Material::bind(DeviceContext* deviceContext, PixelShader* pixelShader) {
    m_bufferData.hasAlbedoMap = (m_baseTexture != nullptr) ? 1 : 0;
    m_bufferData.hasNormalMap = (m_normalTexture != nullptr) ? 1 : 0;
    m_bufferData.hasMetallicSmoothnessMap = (m_metSmoothTexture != nullptr) ? 1 : 0;

    m_materialConstantBuffer->update(deviceContext, &m_bufferData); 
    deviceContext->setConstantBuffer(pixelShader, m_materialConstantBuffer, 1);

    if (m_baseTexture) m_baseTexture->bind(deviceContext, 0); 
    if (m_normalTexture) m_normalTexture->bind(deviceContext, 1);
    if (m_metSmoothTexture) m_metSmoothTexture->bind(deviceContext, 2);

    ID3D11SamplerState* samplerState = GraphicsEngine::getInstance()->getSamplerState();
    if (samplerState) {
        deviceContext->getD3DContext()->PSSetSamplers(0, 1, &samplerState);
    }
}