#include "PlayerAnimationController.h"
#include "AnimDB.h"

PlayerAnimationController::PlayerAnimationController(AnimationSprite* bank)
    : anim(bank)
{
    PlayByTag(AnimTag{ AnimGroup::Player, AnimAction::Idle, AnimMood::Normal });
    state = State::Idle;
}

void PlayerAnimationController::OnTurnWin()
{
    const AnimTag tag{
        AnimGroup::Player,
        AnimAction::TurnWin,
        AnimMood::Normal
    };

    auto specOpt = AnimDB::Find(tag);
    if (!specOpt.has_value())
    {
        return;
    }

    const AnimPlaySpec& spec = specOpt.value();

    if (!anim.Play(spec.key, spec.loop, spec.fps))
    {
        return;
    }

    state = State::TurnWin;
    currentTag = tag;
}

void PlayerAnimationController::StopTurnWin()
{
    if (state == State::TurnWin)
    {
        state = State::Idle;
        EnsureIdle();
    }
}

void PlayerAnimationController::Update(float dt)
{
    anim.Update(dt);

    switch (state)
    {
    case State::Idle:
        EnsureIdle();
        break;

    case State::TurnWin:
        if (anim.IsFinished())
        {
            state = State::Idle;
            EnsureIdle();
        }
        break;
    }
}

void PlayerAnimationController::Draw(int x, int y) const
{
    if (state == State::TurnWin)
    {
        anim.Draw(x, y);
    }
}

void PlayerAnimationController::EnsureIdle()
{
    const AnimTag idle{
        AnimGroup::Player,
        AnimAction::Idle,
        AnimMood::Normal
    };

    if (!(currentTag == idle))
    {
        PlayByTag(idle);
    }
}

void PlayerAnimationController::PlayByTag(const AnimTag& tag)
{
    auto specOpt = AnimDB::Find(tag);
    if (!specOpt.has_value())
    {
        return;
    }

    const AnimPlaySpec& spec = specOpt.value();

    if (!anim.Play(spec.key, spec.loop, spec.fps))
    {
        return;
    }

    currentTag = tag;
}

bool PlayerAnimationController::IsPlayingTurnWin() const
{
    return state == State::TurnWin;
}