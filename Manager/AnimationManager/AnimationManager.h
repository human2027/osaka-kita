#pragma once

#include "AnimationSprite.h"
#include "AIAnimationController.h"
#include "AnimationTag.h"
#include "InitialValue.h"
#include "PlayerAnimationController.h"

class AnimationManager
{
public:
    AnimationManager();

    // アニメ素材の読み込みなど
    void Initialize();

    // 毎フレーム更新
    void Update(float dt);

    // 毎フレーム描画
    void Draw() const;

    // AIがカードを選んだ時に呼ぶ
    void OnAIChooseCard(int cardValue, AnimMood mood);

    // 描画位置変更
    void SetAIDrawPosition(int x, int y);

    bool IsAIShowingCard() const;

    void OnPlayerTurnWin();
    void SetPlayerDrawPosition(int x, int y);
    bool IsPlayerTurnWinPlaying() const;
    //プレイヤーWIN
    void StopPlayerTurnWin();

    //敵マップアニメーション
    void DrawEnemyMapChar(int centerX, int centerY) const;
    void PlayEnemyMapAnimation();
    void DrawPlayerMapChar(int centerX, int centerY) const;
   

private:
    AnimationSprite aiBank;
    AIAnimationController aiController;

    AnimationSprite playerBank;
    PlayerAnimationController playerController;
    //マップ上の敵表示用アニメ
    AIAnimationFrame enemyMapAnim;
    //マップ上のプレイヤー表示用アニメ
    AIAnimationFrame playerMapAnim;

    int aiDrawX = 0;
    int aiDrawY = 0;

    int playerDrawX = 0;
    int playerDrawY = 0;
};