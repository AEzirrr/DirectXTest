#pragma once
#include <string>
#include "Vector4D.h" 
#include "Vector2D.h" 

class ConstantBuffer;
class DeviceContext;
class PixelShader;

__declspec(align(16)) struct MaterialConstantData {
    Vector4D albedoColor;
    Vector2D tiling;     
    Vector2D offset;     
    float metallic;
    float smoothness;
    int hasAlbedoMap;             
    int hasNormalMap;            
    int hasMetallicSmoothnessMap; 
    float padding[3];
};

class Texture;

class Material {
public:
    Material(std::string name);
    ~Material();

    void setAlbedoColor(const Vector4D& color);
    Vector4D getAlbedoColor() const;
    std::string getName() const;

    void setTiling(Vector2D tile) { m_bufferData.tiling = tile; }
    Vector2D getTiling() const { return m_bufferData.tiling; }

    void setOffset(Vector2D off) { m_bufferData.offset = off; }
    Vector2D getOffset() const { return m_bufferData.offset; }

    void setBaseTexture(Texture* texture) { delete m_baseTexture; m_baseTexture = texture; }

    void setMetallic(float val) { m_bufferData.metallic = val; }
    float getMetallic() const { return m_bufferData.metallic; }

    void setSmoothness(float val) { m_bufferData.smoothness = val; }
    float getSmoothness() const { return m_bufferData.smoothness; }

    void setNormalTexture(Texture* texture) { delete m_normalTexture; m_normalTexture = texture; }

    void setMetallicSmoothnessTexture(Texture* texture) { delete m_metSmoothTexture; m_metSmoothTexture = texture; }

    void bind(DeviceContext* deviceContext, PixelShader* pixelShader);

private:
    std::string m_name;
    MaterialConstantData m_bufferData;    
    ConstantBuffer* m_materialConstantBuffer;
    Texture* m_baseTexture = nullptr;
    Texture* m_normalTexture = nullptr;    
    Texture* m_metSmoothTexture = nullptr;  
};