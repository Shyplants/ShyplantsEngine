#pragma once

/*
    LevelResourceSet
    -------------------------------------------------
    - Level 진입 시 로드되는 리소스 묶음
    - Level 종료 시 해제되는 리소스 집합
    - 구체 리소스는 Client에서 정의
*/
class LevelResourceSet
{
public:
    virtual ~LevelResourceSet();

    virtual void Load() = 0;
    virtual void Unload() = 0;
};
