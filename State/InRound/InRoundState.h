#pragma once
#include "StateBase.h"
#include "GameManager.h"

class InRoundState : public StateBase
{
public:
    // ステートに入った瞬間（ラウンド開始直後など）
    void Enter(GameManager& gm) override;

    // 毎フレームの更新処理
    void Update(GameManager& gm) override;

    // 毎フレームの描画処理
    void Draw(GameManager& gm) override;

    // ステートを抜ける瞬間（ラウンド終了時など）
    void Exit(GameManager& gm) override;
};
