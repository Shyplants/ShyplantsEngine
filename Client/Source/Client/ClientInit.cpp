#include "ClientInit.h"

#include "Engine/Core/Engine.h"

// Game-side includes
#include "Game/GameModes/DemoGameMode.h"
#include "Game/Levels/DemoLevel.h"

namespace ClientInit
{
    void SetupGame(Engine& engine)
    {
        // -----------------------------------------
        // GameMode 선택
        // -----------------------------------------
        engine.SetGameMode(
            std::make_unique<DemoGameMode>(engine.GetWorld())
        );

        // -----------------------------------------
        // GameplayLevel 로드
        // -----------------------------------------
        engine.LoadGameplayLevel(
            std::make_unique<DemoLevel>()
        );
    }
}
