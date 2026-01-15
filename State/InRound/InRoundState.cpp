#include "InRoundState.h"
#include "GameManager.h"
#include "RoundResultState.h"
#include "GameEndState.h"

void InRoundState::Enter(GameManager& gm)
{
    // ラウンド突入時に何か演出を入れるならここ
}

void InRoundState::Update(GameManager& gm)
{
    // 1フレーム分の「ラウンド中の処理」は GameManager に任せる
    gm.UpdateInRound();

    // その結果、ラウンドが終了しているかどうかをチェック
    if (gm.IsRoundFinished())
    {
        // ゴールに到達したなら、そのままゲーム終了ステートへ
        if (gm.HasPlayerReachedGoal() || gm.HasAIReachedGoal())
        {
            gm.ChangeState(std::make_shared<GameEndState>());
        }
        else
        {
            // それ以外（ターン上限など）の場合はラウンド結果表示ステートへ
            gm.ChangeState(std::make_shared<RoundResultState>());
        }
    }
}

void InRoundState::Draw(GameManager& gm)
{
    // 描画は GameManager 側の共通関数に任せる
    gm.DrawGame();
}

void InRoundState::Exit(GameManager& gm)
{
    // ラウンド終了直前に何か後片付けしたいならここで
}
