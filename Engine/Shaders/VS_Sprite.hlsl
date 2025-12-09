cbuffer SpriteCB : register(b0)
{
    float4x4 WVP;
    float4 Color;
    
    float2 TexCoord;
    float2 TexSize;
};

struct VSInput
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct PSInput
{
    float4 posH : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

PSInput VS_Main(VSInput input)
{
    PSInput output;

    float4 localPos = float4(input.pos, 1.0f);
    output.posH = mul(localPos, WVP);

    output.color = input.color * Color;
    
    output.uv = TexCoord + input.uv * TexSize;

    return output;
}
