#include "TurnManager.h"
#include "CharacterBase.h"
#include "MapManager.h"
#include "Judge.h"
#include "InitialValue.h"
#include "RoundRule.h"
#include "RoundEndRule.h"
#include "Blackboard.h" 
TurnManager::TurnManager(int maxTurnsPerRound)
    : maxTurnsPerRound(maxTurnsPerRound)
{
}

void TurnManager::StartNewRound(CharacterBase& player, CharacterBase& ai)
{
    player.ResetRound();
    ai.ResetRound();

    playsThisRound = 0;
    lastPlayerCard = 0;
    lastAICard = 0;
    roundFinished = false;
    playerReachedGoal = false;
    aiReachedGoal = false;

    playerWinsThisRound = 0;
    aiWinsThisRound = 0;
    drawsThisRound = 0;
    sameNumberOccurred = false;

    segmentDraws = 0;
    segmentSameNumber = false;

    isExtended = false;
    currentTurnLimit = maxTurnsPerRound;
}

void TurnManager::PushUIEvent(UIMessageType type)
{
    PushUIEvent(type, 0, 0, 0);
}

void TurnManager::PushUIEvent(UIMessageType type, int value1)
{
    PushUIEvent(type, value1, 0, 0);
}

void TurnManager::PushUIEvent(UIMessageType type, int value1, int duration)
{
    PushUIEvent(type, value1, 0, duration);
}

void TurnManager::PushUIEvent(UIMessageType type, int value1, int value2, int duration)
{
    if (!uiEventFunc) return;

    UIEvent e;
    e.type = type;
    e.value1 = value1;
    e.value2 = value2;
    e.duration = duration;

    uiEventFunc(e);
}

