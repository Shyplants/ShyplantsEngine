// ============================================================
// SpritePS.hlsl
// - Sprite Pixel Shader (Texture * Color)
// ============================================================

Texture2D gSpriteTexture : register(t0);
SamplerState gSampler : register(s0);

// ------------------------------------------------------------
// Input
// ------------------------------------------------------------
struct PSInput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
};

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
float4 main(PSInput input) : SV_TARGET
{
    float4 texColor = gSpriteTexture.Sample(gSampler, input.TexCoord);

    // Multiply texture with vertex & constant color
    float4 finalColor = texColor * input.Color;

    // Optional alpha discard (cutout sprites)
    // if (finalColor.a < 0.001f)
    //     discard;

    return finalColor;
}
