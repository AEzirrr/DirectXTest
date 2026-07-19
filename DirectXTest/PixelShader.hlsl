struct PS_INPUT
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
    row_major float4x4 m_proj;
    unsigned int m_time;
    float3 padding; 
    float3 cameraPosition;
};

cbuffer MaterialBuffer : register(b1)
{
    float4 albedoColor;
    float2 tiling;
    float2 offset;
    float metallicMultiplier;
    float smoothnessMultiplier;
    int hasAlbedoMap;
    int hasNormalMap;
    int hasMetallicSmoothnessMap;
    float2 paddingMat;
};

Texture2D AlbedoMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D MetallicSmoothnessMap : register(t2);
SamplerState TextureSampler : register(s0);

static const float PI = 3.14159265359;

// --- PBS Utility Functions ---
float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return num / (PI * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    return GeometrySchlickGGX(NdotV, roughness) * GeometrySchlickGGX(NdotL, roughness);
}

float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float3 TextureNormalToWorldSpace(float3 perturbedNormal, float3 worldPos, float3 vertexNormal, float2 uv)
{
    float3 dp1 = ddx(worldPos);
    float3 dp2 = ddy(worldPos);
    float2 duv1 = ddx(uv);
    float2 duv2 = ddy(uv);

    float3 dp2perp = cross(dp2, vertexNormal);
    float3 dp1perp = cross(vertexNormal, dp1);
    float3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    float3 B = dp2perp * duv1.y + dp1perp * duv2.y;

    float invmax = rsqrt(max(dot(T, T), dot(B, B)));
    float3x3 TBN = float3x3(T * invmax, B * invmax, vertexNormal);

    return normalize(mul(perturbedNormal, TBN));
}

float4 psmain(PS_INPUT input) : SV_TARGET
{
    float2 transformedUV = input.texCoord * tiling + offset;

    float3 albedo = albedoColor.rgb;
    if (hasAlbedoMap == 1)
    {
        albedo *= AlbedoMap.Sample(TextureSampler, transformedUV).rgb;
    }

    float metallic = metallicMultiplier;
    float smoothness = smoothnessMultiplier;

    if (hasMetallicSmoothnessMap == 1)
    {
        float4 metSmoothSample = MetallicSmoothnessMap.Sample(TextureSampler, transformedUV);
        metallic *= metSmoothSample.r;
        smoothness *= metSmoothSample.a;
    }

    float roughness = 1.0 - smoothness;
    roughness = max(roughness, 0.05);

    float3 N = normalize(input.normal);
    
    if (hasNormalMap == 1)
    {
        float3 normalSample = NormalMap.Sample(TextureSampler, transformedUV).xyz;
        
        float3 localNormal = normalSample * 2.0f - 1.0f;
        
        N = TextureNormalToWorldSpace(localNormal, input.worldPos, N, transformedUV);
    }

    float3 V = normalize(cameraPosition - input.worldPos);
    float3 L = normalize(float3(0.5f, 1.0f, -0.5f));
    float3 H = normalize(V + L);

    float3 F0 = float3(0.04f, 0.04f, 0.04f);
    F0 = lerp(F0, albedo, metallic);

    float D = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    float3 F = fresnelSchlick(max(dot(H, V), 0.0f), F0);

    float3 numerator = D * G * F;
    float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.0001f;
    float3 specular = numerator / denominator;

    float3 kS = F;
    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
    kD *= 1.0f - metallic;

    float NdotL = max(dot(N, L), 0.0f);
    float3 directLight = (kD * albedo / PI + specular) * float3(1.0f, 0.95f, 0.9f) * 1.5f * NdotL;
    float3 ambient = float3(0.03f, 0.04f, 0.05f) * albedo;

    return float4(directLight + ambient, 1.0f);
}