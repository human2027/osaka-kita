#pragma once
#include <vector>
#include <array>
#include "MapBase.h"

// 5マス先のマス情報を格納する軽量構造体
struct TileInfoForAI
{
    int index = -1;                      // マス番号（範囲外なら -1）
    TileType type = TileType::Normal;    // ダメージ/回復/アイテム/通常 など
    int value = 0;                       // ダメージ/回復量
    int itemId = -1;                     // アイテム種類（未使用なら -1 のまま）
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

    // プレイヤー残り札
    void SetPlayerHand(const std::vector<int>& hand);
    const std::vector<int>& GetPlayerHand() const;

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
    void SetHP(int playerHP, int playerMaxHP, int aiHP, int aiMaxHP);
    int  GetPlayerHP() const;
    int  GetPlayerMaxHP() const;
    int  GetAIHP() const;
    int  GetAIMaxHP() const;

    // 危険判定（閾値は好きに調整）
    bool IsAIDangerHP(float ratio = 0.3f) const;     // 30%以下を危険
    bool IsAICriticalHP(float ratio = 0.15f) const;  // 15%以下を致命

private:
    // 座標
    int playerPos;
    int aiPos;

    // ラウンド／ターン
    int roundNumber;
    int turnInRound;

    // プレイヤー残り札
    std::vector<int> playerHand;

    // アイテム所持フラグ
    bool playerHasItem;
    bool aiHasItem;

    // 5マス先のタイル情報
    std::array<TileInfoForAI, 5> playerForward;
    std::array<TileInfoForAI, 5> aiForward;

    // HP情報
    int playerHP;
    int playerMaxHP;
    int aiHP;
    int aiMaxHP;
};
