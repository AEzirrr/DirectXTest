struct VS_INPUT
{
    float4 position : POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPos : TEXCOORD1; 
};

cbuffer TransformBuffer : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_projection;
    unsigned int m_time;
    float3 padding;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    float4 worldPos = mul(input.position, m_world);
    output.worldPos = worldPos.xyz;
    
    float4 viewPos = mul(worldPos, m_view);
    
    output.position = mul(viewPos, m_projection);
    
    output.normal = normalize(mul(input.normal, (float3x3) m_world));
    
    output.texCoord = input.texCoord;

    return output;
}