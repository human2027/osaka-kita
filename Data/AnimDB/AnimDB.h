#pragma once
#include <string>
#include <optional>
#include "AnimationTag.h"

// タグから再生仕様を引く（key / loop / fps）
struct AnimPlaySpec
{
    std::string key;
    bool  loop = true;
    float fps = 8.0f;
};
struct AnimDB
{
public:
    // タグに完全一致するものを返す
    static std::optional<AnimPlaySpec> Find(const AnimTag& tag);

    // よく使う：moodが無ければ Normal → None の順にフォールバックして探す
    static std::optional<AnimPlaySpec> FindWithFallback(AnimGroup g, AnimAction a, AnimMood m);
};