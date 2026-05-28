#include "RoundResultState.h"
#include "GameManager.h"
#include "DxLib.h"

void RoundResultState::Enter(GameManager& gm)
{
  
}

void RoundResultState::Update(GameManager& gm)
{
        gm.NextRound();
}

void RoundResultState::Draw(GameManager& gm)
{
    gm.DrawGame();
}