bool TurnManager::ResolveTurn(
    CharacterBase& player,
    CharacterBase& ai,
    int playerCard,
    int aiCard,
    MapManager& map,
    Judge& judge,
    Blackboard& bb
)
{
    if (roundFinished)
        return false;

    if (playerCard <= 0)
        return false;

    if (!player.HasCard(playerCard))
    {
        PushUIEvent(UIMessageType::InvalidCard);
        return false;
    }

    if (!ai.HasCard(aiCard))
    {
        const auto& hand = ai.GetHand();
        aiCard = hand.empty() ? 0 : static_cast<int>(hand.front());

        if (aiCard <= 0 || !ai.HasCard(aiCard))
            return false;
    }

    // ターンごとの通知フラグをクリア
    bb.ClearTurnFlags();

    bool roundEndedThisTurn = false;

    // 移動前位置
    const int prevPlayerPos = player.GetPos();
    const int prevAIPos = ai.GetPos();

    lastPlayerCard = playerCard;
    lastAICard = aiCard;

    int playerMove = playerCard;
    int aiMove = aiCard;

    // アイテム発動検知
    const bool pBoostBefore = player.IsBoostActive();
    const bool aBoostBefore = ai.IsBoostActive();
    const bool pRevBefore = player.IsReverseActive();
    const bool aRevBefore = ai.IsReverseActive();

    RoundRule::ApplyBoosts(player, ai, playerMove, aiMove);

    // AFTER差分で「使った」を通知
    if (!pBoostBefore && player.IsBoostActive()) bb.NotifyPlayerUsedBoost();
    if (!aBoostBefore && ai.IsBoostActive())     bb.NotifyAIUsedBoost();
    if (!pRevBefore && player.IsReverseActive()) bb.NotifyPlayerUsedReverse();
    if (!aRevBefore && ai.IsReverseActive())     bb.NotifyAIUsedReverse();

    if (player.IsBoostActive())
        PushUIEvent(UIMessageType::PlayerBoost);

    if (ai.IsBoostActive())
        PushUIEvent(UIMessageType::AIBoost);

    bool reverse = player.IsReverseActive() || ai.IsReverseActive();

    // 勝敗判定
    int result = judge.JudgeWinner(playerCard, aiCard, reverse);

    // BBへ「直前ターンの事実」
    bb.SetLastTurn(playerCard, aiCard, result);
    bb.SetReverseActive(reverse);

    RoundRule::UpdateStatsOnResult(
        playerCard, aiCard, result,
        playerWinsThisRound, aiWinsThisRound,
        drawsThisRound, sameNumberOccurred,
        segmentDraws, segmentSameNumber
    );

    // 移動適用
    RoundRule::ApplyMove(player, ai, result, playerMove, aiMove);

    if (result > 0)
        PushUIEvent(UIMessageType::PlayerWinMove, playerMove);
    else if (result < 0)
        PushUIEvent(UIMessageType::AIWinMove, aiMove);
    else
        PushUIEvent(UIMessageType::Draw);

    // 手札確定
    player.RemoveCard(playerCard);
    ai.RemoveCard(aiCard);

    player.AddRoundHistory(playerCard);
    ai.AddRoundHistory(aiCard);

    // 残り札をBBに反映
    bb.SetPlayerHand(player.GetHand());
    bb.SetAIHand(ai.GetHand());

    // タイル効果
    if (player.GetPos() != prevPlayerPos)
    {
        map.ApplyTileEffect(player);
    }
    if (ai.GetPos() != prevAIPos)
    {
        map.ApplyTileEffect(ai);
    }

    // HP/位置確定をBBに反映
    bb.SetPositions(player.GetPos(), ai.GetPos());
    bb.SetHP(player.GetHP(), MAX_Player_HP, ai.GetHP(), MAX_Player_HP);

    // ゴール判定
    RoundRule::CheckGoal(
        map, player, ai,
        playerReachedGoal, aiReachedGoal,
        roundFinished, roundEndedThisTurn
    );

    if (roundEndedThisTurn)
    {
        if (playerReachedGoal)
            PushUIEvent(UIMessageType::PlayerReachGoal);

        if (aiReachedGoal)
            PushUIEvent(UIMessageType::AIReachGoal);
    }

    ++playsThisRound;

    bool hadAnyDraw = (segmentDraws > 0);
    bool hadSameNum = segmentSameNumber;

    bool extendedThisTurn = false;

    RoundRule::HandleTurnLimitAndExtension(
        playsThisRound,
        currentTurnLimit,
        Max_Turn_Limit,
        segmentDraws,
        segmentSameNumber,
        roundFinished,
        roundEndedThisTurn,
        extendedThisTurn
    );

    if (extendedThisTurn)
    {
        isExtended = true;
        PushUIEvent(UIMessageType::RoundExtended, currentTurnLimit);
    }
    else if (roundEndedThisTurn && roundFinished)
    {
        if (!hadAnyDraw && !hadSameNum)
            PushUIEvent(UIMessageType::RoundEndQuick, playsThisRound);
        else if (currentTurnLimit >= Max_Turn_Limit)
            PushUIEvent(UIMessageType::RoundEndMaxTurn, Max_Turn_Limit);
    }

    if (roundEndedThisTurn)
    {
        RoundEndRule::HandleEndOfRound(
            player, ai,
            playerReachedGoal, aiReachedGoal
        );

        PushUIEvent(UIMessageType::RoundHpPenalty, HP_decrease);

        if (playerReachedGoal || aiReachedGoal)
        {
            if (playerReachedGoal && aiReachedGoal)
                PushUIEvent(UIMessageType::BothGoalDraw);
            else if (playerReachedGoal)
                PushUIEvent(UIMessageType::PlayerGoalWin);
            else
                PushUIEvent(UIMessageType::AIGoalWin);
        }
        else
        {
            if (player.GetHP() <= 0 && ai.GetHP() <= 0)
                PushUIEvent(UIMessageType::BothDeadDraw);
            else if (player.GetHP() <= 0)
                PushUIEvent(UIMessageType::PlayerDeadLose);
            else if (ai.GetHP() <= 0)
                PushUIEvent(UIMessageType::AIDeadLose);
        }
    }

    return true;
}