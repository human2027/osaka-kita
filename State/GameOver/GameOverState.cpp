#include "GameOverState.h"
#include "GameManager.h"
#include "TitleState.h"
#include <DxLib.h>

void GameOverState::Enter(GameManager& gm)
{
    gm.GetUIManager().ClearMessages();
}

void GameOverState::Update(GameManager& gm)
{
    if (CheckHitKey(KEY_INPUT_RETURN))
    {
        gm.ChangeState(std::make_shared<TitleState>());
    }
}

void GameOverState::Draw(GameManager& gm)
{
    DrawString(100, 100, "GAME OVER", GetColor(255, 255, 255));
    DrawString(100, 160, "Press ENTER to Title", GetColor(255, 255, 255));
}

void GameOverState::Exit(GameManager& gm)
{
}