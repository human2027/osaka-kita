#pragma once
#include <string>
#include "AnimationSprite.h"

class AIAnimationFrame
{
public:
    AIAnimationFrame(const AnimationSprite* bank);

    // 指定アニメを再生開始
    void Play(const std::string& key, bool loop, float fps);

    // 毎フレーム更新（deltaTime は秒）
    void Update(float deltaTime);

    // 描画
    void Draw(int x, int y) const;

    bool IsFinished() const { return finished; }
    const std::string& GetCurrentKey() const { return currentKey; }

private:
    const AnimationSprite* bank = nullptr;

    std::string currentKey;
    float frameTime = 0.1f;   // 1フレームあたりの時間(秒) = 1/fps
    float timer = 0.0f;
    int   currentFrame = 0;
    bool  looping = true;
    bool  finished = false;
};
