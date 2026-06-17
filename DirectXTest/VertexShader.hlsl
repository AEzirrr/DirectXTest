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
    
    // 1. Calculate the position in world space
    float4 worldPos = mul(input.position, m_world);
    
    // 2. Calculate the position relative to the camera (View Space)
    float4 viewPos = mul(worldPos, m_view);
    
    // 3. Final position projected to the 2D screen
    output.position = mul(viewPos, m_projection);
    
    // 4. Pass the color through to the pixel shader
    output.color = input.color;

    return output;
}