#pragma once

#include "AIAnimationFrame.h"
#include "AnimationSprite.h"
#include "AnimationTag.h"

class PlayerAnimationController
{
public:
    PlayerAnimationController(AnimationSprite* bank);

    void Update(float dt);
    void Draw(int x, int y) const;

    void OnTurnWin();
    void StopTurnWin();
    bool IsPlayingTurnWin() const;

private:
    enum class State
    {
        Idle,
        TurnWin
    };

    void EnsureIdle();
    void PlayByTag(const AnimTag& tag);

private:
    AIAnimationFrame anim;

    State state = State::Idle;
    AnimTag currentTag{};
};