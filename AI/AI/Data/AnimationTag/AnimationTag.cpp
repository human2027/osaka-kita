#include <unordered_map>
#include <string>
#include "AnimationTag.h"

namespace
{
    // キー名 → タグ の対応表
    // 必要に応じてここへ追加
    const std::unordered_map<std::string, AnimTag> ANIM_TAG_TABLE =
    {
        // ▼ プレイヤー
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
    };
}
// キー文字列から AnimTag3 を返す
AnimTag GetAnimTagByKey(const std::string& key)
{
    auto it = ANIM_TAG_TABLE.find(key);
    if (it != ANIM_TAG_TABLE.end())
    {
        return it->second;
    }

    // 見つからなかった場合はNone
    return AnimTag{};
}
