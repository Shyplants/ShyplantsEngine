#pragma once

class RenderDevice;

/*
    ResourceLoadContext
    -------------------------------------------------
    - 리소스 로딩 시 필요한 외부 시스템 묶음
    - ResourceManager는 내용을 해석하지 않는다
*/
struct ResourceLoadContext
{
    RenderDevice* renderDevice{ nullptr };
};
