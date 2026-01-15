#include "AIAnimationFrame.h"
#include <DxLib.h>
#include "AnimationSprite.h"

AIAnimationFrame::AIAnimationFrame(const AnimationSprite* bank)
    : bank(bank)
{
}

void AIAnimationFrame::Play(const std::string& key, bool loop, float fps)
{
    if (!bank || !bank->HasSequence(key)) return;

    currentKey = key;
    looping = loop;
    frameTime = (fps > 0.0f) ? (1.0f / fps) : 0.1f;
    timer = 0.0f;
    currentFrame = 0;
    finished = false;
}

void AIAnimationFrame::Update(float deltaTime)
{
    if (finished || !bank || currentKey.empty())
        return;

    timer += deltaTime;

    // 必要なだけフレームを進める
    while (timer >= frameTime)
    {
        timer -= frameTime;
        currentFrame++;

        const auto* seq = bank->GetSequence(currentKey);
        if (!seq) { finished = true; return; }

        int frameCount = static_cast<int>(seq->handles.size());

        if (currentFrame >= frameCount)
        {
            if (looping)
            {
                currentFrame = 0; // ループ
            }
            else
            {
                currentFrame = frameCount - 1;
                finished = true;
                break;
            }
        }
    }
}

void AIAnimationFrame::Draw(int x, int y) const
{
    if (!bank || currentKey.empty()) return;

    int handle = bank->GetFrame(currentKey, currentFrame);
    if (handle == -1) return;

    DrawGraph(x, y, handle, TRUE); // 左上基準で描画（必要なら中心に補正する）
}
