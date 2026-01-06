#include "AIAnimationController.h"

AIAnimationController::AIAnimationController(AnimationSprite* bank)
    : anim(bank)
{
    // 最初は敵Idleアニメ
    anim.Play("Enemy_Idle", true, 8.0f);
}

std::string AIAnimationController::MakeHandKey(int cardValue) const
{
    // カードの数値に応じてキー名を返す
    // 実際のファイル名に合わせて修正してください
    switch (cardValue)
    {
    case 1: return "Enemy_Hand1";
    case 2: return "Enemy_Hand2";
    case 3: return "Enemy_Hand3";
    case 4: return "Enemy_Hand4";
    case 5: return "Enemy_Hand5";
    default:
        return "Enemy_Hand1"; // 保険
    }
}

void AIAnimationController::OnChooseCard(int cardValue)
{
    state = State::ShowNumber;

    const std::string key = MakeHandKey(cardValue);

    // 1回だけ再生する（終わったら Idle に戻す）
    anim.Play(key, false, 12.0f);
}

void AIAnimationController::Update(float dt)
{
    anim.Update(dt);

    switch (state)
    {
    case State::Idle:
        if (anim.GetCurrentKey() != "Enemy_Idle")
        {
            anim.Play("Enemy_Idle", true, 8.0f);
        }
        break;

    case State::ShowNumber:
        if (anim.IsFinished())
        {
            state = State::Idle;
            anim.Play("Enemy_Idle", true, 8.0f);
        }
        break;
    }
}

void AIAnimationController::Draw(int x, int y)
{
    anim.Draw(x, y);
}
