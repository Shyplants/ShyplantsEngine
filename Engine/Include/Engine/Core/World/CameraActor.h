#pragma once

#include "Engine/Core/World/WorldActor.h"

// Forward declarations
class CameraComponent2D;

/*
    CameraActor
    -------------------------------------------------
    - World-space 2D Camera Actor
    - PersistentLevel 소속
    - WorldTransform + CameraComponent2D 조립자
*/
class CameraActor final : public WorldActor
{
public:
    CameraActor();
    ~CameraActor() override = default;

    CameraActor(const CameraActor&) = delete;
    CameraActor& operator=(const CameraActor&) = delete;

public:
    // =====================================================
    // Actor overrides
    // =====================================================
    void OnSpawned() override;

public:
    // =====================================================
    // Level ownership
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
