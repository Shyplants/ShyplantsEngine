#pragma once

#include "Engine/Core/World/Actor.h"

// Forward declarations
class CameraComponent2D;

/*
    CameraActor
    -------------------------------------------------
    - 엔진 기본 제공 카메라 Actor
    - PersistentLevel에 소속
    - CameraComponent2D를 기본 소유
    - World에 ActiveCamera로 자동 등록
*/
class CameraActor : public Actor
{
public:
    CameraActor();
    ~CameraActor() override;

    CameraActor(const CameraActor&) = delete;
    CameraActor& operator=(const CameraActor&) = delete;

public:
    // =====================================================
    // Actor Overrides
    // =====================================================
    void OnSpawned() override;

public:
    // =====================================================
    // Level Ownership Policy
    // =====================================================
    bool IsPersistentActor() const override { return true; }

public:
    // =====================================================
    // Accessors
    // =====================================================
    CameraComponent2D* GetCamera() const { return m_camera; }

private:
    CameraComponent2D* m_camera{ nullptr };
};
