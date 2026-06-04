struct VS_INPUT
{
    float4 position : POSITION;
    float3 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_projection;
    unsigned int m_time;
    float3 m_color;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    // 1. Convert to float4 for matrix multiplication
    float4 worldPos = mul(input.position, m_world);
    float4 viewPos = mul(worldPos, m_view);
    
    // 2. Final position projected to the screen
    output.position = mul(viewPos, m_projection);
    output.color = input.color;

    output.color = input.color;

    return output;
}