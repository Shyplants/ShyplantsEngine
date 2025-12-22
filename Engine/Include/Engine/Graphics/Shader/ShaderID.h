#pragma once

#include "Common/Types.h"

enum class ShaderStage : uint8
{
    Vertex,
    Pixel
};

struct ShaderID
{
    uint32_t Value = 0;
    ShaderStage Stage{ ShaderStage::Vertex };

    bool operator==(const ShaderID& rhs) const
    {
        return Value == rhs.Value && Stage == rhs.Stage;
    }

    bool operator!=(const ShaderID& rhs) const
    {
        return !(*this == rhs);
    }
};

constexpr ShaderID InvalidVertexShaderID{ 0, ShaderStage::Vertex };
constexpr ShaderID InvalidPixelShaderID{ 0, ShaderStage::Pixel };