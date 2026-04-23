#pragma once
#include <cstdint>
#include "AIAnimationFrame.h"
#include "AnimationTag.h"   
#include "AnimDB.h"
#include "AnimationSprite.h"

class AIAnimationController
{
public:
    enum class State : uint8_t
    {
        Idle,
        ShowNumber
    };

    explicit AIAnimationController(AnimationSprite* bank);

    // AIがカードを選んだとき呼ぶ
    //「ブラフっぽい顔」「強気」など、演出側の気分を渡せる
    void OnChooseCard(int cardValue, AnimMood mood = AnimMood::Normal);

    void Update(float dt);
    void Draw(int x, int y) const;
    bool IsShowingCard() const;
private:
    void PlayByTag(const AnimTag& tag);
    void EnsureIdle();
   
private:
    AIAnimationFrame anim;

    State  state = State::Idle;
    int    shownNumber = 0;    //表示する数字
    AnimTag currentTag{};      //今なにを再生してるかのタグ
};