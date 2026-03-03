#pragma once
#include <string>

// グループ（誰のアニメか）
enum class AnimGroup
{
    None,
    Player,
    Enemy,
};

// 行動
enum class AnimAction
{
    None,
    Idle,
    WeakAtk,
    MidAtk,
    StrongAtk,
    ItemUse,
    Bluff,
    ShowHand
};

// 感情・雰囲気
enum class AnimMood
{
    None,
    Normal,
    Confident,
    Desperate,
    Bluffing,
};

// 3軸タグ（variant無し）
struct AnimTag
{
    AnimGroup  group = AnimGroup::None;
    AnimAction action = AnimAction::None;
    AnimMood   mood = AnimMood::None;
};

// 等価比較（vector探索に必要）
inline bool operator==(const AnimTag& a, const AnimTag& b) noexcept
{
    return a.group == b.group && a.action == b.action && a.mood == b.mood;
}

// キーからタグを取得（必要なら）
AnimTag GetAnimTagByKey(const std::string& key);