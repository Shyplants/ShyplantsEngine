#pragma once

/*
    WindowsPlatform
    -------------------------------------------------
    - Windows 전용 Platform 구현
    - Platform facade에서만 사용
    - Engine/Core/Graphics는 이 파일을 직접 include 금지
*/
class WindowsPlatform
{
public:
    // Window
    static class Window* CreatePlatformWindow();

    // Time
    static double GetTimeSeconds();
};
