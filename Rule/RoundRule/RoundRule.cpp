#include "RoundRule.h"
#include "CharacterBase.h"
#include "MapManager.h"
#include "InitialValue.h" 

// ブースト処理
void RoundRule::ApplyBoosts(
    CharacterBase& player,
    CharacterBase& ai,
    int& playerMove,
    int& aiMove
)
{
    if (player.IsBoostActive())
    {
        playerMove += 2;
    }

    if (ai.IsBoostActive())
    {
        aiMove += 2;
    }
}

// 勝敗結果からラウンド統計を更新
void RoundRule::UpdateStatsOnResult(
    int playerCard,
    int aiCard,
    int result,
    int& playerWinsThisRound,
    int& aiWinsThisRound,
    int& drawsThisRound,
    bool& sameNumberOccurred,
    int& segmentDraws,
    bool& segmentSameNumber
)
{
    if (result > 0)
    {
        ++playerWinsThisRound;
    }
    else if (result < 0)
    {
        ++aiWinsThisRound;
    }
    else
    {
        ++drawsThisRound;
        ++segmentDraws;

        if (playerCard == aiCard)
        {
            sameNumberOccurred = true;
            segmentSameNumber = true;
        }
    }
}

// 勝敗結果に応じてマス移動のみ
void RoundRule::ApplyMove(
    CharacterBase& player,
    CharacterBase& ai,
    int result,
    int playerMove,
    int aiMove
)
{
    if (result > 0)
    {
        player.MovePos(playerMove);
    }
    else if (result < 0)
    {
        ai.MovePos(aiMove);
    }
    else
    {
        // 引き分け 誰も動かない
    }
}

// ゴール判定
void RoundRule::CheckGoal(
    MapManager& map,
    CharacterBase& player,
    CharacterBase& ai,
    bool& playerReachedGoal,
    bool& aiReachedGoal,
    bool& roundFinished,
    bool& roundEndedThisTurn
)
{
    if (roundFinished) return;

    if (map.IsGoal(player.GetPos()))
    {
        playerReachedGoal = true;
        roundFinished = true;
        roundEndedThisTurn = true;
    }

    if (map.IsGoal(ai.GetPos()))
    {
        aiReachedGoal = true;
        roundFinished = true;
        roundEndedThisTurn = true;
    }
}


// ターン上限と延長ロジック
void RoundRule::HandleTurnLimitAndExtension(
    int playsThisRound,
    int& currentTurnLimit,
    int maxTurnLimit,
    int segmentDraws,
    bool segmentSameNumber,
    bool& roundFinished,
    bool& roundEndedThisTurn,
    bool& isExtended
)
{
    if (roundFinished)
        return;

    if (playsThisRound < currentTurnLimit)
        return;

    bool hasAnyDraw = (segmentDraws > 0);
    bool hasSameNumDraw = segmentSameNumber;

    if (!hasAnyDraw && !hasSameNumDraw)
    {
        // この区間では引き分けなし → ここでラウンド終了
        roundFinished = true;
        roundEndedThisTurn = true;
        return;
    }

    // 引き分けあり → 延長トライ
    if (currentTurnLimit < maxTurnLimit)
    {
        ++currentTurnLimit;
        isExtended = true;
    }
    else
    {
        // すでに最大回数(=maxTurnLimit) → 強制終了
        roundFinished = true;
        roundEndedThisTurn = true;
    }
}
