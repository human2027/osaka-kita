#include "AIAnimationController.h"
#include "AnimDB.h"          
#include <DxLib.h>          
AIAnimationController::AIAnimationController(AnimationSprite* bank)
    : anim(bank)
{
    // 最初は Idle
    PlayByTag(AnimTag{ AnimGroup::Enemy, AnimAction::Idle, AnimMood::Normal });
    state = State::Idle;
}

void AIAnimationController::OnChooseCard(int cardValue, AnimMood mood)
{
    shownNumber = cardValue;
    state = State::ShowNumber;

    // 数字表示アニメ（モーションは1種類、表情差分は mood で分岐させる）
    PlayByTag(AnimTag{ AnimGroup::Enemy, AnimAction::ShowHand, mood });
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

void AIAnimationController::Draw(int x, int y)
{
    // アニメ本体
    anim.Draw(x, y);

    // 数字は「アニメとは別」に描画
    // ここは好みで：数字画像を描く/フォント描画/位置補正など
    if (state == State::ShowNumber)
    {
        // 例：簡易に文字表示（あなたのUIに合わせて置き換えてOK）
        // 数字を手元に出すなら x/y のオフセットを調整
        const int ox = 40;
        const int oy = 10;
        DrawFormatString(x + ox, y + oy, GetColor(255, 255, 255), "%d", shownNumber);
    }
}

// ---- private ----

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
#ifdef _DEBUG
        // 登録漏れが一発で分かるようにする（DxLib）
        // ※ ToStringが無いならここは消してもOK
        // printfDx("AnimDB missing tag\n");
#endif
        return;
    }

    const AnimPlaySpec& spec = specOpt.value();
    anim.Play(spec.key, spec.loop, spec.fps);

    currentTag = tag;
}