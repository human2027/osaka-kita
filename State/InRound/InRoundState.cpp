#include "InRoundState.h"
#include "GameManager.h"

void InRoundState::Enter(GameManager& gm)
{
    // ラウンド突入時に何か演出を入れるならここ
}

void InRoundState::Update(GameManager& gm)
{
    // ラウンド中の進行・状態遷移は GameManager 側
        gm.UpdateInRound();
}

void InRoundState::Draw(GameManager& gm)
{
    gm.DrawGame();
}

void InRoundState::Exit(GameManager& gm)
{
    // ラウンド終了直前に何かするならここ

}