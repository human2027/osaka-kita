#include "ClearState.h"
#include "GameManager.h"
#include "TitleState.h"
#include <DxLib.h>

void ClearState::Enter(GameManager& gm)
{
}

void ClearState::Update(GameManager& gm)
{
    if (CheckHitKey(KEY_INPUT_RETURN))
    {
        gm.ChangeState(std::make_shared<TitleState>());
    }
}

void ClearState::Draw(GameManager& gm)
{
    DrawString(100, 100, "GAME CLEAR!", GetColor(255, 255, 255));
    DrawString(100, 160, "Press ENTER to Title", GetColor(255, 255, 255));
}

void ClearState::Exit(GameManager& gm)
{
}