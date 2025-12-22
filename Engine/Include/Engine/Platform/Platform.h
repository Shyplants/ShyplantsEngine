#pragma once

#include <memory>

#include "Common/Types.h"

// Forward declarations
class Window;

/*
    Platform
    -------------------------------------------------
    - 플랫폼 추상 Facade
    - Engine은 오직 이 클래스만 사용
    - OS 분기 / 구현체 선택 책임
*/
class Platform
{
public:
    // =========================================================
    // Window
    // =========================================================
    static std::unique_ptr<Window> CreatePlatformWindow();

    // =========================================================
    // Time
    // =========================================================
    // - High resolution time (seconds)
    static double GetTimeSeconds();
};
