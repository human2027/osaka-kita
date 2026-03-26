#include "AIAnimationController.h"
#include "AnimDB.h"          
#include <DxLib.h>     
#include "InitialValue.h"

AIAnimationController::AIAnimationController(AnimationSprite* bank)
    : anim(bank)
{
    // 最初は Idle
    PlayByTag(AnimTag{ AnimGroup::Enemy, AnimAction::Idle, AnimMood::Normal });
    state = State::Idle;
}

void AIAnimationController::OnChooseCard(int cardValue, AnimMood mood)
{
    const AnimTag tag{ AnimGroup::Enemy, AnimAction::ShowHand, mood };
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

    shownNumber = cardValue;
    state = State::ShowNumber;
    currentTag = tag;
}
void AIAnimationController::Update(float dt)
{
    anim.Update(dt);

    switch (state)
    {
    case State::Idle:
        EnsureIdle();
        break;

    case State::ShowNumber:
        if (anim.IsFinished())
        {
            state = State::Idle;
            EnsureIdle();
        }
        break;
    }
}

void AIAnimationController::Draw(int x, int y) const
{
    // アニメ本体
    anim.Draw(x, y);

    //数字画像(フォント描画/位置補正など
    if (state == State::ShowNumber)
    {
        DrawFormatString(x + Count_Position_x, y + Count_Position_y, GetColor(255, 255, 255), "%d", shownNumber);
    }
}

void AIAnimationController::EnsureIdle()
{
    const AnimTag idle{ AnimGroup::Enemy, AnimAction::Idle, AnimMood::Normal };
    if (!(currentTag == idle))
    {
        PlayByTag(idle);
    }
}

void AIAnimationController::PlayByTag(const AnimTag& tag)
{
    auto specOpt = AnimDB::Find(tag);
    if (!specOpt.has_value())
    {
        return;
    }

    const AnimPlaySpec& spec = specOpt.value();
    anim.Play(spec.key, spec.loop, spec.fps);

    currentTag = tag;
}
bool AIAnimationController::IsShowingCard() const
{
    return state == State::ShowNumber;
}