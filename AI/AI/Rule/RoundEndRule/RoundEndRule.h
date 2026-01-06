#pragma once

#include "CharacterBase.h"
#include "InitialValue.h"

// ラウンド終了時の処理だけを担当するルールクラス
class RoundEndRule
{
public:
    // ラウンド終了時の処理
    // ・HPペナルティの適用
    // ・HPによる勝敗決定（ゴールフラグの更新）
    static void HandleEndOfRound(
        CharacterBase& player,
        CharacterBase& ai,
        bool& playerReachedGoal,
        bool& aiReachedGoal
    );
};
