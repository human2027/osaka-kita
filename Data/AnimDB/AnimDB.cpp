#include "AnimDB.h"

namespace
{
    struct Entry
    {
        AnimTag tag;
        AnimPlaySpec spec;
    };

    const Entry TABLE[] =
    {
        // Enemy
        { {AnimGroup::Enemy, AnimAction::Idle,     AnimMood::Normal},       {"Enemy_Idle", true, 8.0f} },

        // 数字を見せる動作
        { {AnimGroup::Enemy, AnimAction::ShowHand, AnimMood::Normal},       {"Enemy_ShowHand", false, 12.0f} },
        { {AnimGroup::Enemy, AnimAction::ShowHand, AnimMood::Confident},    {"Enemy_ShowHand_Confident", false, 12.0f} },
        { {AnimGroup::Enemy, AnimAction::ShowHand, AnimMood::NoConfident},  {"Enemy_ShowHand_NoConfident", false, 12.0f} },
        { {AnimGroup::Enemy, AnimAction::ShowHand, AnimMood::Desperate},    {"Enemy_ShowHand_Desperate", false, 12.0f} },
        { {AnimGroup::Enemy, AnimAction::ShowHand, AnimMood::Bluffing},     {"Enemy_ShowHand_Bluffing", false, 12.0f} },
        //敵マップキャラ
        { {AnimGroup::Enemy, AnimAction::Map, AnimMood::Normal},     {"Enemy_Map_char", true, 5.0f}},
        // プレイヤーマップキャラ
        { {AnimGroup::Player, AnimAction::Map, AnimMood::Normal}, { "Player_Map_char", true, 5.0f } },  
        //プレイヤーWIN
        { {AnimGroup::Player, AnimAction::TurnWin, AnimMood::Normal},     {"Player_TurnWin", false, 7.0f} },
    };

    constexpr int TABLE_SIZE = static_cast<int>(sizeof(TABLE) / sizeof(TABLE[0]));
}

std::optional<AnimPlaySpec> AnimDB::Find(const AnimTag& tag)
{
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        if (TABLE[i].tag == tag)
        {
            return TABLE[i].spec;
        }
    }

    return std::nullopt;
}

std::optional<AnimPlaySpec> AnimDB::FindWithFallback(AnimGroup g, AnimAction a, AnimMood m)
{
    // 1) 指定された mood のアニメを探す
    if (auto r = Find(AnimTag{ g, a, m }); r.has_value())
    {
        return r;
    }

    // 2) 見つからなければ Normal に落とす
    if (m != AnimMood::Normal)
    {
        if (auto r = Find(AnimTag{ g, a, AnimMood::Normal }); r.has_value())
        {
            return r;
        }
    }

    return std::nullopt;
}