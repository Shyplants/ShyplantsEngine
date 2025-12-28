#pragma once

#include "Engine/Core/World/Actor.h"

/*
    UIActor
    -------------------------------------------------
    - Screen-space UI 傈侩 Actor
    - World Transform / Camera / Gameplay客 公包
    - 亲惑 PersistentLevel俊 家加
*/
class UIActor : public Actor
{
public:
    UIActor();
    ~UIActor() override = default;

public:
    // =====================================================
    // Actor override
    // =====================================================
    bool IsPersistentActor() const override
    {
        return true;
    }

    void OnSpawned() override;
};
