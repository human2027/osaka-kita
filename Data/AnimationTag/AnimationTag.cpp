#include <unordered_map>
#include <string>
#include "AnimationTag.h"

namespace
{
    // キー名 → タグ の対応表
    // 必要に応じてここへ追加
    const std::unordered_map<std::string, AnimTag> ANIM_TAG_TABLE =
    {
        // ===== プレイヤー =====
        {
            "Player_Idle",
            AnimTag{
                AnimGroup::Player,
                AnimAction::Idle,
                AnimMood::Normal
            }
        },
        {
            "Player_AttackWeak",
            AnimTag{
                AnimGroup::Player,
                AnimAction::WeakAtk,
                AnimMood::Normal
            }
        },
        {
            "Player_TurnWin",
            AnimTag{
                AnimGroup::Player,
                AnimAction::TurnWin,
                AnimMood::Normal
            }
        },
        {
            "Player_Map_char",
            AnimTag{
                AnimGroup::Player,
                AnimAction::Map,
                AnimMood::Normal
            }
        },
        // ===== 敵 / AI =====
        {
            "Enemy_Idle",
            AnimTag{
                AnimGroup::Enemy,
                AnimAction::Idle,
                AnimMood::Normal
            }
        },
        {
            "Enemy_TurnWin",
            AnimTag{
                AnimGroup::Enemy,
                AnimAction::TurnWin,
                AnimMood::Normal
            }
        },
        {
            "Enemy_Map_char",
            AnimTag{
                AnimGroup::Enemy,
                AnimAction::Map,
                AnimMood::Normal
            }
        },
    };
}

// キー文字列から AnimTag を返す
AnimTag GetAnimTagByKey(const std::string& key)
{
    auto it = ANIM_TAG_TABLE.find(key);

    if (it != ANIM_TAG_TABLE.end())
    {
        return it->second;
    }

    // 見つからなかった場合は None
    return AnimTag{};
}