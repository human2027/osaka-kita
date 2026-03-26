#pragma once

#include "AnimationSprite.h"
#include "AIAnimationController.h"
#include "AnimationTag.h"
#include "InitialValue.h"

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
private:
    AnimationSprite aiBank;
    AIAnimationController aiController;
    int aiDrawX = 0;
    int aiDrawY = 0;
};