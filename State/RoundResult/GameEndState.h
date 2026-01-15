#pragma once
#include "StateBase.h"

class GameEndState : public StateBase
{
public:
    void Enter(GameManager& gm) override;
    void Update(GameManager& gm) override;
    void Draw(GameManager& gm) override;
    void Exit(GameManager& gm) override {}
};
