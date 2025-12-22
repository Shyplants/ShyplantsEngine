#include "ClientPCH/ClientPCH.h"

#include "Game/GameBootstrap.h"

#include "Engine/Core/Engine.h"
#include "Engine/Core/World/World.h"

// -----------------------------------------------------
// Game Content
// -----------------------------------------------------
#include "Game/Levels/DemoLevel.h"

namespace Game
{
    // =====================================================
    // Initial Game Setup
    // =====================================================

    void LoadInitialLevel(Engine& engine)
    {
        auto& world = engine.GetWorld();

        world.LoadLevel(std::make_unique<DemoLevel>());

#if defined(_DEBUG)
        SP_LOG(LogCore, ELogLevel::Trace,
            "GameBootstrap: DemoLevel loaded");
#endif
    }
}
