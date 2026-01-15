#include "Blackboard.h"

// コンストラクタ
Blackboard::Blackboard()
    : playerPos(0)
    , aiPos(0)
    , roundNumber(1)
    , turnInRound(0)
    , playerHasItem(false)
    , aiHasItem(false)
    , playerHP(0)
    , playerMaxHP(1)
    , aiHP(0)
    , aiMaxHP(1)
{
}

// 位置情報
void Blackboard::SetPositions(int playerPos_, int aiPos_)
{
    playerPos = playerPos_;
    aiPos = aiPos_;
}

int Blackboard::GetPlayerPos() const
{
    return playerPos;
}

int Blackboard::GetAIPos() const
{
    return aiPos;
}
// ラウンド / ターン情報
void Blackboard::SetRoundInfo(int round, int turn)
{
    roundNumber = round;
    turnInRound = turn;
}

int Blackboard::GetRoundNumber() const
{
    return roundNumber;
}

int Blackboard::GetTurnInRound() const
{
    return turnInRound;
}

// プレイヤー残り札
void Blackboard::SetPlayerHand(const std::vector<int>& hand)
{
    playerHand = hand;
}

const std::vector<int>& Blackboard::GetPlayerHand() const
{
    return playerHand;
}

// アイテム保持フラグ
void Blackboard::SetItemFlags(bool playerHas, bool aiHas)
{
    playerHasItem = playerHas;
    aiHasItem = aiHas;
}

bool Blackboard::PlayerHasItem() const
{
    return playerHasItem;
}

bool Blackboard::AIHasItem() const
{
    return aiHasItem;
}

// 5マス先のタイル情報
void Blackboard::SetPlayerForwardTiles(const std::array<TileInfoForAI, 5>& tiles)
{
    playerForward = tiles;
}

void Blackboard::SetAIForwardTiles(const std::array<TileInfoForAI, 5>& tiles)
{
    aiForward = tiles;
}

const std::array<TileInfoForAI, 5>& Blackboard::GetPlayerForwardTiles() const
{
    return playerForward;
}

const std::array<TileInfoForAI, 5>& Blackboard::GetAIForwardTiles() const
{
    return aiForward;
}

// HP情報
void Blackboard::SetHP(int playerHP_, int playerMaxHP_, int aiHP_, int aiMaxHP_)
{
    playerHP = playerHP_;
    playerMaxHP = (playerMaxHP_ <= 0) ? 1 : playerMaxHP_;

    aiHP = aiHP_;
    aiMaxHP = (aiMaxHP_ <= 0) ? 1 : aiMaxHP_;
}

int Blackboard::GetPlayerHP() const
{
    return playerHP;
}

int Blackboard::GetPlayerMaxHP() const
{
    return playerMaxHP;
}

int Blackboard::GetAIHP() const
{
    return aiHP;
}

int Blackboard::GetAIMaxHP() const
{
    return aiMaxHP;
}

bool Blackboard::IsAIDangerHP(float ratio) const
{
    if (aiMaxHP <= 0) return false;
    const float r = static_cast<float>(aiHP) / static_cast<float>(aiMaxHP);
    return r <= ratio;
}

bool Blackboard::IsAICriticalHP(float ratio) const
{
    if (aiMaxHP <= 0) return false;
    const float r = static_cast<float>(aiHP) / static_cast<float>(aiMaxHP);
    return r <= ratio;
}
