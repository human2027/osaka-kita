#include "TitleState.h"
#include "GameManager.h"
#include <DxLib.h>

void TitleState::Enter(GameManager& gm)
{
}

void TitleState::Update(GameManager& gm)
{
    // Enterキーで開始
    if (CheckHitKey(KEY_INPUT_RETURN))
    {
        gm.StartGame();
    }

    // 左クリックでも開始
    if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
    {
        gm.StartGame();
    }
}

void TitleState::Draw(GameManager& gm)
{
    gm.GetUIManager().DrawTitleScreen();
}

void TitleState::Exit(GameManager& gm)
{
}