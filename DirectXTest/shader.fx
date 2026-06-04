struct VS_INPUT
{
    float4 pos : POSITION;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = input.pos;
    return output;
}

float4 psmain(VS_OUTPUT input) : SV_TARGET
{
    return float4(.1, .8, .8, 1);
}