#include "RoundResultState.h"
#include "GameManager.h"
#include "DxLib.h"

void RoundResultState::Enter(GameManager& gm)
{
  
}

void RoundResultState::Update(GameManager& gm)
{
    // ラウンド結果表示中はスペース待ち
    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        gm.NextRound();
    }
}

void RoundResultState::Draw(GameManager& gm)
{
    gm.DrawGame();
}
