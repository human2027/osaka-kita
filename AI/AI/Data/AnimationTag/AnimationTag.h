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

// 3軸タグ
struct AnimTag
{
    AnimGroup  group = AnimGroup::None;
    AnimAction action = AnimAction::None;
    AnimMood   mood = AnimMood::None;
};

// キーからタグを取得
// 登録されていないキー → 全部 None の AnimTag3 を返す
AnimTag GetAnimTagByKey(const std::string& key);
