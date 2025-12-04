Texture2D tex0 : register(t0);
SamplerState samLinear : register(s0);

struct PSInput
{
    float4 posH : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

float4 PS_Main(PSInput input) : SV_TARGET
{
    float4 texColor = tex0.Sample(samLinear, input.uv);
    return texColor * input.color;
}
