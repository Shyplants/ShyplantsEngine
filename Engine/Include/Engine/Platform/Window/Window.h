#pragma once
#include <functional>
#include "Common/Types.h"
#include "Engine/Platform/Window/WindowDesc.h"

class Window
{
public:
    using ResizeCallback = std::function<void(uint32, uint32)>;

    virtual ~Window() = default;

    virtual bool Create(const WindowDesc& desc) = 0;
    virtual void Destroy() = 0;
    virtual void PollEvents() = 0;

    virtual uint32 GetWidth() const = 0;
    virtual uint32 GetHeight() const = 0;

    virtual bool IsMinimized() const = 0;
    virtual bool ShouldClose() const = 0;

    virtual void* GetNativeHandle() const = 0;

public:
    void SetResizeCallback(ResizeCallback callback)
    {
        m_onResize = std::move(callback);
    }

protected:
    void NotifyResize(uint32 width, uint32 height)
    {
        if (m_onResize)
            m_onResize(width, height);
    }

private:
    ResizeCallback m_onResize;
};
