#pragma once

#include "Common/Types.h"

/*
    WindowDesc
    -------------------------------------------------
    - 플랫폼 독립적인 윈도우 생성 정보
    - OS 고유 개념 (HWND, HINSTANCE 등) 포함 금지
*/
struct WindowDesc
{
    uint32 Width = 1920;
    uint32 Height = 1080;

    const wchar_t* Title = L"ShyplantsEngine";

    bool bResizable = true;
    bool bFullscreen = false;
};
