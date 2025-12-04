#pragma once

enum class BlendState
{
    AlphaBlend,
    OneOne,
    End
};

enum class DepthStencilState
{
    DepthNone,
    LessEqual,
    End,
};

enum class RasterizerState
{
    SolidBack,
    SolidFront,
    SolidNone,
    Wireframe,
    End,
};

enum class SamplerState
{
    Point,
    Linear,
    End
};