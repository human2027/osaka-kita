#pragma once
#include <vector>
#include <array>
#include "MapBase.h"

struct TileInfoForAI
{
    int index = -1;
    TileType type = TileType::Normal;
    int value = 0;
    int itemId = -1;
};

class Blackboard
{
public:
    Blackboard();

    // 位置情報
    void SetPositions(int playerPos, int aiPos);
    int  GetPlayerPos() const;
    int  GetAIPos()     const;

    // ラウンド／ターン情報
    void SetRoundInfo(int round, int turnInRound);
    int  GetRoundNumber() const;
    int  GetTurnInRound() const;

    // 残り札（両者）
    void SetPlayerHand(const std::vector<int>& hand);
    const std::vector<int>& GetPlayerHand() const;

    void SetAIHand(const std::vector<int>& hand);
    const std::vector<int>& GetAIHand() const;

    // アイテム保持
    void SetItemFlags(bool playerHas, bool aiHas);
    bool PlayerHasItem() const;
    bool AIHasItem()     const;

    // 5マス先のタイル情報
    void SetPlayerForwardTiles(const std::array<TileInfoForAI, 5>& tiles);
    void SetAIForwardTiles(const std::array<TileInfoForAI, 5>& tiles);
    const std::array<TileInfoForAI, 5>& GetPlayerForwardTiles() const;
    const std::array<TileInfoForAI, 5>& GetAIForwardTiles() const;

    // HP情報
    void SetHP(int playerHP, int playerMaxHP,
        int aiHP, int aiMaxHP);
    int  GetPlayerHP() const;
    int  GetPlayerMaxHP() const;
    int  GetAIHP() const;
    int  GetAIMaxHP() const;

    bool IsAIDangerHP(float ratio = 0.3f) const;
    bool IsAICriticalHP(float ratio = 0.15f) const;

    // 直前ターン情報（要件2,3,4で効く）
    void SetLastTurn(int playerCard, int aiCard, int result);
    int  GetLastPlayerCard() const;
    int  GetLastAICard() const;
    int  GetLastResult() const;

    void SetReverseActive(bool active);
    bool IsReverseActive() const;

    // ターン内通知フラグ（アイテム使用＝相手にも見える前提）
    void ClearTurnFlags(); // ExecuteTurn冒頭で呼ぶ
    void NotifyPlayerUsedBoost();
    void NotifyAIUsedBoost();
    void NotifyPlayerUsedReverse();
    void NotifyAIUsedReverse();

    bool PlayerUsedBoostThisTurn() const;
    bool AIUsedBoostThisTurn() const;
    bool PlayerUsedReverseThisTurn() const;
    bool AIUsedReverseThisTurn() const;

private:
    int playerPos = 0;
    int aiPos = 0;

    int roundNumber = 1;
    int turnInRound = 0;

    std::vector<int> playerHand;
    std::vector<int> aiHand;

    bool playerHasItem = false;
    bool aiHasItem = false;

    std::array<TileInfoForAI, 5> playerForward{};
    std::array<TileInfoForAI, 5> aiForward{};

    int playerHP = 0;
    int playerMaxHP = 1;
    int aiHP = 0;
    int aiMaxHP = 1;

    // last turn
    int lastPlayerCard = 0;
    int lastAICard = 0;
    int lastResult = 0;

    bool reverseActive = false;

    // notifications
    bool playerUsedBoostThisTurn = false;
    bool aiUsedBoostThisTurn = false;
    bool playerUsedReverseThisTurn = false;
    bool aiUsedReverseThisTurn = false;
};