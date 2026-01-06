#include "RoundEndRule.h"

// ラウンド終了時の処理
void RoundEndRule::HandleEndOfRound(
    CharacterBase& player,
    CharacterBase& ai,
    bool& playerReachedGoal,
    bool& aiReachedGoal
)
{
    // ラウンド終了ダメージ（ターンごとではなくラウンド単位）
    player.SetHP(player.GetHP() - HP_decrease);
    ai.SetHP(ai.GetHP() - HP_decrease);

    // ① ゴール勝利を優先
    if (playerReachedGoal || aiReachedGoal)
    {
        // ここではフラグの整合性のみ見る
        // 実際の演出やメッセージは UI 側・TurnManager 側でやる
        return;
    }

    // ② 誰もゴールしていない場合だけ HP0 で決める
    if (player.GetHP() <= 0 && ai.GetHP() <= 0)
    {
        // 引き分け → ゴールフラグはそのまま（両 false のまま）
    }
    else if (player.GetHP() <= 0)
    {
        aiReachedGoal = true;
        playerReachedGoal = false;
    }
    else if (ai.GetHP() <= 0)
    {
        playerReachedGoal = true;
        aiReachedGoal = false;
    }
}
