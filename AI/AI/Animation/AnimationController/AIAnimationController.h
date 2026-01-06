#pragma once
#include <string>
#include "AIAnimationFrame.h"  

class AIAnimationController
{
public:
    AIAnimationController(AnimationSprite* bank);

    void Update(float dt);
    void Draw(int x, int y);

    // AI がカードを選んだときに呼ぶ
    void OnChooseCard(int cardValue);

private:
    enum class State
    {
        Idle,
        ShowNumber,
    };

    AIAnimationFrame anim;
    State state = State::Idle;

    std::string MakeHandKey(int cardValue) const;
};
