#pragma once

/*
    ConstantBufferUsage
    -------------------------------------------------
    - GPU 버퍼 갱신 전략
*/
enum class ConstantBufferUsage
{
    Static,     // 거의 갱신 없음 (Init-only)
    Dynamic     // 매 프레임 / 매 드로우 갱신
};
