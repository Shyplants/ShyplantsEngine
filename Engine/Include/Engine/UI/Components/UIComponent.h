#pragma once

#include "Engine/Core/Component/ActorComponent.h"

/*
    UIComponent
    -------------------------------------------------
    - UI 전용 컴포넌트 베이스
    - World Transform / Scene hierarchy와 완전히 분리
    - Actor 생명주기만 공유
*/
class UIComponent : public ActorComponent
{
public:
    explicit UIComponent(Actor* owner);
    ~UIComponent() override;

    UIComponent(const UIComponent&) = delete;
    UIComponent& operator=(const UIComponent&) = delete;
};
