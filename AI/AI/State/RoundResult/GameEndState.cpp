#include "GameEndState.h"
#include "GameManager.h"
#include "DxLib.h"

void GameEndState::Enter(GameManager& gm)
{
    
}

void GameEndState::Update(GameManager& gm)
{
    // ゲーム終了中：スペースで最初からやり直し
    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        gm.RestartGame();
    }
}

void GameEndState::Draw(GameManager& gm)
{
    gm.DrawGame();
}
