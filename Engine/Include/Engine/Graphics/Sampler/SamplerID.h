#pragma once

#include "Common/Types.h"

// ------------------------------------------------------------
// SamplerStage
// ------------------------------------------------------------
// - 현재는 Pixel 고정
// - 향후 VS/CS/GS 대응 가능
enum class SamplerStage : uint8
{
    Pixel,
    Vertex,
    Compute
};

// ------------------------------------------------------------
// SamplerID
// ------------------------------------------------------------
struct SamplerID
{
    uint32 Value;
    SamplerStage Stage;

    constexpr SamplerID(
        uint32 value = 0,
        SamplerStage stage = SamplerStage::Pixel)
        : Value(value)
        , Stage(stage)
    {
    }

    constexpr bool IsValid() const
    {
        return Value != 0;
    }

    constexpr bool operator==(const SamplerID& rhs) const 
    { 
        return Value == rhs.Value && Stage == rhs.Stage; 
    }
};

// ------------------------------------------------------------
// BuiltinSampler
// ------------------------------------------------------------
// - 엔진이 기본 제공하는 Sampler ID
// - 절대 삭제/변경 금지 (ABI 안정성)
namespace BuiltinSampler
{
    inline constexpr SamplerID LinearClamp{ 1, SamplerStage::Pixel };
    inline constexpr SamplerID PointWrap{2, SamplerStage::Pixel };
}


inline constexpr SamplerID InvalidSamplerID{};

// ------------------------------------------------------------
// Hash
// ------------------------------------------------------------
struct SamplerIDHash
{
    size_t operator()(const SamplerID& id) const noexcept
    {
        return (static_cast<size_t>(id.Value) << 3)
            ^ static_cast<size_t>(id.Stage);
    }
};
