// ============================================================
// SpriteVS.hlsl
// - Sprite Vertex Shader (WVP + UVRect)
// ============================================================

// ------------------------------------------------------------
// Constant Buffer (b0)
// ------------------------------------------------------------
cbuffer SpriteCB : register(b0)
{
    float4x4 gWVP; // World * View * Projection
    float4 gColor; // RGBA
    float4 gUVRect; // (u, v, width, height)
    float4 gPadding; // unused
};

// ------------------------------------------------------------
// Input / Output
// ------------------------------------------------------------
struct VSInput
{
    float3 Position : POSITION; // local space
    float2 TexCoord : TEXCOORD0; // 0~1
};

struct VSOutput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
};

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
VSOutput main(VSInput input)
{
    VSOutput output;

    // Transform
    output.Position = mul(float4(input.Position, 1.0f), gWVP);

    // Vertex Color * Constant Color
    output.Color = gColor;

    // UV Rect (Atlas / Sub-rect)
    output.TexCoord = gUVRect.xy + input.TexCoord * gUVRect.zw;

    return output;
}
